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


#include "Sourcey/Task.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Runner.h"
#include "Sourcey/CryptoProvider.h"

#include <assert.h>


using namespace std;


namespace Sourcey {

	
Task::Task(bool repeating) : 
	_id(CryptoProvider::generateRandomNumber(8)),
	_repeating(repeating),
	_destroyed(false),
	_cancelled(true),
	_runner(NULL)
{ 
}

	
Task::Task(Runner& runner, bool repeating, bool autoStart) : 
	_id(CryptoProvider::generateRandomNumber(8)),
	_repeating(repeating),
	_destroyed(false),
	_cancelled(true),
	_runner(&runner)
{ 	
	if (autoStart)
		start();
}


Task::~Task()
{
	Log("trace") << "[Task:" << this << "] Destroying" << endl;
	//assert(destroyed());
}


void Task::start()
{ 
	Log("trace") << "[Task:" << this << "] Starting" << endl;
	if (cancelled())
		runner().start(this);
}


void Task::cancel()			
{
	Log("trace") << "[Task:" << this << "] Stopping" << endl;
	if (!cancelled())
		runner().cancel(this);
}


void Task::destroy()			
{
	Log("trace") << "[Task:" << this << "] Destroying" << endl;
	if (!destroyed())
		runner().destroy(this);
}


bool Task::beforeRun()			
{
	Poco::FastMutex::ScopedLock lock(_mutex);	
	return !_destroyed && !_cancelled;
}


UInt32 Task::id() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);	
	return _id;
}


bool Task::cancelled() const						 
{ 
	Poco::FastMutex::ScopedLock lock(_mutex);	
	return _cancelled;
}


bool Task::afterRun()			
{
	return repeating();
}


bool Task::destroyed() const						 
{ 
	Poco::FastMutex::ScopedLock lock(_mutex);	
	return _destroyed;
}


bool Task::repeating() const						 
{ 
	Poco::FastMutex::ScopedLock lock(_mutex);	
	return _repeating;
}


Runner& Task::runner()						 
{ 
	Poco::FastMutex::ScopedLock lock(_mutex);	
	if (!_runner)
		throw Exception("Tasks must be started with a Runner instance.");
	return *_runner;
}


} // namespace Sourcey








	//	Poco::FastMutex::ScopedLock lock(_mutex);
	//	_!cancelled = true;
	//else

	//assert(!_!cancelled);
	//
	//	throw Exception("The tasks is already !cancelled.");

	//Poco::FastMutex::ScopedLock lock(_mutex);
	//assert(_!cancelled);	
	//if (!cancelled())
	//	throw Exception("The tasks is not !cancelled.");
	//Poco::FastMutex::ScopedLock lock(_mutex);
	//assert(!_destroyed);
	//const string& name, //const string& name, //,
	//_name(name)//,
	//_name(name)
	

/*
string Task::name() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);	
	return _name;
}


void Task::setName(const string& name) 
{
	Poco::FastMutex::ScopedLock lock(_mutex);	
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
	Poco::FastMutex::ScopedLock lock(_mutex);
	assert(!_!cancelled);
	_!cancelled = true;
}	


void Task::stop()			
{ 
	Poco::FastMutex::ScopedLock lock(_mutex);
	//assert(!_!cancelled);
	_!cancelled = false;
}
*/


	//Runner& runner, bool autoStart, bool runOnce, 
	//_runner(runner),

	// NOTE: Can't lock mutex here as this call will 
	// result in task pointer deletion if not !cancelled.
	//_runner.abort(this); 

/*
	
	//return _runner.start(this);	
	//return _runner.stop(this); 


bool Task::runOnce() const						 
{ 
	Poco::FastMutex::ScopedLock lock(_mutex);	
	return _runOnce;
}
*/
