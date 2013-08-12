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


class SyncContext: public uv::Handle
	// SyncContext enables any thread to  
	// communicate with the associated event
	// loop via synchronized callbacks.
	//
	// TODO: Cancelling when uv_cancel is available on win.
{
public:
	SyncContext(uv::Loop& loop) : 
		uv::Handle(&loop, new uv_async_t),
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

	virtual void run() = 0;
		// Performs synchronized actions.
		// This method will block the event loop.
	 
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
	RunnableQueue(int limit = 2048) :
		_limit(limit)
	{
	}	
	
	virtual void push(T* item)
		// Pushes an item onto the queue.
		// Item pointers are now managed by the RunnableQueue.		
	{
		Mutex::ScopedLock lock(_mutex);	
				
		while (static_cast<int>(_queue.size()) >= (_limit)) {
			traceL("RunnableQueue", this) << "Purging: " << _queue.size() << std::endl;
			delete _queue.front();
			_queue.pop_front();
		}
			
		//traceL("RunnableQueue", this) << "Pushing: " << item << ": " << _queue.size() << std::endl;
		_queue.push_back(item);
	}
	
	virtual void flush()
		// Flushes all outgoing items.
	{
		//traceL("RunnableQueue", this) << "Flushing" << std::endl;
		
		while (emitNext()) 
			scy::sleep(1);
	}
	
	virtual std::deque<T*> queue()
	{
		Mutex::ScopedLock lock(_mutex);
		return _queue;
	}
	
	virtual void emit(T& item) = 0;
		// Override this method to emit the item.
	
	virtual void run()
		// Called asynchronously to flush outgoing items.
		// This method blocks until cancel() is called.
	{
		while (!cancelled())
			scy::sleep(emitNext() ? 1 : 50);
	}
	
	virtual void run(int timeout)
		// Called asynchronously to flush outgoing items.
		// This methods blocks until the queue is empty or
		// the timeout expires.
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
			//traceL("RunnableQueue", this) << "Emitting: " << next << ": " << _queue.size() << std::endl;
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
		// Override this method to emit the synchronized item.

	virtual void run()
	{
		// Run until timeout
		RunnableQueue::run(_timeout);
	}

	int _timeout;
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
	AsyncQueue(int limit = 2048) :
		RunnableQueue<T>(limit)
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

	virtual void run()
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