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


#ifndef SOURCEY_TimedManager_H
#define SOURCEY_TimedManager_H


#include "Sourcey/Base.h"
#include "Sourcey/Containers.h"
#include "Sourcey/Timer.h"


namespace scy {


template <class TKey, class TValue>
class TimedManager: public PointerManager<TKey, TValue>
	/// Provides timed persistent data storage for class instances.
	/// TValue must implement the clone() method.
{
public:
	typedef PointerManager<TKey, TValue> Base;

public:
	TimedManager() {		
		timer.Timeout += delegate(this, &TimedManager::onTimer);
	};
	virtual ~TimedManager() {
		timer.Timeout -= delegate(this, &TimedManager::onTimer);
	};
	
	virtual void add(const TKey& name, TValue* item, long timeout = 0)
		/// Add the item with a timeout specified by the timeout value.
		/// If the timeout is 0 the item will be stored indefinitely.
		/// The item pointer MUST be destroyed by the manager or the 
		/// application will crash.
	{
		// Remove existing items for the
		// given key and store the item.
		Base::free(name);
		Base::add(name, item);
		//if (timeout)
		//	timer.start(TimerCallback<TimedManager>(this, &TimedManager::onItemTimeout, timeout, 0, item));
	}
	
	virtual void expires(const TKey& name, long timeout) 
	{
		TValue* item = get(name);
		if (item) {			
			assert(0);
			// need to sort entiries by timeout
			// and set nearest timeout value
			//timer.start(5000, 5000);

			//timeout ? 
			//	timer.start(TimerCallback<TimedManager>(this, &TimedManager::onItemTimeout, timeout, 0, item)) :
			//	timer.stop(TimerCallback<TimedManager>(this, &TimedManager::onItemTimeout, 0, 0, item));
		}
	}
	
	virtual void expires(TValue* item, long timeout) 
	{
		if (!exists(item))
			throw Poco::NotFoundException("Item not found");
		
		
		assert(0);


		//timeout ? 
		//	timer.start(TimerCallback<TimedManager>(this, &TimedManager::onItemTimeout, timeout, 0, item)) :
		//	timer.stop(TimerCallback<TimedManager>(this, &TimedManager::onItemTimeout, 0, 0, item));
	}

	virtual void onRemove(const TKey& key, TValue* item) 
	{ 
		//timer.stop(TimerCallback<TimedManager>(this, &TimedManager::onItemTimeout, 0, 0, item));
		Base::onRemove(key, item);
	}

	virtual void clear()
	{
		//timer.stopAll(this);
		Base::clear();
	}
	
	/*
	virtual void onItemTimeout(TimerCallback<TimedManager>& timer)
	{
		TValue* item = reinterpret_cast<TValue*>(timer.opaque());
		if (Base::remove(item))
			delete item;
	}
	*/

	virtual void onItemTimeout(TValue* item)
	{
		//TValue* item = reinterpret_cast<TValue*>(timer.opaque());
		if (Base::remove(item))
			delete item;
	}
	
	void onTimer(void*)
	{
		// loop items and destroy
		// TODO: Call onItemTimeout

		/*
		for (ClientConnectionList::iterator it = connections.begin(); it != connections.end();) {
			if ((*it)->deleted()) {
				traceL("Client", this) << "Deleting connection: " << (*it) << std::endl;
				delete *it;
				it = connections.erase(it);
			}
			else
				++it;
		}
		*/
	}

	Timer timer;
};


} // namespace scy:


#endif // SOURCEY_TimedManager_H