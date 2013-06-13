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


#ifndef SOURCEY_GarbageCollection_H
#define SOURCEY_GarbageCollection_H


#include "Sourcey/Types.h"
#include "Sourcey/Task.h"


namespace Scy {
	

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


} // namespace Scy


#endif // SOURCEY_GarbageCollection_H