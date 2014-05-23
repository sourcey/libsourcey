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
	//TraceLS(this) << "Create" << endl;
	init();
}


Timer::~Timer()
{
	//TraceLS(this) << "Destroy" << endl;
}


void Timer::init()
{
	//TraceLS(this) << "Init" << endl;	

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
	//TraceLS(this) << "Starting: " << << timeout << ": " << interval << endl;
	assert(_handle.ptr());
	assert(timeout > 0 || interval > 0);

	_timeout = timeout;
	_interval = interval;
	_count = 0;
				
    int err = uv_timer_start(_handle.ptr<uv_timer_t>(), [](uv_timer_t* req) {
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
	//TraceLS(this) << "Stopping: " << __handle.ptr << endl;
	
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
	//TraceLS(this) << "Restarting: " << __handle.ptr << endl;
	if (!active())
		return start(_timeout, _interval);
	return again();
}


void Timer::again() 
{
	//TraceLS(this) << "Again: " << __handle.ptr << endl;

	assert(_handle.ptr());
    int err = uv_timer_again(_handle.ptr<uv_timer_t>());
    if (err < 0)
		_handle.setAndThrowError("Invalid timer", err);
	assert(active());
	_count = 0;
}


void Timer::setInterval(Int64 interval)
{
	//TraceLS(this) << "Set interval: " << interval << endl;
	
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
	//TraceLS(this) << "Create" << endl;
	init();
}


Timer2::~Timer2()
{
	//TraceLS(this) << "Destroy" << endl;
}


void Timer2::init()
{
	//TraceLS(this) << "Init" << endl;	

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
	//TraceLS(this) << "Starting: " << << timeout << ": " << interval << endl;
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
	//TraceLS(this) << "Stopping: " << ptr.ptr() << endl;
	
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
	//TraceLS(this) << "Restarting: " << ptr.ptr() << endl;
	if (!active())
		return start(_timeout, _interval);
	return again();
}


void Timer2::again() 
{
	//TraceLS(this) << "Again: " << ptr.ptr() << endl;

	assert(ptr.ptr());
    int err = uv_timer_again(ptr.ptr<uv_timer_t>());
    if (err < 0)
		ptr.setAndThrowError("Invalid timer", err);
	assert(active());
	_count = 0;
}


void Timer2::setInterval(Int64 interval)
{
	//TraceLS(this) << "Set interval: " << interval << endl;
	
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