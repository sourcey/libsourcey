///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_SyncContext_H
#define SCY_SyncContext_H


#include "scy/interface.h"
#include "scy/interface.h"
#include "scy/logger.h"
#include "scy/platform.h"
#include "scy/uv/uvpp.h"

#include <deque>


namespace scy {


//
// Synchronization Context
//

/// SyncContext enables any thread to communicate with
/// the associated event loop via synchronized callbacks.
class SyncContext : public Runner
{
public:
    /// Create the synchronization context the given event loop and method.
    /// The target method will be called from the event loop context.
    SyncContext(uv::Loop* loop);

    SyncContext(std::function<void()> target,
                uv::Loop* loop = uv::defaultLoop());

    template<class Function, class... Args>
    explicit SyncContext(Function func, Args... args,
                         uv::Loop* loop = uv::defaultLoop())
        : _handle(loop, new uv_async_t)
    {
        start(func, args...);
    }

    virtual ~SyncContext();

    /// Send a synchronization request to the event loop.
    /// Call this each time you want the target method called synchronously.
    /// The synchronous method will be called on next iteration.
    /// This is not atomic, so do not expect a callback for every request.
    void post();

    template<class Function, class... Args>
    void start(Function func, Args... args)
    {
        assert(!_handle.active());
        assert(!_handle.active());
        assert(!_context->running);

        _context->reset();
        _context->running = true;
        _context->repeating = true; // always repeating

        // Use a FunctionWrap instance since we can't pass the capture lambda
        // to the libuv callback without compiler trickery.
        _handle.ptr()->data = new FunctionWrap<Function, Args...>(func, args..., _context);
        int r = uv_async_init(_handle.loop(), _handle.ptr<uv_async_t>(), [](uv_async_t* req) {
            auto wrap = reinterpret_cast<FunctionWrap<Function, Args...>*>(req->data);
            if (!wrap->ctx->cancelled) {
                wrap->call();
            }
            else {
                req->data = nullptr;
                wrap->ctx->running = false;
                delete wrap;
            }
        });

        if (r < 0)
            _handle.setAndThrowError("Cannot initialize async", r);

        assert(_handle.active());
    }

    virtual void start(std::function<void()> target);

    virtual void cancel();

    virtual void close();
    virtual bool closed();

    uv::Handle& handle();

protected:
    // virtual void startAsync();
    virtual bool async() const;

    uv::Handle _handle;
};


#if 0

//
// Synchronization Delegate
//

/// This template class implements an adapter that sits between
/// an DelegateBase and an object receiving notifications from it.
template <class C, class BaseT, class CallbackT, DelegateDefaultArgs>
class SyncDelegate: public Delegate<C, BaseT, CallbackT, P, P2, P3, P4>
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


#endif // SCY_SyncContext_H


/// @\}
