///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_Idler_H
#define SCY_Idler_H


#include "scy/runner.h"
#include "scy/uv/uvpp.h"

#include <functional>


namespace scy {


class Idler : public /*basic::*/Runner
{
public:
    /// Create the Idler context the given event loop and method.
    Idler(uv::Loop* loop = uv::defaultLoop());

    template<class Function, class... Args>
    explicit Idler(Function func, Args... args,
                    uv::Loop* loop = uv::defaultLoop())
        : _handle(loop, new uv_idle_t)
    {
        init();
        start(func, args...);
    }

    template<class Function, class... Args>
    void start(Function func, Args... args)
    {
        assert(!_handle.active());
        assert(!_handle.active());
        assert(!_context->running);

        _context->reset();
        _context->running = true;
        _context->repeating = true; // idler is always repeating

        // Use a FunctionWrap instance since we can't pass the capture lambda
        // to the libuv callback without compiler trickery.
        _handle.ptr()->data = new FunctionWrap<Function, Args...>(func, args..., _context);
        int r = uv_idle_start(_handle.ptr<uv_idle_t>(), [](uv_idle_t* req) {
            auto wrap = reinterpret_cast<FunctionWrap<Function, Args...>*>(req->data);
            if (!wrap->ctx->cancelled) {
                wrap->call();
            }
            else {
                wrap->ctx->running = false;
                uv_idle_stop(req);
                delete wrap;
            }
        });

        if (r < 0)
            _handle.setAndThrowError("Cannot initialize idler", r);

        assert(_handle.active());
    }

    /// Start a `Runnable` target.
    ///
    /// The target `Runnable` instance must outlive the idler.
    virtual void start(std::function<void()> target);

    virtual ~Idler();

    uv::Handle& handle();

protected:
    virtual void init();

    virtual bool async() const;

    uv::Handle _handle;
};


// class Idler : public Runner
// {
// public:
//     /// Create the idler context the given event loop and method.
//     Idler(uv::Loop* loop = uv::defaultLoop());
//     Idler(std::function<void()> target, uv::Loop* loop = uv::defaultLoop());
//     Idler(std::function<void(void*)> target, void* arg, uv::Loop* loop = uv::defaultLoop());
//
//     virtual ~Idler();
//
//     uv::Handle& handle();
//
// protected:
//     virtual void init();
//     virtual void startAsync();
//     virtual bool async() const;
//
//     uv::Handle _handle;
// };


} // namespace scy


#endif // SCY_Idler_H


/// @\}
