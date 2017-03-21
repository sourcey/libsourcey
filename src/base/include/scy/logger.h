///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_Logger_H
#define SCY_Logger_H


#include "scy/base.h"
#include "scy/error.h"
#include "scy/interface.h"
#include "scy/singleton.h"
#include "scy/thread.h"

#include <ctime>
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <mutex>
#include <string.h>


namespace scy {


enum LogLevel
{
    LTrace = 0,
    LDebug = 1,
    LInfo = 2,
    LWarn = 3,
    LError = 4,
    LFatal = 5,
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
    switch (level) {
        case LTrace:
            return "trace";
        case LDebug:
            return "debug";
        case LInfo:
            return "info";
        case LWarn:
            return "warn";
        case LError:
            return "error";
        case LFatal:
            return "fatal";
    }
    return "debug";
}


struct Base_API LogStream;
class Base_API LogChannel;


//
// Default Log Writer
//


/// Log output stream writer.
class Base_API LogWriter
{
public:
    LogWriter();
    virtual ~LogWriter();

    /// Writes the given log message stream.
    virtual void write(LogStream* stream);
};


//
// Asynchronous Log Writer
//


/// Thread based log output stream writer.
class Base_API AsyncLogWriter : public LogWriter, public basic::Runnable
{
public:
    AsyncLogWriter();
    virtual ~AsyncLogWriter();

    /// Queues the given log message stream.
    virtual void write(LogStream* stream);

    /// Flushes queued messages.
    void flush();

    /// Writes queued messages asynchronously.
    void run();

    /// Clears all queued messages.
    void clear();

protected:
    bool writeNext();

    Thread _thread;
    std::deque<LogStream*> _pending;
    mutable std::mutex _mutex;
};


//
// Logger
//


/// Logger class.
class Base_API Logger
{
public:
    Logger();
    ~Logger();

    /// Returns the default logger singleton.
    /// Logger instances may be created separately as needed.
    static Logger& instance();

    /// Sets the default logger singleton instance.
    static void setInstance(Logger* logger, bool freeExisting = true);

    /// Destroys the default logger singleton instance.
    static void destroy();

    /// Adds the given log channel.
    void add(LogChannel* channel);

    /// Removes the given log channel by name,
    /// and optionally frees the pointer.
    void remove(const std::string& name, bool freePointer = true);

    /// Returns the specified log channel.
    /// Throws an exception if the channel doesn't exist.
    LogChannel* get(const std::string& name, bool whiny = true) const;

    /// Sets the default log to the specified log channel.
    void setDefault(const std::string& name);

    /// Sets the log writer instance.
    void setWriter(LogWriter* writer);

    /// Returns the default log channel, or the nullptr channel
    /// if no default channel has been set.
    LogChannel* getDefault() const;

    /// Writes the given message to the default log channel.
    /// The message will be copied.
    void write(const LogStream& stream);

    /// Writes the given message to the default log channel.
    /// The stream pointer will be deleted when appropriate.
    void write(LogStream* stream);

    /// Sends to the default log using the given class instance.
    /// Recommend using write(LogStream&) to avoid copying data.
    LogStream& send(const char* level = "debug", const char* realm = "",
                    const void* ptr = nullptr,
                    const char* channel = nullptr) const;

protected:
    /// NonCopyable and NonMovable
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    typedef std::map<std::string, LogChannel*> LogChannelMap;

    friend class Singleton<Logger>;
    friend class Thread;

    mutable std::mutex _mutex;
    LogChannelMap _channels;
    LogChannel* _defaultChannel;
    LogWriter* _writer;
};


//
// Recursive logging for veradic arguments
//


template <typename T>
void logArgs(std::ostream& o, T t)
{
    o << t << ' '; // << std::endl;
}

template<typename T, typename... Args>
void logArgs(std::ostream& o, T t, Args... args) // recursive variadic function
{
    logArgs(o, t);
    logArgs(o, args...);
}


//
// Log Stream
//


#ifdef SCY_ENABLE_LOGGING

struct LogStream
{
    LogLevel level;
    int line;
    std::string realm;
    std::string address; // deprecated - encode in message
    std::ostringstream message;
    std::time_t ts;
    LogChannel* channel;
    bool flushed;

