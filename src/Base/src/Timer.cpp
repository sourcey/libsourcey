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
#include "Sourcey/Platform.h"
#include "assert.h"


using namespace std;


namespace scy {
	

Timer::Timer(uv::Loop& loop, bool ghost) : 
	uv::Handle(&loop, new uv_timer_t), _ghost(ghost)
{
	//traceL("Timer", this) << "Create" << endl;
	init();
}


Timer::~Timer()
{
	//traceL("Timer", this) << "Destroy" << endl;
}


void Timer::init()
{
	//traceL("Timer", this) << "Init" << endl;	

	_count = 0;
	_timeout = 0;
	_interval = 0;

	assert(_handle);
	_handle->data = this;
			
	if (uv_timer_init(loop(), handle<uv_timer_t>()))
		setAndThrowLastError("Cannot initialize timer");

	// Ghost timers do not reference the main loop
	if (_ghost)
		uv_unref(handle());
}


void Timer::start(Int64 interval)
{
	start(interval, interval);
}


void Timer::start(Int64 timeout, Int64 interval) 
{
	//traceL("Timer", this) << "Starting: " << << timeout << ": " << interval << endl;
	assert(_handle);
	assert(timeout > 0);

	_timeout = timeout;
	_interval = interval;
	_count = 0;

    if (uv_timer_start(handle<uv_timer_t>(), [](uv_timer_t* req, int) {
		auto self = reinterpret_cast<Timer*>(req->data);
		self->_count++;
		self->Timeout.emit(self);
	}, timeout, interval))
		setAndThrowLastError("Invalid timer");
	assert(active());
}


void Timer::stop() 
{
	//traceL("Timer", this) << "Stopping: " << _handle << endl;
	
	if (!active())
		return; // do nothing
	
	_count = 0;
    if (uv_timer_stop(handle<uv_timer_t>())) 
		setAndThrowLastError("Invalid timer");
	assert(!active());	
}


void Timer::restart()
{
	//traceL("Timer", this) << "Restarting: " << _handle << endl;
	if (!active())
		return start(_timeout, _interval);
	return again();
}


void Timer::again() 
{
	//traceL("Timer", this) << "Again: " << _handle << endl;

	assert(handle());
    if (uv_timer_again(handle<uv_timer_t>())) 
		setAndThrowLastError("Invalid timer");
	assert(active());
	_count = 0;
}


void Timer::setInterval(Int64 interval)
{
	//traceL("Timer", this) << "Set interval: " << interval << endl;
	
	assert(handle());
    uv_timer_set_repeat(handle<uv_timer_t>(), interval);
}


bool Timer::active() const
{
	return handle() && uv_is_active(handle()) == 1;
}


Int64 Timer::timeout() const
{	
	return _timeout;
}


Int64 Timer::interval() const
{	
	assert(handle());
	return std::min<Int64>(uv_timer_get_repeat(handle<uv_timer_t>()), 0);
}


Int64 Timer::count()
{
	return _count;
}


} // namespace scy





	

/*
Timer::Timer(Int64 timeout, Int64 interval, uv::Loop& loop, bool ghost) : 
	uv::Handle(&loop, new uv_timer_t), _ghost(ghost)
{
	//traceL("Timer", this) << "Create" << endl;	
	init();
	//if (timeout || interval)
	//	start(timeout, interval);
}
*/
/*
void Timer::onTimeout()
{	
	//traceL("Timer", this) << "On timeout: " << _count << endl;
}
*/
/*
void Timer::close()
{
	//traceL("Timer", this) << "Closing: " << _handle << endl;
	Base::close();
}
*/


/*
void Timer::updateState()
{
	_count = 0;
	bool wasActive = _active;
	_active = !!uv_is_active((uv_handle_t*) handle<uv_timer_t>());
		
	//traceL("Timer", this) << "Update State: " << _active << endl;
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