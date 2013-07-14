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


#ifndef SOURCEY_Manager_H
#define SOURCEY_Manager_H


//#include "Sourcey/IManager.h"
#include "Sourcey/Signal.h"
#include "Sourcey/Memory.h"
#include "Sourcey/Util.h"
//#include "Sourcey/Mutex.h"
#include <sstream>
#include <assert.h>


namespace scy { 
	

template <class TKey, class TValue>
class IManager
	/// IManager is an abstract interface for managing a
	/// key-value store of indexed pointers.
{	
public:
	IManager() {};
	virtual ~IManager() {}

	virtual bool add(const TKey& key, TValue* item, bool whiny = true) = 0;
	virtual bool remove(const TValue* item) = 0;
	virtual TValue* remove(const TKey& key) = 0;
	virtual bool exists(const TKey& key) const = 0;
	virtual bool exists(const TValue* item) const = 0;
	virtual bool free(const TKey& key) = 0;
	virtual bool empty() const = 0;
	virtual int size() const = 0;
	virtual TValue* get(const TKey& key, bool whiny = true) const = 0;
	virtual void clear() = 0;
};


// ---------------------------------------------------------------------
//
template <class TKey, class TValue, class TDeleter = DefaultDeleter<TValue>>
class BasicManager: public IManager<TKey, TValue>
	/// A reusable map based manager interface for DRY coding.
{
public:
	typedef std::map<TKey, TValue*> Map;
	typedef TDeleter Deleter;

public:
	BasicManager() 
	{
	}

	virtual ~BasicManager() 
	{
		clear();
	}

	virtual bool add(const TKey& key, TValue* item, bool whiny = true) 
	{	
		if (exists(key)) {
			if (whiny) {
				std::ostringstream ss;
				ss << "An item already exists: " << key << std::endl;
				throw Poco::NotFoundException(ss.str());
				//throw Poco::ExistsException("Item already exists");
			}
			return false;
		}
		{		
			Mutex::ScopedLock lock(_mutex);
			_store[key] = item;
		}
		onAdd(key, item);
		return true;		
	}

	virtual void update(const TKey& key, TValue* item) 
	{
		// NOTE: This method will not delete existing values.
		{
			Mutex::ScopedLock lock(_mutex);
			_store[key] = item;
		}
		onAdd(key, item);
	}
	
	virtual TValue* get(const TKey& key, bool whiny = true) const 
	{
		Mutex::ScopedLock lock(_mutex); 
		typename Map::const_iterator it = _store.find(key);	
		if (it != _store.end()) {
			return it->second;	 
		} else if (whiny) {
			std::ostringstream ss;
			ss << "Invalid item requested: " << key << std::endl;
			throw Poco::NotFoundException(ss.str());
			//throw Poco::NotFoundException("Item not found");
		}

		return NULL;
	}

	virtual bool free(const TKey& key) 
	{
		TValue* item = remove(key);
		if (item) {
			TDeleter::func(item);
			return true;
		}
		return false;
	}

	virtual TValue* remove(const TKey& key) 
	{
		TValue* item = NULL;
		{
			Mutex::ScopedLock lock(_mutex);
			typename Map::iterator it = _store.find(key);	
			if (it != _store.end()) {
				item = it->second;
				_store.erase(it);
			}
		}
		if (item)
			onRemove(key, item);
		return item;
	}

	virtual bool remove(const TValue* item) 
	{	
		TKey key;
		TValue* ptr = NULL;
		{
			Mutex::ScopedLock lock(_mutex); 	
			for (typename Map::iterator it = _store.begin(); it != _store.end(); ++it) {
				if (item == it->second) {
					key = it->first;
					ptr = it->second;
					_store.erase(it);
					break;
				}
			}
		}
		if (ptr)
			onRemove(key, ptr);
		return ptr != NULL;
	}

	virtual bool exists(const TKey& key) const 
	{ 
		Mutex::ScopedLock lock(_mutex); 	
		typename Map::const_iterator it = _store.find(key);	
		return it != _store.end();	 
	}

	virtual bool exists(const TValue* item) const 
	{ 
		Mutex::ScopedLock lock(_mutex); 	
		for (typename Map::const_iterator it = _store.begin(); it != _store.end(); ++it) {
			if (item == it->second)
				return true;
		}
		return false;
	}

	virtual bool empty() const
	{
		Mutex::ScopedLock lock(_mutex); 	
		return _store.empty();
	}

	virtual int size() const
	{
		Mutex::ScopedLock lock(_mutex); 	
		return _store.size();
	}

	virtual void clear()
	{
		Mutex::ScopedLock lock(_mutex); 	
		util::clearMap<TDeleter>(_store);
	}

	virtual Map store() const 
	{ 
		Mutex::ScopedLock lock(_mutex); 	
		return _store; 
	}

	virtual Map& store() 
	{ 
		Mutex::ScopedLock lock(_mutex); 	
		return _store; 
	}

	virtual void onAdd(const TKey&, TValue*) 
	{
		// override me
	}

	virtual void onRemove(const TKey&, TValue*) 
	{ 
		// override me
	}

protected:
	Map _store;	
	mutable Mutex _mutex;
};


// ---------------------------------------------------------------------
//
template <class TKey, class TValue, class TDeleter = DefaultDeleter<TValue>>
class EventedManager: public BasicManager<TKey, TValue, TDeleter>
{	
public:
	typedef BasicManager<TKey, TValue> Base;

public:	
	virtual void onAdd(const TKey&, TValue* item) 
	{
		ItemAdded.emit(this, *item);
	}

