///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_Thread_H
#define SCY_Thread_H


#include "scy/base.h"
#include "scy/runner.h"
#include "scy/util.h"

#include <thread>
#include <tuple>
#include <utility>
#include <mutex>


namespace scy {


/// Platform-independent wrapper around an operating system thread.
///
/// This class inherits the `Runner` interface and may be used with any
/// implementation that's powered by an asynchronous `Runner`.
///
class Base_API Thread : public Runner
{
public:
    typedef std::shared_ptr<Thread> Ptr;

    /// Default constructor.
    Thread();

    /// Templated constructor.
    ///
    /// This constructor starts the thread with the given function.
    template<typename Function, typename... Args>
    explicit Thread(Function&& func, Args&&... args) :
        _thread(internal::runAsync<Function, Args...>, _context,
                std::forward<Function>(func),
                std::forward<Args>(args)...)
    {
    }

    /// Destructor.
    virtual ~Thread();

    /// Start a function with veradic arguments.
    ///
    /// This method starts the thread with the given function.
    template<typename Function, typename... Args>
    void start(Function&& func, Args&&... args)
    {
        _thread = std::thread(internal::runAsync<Function, Args...>, _context,
                              std::forward<Function>(func),
                              std::forward<Args>(args)...);
    }

    /// Start the asynchronous context with the given void function.
    virtual void start(std::function<void()> func);

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

    bool async() const override;

    std::thread _thread;
};


#if 0
/// This class is an invisible wrapper around a Startable instance,
/// which provides asynchronous access to the Startable start() and
/// stop() methods.
///
/// TStartable is an instance of basic::Startable.
/// @deprecated
template <class TStartable>
class Base_API AsyncStartable: public TStartable
{
public:
    AsyncStartable() = default;
    virtual ~AsyncStartable() = default;

    static void runAsync(void* arg) {
        try {
            // Call the blocking start() function once only
            static_cast<TStartable*>(arg)->start();
        }
        catch (std::exception& exc) {
            // errorL("AsyncLtartable")(exc.what(), )
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
