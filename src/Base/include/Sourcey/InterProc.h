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


#ifndef SOURCEY_InterProcess_H
#define SOURCEY_InterProcess_H


#include "Sourcey/UV/UVPP.h"
#include "Sourcey/Platform.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Timer.h"
#include "Sourcey/Thread.h"

#include <deque>


namespace scy {
		
	
//
// Synchronization Context
//


class SyncContext: public uv::Handle
	/// SyncContext enables any thread to communicate
	/// with the associated event loop via synchronized
	/// callbacks.
	///
	/// TODO: Cancelling when uv_cancel is available on win.
{
public:
	SyncContext(uv::Loop& loop) : 
		uv::Handle(&loop, new uv_async_t),
		_loop(loop)
	{
		_handle->data = this;
		uv_async_init(&loop, handle<uv_async_t>(), [](uv_async_t* req, int)
		{
			auto self = reinterpret_cast<SyncContext*>(req->data);

			// The context pointer will always  
			// outlive the next libuv iteration.
			if (self->closed()) {
				warnL("SyncContext", self) << "late callback" << std::endl;
				return;
			}

			self->run();
		});
	}
	
	virtual void destroy()
	{
		uv::Handle::close();

		// Defer deletion since we need to wait 
		// for all libuv callbacks to return.
		deleteLater<SyncContext>(this);
	}

	void send()
		// Can be called from any thread to send  
		// the synchronized callback.
	{
		assert(!closed());
		uv_async_send(handle<uv_async_t>());
	}
	
protected:	
	SyncContext(const SyncContext&); // = delete;
	SyncContext(SyncContext&&); // = delete;
	SyncContext& operator=(const SyncContext&); // = delete;
	SyncContext& operator=(SyncContext&&); // = delete;

	virtual ~SyncContext()
	{
		assert(closed());
	}

	virtual void run() = 0;
		// Called by the libuv event loop to 
		// perform synchronized actions.

	uv::Loop& _loop;

	friend struct std::default_delete<SyncContext>;
};


//
// Runnable Queue
//


template<class T>
class RunnableQueue: public basic::Runnable
{
public:
	RunnableQueue(int limit = 2048) :
		_limit(limit)
	{
	}	
	
	virtual void push(T* item)
		// Pushes an item onto the queue.
		// Ownership is taken of the item pointer.		
	{
		Mutex::ScopedLock lock(_mutex);	
				
		while (static_cast<int>(_queue.size()) >= (_limit)) {
			traceL("RunnableQueue", this) << "purging: " << _queue.size() << std::endl;
			delete _queue.front();
			_queue.pop_front();
		}
			
		_queue.push_back(item);
	}
	
	virtual void flush()
		// Flushes all outgoing items.
	{
		while (emitNext()) 
			scy::sleep(1);
	}
	
	virtual std::deque<T*> queue()
	{
		Mutex::ScopedLock lock(_mutex);
		return _queue;
	}
	
	virtual void emit(T& item) = 0;
		// Override this method to emit a single item.
	
	virtual void run()
		// Called asynchronously to dispatch queued items.
		// This method blocks until cancel() is called.
	{
		while (!cancelled())
			scy::sleep(emitNext() ? 1 : 50);
	}
	
	virtual void runFor(int timeout)
		// Called asynchronously to dispatch queued items
		// until the queue is empty or the timeout expires.
	{
		Stopwatch sw;
		sw.start();
		while (!cancelled() && sw.elapsedMilliseconds() < timeout && emitNext())
			scy::sleep(1);
	}
	
	bool emitNext()
		// Pops and emits the next waiting item.
	{
		T* next;
		{
			Mutex::ScopedLock lock(_mutex);
			if (_queue.empty())
				return false;

			next = _queue.front();
			_queue.pop_front();
		}		
		emit(*next);
		delete next;
		return true;
	}
	
	void clear()
		// Clears all queued items.
	{
		Mutex::ScopedLock lock(_mutex);	
		util::clearDeque(_queue);
	}

protected:
	virtual ~RunnableQueue() 
	{
		clear();
	}
		
