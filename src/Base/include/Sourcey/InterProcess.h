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


#ifndef SOURCEY_SyncQueue_H
#define SOURCEY_SyncQueue_H


#include "Sourcey/UV/UVPP.h"
#include "Sourcey/Memory.h"
#include "Sourcey/Platform.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Timer.h"
#include "Sourcey/Thread.h"

#include <deque>


namespace scy {
		
	
//
// Synchronization Context
//


class SyncContext: public uv::Base
	// SyncContext enables any thread to  
	// communicate with the associated event
	// loop via synchronized callbacks.
	//
	// TODO: Cancelling when uv_cancel is available on win.
{
public:
	SyncContext(uv::Loop& loop) : 
		uv::Base(&loop, new uv_async_t),
		_loop(loop)
	{
		traceL("SyncContext", this) << "Creating" << std::endl;
		_handle->data = this;
		uv_async_init(&loop, handle<uv_async_t>(), _callback);
	}
	
	virtual void destroy()
		// Defer deletion as we need to wait 
		// for async callbacks to return.
	{
		traceL("SyncContext") << "Destroy later" << std::endl;
		close();
		deleteLater<SyncContext>(this);
	}

	void send()
		// Called from any thread to send the 
		// synchronized callback.
	{
		assert(!closed());
		uv_async_send(handle<uv_async_t>());
	}
	
protected:	
	virtual ~SyncContext()
	{
		traceL("SyncContext", this) << "Destroying" << std::endl;
		assert(closed());
	}

	virtual bool run() = 0;
		// Performs synchronized actions.
	 
	static void _callback(uv_async_t* req, int)
	{
		SyncContext* self = reinterpret_cast<SyncContext*>(req->data);
		if (self->closed()) {
			warnL("SyncContext", self) << "Ignoring late callback" << std::endl;
			assert(0);
			return;
		}

		self->run();
	}

	uv::Loop& _loop;

	friend class GCDeleter<SyncContext>;
};


//
// Runnable Queue
//


template<class T>
class RunnableQueue: public abstract::Runnable
{
public:
	RunnableQueue(int maxSize = 2048, int dispatchTimeout = DEFAULT_TIMEOUT) :
		_maxSize(maxSize),
		_timeout(dispatchTimeout),
		_cancelled(false)
	{
	}	
	
	virtual void push(T* item)
		// Pushes an item onto the queue.
		// Item pointers are now managed by the RunnableQueue.		
	{
		{
			ScopedLock lock(_mutex);	
				
			while (static_cast<int>(_queue.size()) >= (_maxSize)) {
				traceL("RunnableQueue", this) << "Purging" << std::endl;
				delete _queue.front();
				_queue.pop_front();
				//assert(0 && "queue overflow"); // remove me
			}
			
			//traceL("RunnableQueue", this) << "Pushing: " << item << ": " << _queue.size() << std::endl;
			_queue.push_back(item);
		}
	}
	
	virtual void cancel()
		// Cancels the queue.
		// No more items will be emitted after the current item.
	{
		ScopedLock lock(_mutex);
		_cancelled = true;
	}
	
	virtual void flush()
		// Flushes all outgoing items.
	{
		//traceL("RunnableQueue", this) << "Flushing" << std::endl;
		
		T* item = nil;
		for(;;) 
		{
			{
				ScopedLock lock(_mutex);
				if (_queue.empty())
					break;

				item = _queue.front();
				_queue.pop_front();
			
				// traceL("RunnableQueue", this) << "Flushing: " << item << ": " << _queue.size() << std::endl;
			}
			
			emit(*item);
			delete item;
		}
	}
	
	virtual std::deque<T*> queue()
	{
		ScopedLock lock(_mutex);
		return _queue;
	}

protected:
	virtual ~RunnableQueue() 
	{
		clear();
	}
	
	virtual void clear()
		// Clears all currently queued items.
	{
		ScopedLock lock(_mutex);	
		traceL("RunnableQueue", this) << "Clearing: " << _queue.size() << std::endl;
		util::clearDeque(_queue);
	}
	
	virtual void emit(T& item) = 0;
		// Override this method to emit the outgoing item.
	
	virtual bool run()
		// Called asynchronously to flush and emit outgoing items.	
	{
		T* item = nil;
		_sw.start();
		for(;;) 
		{
			{
				ScopedLock lock(_mutex);
				if (_cancelled || _queue.empty() || _sw.elapsed() > _timeout) 			
					break;

				item = _queue.front();
				_queue.pop_front();
			}

			emit(*item);
			delete item;
		}
		_sw.reset();
		return !_cancelled;
	}
		
	typedef std::deque<T*> Queue;

	enum
	{
		DEFAULT_TIMEOUT = 250000 // 250ms
	};
	
	Queue _queue;
	Stopwatch _sw;
	int _maxSize;
	int	_timeout;
	bool _cancelled;	
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
	SyncQueue(uv::Loop& loop, int maxSize = 2048, int dispatchTimeout = DEFAULT_TIMEOUT) :
		RunnableQueue<T>(maxSize, dispatchTimeout), SyncContext(loop)
	{
	}	
	
	virtual void push(T* item)
		// Pushes an item onto the queue.
		// Pointers are now managed by the SyncQueue.		
	{
		RunnableQueue::push(item);
		SyncContext::send();
	}

protected:
	virtual ~SyncQueue() 
	{
	}	
	
	virtual void emit(T& item) = 0;
		// Override this method to emit the synchronized item.

	virtual bool run()
	{
		return RunnableQueue::run();
	}
};


//
// Asynchronous Queue
//


template<class T>
class AsyncQueue: public RunnableQueue<T>
	// AsyncQueue is a thread-based queue which flushes outgoing T packets
	// on each thread iteration.
	// This class is useful for deferring load to free up operation critical
	// system devices from the burden of CPU intensive or long running tasks.
{
public:
	AsyncQueue(int maxSize = 2048, int dispatchTimeout = DEFAULT_TIMEOUT) :
		RunnableQueue<T>(maxSize, dispatchTimeout)
	{
		// The thread will call the RunnableQueue's run 
		// method constantly to flush outgoing packets 
		// until cancel() is called. 
		_thread.start(*this);
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


class SyncCallback: public SyncContext 
{
public:		
	Callable target;
		// Synchronized callback method

	void* opaque;
		// Client data pointer (unmanaged)

	SyncCallback(uv::Loop& loop, Callable target = 0, void* opaque = 0) : 
		SyncContext(loop), target(target), opaque(opaque)
	{
	}
	
protected:	
	virtual ~SyncCallback()
	{
	}

	virtual bool run()
		// Calls the synchronized callback 
		// method from the event loop thread.
	{		
		assert(opaque);
		target(opaque);
	}	
};


} // namespace scy


#endif // SOURCEY_SyncQueue_H


		
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