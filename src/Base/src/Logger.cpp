//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// LibSourcey is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//


#include "Sourcey/Logger.h"
#include "Sourcey/DateTime.h"
#include "Sourcey/Platform.h"
#include "Sourcey/Filesystem.h"
#include "Sourcey/Util.h"
#include <assert.h>


using namespace std;


namespace scy {

	
static Singleton<Logger> singleton;


Logger::Logger() :
	_defaultChannel(nullptr),
	_writer(new LogWriter)
{
}


Logger::~Logger()
{
	delete _writer;
	util::clearMap(_channels);
	_defaultChannel = nullptr;
}


Logger& Logger::instance() 
{
	return *singleton.get();
}


void Logger::setInstance(Logger* logger) 
{
	// Should be done before the logger is initialized
	assert(singleton.swap(logger) == nullptr);
}

	
void Logger::shutdown()
{
	singleton.destroy();
}


void Logger::add(LogChannel* channel) 
{
	Mutex::ScopedLock lock(_mutex);
	// The first channel added will be the default channel.
	if (_defaultChannel == nullptr)
		_defaultChannel = channel;
	_channels[channel->name()] = channel;
}


void Logger::remove(const std::string& name, bool freePointer) 
{
	Mutex::ScopedLock lock(_mutex);
	LogChannelMap::iterator it = _channels.find(name);	
	assert(it != _channels.end());
	if (it != _channels.end()) {
		if (_defaultChannel == it->second)
			_defaultChannel = nullptr;
		if (freePointer)
			delete it->second;	
		_channels.erase(it);
	}
}


LogChannel* Logger::get(const std::string& name, bool whiny) const
{
	Mutex::ScopedLock lock(_mutex);
	LogChannelMap::const_iterator it = _channels.find(name);	
	if (it != _channels.end())
		return it->second;
	if (whiny)
		throw NotFoundException("No log channel named: " + name);
	return nullptr;
}


void Logger::setDefault(const std::string& name)
{
	Mutex::ScopedLock lock(_mutex);
	_defaultChannel = get(name, true);
}


LogChannel* Logger::getDefault() const
{
	Mutex::ScopedLock lock(_mutex);
	return _defaultChannel;
}


void Logger::setWriter(LogWriter* writer)
{
	Mutex::ScopedLock lock(_mutex);
	if (_writer)
		delete _writer;
	_writer = writer;
}


void Logger::write(const LogStream& stream)
{	
	// avoid if possible, requires extra copy
	write(new LogStream(stream));
}


void Logger::write(LogStream* stream)
{	
	Mutex::ScopedLock lock(_mutex);	
	if (stream->channel == nullptr)
		stream->channel = _defaultChannel;

	// Drop messages if there is no output channel
	if (stream->channel == nullptr) {
		delete stream;
		return;
	}
	_writer->write(stream);
}

	
LogStream& Logger::send(const char* level, const std::string& realm, const void* ptr, const char* channel) const
{
	return *new LogStream(getLogLevelFromString(level), realm, ptr, channel);
}


//
// Log Writer
//


LogWriter::LogWriter()
{
}


LogWriter::~LogWriter()
{
}


void LogWriter::write(LogStream* stream)
{
	stream->channel->write(*stream);	
	delete stream;
}


//
// Asynchronous Log Writer
//


AsyncLogWriter::AsyncLogWriter()
{
	_thread.start(*this);
}


AsyncLogWriter::~AsyncLogWriter()
{
	// Cancel and wait for the thread
	cancel();

	// Note: Not using join here as it is causing a deadlock
	// when unloading shared libraries when the logger is not
	// explicitly shutdown().
	//while (_thread.running())
	//	scy::sleep(10);
	_thread.join();	

	// Flush remaining items synchronously
	flush();
	
	assert(_pending.empty());
}


void AsyncLogWriter::write(LogStream* stream)
{
	Mutex::ScopedLock lock(_mutex);
	_pending.push_back(stream);
}


void AsyncLogWriter::clear()
{
	Mutex::ScopedLock lock(_mutex);
	LogStream* next = nullptr;
	while (!_pending.empty()) {
		next = _pending.front();
		delete next;
		_pending.pop_front();
	}
}


void AsyncLogWriter::flush()
{
	while (writeNext()) 
		scy::sleep(1);
}


void AsyncLogWriter::run()
{
	while (!cancelled()) 
		scy::sleep(writeNext() ? 1 : 50);
}


bool AsyncLogWriter::writeNext()
{	
	LogStream* next;
	{
		Mutex::ScopedLock lock(_mutex);
		if (_pending.empty())
			return false;

		next = _pending.front();
		_pending.pop_front();
	}		
	next->channel->write(*next);
	delete next;
	return true;
}


//
// Log Stream
//


LogStream::LogStream(LogLevel level, const std::string& realm, const void* ptr, const char* channel) : 
	level(level), realm(realm), address(ptr ? util::memAddress(ptr) : ""), ts(time(0)), channel(nullptr)
{
#ifndef DISABLE_LOGGING
	if (channel)
		this->channel = Logger::instance().get(channel, false);
#endif
}


LogStream::LogStream(LogLevel level, const std::string& realm, const std::string& address) :
	level(level), realm(realm), address(address), ts(time(0)), channel(nullptr)
{
}

	
LogStream::LogStream(const LogStream& that) :
	level(that.level), realm(that.realm), address(that.address), 
	ts(that.ts), channel(that.channel)
{
	// try to avoid copy assign
	message.str(that.message.str());
}

		
//
// Log Channel
//


LogChannel::LogChannel(const std::string& name, LogLevel level, const char* dateFormat) : 
	_name(name), 
	_level(level), 
	_dateFormat(dateFormat)
{
}


void LogChannel::write(const std::string& message, LogLevel level, const std::string& realm, const void* ptr) 
{	
	LogStream stream(level, realm, ptr);
	stream << message;
	write(stream);
}


void LogChannel::write(const LogStream& stream)
{
	(void)stream;
}


void LogChannel::format(const LogStream& stream, std::ostream& ost)
{ 
	if (_dateFormat)
		ost << scy::formatTime(stream.ts, _dateFormat); //DateTimeFormatter::format(Timestamp(stream.ts), _dateFormat);Local
	ost << " [" << getStringFromLogLevel(stream.level) << "] ";
	if (!stream.realm.empty() || !stream.address.empty()) {		
		ost << "[";		
		if (!stream.realm.empty())
			ost << stream.realm;
		if (!stream.address.empty())
			ost << ":" << stream.address;
		ost << "] ";
	}
	ost << stream.message.str();
	ost.flush();
}


//
// Console Channel
//


ConsoleChannel::ConsoleChannel(const std::string& name, LogLevel level, const char* dateFormat) : 
	LogChannel(name, level, dateFormat) 
{
}


void ConsoleChannel::write(const LogStream& stream)
{ 	
	if (this->level() > stream.level)
		return;
	
	std::ostringstream ss;
	format(stream, ss);
#if defined(_CONSOLE) || defined(_DEBUG)
	//cout << ss.str();
#endif
#if defined(_MSC_VER) && defined(_DEBUG) 
	std::string s(ss.str());
	std::wstring temp(s.length(), L' ');
	std::copy(s.begin(), s.end(), temp.begin());
	OutputDebugString(temp.c_str());
#endif
}


//
// File Channel
//


FileChannel::FileChannel(const std::string& name,
						 const std::string& path, 
						 LogLevel level, 
						 const char* dateFormat) : 
	LogChannel(name, level, dateFormat),
	_path(path)
{
}


FileChannel::~FileChannel() 
{
	close();
}


void FileChannel::open() 
{
	// Ensure a path was set
	if (_path.empty())
		throw std::runtime_error("Log file path must be set.");
	
	// Create directories if needed
	//Poco::Path dir(_path);
	//dir.setFileName("");
	//Poco::File(dir).createDirectories();
	fs::mkdirr(fs::dirname(_path));
	
	// Open the file stream
	_fstream.close();
	_fstream.open(_path.c_str(), ios::out | ios::app);	

	// Throw on failure
	if (!_fstream.is_open())
		throw std::runtime_error("Failed to open log file: " + _path);
}


void FileChannel::close() 
{ 
	_fstream.close();
}


void FileChannel::write(const LogStream& stream)
{	
	if (this->level() > stream.level)
		return;
	
	if (!_fstream.is_open())	
		open();
	
	std::ostringstream ss;
	format(stream, ss);
	_fstream << ss.str();
	_fstream.flush();

#if defined(_CONSOLE) || defined(_DEBUG)
	cout << ss.str();
#endif
#if defined(_MSC_VER) && defined(_DEBUG) 
	std::string s(ss.str());
	std::wstring temp(s.length(), L' ');
	std::copy(s.begin(), s.end(), temp.begin());
	OutputDebugString(temp.c_str());
#endif
}


void FileChannel::setPath(const std::string& path) 
{ 
	_path = path; 
	open();
}


string FileChannel::path() const 
{ 
	return _path;
}


//
// Rotating File Channel
//


RotatingFileChannel::RotatingFileChannel(const std::string& name,
	                                     const std::string& dir, 
										 LogLevel level, 
										 const std::string& extension, 
										 int rotationInterval, 
										 const char* dateFormat) : 
	LogChannel(name, level, dateFormat),
	_fstream(nullptr),
	_dir(dir),
	_extension(extension),
	_rotationInterval(rotationInterval),
	_rotatedAt(0)
{
	// The initial log file will be opened on the first call to rotate()
}
	

RotatingFileChannel::~RotatingFileChannel() 
{
	if (_fstream) {
		_fstream->close();
		delete _fstream;	
	}
}


void RotatingFileChannel::write(const LogStream& stream)
{	
	if (this->level() > stream.level)
		return;

	if (_fstream == nullptr || stream.ts - _rotatedAt > _rotationInterval)	
		rotate();
	
	std::ostringstream ss;
	format(stream, ss);
	*_fstream << ss.str();
	_fstream->flush();
	
#if defined(_CONSOLE) // && defined(_DEBUG)
	cout << ss.str();
#endif
#if defined(_MSC_VER) // && defined(_DEBUG) 
	std::string s(ss.str());
	std::wstring temp(s.length(), L' ');
	std::copy(s.begin(), s.end(), temp.begin());
	OutputDebugString(temp.c_str());
#endif
}


void RotatingFileChannel::rotate() 
{
	if (_fstream) {
		_fstream->close();
		delete _fstream;
	}

	// Always try to create the directory
	fs::mkdirr(_dir);

	// Open the next log file
	_filename = util::format("%s_%ld.%s", _name.c_str(), static_cast<long>(Timestamp().epochTime()), _extension.c_str());

	std::string path(_dir);
	fs::addnode(path, _filename);
	_fstream = new std::ofstream(path);	
	_rotatedAt = time(0);
}


} // namespace scy


