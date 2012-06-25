//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is is distributed under a dual license that allows free, 
// open source use and closed source use under a standard commercial
// license.
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

#ifndef SOURCEY_Logger_H
#define SOURCEY_Logger_H


#include "Sourcey/Util.h"
#include "Sourcey/Signal.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Thread.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include <string.h>


namespace Sourcey {


enum LogLevel
{
	TraceLevel		= 0,
	DebugLevel		= 1,
	InfoLevel		= 2,
	WarningLevel	= 3,
	ErrorLevel		= 4,
	FatalLevel		= 5,
};


inline LogLevel getLogLevelFromString(const char* level)
{
    if (strcmp(level, "trace") == 0)
        return TraceLevel;
    if (strcmp(level, "debug") == 0)
        return DebugLevel;
    if (strcmp(level, "info") == 0)
        return InfoLevel;
    if (strcmp(level, "warn") == 0)
        return WarningLevel;
    if (strcmp(level, "error") == 0)
        return ErrorLevel;
    if (strcmp(level, "fatal") == 0)
        return FatalLevel;
    return DebugLevel;
}


inline const char* getStringFromLogLevel(LogLevel level) 
{
	switch(level)
	{
		case TraceLevel:		return "trace";
		case DebugLevel:		return "debug";
		case InfoLevel:			return "info";
		case WarningLevel:		return "warn";
		case ErrorLevel:		return "error";
		case FatalLevel:		return "fatal";
	}
	return "debug";
}


class LogStream;


// ---------------------------------------------------------------------
//
struct ILoggable 
{ 
	ILoggable();
	virtual ~ILoggable(); 
	virtual const char* className() const = 0;
	virtual LogStream log(const char* level = "debug") const;
	virtual void printLog(std::ostream& ost) const;
};


// ---------------------------------------------------------------------
//
class LogChannel
{
public:	
	LogChannel(const std::string& name, LogLevel level = DebugLevel, const char* dateFormat = "%H:%M:%S");
	virtual ~LogChannel() {}; 

	virtual void write(const std::string& message, LogLevel level = DebugLevel, const ILoggable* klass = NULL);
	virtual void format(std::ostream& out, const std::string& message, LogLevel level = DebugLevel, const ILoggable* klass = NULL);

	std::string	name() const { return _name; };
	LogLevel level() const { return _level; };
	const char* dateFormat() const { return _dateFormat; };
	
	void setLevel(LogLevel level) { _level = level; };
	void setDateFormat(const char* format) { _dateFormat = format; };

protected:
	std::string _name;
	LogLevel	_level;
	const char*	_dateFormat;
};


// ---------------------------------------------------------------------
//
class ConsoleChannel: public LogChannel
{		
public:
	ConsoleChannel(const std::string& name, LogLevel level = DebugLevel, const char* dateFormat = "%H:%M:%S");
	virtual ~ConsoleChannel() {}; 
		
	virtual void write(const std::string& message, LogLevel level = DebugLevel, const ILoggable* klass = NULL);
};


// ---------------------------------------------------------------------
//
class FileChannel: public LogChannel
{	
public:
	FileChannel(
		const std::string& name,
		const std::string& path,
		LogLevel level = DebugLevel, 
		const char* dateFormat = "%H:%M:%S");
	virtual ~FileChannel();

	virtual void write(const std::string& message, LogLevel level = DebugLevel, const ILoggable* klass = NULL);
	
	void setPath(const std::string& path);
	std::string	path() const;

protected:
	virtual void open();
	virtual void close();

protected:
	std::ofstream	_stream;
	std::string		_path;
};


// ---------------------------------------------------------------------
//
class RotatingFileChannel: public LogChannel
{	
public:
	RotatingFileChannel(
		const std::string& name,
		const std::string& dir,
		LogLevel level = DebugLevel, 
		const std::string& extension = "log", 
		int rotationInterval = 12 * 3600, 
		const char* dateFormat = "%H:%M:%S");
	virtual ~RotatingFileChannel();

	virtual void write(const std::string& message, LogLevel level = DebugLevel, const ILoggable* klass = NULL);
	virtual void rotate();

	std::string	dir() const { return _dir; };
	std::string	filename() const { return _filename; };
	int	rotationInterval() const { return _rotationInterval; };
	
