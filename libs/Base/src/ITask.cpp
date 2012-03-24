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


#include "Sourcey/ITask.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Runner.h"

#include <assert.h>


using namespace std;


namespace Sourcey {

	
ITask::ITask(Runner& runner, bool autoStart, bool runOnce, const std::string& name) :   
	_runner(runner),
	_runOnce(runOnce),
	_running(false),
	_destroyed(false),
	_name(name)
{
	if (autoStart)
		start();
}


ITask::~ITask()
{
	Log("trace") << "[ITask: " << this << "] Destroying" << endl;
	assert(_runOnce || !_running);
}


bool ITask::start()
{ 
	Poco::FastMutex::ScopedLock lock(_mutex);	
	assert(!_running);
	return _runner.start(this);	
}	


bool ITask::stop()			
{ 
	Poco::FastMutex::ScopedLock lock(_mutex);	
	return _runner.stop(this); 
}


void ITask::destroy()			
{
	Poco::FastMutex::ScopedLock lock(_mutex);	
	_runner.abort(this); 
}


bool ITask::runOnce() const						 
{ 
	Poco::FastMutex::ScopedLock lock(_mutex);	
	return _runOnce;
}


bool ITask::running() const						 
{ 
	Poco::FastMutex::ScopedLock lock(_mutex);	
	return _running;
}


bool ITask::destroyed() const						 
{ 
	Poco::FastMutex::ScopedLock lock(_mutex);	
	return _destroyed;
}

	
std::string ITask::name() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);	
	return _name;
}


void ITask::setName(const std::string& name) 
{
	Poco::FastMutex::ScopedLock lock(_mutex);	
	_name = name;
}


} // namespace Sourcey
