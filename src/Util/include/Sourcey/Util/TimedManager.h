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
#include "Sourcey/Collection.h"
#include "Sourcey/Timer.h"


namespace scy {


template <class TKey, class TValue>
class TimedManager: public PointerCollection<TKey, TValue>
	/// Provides timed persistent data storage for class instances.
	/// TValue must implement the clone() method.
{
public:
	typedef PointerCollection<TKey, TValue> Base;
	typedef std::map<TValue*, Timeout> TimeoutMap;

	TimedManager(uv::Loop& loop = uv::defaultLoop()) :
		_timer(loop)
	{		
		_timer.Timeout += delegate(this, &TimedManager::onTimer);
		_timer.start(100); // check every 100ms
	}

	virtual ~TimedManager() 
	{
		_timer.Timeout -= delegate(this, &TimedManager::onTimer);
	}
	
	virtual void add(const TKey& key, TValue* item, long timeout = 0)
		// Adds an item which will expire (and be deleted) after the 
		// specified timeout value.
		// If the timeout is 0 the item will be stored indefinitely.
		// The TimedManager assumes ownership of the given pointer.
	{
		Base::free(key); // Free existing item and timeout (if any)
		Base::add(key, item); // Add new entry
		
		if (timeout > 0) {			
			setTimeout(item, timeout);
			/*
			traceL("TimedManager", this) << "Set timeout: " 
				<< key << ": " << item << ": " << timeout << std::endl;	
			Mutex::ScopedLock lock(_tmutex);
			auto& t = _timeouts[item];
			t.setDelay(timeout);
			t.start();
			*/
		}
	}
	
	virtual bool expires(const TKey& key, long timeout) 
		// Updates the item expiry timeout
	{
		traceL("TimedManager", this) << "Set expires: " << key << ": " << timeout << std::endl;	
		return expires(Base::get(key, false), timeout);
	}
	
	virtual bool expires(TValue* item, long timeout) 
		// Updates the item expiry timeout
	{
		traceL("TimedManager", this) << "Set expires: " << item << ": " << timeout << std::endl;	
		/*	
		if (item) {
			Mutex::ScopedLock lock(_tmutex);

			// Update the existing entry
			auto it = _timeouts.find(item);	
			if (it != _timeouts.end()) {
				if (timeout > 0) {
					it->second.reset();
					it->second.setDelay(timeout);
				}
				else
					_timeouts.erase(it);
				return true;
			}

			// Add a new entry
			else {
			}		
		}
		*/
		return setTimeout(item, timeout);
	}

	virtual void clear()
	{
		Base::clear();
	}

protected:
	virtual bool setTimeout(TValue* item, long timeout) 
	{ 
		if (item) {
			Mutex::ScopedLock lock(_tmutex);
			if (timeout > 0) {
				traceL("TimedManager", this) << "Set timeout: " << item << ": " << timeout << std::endl;	
				auto& t = _timeouts[item];
				t.setDelay(timeout);
				t.start();
			}
			else {			
				auto it = _timeouts.find(item);	
				if (it != _timeouts.end()) {
					_timeouts.erase(it);
				}
			}
			return true;
		}
		assert(0 && "unknown item");
		return false;
	}

	virtual void onRemove(const TKey& key, TValue* item) 
	{ 
		// Remove timeout entry
		Mutex::ScopedLock lock(_tmutex);	
		auto it = _timeouts.find(item);	
		if (it != _timeouts.end())
			_timeouts.erase(it);
		
		// Remove pointer entry
		Base::onRemove(key, item);
	}
	
	void onTimer(void*)
	{
		Mutex::ScopedLock lock(_tmutex);
		for (auto it = _timeouts.begin(); it != _timeouts.end();) {
			if (it->second.expired()) {	
				traceL("TimedManager", this) << "Item expired: " << it->first << std::endl;				
				if (Base::remove(it->first))
					delete it->first;
				it = _timeouts.erase(it);
			}
			else ++it;
		}
	}

	mutable Mutex _tmutex;
	TimeoutMap _timeouts;
	Timer _timer;
};


} // namespace scy:


#endif // SOURCEY_TimedManager_H



		/*
			//(timeout, true);
			//_timeouts.insert(TimeoutMap::value_type(item, ));
		//_timer.stopAll(this);
		// loop items and destroy
		// TODO: Call onItemTimeout

		for (Base::Map::iterator it = connections.begin(); it != connections.end();) {
			if ((*it)->deleted()) {
				traceL("Client", this) << "Deleting connection: " << (*it) << std::endl;
				delete *it;
				it = connections.erase(it);
			}
			else
				++it;
		}
		*/
		
		/*;
		TValue* item = get(key, false);
		if (item) {		
			
		}
		assert(0 && "unknown item");
			Mutex::ScopedLock lock(_tmutex);	
			TimeoutMap::iterator it = _timeouts.find(key);	
			if (it != _timeouts.end()) {
				_map.erase(it);
				return true;
			}

		//throw Not

		//if (exists(item)) {
		//}FoundException("Item not found");	
			*/
		//throw std::runtime_error("Not found: Item not found");
		//throw std::runtime_error("Not found: Timeout entry not found");

		//TimeoutMap::iterator it = _timeouts.find(key);	
		//if (it != _timeouts.end())
		//	return it->second;	 


			//assert(0);
			// need to sort entiries by timeout
			// and set nearest timeout value
			//_timer.start(5000, 5000);

			//timeout ? 
			//	_timer.start(TimerCallback<TimedManager>(this, &TimedManager::onItemTimeout, timeout, 0, item)) :
			//	_timer.stop(TimerCallback<TimedManager>(this, &TimedManager::onItemTimeout, 0, 0, item));
	
	/*
	virtual void onItemTimeout(TimerCallback<TimedManager>& _timer)
	{
		TValue* item = reinterpret_cast<TValue*>(_timer.opaque());
		if (Base::remove(item))
			delete item;
	}

	virtual void onItemTimeout(TValue* item)
	{
		//TValue* item = reinterpret_cast<TValue*>(_timer.opaque());
		if (Base::remove(item))
			delete item;
	}
	*/