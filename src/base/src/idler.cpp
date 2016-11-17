///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup base
/// @{

#include "scy/idler.h"
#include "scy/logger.h"

using std::endl;

namespace scy {

Idler::Idler(uv::Loop* loop)
    : _handle(loop, new uv_async_t)
{
    init();
}

Idler::Idler(uv::Loop* loop, std::function<void()> target)
    : _handle(loop, new uv_async_t)
{
    init();
    start(target);
}

Idler::Idler(uv::Loop* loop, std::function<void(void*)> target, void* arg)
    : _handle(loop, new uv_async_t)
{
    init();
    start(target, arg);
}

Idler::~Idler()
{
    // assert(_handle.closed()); // must be dispose()d
}

void Idler::init()
{
    pContext->repeating = true;
    pContext->handle = _handle.ptr<uv_idle_t>();
    uv_idle_init(_handle.loop(), _handle.ptr<uv_idle_t>());
    _handle.unref(); // unref by default
}

void Idler::startAsync()
{
    assert(!_handle.closed()); // close() must not have been called

    _handle.ptr()->data = new async::Runner::Context::ptr(pContext);
    int r = uv_idle_start(_handle.ptr<uv_idle_t>(), [](uv_idle_t* req) {
        auto ctx = reinterpret_cast<async::Runner::Context::ptr*>(req->data);
        runAsync(ctx->get());
        if (ctx->get()->handle && ctx->get()->cancelled()) {
            uv_idle_stop(reinterpret_cast<uv_idle_t*>(ctx->get()->handle));
            delete ctx; // delete the context and free memory
        }
        // scy::sleep(1); // prevent 100% idle CPU
        // TODO: uv_thread_yield when available
    });

    if (r < 0)
        _handle.setAndThrowError("Cannot initialize idler", r);
}

uv::Handle& Idler::handle()
{
    return _handle;
}

bool Idler::async() const
{
    return false;
}

} // namespace scy

/// @\}
