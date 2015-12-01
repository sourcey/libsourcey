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

#ifndef SCY_IPC_H
#define SCY_IPC_H


#include "scy/mutex.h"
#include "scy/synccontext.h"

#include <string>
#include <deque>


namespace scy {
namespace ipc {


struct Action
	/// Default action type for executing synchronized callbacks.
{
	typedef std::function<void(const Action&)> callback_t;
	callback_t target;
	void* arg;
	std::string data;
		
	Action(callback_t target, void* arg = nullptr, const std::string& data = "") :
		target(target), arg(arg), data(data) {} 
};


template<typename TAction = ipc::Action> 
class Queue
	/// IPC queue is for safely passing templated   
	/// actions between threads and processes.
{
public:	
	Queue()
	{
	}

	virtual ~Queue() 
	{
	}

	virtual void push(TAction* action)
	{
		{
			Mutex::ScopedLock lock(_mutex);
			_actions.push_back(action);
		}
		post();
	}

	virtual TAction* pop()
	{
		if (_actions.empty()) 
			return nullptr;
		Mutex::ScopedLock lock(_mutex);
		TAction* next = _actions.front();
		_actions.pop_front();
		return next;
	}
	
	virtual void runSync()
	{
		TAction* next = nullptr;
		while (next = pop()) {
			next->target(*next);
			delete next;
		}
	}

	virtual void close()
	{
	}
	
	virtual void post()
	{
	}

	void waitForSync()
	{
		// TODO: Impose a time limit
		while(true) {
			{
				Mutex::ScopedLock lock(_mutex);
				if (_actions.empty())
					return;
			}
			DebugL << "Wait for sync" << std::endl;	
			scy::sleep(10);
		}	
	}

protected:	
	mutable Mutex _mutex;
	std::deque<TAction*> _actions;
};


template<typename TAction = ipc::Action> 
class SyncQueue: public Queue<TAction>
	/// IPC synchronization queue is for passing templated 
	/// actions between threads and the event loop we are 
	/// synchronizing with.
{
public:	
	SyncQueue(uv::Loop* loop = uv::defaultLoop()) : 
		_sync(loop, std::bind(&Queue<TAction>::runSync, this)) 
	{
	}

	virtual ~SyncQueue() 
	{
	}

	virtual void close()
	{
		_sync.close();
	}
	
	virtual void post()
	{
		_sync.post();
	}
	
	virtual SyncContext& sync()
	{
		return _sync;
	}

protected:	
	SyncContext _sync;
};


typedef ipc::Queue<ipc::Action> ActionQueue;
typedef ipc::SyncQueue<ipc::Action> ActionSyncQueue;


} } // namespace scy::ipc


#endif
