//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is is distributed under a dual license that allows free, 
// open source use and closed source use under a standard commercial
// license.
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


#ifndef SOURCEY_TimerTask_H
#define SOURCEY_TimerTask_H


#include "Sourcey/Base.h"
#include "Sourcey/Task.h"
#include "Sourcey/Signal.h"
#include "Sourcey/Timeout.h"
#include "Poco/Mutex.h"

#include <string>


namespace Sourcey {
	

class Runner;
	

class TimerTask: public Task
	/// This class is an async a timer task which sends
	/// callbacks at specified timeout intervals.
	///
	/// TimerTasks with only a timeout value and no repeat
	/// interval will be stopped after the initial timeout.
	/// Calling start() again will restart the timer.
	///
	/// TimerTasks must be explicitly destroyed using the 
	/// destroy() method.
{
public:	
	TimerTask(long timeout = 0, long interval = 0);
	TimerTask(Runner& runner, long timeout = 0, long interval = 0);
	
	virtual void start();
	virtual void cancel();
	virtual void destroy();

	virtual void setTimeout(long timeout);
		/// Sets the initial timeout value. Note that if the
		/// timer has already been started then setting this
		/// value will have no effect.

	virtual void setInterval(long interval);
		/// Sets the repeat value. Note that if the repeat
		/// value is set from a timer callback it does not
		/// immediately take effect. If the timer was
		/// non-repeating before, it will have been stopped.
		/// If it was repeating, then the old repeat value
		/// will have been used to schedule the next timeout.
	
	virtual long timeout() const;
		/// Returns the timer timeout value.

	virtual long interval() const;
		/// Returns the timer interval value.
	
	virtual Sourcey::Timeout& scheduleAt();
	virtual Sourcey::Timeout scheduleAt() const;
		/// Returns the next scheduled time value.
	
	virtual void onTimeout();
		/// Derived classes can extend this method to 
		/// implement processing logic when the timer fires.
	
	NullSignal Timeout;
		/// Signals on timeout and interval.

protected:
	TimerTask& operator=(TimerTask const&) {}
	virtual ~TimerTask();	
	
	virtual bool beforeRun();
	virtual bool afterRun();
	virtual void run();
	
	long _timeout;
	long _interval;
	Sourcey::Timeout _scheduleAt;
};


} // namespace Sourcey


#endif // SOURCEY_TimerTask_H