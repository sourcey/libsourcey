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


#ifndef SOURCEY_StackManager_H
#define SOURCEY_StackManager_H


#include "Sourcey/Memory.h"
#include "Sourcey/Util.h"
#include "Poco/Mutex.h"
#include <sstream>
#include <assert.h>


namespace scy { 


template <class TKey, class TValue>
class StackManager
	/// A reusable map based manager interface for DRY coding.
{
public:
	typedef std::map<TKey, TValue> Map;

public:
	StackManager() 
	{
	}

	virtual ~StackManager() 
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
			//Poco::FastMutex::ScopedLock lock(_mutex);
			_map[key] = item;
		}
		return true;		
	}

	virtual TValue& get(const TKey& key) //const //, bool whiny = true
	{
		//Poco::FastMutex::ScopedLock lock(_mutex); 
		typename Map::iterator it = _map.find(key);	
		if (it != _map.end())
			return it->second;	 
		else
			throw Poco::NotFoundException();
	}
	
	virtual bool remove(const TKey& key) 
	{
		//Poco::FastMutex::ScopedLock lock(_mutex);
		typename Map::iterator it = _map.find(key);	
		if (it != _map.end()) {
			_map.erase(it);
			return true;
		}
		return false;
	}

	virtual bool exists(const TKey& key) const 
	{ 
		//Poco::FastMutex::ScopedLock lock(_mutex); 	
		typename Map::const_iterator it = _map.find(key);	
		return it != _map.end();	 
	}

	virtual bool empty() const
	{
		//Poco::FastMutex::ScopedLock lock(_mutex); 	
		return _map.empty();
	}

	virtual int size() const
	{
		//Poco::FastMutex::ScopedLock lock(_mutex); 	
		return _map.size();
	}

	virtual void clear()
	{
		//Poco::FastMutex::ScopedLock lock(_mutex); 	
		_map.clear();
	}

	virtual Map& map() 
	{ 
		//Poco::FastMutex::ScopedLock lock(_mutex); 	
		return _map; 
	}

//protected:
	Map _map;	
	//mutable Poco::FastMutex _mutex;
};


} // namespace scy


#endif // SOURCEY_StackManager_H

	/*
	virtual bool remove(const TValue* item) 
	{	
		{
			//Poco::FastMutex::ScopedLock lock(_mutex); 	
			for (typename Map::iterator it = _map.begin(); it != _map.end(); ++it) {
				if (item == it->second) {
					_map.erase(it);
					return true;
				}
			}
		}
		return false;
	}

	virtual bool exists(const TValue* item) const 
	{ 
		//Poco::FastMutex::ScopedLock lock(_mutex); 	
		for (typename Map::const_iterator it = _map.begin(); it != _map.end(); ++it) {
			if (item == it->second)
				return true;
		}
		return false;
	}
	*/

	/*
	virtual void update(const TKey& key, TValue* item) 
	{
		// NOTE: This method will not delete existing values.
		{
			//Poco::FastMutex::ScopedLock lock(_mutex);
			_map[key] = item;
		}
		onAdd(key, item);
	}
	*/
	

	/*
	virtual bool free(const TKey& key) 
	{
		TValue* item = remove(key);
		if (item) {
			delete item;
			return true;
		}
		return false;
	}
	*/

	
	/*
	virtual Map items() const 
	{ 
		//Poco::FastMutex::ScopedLock lock(_mutex); 	
		return _map; 
	}
	*/