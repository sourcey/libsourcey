///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_Thread_H
#define SCY_Thread_H


#include "scy/runner.h"
#include "scy/platform.h"
#include "scy/uv/uvpp.h"
// #include "scy/logger.h"
#include "scy/util.h"
#include <thread>
#include <tuple>
#include <utility>
#include <mutex>


namespace scy {


/// Helper function for running an async context.
template<class Function, class... Args>
void runAsync(Runner::Context::Ptr c, Function func, Args... args)
{
    // std::cout << "Runner::runAsync" << std::endl;
    c->tid = std::this_thread::get_id();
    c->running = true;
    do {
        try {
            func(std::forward<Args>(args)...);
        } catch (std::exception& exc) {
            // ErrorL << "Runner error: " << exc.what() << std::endl;
    #ifdef _DEBUG
            throw exc;
    #endif
        }
        scy::sleep(1);
    } while (c->repeating && !c->cancelled);
    c->running = false;
}


/// This class implements a platform-independent
/// wrapper around an operating system thread.
class Thread : public Runner
{
public:
    typedef std::shared_ptr<Thread> Ptr;

    /// Default constructor.
    Thread();

    /// Templated constructor.
    ///
    /// This constructor starts the thread with the given function.
    template<class Function, class... Args>
    explicit Thread(Function func, Args... args) :
        _thread(runAsync<Function, Args...>, _context, func,
                std::forward<Args>(args)...)
    {
    }

    /// Destructor.
    virtual ~Thread();

    /// Start a function with veradic arguments.
    ///
    /// This method starts the thread with the given function.
    template<class Function, class... Args>
    void start(Function func, Args... args)
    {
        _thread = std::thread(runAsync<Function, Args...>, _context, func,
                              std::forward<Args>(args)...);
    }

    /// Start a `Runnable` target.
    ///
    /// The target `Runnable` instance must outlive the thread.
    virtual void start(std::function<void()> target);

    /// Wait until the thread exits.
    void join();

    /// Return the native thread handle.
    std::thread::id id() const;

    /// Return the native thread ID of the current thread.
    static std::thread::id currentID();

    /// Accessor for the main thread ID.
    static const std::thread::id mainID;

protected:
    /// NonCopyable and NonMovable
    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;

    bool async() const;

    std::thread _thread;
};


#if 0
/// This class is an invisible wrapper around a TStartable instance,
/// which provides asynchronous access to the TStartable start() and
/// stop() methods. TStartable is an instance of basic::Startable.
/// @deprecated
template <class TStartable>
class AsyncStartable: public TStartable
{
public:
    AsyncStartable() {};
    virtual ~AsyncStartable() {};

    static void runAsync(void* arg) {
        try {
            // Call the blocking start() function once only
            static_cast<TStartable*>(arg)->start();
        }
        catch (std::exception& exc) {
            // errorL("AsyncStartable") << exc.what() << std::endl;
#ifdef _DEBUG
            throw exc;
#endif
        }
    }

    virtual bool start()
    {
        _thread.start(*this);
        return true;
    }

    virtual void stop()
    {
        TStartable::stop();
        _thread.join();
    }

protected:
    Thread _thread;
};
#endif


} // namespace scy


#endif


/// @\}
