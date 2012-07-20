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


#ifndef SOURCEY_GarbageCollection_H
#define SOURCEY_GarbageCollection_H


#include "Sourcey/Base.h"
#include "Sourcey/Task.h"


namespace Sourcey {
	

class Runner;


template <class DeletableT>
class GarbageCollectionTask: public Task
{
public:
	GarbageCollectionTask(Runner& runner, void* ptr) : 
	  Task(runner, false, true), _ptr(ptr) {
		  /*
#ifdef _DEBUG
		  ostringstream ss;
		  ss << "GarbageCollection[" << _ptr << "]";
		  _name = ss.str();
#endif
		  */
	}

	~GarbageCollectionTask() {
		if (_ptr)
			free();
	}
	
	virtual void free() {
		if (_ptr) {
			DeletableT* ptr = static_cast<DeletableT*>(_ptr);
			delete ptr;
			_ptr = NULL;
		}
	}

	virtual void run() {
		free();
	}
	
private:
	void*	_ptr;
};


// ---------------------------------------------------------------------
//
template <class DeletableT>
class DelayedGarbageCollectionTask: public Task
{
public:
	DelayedGarbageCollectionTask(Runner& runner, void* ptr, UInt32 delay = 100) :
	  Task(runner, true, true), _ptr(ptr), _timeout(delay, true) {
		  /*
#ifdef _DEBUG
		  ostringstream ss;
		  ss << "TimedGarbageCollection[" << _ptr << "]";
		  _name = ss.str();
#endif
		  */
	}

	~DelayedGarbageCollectionTask() {
		if (_ptr)
			free();
	}
	
	virtual void free() {
		if (_ptr) {
			DeletableT* ptr = static_cast<DeletableT*>(_ptr);
			delete ptr;
			_ptr = NULL;
		}
	}

	virtual void run() {
		if (_timeout.expired()) {		
			free();
			destroy();
		}
	}
	
private:
	void*	_ptr;
	Timeout	_timeout;
};


} // namespace Sourcey


#endif // SOURCEY_GarbageCollection_H