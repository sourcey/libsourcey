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
	/// Default action type for passing between threads.
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
	/// IPC queue for passing templated actions between 
	/// threads and the event loop we are synchronizing.
{
public:	
	Queue(uv::Loop* loop = uv::defaultLoop()) : 
		_sync(loop, std::bind(&Queue::runSync, this)) 
	{
	}

	virtual ~Queue() 
	{
	}

	void push(TAction* action)
	{
		{
			Mutex::ScopedLock lock(_mutex);
			_actions.push_back(action);
		}
		post();
	}

	TAction* pop()
	{
		if (_actions.empty()) 
			return nullptr;
		Mutex::ScopedLock lock(_mutex);
		TAction* next = _actions.front();
		_actions.pop_front();
		return next;
	}
	
	void runSync()
	{
		TAction* next = nullptr;
		while (next = pop()) {
			next->target(*next);
			delete next;
		}
	}

	void close()
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
	mutable Mutex _mutex;
	std::deque<Action*> _actions;
	SyncContext _sync;
};


typedef ipc::Queue<ipc::Action> ActionQueue;


} } // namespace scy::ipc


#endif