///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup base
/// @{


#include "scy/timer.h"
#include "assert.h"
#include "scy/logger.h"
#include "scy/platform.h"


using std::endl;


namespace scy {


Timer::Timer(uv::Loop* loop)
    : _handle(loop, new uv_timer_t)
    , _timeout(0)
    , _interval(0)
    , _count(0)
{
    // TraceS(this) << "Create" << endl;
    init();
}


Timer::Timer(std::int64_t timeout, uv::Loop* loop)
    : _handle(loop, new uv_timer_t)
    , _timeout(timeout)
    , _interval(0)
    , _count(0)
{
    // TraceS(this) << "Create" << endl;
    init();
}


Timer::Timer(std::int64_t timeout, std::int64_t interval, uv::Loop* loop)
    : _handle(loop, new uv_timer_t)
    , _timeout(timeout)
    , _interval(interval)
    , _count(0)
{
    // TraceS(this) << "Create" << endl;
    init();
}


Timer::~Timer()
{
    // TraceS(this) << "Destroy" << endl;
}


void Timer::init()
{
    // TraceS(this) << "Init" << endl;

    // _count = 0;
    // _timeout = 0;
    // _interval = 0;

    assert(_handle.ptr());
    _handle.ptr()->data = this;

    int err = uv_timer_init(_handle.loop(), _handle.ptr<uv_timer_t>());
    if (err < 0)
        _handle.setAndThrowError("Cannot initialize timer", err);

    _handle.unref(); // unref by default
}


// void Timer::start(std::int64_t interval)
// {
//     start(interval, interval);
// }


void Timer::start(std::function<void()> func)
{
    start();
    Timeout += func;
}


void Timer::start() //std::int64_t timeout, std::int64_t interval)
{
    // TraceS(this) << "Starting: " << << timeout << ": " << interval << endl;
    assert(!active());
    assert(_handle.ptr());
    assert(_timeout > 0 || _interval > 0);

    // _timeout = timeout;
    // _interval = interval;
    _count = 0;

    int err = uv_timer_start(_handle.ptr<uv_timer_t>(), [](uv_timer_t* req) {
        auto self = reinterpret_cast<Timer*>(req->data);
        self->_count++;
        self->Timeout.emit();
    }, _timeout, _interval);
    if (err < 0)
        _handle.setAndThrowError("Invalid timer", err);

    assert(active());
}


void Timer::stop()
{
    // TraceS(this) << "Stopping: " << __handle.ptr << endl;

    if (!active())
        return; // do nothing

    _count = 0;
    int err = uv_timer_stop(_handle.ptr<uv_timer_t>());
    if (err < 0)
        _handle.setAndThrowError("Invalid timer", err);

    assert(!active());
}


void Timer::restart()
{
    // TraceS(this) << "Restarting: " << __handle.ptr << endl;
    if (!active())
        return start(); //_timeout, _interval);
    return again();
}


void Timer::again()
{
    // TraceS(this) << "Again: " << __handle.ptr << endl;

    assert(_handle.ptr());
    int err = uv_timer_again(_handle.ptr<uv_timer_t>());
    if (err < 0)
        _handle.setAndThrowError("Invalid timer", err);

    // assert(active());
    _count = 0;
}


void Timer::setTimeout(std::int64_t timeout)
{
    assert(!active());
    _timeout = timeout;
}


void Timer::setInterval(std::int64_t interval)
{
    assert(_handle.ptr());
    uv_timer_set_repeat(_handle.ptr<uv_timer_t>(), interval);
}


bool Timer::active() const
{
    return _handle.active();
}


std::int64_t Timer::timeout() const
{
    return _timeout;
}


std::int64_t Timer::interval() const
{
    assert(_handle.ptr());
    return std::min<std::int64_t>(
        uv_timer_get_repeat(_handle.ptr<uv_timer_t>()), 0);
}


std::int64_t Timer::count()
{
    return _count;
}


bool Timer::async() const
{
    return false;
}


uv::Handle& Timer::handle()
{
    return _handle;
}


} // namespace scy


/// @\}
