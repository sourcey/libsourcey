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


#ifndef SOURCEY_Runner_H
#define SOURCEY_Runner_H


#include "Sourcey/UV/UVPP.h"
#include "Sourcey/Memory.h"
#include "Sourcey/Interfaces.h"
#include "Sourcey/Polymorphic.h"
#include "Sourcey/Signal.h"
#include "Sourcey/Task.h"
#include "Sourcey/Idler.h"


namespace scy {

	
class Runner: public Idler, public Polymorphic
	// The Runner is an asynchronous event loop in 
	// charge of running one or many tasks. 
	//
	// The Runner continually loops through each task in
	// the task list calling the task's run() method.
	//
	// TODO: 
	//	- Different async sources ie. TimedRunner, 
	//   IdleRunner, ThreadedRunner
{
public:
	Runner(uv::Loop& loop = uv::defaultLoop());
	virtual ~Runner();
	
	virtual bool start(Task* task);
		// Starts a task, adding it if it doesn't exist.

	virtual bool cancel(Task* task);
		// Cancels a task.
		// The task reference will be managed the Runner
		// until the task is destroyed.

	virtual bool destroy(Task* task);
		// Queues a task for destruction.

	virtual bool exists(Task* task) const;
		// Returns weather or not a task exists.

	virtual Task* get(UInt32 id) const;
		// Returns the task pointer matching the given ID, 
		// or NULL if no task exists.

	static Runner& getDefault();
		// Returns the default Runner singleton, although
		// Runner instances may be initialized individually.
		// The default runner should be kept for short running
		// tasks such as timers in order to maintain performance.
	
	NullSignal Idle;	
		// Fires after completing an iteration of all tasks.

	NullSignal Shutdown;
		// Fires when the Runner is shutting down.
	
	virtual const char* className() const { return "Runner"; }
		
protected:
	virtual void onIdle();
		// Called by the thread to run managed tasks.
	
	virtual bool add(Task* task);
		// Adds a task to the runner.
	
	virtual bool remove(Task* task);
		// Removes a task from the runner.

	virtual Task* next() const;
		// Returns the next task to be run.
	
	virtual void clear();
		// Destroys and clears all manages tasks.
		
	virtual void onAdd(Task* task);
		// Called after a task is added.
		
	virtual void onStart(Task* task);
		// Called after a task is started.
		
	virtual void onCancel(Task* task);
		// Called after a task is cancelled.
	
	virtual void onRemove(Task* task);
		// Called after a task is removed.
	
	virtual void onRun(Task* task);
		// Called after a task has run.

protected:
	typedef std::deque<Task*> TaskList;
	
	mutable Mutex	_mutex;
	TaskList		_tasks;
	Idler			_idler;
};


} // namespace scy


#endif // SOURCEY_Runner_H