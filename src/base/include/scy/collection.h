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


#ifndef SCY_Containers_H
#define SCY_Containers_H


#include "scy/signal.h"
#include "scy/memory.h"
#include "scy/util.h"
#include "scy/logger.h"

#include <vector>
#include <map>
#include <sstream>
#include <stdexcept>
#include <assert.h>


namespace scy { 
	

template <class TKey, class TValue>
class AbstractCollection
	/// AbstractCollection is an abstract interface for managing a
	/// key-value store of indexed pointers.
{	
public:
	AbstractCollection() {};
	virtual ~AbstractCollection() {}

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


//
// Pointer Collection
//


template <class TKey, class TValue, class TDeleter = std::default_delete<TValue>>
class PointerCollection: public AbstractCollection<TKey, TValue>
	/// This collection is used to maintain an map of any pointer
	/// type indexed by key value in a thread-safe way. 
	///
	/// This class allows for custom memory handling of managed 
	/// pointers via the TDeleter argument.
{
public:
	typedef std::map<TKey, TValue*> Map;
	typedef TDeleter Deleter;

public:
	PointerCollection() 
	{
	}

	virtual ~PointerCollection() 
	{
		clear();
	}

	virtual bool add(const TKey& key, TValue* item, bool whiny = true) 
	{	
		if (exists(key)) {
			if (whiny) {
				//std::ostringstream ss;
				//ss << "An item already exists: " << key << std::endl;
				throw std::runtime_error("Item already exists");
			}
			return false;
		}
		{		
			Mutex::ScopedLock lock(_mutex);
			_map[key] = item;
		}
		onAdd(key, item);
		return true;		
	}

	virtual void update(const TKey& key, TValue* item) 
	{
		// Note: This method will not delete existing values.
		{
			Mutex::ScopedLock lock(_mutex);
			_map[key] = item;
		}
		onAdd(key, item);
	}
	
	virtual TValue* get(const TKey& key, bool whiny = true) const 
	{
		Mutex::ScopedLock lock(_mutex); 
		typename Map::const_iterator it = _map.find(key);	
		if (it != _map.end()) {
			return it->second;	 
		} 
		else if (whiny) {
			//std::ostringstream ss;
			//ss << "Invalid item requested: " << key << std::endl;
			throw std::runtime_error("Item not found");
		}

		return nullptr;
	}

	virtual bool free(const TKey& key) 
	{
		TValue* item = remove(key);
		if (item) {
			TDeleter func;
			func(item);
			return true;
		}
		return false;
	}

	virtual TValue* remove(const TKey& key) 
	{
		TValue* item = nullptr;
		{
			Mutex::ScopedLock lock(_mutex);
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
		TValue* ptr = nullptr;
		{
			Mutex::ScopedLock lock(_mutex); 	
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
		return ptr != nullptr;
	}

	virtual bool exists(const TKey& key) const 
	{ 
		Mutex::ScopedLock lock(_mutex); 	
		typename Map::const_iterator it = _map.find(key);	
		return it != _map.end();	 
	}

	virtual bool exists(const TValue* item) const 
	{ 
		Mutex::ScopedLock lock(_mutex); 	
		for (typename Map::const_iterator it = _map.begin(); it != _map.end(); ++it) {
			if (item == it->second)
				return true;
		}
		return false;
	}

	virtual bool empty() const
	{
		Mutex::ScopedLock lock(_mutex); 	
		return _map.empty();
	}

	virtual int size() const
	{
		Mutex::ScopedLock lock(_mutex); 	
		return _map.size();
	}

	virtual void clear()
	{
		Mutex::ScopedLock lock(_mutex); 	
		util::clearMap<TDeleter>(_map);
	}

	virtual Map map() const 
	{ 
		Mutex::ScopedLock lock(_mutex); 	
		return _map; 
	}

	virtual Map& map() 
	{ 
		Mutex::ScopedLock lock(_mutex); 	
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


//
// Live Collection
//


template <class TKey, class TValue, class TDeleter = std::default_delete<TValue>>
class LiveCollection: public PointerCollection<TKey, TValue, TDeleter>
{	
public:
	typedef PointerCollection<TKey, TValue> Base;

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


//
// KV Collection
//


template <class TKey, class TValue>
class KVCollection
	/// A reusable stack based unique key-value store for DRY coding.
{
public:
	typedef std::map<TKey, TValue> Map;

public:
	KVCollection() 
	{
	}

	virtual ~KVCollection() 
	{
		clear();
	}
	
	virtual bool add(const TKey& key, const TValue& item, bool update = true, bool whiny = true)
	{	
		if (!update && has(key)) {
			if (whiny)
				throw std::runtime_error("Item already exists");
			return false;
		}		
		//Mutex::ScopedLock lock(_mutex);
		_map[key] = item;
		return true;		
	}

	virtual TValue& get(const TKey& key)
	{
		//Mutex::ScopedLock lock(_mutex); 
		typename Map::iterator it = _map.find(key);	
		if (it != _map.end())
			return it->second;	 
		else
			throw std::runtime_error("Item not found");
	}

	virtual const TValue& get(const TKey& key, const TValue& defaultValue) const
	{
		//Mutex::ScopedLock lock(_mutex); 
		typename Map::const_iterator it = _map.find(key);	
		if (it != _map.end())
			return it->second;	 
		return defaultValue;
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

	virtual bool has(const TKey& key) const 
	{ 
		//Mutex::ScopedLock lock(_mutex); 	
		return _map.find(key) != _map.end();	 
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
	//mutable Mutex _mutex;
	Map _map;	
};


//
// NV Collection
//


class NVCollection
	/// A storage container for a name value collections.
	/// This collection can store multiple entries for each 
	/// name, and it's getters are case-insensitive.
{
public:
	struct ILT
	{
		bool operator() (const std::string& s1, const std::string& s2) const
		{
			return util::icompare(s1, s2) < 0;
		}
	};
	
	typedef std::multimap<std::string, std::string, ILT> Map;
	typedef Map::iterator Iterator;
	typedef Map::const_iterator ConstIterator;
	
	NVCollection()
	{
	}

	NVCollection(const NVCollection& nvc) :
		_map(nvc._map)
	{
	}

	virtual ~NVCollection()
	{
	}

	NVCollection& operator = (const NVCollection& nvc);
		// Assigns the name-value pairs of another NVCollection to this one.
		
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


inline NVCollection& NVCollection::operator = (const NVCollection& nvc)
{
	if (&nvc != this) {
		_map = nvc._map;
	}
	return *this;
}

	
inline const std::string& NVCollection::operator [] (const std::string& name) const
{
	ConstIterator it = _map.find(name);
	if (it != _map.end())
		return it->second;
	else
		throw std::runtime_error("Item not found: " + name);
}

	
inline void NVCollection::set(const std::string& name, const std::string& value)	
{
	Iterator it = _map.find(name);
	if (it != _map.end())
		it->second = value;
	else
		_map.insert(Map::value_type(name, value));
}

	
inline void NVCollection::add(const std::string& name, const std::string& value)
{
	_map.insert(Map::value_type(name, value));
}

	
inline const std::string& NVCollection::get(const std::string& name) const
{
	ConstIterator it = _map.find(name);
	if (it != _map.end())
		return it->second;
	else
		throw std::runtime_error("Item not found: " + name);
}


inline const std::string& NVCollection::get(const std::string& name, const std::string& defaultValue) const
{
	ConstIterator it = _map.find(name);
	if (it != _map.end())
		return it->second;
	else
		return defaultValue;
}


inline bool NVCollection::has(const std::string& name) const
{
	return _map.find(name) != _map.end();
}


inline NVCollection::ConstIterator NVCollection::find(const std::string& name) const
{
	return _map.find(name);
}

	
inline NVCollection::ConstIterator NVCollection::begin() const
{
	return _map.begin();
}

	
inline NVCollection::ConstIterator NVCollection::end() const
{
	return _map.end();
}

	
inline bool NVCollection::empty() const
{
	return _map.empty();
}


inline int NVCollection::size() const
{
	return (int)_map.size();
}


inline void NVCollection::erase(const std::string& name)
{
	_map.erase(name);
}


inline void NVCollection::clear()
{
	_map.clear();
}
	

typedef std::map<std::string, std::string> StringMap;
typedef std::vector<std::string> StringVec;


} // namespace scy


#endif // SCY_Containers_H