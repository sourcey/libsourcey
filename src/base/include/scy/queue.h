///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_Queue_H
#define SCY_Queue_H


#include "scy/interface.h"
#include "scy/thread.h"
#include "scy/platform.h"
#include "scy/synccontext.h"
#include "scy/datetime.h"
#include <queue>


namespace scy {


/// Implements a thread-safe queue container.
// TODO: Iterators
template<typename T>
class Queue
{
private:
    std::deque<T> _queue;
    mutable Mutex _mutex;

public:
    void push(const T& data)
    {
        Mutex::ScopedLock lock(_mutex);
        _queue.push_back(data);
    }

    bool empty() const
    {
        //Mutex::ScopedLock lock(_mutex);
        return _queue.empty();
    }

    T& front()
    {
        Mutex::ScopedLock lock(_mutex);
        return _queue.front();
    }

    T const& front() const
    {
        Mutex::ScopedLock lock(_mutex);
        return _queue.front();
    }

    T& back()
    {
        Mutex::ScopedLock lock(_mutex);
        return _queue.back();
    }

    T const& back() const
    {
        Mutex::ScopedLock lock(_mutex);
        return _queue.back();
    }

    void pop()
    {
        Mutex::ScopedLock lock(_mutex);
        _queue.pop_front();
    }

    template<typename Compare>
    std::size_t sort()
    {
        Mutex::ScopedLock lock(_mutex);
        return std::sort(_queue.begin(), _queue.end(), Compare());
    }

    std::size_t size()
    {
        Mutex::ScopedLock lock(_mutex);
        return _queue.size();
    }

    std::deque<T>& queue()
    {
        Mutex::ScopedLock lock(_mutex);
        return _queue;
    }
};


//
// Runnable Queue
//


template<class T>
class RunnableQueue: public Queue<T*>, public async::Runnable
{
public:
    typedef Queue<T*> queue_t;    /// The default dispatch function.
    /// Must be set before the queue is running.
    std::function<void(T&)> ondispatch;


    RunnableQueue(int limit = 2048, int timeout = 0) :
        _limit(limit),
        _timeout(timeout)
    {
    }

    virtual ~RunnableQueue()
    {
        clear();
    }

    virtual void push(T* item)    // Push an item onto the queue.
    /// The queue takes ownership of the item pointer.
    {
        Mutex::ScopedLock lock(_mutex);

        while (_limit > 0 && static_cast<int>(queue_t::size()) >= _limit) {
            WarnS(this) << "Purging: " << queue_t::size() << std::endl;
            delete queue_t::front();
            queue_t::pop();
        }

        queue_t::push(reinterpret_cast<T*>(item));
    }

    virtual void flush()    // Flushes all outgoing items.
    {
    /// do {
    /// }    // while (dispatchNext());    //     // scy::sleep(1);


        while (!queue_t::empty()) {
            auto next = queue_t::front();
            dispatch(*next);
            queue_t::pop();
            delete next;
        }
    }

    void clear()    // Clears all queued items.
    {
        Mutex::ScopedLock lock(_mutex);
        while (!queue_t::empty()) {
            delete queue_t::front();
            queue_t::pop();
        }
    }
    /// bool empty()    /// {    ///     // Disabling mutex lock for bool check.
    ///     //Mutex::ScopedLock lock(_mutex);    ///     return empty();    /// }

    virtual void run()    // Called asynchronously to dispatch queued items.
    /// If not timeout is set this method blocks until cancel()    // is called, otherwise runTimeout() will be called.
    /// Pseudo protected for std::bind compatability.
    {
        if (_timeout) {
            runTimeout();
        }
        else {
            while (!cancelled()) {
                dispatchNext();
                scy::sleep(1);
                // scy::sleep(dispatchNext() ? 1 : 50);
            }
        }
    }

    virtual void runTimeout()    // Called asynchronously to dispatch queued items
    /// until the queue is empty or the timeout expires.
    /// Pseudo protected for std::bind compatability.
    {
        Stopwatch sw;
        sw.start();
        do {
            // scy::sleep(1);
        }
        while (!cancelled() && sw.elapsedMilliseconds() < _timeout && dispatchNext());
    }