	void setDir(const std::string& dir) { _dir = dir; };
	void setExtension(const std::string& ext) { _extension = ext; };
	void setRotationInterval(int interval) { _rotationInterval = interval; };

protected:
	std::ofstream*	_stream;
	std::string		_dir;
	std::string		_filename;
	std::string		_extension;
	time_t			_rotatedAt;			// The time the log was last rotated
	int				_rotationInterval;	// The log rotation interval in seconds
};


/*
class EventedFileChannel: public FileChannel
{	
public:
	EventedFileChannel(
		const std::string& name,
		const std::string& dir,
		LogLevel level = DebugLevel, 
		const std::string& extension = "log", 
		int rotationInterval = 12 * 3600, 
		const char* dateFormat = "%H:%M:%S");
	virtual ~EventedFileChannel();
	
	virtual void write(const std::string& message, LogLevel level = DebugLevel, const ILoggable* klass = NULL);

	Signal3<const std::string&, LogLevel&, const ILoggable*&> OnLogMessage;
};
*/


typedef std::map<std::string, LogChannel*> LogMap;


// ---------------------------------------------------------------------
//
class Logger 
{
public:
	static Logger& instance();
	static void initialize();	
	static void uninitialize();
	
	static void setInstance(Logger* logger);
		// Sets the logger instance and deletes the current instance.

	void add(LogChannel* channel);
	void remove(const std::string& name, bool deletePointer = true);
	LogChannel& get(const std::string& name) const;
		// Returns the specified log channel. 
		// Throws an exception if the channel doesn't exist.
	
	void setDefault(const std::string& name);
		// Sets the default log to the specified log channel.

	LogChannel& getDefault() const;
		// Returns the default log channel, or the null channel
		// if no default channel has been set.

	void write(const std::string& name, const std::string& message, LogLevel level = DebugLevel, const ILoggable* klass = NULL);
		// Writes the given message to the specified log channel.

	void write(const std::string& message, LogLevel level = DebugLevel, const ILoggable* klass = NULL);
		// Writes the given message to the default log channel.

	LogStream send(const std::string& name, const char* level = "debug") const;
		// Creates a LogStream which will write to the specified log channel.

	LogStream send(const char* level = "debug") const;
		// Creates a LogStream which will write to the default log channel.

	LogStream send(const ILoggable* klass, const std::string& name, const char* level = "debug") const;
		// Sends to the specified log channel using the loggable class instance.

	LogStream send(const ILoggable* klass, const char* level = "debug") const;
		// Sends to the default log using the loggable class instance.

protected:
	Logger();								// Private so that it can not be called
	Logger(Logger const&) {};				// Copy constructor is protected
	Logger& operator=(Logger const&) {};	// Assignment operator is protected
	~Logger();
	
	LogChannel*				_nullChannel;
	LogChannel*				_defaultChannel;
	LogMap					_map;

	static Logger*			_instance;
	//static LogMap			_map;
	static Poco::FastMutex	_mutex;
};



class LogStream 
{
public:
	LogStream(const std::string& name = "", LogLevel level = DebugLevel, const ILoggable* klass = NULL) : 
		_channel(NULL), 
		_name(name),   
		_level(level),   
		_klass(klass) {	}

	LogStream(LogChannel* channel, LogLevel level = DebugLevel, const ILoggable* klass = NULL) : 
		_channel(channel), 
		_level(level),   
		_klass(klass) {	}

	LogStream(const LogStream& r) : 
		_channel(r._channel), 
		_name(r._name),  
		_level(r._level),   
		_klass(r._klass) { 
		*this << r._os.rdbuf();
	}

	template<typename T>
	LogStream& operator << (const T& data) {
#ifndef DISABLE_LOGGING
		_os << data;
#endif
		return *this;
	}

	LogStream& operator << (std::ostream&(*f)(std::ostream&)) {
#ifndef DISABLE_LOGGING
		_os << f;

		// write direct to channel inst
		if (_channel)
			_channel->write(_os.str(), _level, _klass);

		// send to default channel
		else if (_name.empty())
			Logger::instance().write(_os.str(), _level, _klass);

		// send to named channel
		else
			Logger::instance().write(_name, _os.str(), _level, _klass);
#endif
		return *this;
	}

protected:
	LogLevel			_level;
	std::string			_name;
	LogChannel*			_channel;
	const ILoggable*	_klass;
	std::ostringstream	_os;
};


// Macros for log stream access
inline LogStream Log(const char* level) 
	{ return LogStream("", getLogLevelFromString(level)); };
inline LogStream Log(const char* level, const std::string& name) 
	{ return LogStream(name, getLogLevelFromString(level)); };
inline LogStream Log(const char* level, const ILoggable* klass) 
	{ return LogStream("", getLogLevelFromString(level), klass); };
inline LogStream Log(const char* level, const std::string& name, const ILoggable* klass) 
	{ return LogStream(name, getLogLevelFromString(level), klass); };


} // namespace Sourcey


#endif
