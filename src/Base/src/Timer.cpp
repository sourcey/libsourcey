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


#include "Sourcey/Timer.h"
#include "Sourcey/Logger.h"
#include "assert.h"


using namespace std;


namespace scy {
	

Timer::Timer(uv::Loop& loop) : 
	Base(&loop, new uv_timer_t)
{
	traceL("Timer", this) << "Creating" << endl;
	init();
}
	

Timer::Timer(Int64 timeout, Int64 interval, uv::Loop& loop) : 
	Base(&loop, new uv_timer_t)
{
	traceL("Timer", this) << "Creating" << endl;	
	init();
	if (timeout || interval)
		start(timeout, interval);
}


Timer::~Timer()
{
	traceL("Timer", this) << "Destroying" << endl;
	close();
	//assert(_handle);
}


void Timer::init()
{
	_count = 0;
	_timeout = 0;
	_interval = 0;
			
	assert(_handle);
	_handle->data = this;
	int r = uv_timer_init(loop(), handle<uv_timer_t>());
	if (r)
		setAndThrowLastError("Invalid timer");

	// Timers do not reference the main loop.
    uv_unref(handle());
}


bool Timer::start(Int64 interval)
{
	return start(interval, interval);
}


bool Timer::start(Int64 timeout, Int64 interval) 
{
	traceL("Timer", this) << "Starting: " << _handle << ": " << timeout << ": " << interval << endl;
	assert(_count == 0);
	_timeout = timeout;
	_interval = interval;
    int r = uv_timer_start(handle<uv_timer_t>(), Timer::onTimeout, timeout, interval);
    if (r) 
		setAndThrowLastError("Invalid timer");
	else {
		//uv_ref(handle());
		//assert(active());
	}
    return r == 0;
}


bool Timer::stop() 
{
	traceL("Timer", this) << "Stoping: " << _handle << endl;
	
	if (!handle())
		return false;

    int r = uv_timer_stop(handle<uv_timer_t>());
    if (r) 
		setAndThrowLastError("Invalid timer");
	else {
		//uv_unref(handle());
		//assert(!active());
	}
	_count = 0;
	
	traceL("Timer", this) << "Closing: " << _handle << endl;
	close(); // Needs to be closed to deref loop

    return r == 0;
}


bool Timer::restart()
{
	traceL("Timer", this) << "Restarting: " << _handle << endl;
	if (!active())
		return start(_timeout, _interval);
	return again();
}


bool Timer::again() 
{
	traceL("Timer", this) << "Again: " << _handle << endl;
    int r = uv_timer_again(handle<uv_timer_t>());
    if (r) 
		setAndThrowLastError("Invalid timer");
	else {
		//if (!active())
		//	uv_ref(handle());
		//assert(active());
	}
	_count = 0;
    return r == 0;
}


void Timer::setInterval(Int64 interval)
{
	traceL("Timer", this) << "Set Interval: " << interval << endl;
    uv_timer_set_repeat(handle<uv_timer_t>(), interval);
}


bool Timer::active() const
{
	return uv_is_active((uv_handle_t*) handle<uv_timer_t>()) == 1;
}


Int64 Timer::timeout() const
{	
	return _timeout;
}


Int64 Timer::interval() const
{	
	return std::min<int>(uv_timer_get_repeat(handle<uv_timer_t>()), 0);
}


Int64 Timer::count()
{
	return _count;
}


void Timer::onTimeout()
{	
	traceL("Timer", this) << "On Timeout: " << _count << endl;
	_count++;
	assert(Timeout.refCount());
	Timeout.emit(this);
}


} // namespace scy::uv



/*
void Timer::close()
{
	traceL("Timer", this) << "Closing: " << _handle << endl;
	Base::close();
}
*/


/*
void Timer::updateState()
{
	_count = 0;
	bool wasActive = _active;
	_active = !!uv_is_active((uv_handle_t*) handle<uv_timer_t>());
		
	traceL("Timer", this) << "Update State: " << _active << endl;
	if (!wasActive && _active) {
		// If our state is changing from inactive to active, we
		// increase the loop's reference count.
		// TODO: reenable
		uv_ref(handle());
		uv_unref(handle());
	} 
	else if (wasActive && !_active) {
		// If our state is changing from active to inactive, we
		// decrease the loop's reference count.
		// TODO: reenable
		uv_unref(handle());
	}
}
*/