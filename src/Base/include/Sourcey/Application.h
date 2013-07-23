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


#ifndef SOURCEY_Application_H
#define SOURCEY_Application_H


#include "Sourcey/UV/UVPP.h"
#include "Sourcey/Memory.h"
#include "Sourcey/Exception.h"
#include "Sourcey/Singleton.h"


namespace scy {

		
class Application
	/// A simple event based application which runs until
	/// the event loop stops or is terminated.
	///
	/// The Application class also provides shutdown handling (Ctrl-C).
	///
	/// TODO: cross platform getopts
{
public:
	static Application& getDefault();
		/// Returns the default Application singleton, although
		/// Application instances may be initialized individually.
		/// The default runner should be kept for short running
		/// tasks such as timers in order to maintain performance.

	uv::Loop& loop;
		/// The active event loop.
		/// May be assigned at construction, otherwise the default
		/// event loop is used. Should only be accessed 

	Application(uv::Loop& loop = uv::defaultLoop()) : 
		loop(loop) 
	{
	}
	
	void run() 
	{ 
		uv_run(&loop, UV_RUN_DEFAULT);
	}

	void stop() 
	{ 
		uv_stop(&loop); 
	}

	void cleanup() 
	{ 
		// print active handles
		uv_walk(&loop, Application::onPrintHandle, NULL);

		// run until handles are closed 
		run(); 
	}		

	//
	// Shutdown handling
	//

	struct ShutdownCommand 
	{
		typedef void (*Fn)(void*);
		Fn callback;
		void* opaque;
	};
	
	void waitForShutdown(ShutdownCommand::Fn callback, void* opaque = nullptr)
	{ 
		ShutdownCommand* cmd = new ShutdownCommand;
		cmd->opaque = opaque;
		cmd->callback = callback;

		uv_signal_t* sig = new uv_signal_t;
		sig->data = cmd;
		uv_signal_init(&loop, sig);
		uv_signal_start(sig, Application::onShutdownSignal, SIGINT);
		
		debugL("Application") << "Wait For Kill" << std::endl;
		run();
		debugL("Application") << "Wait For Kill: Cleanup" << std::endl;
		cleanup();
		debugL("Application") << "Wait For Kill: Ending" << std::endl;
	}
			
	static void onShutdownSignal(uv_signal_t *req, int signum)
	{
		ShutdownCommand* cmd = reinterpret_cast<ShutdownCommand*>(req->data);
		uv_close((uv_handle_t*)req, uv::afterClose);
		cmd->callback(cmd->opaque);
		delete cmd;
	}
		
	static void onPrintHandle(uv_handle_t* handle, void* arg) 
	{
		debugL("Application") << "#### Active Handle: " << handle << ": " << handle->type << std::endl;
	}
};


inline Application& Application::getDefault() 
{
	static Singleton<Application> sh;
	return *sh.get();
}


/*
#include "Sourcey/UV/UVPP.h"
#include "Sourcey/Task.h"
#include "Sourcey/Signal.h"
#include "Sourcey/Polymorphic.h"
#include "Poco/Thread.h"
#include "Poco/Event.h"

#include <deque>
*/

/*
class Application: public abstract::Runnable, public Polymorphic
	/// The Application is an asynchronous event loop in charge
	/// of one or many tasks. 
	///
	/// The Application continually loops through each task in
	/// the task list calling the task's run() method.
{
public:
	Application();
	virtual ~Application();
	
	virtual bool start(Task* task);
		/// Starts a task, adding it if it doesn't exist.

	virtual bool cancel(Task* task);
		/// Cancels a task.
		/// The task reference will be managed the Application
		/// until the task is destroyed.

	virtual bool destroy(Task* task);
		/// Queues a task for destruction.

	virtual bool exists(Task* task) const;
		/// Returns weather or not a task exists.

	virtual Task* get(UInt32 id) const;
		/// Returns the task pointer matching the given ID, 
		/// or NULL if no task exists.

	static Application& getDefault();
		/// Returns the default Application singleton, although
		/// Application instances may be initialized individually.
		/// The default runner should be kept for short running
		/// tasks such as timers in order to maintain performance.
	
	NullSignal Idle;
	NullSignal Shutdown;
	
	virtual const char* className() const { return "Application"; }
		
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
	
	mutable Mutex	_mutex;
	TaskList		_tasks;
	Poco::Thread	_thread;
	Poco::Event		_wakeUp;
	bool			_stopped;
};
*/


} // namespace scy


#endif // SOURCEY_Application_H





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

	TaskEntry(Task* task = nullptr, bool repeat = true, bool running = false) : 
		task(task), repeat(repeat), running(running), destroy(false) {}

	virtual bool beforeRun() { return running; }
};


struct Scheduler::TaskEntry: public TaskEntry
{
	Timestamp scheduleAt;	
	//TimeDiff interval;

	Scheduler::TaskEntry() {}
		
	//: TaskEntry() : task(nullptr), repeat(true), running(false) {}
	//long timeout;
};
*/

	//virtual bool running(Task* task) const;
		/// Returns weather or not a task is started.
	
	//virtual void wakeUp();
		/// Tells the runner to wake up and loop internal tasks.
	
	 //, bool repeat = true
	//virtual bool schedule(Task* task, const DateTime& runAt);
	//virtual bool scheduleRepeated(Task* task, const Timespan& interval);

	 //Task*
	//virtual TaskEntry& get(Task* task) const;
		/// Returns the TaskEntry for a task.