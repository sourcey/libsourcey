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


#include "Sourcey/Runner.h"
#include "Sourcey/Logger.h"

#include "Poco/SingletonHolder.h"

#include <iostream>
#include <assert.h>


using namespace std;
using namespace Poco;


namespace Sourcey {


Runner::Runner() : 
	_thread("Runner"),
	_stop(false) 
{	
	_thread.start(*this);
}


Runner::~Runner()
{		
	cout << "[Runner:" << this << "] Destroying" << endl;
	{
		Mutex::ScopedLock lock(_mutex);	
		_stop = true;
		_wakeUp.set();
	}
	
	// The Runner should never be destroyed inside a Task
	// callback, otherwise we will result in deadlock.
	_thread.join();

	cout << "[Runner:" << this << "] Destroying: OK" << endl;
}
	

bool Runner::start(ITask* task)
{
	if (running(task))
		return false;	

	Mutex::ScopedLock lock(_mutex);	
	task->_running = true;
	_tasks.push_back(task);
	Log("trace") << "[Runner:" << this << "] Started Task: " << task << endl;
	_wakeUp.set();
	return true;
}


bool Runner::stop(ITask* task)
{			
	if (!running(task))
		return false;

	Mutex::ScopedLock lock(_mutex);
	Log("trace") << "[Runner:" << this << "] Stopped Task: " 
		<< task << ": " << _tasks.size() << " tasks in queue." << endl;	
	task->_running = false;
	return true;
}


void Runner::abort(ITask* task)
{
	Log("trace") << "[Runner:" << this << "] Aborting Task: " 
		<< task << endl;

	Mutex::ScopedLock lock(_mutex);

	if (running(task)) {
		Log("trace") << "[Runner:" << this << "] Aborting Running Task: " << task << endl;
		task->_destroyed = true;
		task->_running = false;
	}
	else {
		Log("trace") << "[Runner:" << this << "] Aborting Stopped Task: " << task << endl;
		delete task;
	}
	/*
	*/
}


bool Runner::running(ITask* task) const
{	
	Log("trace") << "[Runner:" << this << "] Check Running: " 
		<< task << endl;

	Mutex::ScopedLock lock(_mutex);
	for (TaskList::const_iterator it = _tasks.begin(); it != _tasks.end(); ++it) {
		if (*it == task)
			return true; //(*it)->running();
	}
	return false;
}


void Runner::run()
{
	while (!_stop) {

		ITask* task = NULL;
		bool running = false;
		bool destroy = false;
		{
			Mutex::ScopedLock lock(_mutex);
				
			// Sometimes stop will be true after we unlock the mutex.
			if (_stop)
				break;

			//Log("trace") << "[Runner:" << this << "] Looping: " << _tasks.size() << endl;
			if (!_tasks.empty()) {
				task = _tasks.front();
				_tasks.pop_front();				
				assert(task);				
				running = task->_running;
				destroy = task->_destroyed || task->_runOnce;
				
				/*
				Log("trace") << "[Runner:" << this << "] Obtained: " 
					<< task
					//<< ": Aborted: " << task->destroyed() 
					//<< ": Running: " << task->running() 
					//<< ": Run Once: " << task->runOnce() 
					<< endl;
					*/

				// Replace the task if it is not single run, stopped or destroyed.
				if (running && !destroy) //!runOnce && 
					_tasks.push_back(task);
			}
		}

		if (!task) {
			Log("trace") << "[Runner:" << this << "] No more tasks to run" << endl;
			_wakeUp.wait();
			Log("trace") << "[Runner:" << this << "] Waking up" << endl;
			continue;
		}

		else if (running) {	
			//Log("trace") << "[Runner:" << this << "] Running: " << task << endl;
			task->run();	
			//Log("trace") << "[Runner:" << this << "] Running: OK: " << task << endl;
		} 

		if (destroy) { // || runOnce
			Log("trace") << "[Runner:" << this << "] Destroying Task: " << task << endl;	
			delete task;
		}

		Thread::sleep(5);
			
		Idle.dispatch(this);
	}	
			
	Log("trace") << "[Runner:" << this << "] Shutdown" << endl;
	
	// Ensure all tasks are destroyed.
	{
		//Mutex::ScopedLock lock(_mutex);
		for (TaskList::iterator it = _tasks.begin(); it != _tasks.end(); ++it) {	
			Log("trace") << "[Runner:" << this << "] Shutdown: Destroying Task: " << *it << endl;
			(*it)->_running = false;
			delete *it;
		}
	}
		
	Shutdown.dispatch(this);

	Log("trace") << "[Runner:" << this << "] Exiting" << endl;
}


/*
Runner& Runner::getDefault() 
{
	static Poco::SingletonHolder<Runner> sh;
	return *sh.get();
}
*/


} // namespace Sourcey