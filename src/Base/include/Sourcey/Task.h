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


} // namespace scy


#endif // SOURCEY_Task_H
	



	//virtual TaskRunner& runner();
		// Returns a reference to the associated TaskRunner or 
		// throws an exception.
/*
	
	virtual bool beforeRun();	
		// Called by the TaskRunner to perform pre-processing, and
		// to determine weather the task should be run or not.
		// By default this method returns true if the task is not 
		// cancelled or destroying.
		// It is safe to destroy() the task from this method.
	
	virtual bool afterRun();	
		// Called by the TaskRunner to perform post-processing, and
		// to determine weather the task should be destroyed or not.
		// This method returns true by default. Return false here
		// to destroy the task.

 // TaskRunner& runner, , bool autoStart = falsebool repeat = falseal
		
	//virtual void start();
	//Task(bool repeat = false);	
template <class async::RunnableT>
class ITask: public async::RunnableT
	// This class defines an asynchronous Task which is
	// managed by a TaskRunner.
{
public:			
	virtual bool start() = 0;
	virtual bool cancel() = 0;
	virtual bool destroy() = 0;

	virtual bool cancelled() const = 0;
	virtual bool destroyed() const = 0;
	virtual bool repeating() const = 0;
		// Returns true if the task should be run once only

	virtual TaskRunner& runner();
		// Returns a reference to the affiliated TaskRunner or 
		// throws an exception.
	
protected:
	Task& operator=(Task const&) = 0; // {}
	virtual ~Task() = 0;
		// CAUTION: The destructor should be private, but we
		// left it protected for implementational flexibility. The
		// reason being that if the derived task is programmatically
		// destroyed there is a chance that the TaskRunner will call
		// run() as a pure virtual method.
	
	virtual bool beforeRun();	
		// Called by the TaskRunner to determine weather the task can
		// be run or not. It is safe to destroy() the task from
		// inside this method.
		// This method returns true by default.

	virtual void run() = 0;	
		// Called by the TaskRunner to run the task.
		// Override this method to implement task logic.
	
protected:	
	mutable Mutex	_mutex;
	
	bool _cancelled;
	bool _destroyed;
	bool _repeating;

	TaskRunner* _runner;
	
	friend class TaskRunner;
};

class TaskBase: public SocketBase<StreamSocketT, TransportT, SocketBaseT>
*/

	//std::string _name;

	//Task(const std::string& name, bool repeating = false);
	//Task(TaskRunner& runner, const std::string& name, bool repeating = false, bool autoStart = false);
	//virtual std::string name() const;
	//virtual void setName(const std::string& name);	

	/*
	//virtual void start();
	//virtual void stop();
	*/
		  
		// Called from outside to abort the task without any
		// more callbacks. The task instance will be deleted
		// shortly by the TaskRunner.	
//, 
//		  bool autoStart = false

//#include <string>

//template <class PointerDeleterT>
//class GarbageCollectorTask;
	//virtual TaskRunner& runner() { return _runner; }

//TaskRunner& runner,
		  
	/*
	virtual void runOnce() const;
	*/

	//EvLoop& _loop;	
	//bool _runOnce;
	//template <class PointerDeleterT> 
	//friend class GarbageCollectorTask;