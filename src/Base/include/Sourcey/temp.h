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


#ifndef SOURCEY_KVStore_H
#define SOURCEY_KVStore_H


#include "Sourcey/Memory.h"
#include "Sourcey/Util.h"
#include "Sourcey/Mutex.h"
#include <sstream>
#include <assert.h>


namespace scy { 


} // namespace scy


#endif // SOURCEY_KVStore_H

	/*
	virtual bool remove(const TValue* item) 
	{	
		{
			//Mutex::ScopedLock lock(_mutex); 	
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
		//Mutex::ScopedLock lock(_mutex); 	
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
			//Mutex::ScopedLock lock(_mutex);
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
		//Mutex::ScopedLock lock(_mutex); 	
		return _map; 
	}
	*/