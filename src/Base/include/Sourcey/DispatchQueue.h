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


#ifndef SOURCEY_DispatchQueue_H
#define SOURCEY_DispatchQueue_H


#include "Sourcey/Runner.h"
#include "Sourcey/Logger.h"
#include "Poco/Stopwatch.h"

#include <deque>


namespace Sourcey {


template<class T>
class DispatchQueue: public Task
{
public:
	DispatchQueue(Runner& runner, int queueSize = 1024, int dispatchTimeout = DEFAULT_TIMEOUT) :
		//Task(runner, false, false),
		_queueSize(queueSize),
		_timeout(dispatchTimeout)
	{
		Log("trace") << "[DispatchQueue:" << this << "] Creating" << std::endl;
	}
	

	virtual bool start()
	{
		Log("trace") << "[DispatchQueue:" << this << "] Starting" << std::endl;
		return Task::start();
	}


	virtual bool stop()
	{
		Log("trace") << "[DispatchQueue:" << this << "] Stopping" << std::endl;
		clear();
		return Task::stop();
	}


	virtual void dispatch(T& item) = 0;
		// Called inside the Runner thread to dispatch an item.
		//
		// Overriding classes implement broadcasting logic here.
	

	virtual void push(T* item)
		// Pushes an item onto the queue.
		// Item pointers are now managed by the DispatchQueue.		
	{
		Poco::FastMutex::ScopedLock lock(_mutex);	

		Log("trace") << "[DispatchQueue:" << this << "] Adding: " << item << std::endl;
				
		while (static_cast<int>(_queue.size()) >= (_queueSize)) {
			Log("trace") << "[DispatchQueue:" << this << "] Purging item" << std::endl;
			delete _queue.front();
			_queue.pop_front();
		}

		_queue.push_back(item);
	}


	virtual void clear()
		// Clears all queued items.
	{
		Poco::FastMutex::ScopedLock lock(_mutex);	
		Util::ClearDeque(_queue);
	}


	virtual void run()
		// Called asynchronously by the Runner to broadcast
		// available items.	
	{
		T* item;
		_stopwatch.start();
		for(;;) 
		{
			{
				Poco::FastMutex::ScopedLock lock(_mutex);
				if (_queue.empty() || _stopwatch.elapsed() > _timeout) // !_running || 
					break;

				item = _queue.front();
				_queue.pop_front();
			}

			dispatch(*item);
			delete item;
		}
		//_stopwatch.stop();
		_stopwatch.reset();
	}


protected:
	virtual ~DispatchQueue() 
	{
		Log("trace") << "[DispatchQueue:" << this << "] Destroying" << std::endl;
		clear();
	};
		
	typedef std::deque<T*> Queue;

	enum
	{
		DEFAULT_TIMEOUT = 250000
	};

	Queue				_queue;
	int					_queueSize;
	int					_timeout;
	Poco::Stopwatch		_stopwatch;	
	mutable Poco::FastMutex	_mutex;
};


} // namespace Sourcey


#endif // SOURCEY_DispatchQueue_H
