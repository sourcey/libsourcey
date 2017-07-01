///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup base
/// @{


#include "scy/task.h"
#include "scy/logger.h"
#include "scy/memory.h"
#include "scy/platform.h"
#include "scy/singleton.h"
#include "scy/util.h"

#include <assert.h>
#include <iostream>


using std::endl;


namespace scy {


//
// Task
//


Task::Task(bool repeat)
    : _id(util::randomNumber())
    , _repeating(repeat)
    , _destroyed(false)
{
}


Task::~Task()
{
    // assert(destroyed());
}


void Task::destroy()
{
    _destroyed = true;
}


uint32_t Task::id() const
{
    return _id;
}


bool Task::destroyed() const
{
    return _destroyed;
}


bool Task::repeating() const
{
    return _repeating;
}


//
// Task Runner
//


TaskRunner::TaskRunner(std::shared_ptr<Runner> runner)
{
    if (runner)
        setRunner(runner);
    else
        setRunner(std::make_shared<Thread>());
}


TaskRunner::~TaskRunner()
{
    Shutdown.emit(/*this*/);
    // Idler::stop();
    if (_runner)
        _runner->cancel();
    clear();
}


bool TaskRunner::start(Task* task)
{
    add(task);

    // if (task->_cancelled) {
    // task->_cancelled = false;
    // task->start();
    LTrace("Start task: ", task)
    onStart(task);
    //_wakeUp.set();
    return true;
    //}
    // return false;
}


bool TaskRunner::cancel(Task* task)
{
    // if (!task->_cancelled) {
    // task->_cancelled = true;
    // task->cancel();
    // LTrace("Cancelled task: ", task)
    // onCancel(task);
    //_wakeUp.set();
    // return true;
    //}

    if (!task->cancelled()) {
        task->cancel();
        LTrace("Cancel task: ", task)
        onCancel(task);
        //_wakeUp.set();
        return true;
    }

    return false;
}


bool TaskRunner::destroy(Task* task)
{
    LTrace("Abort task: ", task)

    // If the task exists then set the destroyed flag.
    if (exists(task)) {
        LTrace("Abort managed task: ", task)
        task->_destroyed = true;
    }

    // Otherwise destroy the pointer.
    else {
        LTrace("Delete unmanaged task: ", task)
        delete task;
    }

    return true; // hmmm
}


bool TaskRunner::add(Task* task)
{
    LTrace("Add task: ", task)
    if (!exists(task)) {
        std::lock_guard<std::mutex> guard(_mutex);
        _tasks.push_back(task);
        onAdd(task);
        return true;
    }
    return false;
}


bool TaskRunner::remove(Task* task)
{
    LTrace("Remove task: ", task)

    std::lock_guard<std::mutex> guard(_mutex);
    for (auto it = _tasks.begin(); it != _tasks.end(); ++it) {
        if (*it == task) {
            _tasks.erase(it);
            onRemove(task);
            return true;
        }
    }
    return false;
}


bool TaskRunner::exists(Task* task) const
{
    std::lock_guard<std::mutex> guard(_mutex);
    for (auto it = _tasks.begin(); it != _tasks.end(); ++it) {
        if (*it == task)
            return true;
    }
    return false;
}


Task* TaskRunner::get(uint32_t id) const
{
    std::lock_guard<std::mutex> guard(_mutex);
    for (auto it = _tasks.begin(); it != _tasks.end(); ++it) {
        if ((*it)->id() == id)
            return *it;
    }
    return nullptr;
}


Task* TaskRunner::next() const
{
    std::lock_guard<std::mutex> guard(_mutex);
    for (auto it = _tasks.begin(); it != _tasks.end(); ++it) {
        if (!(*it)->cancelled())
            return *it;
    }
    return nullptr;
}


void TaskRunner::clear()
{
    std::lock_guard<std::mutex> guard(_mutex);
    for (auto it = _tasks.begin(); it != _tasks.end(); ++it) {
        LTrace("Clear: Destroying task: ", *it)
        delete *it;
    }
    _tasks.clear();
}


void TaskRunner::setRunner(std::shared_ptr<Runner> runner)
{
    LTrace("Set async: ", runner.get())

    std::lock_guard<std::mutex> guard(_mutex);
    assert(!_runner);
    _runner = runner;
    _runner->setRepeating(true);
    _runner->start(std::bind(&TaskRunner::run, this));
}


void TaskRunner::run()
{
    Task* task = next();
    // LTrace("Next task: ", task)

    // Run the task
    if (task) {
        // Check once more that the task has not been cancelled
        if (!task->cancelled()) {
            LTrace("Run task: ", task)
            task->run();

            onRun(task);

            // Cancel the task if not repeating
            if (!task->repeating())
                task->cancel();

            // if (task->cancelled())
            //    task->_destroyed = true;
        }

        // Advance the task queue
        {
            std::lock_guard<std::mutex> guard(_mutex);
            Task* t = _tasks.front();
            _tasks.pop_front();
            _tasks.push_back(t);
        }

        // Destroy the task if required
        if (task->destroyed()) {
            LTrace("Destroy task: ", task)
            remove(task);
            delete task;
        }
    }

    // Dispatch the Idle signal
    // LTrace("idle: "<< Idle.nslots(), )
    Idle.emit(/*this*/);

    // Prevent 100% CPU
    scy::sleep(1);
}


void TaskRunner::onAdd(Task*)
{
}


void TaskRunner::onStart(Task*)
{
}


void TaskRunner::onCancel(Task*)
{
}


void TaskRunner::onRemove(Task*)
{
}


void TaskRunner::onRun(Task*)
{
}


TaskRunner& TaskRunner::getDefault()
{
    static Singleton<TaskRunner> sh;
    return *sh.get();
}


} // namespace scy


/// @\}
