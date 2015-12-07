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


#ifndef SCY_TimedManager_H
#define SCY_TimedManager_H


#include "scy/base.h"
#include "scy/collection.h"
#include "scy/timer.h"


namespace scy {


template <class TKey, class TValue, class TDeleter = std::default_delete<TValue>>
class TimedManager: public PointerCollection<TKey, TValue, TDeleter>
    /// Provides timed persistent data storage for class instances.
    /// TValue must implement the clone() method.
{
public:
    typedef PointerCollection<TKey, TValue, TDeleter> Base;
    typedef std::map<TValue*, Timeout> TimeoutMap;

    TimedManager(uv::Loop* loop = uv::defaultLoop()) :
        _timer(loop)
    {        
        _timer.Timeout += sdelegate(this, &TimedManager::onTimerUpdate);
        _timer.start(100); // check every 100ms
    }

    virtual ~TimedManager() 
    {
        _timer.Timeout -= sdelegate(this, &TimedManager::onTimerUpdate);
    }
    
    virtual void add(const TKey& key, TValue* item, long timeout = 0)
        // Add an item which will expire (and be deleted) after the 
        // specified timeout value.
        // If the timeout is 0 the item will be stored indefinitely.
        // The TimedManager assumes ownership of the given pointer.
    {
        Base::free(key); // Free existing item and timeout (if any)
        Base::add(key, item); // Add new entry        
        if (timeout > 0)
            setTimeout(item, timeout);
    }
    
    virtual bool expires(const TKey& key, long timeout) 
        // Update the item expiry timeout
    {
        TraceLS(this) << "Set expires: " << key << ": " << timeout << std::endl;    
        return expires(Base::get(key, false), timeout);
    }
    
    virtual bool expires(TValue* item, long timeout) 
        // Update the item expiry timeout
    {
        TraceLS(this) << "Set expires: " << item << ": " << timeout << std::endl;    
        return setTimeout(item, timeout);
    }

    virtual void clear()
    {
        Base::clear();
        Mutex::ScopedLock lock(_tmutex);
        _timeouts.clear();
    }

protected:
    virtual bool setTimeout(TValue* item, long timeout) 
    { 
        if (item) {
            Mutex::ScopedLock lock(_tmutex);
            if (timeout > 0) {
                TraceLS(this) << "Set timeout: " << item << ": " << timeout << std::endl;    
                auto& t = _timeouts[item];
                t.setDelay(timeout);
                t.start();
            }
            else {            
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

    virtual void onRemove(const TKey& key, TValue* item) 
    { 
        // Remove timeout entry
        Mutex::ScopedLock lock(_tmutex);    
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
    
    void onTimerUpdate(void*)
    {
        _tmutex.lock();
        TimeoutMap timeouts(_timeouts);
        _tmutex.unlock();
        
        //for (auto ref : timeouts) {}
        for (auto it = timeouts.begin(); it != timeouts.end(); ++it) {
            //TraceLS(this) << "Check item: " 
            //    << it->first << ": " 
            //    << it->second.delay() << ": " 
            //    << it->second.remaining() << std::endl;    
            if (it->second.expired()) {    
                //auto item = it->first;
                TraceLS(this) << "Item expired: " << it->first << std::endl;
                onTimeout(it->first);
                //it = _timeouts.erase(it);
            }
            //else ++it;
        }
    }

    mutable Mutex _tmutex;
    TimeoutMap _timeouts;
    Timer _timer;
};


} // namespace scy:


#endif // SCY_TimedManager_H