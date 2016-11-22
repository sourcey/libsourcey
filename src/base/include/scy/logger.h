///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
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

    /// Writes the given log message stream.
    virtual void write(LogStream* stream);
};


//
// Asynchronous Log Writer
//


class AsyncLogWriter : public LogWriter, public basic::Runnable
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


class Logger
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
    /// Non-copyable and non-movable
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&&) = delete;

    typedef std::map<std::string, LogChannel*> LogChannelMap;

    friend class Singleton<Logger>;
    friend class Thread;

    mutable std::mutex _mutex;
    LogChannelMap _channels;
    LogChannel* _defaultChannel;
    LogWriter* _writer;
};


//
// Log Stream
//


struct LogStream
{
    LogLevel level;
    int line;
    std::string realm;
    std::string address; // deprecated - encode in message
    std::ostringstream message;
    std::time_t ts;
    LogChannel* channel;

    LogStream(LogLevel level = LDebug, const std::string& realm = "",
              int line = 0, const void* ptr = nullptr,
              const char* channel = nullptr);
    LogStream(LogLevel level, const std::string& realm = "",
              const std::string& address = "");
    LogStream(const LogStream& that);
    ~LogStream();

    LogStream& operator<<(const LogLevel data)
    {
#ifndef SCY_DISABLE_LOGGING
        level = data;
#endif
        return *this;
    }

    LogStream& operator<<(LogChannel* data)
    {
#ifndef SCY_DISABLE_LOGGING
        channel = data;
#endif
        return *this;
    }

    template <typename T> LogStream& operator<<(const T& data)
    {
#ifndef SCY_DISABLE_LOGGING
        message << data;
#endif
        return *this;
    }

    /// Handle std::endl flags.
    /// This method flushes the log message and queues it for write.
    /// WARNING: After using std::endl to flush the message pointer
    /// should not be accessed.
    LogStream& operator<<(std::ostream& (*f)(std::ostream&))
    {
#ifndef SCY_DISABLE_LOGGING
        message << f;

        /// Send to default channel
        /// Channel flag or stream operation
        Logger::instance().write(this);
#else
        /// Free the pointer
        delete this;
#endif
        return *this;
    }
};


//
// Inline stream accessors
//


// Default output
inline LogStream& traceL(const char* realm = "", const void* ptr = nullptr)
{
    return *new LogStream(LTrace, realm, 0, ptr);
}

inline LogStream& debugL(const char* realm = "", const void* ptr = nullptr)
{
    return *new LogStream(LDebug, realm, 0, ptr);
}

inline LogStream& infoL(const char* realm = "", const void* ptr = nullptr)
{
    return *new LogStream(LInfo, realm, 0, ptr);
}

inline LogStream& warnL(const char* realm = "", const void* ptr = nullptr)
{
    return *new LogStream(LWarn, realm, 0, ptr);
}

inline LogStream& errorL(const char* realm = "", const void* ptr = nullptr)
{
    return *new LogStream(LError, realm, 0, ptr);
}

inline LogStream& fatalL(const char* realm = "", const void* ptr = nullptr)
{
    return *new LogStream(LFatal, realm, 0, ptr);
}


// Channel output
inline LogStream& traceC(const char* channel, const char* realm = "",
                         const void* ptr = nullptr)
{
    return *new LogStream(LTrace, realm, 0, ptr, channel);
}

inline LogStream& debugC(const char* channel, const char* realm = "",
                         const void* ptr = nullptr)
{
    return *new LogStream(LDebug, realm, 0, ptr, channel);
}

inline LogStream& infoC(const char* channel, const char* realm = "",
                        const void* ptr = nullptr)
{
    return *new LogStream(LInfo, realm, 0, ptr, channel);
}

inline LogStream& warnC(const char* channel, const char* realm = "",
                        const void* ptr = nullptr)
{
    return *new LogStream(LWarn, realm, 0, ptr, channel);
}

inline LogStream& errorC(const char* channel, const char* realm = "",
                         const void* ptr = nullptr)
{
    return *new LogStream(LError, realm, 0, ptr, channel);
}

inline LogStream& fatalC(const char* channel, const char* realm = "",
                         const void* ptr = nullptr)
{
    return *new LogStream(LFatal, realm, 0, ptr, channel);
}


// Level output
inline LogStream& printL(const char* level = "debug", const char* realm = "",
                         const void* ptr = nullptr,
                         const char* channel = nullptr)
{
    return *new LogStream(getLogLevelFromString(level), realm, 0, ptr, channel);
}

inline LogStream& printL(const char* level, const void* ptr,
                         const char* realm = "", const char* channel = nullptr)
{
    return *new LogStream(getLogLevelFromString(level), realm, 0, ptr, channel);
}


// Macros for debug logging
//
// Other useful macros for debug logging: __FILE__, __FUNCTION__, __LINE__
// KLUDGE: Need a way to shorten __FILE__  which prints the entire relative path
// __FUNCTION__ might need a fallback on some platforms
#ifndef __CLASS_FUNCTION__
#if _MSC_VER
#define __CLASS_FUNCTION__ __FUNCTION__
#else
inline std::string _methodName(const std::string& fsig)
{
    std::size_t colons = fsig.find("::");
    std::size_t sbeg = fsig.substr(0, colons).rfind(" ") + 1;
    std::size_t send = fsig.rfind("(") - sbeg;
    return fsig.substr(sbeg, send) + "()";
}
#define __CLASS_FUNCTION__ _methodName(__PRETTY_FUNCTION__)
#endif
#endif

#define TraceL *new LogStream(LTrace, __CLASS_FUNCTION__, __LINE__)
#define DebugL *new LogStream(LDebug, __CLASS_FUNCTION__, __LINE__)
#define InfoL *new LogStream(LInfo, __CLASS_FUNCTION__, __LINE__)
#define WarnL *new LogStream(LWarn, __CLASS_FUNCTION__, __LINE__)
#define ErrorL *new LogStream(LError, __CLASS_FUNCTION__, __LINE__)

#define TraceS(self) *new LogStream(LTrace, __CLASS_FUNCTION__, __LINE__, self)
#define DebugS(self) *new LogStream(LDebug, __CLASS_FUNCTION__, __LINE__, self)
#define InfoS(self) *new LogStream(LInfo, __CLASS_FUNCTION__, __LINE__, self)
#define WarnS(self) *new LogStream(LWarn, __CLASS_FUNCTION__, __LINE__, self)
#define ErrorS(self) *new LogStream(LError, __CLASS_FUNCTION__, __LINE__, self)

#define TraceN(self) *new LogStream(LTrace, self->className(), __LINE__, self)
#define DebugN(self) *new LogStream(LDebug, self->className(), __LINE__, self)
#define InfoN(self) *new LogStream(LInfo, self->className(), __LINE__, self)
#define WarnN(self) *new LogStream(LWarn, self->className(), __LINE__, self)
#define ErrorN(self) *new LogStream(LError, self->className(), __LINE__, self)


//
// Log Channel
//


class LogChannel
{
public:
    LogChannel(const std::string& name, LogLevel level = LDebug,
               const std::string& timeFormat = "%H:%M:%S");
    virtual ~LogChannel(){};

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


//
// Console Channel
//


class ConsoleChannel : public LogChannel
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


class FileChannel : public LogChannel
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


class RotatingFileChannel : public LogChannel
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
class EventedFileChannel: public FileChannel
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


} // namespace scy


#endif


/// @\}
