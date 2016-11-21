///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_Async_H
#define SCY_Async_H


#include "scy/uv/uvpp.h"
#include <atomic>
#include <cstdint>
#include <functional>
#include <memory>
#include <stdexcept>


namespace scy {


/// Classes for asynchronous programming
namespace async {


/// A generic interface for classes that can be run and cancelled.
class Runnable
{
    std::atomic<bool> exit;

public:
    Runnable()
        : exit(false)
    {
    }
    virtual ~Runnable() {}

    /// The run method will be called by the async context.
    virtual void run() = 0;

    /// Cancel the current task.
    /// The run() method should return ASAP.
    virtual void cancel(bool flag = true)
    {
        exit.store(flag, std::memory_order_release);
    }

    /// True when the task has been cancelled.
    virtual bool cancelled() const
    {
        bool s = exit.load(std::memory_order_relaxed);
        if (s)
            std::atomic_thread_fence(std::memory_order_acquire);
        return s;
    };
};


//
// Runner Interface
//


/// Runner is a virtual interface for implementing
/// asynchronous objects such as threads and futures.
class Runner
{
public:
    Runner();
    virtual ~Runner();

    virtual void start(async::Runnable& target);
    virtual void start(std::function<void()> target);

    /// Starts the thread with the given target.
    // TODO: veradic templates when win support is better (vs2013)
    virtual void start(std::function<void(void*)> target, void* arg);

    /// Returns true if the async context has been started.
    bool started() const;

    /// Returns true if the async context is currently running.
    bool running() const;

    /// Cancels the async context.
    void cancel();

    /// True when the task has been cancelled.
    /// It is up to the implementation to return at the
    /// earliest possible time.
    bool cancelled() const;

    /// Returns true if the Runner is operating in repeating mode.
    bool repeating() const;

    /// Return the native thread ID.
    uv_thread_t tid() const;

    /// This setting means the implementation should call the
    /// target function repeatedly until cancelled. The importance
    /// of this method to normalize the functionality of threadded
    /// and event loop driven Runner models.
    void setRepeating(bool flag);

    /// Returns true if the implementation is thread-based, or false
    /// if it belongs to an event loop.
    virtual bool async() const = 0;

    typedef std::shared_ptr<Runner> Ptr;

    /// The context which we send to the thread context.
    /// This allows us to garecefully handle late callbacks
    /// and avoid the need for deferred destruction of Runner objects.
    struct Context
    {
        typedef std::shared_ptr<Context> ptr;

        /// Thread-safe POD members
        /// May be accessed at any time
        uv_thread_t tid;
        bool started;
        bool running;
        bool repeating;
        std::atomic<bool> exit;

        /// Non thread-safe members
        /// Should not be accessed once the Runner is started
        std::function<void()> target;
        std::function<void(void*)> target1;
        void* arg;
        void* handle; ///< private implementation data

        // Cancels the async context.
        void cancel();

        // True when the task has been cancelled.
        // It is up to the implementation to return at the
        // earliest possible time.
        bool cancelled() const;

        // The implementation is responsible for resetting
        // the context if it is to be reused.
        void reset()
        {
            tid = (uv_thread_t)(0);
            arg = nullptr;
            target = nullptr;
            target1 = nullptr;
            started = false;
            running = false;
            exit = false;
        }

        Context()
        {
            reset();

            // Non-reseting members
            repeating = false;
            handle = nullptr;
        }
    };

protected:
    /// Shared pointer to the internal Runner::Context.
    Context::ptr pContext;

    /// Start the context from the control thread.
    virtual void startAsync() = 0;

    /// Run the context from the async thread.
    static void runAsync(Context* context);

    Runner(const Runner&);
    Runner& operator=(const Runner&);
};


//
// Concurrent Flag
//

/// A concurrent flag which can be
/// used to request task cancellation.
class Flag
{
    std::atomic<bool> state;

    /// Non-copyable and non-movable
    Flag(const Flag&) = delete;
    Flag(Flag&&) = delete;
    Flag& operator=(const Flag&) = delete;
    Flag& operator=(Flag&&) = delete;

public:
    Flag() : state(false) {};

    bool get() const { return state.load(); }
    void set(bool flag) { state = flag; }
};


/// For C client data callbacks.
// typedef void (*Callable)(void*);


/// A generic interface for a classes
/// that can be started and stopped.
class Startable
{
public:
    virtual void start() = 0;
    virtual void stop() = 0;
};


/// A generic interface for classes
/// that can be sent and cancelled.
class Sendable
{
public:
    virtual bool send() = 0;
    virtual void cancel(){};
};


} // namespace async
} // namespace scy


#endif // SCY_Async_H


/// @\}
