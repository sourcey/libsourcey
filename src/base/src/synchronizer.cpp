///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:    LGPL-2.1+
//
/// @addtogroup base
/// @{


#include "scy/synchronizer.h"


namespace scy {


Synchronizer::Synchronizer(uv::Loop* loop)
    : _handle(loop, new uv_async_t)
{
}


Synchronizer::Synchronizer(std::function<void()> target, uv::Loop* loop)
    : _handle(loop, new uv_async_t)
{
    start<std::function<void()>>(std::forward<std::function<void()>>(target));
}


Synchronizer::~Synchronizer()
{
    close();
}


void Synchronizer::start(std::function<void()> target)
{
    start<std::function<void()>>(std::forward<std::function<void()>>(target));
}


void Synchronizer::post()
{
    assert(!_handle.closed());
    uv_async_send(_handle.ptr<uv_async_t>());
}


// void Synchronizer::startAsync() // FIXME
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


void Synchronizer::cancel()
{
    Runner::cancel();
}


void Synchronizer::close()
{
    if (closed())
        return;
    cancel();
    post(); // post to wake up event loop
    _handle.close();
}


bool Synchronizer::closed()
{
    return _handle.closed();
}


bool Synchronizer::async() const
{
    return false;
}


uv::Handle& Synchronizer::handle()
{
    return _handle;
}


} // namespace scy


/// @\}
