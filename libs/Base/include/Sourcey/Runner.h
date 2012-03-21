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


#include "Sourcey/ITask.h"
#include "Sourcey/Signal.h"
#include "Poco/Thread.h"
#include "Poco/Event.h"

#include <deque>


namespace Sourcey {


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
	
	virtual bool start(ITask* task);
	virtual bool stop(ITask* task);
	virtual void abort(ITask* task);

	virtual bool running(ITask* task) const;
		// Returns weather or not a task is currently active.

	virtual void run();
		// Called by the thread to run internal tasks.

	static Runner& getDefault();
		// Returns the default Runner singleton, although
		// Runner instances may be initialized individually. 
	
	template <class T>
	void deleteLater(void* ptr)
		// Schedules a pointer for asynchronous deletion.
	{
		(void)new GarbageCollectionTask<T>(*this, ptr);
	}
	
	NullSignal Idle;
	NullSignal Shutdown;

protected:
	typedef std::deque<ITask*> TaskList;

	Poco::Thread	_thread;
	TaskList		_tasks;
	Poco::Event		_wakeUp;
	bool			_stop;
	mutable Poco::Mutex	_mutex;
};


} // namespace Sourcey


#endif // SOURCEY_Runner_H