///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup util
/// @{


#ifndef SCY_IRegistry_H
#define SCY_IRegistry_H


#include "scy/signal.h"

#include <map>


namespace scy {


template <class ItemT> class IRegistry
{
    template <typename T> ItemT* createT() { return new T(); }

public:
    typedef std::map<std::string, ItemT* (*)()> TypeMap;

    IRegistry() = default;
    virtual ~IRegistry() = default;

    virtual ItemT* createInstance(const std::string& s)
    {
        TypeMap::iterator it = _types.find(s);
        if (it == _types.end())
            return NULL;
        return it->second();
    }

    template <typename T> void registerType(const std::string& s)
    {
        _types.insert(std::make_pair(s, &createT<T>));
        TypeRegistered.emit(s);
    }

    virtual void unregisterType(const std::string& s)
    {
        auto it = _types.find(s);
        if (it == _types.end())
            return;
        _types.erase(it);
        TypeUnregistered.emit(s);
    }

    TypeMap types() const { return _types; }

    Signal<void(const std::string&)> TypeRegistered;
    Signal<void(const std::string&)> TypeUnregistered;

private:
    TypeMap _types;
};


} // namespace scy


#endif // SCY_IRegistry_H


/// @\}
