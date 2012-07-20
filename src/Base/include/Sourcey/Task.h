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


#ifndef SOURCEY_Task_H
#define SOURCEY_Task_H


#include "Sourcey/Base.h"

#include "Poco/Mutex.h"


namespace Sourcey {
	

class Runner;
	

class Task
	/// This class defines an asynchronous Task which is managed
	/// by a Runner.
{
public:	
	Task(bool repeating = false);	
	Task(Runner& runner, bool repeating = false, bool autoStart = false);
		
	virtual bool start();
	virtual bool stop();
	virtual bool destroy();

	virtual bool running() const;
	virtual bool destroyed() const;
	virtual bool repeating() const;

	virtual Runner& runner();
		/// Returns a reference to the affiliated Runner or 
		/// throws an exception.
	
protected:
	Task& operator=(Task const&) {}
	virtual ~Task();
		/// CAUTION: The destructor should be private, but we
		/// left it protected for implementational flexibility. The
		/// reason being that if the derived task is programmatically
		/// destroyed there is a chance that the Runner will call
		/// run() as a pure virtual method.
	
	virtual bool canRun();	
		/// Called by the Runner to determine weather the task can
		/// be run or not. It is safe to destroy() the task from
		/// inside this method.
		/// This method returns true by default.

	virtual void run() = 0;	
		/// Called by the Runner to run the task.
		/// Override this method to implement task logic.
	
protected:	
	mutable Poco::FastMutex	_mutex;
	
	bool _running;
	bool _destroyed;
	bool _repeating;

	Runner* _runner;
	
	friend class Runner;
};


} // namespace Sourcey


#endif // SOURCEY_Task_H


	

	//std::string _name;

	//Task(const std::string& name, bool repeating = false);
	//Task(Runner& runner, const std::string& name, bool repeating = false, bool autoStart = false);
	//virtual std::string name() const;
	//virtual void setName(const std::string& name);	

	/*
	//virtual void start();
	//virtual void stop();
	*/
		  
		/// Called from outside to abort the task without any
		/// more callbacks. The task instance will be deleted
		/// shortly by the Runner.	
//, 
//		  bool autoStart = false

//#include <string>

//template <class DeletableT>
//class GarbageCollectionTask;
	//virtual Runner& runner() { return _runner; }

//Runner& runner,
		  
	/*
	virtual bool runOnce() const;
	*/

	//Runner& _runner;	
	//bool _runOnce;
	//template <class DeletableT> 
	//friend class GarbageCollectionTask;