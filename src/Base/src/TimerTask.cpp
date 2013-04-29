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


TimerTask::TimerTask(long timeout, long interval) : 
	Task(true),
	_timeout(timeout), 
	_interval(interval),
	_scheduleAt(timeout)
{
	LogTrace() << "[TimerTask:" << this << "] Creating" << endl;
}


TimerTask::TimerTask(Runner& runner, long timeout, long interval) : 
	Task(runner, true, false), 
	_timeout(timeout), 
	_interval(interval),
	_scheduleAt(timeout)
{
	LogTrace() << "[TimerTask:" << this << "] Creating" << endl;
}


TimerTask::~TimerTask()
{
	LogTrace() << "[TimerTask:" << this << "] Destroying" << endl;
}


void TimerTask::start()
{
	LogTrace() << "[TimerTask:" << this << "] Start" << endl;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);	
		_scheduleAt.setDelay(_timeout);
		_scheduleAt.reset();
	}
	Task::start();	
}	


void TimerTask::cancel()			
{ 
	LogTrace() << "[TimerTask:" << this << "] Cancel" << endl;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);	
		_scheduleAt.stop();
	}
	Task::cancel();
}


void TimerTask::destroy()			
{ 
	LogTrace() << "[TimerTask:" << this << "] Destroying" << endl;
	Task::destroy();
}


bool TimerTask::beforeRun()
{ 
	Poco::FastMutex::ScopedLock lock(_mutex);	
	if (_scheduleAt.expired()) {
		if (_interval > 0) {
			_scheduleAt.setDelay(_interval);
			_scheduleAt.reset();
		}
		LogTrace() << "[TimerTask:" << this << "] Before Run: Timeout"  << endl;
		return true;
	}
	return false;
}
	//bool cancel = false;
	//{
	//cancel = _scheduleAt.expired();		
	//LogTrace() << "[TimerTask:" << this << "] Before Run: " 
	//	<< cancel << ": " 
	//	<< _scheduleAt.remaining() << endl;
	//}

	//if (cancel)
	//	Task::cancel();



bool TimerTask::afterRun()
{ 
	// TODO: Destroy the task?
	bool cancel = false;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);	
		cancel = _scheduleAt.expired();
		LogTrace() << "[TimerTask:" << this << "] After Run: " 
			<< cancel << ": " 
			<< _scheduleAt.remaining() << endl;
	}
	if (cancel)
		Task::cancel();
	return true;
}

		/*
		if (_scheduleAt.expired()) {
			if (_interval > 0) {
				_scheduleAt.setDelay(_interval);
				_scheduleAt.reset();
			}
			LogTrace() << "[TimerTask:" << this << "] Can Run: Timeout"  << endl;
			return true;
		}
		*/


void TimerTask::run()
{ 
	LogTrace() << "[TimerTask:" << this << "] Running" << endl;
	Timeout.emit(this);
	onTimeout();
}


void TimerTask::onTimeout()
{ 
	LogTrace() << "[TimerTask:" << this << "] Timeout" << endl;
	// override me...
}


void TimerTask::setTimeout(long timeout) 
{
	Poco::FastMutex::ScopedLock lock(_mutex);	
	_timeout = timeout;
}


void TimerTask::setInterval(long interval) 
{
	Poco::FastMutex::ScopedLock lock(_mutex);	
	_interval = interval;
}
	

long TimerTask::timeout() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);	
	return _timeout;
}


long TimerTask::interval() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);	
	return _interval;
}
	

Sourcey::Timeout& TimerTask::scheduleAt()
{
	Poco::FastMutex::ScopedLock lock(_mutex);	
	return _scheduleAt;
}
	

Sourcey::Timeout TimerTask::scheduleAt() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);	
	return _scheduleAt;
}


} // namespace Sourcey




	/*
	bool timeout = false;
	bool cancel = false;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);	
		if (_scheduleAt.expired()) {
			timeout = true;
			if (_interval > 0) {
				_scheduleAt.setDelay(_interval);
				_scheduleAt.reset();
			}
			else cancel = true;
		}
	}
	if (timeout)
		onTimeout();
	if (cancel)
		Task::destroy();
		*/



 //Runner::getDefault(), 
	
/*
TimerTask::TimerTask(Runner& runner, long timeout, long interval) : 
	Task(runner, true), 
	_timeout(timeout), 
	_interval(interval),
	_scheduleAt(timeout)
{
	LogTrace() << "[TimerTask:" << this << "] Creating" << endl;
}
*/