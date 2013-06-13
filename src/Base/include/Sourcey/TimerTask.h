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


#ifndef SOURCEY_TimerTask_H
#define SOURCEY_TimerTask_H


#include "Sourcey/Types.h"
#include "Sourcey/Task.h"
#include "Sourcey/Signal.h"
#include "Sourcey/Timeout.h"
#include "Poco/Mutex.h"

#include <string>


namespace Scy {
	

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
	
	virtual Scy::Timeout& scheduleAt();
	virtual Scy::Timeout scheduleAt() const;
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
	Scy::Timeout _scheduleAt;
};


} // namespace Scy


#endif // SOURCEY_TimerTask_H