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


#include "Sourcey/Runner.h"
#include "Sourcey/Logger.h"

#include "Poco/SingletonHolder.h"

#include <iostream>
#include <assert.h>


using namespace std;
using namespace Poco;

// TODO: Inner loop timeout recovery procedure

namespace scy {


Runner::Runner() : 
	_thread("Runner"),
	_stopped(false) 
{	
	_thread.start(*this);
}


Runner::~Runner()
{		
	//cout << "[Runner:" << this << "] Destroying" << endl;
	{
		FastMutex::ScopedLock lock(_mutex);	
		_stopped = true;
		_wakeUp.set();
	}
	
	// The Runner should never be destroyed inside a Task
	// callback, otherwise we will result in deadlock.
	_thread.join();
	clear();
	//cout << "[Runner:" << this << "] Destroying: OK" << endl;
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
		log("trace") << "Started Task: " << task << endl;
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
		log("trace") << "Cancelled Task: " << task << endl;
		onCancel(task);
		_wakeUp.set();
		return true;
	}
	
	return false;
}


bool Runner::destroy(Task* task)
{
	log("trace") << "Aborting Task: " << task << endl;

	if (task->_runner != this)
		throw Exception("Runner instance mismatch.");
	
	// If the task exists then set the destroyed flag.
	if (exists(task)) {
		log("trace") << "Aborting Managed Task: " << task << endl;
		task->_destroyed = true;
	}
		
	// Otherwise destroy the pointer.
	else {
		log("trace") << "Aborting Unmanaged Task: " << task << endl;
		delete task;
	}

	_wakeUp.set();
	return true; // hmmm
}
	

bool Runner::add(Task* task)
{
	log("trace") << "Adding Task: " << task << endl;
	if (!exists(task)) {
		FastMutex::ScopedLock lock(_mutex);	
		_tasks.push_back(task);
		task->_runner = this;
		log("trace") << "Added Task: " << task << endl;
		onAdd(task);
		return true;
	}
	return false;
}


bool Runner::remove(Task* task)
{	
	log("trace") << "Removing Task: " << task << endl;

	FastMutex::ScopedLock lock(_mutex);
	for (TaskList::iterator it = _tasks.begin(); it != _tasks.end(); ++it) {
		if (*it == task) {					
			_tasks.erase(it);
			log("trace") << "Removed Task: " << task << endl;
			onRemove(task);
			return true;
		}
	}
	return false;
}


bool Runner::exists(Task* task) const
{	
	log("trace") << "Check Exists: " << task << endl;

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
		log("trace") << "Clear: Destroying Task: " << *it << endl;
		delete *it;
	}
	_tasks.clear();
}


void Runner::run()
{
	while (!_stopped) 
	{
		Task* task = next();
		
		// Run the task
		if (task) {
			//log("trace") << "Before Run Task: " << task << endl;
			if (task->beforeRun()) {
				//log("trace") << "Run Task: " << task << endl;
				task->run();	
				//log("trace") << "After Task: " << task << endl;
				if (task->afterRun())			
					onRun(task);
				else
					task->_destroyed = true; //destroy();	
			}

			// Advance the task queue
			else {
				FastMutex::ScopedLock lock(_mutex);
				Task* t = _tasks.front();
				_tasks.pop_front();
				_tasks.push_back(t);
			}
						
			// Destroy the task if required
			if (task->destroyed()) {
				log("trace") << "Destroying Task: " << task << endl;
				remove(task);
				delete task;
			}
		}
		
		// Go to sleep if we have no tasks
		else {			
			log("trace") << "Sleeping" << endl;
			_wakeUp.wait();
			log("trace") << "Waking up" << endl;
		}

		// Gulp
		Thread::sleep(5);		

		// Dispatch the Idle signal
		// TODO: Idle on each complete iteration of all tasks
		Idle.emit(this);
	}	
			
	log("trace") << "Shutdown" << endl;
		
	Shutdown.emit(this);

	log("trace") << "Exiting" << endl;
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


} // namespace scy