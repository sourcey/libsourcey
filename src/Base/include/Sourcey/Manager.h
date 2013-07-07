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


#include "Sourcey/IManager.h"
#include "Sourcey/Signal.h"
#include "Sourcey/Util.h"
#include "Poco/Mutex.h"
#include <sstream>
#include <assert.h>


namespace scy { 
	

template <class TKey, class TValue>
class IManager
	/// Abstract interface for managing a key-value
	/// store of indexed pointers.
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
template <class TKey, class TValue>
class BasicManager: public IManager<TKey, TValue>
	/// A reusable map based manager interface for DRY coding.
{
public:
	typedef std::map<TKey, TValue*> Map;

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
			Poco::FastMutex::ScopedLock lock(_mutex);
			_store[key] = item;
		}
		onAdd(key, item);
		return true;		
	}

	virtual void update(const TKey& key, TValue* item) 
	{
		// NOTE: This method will not delete existing values.
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			_store[key] = item;
		}
		onAdd(key, item);
	}
	
	virtual TValue* get(const TKey& key, bool whiny = true) const 
	{
		Poco::FastMutex::ScopedLock lock(_mutex); 
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
			delete item;
			return true;
		}
		return false;
	}

	virtual TValue* remove(const TKey& key) 
	{
		TValue* item = NULL;
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
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
			Poco::FastMutex::ScopedLock lock(_mutex); 	
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
		Poco::FastMutex::ScopedLock lock(_mutex); 	
		typename Map::const_iterator it = _store.find(key);	
		return it != _store.end();	 
	}

	virtual bool exists(const TValue* item) const 
	{ 
		Poco::FastMutex::ScopedLock lock(_mutex); 	
		for (typename Map::const_iterator it = _store.begin(); it != _store.end(); ++it) {
			if (item == it->second)
				return true;
		}
		return false;
	}

	virtual bool empty() const
	{
		Poco::FastMutex::ScopedLock lock(_mutex); 	
		return _store.empty();
	}

	virtual int size() const
	{
		Poco::FastMutex::ScopedLock lock(_mutex); 	
		return _store.size();
	}

	virtual void clear()
	{
		Poco::FastMutex::ScopedLock lock(_mutex); 	
		util::ClearMap(_store);
	}

	virtual Map store() const 
	{ 
		Poco::FastMutex::ScopedLock lock(_mutex); 	
		return _store; 
	}

	virtual Map& store() 
	{ 
		Poco::FastMutex::ScopedLock lock(_mutex); 	
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
	mutable Poco::FastMutex _mutex;
};


// ---------------------------------------------------------------------
//
template <class TKey, class TValue>
class EventfulManager: public BasicManager<TKey, TValue>
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

} // namespace scy


#endif // SOURCEY_Manager_H