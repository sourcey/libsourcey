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
		FastMutex::ScopedLock lock(_mutex);	
		_stop = true;
		_wakeUp.set();
	}
	
	// The Runner should never be destroyed inside a Task
	// callback, otherwise we will result in deadlock.
	_thread.join();
	clear();
	cout << "[Runner:" << this << "] Destroying: OK" << endl;
}


bool Runner::start(Task* task)
{
	if (task->_runner && 
		task->_runner != this)
		throw Exception("Runner instance mismatch.");
	
	add(task);
	if (task->_cancelled) {
		task->_cancelled = false;
		task->start();
		Log("trace", this) << "Started Task: " << task << endl;
		onStart(task);
		_wakeUp.set();
		return true;
	}
	return false;
}


bool Runner::cancel(Task* task)
{		
	if (task->_runner != this)
		throw Exception("Runner instance mismatch.");

	if (!task->_cancelled) {
		task->_cancelled = true;
		task->cancel();
		Log("trace", this) << "Cancelled Task: " << task << endl;
		onCancel(task);
		_wakeUp.set();
		return true;
	}
	
	return false;
}


bool Runner::destroy(Task* task)
{
	Log("trace", this) << "Aborting Task: " << task << endl;

	if (task->_runner != this)
		throw Exception("Runner instance mismatch.");
	
	// If the task exists then set the destroyed flag.
	if (exists(task)) {
		Log("trace", this) << "Aborting Managed Task: " << task << endl;
		task->_destroyed = true;
	}
		
	// Otherwise destroy the pointer.
	else {
		Log("trace", this) << "Aborting Unmanaged Task: " << task << endl;
		delete task;
	}

	_wakeUp.set();
	return true; // hmmm
}
	

bool Runner::add(Task* task)
{
	Log("trace", this) << "Adding Task: " << task << endl;
	if (!exists(task)) {
		FastMutex::ScopedLock lock(_mutex);	
		_tasks.push_back(task);
		task->_runner = this;
		Log("trace", this) << "Added Task: " << task << endl;
		onAdd(task);
		return true;
	}
	return false;
}


bool Runner::remove(Task* task)
{	
	Log("trace", this) << "Removing Task: " << task << endl;

	FastMutex::ScopedLock lock(_mutex);
	for (TaskList::iterator it = _tasks.begin(); it != _tasks.end(); ++it) {
		if (*it == task) {					
			_tasks.erase(it);
			Log("trace", this) << "Removed Task: " << task << endl;
			onRemove(task);
			return true;
		}
	}
	return false;
}


bool Runner::exists(Task* task) const
{	
	Log("trace", this) << "Check Exists: " << task << endl;

	FastMutex::ScopedLock lock(_mutex);
	for (TaskList::const_iterator it = _tasks.begin(); it != _tasks.end(); ++it) {
		if (*it == task)
			return true;
	}
	return false;
}


Task* Runner::get(UInt32 id) const
{
	FastMutex::ScopedLock lock(_mutex);
	for (TaskList::const_iterator it = _tasks.begin(); it != _tasks.end(); ++it) {
		if ((*it)->id() == id)
			return *it;
	}			
	return NULL;
}


Task* Runner::next() const
{
	FastMutex::ScopedLock lock(_mutex);
	for (TaskList::const_iterator it = _tasks.begin(); it != _tasks.end(); ++it) {
		if (!(*it)->cancelled())
			return *it;
	}			
	return NULL;
}


void Runner::clear()
{
	FastMutex::ScopedLock lock(_mutex);
	for (TaskList::iterator it = _tasks.begin(); it != _tasks.end(); ++it) {	
		Log("trace", this) << "Shutdown: Destroying Task: " << *it << endl;
		delete *it;
	}
}


