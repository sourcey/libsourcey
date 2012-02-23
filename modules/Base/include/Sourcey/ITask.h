//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2002 Sourcey
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


#ifndef SOURCEY_ITask_H
#define SOURCEY_ITask_H


#include "Sourcey/Base.h"
#include "Poco/Mutex.h"

#include <string>


namespace Sourcey {
	

class Runner;

template <class DeletableT>
class GarbageCollectionTask;
	

class ITask
	/// This class defines a Task whose run() method
	/// will be called asynchronous by a Runner.
{
public:	
	ITask(Runner& runner,
		  bool autoStart = false, 
		  bool runOnce = false, 
		  const std::string& name = "");

	virtual bool start();
	virtual bool stop();
	virtual void destroy();
		/// Called from outside to abort the task without any
		/// more callbacks. The task instance will be deleted
		/// shortly by the Runner.	

	virtual bool runOnce() const;
	virtual bool running() const;
	
	virtual std::string name() const;
	virtual void setName(const std::string& name);
		  
	virtual Runner& runner() { return _runner; }
	
	virtual void run() = 0;
	
protected:
	ITask& operator=(ITask const&) {}
	virtual ~ITask();
		/// CAUTION: The destructor should be private, but we have left
		/// it protected for implementational flexibility. The reason
		/// being that if the derived task is programmatically destroyed
		/// there is a small chance that the Runner will call run() as
		/// a pure virtual method.

	virtual bool destroyed() const;

	
protected:
	Runner& _runner;	
	bool _runOnce;
	bool _running;
	bool _destroyed;
	std::string _name;
	
	mutable Poco::FastMutex	_mutex;
	
	friend class Runner;
	//template <class DeletableT> 
	//friend class GarbageCollectionTask;
};


} // namespace Sourcey


#endif // SOURCEY_ITask_H