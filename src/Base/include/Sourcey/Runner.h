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


	//
	//bool			_stopped;

	//Thread	_thread;
	//Poco::Event	_wakeUp;
// -------------------------------------------------------------------
//
/*
typedef EvLoop Runner;
class Runner
{
public:
	Loop* loop;

	Runner(uv_loop_t* loop = uv_default_loop()) : 
		loop(loop) 
	{
	}
	
	bool run() 
	{ 
		uv_run(loop, UV_RUN_DEFAULT);
	}

	void stop() 
	{ 
		uv_stop(loop); 
	}

	void cleanup(bool destroy = false) 
	{ 
		// print active handles
		uv_walk(loop, Runner::onPrintHandle, NULL);

		// run until handles are closed 
		run(); 
		if (destroy) {
			uv_loop_delete(loop);
			loop = NULL;
		}
	}	

	typedef void (*ShutdownCallback)(void*);
	
	struct ShutdownCommand 
	{
		void* opaque;
		ShutdownCallback callback;
	};
	
	void waitForShutdown(ShutdownCallback callback, void* opaque = NULL)
	{ 
		ShutdownCommand* cmd = new ShutdownCommand;
		cmd->opaque = opaque;
		cmd->callback = callback;

		uv_signal_t* sig = new uv_signal_t;
		sig->data = cmd;
		uv_signal_init(loop, sig);
		uv_signal_start(sig, Runner::onKillSignal, SIGINT);
		
		debugL("Runner") << "Wait For Kill" << std::endl;
		run();
		debugL("Runner") << "Wait For Kill: Cleanup" << std::endl;
		cleanup();
		debugL("Runner") << "Wait For Kill: Cleanup 2" << std::endl;
		cleanup();
		debugL("Runner") << "Wait For Kill: Ending" << std::endl;
	}
			
	static void onKillSignal(uv_signal_t *req, int signum)
	{
		ShutdownCommand* cmd = reinterpret_cast<ShutdownCommand*>(req->data);
		uv_close((uv_handle_t*)req, uv::afterClose);
		cmd->callback(cmd->opaque);
		delete cmd;
	}
	
	static void onPrintHandle(uv_handle_t* handle, void* arg) 
	{
		debugL("Runner") << "#### Active Handle: " << handle << ": " << handle->type << std::endl;
	}

	static Runner& getDefault();
		// Returns the default Runner singleton, although
		// Runner instances may be initialized individually.
		// The default runner should be kept for short running
		// tasks such as timers in order to maintain performance.
};

inline Runner& Runner::getDefault() 
{
	static Singleton<Runner> sh;
	return *sh.get();
}
*/



/*
#include "Sourcey/UV/UVPP.h"
#include "Sourcey/Task.h"
#include "Sourcey/Signal.h"
#include "Sourcey/Polymorphic.h"

#include "Poco/Event.h"

#include <deque>
*/

/*
*/


} // namespace scy


#endif // SOURCEY_Runner_H





	/*
	virtual Task* pop();
	virtual Task* popNonCancelled();
		// Pop a non cancelled task from the front of the queue.

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
	Timestamp scheduleAt;	
	//TimeDiff interval;

	Scheduler::TaskEntry() {}
		
	//: TaskEntry() : task(NULL), repeat(true), running(false) {}
	//long timeout;
};
*/

	//virtual bool running(Task* task) const;
		// Returns weather or not a task is started.
	
	//virtual void wakeUp();
		// Tells the runner to wake up and loop internal tasks.
	
	 //, bool repeat = true
	//virtual bool schedule(Task* task, const DateTime& runAt);
	//virtual bool scheduleRepeated(Task* task, const Timespan& interval);

	 //Task*
	//virtual TaskEntry& get(Task* task) const;
		// Returns the TaskEntry for a task.