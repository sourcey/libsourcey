//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2002 Sourcey
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


#ifndef SOURCEY_Queue_H
#define SOURCEY_Queue_H


#include "Poco/Thread.h"
#include <queue>


namespace Sourcey {


template<typename T>
class Queue
	/// Implements a thread-safe concurrent queue.
{
private:
    std::queue<T> _queue;
	mutable Poco::Mutex	_mutex;

public:
    void push(const T& data)
    {
		Mutex::ScopedLock lock(_mutex);
        _queue.push(data);
    }

    bool empty() const
    {
		Mutex::ScopedLock lock(_mutex);
        return _queue.empty();
    }

    T& front()
    {
		Mutex::ScopedLock lock(_mutex);
        return _queue.front();
    }
    
    T const& front() const
    {
		Mutex::ScopedLock lock(_mutex);
        return _queue.front();
    }

    void pop()
    {
		Mutex::ScopedLock lock(_mutex);
        _queue.pop();
    }
};


template<typename T>
class ConcurrentQueue
	/// Implements a simple thread-safe multiple producer, 
	/// multiple consumer queue. 
{
private:
    std::queue<T> _queue;
	mutable Poco::Mutex	_mutex;
	Poco::Condition _condition;
public:
    void push(T const& data)
    {
		Mutex::ScopedLock lock(_mutex);
        _queue.push(data);
        lock.unlock();
        _condition.signal();
    }

    bool empty() const
    {
		Mutex::ScopedLock lock(_mutex);
        return _queue.empty();
    }

    bool tryPop(T& popped_value)
    {
		Mutex::ScopedLock lock(_mutex);
        if (_queue.empty())
        {
            return false;
        }
        
        popped_value = _queue.front();
        _queue.pop();
        return true;
    }

    void waitAndPpop(T& popped_value)
    {
		Mutex::ScopedLock lock(_mutex);
        while (_queue.empty())
			_cond.wait(_mutex);
        
        popped_value = _queue.front();
        _queue.pop();
    }

};


} // namespace Sourcey



#endif // SOURCEY_Queue_H