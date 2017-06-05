///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_Containers_H
#define SCY_Containers_H


#include "scy/base.h"
#include "scy/logger.h"
#include "scy/signal.h"
#include "scy/util.h"

#include <assert.h>
#include <map>
#include <sstream>
#include <stdexcept>
#include <vector>


namespace scy {


/// AbstractCollection is an abstract interface for managing a
/// key-value store of indexed pointers.
template <class TKey, class TValue> class AbstractCollection
{
public:
    AbstractCollection(){};
    virtual ~AbstractCollection() {}

    virtual bool add(const TKey& key, TValue* item, bool whiny = true) = 0;
    virtual bool remove(const TValue* item) = 0;
    virtual TValue* remove(const TKey& key) = 0;
    virtual bool exists(const TKey& key) const = 0;
    virtual bool exists(const TValue* item) const = 0;
    virtual bool free(const TKey& key) = 0;
    virtual bool empty() const = 0;
    virtual size_t size() const = 0;
    virtual TValue* get(const TKey& key, bool whiny = true) const = 0;
    virtual void clear() = 0;
};


//
// Pointer Collection
//


/// This collection is used to maintain an map of any pointer
/// type indexed by key value in a thread-safe way.
///
/// This class allows for custom memory handling of managed
/// pointers via the TDeleter argument.
template <class TKey, class TValue,
          class TDeleter = std::default_delete<TValue>>
class /* Base_API */ PointerCollection : public AbstractCollection<TKey, TValue>
{
public:
    typedef std::map<TKey, TValue*> Map;
    typedef TDeleter Deleter;

public:
    PointerCollection() {}

    virtual ~PointerCollection() { clear(); }

    virtual bool add(const TKey& key, TValue* item, bool whiny = true) override
    {
        if (exists(key)) {
            if (whiny) {
                // std::ostringstream ss;
                // ss << "An item already exists: " << key << std::endl;
                throw std::runtime_error("Item already exists");
            }
            return false;
        }
        {
            std::lock_guard<std::mutex> guard(_mutex);
            _map[key] = item;
        }
        onAdd(key, item);
        return true;
    }

    virtual void update(const TKey& key, TValue* item)
    {
        // Note: This method will not delete existing values.
        {
            std::lock_guard<std::mutex> guard(_mutex);
            _map[key] = item;
        }
        onAdd(key, item);
    }

    virtual TValue* get(const TKey& key, bool whiny = true) const override
    {
        std::lock_guard<std::mutex> guard(_mutex);
        typename Map::const_iterator it = _map.find(key);
        if (it != _map.end()) {
            return it->second;
        } else if (whiny) {
            // std::ostringstream ss;
            // ss << "Invalid item requested: " << key << std::endl;
            throw std::runtime_error("Item not found");
        }

        return nullptr;
    }

    virtual bool free(const TKey& key) override
    {
        TValue* item = remove(key);
        if (item) {
            TDeleter func;
            func(item);
            return true;
        }
        return false;
    }

