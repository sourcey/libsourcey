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


#ifndef SOURCEY_BaseManager_H
#define SOURCEY_BaseManager_H


#include "Sourcey/IManager.h"
#include "Sourcey/Util.h"
#include "Poco/Mutex.h"
#include <sstream>
#include <assert.h>


namespace Sourcey { 


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
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			if (exists(key)) {
				if (whiny) {
					std::ostringstream ss;
					ss << "An item already exists: " << key << std::endl;
					throw Poco::NotFoundException(ss.str());
					//throw Poco::ExistsException("Item already exists");
				}
				return false;
			}
		
			_items[key] = item;
		}
		onAdd(key, item);
		return true;		
	}

	virtual void update(const TKey& key, TValue* item) 
	{
		// NOTE: This method will not delete existing values.
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			_items[key] = item;
		}
		onAdd(key, item);
	}
	
	virtual TValue* get(const TKey& key, bool whiny = true) const 
	{
		Poco::FastMutex::ScopedLock lock(_mutex); 
		typename Map::const_iterator it = _items.find(key);	
		if (it != _items.end()) {
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
		Poco::FastMutex::ScopedLock lock(_mutex);
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
			typename Map::iterator it = _items.find(key);	
			if (it != _items.end()) {
				item = it->second;
				_items.erase(it);
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
			for (typename Map::iterator it = _items.begin(); it != _items.end(); ++it) {
				if (item == it->second) {
					key = it->first;
					ptr = it->second;
					_items.erase(it);
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
		typename Map::const_iterator it = _items.find(key);	
		return it != _items.end();	 
	}

	virtual bool exists(const TValue* item) const 
	{ 
		Poco::FastMutex::ScopedLock lock(_mutex); 	
		for (typename Map::const_iterator it = _items.begin(); it != _items.end(); ++it) {
			if (item == it->second)
				return true;
		}
		return false;
	}

	virtual bool empty() const
	{
		Poco::FastMutex::ScopedLock lock(_mutex); 	
		return _items.empty();
	}

	virtual void clear()
	{
		Poco::FastMutex::ScopedLock lock(_mutex); 	
		Util::ClearMap(_items);
	}

	virtual Map copy() const 
	{ 
		Poco::FastMutex::ScopedLock lock(_mutex); 	
		return _items; 
	}

	virtual Map& items() 
	{ 
		Poco::FastMutex::ScopedLock lock(_mutex); 	
		return _items; 
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
	Map _items;	
	mutable Poco::FastMutex _mutex;
};


} // namespace Sourcey


#endif // SOURCEY_BaseManager_H