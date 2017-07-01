///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup util
/// @{


#ifndef SCY_TimedManager_H
#define SCY_TimedManager_H


#include "scy/base.h"
#include "scy/collection.h"
#include "scy/util/timeout.h"


namespace scy {

/// Timed pointer manager
///
/// Provides timed persistent data storage for class instances.
/// TValue must implement the clone() method.
template <class TKey, class TValue, class TDeleter = std::default_delete<TValue>>
class /* SCY_EXTERN */ TimedManager : public PointerCollection<TKey, TValue, TDeleter>
{
public:
    typedef PointerCollection<TKey, TValue, TDeleter> Base;
    typedef std::map<TValue*, Timeout> TimeoutMap;

    TimedManager(uv::Loop* loop = uv::defaultLoop())
        : _timer(100, 100, loop) // check every 100ms
    {
        _timer.start(std::bind(&TimedManager::onTimerUpdate, this));
    }

    virtual ~TimedManager()
    {
    }

    /// Add an item which will expire (and be deleted) after the
    /// specified timeout value.
    /// If the timeout is 0 the item will be stored indefinitely.
    /// The TimedManager assumes ownership of the given pointer.
    virtual void add(const TKey& key, TValue* item, long timeout = 0)
    {
        Base::free(key);      // Free existing item and timeout (if any)
        Base::add(key, item); // Add new entry
        if (timeout > 0)
            setTimeout(item, timeout);
    }

    /// Update the item expiry timeout
    virtual bool expires(const TKey& key, long timeout)
    {
        LTrace("Set expires: ", key, ": ", timeout)
        return expires(Base::get(key, false), timeout);
    }

    /// Update the item expiry timeout
    virtual bool expires(TValue* item, long timeout)
    {
        LTrace("Set expires: ", item, ": ", timeout)
        return setTimeout(item, timeout);
    }

    virtual void clear() override
    {
        Base::clear();
        std::lock_guard<std::mutex> guard(_tmutex);
        _timeouts.clear();
    }

protected:
    virtual bool setTimeout(TValue* item, long timeout)
    {
        if (item) {
            std::lock_guard<std::mutex> guard(_tmutex);
            if (timeout > 0) {
                LTrace("Set timeout: ", item, ": ", timeout)
                auto& t = _timeouts[item];
                t.setDelay(timeout);
                t.start();
            } else {
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

    virtual void onRemove(const TKey& key, TValue* item) override
    {
        // Remove timeout entry
        std::lock_guard<std::mutex> guard(_tmutex);
        auto it = _timeouts.find(item);
        if (it != _timeouts.end())
            _timeouts.erase(it);

        Base::onRemove(key, item);
    }

    virtual void onTimeout(TValue* item)
    {
        if (Base::remove(item)) { // will call onRemove
            TDeleter func;
            func(item);
        }
    }

    void onTimerUpdate()
    {
        _tmutex.lock();
        TimeoutMap timeouts(_timeouts);
        _tmutex.unlock();

        // for (auto ref : timeouts) {}
        for (auto it = timeouts.begin(); it != timeouts.end(); ++it) {
            // STrace << "Check item: "
            //    << it->first << ": "
            //    << it->second.delay() << ": "
            //    << it->second.remaining() << std::endl;
            if (it->second.expired()) {
                // auto item = it->first;
                LTrace("Item expired: ", it->first)
                onTimeout(it->first);
                // it = _timeouts.erase(it);
            }
            // else ++it;
        }
    }

    mutable std::mutex _tmutex;
    TimeoutMap _timeouts;
    Timer _timer;
};


} // namespace scy:


#endif // SCY_TimedManager_H


/// @\}
