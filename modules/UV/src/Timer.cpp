//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2005 Sourcey
//
// Non-Commercial Use:
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// Commercial Use:
// Please contact mail@sourcey.com
//


#include "Sourcey/UV/Timer.h"

#include "Sourcey/Logger.h"
#include "assert.h"


using namespace std;


namespace Sourcey {
namespace UV {


Timer::Timer(LoopHandle* loop) :
	Interface(loop),
	_active(false),
	_count(0)
{
	Log("debug") << "[Timer:" << this << "] Creating" << endl;

    int r = uv_timer_init(_loop, &_handle);
    assert(r == 0);

    _handle.data = this;

    // Only increase the ref count of the
	// loop when the timer is active.
    uv_unref(_loop);
}


Timer::~Timer()
{
	Log("debug") << "[Timer:" << this << "] Destroying" << endl;

    if (!_active) 
		uv_ref(_loop);
    uv_close((Handle*)&_handle, UV::defaultClose);
}


bool Timer::start(Int64 timeout, Int64 interval) 
{
    int r = uv_timer_start(&_handle, UV::timerCallback, timeout, interval);
    if (r) setErrno(uv_last_error(_loop));
	updateState();
    return r == 0;
}


bool Timer::stop() 
{
    int r = uv_timer_stop(&_handle);
    if (r) setErrno(uv_last_error(_loop));
	updateState();
    return r == 0;
}


bool Timer::again() 
{
    int r = uv_timer_again(&_handle);
    if (r) setErrno(uv_last_error(_loop));
	updateState();
    return r == 0;
}


void Timer::setInterval(Int64 interval)
{
    uv_timer_set_repeat(&_handle, interval);
}


Int64 Timer::getInterval()
{	
    Int64 interval = uv_timer_get_repeat(&_handle);
    if (interval < 0) 
		setErrno(uv_last_error(_loop));
	return interval;
}


/*
void Timer::setErrno(const uv_err_t& err)
{
	Log("error") << "[Timer:" << this << "] Error: " << uv_strerror(err) << endl;	
}
*/


void Timer::updateState()
{
	bool wasActive = _active;
	_active = uv_is_active((Handle*) &_handle);

	if (!wasActive && _active) {
		// If our state is changing from inactive to active, we
		// increase the loop's reference count.
		uv_ref(_loop);
	} 
	else if (wasActive && !_active) {
		// If our state is changing from active to inactive, we
		// decrease the loop's reference count.
		uv_unref(_loop);
	}

	_count = 0;
}


Int64 Timer::count()
{
	return _count;
}



//
// Callbacks
//
void Timer::timerCallback(int status)
{
	_count++;
	OnTimeout.dispatch(this, _count);
}


} } // namespace Sourcey::UV
