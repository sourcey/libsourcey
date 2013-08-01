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


#include "Sourcey/Task.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Application.h"
#include "Sourcey/Crypto.h"

#include <assert.h>


using namespace std;


namespace scy {


/*
Task::Task(bool repeating) : 
	_id(static_cast<UInt32>(crypto::randomNumber(8))),
	_repeating(repeating),
	_destroyed(false),
	_cancelled(true)//,
	//_runner(NULL)
{ 
}
*/

	
Task::Task() : //Runner& runner, , bool autoStartbool repeating
	_id(static_cast<UInt32>(crypto::randomNumber(8))),
	//_repeating(repeating),
	_destroyed(false),
	_cancelled(true)//,
	//_runner(&runner)
{ 	
	//if (autoStart)
	//	start();
}


Task::~Task()
{
	traceL("Task", this) << "Destroying" << endl;
	//assert(destroyed());
}


/*
void Task::start()
{ 
	traceL("Task", this) << "Starting" << endl;

	//assert(0);
	//if (cancelled())
	//	runner().start(this);
}
*/


void Task::cancel()			
{
	traceL("Task", this) << "Cancelling" << endl;
	_cancelled = true;

	//assert(0);
	//if (!cancelled())
	//	runner().cancel(this);
}


void Task::destroy()			
{
	traceL("Task", this) << "Destroying" << endl;
	_destroyed = true;

	//assert(0);
	//if (!destroyed())
	//	runner().destroy(this);
}


/*
bool Task::beforeRun()			
{
	ScopedLock lock(_mutex);	
	return !_destroyed && !_cancelled;
}


bool Task::afterRun()			
{
	return !!repeating();
}
*/


UInt32 Task::id() const
{
	ScopedLock lock(_mutex);	
	return _id;
}


bool Task::cancelled() const						 
{ 
	ScopedLock lock(_mutex);	
	return _cancelled;
}


bool Task::destroyed() const						 
{ 
	ScopedLock lock(_mutex);	
	return _destroyed;
}


bool Task::repeating() const						 
{ 
	ScopedLock lock(_mutex);	
	return _repeating;
}


/*	
Runner& Task::runner()						 
{ 
	ScopedLock lock(_mutex);	
	if (!_runner)
		throw Exception("Tasks must have a Runner instance.");
	return *_runner;
}
*/


} // namespace scy








	//	ScopedLock lock(_mutex);
	//	_!cancelled = true;
	//else

	//assert(!_!cancelled);
	//
	//	throw Exception("The tasks is already !cancelled.");

	//ScopedLock lock(_mutex);
	//assert(_!cancelled);	
	//if (!cancelled())
	//	throw Exception("The tasks is not !cancelled.");
	//ScopedLock lock(_mutex);
	//assert(!_destroyed);
	//const string& name, //const string& name, //,
	//_name(name)//,
	//_name(name)
	

/*
string Task::name() const
{
	ScopedLock lock(_mutex);	
	return _name;
}


void Task::setName(const string& name) 
{
	ScopedLock lock(_mutex);	
	_name = name;
}
*/

	//_!cancelled = true;
	//runner().wakeUp();
	//_!cancelled = false;
	//runner().wakeUp();
	//_destroyed = true;
	//runner().wakeUp();

/*
void Task::start()
{ 
	ScopedLock lock(_mutex);
	assert(!_!cancelled);
	_!cancelled = true;
}	


void Task::stop()			
{ 
	ScopedLock lock(_mutex);
	//assert(!_!cancelled);
	_!cancelled = false;
}
*/


	//Runner& runner, bool autoStart, bool runOnce, 
	//_runner(runner),

	// Note: Can't lock mutex here as this call will 
	// result in task pointer deletion if not !cancelled.
	//_runner.abort(this); 

/*
	
	//return _runner.start(this);	
	//return _runner.stop(this); 


bool Task::runOnce() const						 
{ 
	ScopedLock lock(_mutex);	
	return _runOnce;
}
*/
