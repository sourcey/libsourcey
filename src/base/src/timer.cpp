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


#include "scy/timer.h"
#include "scy/logger.h"
#include "scy/platform.h"
#include "assert.h"


using std::endl;


namespace scy {
	

Timer::Timer(uv::Loop* loop) : 
	_handle(loop, new uv_timer_t)
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

	assert(_handle.ptr());
	_handle.ptr()->data = this;
			
    int err = uv_timer_init(_handle.loop(), _handle.ptr<uv_timer_t>());
    if (err < 0)
		_handle.setAndThrowError("Cannot initialize timer", err);
	
	_handle.unref(); // unref by default
}


void Timer::start(Int64 interval)
{
	start(interval, interval);
}


void Timer::start(Int64 timeout, Int64 interval) 
{
	//traceL("Timer", this) << "Starting: " << << timeout << ": " << interval << endl;
	assert(_handle.ptr());
	assert(timeout > 0);

	_timeout = timeout;
	_interval = interval;
	_count = 0;
				
    int err = uv_timer_start(_handle.ptr<uv_timer_t>(), [](uv_timer_t* req, int) {
		auto self = reinterpret_cast<Timer*>(req->data);
		self->_count++;
		self->Timeout.emit(self);
	}, timeout, interval);
    if (err < 0)
		_handle.setAndThrowError("Invalid timer", err);
	assert(active());
}


void Timer::stop() 
{
	//traceL("Timer", this) << "Stopping: " << __handle.ptr << endl;
	
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
	//traceL("Timer", this) << "Restarting: " << __handle.ptr << endl;
	if (!active())
		return start(_timeout, _interval);
	return again();
}


void Timer::again() 
{
	//traceL("Timer", this) << "Again: " << __handle.ptr << endl;

	assert(_handle.ptr());
    int err = uv_timer_again(_handle.ptr<uv_timer_t>());
    if (err < 0)
		_handle.setAndThrowError("Invalid timer", err);
	assert(active());
	_count = 0;
}


void Timer::setInterval(Int64 interval)
{
	//traceL("Timer", this) << "Set interval: " << interval << endl;
	
	assert(_handle.ptr());
    uv_timer_set_repeat(_handle.ptr<uv_timer_t>(), interval);
}


bool Timer::active() const
{
	return _handle.active();
}


Int64 Timer::timeout() const
{	
	return _timeout;
}


Int64 Timer::interval() const
{	
	assert(_handle.ptr());
	return std::min<Int64>(uv_timer_get_repeat(_handle.ptr<uv_timer_t>()), 0);
}


Int64 Timer::count()
{
	return _count;
}


uv::Handle& Timer::handle()
{
	return _handle;
}


#if 0

//
// Timer 2
//

Timer2::Timer2(uv::Loop* loop) : 
	ptr(loop, new uv_timer_t)
{
	//traceL("Timer2", this) << "Create" << endl;
	init();
}


Timer2::~Timer2()
{
	//traceL("Timer2", this) << "Destroy" << endl;
}


void Timer2::init()
{
	//traceL("Timer2", this) << "Init" << endl;	

	_count = 0;
	_timeout = 0;
	_interval = 0;

	assert(ptr.ptr());
	ptr.ptr()->data = this;
			
    int err = uv_timer_init(loop(), ptr.ptr<uv_timer_t>());
    if (err < 0)
		setAndThrowError("Cannot initialize timer", err);

}


void Timer2::unref()
{
	// Ghost timers do not reference the main loop
	//if (_ghost)
	uv_unref(ptr.ptr());
}


void Timer2::start(Int64 interval)
{
	start(interval, interval);
}


void Timer2::start(Int64 timeout, Int64 interval) 
{
	//traceL("Timer2", this) << "Starting: " << << timeout << ": " << interval << endl;
	assert(ptr.ptr());
	assert(timeout > 0);

	_timeout = timeout;
	_interval = interval;
	_count = 0;
				
    int err = uv_timer_start(ptr.ptr<uv_timer_t>(), [](uv_timer_t* req, int) {
		auto self = reinterpret_cast<Timer2*>(req->data);
		self->_count++;
		self->Timeout.emit(self);
	}, timeout, interval);
    if (err < 0)
		ptr.setAndThrowError("Invalid timer", err);
	assert(active());
}


void Timer2::stop() 
{
	//traceL("Timer2", this) << "Stopping: " << ptr.ptr() << endl;
	
	if (!active())
		return; // do nothing
	
	_count = 0;
    int err = uv_timer_stop(ptr.ptr<uv_timer_t>());
    if (err < 0)
		ptr.setAndThrowError("Invalid timer", err);
	assert(!active());	
}


void Timer2::restart()
{
	//traceL("Timer2", this) << "Restarting: " << ptr.ptr() << endl;
	if (!active())
		return start(_timeout, _interval);
	return again();
}


void Timer2::again() 
{
	//traceL("Timer2", this) << "Again: " << ptr.ptr() << endl;

	assert(ptr.ptr());
    int err = uv_timer_again(ptr.ptr<uv_timer_t>());
    if (err < 0)
		ptr.setAndThrowError("Invalid timer", err);
	assert(active());
	_count = 0;
}


void Timer2::setInterval(Int64 interval)
{
	//traceL("Timer2", this) << "Set interval: " << interval << endl;
	
	assert(ptr.ptr());
    uv_timer_set_repeat(ptr.ptr<uv_timer_t>(), interval);
}


bool Timer2::active() const
{
	return ptr.ptr() && uv_is_active(ptr.ptr()) == 1;
}


Int64 Timer2::timeout() const
{	
	return _timeout;
}


Int64 Timer2::interval() const
{	
	assert(ptr.ptr());
	return std::min<Int64>(uv_timer_get_repeat(ptr.ptr<uv_timer_t>()), 0);
}


Int64 Timer2::count()
{
	return _count;
}

#endif


} // namespace scy





	


/*

	// Ghost timers do not reference the main loop
	//if (_ghost)
	//	uv_unref(_handle.ptr());
void Timer::unref()
{
	uv_unref(_handle.ptr());
}
*/

/*
Timer::Timer(Int64 timeout, Int64 interval, uv::Loop* loop, bool ghost) : 
	uv::Handle(loop, new uv_timer_t), _ghost(ghost)
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
	//traceL("Timer", this) << "Closing: " << __handle.ptr << endl;
	Base::close();
}
*/


/*
void Timer::updateState()
{
	_count = 0;
	bool wasActive = _active;
	_active = !!uv_is_active((uv__handle.ptr_t*) _handle.ptr<uv_timer_t>());
		
	//traceL("Timer", this) << "Update State: " << _active << endl;
	if (!wasActive && _active) {
		// If our state is changing from inactive to active, we
		// increase the loop's reference count.
		// TODO: reenable
		uv_ref(_handle.ptr());
		uv_unref(_handle.ptr());
	} 
	else if (wasActive && !_active) {
		// If our state is changing from active to inactive, we
		// decrease the loop's reference count.
		// TODO: reenable
		uv_unref(_handle.ptr());
	}
}
*/