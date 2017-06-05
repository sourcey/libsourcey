///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup sched
/// @{


#ifndef SCY_Sched_TaskFactory_H
#define SCY_Sched_TaskFactory_H


#include "scy/sched/sched.h"
#include "scy/sched/task.h"
#include "scy/sched/trigger.h"
#include "scy/logger.h"
#include "scy/singleton.h"

#include <vector>


namespace scy {
namespace sched {


class Sched_API Scheduler;


template <typename T> sched::Task* instantiateTask()
{
    return new T;
}

template <typename T> sched::Trigger* instantiateTrigger()
{
    return new T;
}


/// The TaskFactory can dynamically instantiate
/// registered sched::Task and sched::Trigger
/// classes from named strings.
class Sched_API TaskFactory
{
public:
    /// Returns the default TaskFactory singleton.
    static TaskFactory& getDefault()
    {
        static Singleton<TaskFactory> sh;
        return *sh.get();
    }

    /// Scheduled Tasks

    typedef std::map<std::string, sched::Task* (*)(/*Scheduler&*/)> TaskMap;

    sched::Task* createTask(const std::string& type/*, Scheduler& scheduler*/)
    {
        std::lock_guard<std::mutex> guard(_mutex);
        auto it = _tasks.find(type);
        if (it == _tasks.end())
            throw std::runtime_error("Failed to create scheduled task: " + type);
        return it->second();
    }

    template <typename T> void registerTask(const std::string& type)
    {
        std::lock_guard<std::mutex> guard(_mutex);
        _tasks[type] = &instantiateTask<T>;
    }

    void unregisterTask(const std::string& type)
    {
        std::lock_guard<std::mutex> guard(_mutex);
        auto it = _tasks.find(type);
        if (it == _tasks.end())
            return;
        _tasks.erase(it);
    }

    TaskMap tasks() const
    {
        std::lock_guard<std::mutex> guard(_mutex);
        return _tasks;
    }

    //
    /// Schedule Triggers

    typedef std::map<std::string, sched::Trigger* (*)()> TriggerMap;

    sched::Trigger* createTrigger(const std::string& type)
    {
        std::lock_guard<std::mutex> guard(_mutex);
        auto it = _triggers.find(type);
        if (it == _triggers.end())
            throw std::runtime_error("Failed to create scheduled trigger: " + type);
        return it->second();
    }

    template <typename T> void registerTrigger(const std::string& type)
    {
        std::lock_guard<std::mutex> guard(_mutex);
        _triggers[type] = &instantiateTrigger<T>;
    }

    void unregisterTrigger(const std::string& type)
    {
        std::lock_guard<std::mutex> guard(_mutex);
        auto it = _triggers.find(type);
        if (it == _triggers.end())
            return;
        _triggers.erase(it);
    }

    TriggerMap triggers() const
    {
        std::lock_guard<std::mutex> guard(_mutex);
        return _triggers;
    }

protected:
    mutable std::mutex _mutex;

    TaskMap _tasks;
    TriggerMap _triggers;
};


} // namespace sched
} // namespace scy


#endif // SCY_Sched_TaskFactory_H


/// @\}
