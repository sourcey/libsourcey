///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_Interfaces_H
#define SCY_Interfaces_H


#include "scy/base.h"
#include "scy/uv/uvpp.h" // ssize_t
#include <atomic>
#include <functional>
#include <memory>
#include <stdexcept>


namespace scy {


/// Interface classes
namespace basic {


class Base_API Decoder
{
public:
    Decoder() {}
    virtual ~Decoder() {}

    virtual ssize_t decode(const char* inbuf, size_t nread, char* outbuf) = 0;
    virtual ssize_t finalize(char* /* outbuf */) { return 0; }
};


class Base_API Encoder
{
public:
    Encoder() {}
    virtual ~Encoder() {}
    virtual ssize_t encode(const char* inbuf, size_t nread, char* outbuf) = 0;
    virtual ssize_t finalize(char* /* outbuf */) { return 0; }
};


/// Abstract interface for classes that can be run and cancelled.
class Base_API Runnable
{
public:
    Runnable()
        : exit(false)
    {
    }

    virtual ~Runnable() {}

    /// The run method will be called by the asynchronous context.
    virtual void run() = 0;

    /// Cancel the current task.
    /// The run() method should return ASAP.
    virtual void cancel(bool flag = true)
    {
        exit = flag;
    }

    /// Returns true when the task has been cancelled.
    virtual bool cancelled() const
    {
        return exit.load();
    };

protected:
    std::atomic<bool> exit;
};


/// Abstract interface for a classes that can be started and stopped.
class Base_API Startable
{
public:
    virtual void start() = 0;
    virtual void stop() = 0;
};


/// Abstract interface for classes that can be sent and cancelled.
class Base_API Sendable
{
public:
    virtual bool send() = 0;
    virtual void cancel() {};
};


#if 0
/// A base module class for C++ callback polymorphism.
/// @deprecated
class Base_API Polymorphic
{
public:
    virtual ~Polymorphic() {};

    template<class T>
    bool is() {
        return dynamic_cast<T*>(this) != nullptr;
    };

    template<class T>
    T* as(bool whiny = false) {
        T* self = dynamic_cast<T*>(this);
        if (self == nullptr && whiny)
            throw std::runtime_error("Polymorphic cast failed");
        return self;
    };

    scy::LogStream& log(const char* level = "debug") const;

    virtual const char* className() const = 0;
};
#endif


} // namespace basic
} // namespace scy


#endif // SCY_Interfaces_H


/// @\}
