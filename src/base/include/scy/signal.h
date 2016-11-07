///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_Signal_H
#define SCY_Signal_H


#include <cstdint>
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


/// This exception is used to break out of a Signal callback scope.
class StopPropagation: public std::exception
{
public:
    virtual ~StopPropagation() throw() {};
};


/// This class implements a thread-safe signal which
/// broadcasts arbitrary data to multiple receiver delegates.
template <class DelegateT, DelegateDefaultArgs>
class SignalBase
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

    /// Attaches a delegate to the signal. If the delegate
    /// already exists it will overwrite the previous delegate.
    void attach(const DelegateT& delegate)
    {
        detach(delegate);
        Mutex::ScopedLock lock(_mutex);
        _delegates.push_back(delegate.clone());
        _delegates.sort(DelegateT::ComparePrioroty);
        _count++;
    }

    /// Detaches a delegate from the signal.
    /// Returns true if the delegate was detached, false otherwise.
    bool detach(const DelegateT& delegate)
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

    /// Detaches all delegates associated with the given class instance.
    void detach(const void* klass)
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

    /// Deletes cancelled delegates.
    void cleanup()
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

    /// Retrieves a list of active delegates while
    /// simultaneously deleting any redundant delegates.
    void obtain(DelegateList& active)
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

    /// Returns the number of delegates connected to the signal.
    /// Use this instead of delegates().size() since the container
    /// is not updated in real time.
    int ndelegates() const
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

/// @\}
