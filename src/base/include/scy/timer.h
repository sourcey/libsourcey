///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_Timer_H
#define SCY_Timer_H


#include "scy/uv/uvpp.h"
#include "scy/datetime.h"
#include "scy/signal.h"
#include "scy/memory.h"
#include "scy/async.h"

#include <cstdint>
#include <functional>


namespace scy {


// TODO: Should be Async, and uv::Handle a member
class Timer
{
public:
    Timer(uv::Loop* loop = uv::defaultLoop());
    virtual ~Timer();

    virtual void start(std::int64_t interval);

    /// Starts the timer, an interval value of zero will only trigger
    /// once after timeout.
    virtual void start(std::int64_t timeout, std::int64_t interval);

    /// Stops the timer.
    virtual void stop();

    /// Restarts the timer, even if it hasn't been started yet.
    /// An interval or interval must be set or an exception will be thrown.
    virtual void restart();

    /// Stop the timer, and if it is repeating restart it using the
    /// repeat value as the timeout. If the timer has never been started
    /// before it returns -1 and sets the error to UV_EINVAL.
    virtual void again();

    /// Set the repeat value. Note that if the repeat value is set from
    /// a timer callback it does not immediately take effect. If the timer
    /// was non-repeating before, it will have been stopped. If it was repeating,
    /// then the old repeat value will have been used to schedule the next timeout.
    virtual void setInterval(std::int64_t interval);

    bool active() const;

    std::int64_t timeout() const;
    std::int64_t interval() const;
    std::int64_t count();

    uv::Handle& handle();

    NullSignal Timeout;

protected:
    Timer(const Timer&);
    Timer& operator = (const Timer&);

    virtual void init();

    uv::Handle _handle;
    std::int64_t _timeout;
    std::int64_t _interval;
    std::int64_t _count;
};


} // namespace scy


#endif // SCY_Timer_H

/// @\}
