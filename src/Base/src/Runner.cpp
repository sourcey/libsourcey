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


bool Runner::start(Task* task)
{
	if (task->_runner && 
		task->_runner != this)
		throw Exception("Runner instance mismatch.");
	
	add(task);
	if (!task->_running) {
		task->_running = true;
		task->start();
		Log("trace") << "[Runner:" << this << "] Started Task: " << task << endl;
		_wakeUp.set();
		return true;
	}
	return false;
}


bool Runner::stop(Task* task)
{		
	if (task->_runner != this)
		throw Exception("Runner instance mismatch.");

	if (task->_running) {
		task->_running = false;
		task->stop();
		Log("trace") << "[Runner:" << this << "] Stopped Task: " << task << endl;
		_wakeUp.set();
		return true;
	}
	
	return false;
}


bool Runner::destroy(Task* task)
{
	Log("trace") << "[Runner:" << this << "] Aborting Task: " << task << endl;

	if (task->_runner != this)
		throw Exception("Runner instance mismatch.");
	
	// If the task exists then set the destroyed flag.
	if (exists(task)) {
		Log("trace") << "[Runner:" << this << "] Aborting Managed Task: " << task << endl;
		task->_destroyed = true;
	}
		
	// Otherwise destroy the pointer.
	else {
		Log("trace") << "[Runner:" << this << "] Aborting Unmanaged Task: " << task << endl;
		delete task;
	}
	_wakeUp.set();
	return true; // hmmm
}
	

bool Runner::add(Task* task)
{
	if (!exists(task)) {
		Log("trace") << "[Runner:" << this << "] Added Task: " << task << endl;
		Mutex::ScopedLock lock(_mutex);	
		_tasks.push_back(task);
		task->_runner = this;
		return true;
	}
	return false;
}


bool Runner::exists(Task* task) const
{	
	Log("trace") << "[Runner:" << this << "] Check Exists: " << task << endl;

	Mutex::ScopedLock lock(_mutex);
	for (TaskList::const_iterator it = _tasks.begin(); it != _tasks.end(); ++it) {
		if (*it == task)
			return true;
	}
	return false;
}


void Runner::run()
{
	while (!_stop) 
	{
		Task* task = NULL;
		bool running = false;
		bool destroy = false;
		{
			Mutex::ScopedLock lock(_mutex);
				
			// Sometimes stop will be true after we
			// unlock the mutex so check again.
			if (_stop)
				break;

			//Log("trace") << "[Runner:" << this << "] Looping: " << _tasks.size() << endl;
			if (!_tasks.empty()) {
				task = _tasks.front();
				_tasks.pop_front();
				
				running = task->_running;
				destroy = task->_destroyed || !task->_repeating;
				
				/*
				Log("trace") << "[Runner:" << this << "] Obtained: " 
					<< task
					<< ": Aborted: " << task->destroyed() 
					<< ": Running: " << task->running() 
					<< ": Repeating: " << task->repeating() 
					<< endl;
					*/

				// Push the task back onto the end of the queue
				// if the task is not pending destruction.
				if (!destroy)
					_tasks.push_back(task);
			}
		}

		// Pause processing if there are no tasks so
		// we can save CPU.
		// TODO: Check if we have any running tasks.
		if (!task) {
			Log("trace") << "[Runner:" << this << "] No more tasks to run" << endl;
			_wakeUp.wait();
			Log("trace") << "[Runner:" << this << "] Waking up" << endl;
			continue;
		}

		// Run the task...
		else if (running) {	
			//Log("trace") << "[Runner:" << this << "] Running: " << task << endl;
			if (task->canRun())
				task->run();	
			//Log("trace") << "[Runner:" << this << "] Running: OK: " << task << endl;
		}

		// Destroy the task if the destroy flag is set,
		// or if the task is not repeatable.
		if (destroy) {
			Log("trace") << "[Runner:" << this << "] Destroying Task: " << task << endl;	
			delete task;
		}

		// Gulp
		Thread::sleep(5);		

		Idle.dispatch(this);
	}	
			
	Log("trace") << "[Runner:" << this << "] Shutdown" << endl;
	
	// Ensure all tasks are destroyed.
	{
		Mutex::ScopedLock lock(_mutex);
		for (TaskList::iterator it = _tasks.begin(); it != _tasks.end(); ++it) {	
			Log("trace") << "[Runner:" << this << "] Shutdown: Destroying Task: " << *it << endl;
			delete *it;
		}
	}
		
	Shutdown.dispatch(this);

	Log("trace") << "[Runner:" << this << "] Exiting" << endl;
}


