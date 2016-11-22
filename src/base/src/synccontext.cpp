///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup base
/// @{


#include "scy/synccontext.h"


namespace scy {


SyncContext::SyncContext(uv::Loop* loop)
    : _handle(loop, new uv_async_t)
{
}


SyncContext::SyncContext(std::function<void()> target, uv::Loop* loop)
    : _handle(loop, new uv_async_t)
{
    start(target);
}


// SyncContext::SyncContext(uv::Loop* loop, std::function<void(void*)> target,
//                          void* arg)
//     : _handle(loop, new uv_async_t)
// {
//     start(target, arg);
// }


SyncContext::~SyncContext()
{
    // assert(_handle.closed()); // must be dispose()d
    close();
}


void SyncContext::start(std::function<void()> target)
{
    start(target);
}


void SyncContext::post()
{
    assert(!_handle.closed());
    uv_async_send(_handle.ptr<uv_async_t>());
}


// void SyncContext::startAsync() // FIXME
// {
//     // assert(!_handle.active()); // active() can be unreliable when called
//     // inside thread
//
//     _handle.ptr()->data = new Runner::Context::ptr(pContext);
//     int r = uv_async_init(
//         _handle.loop(), _handle.ptr<uv_async_t>(), [](uv_async_t* req) {
//             assert(req->data != nullptr); // catch late callbacks, may need to
//                                           // make uv handle a context member
//             auto ctx =
//                 reinterpret_cast<Runner::Context::ptr*>(req->data);
//             if (ctx->get()->cancelled()) {
//                 delete ctx; // delete the context and free memory
//                 req->data = nullptr;
//                 return;
//             }
//
//             runAsync(ctx->get());
//         });
//
//     if (r < 0)
//         _handle.setAndThrowError("Cannot initialize async", r);
// }


void SyncContext::cancel()
{
    Runner::cancel();
}


void SyncContext::close()
{
    if (closed())
        return;
    cancel();
    post(); // post to wake up event loop
    _handle.close();
}


bool SyncContext::closed()
{
    return _handle.closed();
}


bool SyncContext::async() const
{
    return false;
}


uv::Handle& SyncContext::handle()
{
    return _handle;
}


} // namespace scy


/// @\}
