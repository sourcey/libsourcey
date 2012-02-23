//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2002 Sourcey
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
#include "Sourcey/ITask.h"
#include "Sourcey/Signal.h"
#include "Sourcey/Timeout.h"
#include "Poco/Mutex.h"

#include <string>


namespace Sourcey {
	

class Runner;
	

class TimerTask: public ITask
	/// This class defines an asynchronous Task whose run() 
	/// method will be called frequently by the Runner.
{
public:	
	TimerTask(Runner& runner, long timeout = 0, long interval = 0);
	TimerTask(long timeout = 0, long interval = 0);

	virtual bool start(); 
	virtual bool stop();
	
	virtual void onTimeout();
		/// Preforms task processing when the timer fires.
	
	//virtual bool again();
		/// Start the timer, and if it is repeating restart
		/// it using the repeat value as the timeout.

	virtual void setInterval(long interval);
		/// Set the repeat value. Note that if the repeat
		/// value is set from a timer callback it does not
		/// immediately take effect. If the timer was
		/// non-repeating before, it will have been stopped.
		/// If it was repeating, then the old repeat value
		/// will have been used to schedule the next timeout.

	virtual long interval() const;
		/// Returns the timer interval value.
	
	NullSignal Invoke;

protected:
	TimerTask& operator=(TimerTask const&) {}
	virtual ~TimerTask();
	virtual void run();
	
	long _timeout;
	long _interval;
	Timeout _scheduleAt;

	///Timeout _timeout;

	friend class Runner;
};

			/// = Runner::getDefault()
		  //bool autoStart = false, 
		  //bool runOnce = false, 
		  //const std::string& name = ""
		  
	/*
	Runner& _runner;	
	bool _runOnce;
	bool _running;
	bool _destroyed;
	std::string _name;
	template <class DeletableT> 
	friend class GarbageCollectionTask;
	*/	

	/*
	virtual void destroy();
		/// Called from outside to abort the task without any
		/// more callbacks. The task instance will be deleted
		/// shortly by the Runner.	

	virtual bool runOnce() const;
	virtual bool running() const;
	virtual bool destroyed() const;
	
	virtual std::string name() const;
	virtual void setName(const std::string& name);
		  
	virtual Runner& runner() { return _runner; }
	
	virtual void run() = 0;
	///TimerTask(const TimerTask&) {}
	///virtual ~Task() {}
	*/


} // namespace Sourcey


#endif // SOURCEY_TimerTask_H