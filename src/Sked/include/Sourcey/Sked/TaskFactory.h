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


#ifndef SOURCEY_Sked_TaskFactory_H
#define SOURCEY_Sked_TaskFactory_H


#include "Sourcey/Logger.h"
#include "Sourcey/Sked/Task.h"
#include "Sourcey/Sked/Trigger.h"

#include "Poco/Thread.h"
#include "Poco/Event.h"
#include "Poco/SingletonHolder.h"

#include <vector>


namespace Sourcey {
namespace Sked {


class Scheduler;


template<typename T> Sked::Task* instantiateTask() //Scheduler& scheduler
	{ return new T; } //(scheduler)
template<typename T> Sked::Trigger* instantiateTrigger()
	{ return new T; }


class TaskFactory
	/// The TaskFactory can dynamically instantiate
	/// registered Sked::Task and Sked::Trigger
	/// classes from named strings.
{
public:	
	static TaskFactory& getDefault()
		/// Returns the default TaskFactory singleton.
	{
		static Poco::SingletonHolder<TaskFactory> sh;
		return *sh.get();
	}


	//
	// Scheduled Tasks
	//
	typedef std::map<std::string, Sked::Task*(*)(/*Scheduler&*/)> TaskMap;

    Sked::Task* createTask(const std::string& name/*, Scheduler& scheduler*/) 
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
        TaskMap::iterator it = _tasks.find(name);
        if (it == _tasks.end())
			throw Exception("Failed to create scheduled task.");
            //return NULL;
        return it->second(); //scheduler);
    }
	
	template<typename T>
    void registerTask(const std::string& name)	
	{ 
		Poco::FastMutex::ScopedLock lock(_mutex);
        _tasks[name] = &instantiateTask<T>;
    }
	
    void unregisterTask(const std::string& name)	
	{ 
		Poco::FastMutex::ScopedLock lock(_mutex);
        TaskMap::iterator it = _tasks.find(name);
        if (it == _tasks.end())
            return;
		_tasks.erase(it);
    }
	
    TaskMap tasks() const
	{ 
		Poco::FastMutex::ScopedLock lock(_mutex);
		return _tasks;
    }
	
	
	//
	// Schedule Triggers
	//
	typedef std::map<std::string, Sked::Trigger*(*)()> TriggerMap;

    Sked::Trigger* createTrigger(const std::string& name) //, TriggerSked& scheduler
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
        TriggerMap::iterator it = _triggers.find(name);
        if (it == _triggers.end())
			throw Exception("Failed to create scheduled trigger.");
            //return NULL;
        return it->second(); //scheduler
    }
	
	template<typename T>
    void registerTrigger(const std::string& name)	
	{ 
		Poco::FastMutex::ScopedLock lock(_mutex);
        _triggers[name] = &instantiateTrigger<T>;
    }
	
    void unregisterTrigger(const std::string& name)	
	{ 
		Poco::FastMutex::ScopedLock lock(_mutex);
        TriggerMap::iterator it = _triggers.find(name);
        if (it == _triggers.end())
            return;
		_triggers.erase(it);
    }
	
    TriggerMap triggers() const
	{ 
		Poco::FastMutex::ScopedLock lock(_mutex);
		return _triggers;
    }
	
protected:
	mutable Poco::FastMutex	_mutex;

    TaskMap    _tasks;
    TriggerMap _triggers;
};


} } // namespace Sourcey::Sked


#endif // SOURCEY_Sked_TaskFactory_H