    virtual void dispatch(T& item)    // Dispatch a single item to listeners.
    {
        if (ondispatch)
            ondispatch(item);
    }
    /// int timeout()    /// {    ///     Mutex::ScopedLock lock(_mutex);    ///     return _timeout;    /// }

    void setTimeout(int milliseconds)
    {
        Mutex::ScopedLock lock(_mutex);
        assert(queue_t::empty() && "queue must not be active");
        _timeout = milliseconds;
    }

protected:
    RunnableQueue(const RunnableQueue&);
    RunnableQueue& operator = (const RunnableQueue&);

    virtual T* popNext()    // Pops the next waiting item.
    {
        T* next;
        {
            Mutex::ScopedLock lock(_mutex);
            if (queue_t::empty())
                return nullptr;

            next = queue_t::front();
            queue_t::pop();
        }
        return next;
    }

    virtual bool dispatchNext()    // Pops and dispatches the next waiting item.
    {
        T* next = popNext();
        if (next) {
            dispatch(*next);
            delete next;
            return true;
        }
        return false;
    }

    int _limit;
    int _timeout;    /// queue_t _queue;
    mutable Mutex _mutex;
};


//
// Synchronization Queue
//


template<class T>

    /// SyncQueue extends SyncContext to implement a synchronized FIFO
    /// queue which receives T objects from any thread and synchronizes
    /// them for safe consumption by the associated event loop.
class SyncQueue: public RunnableQueue<T>
{
public:
    typedef RunnableQueue<T> base_t;

    SyncQueue(uv::Loop* loop, int limit = 2048, int timeout = 20) :
        base_t(limit, timeout),    // Note: The SyncQueue instance must not be destroyed
    /// while the RunnableQueue is still dispatching items.
        _sync(loop, std::bind(&base_t::run, this))
    {
    }

    virtual ~SyncQueue()    // Destruction is deferred to allow enough
    /// time for all callbacks to return.
    {
    }

    virtual void push(T* item)    // Pushes an item onto the queue.
    /// Item pointers are now managed by the SyncQueue.
    {
        base_t::push(item);
        _sync.post();
    }

    virtual void cancel()
    {
        base_t::cancel();
        _sync.cancel();    /// Call uv_close on the handle if calling from
    /// the event loop thread or we deadlock.
        if (_sync.tid() == Thread::currentID())
            _sync.close();
    }

    SyncContext& sync()
    {
        return _sync;
    }

protected:
    SyncContext _sync;
};


//
// Asynchronous Queue
//


template<class T>

    /// AsyncQueue is a thread-based queue which receives packets
    /// from any thread source and dispatches them asynchronously.
    ///
    /// This queue is useful for deferring load from operation
    /// critical system devices before performing long running tasks.
    ///
    /// The thread will call the RunnableQueue's run() method to
    /// constantly flush outgoing packets until cancel() is called.
class AsyncQueue: public RunnableQueue<T>
{
public:
    typedef RunnableQueue<T> base_t;

    AsyncQueue(int limit = 2048) :
        base_t(limit),
        _thread(std::bind(&base_t::run, this))
    {
    }

    virtual void cancel()
    {
        base_t::cancel();
        _thread.cancel();
    }

protected:
    virtual ~AsyncQueue()
    {
    }

    Thread _thread;
};


#if 0
//
// Concurrent Queue
//
// TODO: Re-implement Condition class from libuv primitives
//

template<typename T>

    /// Implements a simple thread-safe multiple producer,    /// multiple consumer queue.
class ConcurrentQueue
{
private:
    std::queue<T> _queue;
    mutable Mutex _mutex;
    Poco::Condition _condition;

public:
    void push(T const& data)
    {
        Mutex::ScopedLock lock(_mutex);
        _queue.push(data);
        lock.unlock();
        _condition.signal();
    }

    bool empty() const
    {
        Mutex::ScopedLock lock(_mutex);
        return _queue.empty();
    }

    bool tryPop(T& out)
    {
        Mutex::ScopedLock lock(_mutex);
        if (_queue.empty())
            return false;

        out = _queue.front();
        _queue.pop();
        return true;
    }

    void waitAndPop(T& out)
    {
        Mutex::ScopedLock lock(_mutex);
        while (_queue.empty())
            _cond.wait(_mutex);

        out = _queue.front();
        _queue.pop();
    }
};
#endif


} // namespace scy



#endif // SCY_Queue_H

/// @\}
