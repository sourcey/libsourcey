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

#ifndef SCY_Logger_H
#define SCY_Logger_H


#include "scy/base.h"
#include "scy/mutex.h"
#include "scy/thread.h"
#include "scy/exception.h"
#include "scy/singleton.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <deque>
#include <map>
#include <ctime>
//#include <time.h>
#include <string.h>


namespace scy {


enum LogLevel
{
	LTrace	= 0,
	LDebug	= 1,
	LInfo	= 2,
	LWarn	= 3,
	LError	= 4,
	LFatal	= 5,
};


inline LogLevel getLogLevelFromString(const char* level)
{
    if (strcmp(level, "trace") == 0)
        return LTrace;
    if (strcmp(level, "debug") == 0)
        return LDebug;
    if (strcmp(level, "info") == 0)
        return LInfo;
    if (strcmp(level, "warn") == 0)
        return LWarn;
    if (strcmp(level, "error") == 0)
        return LError;
    if (strcmp(level, "fatal") == 0)
        return LFatal;
    return LDebug;
}


inline const char* getStringFromLogLevel(LogLevel level) 
{
	switch(level)
	{
		case LTrace:	return "trace";
		case LDebug:	return "debug";
		case LInfo:		return "info";
		case LWarn:		return "warn";
		case LError:	return "error";
		case LFatal:	return "fatal";
	}
	return "debug";
}


struct LogStream;
class LogChannel;


//
// Default Log Writer
//


class LogWriter
{
public:	
	LogWriter();
	virtual ~LogWriter();

	virtual void write(LogStream* stream);
		// Writes the given log message stream.
};


//
// Asynchronous Log Writer
//


class AsyncLogWriter: public LogWriter, public async::Runnable
{
public:	
	AsyncLogWriter();
	virtual ~AsyncLogWriter();

	virtual void write(LogStream* stream);
		// Queues the given log message stream.
	
	void flush();
		// Flushes queued messages.

	void run();
		// Writes queued messages asynchronously.

	void clear();
		// Clears all queued messages.
	
protected:	
	bool writeNext();

	Thread _thread;
	std::deque<LogStream*> _pending;
	mutable Mutex _mutex;
};


//
// Logger
//


class Logger
{
public:
	Logger();
	~Logger();

	static Logger& instance();
		// Returns the default logger singleton.
		// Logger instances may be created separately as needed.

	static void shutdown();
		// Shuts down the logger and deletes the singleton instance.

	static void setInstance(Logger* logger);
		// Sets the default logger singleton instance.

	void add(LogChannel* channel);
		// Adds the given log channel.

	void remove(const std::string& name, bool freePointer = true);
		// Removes the given log channel by name, 
		// and optionally frees the pointer.

	LogChannel* get(const std::string& name, bool whiny = true) const;
		// Returns the specified log channel. 
		// Throws an exception if the channel doesn't exist.
	
	void setDefault(const std::string& name);
		// Sets the default log to the specified log channel.

	void setWriter(LogWriter* writer);
		// Sets the log writer instance.

	LogChannel* getDefault() const;
		// Returns the default log channel, or the nullptr channel
		// if no default channel has been set.
	
	void write(const LogStream& stream);
		// Writes the given message to the default log channel.
		// The message will be copied.
	
	void write(LogStream* stream);
		// Writes the given message to the default log channel.
		// The stream pointer will be deleted when appropriate.
	
	LogStream& send(const char* level = "debug", const std::string& realm = "", 
		const void* ptr = nullptr, const char* channel = nullptr) const;
		// Sends to the default log using the given class instance.
		// Recommend using write(LogStream&) to avoid copying data.

protected:
	// Non-copyable and non-movable
	Logger(const Logger&); // = delete;
	Logger(Logger&&); // = delete;
	Logger& operator=(const Logger&); // = delete;
	Logger& operator=(Logger&&); // = delete;

	typedef std::map<std::string, LogChannel*> LogChannelMap;
	
	friend class Singleton<Logger>;
	friend class Thread;
		
	mutable Mutex _mutex;
	LogChannelMap _channels;
	LogChannel*   _defaultChannel;
	LogWriter*    _writer;
};


//
// Log Stream
//


struct LogStream
{
	LogLevel level;
	std::string realm;              // depreciate - encode in message
	std::string address;            // depreciate - encode in message
	std::ostringstream message;
	std::time_t ts;
	LogChannel* channel;

	LogStream(LogLevel level = LDebug, const std::string& realm = "", const void* ptr = nullptr, const char* channel = nullptr);
	LogStream(LogLevel level, const std::string& realm = "", const std::string& address = "");
	LogStream(const LogStream& that); 

	template<typename T>
	LogStream& operator << (const T& data) {
#ifndef DISABLE_LOGGING
		message << data;
#endif
		return *this;
	}

	LogStream& operator << (const LogLevel data) {
#ifndef DISABLE_LOGGING
		level = data;
#endif
		return *this;
	}