    LogStream(LogLevel level = LDebug, const std::string& realm = "", int line = 0,
              const void* ptr = nullptr, const char* channel = nullptr);
    LogStream(const LogStream& that);
    ~LogStream();

    /// Recursively log veradic arguments.
    template<typename... Args>
    void write(Args... args)
    {
        logArgs(message, args...);
        flush();
    }

    /// Write data to the log message.
    template <typename T> LogStream& operator<<(const T& data)
    {
        message << data;
        return *this;
    }

    LogStream& operator<<(const LogLevel data)
    {
        level = data;
        return *this;
    }

    LogStream& operator<<(LogChannel* data)
    {
        channel = data;
        return *this;
    }

    /// Handle std::endl manipulators.
    /// This method flushes the log message and queues it for write.
    LogStream& operator<<(std::ostream& (*f)(std::ostream&))
    {
        flush();
        return *this;
    }

    void flush()
    {
        if (!flushed) {
            flushed = true;
            message << std::endl;
            Logger::instance().write(new LogStream(*this));
        }
    }
};

#else

struct LogStream
{
    LogStream(LogLevel level = LDebug, const std::string& realm = "",
        int line = 0, const void* ptr = nullptr, const char* channel = nullptr) {};
    LogStream(const LogStream& that) {};

    template<typename... Args>
    void write(Args... args)
    {
    }

    template <typename T> LogStream& operator<<(const T&)
    {
        return *this;
    }

    LogStream& operator<<(std::ostream& (*f)(std::ostream&))
    {
        return *this;
    }
};

#endif


//
// Log Channel
//


class Base_API LogChannel
{
public:
    LogChannel(const std::string& name, LogLevel level = LDebug,
               const std::string& timeFormat = "%H:%M:%S");
    virtual ~LogChannel() {};

    virtual void write(const LogStream& stream);
    virtual void write(const std::string& message, LogLevel level = LDebug,
                       const char* realm = "", const void* ptr = nullptr);
    virtual void format(const LogStream& stream, std::ostream& ost);

    std::string name() const { return _name; };
    LogLevel level() const { return _level; };
    std::string timeFormat() const { return _timeFormat; };

    void setLevel(LogLevel level) { _level = level; };
    void setDateFormat(const std::string& format) { _timeFormat = format; };
    void setFilter(const std::string& filter) { _filter = filter; }

protected:
    std::string _name;
    LogLevel _level;
    std::string _timeFormat;
    std::string _filter;
};


/// Null log channel.
///
/// Redifine the base `LogChannel` as `NullChannel` so it can be logically used
/// as a disabled log channel.
typedef LogChannel NullChannel;


//
// Console Channel
//


class Base_API ConsoleChannel : public LogChannel
{
public:
    ConsoleChannel(const std::string& name, LogLevel level = LDebug,
                   const std::string& timeFormat = "%H:%M:%S");
    virtual ~ConsoleChannel(){};

    virtual void write(const LogStream& stream);
};


//
// File Channel
//


class Base_API FileChannel : public LogChannel
{
public:
    FileChannel(const std::string& name, const std::string& path,
                LogLevel level = LDebug, const char* timeFormat = "%H:%M:%S");
    virtual ~FileChannel();

    virtual void write(const LogStream& stream);

