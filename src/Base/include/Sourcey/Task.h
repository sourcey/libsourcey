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


#ifndef SOURCEY_Task_H
#define SOURCEY_Task_H


#include "Sourcey/Types.h"
#include "Sourcey/Interface.h"
#include "Sourcey/Mutex.h"


namespace scy {
	

class Runner;


class Task: public basic::Runnable
	/// This class is for implementing any kind 
	/// async task that is compatible with a Runner.
{
public:	
	Task(bool repeat = false);
	
	virtual void destroy();
		// Sets the task to destroyed state.

	virtual bool destroyed() const;
		// Signals that the task should be disposed of.

	virtual bool repeating() const;
		// Signals that the task's should be called
		// repeatedly by the Runner.
		// If this returns false the task will be cancelled()

	virtual UInt32 id() const;
		// Unique task ID.
	
	// basic::Runnable:
	//
	// virtual void run();
	// virtual void cancel();
	// virtual bool cancelled() const;
	
protected:
	Task& operator=(Task const&) {}
	virtual ~Task();
		// Should remain protected.

	virtual void run() = 0;	
		// Called by the Runner to run the task.
		// Override this method to implement task action.
		// Returning true means the true should be called again,
		// and false will cause the task to be destroyed.
		// The task will similarly be destroyed id destroy()
		// was called during the current task iteration.

	friend class Runner;
		// Tasks belong to a Runner instance.

	bool _destroyed;
	bool _repeating;
	UInt32 _id;
};


} // namespace scy


#endif // SOURCEY_Task_H
	



	//virtual Runner& runner();
		// Returns a reference to the associated Runner or 
		// throws an exception.
/*
	
	virtual bool beforeRun();	
		// Called by the Runner to perform pre-processing, and
		// to determine weather the task should be run or not.
		// By default this method returns true if the task is not 
		// cancelled or destroying.
		// It is safe to destroy() the task from this method.
	
	virtual bool afterRun();	
		// Called by the Runner to perform post-processing, and
		// to determine weather the task should be destroyed or not.
		// This method returns true by default. Return false here
		// to destroy the task.

 // Runner& runner, , bool autoStart = falsebool repeat = falseal
		
	//virtual void start();
	//Task(bool repeat = false);	
template <class basic::RunnableT>
class ITask: public basic::RunnableT
	// This class defines an asynchronous Task which is
	// managed by a Runner.
{
public:			
	virtual bool start() = 0;
	virtual bool cancel() = 0;
	virtual bool destroy() = 0;

	virtual bool cancelled() const = 0;
	virtual bool destroyed() const = 0;
	virtual bool repeating() const = 0;
		// Returns true if the task should be run once only

	virtual Runner& runner();
		// Returns a reference to the affiliated Runner or 
		// throws an exception.
	
protected:
	Task& operator=(Task const&) = 0; // {}
	virtual ~Task() = 0;
		// CAUTION: The destructor should be private, but we
		// left it protected for implementational flexibility. The
		// reason being that if the derived task is programmatically
		// destroyed there is a chance that the Runner will call
		// run() as a pure virtual method.
	
	virtual bool beforeRun();	
		// Called by the Runner to determine weather the task can
		// be run or not. It is safe to destroy() the task from
		// inside this method.
		// This method returns true by default.

	virtual void run() = 0;	
		// Called by the Runner to run the task.
		// Override this method to implement task logic.
	
protected:	
	mutable Mutex	_mutex;
	
	bool _cancelled;
	bool _destroyed;
	bool _repeating;

	Runner* _runner;
	
	friend class Runner;
};

class TaskBase: public SocketBase<StreamSocketT, TransportT, SocketBaseT>
*/

	//std::string _name;

	//Task(const std::string& name, bool repeating = false);
	//Task(Runner& runner, const std::string& name, bool repeating = false, bool autoStart = false);
	//virtual std::string name() const;
	//virtual void setName(const std::string& name);	

	/*
	//virtual void start();
	//virtual void stop();
	*/
		  
		// Called from outside to abort the task without any
		// more callbacks. The task instance will be deleted
		// shortly by the Runner.	
//, 
//		  bool autoStart = false

//#include <string>

//template <class AsyncDeleterT>
//class GarbageCollectorTask;
	//virtual Runner& runner() { return _runner; }

//Runner& runner,
		  
	/*
	virtual void runOnce() const;
	*/

	//EvLoop& _loop;	
	//bool _runOnce;
	//template <class AsyncDeleterT> 
	//friend class GarbageCollectorTask;