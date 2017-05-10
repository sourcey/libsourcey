///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup base
/// @{
//


#include "scy/util/timeout.h"
#include "scy/util.h"


namespace scy {


//
// Timeout
//


Timeout::Timeout(long delay, bool autoStart)
    : _startAt(0)
    , _delay(delay)
{
    if (autoStart)
        start();
}


Timeout::Timeout(const Timeout& src)
    : _startAt(src._startAt)
    , _delay(src._delay)
{
}


Timeout& Timeout::operator=(const Timeout& src)
{
    _startAt = src._startAt;
    _delay = src._delay;
    return *this;
}


Timeout::~Timeout()
{
}


bool Timeout::running() const
{
    return _startAt != 0;
}


void Timeout::start()
{
    //_startAt = time::ticks();
    _startAt = clock();
}


void Timeout::stop()
{
    _startAt = 0;
}


void Timeout::reset()
{
    //_startAt = time::ticks();
    _startAt = clock();
}


long Timeout::remaining() const
{
    // time_t current = time::ticks();
    clock_t current = clock();
    long remaining = static_cast<long>(_delay - (current - _startAt));
    return remaining > 0 ? remaining : 0;
}


bool Timeout::expired() const
{
    if (_delay == 0) // _startAt == 0 ||
        return false;

    return remaining() == 0;
}


//
// Timed Token
//


TimedToken::TimedToken(long duration)
    : Timeout(duration)
    , _id(util::randomString(32))
{
}


TimedToken::TimedToken(const std::string& id, long duration)
    : Timeout(duration)
    , _id(id)
{
}


} // namespace scy


/// @\}