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


#include "Sourcey/Task.h"
#include "Sourcey/Signal.h"
#include "Sourcey/IPolymorphic.h"
#include "Poco/Thread.h"
#include "Poco/Event.h"

#include <deque>


namespace Scy {


class Runner: public Poco::Runnable, public IPolymorphic
	/// The Runner is an asynchronous event loop in charge
	/// of one or many tasks. 
	///
	/// The Runner continually loops through each task in
	/// the task list calling the task's run() method.
{
public:
	Runner();
	virtual ~Runner();
	
	virtual bool start(Task* task);
		/// Starts a task, adding it if it doesn't exist.

	virtual bool cancel(Task* task);
		/// Cancels a task.
		/// The task reference will be managed the Runner
		/// until the task is destroyed.

	virtual bool destroy(Task* task);
		/// Queues a task for destruction.

	virtual bool exists(Task* task) const;
		/// Returns weather or not a task exists.

	virtual Task* get(UInt32 id) const;
		/// Returns the task pointer matching the given ID, 
		/// or NULL if no task exists.

	static Runner& getDefault();
		/// Returns the default Runner singleton, although
		/// Runner instances may be initialized individually.
		/// The default runner should be kept for short running
		/// tasks such as timers in order to maintain performance.
	
	template <class T>
	void deleteLater(void* ptr)
		/// Schedules a pointer for asynchronous deletion.
	{
		(void)new GarbageCollectionTask<T>(*this, ptr);
	}
	
	NullSignal Idle;
	NullSignal Shutdown;
	
	virtual const char* className() const { return "Runner"; }
		
protected:
	virtual void run();
		/// Called by the thread to run managed tasks.
	
	virtual bool add(Task* task);
		/// Adds a task to the runner.
	
	virtual bool remove(Task* task);
		/// Removes a task from the runner.

	virtual Task* next() const;
		/// Returns the next task to be run.
	
	virtual void clear();
		/// Destroys and clears all manages tasks.
		
	virtual void onAdd(Task* task);
		/// Called after a task is added.
		
	virtual void onStart(Task* task);
		/// Called after a task is started.
		
	virtual void onCancel(Task* task);
		/// Called after a task is cancelled.
	
	virtual void onRemove(Task* task);
		/// Called after a task is removed.
	
	virtual void onRun(Task* task);
		/// Called after a task has run.

protected:
	typedef std::deque<Task*> TaskList;
	
	mutable Poco::FastMutex	_mutex;
	TaskList		_tasks;
	Poco::Thread	_thread;
	Poco::Event		_wakeUp;
	bool			_stop;
};


} // namespace Scy


#endif // SOURCEY_Runner_H





	/*
	virtual Task* pop();
	virtual Task* popNonCancelled();
		/// Pop a non cancelled task from the front of the queue.

	virtual void push(Task* task);
		// Push a task onto the back of the queue.
		*/
	/*
struct TaskEntry
{
	bool repeat;
	bool running;
	bool destroy;
	Task* task;

	TaskEntry(Task* task = NULL, bool repeat = true, bool running = false) : 
		task(task), repeat(repeat), running(running), destroy(false) {}

	virtual bool beforeRun() { return running; }
};


struct Scheduler::TaskEntry: public TaskEntry
{
	Poco::Timestamp scheduleAt;	
	//Poco::TimeDiff interval;

	Scheduler::TaskEntry() {}
		
	//: TaskEntry() : task(NULL), repeat(true), running(false) {}
	//long timeout;
};
*/

	//virtual bool running(Task* task) const;
		/// Returns weather or not a task is started.
	
	//virtual void wakeUp();
		/// Tells the runner to wake up and loop internal tasks.
	
	 //, bool repeat = true
	//virtual bool schedule(Task* task, const Poco::DateTime& runAt);
	//virtual bool scheduleRepeated(Task* task, const Poco::Timespan& interval);

	 //Task*
	//virtual TaskEntry& get(Task* task) const;
		/// Returns the TaskEntry for a task.