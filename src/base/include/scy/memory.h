///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup base
/// @{

#ifndef SCY_Memory_H
#define SCY_Memory_H

#include "scy/logger.h"
#include "scy/mutex.h"
#include "scy/singleton.h"
#include "scy/uv/uvpp.h"
#include <atomic>
#include <cstdint>
#include <exception>
#include <memory>
#include <vector>

namespace scy {

class ScopedPointer;

/// Simple garbage collector for deferred pointer deletion.
class GarbageCollector
{
public:
    GarbageCollector();
    ~GarbageCollector();

    /// Returns the GarbageCollector singleton.
    static GarbageCollector& instance();

    /// Shuts down the garbage collector and deletes
    /// the singleton instance.
    /// This method must be called from the main thread
    /// while the event loop is inactive.
    static void destroy();

    /// Schedules a pointer for deferred deletion.
    template <class C> void deleteLater(C* ptr);

    /// Schedules a shared pointer for deferred deletion.
    template <class C> void deleteLater(std::shared_ptr<C> ptr);

    /// Frees all scheduled pointers now.
    /// This method must be called from the main thread
    /// while the event loop is inactive.
    void finalize();

    /// Returns the TID of the garbage collector event loop thread.
    /// The garbage collector must be running.
    uv_thread_t tid();

protected:
    static void onTimer(uv_timer_t* handle);
    void runAsync();

    mutable Mutex _mutex;
    std::vector<ScopedPointer*> _pending;
    std::vector<ScopedPointer*> _ready;
    uv::Handle _handle;
    bool _finalize;
    uv_thread_t _tid;
};

//
/// Deleter Functors
//

namespace deleter {

#if 0 // use std::default_delete instead
template<class T> struct Default
{
    void operator()(T *ptr)
    {
        assert(ptr);
        static_assert(0 < sizeof(T),
            "can't delete an incomplete type");
        delete ptr;
    }
};
#endif

template <class T> struct Deferred
{
    void operator()(T* ptr)
    {
        assert(ptr);
        static_assert(0 < sizeof(T), "can't delete an incomplete type");
        GarbageCollector::instance().deleteLater(ptr);
    }
};

template <class T> struct Dispose
{
    void operator()(T* ptr)
    {
        assert(ptr);
        static_assert(0 < sizeof(T), "can't delete an incomplete type");
        ptr->dispose();
    }
};

template <class T> struct Array
{
    void operator()(T* ptr)
    {
        assert(ptr);
        static_assert(0 < sizeof(T), "can't delete an incomplete type");
        delete[] ptr;
        ptr->dispose();
    }
};

} // namespace deleter

//
/// Scoped Pointer Classes
//

/// ScopedPointer provides an interface for holding
/// and ansynchronously deleting a pointer in various ways.
class ScopedPointer
{
public:
    ScopedPointer() {}
    virtual ~ScopedPointer() {}
};

/// ScopedRawPointer implements the ScopedPointer interface
/// to provide a method for deleting a raw pointer.
template <class T, typename D = std::default_delete<T>>
class ScopedRawPointer : public ScopedPointer
{
public:
    void* ptr;

    ScopedRawPointer(void* p)
        : ptr(p)
    {
    }

    virtual ~ScopedRawPointer()
    {
        D func;
        func((T*)ptr);
        ptr = nullptr;
    }
};

/// ScopedSharedPointer implements the ScopedPointer interface to
/// provide deferred deletion for shared_ptr managed pointers.
/// Note that this class does not guarantee deletion of the managed
/// pointer; all it does is copy the shared_ptr and release it when
/// the ScopedSharedPointer instance is deleted, which makes it useful
/// for certain asyncronous scenarios.
template <class T> //, typename D = std::default_delete<T>
class ScopedSharedPointer : public ScopedPointer
{
public:
    std::shared_ptr<T> ptr;

    ScopedSharedPointer(std::shared_ptr<T> p)
        : ptr(p)
    {
        assert(ptr);
    }

    virtual ~ScopedSharedPointer() {}
};

//
// Garbage Collector inlines
//

/// Schedules a pointer for deferred deletion.
template <class C> inline void GarbageCollector::deleteLater(C* ptr)
{
    Mutex::ScopedLock lock(_mutex);
    _pending.push_back(new ScopedRawPointer<C>(ptr));
}

/// Schedules a shared pointer for deferred deletion.
template <class C>
inline void GarbageCollector::deleteLater(std::shared_ptr<C> ptr)
{
    Mutex::ScopedLock lock(_mutex);
    _pending.push_back(new ScopedSharedPointer<C>(ptr));
}

/// Convenience function for accessing GarbageCollector::deleteLater
template <class C> inline void deleteLater(C* ptr)
{
    GarbageCollector::instance().deleteLater(ptr);
}

/// Convenience function for accessing GarbageCollector::deleteLater
template <class C> inline void deleteLater(std::shared_ptr<C> ptr)
{
    GarbageCollector::instance().deleteLater(ptr);
}

//
// Memory and Reference Counted Objects
//

/// SharedObject is the base class for objects that
/// employ reference counting based garbage collection.
///
/// Reference-counted objects inhibit construction by
/// copying and assignment.
class SharedObject
{
public:
    /// Creates the SharedObject with an
    /// initial reference count of one.
    SharedObject(bool deferred = false)
        : count(1)
        , deferred(deferred)
    {
    }

    /// Increment the object's reference count.
    void duplicate()
    {
        std::atomic_fetch_add_explicit(&count, 1u, std::memory_order_relaxed);
    }

    /// Decrement the object's reference count and
    /// calls delete if the count reaches zero.
    void release()
    {
        if (std::atomic_fetch_sub_explicit(&count, 1u,
                                           std::memory_order_release) == 1) {
            std::atomic_thread_fence(std::memory_order_acquire);
            freeMemory();
        }
    }

    unsigned refCount() const { return count; }

protected:
    /// Deletes the instance when the reference count reaches zero.
    /// This method can be overridden for different deletion strategies.
    virtual void freeMemory()
    {
        if (deferred)
            deleteLater<SharedObject>(this);
        else
            delete this;
    }

    /// Destroys the SharedObject.
    /// The destructor should never be called directly.
    virtual ~SharedObject() {}

    SharedObject(const SharedObject&);
    SharedObject& operator=(const SharedObject&);

    friend struct std::default_delete<SharedObject>;
    // friend struct deleter::Deferred<SharedObject>;

    std::atomic<unsigned> count;
    bool deferred;
};

} // namespace scy

#endif // SCY_Memory_H

/// @\}
