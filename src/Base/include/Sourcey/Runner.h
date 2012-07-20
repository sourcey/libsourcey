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


#ifndef SOURCEY_Runner_H
#define SOURCEY_Runner_H


#include "Sourcey/Task.h"
#include "Sourcey/Signal.h"
#include "Poco/Thread.h"
#include "Poco/Event.h"

#include <deque>


namespace Sourcey {


	/*
struct TaskEntry
{
	bool repeat;
	bool running;
	bool destroy;
	Task* task;

	TaskEntry(Task* task = NULL, bool repeat = true, bool running = false) : 
		task(task), repeat(repeat), running(running), destroy(false) {}

	virtual bool canRun() { return running; }
};


struct ScheduledTaskEntry: public TaskEntry
{
	Poco::Timestamp scheduleAt;	
	//Poco::TimeDiff interval;

	ScheduledTaskEntry() {}
		
	//: TaskEntry() : task(NULL), repeat(true), running(false) {}
	//long timeout;
};
*/


class Runner: public Poco::Runnable
	/// The Runner is an asynchronous event loop in charge
	/// of one or many tasks. 
	///
	/// The Runner continually loops through each task in
	/// the task list calling the task's run() method.
{
public:
	Runner();
	virtual ~Runner();
	
	virtual bool add(Task* task);
		/// Adds a task to the runner.

	virtual bool start(Task* task);
		/// Starts a task, adding it if it doesn't exist.

	virtual bool stop(Task* task);
		/// Stops a task.
		/// The task reference will be managed the Runner
		/// until the task is destroyed.

	virtual bool destroy(Task* task);
		/// Queues a task for destruction.
	
	virtual bool exists(Task* task) const;
		/// Returns weather or not a task exists.

	//virtual bool running(Task* task) const;
		/// Returns weather or not a task is started.
	
	//virtual void wakeUp();
		/// Tells the runner to wake up and loop internal tasks.

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
		
protected:
	virtual void run();
		/// Called by the thread to run internal tasks.

protected:
	typedef std::deque<Task*> TaskList;

	Poco::Thread	_thread;
	TaskList		_tasks;
	Poco::Event		_wakeUp;
	bool			_stop;
	mutable Poco::Mutex	_mutex;
};


} // namespace Sourcey


#endif // SOURCEY_Runner_H


	
	 //, bool repeat = true
	//virtual bool schedule(Task* task, const Poco::DateTime& runAt);
	//virtual bool scheduleRepeated(Task* task, const Poco::Timespan& interval);

	 //Task*
	//virtual TaskEntry& get(Task* task) const;
		/// Returns the TaskEntry for a task.