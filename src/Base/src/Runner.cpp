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

#include "Sourcey/Singleton.h"

#include <iostream>
#include <assert.h>


using namespace std;



namespace scy {


Runner::Runner(uv::Loop& loop) : 
	//_thread("Runner"),
	Idler(loop)
{	
	Idler::start(std::bind(&Runner::onIdle, this));
	//_thread.start(*this);
}


Runner::~Runner()
{	
	Shutdown.emit(this);
	Idler::stop();
	clear();
}


bool Runner::start(Task* task)
{
	add(task);
	//if (task->_cancelled) {
		//task->_cancelled = false;
		//task->start();
		log("trace") << "Started task: " << task << endl;
		onStart(task);
		//_wakeUp.set();
		return true;
	//}
	//return false;
}


bool Runner::cancel(Task* task)
{		
	//if (!task->_cancelled) {
		//task->_cancelled = true;
		//task->cancel();
		//log("trace") << "Cancelled task: " << task << endl;
		//onCancel(task);
		//_wakeUp.set();
		//return true;
	//}
	
	if (!task->cancelled()) {
		task->cancel();
		log("trace") << "Cancelled task: " << task << endl;
		onCancel(task);
		//_wakeUp.set();
		return true;
	}
	
	return false;
}


bool Runner::destroy(Task* task)
{
	log("trace") << "Aborting task: " << task << endl;
	
	// If the task exists then set the destroyed flag.
	if (exists(task)) {
		log("trace") << "Aborting Managed task: " << task << endl;
		task->_destroyed = true;
	}
		
	// Otherwise destroy the pointer.
	else {
		log("trace") << "Aborting Unmanaged task: " << task << endl;
		delete task;
	}

	return true; // hmmm
}
	

bool Runner::add(Task* task)
{
	log("trace") << "Adding task: " << task << endl;
	if (!exists(task)) {
		Mutex::ScopedLock lock(_mutex);	
		_tasks.push_back(task);
		log("trace") << "Added task: " << task << endl;		
		uv_ref(Idler::ptr.handle()); // reference the idler handle when a task is added
		onAdd(task);
		return true;
	}
	return false;
}


bool Runner::remove(Task* task)
{	
	log("trace") << "Removing task: " << task << endl;

	Mutex::ScopedLock lock(_mutex);
	for (TaskList::iterator it = _tasks.begin(); it != _tasks.end(); ++it) {
		if (*it == task) {					
			_tasks.erase(it);
			log("trace") << "Removed task: " << task << endl;
			uv_unref(Idler::ptr.handle()); // dereference the idler handle when a task is removed
			onRemove(task);
			return true;
		}
	}
	return false;
}


bool Runner::exists(Task* task) const
{	
	log("trace") << "Check Exists: " << task << endl;

	Mutex::ScopedLock lock(_mutex);
	for (TaskList::const_iterator it = _tasks.begin(); it != _tasks.end(); ++it) {
		if (*it == task)
			return true;
	}
	return false;
}


Task* Runner::get(UInt32 id) const
{
	Mutex::ScopedLock lock(_mutex);
	for (TaskList::const_iterator it = _tasks.begin(); it != _tasks.end(); ++it) {
		if ((*it)->id() == id)
			return *it;
	}			
	return nullptr;
}


Task* Runner::next() const
{
	Mutex::ScopedLock lock(_mutex);
	for (TaskList::const_iterator it = _tasks.begin(); it != _tasks.end(); ++it) {
		if (!(*it)->cancelled())
			return *it;
	}			
	return nullptr;
}


void Runner::clear()
{
	Mutex::ScopedLock lock(_mutex);
	for (TaskList::iterator it = _tasks.begin(); it != _tasks.end(); ++it) {	
		log("trace") << "Clear: Destroying task: " << *it << endl;
		delete *it;
	}
	_tasks.clear();
}


void Runner::onIdle()
{
	Task* task = next();
		
	// Run the task
	if (task) 
	{
		log("trace") << "Run task: " << task << endl;
		if (!task->cancelled()) {
			task->run();
			log("trace") << "Task destroying: " << task << endl;	
		}
		if (task->cancelled())
			task->_destroyed = true;

		onRun(task);

		// Advance the task queue
		{
			Mutex::ScopedLock lock(_mutex);
			Task* t = _tasks.front();
			_tasks.pop_front();
			_tasks.push_back(t);
		}
						
		// Destroy the task if required
		if (task->destroyed()) {
			log("trace") << "Destroy task: " << task << endl;
			remove(task);
			delete task;
		}
	}

	// Dispatch the Idle signal
	Idle.emit(this);
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
	static Singleton<Runner> sh;
	return *sh.get();
}


} // namespace scy