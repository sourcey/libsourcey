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


#ifndef SOURCEY_SyncContext_H
#define SOURCEY_SyncContext_H


#include "Sourcey/UV/UVPP.h"
#include "Sourcey/Platform.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Async.h"
#include "Sourcey/Interface.h"

#include <deque>


namespace scy {
	

//
// Synchronization Context
//


class SyncContext: public async::Runner
	/// SyncContext enables any thread to communicate with
	/// the associated event loop via synchronized callbacks.
{
public:
	SyncContext(uv::Loop* loop);
	SyncContext(uv::Loop* loop, std::function<void()> target);
	SyncContext(uv::Loop* loop, std::function<void(void*)> target, void* arg);
		// Create the synchronization context the given event loop and method.
		// The target method will be called from the event loop context.

	virtual ~SyncContext();

	void post();
		// Send a synchronization request to the event loop.
		// Call this each time you want the target method called synchronously.
		// The synchronous method will be called on next iteration.
		// This is not atomic, so do not expect a callback for every request.

	virtual void cancel();

	virtual void close();
	virtual bool closed();
	
	uv::Handle& handle();
	
protected:	
	virtual void startAsync();
	virtual bool async() const;

	uv::Handle _handle;
};

	
#if 0	

//
// Synchronization Delegate
//


template <class C, class BaseT, class CallbackT, DelegateDefaultArgs>
class SyncDelegate: public Delegate<C, BaseT, CallbackT, P, P2, P3, P4>
	// This template class implements an adapter that sits between
	// an DelegateBase and an object receiving notifications from it.
{
public:
	//typedef DelegateBase<P, P2, P3, P4> DerivedT;
	typedef typename CallbackT::Method Method;
	typedef typename BaseT::DataT DataT;

	struct Context 
	{
		void* sender;
		P arg;
		P2 arg2;
		P3 arg3;
		P4 arg4;
		Context(void* sender, P arg, P2 arg2, P3 arg3, P4 arg4) : 
			sender(sender), arg(arg), arg2(arg2), arg3(arg3), arg4(arg4) {}
	};

	SyncDelegate(C* object, Method method, uv::Loop* loop = uv::defaultLoop(), int priority = 0) : 
		Delegate(object, method, priority),
		_loop(loop),
		_sync(nullptr)
	{
	}

	SyncDelegate(C* object, Method method, DataT filter, uv::Loop* loop = uv::defaultLoop(), int priority = 0) :
		Delegate(object, method, filter, priority),
		_loop(loop),
		_sync(nullptr)
	{
	}

	virtual ~SyncDelegate() 
	{ 
		if (_sync)
			_sync->dispose();
	}

	SyncDelegate(const SyncDelegate& r) : 
		Delegate(r), 
		_loop(r._loop),
		_sync(nullptr)	
	{
	}
	
	BaseT* clone() const 
	{
		return new SyncDelegate(*this);
	}

	virtual void emit(void* sender, P arg, P2 arg2, P3 arg3, P4 arg4)
	{
		// Create the context on first emit
		if (!_sync) {
			_sync = new SyncQueue<Context>(_loop);
			_sync->ondispatch = std::bind(&SyncDelegate::emitSync, this, std::placeholders::_1);
		}
		_sync->push(new Context(sender, arg, arg2, arg3, arg4));
	}
		
	void emitSync(Context& ctx)
	{
		if (!_cancelled)
			CallbackT::emit(ctx.sender, ctx.arg, ctx.arg2, ctx.arg3, ctx.arg4);
	} 
	
protected:
	SyncDelegate() 
	{ 
	}

	uv::Loop& _loop;
	SyncQueue<Context>* _sync;
};


template <class C>
static SyncDelegate<C, 
	DelegateBase<>, 
	DelegateCallback<C, 0, true>
> syncDelegate(C* pObj, void (C::*Method)(void*), uv::Loop* loop = uv::defaultLoop(), int priority = 0) 
{
	return SyncDelegate<C,
		DelegateBase<>,
		DelegateCallback<C, 0, true>
	>(pObj, Method, loop, priority);
}


template <class C, typename P>
static SyncDelegate<C, 
	DelegateBase<P>, 
	DelegateCallback<C, 1, true, P>, P
> syncDelegate(C* pObj, void (C::*Method)(void*,P), uv::Loop* loop = uv::defaultLoop(), int priority = 0) 
{
	return SyncDelegate<C, 
		DelegateBase<P>, 
		DelegateCallback<C, 1, true, P>, P
	>(pObj, Method, loop, priority);
}

#endif


} // namespace scy