void Runner::run()
{
	while (!_stop) 
	{
		Task* task = next();
		
		// Run the task
		if (task) {
			if (task->beforeRun()) {
				task->run();	
				if (task->afterRun())			
					onRun(task);
				else
					task->_destroyed = true; //destroy();	
			}
						
			// Destroy the task if required
			if (task->destroyed()) {
				Log("trace", this) << "Destroying Task: " << task << endl;
				remove(task);
				delete task;
			}	
		}
		
		// Go to sleep if we have no tasks
		else {			
			Log("trace", this) << "Sleeping" << endl;
			_wakeUp.wait();
			Log("trace", this) << "Waking up" << endl;
		}

		// Gulp
		Thread::sleep(5);		

		// Dispatch the Idle signal
		// TODO: Idle on each complete iteration of all tasks
		Idle.dispatch(this);
	}	
			
	Log("trace", this) << "Shutdown" << endl;
		
	Shutdown.dispatch(this);

	Log("trace", this) << "Exiting" << endl;
}


void Runner::onAdd(Task*) 
{
}


void Runner::onStart(Task*) 
{
}


void Runner::onCancel(Task*) 
{
}


void Runner::onRemove(Task*) 
{
}


void Runner::onRun(Task*) 
{
}


Runner& Runner::getDefault() 
{
	static Poco::SingletonHolder<Runner> sh;
	return *sh.get();
}


} // namespace Sourcey



	//FastMutex::ScopedLock lock(_mutex);	
	//return _tasks.empty() ? NULL : _tasks.front();

			//Log("trace", this) << "Running: " << task << endl;
			//Log("trace", this) << "Running: OK: " << task << endl;



				//continue;
			
			// Push the task back onto the end of the queue
			//else {
			//	FastMutex::ScopedLock lock(_mutex);
			//	_tasks.push_back(task);
			//}

		/*
		TaskList::iterator it;
		{
			FastMutex::ScopedLock lock(_mutex);
			for (it = _tasks.begin(); it != _tasks.end(); ++it) {
				if (!(*it)->cancelled()) {
					task = *it;
					break;
				}
			}
		}
		*/

				/*
				{
					FastMutex::ScopedLock lock(_mutex);
					_tasks.erase(it);
				}
				*/

/*
Task* Runner::pop()
{
	FastMutex::ScopedLock lock(_mutex);
	Task* task = NULL;
	if (!_tasks.empty())
		task = _tasks.front();
	_tasks.pop_front();				
	return task;
}


Task* Runner::popNonCancelled()
{
	FastMutex::ScopedLock lock(_mutex);
	for (TaskList::iterator it = _tasks.begin(); it != _tasks.end(); ++it) {
		Task* task = *it;
		if (!task->cancelled()) {			
			_tasks.erase(it);
			return task;
		}
	}			
	return NULL;
}


void Runner::push(Task* task)
{
	FastMutex::ScopedLock lock(_mutex);
	_tasks.push_back(task);
}
*/


			//else { //if (!task->cancelled()) {	
			//}

	//Task* task = NULL;
	//bool cancel = false;
	//bool destroy = false;

