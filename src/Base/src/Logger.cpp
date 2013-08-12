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
#include "Sourcey/Util.h"
#include <assert.h>


using namespace std;


namespace scy {

	
static Singleton<Logger> singleton;


Logger::Logger() :
	_defaultChannel(nil),
	_writer(new LogWriter)
{
}


Logger::~Logger()
{
	delete _writer;
	util::clearMap(_channels);
	_defaultChannel = nil;
}


Logger& Logger::instance() 
{
	return *singleton.get();
}


void Logger::setInstance(Logger* logger) 
{
	// Should be done before the logger is initialized
	assert(singleton.swap(logger) == nil);
}

	
void Logger::shutdown()
{
	singleton.destroy();
}


void Logger::add(LogChannel* channel) 
{
	Mutex::ScopedLock lock(_mutex);
	// The first channel added will be the default channel.
	if (_defaultChannel == nil)
		_defaultChannel = channel;
	_channels[channel->name()] = channel;
}


void Logger::remove(const string& name, bool freePointer) 
{
	Mutex::ScopedLock lock(_mutex);
	LogChannelMap::iterator it = _channels.find(name);	
	assert(it != _channels.end());
	if (it != _channels.end()) {
		if (_defaultChannel == it->second)
			_defaultChannel = nil;
		if (freePointer)
			delete it->second;	
		_channels.erase(it);
	}
}


LogChannel* Logger::get(const string& name, bool whiny) const
{
	Mutex::ScopedLock lock(_mutex);
	LogChannelMap::const_iterator it = _channels.find(name);	
	if (it != _channels.end())
		return it->second;
	if (whiny)
		throw NotFoundException("No log channel named: " + name);
	return nil;
}


void Logger::setDefault(const string& name)
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
	assert(0); // can remove, but shouldn't be using this method in internal code
	write(new LogStream(stream));
}


void Logger::write(LogStream* stream)
{	
	Mutex::ScopedLock lock(_mutex);	
	if (stream->channel == nil)
		stream->channel = _defaultChannel;

	// Drop messages if there is no output channel
	if (stream->channel == nil) {
		delete stream;
		return;
	}
	_writer->write(stream);
}

	
LogStream& Logger::send(const char* level, const string& realm, const void* ptr, const char* channel) const
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
	LogStream* next = nil;
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


LogStream::LogStream(LogLevel level, const string& realm, const void* ptr, const char* channel) : 
	level(level), realm(realm), address(ptr ? util::memAddress(ptr) : ""), channel(nil)
{
#ifndef DISABLE_LOGGING
	if (channel)
		this->channel = Logger::instance().get(channel, false);
#endif
}


LogStream::LogStream(LogLevel level, const string& realm, const string& address) :
	level(level), realm(realm), address(address), channel(nil)
{
}

	
LogStream::LogStream(const LogStream& that) :
	level(that.level), realm(that.realm), address(that.address), channel(nil)
{
	// try to avoid copy assign
	message.str(that.message.str());
}

		
//
// Log Channel
//


LogChannel::LogChannel(const string& name, LogLevel level, const char* dateFormat) : 
	_name(name), 
	_level(level), 
	_dateFormat(dateFormat)
{
}


void LogChannel::write(const string& message, LogLevel level, const string& realm, const void* ptr) 
{	
	LogStream stream(level, realm, ptr);
	stream << message;
	write(stream);
}


void LogChannel::write(const LogStream& stream)
{
	(void)stream;
}


void LogChannel::format(const LogStream& stream, ostream& ost)
{ 
	if (_dateFormat)
		ost << DateTimeFormatter::format(Timestamp(), _dateFormat);
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


ConsoleChannel::ConsoleChannel(const string& name, LogLevel level, const char* dateFormat) : 
	LogChannel(name, level, dateFormat) 
{
}


void ConsoleChannel::write(const LogStream& stream)
{ 	
	if (this->level() > stream.level)
		return;
	
	ostringstream ss;
	format(stream, ss);
#if defined(_CONSOLE) || defined(_DEBUG)
	//cout << ss.str();
#endif
#if defined(_MSC_VER) && defined(_DEBUG) 
	string s(ss.str());
	wstring temp(s.length(), L' ');
	copy(s.begin(), s.end(), temp.begin());
	OutputDebugString(temp.c_str());
#endif
}


//
// File Channel
//


FileChannel::FileChannel(const string& name,
						 const string& path, 
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
		throw Exception("Log file path must be set.");
	
	// Create directories if needed
	//Poco::Path dir(_path);
	//dir.setFileName("");
	//Poco::File(dir).createDirectories();
	
	// Open the file stream
	_fstream.close();
	_fstream.open(_path.c_str(), ios::out | ios::app);	

	// Throw on failure
	if (!_fstream.is_open())
		throw Exception("Failed to open log file: " + _path);
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
	
	ostringstream ss;
	format(stream, ss);
	_fstream << ss.str();
	_fstream.flush();

#if defined(_CONSOLE) || defined(_DEBUG)
	cout << ss.str();
#endif
#if defined(_MSC_VER) && defined(_DEBUG) 
	string s(ss.str());
	wstring temp(s.length(), L' ');
	copy(s.begin(), s.end(), temp.begin());
	OutputDebugString(temp.c_str());
#endif
}


void FileChannel::setPath(const string& path) 
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


RotatingFileChannel::RotatingFileChannel(const string& name,
	                                     const string& dir, 
										 LogLevel level, 
										 const string& extension, 
										 int rotationInterval, 
										 const char* dateFormat) : 
	LogChannel(name, level, dateFormat),
	_fstream(nil),
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

	if (_fstream == nil || time(0) - _rotatedAt > _rotationInterval)	
		rotate();
	
	ostringstream ss;
	format(stream, ss);
	*_fstream << ss.str();
	_fstream->flush();
	
#if defined(_CONSOLE) // && defined(_DEBUG)
	cout << ss.str();
#endif
#if defined(_MSC_VER) // && defined(_DEBUG) 
	string s(ss.str());
	wstring temp(s.length(), L' ');
	copy(s.begin(), s.end(), temp.begin());
	OutputDebugString(temp.c_str());
#endif
}


void RotatingFileChannel::rotate() 
{
	if (_fstream) {
		_fstream->close();
		delete _fstream;
	}

	// Open the next log file
	_filename = util::format("%s_%ld.%s", _name.c_str(), static_cast<long>(Timestamp().epochTime()), _extension.c_str());
	//Poco::Path path(_dir);	
	//Poco::File(path).createDirectories();
	//path.setFileName(_filename);
	std::string path(_dir + _filename);
	_fstream = new ofstream(path.c_str());	
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


void Logger::write(const string& message, const char* level, 
		const string& realm, const void* ptr) const
{
	// will write to the nil channel if no default channel exists
	LogChannel* c = getDefault();
	if (c)
		c->write(message, getLogLevelFromString(level), realm, ptr);
}
*/

/*
// ---------------------------------------------------------------------
// Evented File Channel
//
EventedFileChannel::EventedFileChannel(const string& name,
						 const string& dir, 
						 LogLevel level, 
						 const string& extension, 
						 int rotationInterval, 
						 const char* dateFormat) : 
	FileChannel(name, dir, level, extension, rotationInterval, dateFormat)
{
}


EventedFileChannel::~EventedFileChannel() 
{
}


void EventedFileChannel::write(const LogStream& stream, LogLevel level, const string& realm, const void* ptr) 
{	
	if (this->level() > level)
		return;

	FileChannel::write(message, level, ptr);	
	OnLogStream.emit(this, message, level, ptr);
}
*/