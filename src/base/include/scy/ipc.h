///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_IPC_H
#define SCY_IPC_H


#include "scy/base.h"
#include "scy/synchronizer.h"

#include <mutex>
#include <deque>
#include <string>


namespace scy {


/// Classes for inter-process communication.
namespace ipc {


/// Default action type for executing synchronized callbacks.
struct Action
{
    typedef std::function<void(const Action&)> Callback;

    Callback target;
    void* arg;
    std::string data;

    Action(Callback target, void* arg = nullptr, const std::string& data = "")
        : target(target)
        , arg(arg)
        , data(data)
    {
    }
};


/// IPC queue is for safely passing templated
/// actions between threads and processes.
template <typename TAction = ipc::Action>
class Queue
{
public:
    Queue() {}

    virtual ~Queue() {}

    virtual void push(TAction* action)
    {
        {
            std::lock_guard<std::mutex> guard(_mutex);
            _actions.push_back(action);
        }
        post();
    }

    virtual TAction* pop()
    {
        if (_actions.empty())
            return nullptr;
        std::lock_guard<std::mutex> guard(_mutex);
        TAction* next = _actions.front();
        _actions.pop_front();
        return next;
    }

    virtual void runSync()
    {
        TAction* next = nullptr;
        while ((next = pop())) {
            next->target(*next);
            delete next;
        }
    }

    virtual void close() {}

    virtual void post() {}

    void waitForSync()
    {
        // TODO: impose a time limit
        while (true) {
            {
                std::lock_guard<std::mutex> guard(_mutex);
                if (_actions.empty())
                    return;
            }
            LDebug("Wait for sync")
            scy::sleep(10);
        }
    }

protected:
    mutable std::mutex _mutex;
    std::deque<TAction*> _actions;
};


/// IPC synchronization queue is for passing templated
/// actions between threads and the event loop we are
/// synchronizing with.
///
template <typename TAction = ipc::Action>
class SyncQueue : public Queue<TAction>
{
public:
    SyncQueue(uv::Loop* loop = uv::defaultLoop())
        : _sync(std::bind(&Queue<TAction>::runSync, this), loop)
    {
    }

    virtual ~SyncQueue() {}

    virtual void close() { _sync.close(); }

    virtual void post() { _sync.post(); }

    virtual Synchronizer& sync() { return _sync; }

protected:
    Synchronizer _sync;
};


typedef ipc::Queue<ipc::Action> ActionQueue;
typedef ipc::SyncQueue<ipc::Action> ActionSyncQueue;


} // namespace ipc
} // namespace scy


#endif


/// @\}