Runner& Runner::getDefault() 
{
	static Poco::SingletonHolder<Runner> sh;
	return *sh.get();
}


} // namespace Sourcey






/*
bool Runner::running(Task* task) const
{	
	Log("trace") << "[Runner:" << this << "] Check Running: " << task << endl;

	Mutex::ScopedLock lock(_mutex);
	for (TaskList::const_iterator it = _tasks.begin(); it != _tasks.end(); ++it) {
		if (*it == task)
			return (*it)->running();
	}
	return false;
}


void Runner::wakeUp()
{
	_wakeUp.set();
}
*/



	//Mutex::ScopedLock lock(_mutex);	

	/* //, repeat, true)
	// If the task exists then start it.
	bool exists = false;
	for (TaskList::const_iterator it = _tasks.begin(); it != _tasks.end(); ++it) {
		if (*it == task) {
			(*it)->_running = true;
			exists = true;
		}
	}
	*/
	
	// Otherwise create an entry.
	//if (!exists(task))
	//	_tasks.push_back(task); //, repeat, true)

	//task->start();




	/*
	Mutex::ScopedLock lock(_mutex);
	for (TaskList::const_iterator it = _tasks.begin(); it != _tasks.end(); ++it) {
		if (*it == task) {
			(*it)->_running = false;
			(*it)->onStop();
			return true;
		}
	}
	return false;
	*/



	/*
	Mutex::ScopedLock lock(_mutex);
	
	bool exists = false;
	for (TaskList::const_iterator it = _tasks.begin(); it != _tasks.end(); ++it) {
		Log("trace") << "[Runner:" << this << "] Aborting Managed Task: " << task << endl;
		if (*it == task) {
			(*it)->_running = false;
			(*it)->_destroyed = true;
			exists = true;
		}
	}
	
	// Otherwise destroy the pointer.
	if (!exists) {
		Log("trace") << "[Runner:" << this << "] Aborting Unmanaged Task: " << task << endl;
		delete task;
	}
	*/
	
	//task->destroy();






			//delete (*it);
		//Task* task = NULL;
		//bool running = false;
		//bool destroy = false;
		//		task = entry->task;
	
	//if (exists(task)) {
	//}	
	
	/*
	return false;
	task->_running = true;
	_tasks.push_back(task);
	*/

	/*
	if (!running(task))
		return false;

	Mutex::ScopedLock lock(_mutex);
	Log("trace") << "[Runner:" << this << "] Stopped Task: " 
		<< task << ": " << _tasks.size() << " tasks in queue." << endl;	
	task->_running = false;
	return true;
	*/

	/*
	if (running(task)) {
		Log("trace") << "[Runner:" << this << "] Aborting Running Task: " << task << endl;
		task->_destroy = true;
		task->_running = false;
	}
	else {
		Log("trace") << "[Runner:" << this << "] Aborting Stopped Task: " << task << endl;
		delete task;
	}
	*/

 // || runOnce
//(*it)->_running = false;

				// Set the destroy flag if we do not repeat.
				//if (!entry->repeat)
				//	entry->destroy = true;

				//running = task->_running;
				//destroy = task->_destroy || task->_runOnce;
				//!runOnce && 		
				//assert(entry->task);	