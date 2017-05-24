///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup base
/// @{


#include "scy/idler.h"
#include "scy/logger.h"


using std::endl;


namespace scy {


Idler::Idler(uv::Loop* loop)
    : _handle(loop, new uv_idle_t)
{
    init();
}


Idler::~Idler()
{
}


void Idler::init()
{
    uv_idle_init(_handle.loop(), _handle.ptr<uv_idle_t>());
    _handle.init();
    _handle.unref(); // unref by default
}


void Idler::start(std::function<void()> target)
{
    start<std::function<void()>>(std::forward<std::function<void()>>(target));
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