	typedef std::deque<T*> Queue;
	
	int _limit;
	Queue _queue;
	mutable Mutex _mutex;
};


//
// Synchronization Queue
//


template<class T>
class SyncQueue: public RunnableQueue<T>, public SyncContext
	// SyncQueue extends SyncContext to implement a synchronized FIFO
	// queue which receives T objects from any thread and synchronizes
	// them for safe consumption by the associated event loop.
{
public:
	SyncQueue(uv::Loop& loop, int limit = 2048, int timeout = 250) :
		RunnableQueue<T>(limit), SyncContext(loop), _timeout(timeout)
	{
	}	
	
	virtual void push(T* item)
		// Pushes an item onto the queue.
		// Pointers are now managed by the SyncQueue.		
	{
		RunnableQueue::push(item);
		SyncContext::send();
	}
	
	virtual void cancel()
	{
		RunnableQueue::cancel();
		SyncContext::close();
	}

protected:
	virtual ~SyncQueue() 
	{
	}	
	
	virtual void emit(T& item) = 0;
		// Emits a single synchronized item.

	virtual void run()
	{
		// Run for x timeout
		RunnableQueue::runFor(_timeout);
	}

	int _timeout;
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
	AsyncQueue(int limit = 2048) : RunnableQueue<T>(limit)
	{
		_thread.start(*this); //std::bind(&RunnableQueue<T>::run, this)
	}	

protected:
	virtual ~AsyncQueue() 
	{
	}
	
	virtual void emit(T& item) = 0;
		// Override this method to emit the async item.

	Thread _thread;
};


//
// Synchronized Callback
//

template<class T>
class SyncCallback: public SyncContext 
{
public:		
	std::function<void(T)> target;
		// Synchronized callback method

	void* opaque;
		// Client data pointer (unmanaged)

	SyncCallback(uv::Loop& loop, std::function<void(T)> target) : 
		SyncContext(loop), target(target), opaque(opaque)
	{
	}
	
protected:	
	virtual void run()
		// Calls the synchronized callback 
		// method from the event loop thread.
	{		
		assert(opaque);
		target(opaque);
	}	
};


} // namespace scy


#endif // SOURCEY_InterProcess_H


	
	/*
	virtual void cancel()
		// Cancels the queue.
		// No more items will be emitted after the current item.
	{
		Mutex::ScopedLock lock(_mutex);
		_cancelled = true;
	}
	
	virtual void cancelled() const
		// Return true when cancelled.
		// TODO:
	{
		Mutex::ScopedLock lock(_mutex);
		return _cancelled;
	}
	*/
		
	/*
	virtual void stop()
	{
		_thread.stop();
	}
	*/

	
	/*
	virtual void cancel()
	{
		traceL("SyncQueue", this) << "Canceling" << std::endl;
		clear();
		traceL("SyncQueue", this) << "Canceling: OK" << std::endl;
	}
	*/
	
	/*

private:
		//SyncContext* ctx = reinterpret_cast<SyncContext*>(ptr);
		//ctx->

	//void operator delete(void* ptr)
	void operator delete[](void* ptr)
		// Defer deletion as we need to wait for
		// async callbacks to return.
	{
		traceL("SyncContext") << "$$$$$$$$$$$$$ Delete Operator" << std::endl;
		SyncContext* ctx = reinterpret_cast<SyncContext*>(ptr);
		ctx->close();
		deleteLater<SyncContext>(ctx);
	}
	*/
	//uv_async_t* _handle;
	//bool _destroyed;

	// Note: Until uv_cancel works on windows we need 
	// to keep track of pending callbacks and deleay 
	// destruction untill all callbacks are received.
	//int pendingCallbacks;


	/*
		//_sw.stop();
		//Task(&loop, false, false),

	virtual void start()
	{
		traceL("SyncQueue", this) << "Starting" << std::endl;
		//Task::start();
	}

		//Task::cancel();
	*/
	
	/*
		//if (flush)
		//	SyncQueue::flush();
	virtual void flush()
		// Flushes all queued items.
	{
	}
	*/