///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_Idler_H
#define SCY_Idler_H


#include "scy/base.h"
#include "scy/runner.h"
#include "scy/uv/uvpp.h"

#include <functional>


namespace scy {


/// Asynchronous type that triggers callbacks when the event loop is idle.
///
/// This class inherits the `Runner` interface and may be used with any
/// implementation that's powered by an asynchronous `Runner`.
///
class SCY_EXTERN Idler : public Runner
{
public:
    /// Create the idler with the given event loop.
    Idler(uv::Loop* loop = uv::defaultLoop());

    /// Create and start the idler with the given callback.
    template<class Function, class... Args>
    explicit Idler(Function&& func, Args&&... args)
        : _handle(uv::defaultLoop(), new uv_idle_t)
    {
        init();
        start(std::forward<Function>(func), std::forward<Args>(args)...);
    }

    /// Create and start the idler with the given callback and event loop.
    template<class Function, class... Args>
    explicit Idler(uv::Loop* loop, Function&& func, Args&&... args)
        : _handle(loop, new uv_idle_t)
    {
        init();
        start(std::forward<Function>(func), std::forward<Args>(args)...);
    }

    /// Start the idler with the given callback function.
    template<class Function, class... Args>
    void start(Function&& func, Args&&... args)
    {
        typedef internal::FunctionWrap<Function, Args...> FunctionWrap;

        assert(!_handle.active());
        assert(!_handle.active());
        assert(!_context->running);

        _context->reset();
        _context->running = true;
        _context->repeating = true; // idler is always repeating

        // Use a FunctionWrap instance since we can't pass the capture lambda
        // to the libuv callback without compiler trickery.
        _handle.ptr()->data = new FunctionWrap(std::forward<Function>(func),
                                               std::forward<Args>(args)...,
                                               _context);
        int r = uv_idle_start(_handle.ptr<uv_idle_t>(), [](uv_idle_t* req) {
            auto wrap = reinterpret_cast<FunctionWrap*>(req->data);
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

    /// Start the idler with the given callback function.
    virtual void start(std::function<void()> func);

    virtual ~Idler();

    uv::Handle& handle();

protected:
    virtual void init();

    virtual bool async() const;

    uv::Handle _handle;
};


} // namespace scy


#endif // SCY_Idler_H


/// @\}
