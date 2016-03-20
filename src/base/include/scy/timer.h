//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// LibSourcey is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//


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


class Timer
    /// TODO: Should be Async, and uv::Handle a member
{
public:
    Timer(uv::Loop* loop = uv::defaultLoop());
    virtual ~Timer();

    virtual void start(std::int64_t interval);
    virtual void start(std::int64_t timeout, std::int64_t interval);
        // Starts the timer, an interval value of zero will only trigger
        // once after timeout.

    virtual void stop();
        // Stops the timer.

    virtual void restart();
        // Restarts the timer, even if it hasn't been started yet.
        // An interval or interval must be set or an exception will be thrown.

    virtual void again();
        // Stop the timer, and if it is repeating restart it using the
        // repeat value as the timeout. If the timer has never been started
        // before it returns -1 and sets the error to UV_EINVAL.

    virtual void setInterval(std::int64_t interval);
        // Set the repeat value. Note that if the repeat value is set from
        // a timer callback it does not immediately take effect. If the timer
        // was non-repeating before, it will have been stopped. If it was repeating,
        // then the old repeat value will have been used to schedule the next timeout.

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


#if 0
//
// Timer 2
//


class Timer2: public async::Runner
    /// TODO: Should be Async, and uv::Handle a member
{
public:
    Timer2(uv::Loop* loop = uv::defaultLoop());
    virtual ~Timer2();

    //virtual void start(std::int64_t interval);
    //virtual void start(std::int64_t timeout, std::int64_t interval);
        // Starts the timer, an interval value of zero will only trigger
        // once after timeout.

    virtual void stop();
        // Stops the timer.

    virtual void restart();
        // Restarts the timer, even if it hasn't been started yet.
        // An interval or interval must be set or an exception will be thrown.

    virtual void again();
        // Stop the timer, and if it is repeating restart it using the
        // repeat value as the timeout. If the timer has never been started
        // before it returns -1 and sets the error to UV_EINVAL.

    virtual void setInterval(std::int64_t interval);
        // Set the repeat value. Note that if the repeat value is set from
        // a timer callback it does not immediately take effect. If the timer
        // was non-repeating before, it will have been stopped. If it was repeating,
        // then the old repeat value will have been used to schedule the next timeout.

    virtual void unref();

    virtual bool active() const;

    virtual std::int64_t timeout() const;
    virtual std::int64_t interval() const;

    std::int64_t count();

    //NullSignal Timeout;

    uv::Handle ptr;

protected:
    virtual void init();

    std::int64_t _timeout;
    std::int64_t _interval;
    std::int64_t _count;
    bool _ghost;
};

#endif


} // namespace scy


#endif // SCY_Timer_H
