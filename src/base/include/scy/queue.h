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


#ifndef SCY_Queue_H
#define SCY_Queue_H


#include "scy/interface.h"
#include "scy/thread.h"
#include "scy/platform.h"
#include "scy/synccontext.h"
#include "scy/datetime.h"
#include <queue>


namespace scy {

	
template<typename T>
class Queue
	/// Implements a thread-safe queue container.
	/// TODO: Iterators
{
private:
    std::queue<T> _queue;
	mutable Mutex _mutex;

public:
    void push(const T& data)
    {
		Mutex::ScopedLock lock(_mutex);
        _queue.push(data);
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
        _queue.pop();
    }

    void popFront()
    {
		Mutex::ScopedLock lock(_mutex);
        _queue.pop_front();
    }
};


//
// Runnable Queue
//


template<class T>
class RunnableQueue: public async::Runnable
{
public:
	RunnableQueue(int limit = 2048, int timeout = 0) :
		_limit(limit), 
		_timeout(timeout)
	{
	}

	virtual ~RunnableQueue() 
	{
		clear();
	}

	std::function<void(T&)> ondispatch;
		// The default dispatch function.
		// Must be set before the queue is running.
		
	virtual void dispatch(T& item)
		// Dispatch a single item to listeners.
	{
		if (ondispatch)
			ondispatch(item);
	}
	
	virtual void push(T* item)
		// Push an item onto the queue.
		// The queue takes ownership of the item pointer.		
	{
		Mutex::ScopedLock lock(_mutex);	
				
		while (_limit > 0 && static_cast<int>(_queue.size()) >= _limit) {
			warnL("RunnableQueue", this) << "Purging: " << _queue.size() << std::endl;
			delete _queue.front();
			_queue.pop_front();
		}
			
		_queue.push_back(item);
	}
	
	virtual void flush()
		// Flushes all outgoing items.
	{
		do {
			// scy::sleep(1);
		}
		while (dispatchNext());			
	}
	
	void clear()
		// Clears all queued items.
	{
		Mutex::ScopedLock lock(_mutex);	
		util::clearDeque(_queue);
	}
	
	bool empty()
	{
		// Disabling mutex lock for bool check.
		//Mutex::ScopedLock lock(_mutex);	
		return _queue.empty();
	}
	
	std::size_t size()
	{
		Mutex::ScopedLock lock(_mutex);	
		return _queue.size();
	}
	
	virtual std::deque<T*> queue()
	{
		Mutex::ScopedLock lock(_mutex);
		return _queue;
	}
	
	virtual void run()
		// Called asynchronously to dispatch queued items.
		// If not timeout is set this method blocks until cancel()
		// is called, otherwise runTimeout() will be called.
		// Pseudo protected for std::bind compatability.
	{
		if (_timeout) {
			runTimeout();
		}
		else {
			while (!cancelled()) {
				scy::sleep(dispatchNext() ? 1 : 50);
			}
		}
	}
	
	virtual void runTimeout()
		// Called asynchronously to dispatch queued items
		// until the queue is empty or the timeout expires.
		// Pseudo protected for std::bind compatability.
	{
		Stopwatch sw;
		sw.start();
		do {
			// scy::sleep(1);
		}
		while (!cancelled() && sw.elapsedMilliseconds() < _timeout && dispatchNext());
	}
	
	int timeout()	
	{
		Mutex::ScopedLock lock(_mutex);
		return _timeout;
	}
	
	void setTimeout(int miliseconds)
	{
		Mutex::ScopedLock lock(_mutex);
		assert(_queue.empty() && "queue must not be active");
		_timeout = miliseconds;
	}
	
protected:	
	RunnableQueue(const RunnableQueue&);
	RunnableQueue& operator = (const RunnableQueue&);

	virtual T* popNext()
		// Pops the next waiting item.
	{
		T* next;
		{
			Mutex::ScopedLock lock(_mutex);
			if (_queue.empty())
				return nullptr;

			next = _queue.front();
			_queue.pop_front();
		}		
		return next;
	}
	
	virtual bool dispatchNext()
		// Pops and dispatches the next waiting item.
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
	int _timeout;
	std::deque<T*> _queue;
	mutable Mutex _mutex;
};


//
// Synchronization Queue
//


template<class T>
class SyncQueue: public RunnableQueue<T>
	// SyncQueue extends SyncContext to implement a synchronized FIFO
	// queue which receives T objects from any thread and synchronizes
	// them for safe consumption by the associated event loop.
{
public:
	SyncQueue(uv::Loop* loop, int limit = 2048, int timeout = 20) :
		RunnableQueue<T>(limit, timeout), 
		// Note: The SyncQueue instance must not be destroyed
		// while the RunnableQueue is still dispatching items.
		_sync(loop, std::bind(&SyncQueue::run, this))
	{
	}

	virtual ~SyncQueue() 
		// Destruction is deferred to allow enough    
		// time for all callbacks to return.
	{
	}
	
	virtual void push(T* item)
		// Pushes an item onto the queue.
		// Item pointers are now managed by the SyncQueue.		
	{
		RunnableQueue<T>::push(item);
		_sync.post();
	}
	
	virtual void cancel()
	{
		RunnableQueue<T>::cancel();
		_sync.cancel();

		// Call uv_close on the handle if calling from  
		// the event loop thread or we deadlock.
		if (Thread::currentID() == _sync.tid())
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
class AsyncQueue: public RunnableQueue<T>
	// AsyncQueue is a thread-based queue which receives packets  
	// from any thread source and dispatches them asynchronously.
	//
	// This queue is useful for deferring load from operation 
	// critical system devices before performing long running tasks.
	//
	// The thread will call the RunnableQueue's run() method to
	// constantly flush outgoing packets until cancel() is called. 
{
public:
	AsyncQueue(int limit = 2048) : 
		RunnableQueue<T>(limit),
		_thread(std::bind(&AsyncQueue::run, this))
	{
	}	
	
	virtual void cancel()
	{
		RunnableQueue<T>::cancel();
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
class ConcurrentQueue
	// Implements a simple thread-safe multiple producer, 
	// multiple consumer queue. 
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