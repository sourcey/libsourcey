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


TaskRunner::TaskRunner(Runner::Ptr runner)
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
    TraceN(this) << "Start task: " << task << endl;
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
    // TraceN(this) << "Cancelled task: " << task << endl;
    // onCancel(task);
    //_wakeUp.set();
    // return true;
    //}

    if (!task->cancelled()) {
        task->cancel();
        TraceN(this) << "Cancel task: " << task << endl;
        onCancel(task);
        //_wakeUp.set();
        return true;
    }

    return false;
}


bool TaskRunner::destroy(Task* task)
{
    TraceN(this) << "Abort task: " << task << endl;

    // If the task exists then set the destroyed flag.
    if (exists(task)) {
        TraceN(this) << "Abort managed task: " << task << endl;
        task->_destroyed = true;
    }

    // Otherwise destroy the pointer.
    else {
        TraceN(this) << "Delete unmanaged task: " << task << endl;
        delete task;
    }

    return true; // hmmm
}


bool TaskRunner::add(Task* task)
{
    TraceN(this) << "Add task: " << task << endl;
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
    TraceN(this) << "Remove task: " << task << endl;

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
        TraceN(this) << "Clear: Destroying task: " << *it << endl;
        delete *it;
    }
    _tasks.clear();
}


void TaskRunner::setRunner(Runner::Ptr runner)
{
    TraceN(this) << "Set async: " << runner.get() << endl;

    std::lock_guard<std::mutex> guard(_mutex);
    assert(!_runner);
    _runner = runner;
    _runner->setRepeating(true);
    _runner->start(std::bind(&TaskRunner::run, this));
}


void TaskRunner::run()
{
    Task* task = next();
    // TraceN(this) << "Next task: " << task << endl;

    // Run the task
    if (task) {
        // Check once more that the task has not been cancelled
        if (!task->cancelled()) {
            TraceN(this) << "Run task: " << task << endl;
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
            TraceN(this) << "Destroy task: " << task << endl;
            remove(task);
            delete task;
        }
    }

    // Dispatch the Idle signal
    // TraceN(this) << "idle: "<< Idle.nslots() << endl;
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