#endif // SOURCEY_SyncContext_H



	//virtual void stop();
	//virtual void restart();

	//virtual void close();
	//virtual bool closed();

	/*
	//virtual void dispose();
	
	//virtual void dispose();
	virtual SyncContext& syncContext()
	{	
		// SyncContext will always outlive the queue.
		Mutex::ScopedLock lock(RunnableQueue<T>::_mutex);
		return *_sync;
	}
		//while(_processing) {
		//	traceL("SyncQueue", this) << "Destroy: Waiting" << std::endl;
		//	sleep(10);
		//}
	//bool _processing;
	//SyncContext* _sync;
		//_processing = true;
		//_processing = false;
	*/
	
/*
template<class T = void*>
class Async//: public async::Runnable
{
public:
	struct Context
	{
		async::Runner* self;
		std::function<void(T)> target;
		T opaque;
	};

	Async(std::function<void(T)> target, T opaque)
	{
		start(target, opaque);
	}

	virtual void start(std::function<void(T)> target, T opaque)
	{	
		auto req = new Context;
		req->self = this;
		req->target = target;
		req->opaque = opaque;
		startAsync(req);
	}

protected:
	Async(const Async&); // = delete;
	Async(Async&&); // = delete;
	Async& operator=(const Async&); // = delete;
	Async& operator=(Async&&); // = delete;

	friend struct std::default_delete<async::Runner>;
		
	virtual void startAsync(Context* req) = 0;

	static void runAsync(std::shared_ptr<Runner::Context> context)
	{
		//async::Runner* self = nullptr;
		try {
			std::unique_handle<async::Runner::Context> req(reinterpret_cast<async::Runner::Context*>(arg));
			//self = req.get()->self;
			req.get()->target(req.get()->opaque);

			//self->_running = true;
			//if (req.get()->target)
			//	req.get()->target();
			//else 
			//	req.get()->target1(req.get()->opaque);
		}
		catch (std::exception& exc) {
			std::cerr << "Runner error: " << exc.what() << std::endl;
	#ifdef _DEBUG
			throw exc;
	#endif
		}
		//if (self)
		//	self->reset();
	}

	virtual ~Async()
	{
	}
};


//
// Synchronization Context
//


//template<class T>
class SyncContext: public Async<T>
	/// SyncContext enables any thread to communicate with
	/// the associated event loop via synchronized callbacks.
	///
	/// TODO: Cancelling when uv_cancel is available on win.
{
public:
	SyncContext(uv::Loop* loop) : 
		ptr(loop, new uv_async_t)
	{
	}

	SyncContext(uv::Loop* loop, std::function<void(T)> target, T opaque) : 
		ptr(loop, new uv_async_t)
	{
		start(target, opaque);
	}

	void post()
		// Can be called from any thread to send  
		// the synchronized callback.
	{
		assert(!ptr.closed());
		uv_async_send(handle.ptr<uv_async_t>());
	}

	void close()
	{
		ptr.close();
	}
	
protected:	
	virtual void startAsync(Context* req)
	{
		assert(!ptr.active());	

		handle.ptr()->data = req;
		int err = uv_async_init(ptr.loop(), handle.ptr<uv_async_t>(), [](uv_async_t* req, int) {
			runAsync(req->data);
		});

		if (err < 0) {
			delete req;
			ptr.setAndThrowError(err, "Cannot initialize synchronization contet");
		}
	}

	virtual ~SyncContext()
	{
		assert(ptr.closed());
	}

	uv::Handle ptr;
};

*/


 //async::Runner::Context* req

			//std::unique_handle<async::Runner::Context> ctx(reinterpret_cast<async::Runner::Context*>(req->data));
			//ctx.get()->self->
			//auto arg = reinterpret_cast<async::Runner::Context*>(req);
			//if (self)
			//self->reset();
			//arg->self->reset(); {}
			//delete req;

	//SyncContext(const SyncContext&); // = delete;
	//SyncContext(SyncContext&&); // = delete;
	//SyncContext& operator=(const SyncContext&); // = delete;
	//SyncContext& operator=(SyncContext&&); // = delete;

	//friend struct std::default_delete<SyncContext>;
	
	//virtual void dispatch(T& item) = 0;
		// Dispatch a synchronized object.
	
	//virtual void dispatch(T& item) = 0;
		// Dispatch a synchronized object.

		//auto req = new CallbackRef;
		//req->target = target;
		//req->self = this;
		//req->arg = arg;
			//auto req = reinterpret_cast<async::RunnerMethod*>(req->data);
			//req->target(req->arg);


		/*

	//virtual void run() = 0;
		// Called by the libuv event loop to 
		// perform synchronized actions.
		 //basic::Callable target, void* arg
	struct CallbackRef
	{
		basic::Callable target;
		Sync* self;
		void* arg;
	};
		handle.ptr()->data = &callback;
		uv_async_init(ptr.loop(), handle.ptr<uv_async_t>(), [](uv_async_t* req, int)
		{
			//auto self = reinterpret_cast<Sync*>(req->data);
			//target()

			auto self = reinterpret_cast<Sync*>(req->data);

			// The context pointer will always  
			// outlive the next libuv iteration.
			if (self->ptr.closed()) {
				warnL("SyncContext", self) << "Late callback" << std::endl;
				return;
			}

			self->run();
		});
		*/
	
		/*
		handle.ptr()->data = &callback;
		ptr._handle->data = this;
		uv_async_init(loop, handle<uv_async_t>(), [](uv_async_t* req, int)
		{
			auto self = reinterpret_cast<SyncContext*>(req->data);

			// The context pointer will always  
			// outlive the next libuv iteration.
			if (self->closed()) {
				warnL("SyncContext", self) << "Late callback" << std::endl;
				return;
			}

			self->run();
		});
		*/
	/*
	virtual void start(std::function<void()> target)
	{
	}


	virtual void start(std::function<void(void*)> target, void* opaque)
	{
		handle.ptr()->data = opaque;
		uv_async_init(ptr.loop(), handle.ptr<uv_async_t>(), [](uv_async_t* req, int)
		{
			//auto self = reinterpret_cast<Sync*>(req->data);
			//target()

			auto self = reinterpret_cast<Sync*>(req->data);

			// The context pointer will always  
			// outlive the next libuv iteration.
			if (self->ptr.closed()) {
				warnL("SyncContext", self) << "Late callback" << std::endl;
				return;
			}

			self->target();
		});
	}
	*/
	




/*
class SyncContext: public uv::Handle
	/// SyncContext enables any thread to communicate with
	/// the associated event loop via synchronized callbacks.
	///
	/// TODO: Cancelling when uv_cancel is available on win.
{
public:
	SyncContext(uv::Loop* loop) : 
		uv::Handle(loop, new uv_async_t)
	{
		_handle->data = this;
		uv_async_init(loop, handle<uv_async_t>(), [](uv_async_t* req, int)
		{
			auto self = reinterpret_cast<SyncContext*>(req->data);

			// The context pointer will always  
			// outlive the next libuv iteration.
			if (self->closed()) {
				warnL("SyncContext", self) << "Late callback" << std::endl;
				return;
			}

			self->run();
		});
	}
	
	virtual void dispose()
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

	friend struct std::default_delete<SyncContext>;
};
*/


/*
		//_thread.start(std::bind(&AsyncQueue::run, this));
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

	SyncCallback(uv::Loop* loop, std::function<void(T)> target) : 
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
*/
	
	/*
	virtual void cancel()
		// Cancels the queue.
		// No more items will be dispatched after the current item.
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
		//Task(loop, false, false),

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