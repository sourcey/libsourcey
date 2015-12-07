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


#ifndef SCY_SyncContext_H
#define SCY_SyncContext_H


#include "scy/uv/uvpp.h"
#include "scy/platform.h"
#include "scy/logger.h"
#include "scy/async.h"
#include "scy/interface.h"

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


#endif // SCY_SyncContext_H