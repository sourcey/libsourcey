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


#ifndef SOURCEY_Containers_H
#define SOURCEY_Containers_H


#include "Sourcey/Signal.h"
#include "Sourcey/Memory.h"
#include "Sourcey/Util.h"
#include "Poco/String.h"
#include <sstream>
#include <assert.h>


namespace scy { 
	

template <class TKey, class TValue>
class IManager
	// IManager is an abstract interface for managing a
	// key-value store of indexed pointers.
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
class PointerManager: public IManager<TKey, TValue>
	// This class is useful for maintaining an indexed map of pointers. 
	//
	// All members are thread-safe.
{
public:
	typedef std::map<TKey, TValue*> Map;
	typedef TDeleter Deleter;

public:
	PointerManager() 
	{
	}

	virtual ~PointerManager() 
	{
		clear();
	}

	virtual bool add(const TKey& key, TValue* item, bool whiny = true) 
	{	
		if (exists(key)) {
			if (whiny) {
				//std::ostringstream ss;
				//ss << "An item already exists: " << key << std::endl;
				//throw Exception("Item not found: " + ss.str());
				throw Exception("Item already exists");
			}
			return false;
		}
		{		
			ScopedLock lock(_mutex);
			_map[key] = item;
		}
		onAdd(key, item);
		return true;		
	}

	virtual void update(const TKey& key, TValue* item) 
	{
		// Note: This method will not delete existing values.
		{
			ScopedLock lock(_mutex);
			_map[key] = item;
		}
		onAdd(key, item);
	}
	
	virtual TValue* get(const TKey& key, bool whiny = true) const 
	{
		ScopedLock lock(_mutex); 
		typename Map::const_iterator it = _map.find(key);	
		if (it != _map.end()) {
			return it->second;	 
		} 
		else if (whiny) {
			//std::ostringstream ss;
			//ss << "Invalid item requested: " << key << std::endl;
			//throw Exception("Item not found: " + ss.str());
			throw Exception("Item not found");
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
			ScopedLock lock(_mutex);
			typename Map::iterator it = _map.find(key);	
			if (it != _map.end()) {
				item = it->second;
				_map.erase(it);
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
			ScopedLock lock(_mutex); 	
			for (typename Map::iterator it = _map.begin(); it != _map.end(); ++it) {
				if (item == it->second) {
					key = it->first;
					ptr = it->second;
					_map.erase(it);
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
		ScopedLock lock(_mutex); 	
		typename Map::const_iterator it = _map.find(key);	
		return it != _map.end();	 
	}

	virtual bool exists(const TValue* item) const 
	{ 
		ScopedLock lock(_mutex); 	
		for (typename Map::const_iterator it = _map.begin(); it != _map.end(); ++it) {
			if (item == it->second)
				return true;
		}
		return false;
	}

	virtual bool empty() const
	{
		ScopedLock lock(_mutex); 	
		return _map.empty();
	}

	virtual int size() const
	{
		ScopedLock lock(_mutex); 	
		return _map.size();
	}

	virtual void clear()
	{
		ScopedLock lock(_mutex); 	
		util::clearMap<TDeleter>(_map);
	}

	virtual Map map() const 
	{ 
		ScopedLock lock(_mutex); 	
		return _map; 
	}

	virtual Map& map() 
	{ 
		ScopedLock lock(_mutex); 	
		return _map; 
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
	Map _map;	
	mutable Mutex _mutex;
};


// ---------------------------------------------------------------------
//
template <class TKey, class TValue, class TDeleter = DefaultDeleter<TValue>>
class EventedManager: public PointerManager<TKey, TValue, TDeleter>
{	
public:
	typedef PointerManager<TKey, TValue> Base;

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
	// A reusable stack based key-value store for DRY coding.
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
	
	virtual bool set(const TKey& key, const TValue& item, bool update = true, bool whiny = true)
	{	
		if (!update && has(key)) {
			if (whiny)
				throw Exception("Item already exists");
			return false;
		}		
		//ScopedLock lock(_mutex);
		_map[key] = item;
		return true;		
	}

	virtual TValue& get(const TKey& key)
	{
		//ScopedLock lock(_mutex); 
		typename Map::iterator it = _map.find(key);	
		if (it != _map.end())
			return it->second;	 
		else
			throw Exception("Item not found");
	}

	virtual const TValue& get(const TKey& key, const TValue& defaultValue) const
	{
		//ScopedLock lock(_mutex); 
		typename Map::const_iterator it = _map.find(key);	
		if (it != _map.end())
			return it->second;	 
		return defaultValue;
	}
	
	virtual bool erase(const TKey& key) 
	{
		//ScopedLock lock(_mutex);
		typename Map::iterator it = _map.find(key);	
		if (it != _map.end()) {
			_map.erase(it);
			return true;
		}
		return false;
	}

	virtual bool has(const TKey& key) const 
	{ 
		//ScopedLock lock(_mutex); 	
		return _map.find(key) != _map.end();	 
	}

	virtual bool empty() const
	{
		//ScopedLock lock(_mutex); 	
		return _map.empty();
	}

	virtual int size() const
	{
		//ScopedLock lock(_mutex); 	
		return _map.size();
	}

	virtual void clear()
	{
		//ScopedLock lock(_mutex); 	
		_map.clear();
	}

	virtual Map& map() 
	{ 
		//ScopedLock lock(_mutex); 	
		return _map; 
	}

protected:
	//mutable Mutex _mutex;
	Map _map;	
};


// ---------------------------------------------------------------------
//
class NVHash
	// A collection of name-value string pairs.
	// The name is case-insensitive and there can be more than 
	// one name-value pair with the same name.
{
public:
	struct ILT
	{
		bool operator() (const std::string& s1, const std::string& s2) const
		{
			return Poco::icompare(s1, s2) < 0;
		}
	};
	
	typedef std::multimap<std::string, std::string, ILT> Map;
	typedef Map::iterator Iterator;
	typedef Map::const_iterator ConstIterator;
	
	NVHash()
	{
	}

	NVHash(const NVHash& nvc) :
		_map(nvc._map)
	{
	}

	virtual ~NVHash()
	{
	}

	NVHash& operator = (const NVHash& nvc);
		// Assigns the name-value pairs of another NVHash to this one.
		
	const std::string& operator [] (const std::string& name) const;
		// Returns the value of the (first) name-value pair with the given name.
		//
		// Throws a NotFoundException if the name-value pair does not exist.
		
	void set(const std::string& name, const std::string& value);	
		// Sets the value of the (first) name-value pair with the given name.
		
	void add(const std::string& name, const std::string& value);
		// Adds a new name-value pair with the given name and value.
		
	const std::string& get(const std::string& name) const;
		// Returns the value of the first name-value pair with the given name.
		//
		// Throws a NotFoundException if the name-value pair does not exist.

	const std::string& get(const std::string& name, const std::string& defaultValue) const;
		// Returns the value of the first name-value pair with the given name.
		// If no value with the given name has been found, the defaultValue is returned.

	bool has(const std::string& name) const;
		// Returns true if there is at least one name-value pair
		// with the given name.

	ConstIterator find(const std::string& name) const;
		// Returns an iterator pointing to the first name-value pair
		// with the given name.
		
	ConstIterator begin() const;
		// Returns an iterator pointing to the begin of
		// the name-value pair collection.
		
	ConstIterator end() const;
		// Returns an iterator pointing to the end of 
		// the name-value pair collection.
		
	bool empty() const;
		// Returns true iff the header does not have any content.

	int size() const;
		// Returns the number of name-value pairs in the
		// collection.

	void erase(const std::string& name);
		// Removes all name-value pairs with the given name.

	void clear();
		// Removes all name-value pairs and their values.

private:
	Map _map;
};


inline NVHash& NVHash::operator = (const NVHash& nvc)
{
	if (&nvc != this) {
		_map = nvc._map;
	}
	return *this;
}

	
inline const std::string& NVHash::operator [] (const std::string& name) const
{
	ConstIterator it = _map.find(name);
	if (it != _map.end())
		return it->second;
	else
		throw Exception("Item not found: " + name);
}

	
inline void NVHash::set(const std::string& name, const std::string& value)	
{
	Iterator it = _map.find(name);
	if (it != _map.end())
		it->second = value;
	else
		_map.insert(Map::value_type(name, value));
}

	
inline void NVHash::add(const std::string& name, const std::string& value)
{
	_map.insert(Map::value_type(name, value));
}

	
inline const std::string& NVHash::get(const std::string& name) const
{
	ConstIterator it = _map.find(name);
	if (it != _map.end())
		return it->second;
	else
		throw Exception("Item not found: " + name);
}


inline const std::string& NVHash::get(const std::string& name, const std::string& defaultValue) const
{
	ConstIterator it = _map.find(name);
	if (it != _map.end())
		return it->second;
	else
		return defaultValue;
}


inline bool NVHash::has(const std::string& name) const
{
	return _map.find(name) != _map.end();
}


inline NVHash::ConstIterator NVHash::find(const std::string& name) const
{
	return _map.find(name);
}

	
inline NVHash::ConstIterator NVHash::begin() const
{
	return _map.begin();
}

	
inline NVHash::ConstIterator NVHash::end() const
{
	return _map.end();
}

	
inline bool NVHash::empty() const
{
	return _map.empty();
}


inline int NVHash::size() const
{
	return (int)_map.size();
}


inline void NVHash::erase(const std::string& name)
{
	_map.erase(name);
}


inline void NVHash::clear()
{
	_map.clear();
}


} // namespace scy


#endif // SOURCEY_Containers_H