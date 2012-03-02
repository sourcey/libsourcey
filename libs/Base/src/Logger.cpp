//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2005 Sourcey
//
// Non-Commercial Use:
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// Commercial Use:
// Please contact mail@sourcey.com
//


#include "Sourcey/Logger.h"
#include "Poco/Format.h"
#include "Poco/Path.h"
#include <assert.h>


using namespace Poco;
using namespace std;


namespace Sourcey {


// ---------------------------------------------------------------------
//
// ILoggable
//
// ---------------------------------------------------------------------
ILoggable::ILoggable()
{
}


ILoggable::~ILoggable()
{
}


LogStream ILoggable::log(const char* level) const 
{ 
	return Log(level, this);
}


void ILoggable::printLog(std::ostream& ost) const
{
	ost << "["
		<< className()
		<< ":"
		<< this // override to reflect derived pid
		<< "] ";
}


// ---------------------------------------------------------------------
//
// File Channel
//
// ---------------------------------------------------------------------
FileChannel::FileChannel(const string& name,
						 const string& dir, 
						 LogLevel level, 
						 const string& extension, 
						 const char* dateFormat, 
						 int rotationInterval) : 
	LogChannel(name, level),
	_stream(NULL),
	_dir(dir),
	_extension(extension),
	_dateFormat(dateFormat),
	_rotationInterval(rotationInterval),
	_rotatedAt(0)
{
	// The initial log file will be opened on the first call to setup()
}
	

FileChannel::~FileChannel() 
{
	if (_stream) {
		_stream->close();
		delete _stream;	
	}
}


void FileChannel::write(const string& message, LogLevel level, const ILoggable* klass) 
{	
	if (this->level() > level)
		return;

	if (_stream == NULL || time(0) - _rotatedAt > _rotationInterval)	
		rotate();
	
	ostringstream ss;
	writeLogFormat(ss, message, level, klass);
	*_stream << ss.str();
	_stream->flush();
#if defined(_DEBUG) //&& defined(_CONSOLE)
	cout << ss.str();
#endif
#if defined(_DEBUG) && defined(_MSC_VER)
	std::string s(ss.str());
	std::wstring temp(s.length(), L' ');
	std::copy(s.begin(), s.end(), temp.begin());
	OutputDebugString(temp.data());
#endif
	
	/*
	// Output the prefix and message to the log file
	*_stream << "[" << getStringFromLogLevel(level) << "] ";
	*_stream << DateTimeFormatter::format(Timestamp(), _dateFormat);
	if (klass)
		klass->printLog(*_stream);
	*_stream << message;
	_stream->flush();
	writeLogFormat(*_stream, message, level, klass, _dateFormat);
	*/
	
	/*
	LogMessage msg(message, getStringFromLogLevel(level), _dateFormat);
	msg.dump(*_stream);
	//OnLogMessage.dispatch(this, msg);
	*/
}

	
void FileChannel::rotate() 
{
	if (_stream) {
		_stream->close();
		delete _stream;	
		_stream = NULL;	
	}

	// Initialize and open the next log file
	_filename = format("%s_%ld.%s", _name, static_cast<long>(Timestamp().epochTime()), _extension);
	Path path(_dir);
	path.setFileName(_filename);
	_stream = new ofstream(path.toString().data());	
	_rotatedAt = time(0);
}


// ---------------------------------------------------------------------
//
// Console Channel
//
// ---------------------------------------------------------------------
ConsoleChannel::ConsoleChannel(const string& name, LogLevel level) : 
	LogChannel(name, level) 
{
}


void ConsoleChannel::write(const string& message, LogLevel level, const ILoggable* klass) 
{ 
#if defined(_DEBUG) || defined(_CONSOLE)
	
	if (this->level() > level)
		return;
	
	ostringstream ss;
	writeLogFormat(ss, message, level, klass);
	cout << ss.str();
#if defined(_MSC_VER) && defined(_DEBUG)
	std::string s(ss.str());
	std::wstring temp(s.length(), L' ');
	std::copy(s.begin(), s.end(), temp.begin());
	OutputDebugString(temp.data());
#endif
	/*
		ostringstream ss;
		ss << "[" << getStringFromLogLevel(level) << "] ";
		if (klass)
			klass->printLog(ss);
		ss << message;
//#if defined(_CONSOLE)
		cout << ss.str();
//#endif
	*/
#endif
};
//OutputDebugString 


// ---------------------------------------------------------------------
//
// Logger
//
// ---------------------------------------------------------------------
Logger*			Logger::_instance;
FastMutex		Logger::_mutex;


Logger& Logger::instance() 
{
	if (_instance == NULL) 
	{
		FastMutex::ScopedLock lock(_mutex);
		if (_instance == NULL)
			_instance = new Logger;
	}
	return *_instance;
} 


void Logger::initialize() 
{
	instance();
}


void Logger::uninitialize() 
{
	if (_instance) {
		delete _instance;
		_instance = NULL;
	}
}


Logger::Logger() :
	_nullChannel(new LogChannel("null")),
	_defaultChannel(NULL)
{
}


Logger::~Logger()
{	
	Util::ClearMap(_map);
	delete _nullChannel;
}


void Logger::add(LogChannel* channel) 
{
	FastMutex::ScopedLock lock(_mutex);
	// The first channel to be added will be our default channel.
	if (_defaultChannel == NULL)
		_defaultChannel = channel;
	_map[channel->name()] = channel;
}


void Logger::remove(const string& name, bool deletePointer) 
{
	FastMutex::ScopedLock lock(_mutex);
	LogMap::iterator it = _map.find(name);	
	assert(it != _map.end());
	if (it != _map.end()) {
		if (_defaultChannel == it->second)
			_defaultChannel = NULL;
		if (deletePointer)
			delete it->second;	
		_map.erase(it);
	}
}


LogChannel& Logger::get(const string& name) const
{
	FastMutex::ScopedLock lock(_mutex);
	LogMap::const_iterator it = _map.find(name);	
	if (it == _map.end())
		throw Poco::NotFoundException("No log channel named: " + name);
	return *it->second;	 	
}


void Logger::setDefault(const string& name)
{
	_defaultChannel = &get(name);
}


LogChannel& Logger::getDefault() const
{
	FastMutex::ScopedLock lock(_mutex);	
	return *(_defaultChannel != NULL ? _defaultChannel : _nullChannel);
}


LogStream Logger::send(const string& name, const char* level) const
{	
	// will throw if the specified channel doesn't exist
	return LogStream(&get(name), getLogLevelFromString(level));
}


LogStream Logger::send(const char* level) const
{	
	// will write to the null channel if no default channel exists
	return LogStream(&getDefault(), getLogLevelFromString(level));
}


LogStream Logger::send(const ILoggable* klass, const string& name, const char* level) const
{	
	// will throw if the specified channel doesn't exist
	return LogStream(&get(name), getLogLevelFromString(level), klass);
}


LogStream Logger::send(const ILoggable* klass, const char* level) const
{	
	// will write to the null channel if no default channel exists
	return LogStream(&getDefault(), getLogLevelFromString(level), klass);
}


void Logger::write(const string& name, const string& message, LogLevel level, const ILoggable* klass)
{	
	// will throw if the specified channel doesn't exist
	get(name).write(message, level, klass);
}


void Logger::write(const string& message, LogLevel level, const ILoggable* klass)
{	
	// will write to the null channel if no default channel exists
	getDefault().write(message, level, klass);
}


} // namespace Sourcey
