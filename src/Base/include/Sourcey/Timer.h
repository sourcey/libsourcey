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


#ifndef SOURCEY_Timer_H
#define SOURCEY_Timer_H


#include "Sourcey/UV/UVPP.h"
#include "Sourcey/DateTime.h"
#include "Sourcey/Types.h"
#include "Sourcey/Signal.h"
#include "Sourcey/Memory.h"


namespace scy {


class Timer: public uv::Base
	// Wraps libev's ev_timer watcher. Used to get woken up at a specified time
	// in the future.
{
public:
	Timer(Int64 timeout, Int64 interval = 0, uv::Loop& loop = uv::defaultLoop());
	Timer(uv::Loop& loop = uv::defaultLoop());
	virtual ~Timer();
	
	virtual bool start(Int64 interval);
	virtual bool start(Int64 timeout, Int64 interval);
		// Start the timer, an interval value of zero will only trigger
		// once after timeout.

	virtual bool stop();
		// Stops the timer.
	
	virtual bool restart();
		// Restarts the timer, even if it hasn't been started yet.
		// An interval or interval must be set or an exception will be thrown.
	
	virtual bool again();
		// Stop the timer, and if it is repeating restart it using the
		// repeat value as the timeout. If the timer has never been started
		// before it returns -1 and sets the error to UV_EINVAL.

	virtual void setInterval(Int64 interval);
		// Set the repeat value. Note that if the repeat value is set from
		// a timer callback it does not immediately take effect. If the timer
		// was non-repeating before, it will have been stopped. If it was repeating,
		// then the old repeat value will have been used to schedule the next timeout.

	virtual bool active() const;
	
	virtual Int64 timeout() const;
	virtual Int64 interval() const;
	
	Int64 count();

	virtual void onTimeout();
	
	NullSignal Timeout;

protected:	
	virtual void init();
	//virtual void close();
	
	//
	// UV Callbacks
	UVEmptyStatusCallback(Timer, onTimeout, uv_timer_t);

	Int64 _count;
	Int64 _timeout;
	Int64 _interval;
};


// ---------------------------------------------------------------------
//
class Timeout 
	// Simple timeout counter which expires
	// after a given delay.
{
public:
	Timeout(long delay = 0, bool autoStart = false);
	Timeout(const Timeout& src);
	~Timeout();
	
	bool running() const;
	void start();
	void stop();
	void reset();
	long available() const;
	bool expired() const;

	void setDelay(long delay) { _delay = delay; };

	time_t startAt() const { return _startAt; };
	long delay() const { return _delay; };

	Timeout& operator = (const Timeout& src);

protected:
	time_t	_startAt;
	long	_delay;
};


// ---------------------------------------------------------------------
//
class Stopwatch
	// A simple facility to measure time intervals
	// with microsecond resolution.
	//
	// Note that Stopwatch is based on the Timestamp
	// class. Therefore, if during a Stopwatch run,
	// the system time is changed, the measured time
	// will not be correct.
{
public:
	Stopwatch();
	~Stopwatch();

	void start();
		// Starts (or restarts) the stopwatch.
		
	void stop();
		// Stops or pauses the stopwatch.
	
	void reset();
		// Resets the stopwatch.
		
	void restart();
		// Resets and starts the stopwatch.
		
	Timestamp::TimeDiff elapsed() const;
		// Returns the elapsed time in microseconds
		// since the stopwatch started.
		
	int elapsedSeconds() const;
		// Returns the number of seconds elapsed
		// since the stopwatch started.

	static Timestamp::TimeVal resolution();
		// Returns the resolution of the stopwatch.

private:
	Stopwatch(const Stopwatch&);
	Stopwatch& operator = (const Stopwatch&);

	Timestamp           _start;
	Timestamp::TimeDiff _elapsed;
	bool                _running;
};


// ---------------------------------------------------------------------
//
class TimedToken: public Timeout
	// A token that expires after the specified duration.
{
public:
	TimedToken(long duration);
	TimedToken(const std::string& id, long duration);
	
	std::string id() const { return _id; }
	
	bool operator == (const TimedToken& r) const 
	{
		return id()  == r.id();
	}
	
	bool operator == (const std::string& r) const
	{
		return id() == r;
	}

protected:
	std::string _id;
};


} // namespace scy


#endif // SOURCEY_Timer_H