/*
void Logger::write(const char* channel, const LogStream& stream)
{	
	LogChannel* ch = get(channel, true);
	QueuedWrite* work = new QueuedWrite;
	work->logger = this;
	work->stream = stream;
	work->channel = ch;
	Mutex::ScopedLock lock(_mutex);
	_pending.push_back(work);	
	// will throw if the specified channel doesn't exist
	//LogChannel* c = get(channel);
	//if (c)
	//	c->write(stream);
}


void Logger::write(const std::string& message, const char* level, 
		const std::string& realm, const void* ptr) const
{
	// will write to the nullptr channel if no default channel exists
	LogChannel* c = getDefault();
	if (c)
		c->write(message, getLogLevelFromString(level), realm, ptr);
}
*/

/*
// ---------------------------------------------------------------------
// Evented File Channel
//
EventedFileChannel::EventedFileChannel(const std::string& name,
						 const std::string& dir, 
						 LogLevel level, 
						 const std::string& extension, 
						 int rotationInterval, 
						 const char* dateFormat) : 
	FileChannel(name, dir, level, extension, rotationInterval, dateFormat)
{
}


EventedFileChannel::~EventedFileChannel() 
{
}


void EventedFileChannel::write(const LogStream& stream, LogLevel level, const std::string& realm, const void* ptr) 
{	
	if (this->level() > level)
		return;

	FileChannel::write(message, level, ptr);	
	OnLogStream.emit(this, message, level, ptr);
}
*/