	virtual void onRemove(const TKey&, TValue* item) 
	{ 
		ItemRemoved.emit(this, *item); 
	}

	Signal<TValue&>			ItemAdded;
	Signal<const TValue&>	ItemRemoved;	
};


// ---------------------------------------------------------------------
//
template <class TKey, class TValue>
class KVStore
	/// A reusable map based manager interface for DRY coding.
{
public:
	typedef std::map<TKey, TValue> Map;

public:
	KVStore() 
	{
	}

	virtual ~KVStore() 
	{
		clear();
	}

	virtual bool add(const TKey& key, const TValue& item, bool whiny = true) 
	{	
		if (exists(key)) {
			if (whiny)
				throw Poco::ExistsException();
			return false;
		}		
		{
			//Mutex::ScopedLock lock(_mutex);
			_map[key] = item;
		}
		return true;		
	}

	virtual TValue& get(const TKey& key) //const //, bool whiny = true
	{
		//Mutex::ScopedLock lock(_mutex); 
		typename Map::iterator it = _map.find(key);	
		if (it != _map.end())
			return it->second;	 
		else
			throw Poco::NotFoundException();
	}
	
	virtual bool remove(const TKey& key) 
	{
		//Mutex::ScopedLock lock(_mutex);
		typename Map::iterator it = _map.find(key);	
		if (it != _map.end()) {
			_map.erase(it);
			return true;
		}
		return false;
	}

	virtual bool exists(const TKey& key) const 
	{ 
		//Mutex::ScopedLock lock(_mutex); 	
		typename Map::const_iterator it = _map.find(key);	
		return it != _map.end();	 
	}

	virtual bool empty() const
	{
		//Mutex::ScopedLock lock(_mutex); 	
		return _map.empty();
	}

	virtual int size() const
	{
		//Mutex::ScopedLock lock(_mutex); 	
		return _map.size();
	}

	virtual void clear()
	{
		//Mutex::ScopedLock lock(_mutex); 	
		_map.clear();
	}

	virtual Map& map() 
	{ 
		//Mutex::ScopedLock lock(_mutex); 	
		return _map; 
	}

protected:
	Map _map;	
	//mutable Mutex _mutex;
};


} // namespace scy


#endif // SOURCEY_Manager_H