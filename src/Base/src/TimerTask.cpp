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


#include "Sourcey/TimerTask.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Runner.h"

#include <assert.h>


using namespace std;


namespace Sourcey {

	
TimerTask::TimerTask(Runner& runner, long timeout, long interval) : 
	ITask(runner, false, false), 
	_timeout(timeout), 
	_interval(interval),
	_scheduleAt(timeout)
{
	Log("trace") << "[TimerTask: " << this << "] Creating" << endl;
}


/*
TimerTask::TimerTask(long timeout, long interval) : 
	ITask(Runner::getDefault(), false, false), 
	_timeout(timeout), 
	_interval(interval),
	_scheduleAt(timeout)
{
	Log("trace") << "[TimerTask: " << this << "] Creating" << endl;
}
*/


TimerTask::~TimerTask()
{
	Log("trace") << "[TimerTask: " << this << "] Destroying" << endl;
}


bool TimerTask::start()
{
	Log("trace") << "[TimerTask: " << this << "] Start" << endl;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);	
		_scheduleAt.setDelay(_timeout);
		_scheduleAt.reset();
	}
	return ITask::start();	
}	


bool TimerTask::stop()			
{ 
	Log("trace") << "[TimerTask: " << this << "] Stop" << endl;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);	
		_scheduleAt.stop();
	}
	return ITask::stop();
}


void TimerTask::run()
{ 
	bool doTimeout = false;
	bool doDestroy = false;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);	
		if (_scheduleAt.expired()) {
			doTimeout = true;
			if (_interval > 0) {
				_scheduleAt.setDelay(_interval);
				_scheduleAt.reset();
			}
			else doDestroy = true;
		}
	}
	if (doTimeout)
		onTimeout();
	if (doDestroy)
		destroy();
}

	
long TimerTask::interval() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);	
	return _interval;
}


void TimerTask::setInterval(long interval) 
{
	Poco::FastMutex::ScopedLock lock(_mutex);	
	_interval = interval;
}


void TimerTask::onTimeout()
{ 
	Timeout.dispatch(this);
}


} // namespace Sourcey