    void setPath(const std::string& path);
    std::string path() const;

protected:
    virtual void open();
    virtual void close();

protected:
    std::ofstream _fstream;
    std::string _path;
};


//
// Rotating File Channel
//


class Base_API RotatingFileChannel : public LogChannel
{
public:
    RotatingFileChannel(const std::string& name, const std::string& dir,
                        LogLevel level = LDebug,
                        const std::string& extension = "log",
                        int rotationInterval = 12 * 3600,
                        const char* timeFormat = "%H:%M:%S");
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
    std::string _dir;
    std::string _filename;
    std::string _extension;
    int _rotationInterval; ///< The log rotation interval in seconds
    time_t _rotatedAt;     ///< The time the log was last rotated
};


#if 0
class Base_API EventedFileChannel: public FileChannel
{
public:
    EventedFileChannel(
        const std::string& name,
        const std::string& dir,
        LogLevel level = LDebug,
        const std::string& extension = "log",
        int rotationInterval = 12 * 3600,
        const char* timeFormat = "%H:%M:%S");
    virtual ~EventedFileChannel();

    virtual void write(const std::string& message, LogLevel level = LDebug,
        const char* realm = "", const void* ptr = nullptr);
    virtual void write(const LogStream& stream);

    Signal<void(const std::string&, LogLevel&, const Polymorphic*&)> OnLogStream;
};
#endif


//
// Compile time helpers for replacing file paths with filenames
//


constexpr const char* str_end(const char *str)
{
    return *str ? str_end(str + 1) : str;
}

constexpr bool str_slant(const char *str)
{
    return *str == '/' || *str == '\\' ? true : (*str ? str_slant(str + 1) : false);
}

constexpr const char* r_slant(const char* str)
{
    return *str == '/' || *str == '\\' ? (str + 1) : r_slant(str - 1);
}

constexpr const char* file_name(const char* str)
{
    return str_slant(str) ? r_slant(str_end(str)) : str;
}


//
// Macros for debug logging
//


// Macro for logging class functions
#ifndef __CLASS_FUNCTION__
#if _MSC_VER
#define __CLASS_FUNCTION__ __FUNCTION__
#else
inline std::string _methodName(const std::string& fsig)
{
    size_t colons = fsig.find("::");
    size_t sbeg = fsig.substr(0, colons).rfind(" ") + 1;
    size_t send = fsig.rfind("(") - sbeg;
    return fsig.substr(sbeg, send) + "()";
}
#define __CLASS_FUNCTION__ _methodName(__PRETTY_FUNCTION__)
#endif
#endif

#define TraceL LogStream(LTrace, file_name(__FILE__), __LINE__)
#define DebugL LogStream(LDebug, file_name(__FILE__), __LINE__)
#define InfoL LogStream(LInfo, file_name(__FILE__), __LINE__)
#define WarnL LogStream(LWarn, file_name(__FILE__), __LINE__)
#define ErrorL LogStream(LError, file_name(__FILE__), __LINE__)

#define TraceA(...) { LogStream(LTrace, file_name(__FILE__), __LINE__).write(__VA_ARGS__); }
#define DebugA(...) { LogStream(LDebug, file_name(__FILE__), __LINE__).write(__VA_ARGS__); }
#define InfoA(...) { LogStream(LInfo, file_name(__FILE__), __LINE__).write(__VA_ARGS__); }
#define WarnA(...) { LogStream(LWarn, file_name(__FILE__), __LINE__).write(__VA_ARGS__); }
#define ErrorA(...) { LogStream(LError, file_name(__FILE__), __LINE__).write(__VA_ARGS__); }

#define TraceS(self) LogStream(LTrace, file_name(__FILE__), __LINE__, self)
#define DebugS(self) LogStream(LDebug, file_name(__FILE__), __LINE__, self)
#define InfoS(self) LogStream(LInfo, file_name(__FILE__), __LINE__, self)
#define WarnS(self) LogStream(LWarn, file_name(__FILE__), __LINE__, self)
#define ErrorS(self) LogStream(LError, file_name(__FILE__), __LINE__, self)

#define TraceN(self) LogStream(LTrace, self->className(), __LINE__, self)
#define DebugN(self) LogStream(LDebug, self->className(), __LINE__, self)
#define InfoN(self) LogStream(LInfo, self->className(), __LINE__, self)
#define WarnN(self) LogStream(LWarn, self->className(), __LINE__, self)
#define ErrorN(self) LogStream(LError, self->className(), __LINE__, self)


} // namespace scy


#endif


/// @\}
