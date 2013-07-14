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
#include "Sourcey/Logger.h"
#include "Poco/Stopwatch.h"

#include <deque>


namespace scy {


// ----------------------------------------------------------------------------
//
class SyncContext: public uv::Base
	/// SyncContext enables any thread to  
	/// communicate with the associated libuv loop via
	/// synchronized callbacks.
	///
	/// TODO:
	///	  - Cancelling when uv_cancel is available on windows.
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
		/// Defer deletion as we need to wait 
		/// for async callbacks to return.
	{
		traceL("SyncContext") << "Destroy later" << std::endl;
		close();
		deleteLater<SyncContext>(this);
	}

	void send()
		/// Called from any thread to send the 
		/// synchronized callback.
	{
		traceL("SyncContext", this) << "Sending" << std::endl;
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
		/// Preforms synchronized actions.
	 
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


// ----------------------------------------------------------------------------
//
class AsyncCallback: public SyncContext 
{
public:
	AsyncCallback(uv::Loop& loop) : 
		SyncContext(loop), opaque(NULL)
	{
	}
		
	typedef void (*Callback)(void*);	
	Callback callback;
		/// Synchronized callback method

	void* opaque;
		/// Client data pointer (unmanaged)
	
protected:	
	virtual ~AsyncCallback()
	{
	}

	virtual void run()
		/// Calls the synchronized callback 
		/// method from the uv::Loop thread.
	{		
		assert(opaque);
		callback(opaque);
	}	
};


// ----------------------------------------------------------------------------
//
template<class T>
class SyncQueue: public SyncContext
	/// SyncQueue extends SyncContext to implement a synchronized FIFO
	/// queue which receives T objects from any thread and synchronizes
	/// them for safe consumption by the associated uv_loop context.
{
public:
	SyncQueue(uv::Loop& loop, int maxSize = 2048, int dispatchTimeout = DEFAULT_TIMEOUT) :
		SyncContext(loop),
		_maxSize(maxSize),
		_timeout(dispatchTimeout)
	{
		traceL("SyncQueue", this) << "Creating" << std::endl;
	}	
		
	virtual void destroy()
	{
		traceL("SyncQueue", this) << "Destroy later" << std::endl;

		clear();
		SyncContext::destroy();
	}
	
	virtual void push(T* item, bool flush = true)
		/// Pushes an item onto the queue.
		/// Item pointers are now managed by the SyncQueue.		
	{
		traceL("SyncQueue", this) << "Adding: " << item << std::endl;
		{
			Mutex::ScopedLock lock(_mutex);	
				
			while (static_cast<int>(_queue.size()) >= (_maxSize)) {
				traceL("SyncQueue", this) << "Purging item" << std::endl;
				delete _queue.front();
				_queue.pop_front();
				assert(0 && "queue overflow"); // remove me
			}

			_queue.push_back(item);
		}		
		SyncContext::send();
	}
	
	virtual void clear()
		// Clears all queued items.
	{
		Mutex::ScopedLock lock(_mutex);	
		util::clearDeque(_queue);
	}

protected:
	virtual ~SyncQueue() 
	{
		traceL("SyncQueue", this) << "Destroying" << std::endl;
		assert(closed());
		assert(_queue.empty());
	}
	
	virtual void emit(T& item) = 0;
		/// Override this method to emit the synchronized item
		/// to the application.
	
	virtual void run()
		/// Called by the uv::Loop thread to flush pending 
		/// synchronized items to the application.	
	{
		T* item = 0;
		_stopwatch.start();
		for(;;) 
		{
			{
				Mutex::ScopedLock lock(_mutex);
				if (_queue.empty() || _stopwatch.elapsed() > _timeout)
					break;

				item = _queue.front();
				_queue.pop_front();
			}
			
			emit(*item);
			delete item;
		}
		_stopwatch.reset();
	}
		
	typedef std::deque<T*> Queue;

	enum
	{
		DEFAULT_TIMEOUT = 250000
	};
	
	Queue _queue;
	int _maxSize;
	int	_timeout;
	Poco::Stopwatch	_stopwatch;	
	mutable Mutex	_mutex;
};


} // namespace scy


#endif // SOURCEY_SyncQueue_H



	
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
		/// Defer deletion as we need to wait for
		/// async callbacks to return.
	{
		traceL("SyncContext") << "$$$$$$$$$$$$$ Delete Operator" << std::endl;
		SyncContext* ctx = reinterpret_cast<SyncContext*>(ptr);
		ctx->close();
		deleteLater<SyncContext>(ctx);
	}
	*/
	//uv_async_t* _handle;
	//bool _destroyed;

	// NOTE: Until uv_cancel works on windows we need 
	// to keep track of pending callbacks and deleay 
	// destruction untill all callbacks are received.
	//int pendingCallbacks;


	/*
		//_stopwatch.stop();
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