	LogStream& operator << (std::ostream&(*f)(std::ostream&)) {
#ifndef DISABLE_LOGGING		
		message << f;

		// Send to default channel
		// Channel flag or stream operation
		Logger::instance().write(this);
#endif
		return *this;
	}
};


//
// Inline stream accessors
//


// Default output
inline LogStream& traceL(const char* realm = "", const void* ptr = nullptr) 
	{ return *new LogStream(LTrace, realm, ptr); }

inline LogStream& debugL(const char* realm = "", const void* ptr = nullptr) 
	{ return *new LogStream(LDebug, realm, ptr); }

inline LogStream& infoL(const char* realm = "", const void* ptr = nullptr) 
	{ return *new LogStream(LInfo, realm, ptr); }

inline LogStream& warnL(const char* realm = "", const void* ptr = nullptr) 
	{ return *new LogStream(LWarn, realm, ptr); }

inline LogStream& errorL(const char* realm = "", const void* ptr = nullptr) 
	{ return *new LogStream(LError, realm, ptr); }

inline LogStream& fatalL(const char* realm = "", const void* ptr = nullptr) 
	{ return *new LogStream(LFatal, realm, ptr); }


// Channel output
inline LogStream& traceC(const char* channel, const char* realm = "", const void* ptr = nullptr) 
	{ return *new LogStream(LTrace, realm, ptr, channel); }

inline LogStream& debugC(const char* channel, const char* realm = "", const void* ptr = nullptr) 
	{ return *new LogStream(LDebug, realm, ptr, channel); }

inline LogStream& infoC(const char* channel, const char* realm = "", const void* ptr = nullptr) 
	{ return *new LogStream(LInfo, realm, ptr, channel); }

inline LogStream& warnC(const char* channel, const char* realm = "", const void* ptr = nullptr) 
	{ return *new LogStream(LWarn, realm, ptr, channel); }

inline LogStream& errorC(const char* channel, const char* realm = "", const void* ptr = nullptr) 
	{ return *new LogStream(LError, realm, ptr, channel); }

inline LogStream& fatalC(const char* channel, const char* realm = "", const void* ptr = nullptr) 
	{ return *new LogStream(LFatal, realm, ptr, channel); }


// Level output
inline LogStream& printL(const char* level = "debug", const char* realm = "", const void* ptr = nullptr, const char* channel = nullptr) 
	{ return *new LogStream(getLogLevelFromString(level), realm, ptr, channel); }

inline LogStream& printL(const char* level, const void* ptr, const char* realm = "", const char* channel = nullptr) 
	{ return *new LogStream(getLogLevelFromString(level), realm, ptr, channel); }


//
// Log Channel
//


class LogChannel
{
public:	
	LogChannel(const std::string& name, LogLevel level = LDebug, const char* dateFormat = "%H:%M:%S");
	virtual ~LogChannel() {}; 
	
	virtual void write(const LogStream& stream);
	virtual void write(const std::string& message, LogLevel level = LDebug, 
		const std::string& realm = "", const void* ptr = nullptr);
	virtual void format(const LogStream& stream, std::ostream& ost);

	std::string	name() const { return _name; };
	LogLevel level() const { return _level; };
	const char* dateFormat() const { return _dateFormat; };
	
	void setLevel(LogLevel level) { _level = level; };
	void setDateFormat(const char* format) { _dateFormat = format; };

protected:
	std::string _name;
	LogLevel    _level;
	const char* _dateFormat;
};


//
// Console Channel
//


class ConsoleChannel: public LogChannel
{		
public:
	ConsoleChannel(const std::string& name, LogLevel level = LDebug, const char* dateFormat = "%H:%M:%S");
	virtual ~ConsoleChannel() {}; 
		
	virtual void write(const LogStream& stream);
};


//
// File Channel
//


class FileChannel: public LogChannel
{	
public:
	FileChannel(
		const std::string& name,
		const std::string& path,
		LogLevel level = LDebug, 
		const char* dateFormat = "%H:%M:%S");
	virtual ~FileChannel();
	
	virtual void write(const LogStream& stream);
	
	void setPath(const std::string& path);
	std::string	path() const;

protected:
	virtual void open();
	virtual void close();

protected:
	std::ofstream	_fstream;
	std::string		_path;
};


//
// Rotating File Channel
//


class RotatingFileChannel: public LogChannel
{	
public:
	RotatingFileChannel(
		const std::string& name,
		const std::string& dir,
		LogLevel level = LDebug, 
		const std::string& extension = "log", 
		int rotationInterval = 12 * 3600, 
		const char* dateFormat = "%H:%M:%S");
	virtual ~RotatingFileChannel();
	
	virtual void write(const LogStream& stream);
	virtual void rotate();

	std::string dir() const { return _dir; };
	std::string filename() const { return _filename; };
	int rotationInterval() const { return _rotationInterval; };
	
	void setDir(const std::string& dir) { _dir = dir; };
	void setExtension(const std::string& ext) { _extension = ext; };
	void setRotationInterval(int interval) { _rotationInterval = interval; };

protected:
	std::ofstream* _fstream;
	std::string    _dir;
	std::string    _filename;
	std::string    _extension;
	int            _rotationInterval;    // The log rotation interval in seconds
	time_t         _rotatedAt;           // The time the log was last rotated
};


#if 0
class EventedFileChannel: public FileChannel
{	
public:
	EventedFileChannel(
		const std::string& name,
		const std::string& dir,
		LogLevel level = LDebug, 
		const std::string& extension = "log", 
		int rotationInterval = 12 * 3600, 
		const char* dateFormat = "%H:%M:%S");
	virtual ~EventedFileChannel();
	
	virtual void write(const std::string& message, LogLevel level = LDebug, 
		const std::string& realm = "", const void* ptr = nullptr);
	virtual void write(const LogStream& stream);

	Signal3<const std::string&, LogLevel&, const Polymorphic*&> OnLogStream;
};
#endif


} // namespace scy


#endif




	//void write(const char* channel, const LogStream& stream);
		// Writes the given message to the given log channel.

	//void write(const std::string& message, const char* level = "debug", 
		//const std::string& realm = "", const void* ptr = nullptr) const;
		// Writes the given message to the default log channel.
