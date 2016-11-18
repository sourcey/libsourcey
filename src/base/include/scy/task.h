///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_Task_H
#define SCY_Task_H


#include "scy/idler.h"
#include "scy/interface.h"
#include "scy/memory.h"
#include "scy/signal.h"
#include "scy/task.h"
#include "scy/uv/uvpp.h"


namespace scy {


class TaskRunner;


/// This class is for implementing any kind
/// async task that is compatible with a TaskRunner.
class Task : public async::Runnable
{
public:
    Task(bool repeat = false);

    /// Sets the task to destroyed state.
    virtual void destroy();

    /// Signals that the task should be disposed of.
    virtual bool destroyed() const;

    /// Signals that the task's should be called
    /// repeatedly by the TaskRunner.
    /// If this returns false the task will be cancelled()
    virtual bool repeating() const;

    /// Unique task ID.
    virtual std::uint32_t id() const;

    // Inherits async::Runnable:
    //
    // virtual void run();
    // virtual void cancel();
    // virtual bool cancelled() const;

protected:
    Task(const Task& task);
    Task& operator=(Task const&);

    /// Should remain protected.
    virtual ~Task();

    /// Called by the TaskRunner to run the task.
    /// Override this method to implement task action.
    /// Returning true means the true should be called again,
    /// and false will cause the task to be destroyed.
    /// The task will similarly be destroyed id destroy()
    /// was called during the current task iteration.
    virtual void run() = 0;

    /// Tasks belong to a TaskRunner instance.
    friend class TaskRunner;

    std::uint32_t _id;
    bool _repeating;
    bool _destroyed;
};


/// The TaskRunner is an asynchronous event loop in
/// charge of running one or many tasks.
///
/// The TaskRunner continually loops through each task in
/// the task list calling the task's run() method.
class TaskRunner : public async::Runnable
{
public:
    TaskRunner(async::Runner::Ptr runner = nullptr);
    virtual ~TaskRunner();

    /// Starts a task, adding it if it doesn't exist.
    virtual bool start(Task* task);

    /// Cancels a task.
    /// The task reference will be managed the TaskRunner
    /// until the task is destroyed.
    virtual bool cancel(Task* task);

    /// Queues a task for destruction.
    virtual bool destroy(Task* task);

    /// Returns weather or not a task exists.
    virtual bool exists(Task* task) const;

    /// Returns the task pointer matching the given ID,
    /// or nullptr if no task exists.
    virtual Task* get(std::uint32_t id) const;

    /// Set the asynchronous context for packet processing.
    /// This may be a Thread or another derivative of Async.
    /// Must be set before the stream is activated.
    virtual void setRunner(async::Runner::Ptr runner);

    /// Returns the default TaskRunner singleton, although
    /// TaskRunner instances may be initialized individually.
    /// The default runner should be kept for short running
    /// tasks such as timers in order to maintain performance.
    static TaskRunner& getDefault();

    /// Fires after completing an iteration of all tasks.
    NullSignal Idle;

    /// Signals when the TaskRunner is shutting down.
    NullSignal Shutdown;

    virtual const char* className() const { return "TaskRunner"; }

protected:
    /// Called by the async context to run the next task.
    virtual void run();

    /// Adds a task to the runner.
    virtual bool add(Task* task);

    /// Removes a task from the runner.
    virtual bool remove(Task* task);

    /// Returns the next task to be run.
    virtual Task* next() const;

    /// Destroys and clears all manages tasks.
    virtual void clear();

    /// Called after a task is added.
    virtual void onAdd(Task* task);

    /// Called after a task is started.
    virtual void onStart(Task* task);

    /// Called after a task is cancelled.
    virtual void onCancel(Task* task);

    /// Called after a task is removed.
    virtual void onRemove(Task* task);

    /// Called after a task has run.
    virtual void onRun(Task* task);

protected:
    typedef std::deque<Task*> TaskList;

    mutable Mutex _mutex;
    TaskList _tasks;
    async::Runner::Ptr _runner;
};


} // namespace scy


#endif // SCY_Task_H


/// @\}