/*
void Runner::run()
{
	Task* task = NULL;
	bool cancel = false;
	bool destroy = false;
	while (!_stop) 
	{
		{
			FastMutex::ScopedLock lock(_mutex);
				
			// Sometimes stop will be true after we
			// unlock the mutex so check again.
			if (_stop)
				break;

			//Log("trace", this) << "Looping: " << _tasks.size() << endl;
			if (!_tasks.empty()) {
				task = _tasks.front();
				//if (task->_cancelled)
				//	continue;

				_tasks.pop_front();
				
				cancel = task->_cancelled;
				destroy = task->_destroyed || !task->_repeating;

				// Push the task back onto the end of the queue
				// if the task is not pending destruction.
				//if (!destroy)
				//	_tasks.push_back(task);
			}
		}

		// Pause processing if there are no tasks so
		// we can save CPU.
		// TODO: Check if we have any cancel tasks.
		if (!task) {
			Log("trace", this) << "No more tasks to run" << endl;
			_wakeUp.wait();
			Log("trace", this) << "Waking up" << endl;
			continue;
		}

		// Run the task...
		else if (!cancel) {	
			//Log("trace", this) << "Running: " << task << endl;
			if (task->beforeRun())
				task->run();	
			//Log("trace", this) << "Running: OK: " << task << endl;
		}

		// Destroy the task if the destroy flag is set,
		// or if the task is not repeatable.
		if (destroy) {
			Log("trace", this) << "Destroying Task: " << task << endl;	
			delete task;
		}

		else 

		// Gulp
		Thread::sleep(5);		

		Idle.dispatch(this);
	}	
			
	Log("trace", this) << "Shutdown" << endl;
	
	// Ensure all tasks are destroyed.
	{
		FastMutex::ScopedLock lock(_mutex);
		for (TaskList::iterator it = _tasks.begin(); it != _tasks.end(); ++it) {	
			Log("trace", this) << "Shutdown: Destroying Task: " << *it << endl;
			delete *it;
		}
	}
		
	Shutdown.dispatch(this);

	Log("trace", this) << "Exiting" << endl;
}
*/





		/*
		{
			FastMutex::ScopedLock lock(_mutex);
				
			// Sometimes stop will be true after we
			// unlock the mutex so check again.
			if (_stop)
				break;

			//Log("trace", this) << "Looping: " << _tasks.size() << endl;
			if (!_tasks.empty()) {
				task = _tasks.front();
				//if (task->_cancelled)
				//	continue;

				_tasks.pop_front();
				
				cancel = task->_cancelled;
				destroy = task->_destroyed || !task->_repeating;

				// Push the task back onto the end of the queue
				// if the task is not pending destruction.
				//if (!destroy)
				//	_tasks.push_back(task);
			}
		}

		*/
				
		/*
		Log("trace", this) << "Obtained: " 
			<< task
			<< ": Aborted: " << task->destroyed() 
			<< ": Running: " << !task->cancel() 
			<< ": Repeating: " << task->repeating() 
			<< endl;

		// Pause processing if there are no tasks so
		// we can save CPU.
		// TODO: Check if we have any cancel tasks.
		if (!task) {
			Log("trace", this) << "No more tasks to run" << endl;
			_wakeUp.wait();
			Log("trace", this) << "Waking up" << endl;
			continue;
		}

		// Run the task...
		else if (!cancel) {	
			//Log("trace", this) << "Running: " << task << endl;
			if (task->beforeRun())
				task->run();	
			//Log("trace", this) << "Running: OK: " << task << endl;
		}

		// Destroy the task if the destroy flag is set,
		// or if the task is not repeatable.
		if (destroy) {
			Log("trace", this) << "Destroying Task: " << task << endl;	
			delete task;
		}

		else 
		*/

/*
bool Runner::running(Task* task) const
{	
	Log("trace", this) << "Check Running: " << task << endl;

	FastMutex::ScopedLock lock(_mutex);
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



	//FastMutex::ScopedLock lock(_mutex);	

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
	FastMutex::ScopedLock lock(_mutex);
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
	FastMutex::ScopedLock lock(_mutex);
	
	bool exists = false;
	for (TaskList::const_iterator it = _tasks.begin(); it != _tasks.end(); ++it) {
		Log("trace", this) << "Aborting Managed Task: " << task << endl;
		if (*it == task) {
			(*it)->_running = false;
			(*it)->_destroyed = true;
			exists = true;
		}
	}
	
	// Otherwise destroy the pointer.
	if (!exists) {
		Log("trace", this) << "Aborting Unmanaged Task: " << task << endl;
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
	!task->_cancelled = true;
	_tasks.push_back(task);
	*/

	/*
	if (!running(task))
		return false;

	FastMutex::ScopedLock lock(_mutex);
	Log("trace", this) << "Stopped Task: " 
		<< task << ": " << _tasks.size() << " tasks in queue." << endl;	
	!task->_cancelled = false;
	return true;
	*/

	/*
	if (running(task)) {
		Log("trace", this) << "Aborting Running Task: " << task << endl;
		task->_destroy = true;
		!task->_cancelled = false;
	}
	else {
		Log("trace", this) << "Aborting Stopped Task: " << task << endl;
		delete task;
	}
	*/

 // || runOnce
//(*it)->_running = false;

				// Set the destroy flag if we do not repeat.
				//if (!entry->repeat)
				//	entry->destroy = true;

				//running = !task->_cancelled;
				//destroy = task->_destroy || task->_runOnce;
				//!runOnce && 		
				//assert(entry->task);	