    virtual TValue* remove(const TKey& key) override
    {
        TValue* item = nullptr;
        {
            std::lock_guard<std::mutex> guard(_mutex);
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

    virtual bool remove(const TValue* item) override
    {
        TKey key;
        TValue* ptr = nullptr;
        {
            std::lock_guard<std::mutex> guard(_mutex);
            for (typename Map::iterator it = _map.begin(); it != _map.end();
                 ++it) {
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

    virtual bool exists(const TKey& key) const override
    {
        std::lock_guard<std::mutex> guard(_mutex);
        typename Map::const_iterator it = _map.find(key);
        return it != _map.end();
    }

    virtual bool exists(const TValue* item) const override
    {
        std::lock_guard<std::mutex> guard(_mutex);
        for (typename Map::const_iterator it = _map.begin(); it != _map.end();
             ++it) {
            if (item == it->second)
                return true;
        }
        return false;
    }

    virtual bool empty() const override
    {
        std::lock_guard<std::mutex> guard(_mutex);
        return _map.empty();
    }

    virtual size_t size() const override
    {
        std::lock_guard<std::mutex> guard(_mutex);
        return _map.size();
    }

    virtual void clear() override
    {
        std::lock_guard<std::mutex> guard(_mutex);
        util::clearMap<TDeleter>(_map);
    }

    virtual Map map() const
    {
        std::lock_guard<std::mutex> guard(_mutex);
        return _map;
    }

    virtual Map& map()
    {
        std::lock_guard<std::mutex> guard(_mutex);
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
    mutable std::mutex _mutex;
    Map _map;
};


//
// Live Collection
//


template <class TKey, class TValue,
          class TDeleter = std::default_delete<TValue>>
class /* Base_API */ LiveCollection : public PointerCollection<TKey, TValue, TDeleter>
{
public:
    typedef PointerCollection<TKey, TValue> Base;

public:
    virtual void onAdd(const TKey&, TValue* item) override
    {
        ItemAdded.emit(*item);
    }

    virtual void onRemove(const TKey&, TValue* item) override
    {
        ItemRemoved.emit(*item);
    }

    Signal<void(TValue&)> ItemAdded;
    Signal<void(const TValue&)> ItemRemoved;
};


//
// KV Collection
//


/// Reusable stack based unique key-value store for DRY coding.
template <class TKey, class TValue>
class /* Base_API */ KVCollection
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

        // _map[key] = item;
        _map.insert(typename Map::value_type(key, item));
        return true;
    }

    virtual TValue& get(const TKey& key)
    {
        typename Map::iterator it = _map.find(key);
        if (it != _map.end())
            return it->second;
        else
            throw std::runtime_error("Item not found");
    }

    virtual const TValue& get(const TKey& key, const TValue& defaultValue) const
    {
        typename Map::const_iterator it = _map.find(key);
        if (it != _map.end())
            return it->second;
        return defaultValue;
    }

    virtual bool remove(const TKey& key)
    {
        typename Map::iterator it = _map.find(key);
        if (it != _map.end()) {
            _map.erase(it);
            return true;
        }
        return false;
    }

    virtual bool has(const TKey& key) const
    {
        return _map.find(key) != _map.end();
    }

    virtual bool empty() const
    {
        return _map.empty();
    }

    virtual size_t size() const
    {
        return _map.size();
    }

    virtual void clear()
    {
        _map.clear();
    }

    virtual Map& map()
    {
        return _map;
    }

protected:
    Map _map;
};


//
// NV Collection
//


/// A storage container for a name value collections.
/// This collection can store multiple entries for each
/// name, and it's getters are case-insensitive.
class Base_API NVCollection
{
public:
    struct ILT
    {
        bool operator()(const std::string& s1, const std::string& s2) const
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

    NVCollection(const NVCollection& nvc)
        : _map(nvc._map)
    {
    }

    virtual ~NVCollection()
    {
    }

    /// Assigns the name-value pairs of another NVCollection to this one.
    NVCollection& operator=(const NVCollection& nvc);

    /// Returns the value of the (first) name-value pair with the given name.
    ///
    /// Throws a NotFoundException if the name-value pair does not exist.
    const std::string& operator[](const std::string& name) const;

    /// Sets the value of the (first) name-value pair with the given name.
    void set(const std::string& name, const std::string& value);

    /// Adds a new name-value pair with the given name and value.
    void add(const std::string& name, const std::string& value);

    /// Returns the value of the first name-value pair with the given name.
    ///
    /// Throws a NotFoundException if the name-value pair does not exist.
    const std::string& get(const std::string& name) const;

    /// Returns the value of the first name-value pair with the given name.
    /// If no value with the given name has been found, the defaultValue is
    /// returned.
    const std::string& get(const std::string& name, const std::string& defaultValue) const;

    /// Returns true if there is at least one name-value pair
    /// with the given name.
    bool has(const std::string& name) const;

    /// Returns an iterator pointing to the first name-value pair
    /// with the given name.
    ConstIterator find(const std::string& name) const;

    /// Returns an iterator pointing to the begin of
    /// the name-value pair collection.
    ConstIterator begin() const;

    /// Returns an iterator pointing to the end of
    /// the name-value pair collection.
    ConstIterator end() const;

    /// Returns true iff the header does not have any content.
    bool empty() const;

    /// Returns the number of name-value pairs in the
    /// collection.
    int size() const;

    /// Removes all name-value pairs with the given name.
    void erase(const std::string& name);

    /// Removes all name-value pairs and their values.
    void clear();

private:
    Map _map;
};


typedef std::map<std::string, std::string> StringMap;
typedef std::vector<std::string> StringVec;


} // namespace scy


#endif // SCY_Containers_H


/// @\}
