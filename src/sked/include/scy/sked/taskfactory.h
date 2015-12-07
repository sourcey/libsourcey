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


#ifndef SCY_Sked_TaskFactory_H
#define SCY_Sked_TaskFactory_H


#include "scy/logger.h"
#include "scy/sked/task.h"
#include "scy/sked/trigger.h"
#include "scy/singleton.h"

#include <vector>


namespace scy {
namespace sked {


class Scheduler;


template<typename T> sked::Task* instantiateTask()
    { return new T; }
template<typename T> sked::Trigger* instantiateTrigger()
    { return new T; }


class TaskFactory
    /// The TaskFactory can dynamically instantiate
    /// registered sked::Task and sked::Trigger
    /// classes from named strings.
{
public:    
    static TaskFactory& getDefault()
        // Returns the default TaskFactory singleton.
    {
        static Singleton<TaskFactory> sh;
        return *sh.get();
    }


    //
    /// Scheduled Tasks

    typedef std::map<std::string, sked::Task*(*)(/*Scheduler&*/)> TaskMap;

    sked::Task* createTask(const std::string& type/*, Scheduler& scheduler*/) 
    {
        Mutex::ScopedLock lock(_mutex);
        TaskMap::iterator it = _tasks.find(type);
        if (it == _tasks.end())
            throw std::runtime_error("Failed to create scheduled task: " + type);
        return it->second();
    }
    
    template<typename T>
    void registerTask(const std::string& type)    
    { 
        Mutex::ScopedLock lock(_mutex);
        _tasks[type] = &instantiateTask<T>;
    }
    
    void unregisterTask(const std::string& type)    
    { 
        Mutex::ScopedLock lock(_mutex);
        TaskMap::iterator it = _tasks.find(type);
        if (it == _tasks.end())
            return;
        _tasks.erase(it);
    }
    
    TaskMap tasks() const
    { 
        Mutex::ScopedLock lock(_mutex);
        return _tasks;
    }
    
    
    //
    /// Schedule Triggers

    typedef std::map<std::string, sked::Trigger*(*)()> TriggerMap;

    sked::Trigger* createTrigger(const std::string& type)
    {
        Mutex::ScopedLock lock(_mutex);
        TriggerMap::iterator it = _triggers.find(type);
        if (it == _triggers.end())
            throw std::runtime_error("Failed to create scheduled trigger: " + type);
        return it->second();
    }
    
    template<typename T>
    void registerTrigger(const std::string& type)    
    { 
        Mutex::ScopedLock lock(_mutex);
        _triggers[type] = &instantiateTrigger<T>;
    }
    
    void unregisterTrigger(const std::string& type)    
    { 
        Mutex::ScopedLock lock(_mutex);
        TriggerMap::iterator it = _triggers.find(type);
        if (it == _triggers.end())
            return;
        _triggers.erase(it);
    }
    
    TriggerMap triggers() const
    { 
        Mutex::ScopedLock lock(_mutex);
        return _triggers;
    }
    
protected:
    mutable Mutex    _mutex;

    TaskMap    _tasks;
    TriggerMap _triggers;
};


} } // namespace scy::sked


#endif // SCY_Sked_TaskFactory_H
