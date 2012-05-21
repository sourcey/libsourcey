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


#ifndef SOURCEY_TimedManager_H
#define SOURCEY_TimedManager_H


#include "Sourcey/BasicManager.h"
#include "Sourcey/Timer.h"


namespace Sourcey {


template <class TKey, class TValue>
class TimedManager: public BasicManager<TKey, TValue>
	/// Provides timed persistent data storage for class instances.
	/// TValue must implement the clone() method.
{
public:
	typedef BasicManager<TKey, TValue> Base;

public:
	TimedManager() {};
	virtual ~TimedManager() {};
	
	virtual void add(const TKey& name, TValue* item, long timeout = 0)
		/// Add the item with a timeout specified by the timeout value.
		/// If the timeout is 0 the item will be stored indefinitely.
		/// The item pointer MUST be destroyed by the manager or the 
		/// application will crash.
	{
		Log("trace") << "[TimedManager] Adding: " << item << ": " << timeout << std::endl;

		// Remove existing items for the
		// given key and store the item.
		Base::free(name);
		Base::add(name, item);
		if (timeout)
			Timer::getDefault().start(TimerCallback<TimedManager>(this, &TimedManager::onItemTimeout, timeout, 0, item));
	}
	
	virtual void expires(const TKey& name, long timeout) 
	{
		TValue* item = get(name);
		if (item) {
			timeout ? 
				Timer::getDefault().start(TimerCallback<TimedManager>(this, &TimedManager::onItemTimeout, timeout, 0, item)) :
				Timer::getDefault().stop(TimerCallback<TimedManager>(this, &TimedManager::onItemTimeout, 0, 0, item));
		}
	}
	
	virtual void expires(TValue* item, long timeout) 
	{
		if (!exists(item))
			throw Poco::NotFoundException("Item not found");
		
		timeout ? 
			Timer::getDefault().start(TimerCallback<TimedManager>(this, &TimedManager::onItemTimeout, timeout, 0, item)) :
			Timer::getDefault().stop(TimerCallback<TimedManager>(this, &TimedManager::onItemTimeout, 0, 0, item));
	}

	virtual void onRemove(const TKey& key, TValue* item) 
	{ 
		Log("trace") << "[TimedManager] Removing: " << item << std::endl;
		Timer::getDefault().stop(TimerCallback<TimedManager>(this, &TimedManager::onItemTimeout, 0, 0, item));
		Base::onRemove(key, item);
	}

	virtual void clear()
	{
		Timer::getDefault().stopAll(this);
		Base::clear();
	}

	virtual void onItemTimeout(TimerCallback<TimedManager>& timer)
	{
		TValue* item = reinterpret_cast<TValue*>(timer.opaque());
		Log("trace") << "[TimedManager] Item Timeout: " << item << std::endl;
		if (Base::remove(item))
			delete item;
	}
};


} // namespace Sourcey:


#endif // SOURCEY_TimedManager_H