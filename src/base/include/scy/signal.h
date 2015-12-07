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


#ifndef SCY_Signal_H
#define SCY_Signal_H


#include "scy/types.h"
#include "scy/delegate.h"
#include "scy/util.h"
#include "scy/mutex.h"
#include <vector>
#include <assert.h>

#if defined(__GNUC__)
   // We're all grown up and we know that simulating
   // reinterpret_cast<> may cause strict aliasing problems
   // at runtime.
#  pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif


namespace scy {


class StopPropagation: public std::exception
    /// This exception is used to break out of a Signal callback scope.
{
public:
    virtual ~StopPropagation() throw() {};
};


template <class DelegateT, DelegateDefaultArgs>
class SignalBase 
    /// This class implements a thread-safe signal which
    /// broadcasts arbitrary data to multiple receiver delegates.
{
public:
    typedef std::list<DelegateT*>                  DelegateList;
    typedef typename DelegateList::iterator       Iterator;
    typedef typename DelegateList::const_iterator ConstIterator;

    SignalBase() : 
        _enabled(true), 
        _dirty(false), 
        _count(0)
    {
    }    

    virtual ~SignalBase() 
    { 
        clear();
    }

    void operator += (const DelegateT& delegate) { attach(delegate); }    
    void operator -= (const DelegateT& delegate) { detach(delegate); }    
    void operator -= (const void* klass) { detach(klass); }

    void attach(const DelegateT& delegate) 
        // Attaches a delegate to the signal. If the delegate 
        // already exists it will overwrite the previous delegate.
    {
        detach(delegate);
        Mutex::ScopedLock lock(_mutex);
        _delegates.push_back(delegate.clone());
        _delegates.sort(DelegateT::ComparePrioroty); 
        _count++;
    }

    bool detach(const DelegateT& delegate) 
        // Detaches a delegate from the signal.
        // Returns true if the delegate was detached, false otherwise.
    {
        Mutex::ScopedLock lock(_mutex);
        for (Iterator it = _delegates.begin(); it != _delegates.end(); ++it) {
            if (delegate.equals(*it) && !(*it)->cancelled()) {    
                (*it)->cancel();
                _dirty = true;
                _count--;
                return true;
            }
        }
        return false;
    }

    void detach(const void* klass) 
        // Detaches all delegates associated with the given class instance.
    {
        {
            Mutex::ScopedLock lock(_mutex);
            for (Iterator it = _delegates.begin(); it != _delegates.end(); ++it) {
                if (klass == (*it)->object() && !(*it)->cancelled()) {    
                    (*it)->cancel();
                    _dirty = true;
                    _count--;
                }
            }
        }

        // Call cleanup after detaching a class
        cleanup();
    }

    void cleanup() 
        // Deletes cancelled delegates.
    {
        Mutex::ScopedLock lock(_mutex);
        if (_dirty) {
            _dirty = false;
            Iterator it = _delegates.begin(); 
            while (it != _delegates.end()) {
                DelegateT* delegate = *it;
                if (delegate->cancelled()) {
                    delete delegate;
                    it = _delegates.erase(it);
                }
                else
                    ++it;
            }
        }
    }

    void obtain(DelegateList& active) 
        // Retrieves a list of active delegates while 
        // simultaneously deleting any redundant delegates.
    {
        Mutex::ScopedLock lock(_mutex);
        if (!_enabled) // skip if disabled
            return;
        Iterator it = _delegates.begin(); 
        while (it != _delegates.end()) {
            DelegateT* delegate = *it;
            if (delegate->cancelled()) {
                assert(_dirty);
                delete delegate;
                it = _delegates.erase(it);
            }
            else {  
                active.push_back(delegate);
                ++it;
            }
        }

        _dirty = false;
    }

    virtual void emit(void* sender) 
    {
        void* empty = nullptr;
        emit(sender, (P)empty, (P2)empty, (P3)empty, (P4)empty);
    }

    virtual void emit(void* sender, P arg) 
    {
        void* empty = nullptr;
        emit(sender, arg, (P2)empty, (P3)empty, (P4)empty);
    }

    virtual void emit(void* sender, P arg, P2 arg2) 
    {
        void* empty = nullptr;
        emit(sender, arg, arg2, (P3)empty, (P4)empty);
    }    

    virtual void emit(void* sender, P arg, P2 arg2, P3 arg3) 
    {
        void* empty = nullptr;
        emit(sender, arg, arg2, arg3, (P4)empty);
    }

    virtual void emit(void* sender, P arg, P2 arg2, P3 arg3, P4 arg4) 
    {
        DelegateList toNotify;
        obtain(toNotify);
        try {
            for (ConstIterator it = toNotify.begin(); it != toNotify.end(); ++it) {
                if ((*it)->accepts(sender, arg, arg2, arg3, arg4))
                    (*it)->emit(sender, arg, arg2, arg3, arg4); 
            }
        }
        catch (StopPropagation&) {
        }
    }

    void clear() 
    {
        Mutex::ScopedLock lock(_mutex);
        util::clearList(_delegates);
        _count = 0;
    }

    void enable(bool flag = true) 
    {
        Mutex::ScopedLock lock(_mutex);
        _enabled = flag;
    }

    bool enabled() const
    {
        Mutex::ScopedLock lock(_mutex);
        return _enabled;
    }

    DelegateList delegates() const 
    {
        Mutex::ScopedLock lock(_mutex);
        return _delegates;
    }
    
    int ndelegates() const 
        // Returns the number of delegates connected to the signal.
        // Use this instead of delegates().size() since the container
        // is not updated in real time.
    {
        Mutex::ScopedLock lock(_mutex);
        return _count;
    }
        
protected:
    DelegateList _delegates;
    bool _enabled;    
    bool _dirty;
    int _count;

    mutable Mutex    _mutex;
};


//
// Signal Types
//


class NullSignal: public SignalBase<DelegateBase<>> {};


template <typename P>
class Signal: public SignalBase<DelegateBase<P>, P> {};


template <typename P, typename P2>
class Signal2: public SignalBase<DelegateBase<P, P2>, P, P2> {};


template <typename P, typename P2, typename P3>
class Signal3: public SignalBase<DelegateBase<P, P2, P3>, P, P2, P3> {};


template <typename P, typename P2, typename P3, typename P4>
class Signal4: public SignalBase<DelegateBase<P, P2, P3, P4>, P, P2, P3, P4> {};


} // namespace scy


#endif // SCY_Signal_H
