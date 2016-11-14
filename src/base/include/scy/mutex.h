///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_Mutex_H
#define SCY_Mutex_H


#include "scy/uv/uvpp.h"


namespace scy {


/// ScopedLock simplifies thread synchronization
/// with a Mutex or similar lockable object.
/// The given Mutex is locked in the constructor,
/// and unlocked it in the destructor.
/// T can be any class with lock() and unlock() functions.
template <class T>
class ScopedLock
{
public:
    explicit ScopedLock(T& m) : _m(m)
    {
        _m.lock();
    }

    ~ScopedLock()
    {
        _m.unlock();
    }

private:
    ScopedLock();
    ScopedLock(const ScopedLock&);
    ScopedLock& operator = (const ScopedLock&);

    T& _m;
};


/// This class is a wrapper around uv_mutex_t.
///
/// A Mutex (mutual exclusion) is a synchronization mechanism
/// used to control access to a shared resource in a concurrent
/// (multithreaded) scenario.
///
/// The ScopedLock class is usually used to obtain a Mutex lock,
/// since it makes locking exception-safe.
class Mutex
{
public:
    typedef scy::ScopedLock<Mutex> ScopedLock;

    Mutex();
    ~Mutex();

    /// Locks the mutex.
    /// Blocks if the mutex is held by another thread.
    void lock();

    /// Tries to lock the mutex. Returns false if the
    /// mutex is already held by another thread.
    /// Returns true if the mutex was successfully locked.
    bool tryLock();

    /// Unlocks the mutex so that it can be acquired by
    /// other threads.
    void unlock();

private:
    Mutex(const Mutex&);
    Mutex& operator = (const Mutex&);

    uv_mutex_t _mx;
};


// TODO: RwLock
// TODO: Condition


} // namespace scy


#endif // SCY_Mutex_H

/// @\}
