///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
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
    _handle.reset<uv_async_t>();
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
