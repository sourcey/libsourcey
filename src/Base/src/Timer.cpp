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
#include "Sourcey/Crypto.h"
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
}


void Timer::init()
{
	traceL("Timer", this) << "Init" << endl;	

	_count = 0;
	_timeout = 0;
	_interval = 0;

	assert(_handle);
	_handle->data = this;
			
	int r = uv_timer_init(loop(), handle<uv_timer_t>());
	if (r)
		setAndThrowLastError("Cannot initialize timer");

	// Timers do not reference the main loop
    uv_unref(handle());
}


bool Timer::start(Int64 interval)
{
	return start(interval, interval);
}


bool Timer::start(Int64 timeout, Int64 interval) 
{
	traceL("Timer", this) << "Starting: " << _handle << ": " << timeout << ": " << interval << endl;
	assert(_handle);
	assert(timeout > 0);

	_timeout = timeout;
	_interval = interval;
	_count = 0;

    int r = uv_timer_start(handle<uv_timer_t>(), Timer::onTimeout, timeout, interval);
    if (r) 
		setAndThrowLastError("Invalid timer");
	//uv_ref(handle());
	assert(active());
    return r == 0;
}


bool Timer::stop() 
{
	traceL("Timer", this) << "Stopping: " << _handle << endl;
	
	if (!active())
		return false;
	
	_count = 0;
    int r = uv_timer_stop(handle<uv_timer_t>());
    if (r) 
		setAndThrowLastError("Invalid timer");
	//uv_unref(handle());
	assert(!active());	
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

	assert(_handle);
    int r = uv_timer_again(handle<uv_timer_t>());
    if (r) 
		setAndThrowLastError("Invalid timer");
	assert(active());
	//uv_ref(handle());
	_count = 0;
    return r == 0;
}


void Timer::setInterval(Int64 interval)
{
	traceL("Timer", this) << "Set interval: " << interval << endl;

	assert(_handle);
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
	assert(_handle);
	return std::min<Int64>(uv_timer_get_repeat(handle<uv_timer_t>()), 0);
}


Int64 Timer::count()
{
	return _count;
}


void Timer::onTimeout()
{	
	//traceL("Timer", this) << "On timeout: " << _count << endl;
	_count++;
	Timeout.emit(this);
}


//
// Timeout
//


Timeout::Timeout(long delay, bool autoStart) :
	_startAt(0), _delay(delay) 
{
	if (autoStart)
		start();
}


Timeout::Timeout(const Timeout& src) :
	_startAt(src._startAt), _delay(src._delay) 
{
}


Timeout& Timeout::operator = (const Timeout& src) 
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
	_startAt = scy::getTimeHR();
}


void Timeout::stop() 
{
	_startAt = 0;
}


void Timeout::reset() 
{
	_startAt = scy::getTimeHR();
}


long Timeout::available() const 
{
	time_t current = scy::getTimeHR();
	long remaining = static_cast<long>(_delay - (current - _startAt));
	return remaining > 0 ? remaining : 0;
}


bool Timeout::expired() const 
{
	if (_delay == 0) //_startAt == 0 || 
		return false;

	return available() == 0;
}


//
// Stopwatch
//


Stopwatch::Stopwatch() : 
	_elapsed(0), _running(false)
{
}


Stopwatch::~Stopwatch()
{
}


void Stopwatch::start()
{
	if (!_running)
	{
		_start.update();
		_running = true;
	}
}


void Stopwatch::stop()
{
	if (_running)
	{
		Timestamp current;
		_elapsed += current - _start;
		_running = false;
	}
}


int Stopwatch::elapsedSeconds() const
{
	return int(elapsed()/resolution());
}


Timestamp::TimeVal Stopwatch::resolution()
{
	return Timestamp::resolution();
}


Timestamp::TimeDiff Stopwatch::elapsed() const
{
	if (_running) {
		Timestamp current;
		return _elapsed + (current - _start);
	}
	else {
		return _elapsed;
	}
}


void Stopwatch::reset()
{
	_elapsed = 0;
	_running = false;
}


void Stopwatch::restart()
{
	_elapsed = 0;
	_start.update();
	_running = true;
}


//
// Timed Token
//


TimedToken::TimedToken(long duration) : 
	Timeout(duration), _id(crypto::randomString(32)) 
{
}


TimedToken::TimedToken(const std::string& id = crypto::randomString(32), long duration = 10000) : 
	Timeout(duration), _id(id) 
{
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