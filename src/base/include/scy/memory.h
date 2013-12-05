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


#ifndef SCY_Memory_H
#define SCY_Memory_H


#include "scy/logger.h"
#include "scy/types.h"
#include "scy/mutex.h"
#include "scy/uv/uvpp.h"
#include "scy/singleton.h"
#include <exception>
#include <memory>
#include <atomic>
#include <vector>


namespace scy {

	
class DeferredDeleter;


class GarbageCollector
	/// Simple garbage collector for deferred pointer deletion.
{
public:	
	GarbageCollector();	
	~GarbageCollector();
		
	static GarbageCollector& instance();
		// Returns the GarbageCollector singleton.
	
	template <class C> void deleteLater(C* ptr);
		// Schedules a pointer for deferred deletion.
	
	template <class C> void deleteLater(std::shared_ptr<C> ptr);
		// Schedules a shared pointer for deferred deletion.

	void finalize();
		// Frees all scheduled pointers now.
		// This method must be called from the main thread
		// while the event loop is inactive.
	
	static void shutdown();
		// Shuts down the garbage collector and deletes 
		// the singleton instance.
		// This method must be called from the main thread
		// while the event loop is inactive.

	unsigned long tid();
		// Returns the TID of the garbage collector event loop thread.
		// The garbage collector must be running.

protected:	
	static void onTimer(uv_timer_t* handle, int status);
	void runAsync();
		
	mutable Mutex _mutex;
	unsigned long _tid;
	std::vector<DeferredDeleter*> _pending;
	std::vector<DeferredDeleter*> _ready;
	uv::Handle _handle;
	bool _finalize;
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


template<class T> struct Deferred
{
	void operator()(T *ptr)
	{
		assert(ptr);
		static_assert(0 < sizeof(T), 
			"can't delete an incomplete type");
		GarbageCollector::instance().deleteLater(ptr);
	}
};


template<class T> struct Dispose
{
	void operator()(T *ptr)
	{
		assert(ptr);		
		static_assert(0 < sizeof(T), 
			"can't delete an incomplete type");
		ptr->dispose();
	}
};


template<class T> struct Array
{
	void operator()(T *ptr)
	{
		assert(ptr);		
		static_assert(0 < sizeof(T), 
			"can't delete an incomplete type");
		delete [] ptr;
		ptr->dispose();
	}
};


} // namespace deleter


//
/// Deleter Storage Classes
//


class DeferredDeleter
	/// DeferredDeleter provides an interface for holding 
	/// and ansynchronously deleting a pointer in various ways. 
{
public:
	DeferredDeleter() {}
	virtual ~DeferredDeleter() {}
};


template <class T, typename D = std::default_delete<T>>
class PointerDeleter: public DeferredDeleter
	/// PointerDeleter implements the DeferredDeleter interface  
	/// to provide a method for deleting a raw pointer.
{
public:
	void* ptr;
	
	PointerDeleter(void* p) : 
		ptr(p)
	{
	}

	virtual ~PointerDeleter()
	{
		D func;
		func((T*)ptr);
		ptr = nullptr;
	}
};


template <class T>
class SharedPtrDeleter: public DeferredDeleter
	/// SharedPtrDeleter implements the DeferredDeleter interface to
	/// provide deferred deletion for shared_ptr managed pointers.
	/// Note that this class does not guarantee deletion of the managed
	/// pointer; all it does is copy the shared_ptr and release it when
	/// the SharedPtrDeleter instance is deleted, which makes it useful
	/// for certain asyncronous scenarios.
{
public:
	std::shared_ptr<T> ptr;
	
	SharedPtrDeleter(std::shared_ptr<T> p) : 
		ptr(p)
	{
		assert(ptr);
	}

	virtual ~SharedPtrDeleter()
	{
	}
};


//
// Garbage Collector inlines
//


template <class C> inline void GarbageCollector::deleteLater(C* ptr)
	/// Schedules a pointer for deferred deletion.
{ 
	Mutex::ScopedLock lock(_mutex);
	_pending.push_back(new PointerDeleter<C>(ptr));
}


template <class C> inline void GarbageCollector::deleteLater(std::shared_ptr<C> ptr)
	/// Schedules a shared pointer for deferred deletion.
{ 
	Mutex::ScopedLock lock(_mutex);
	_pending.push_back(new SharedPtrDeleter<C>(ptr));
}


template <class C> inline void deleteLater(C* ptr)
	/// Convenience function for accessing GarbageCollector::deleteLater
{
	GarbageCollector::instance().deleteLater(ptr);
}


template <class C> inline void deleteLater(std::shared_ptr<C> ptr)
	/// Convenience function for accessing GarbageCollector::deleteLater
{
	GarbageCollector::instance().deleteLater(ptr);
}
	

//
// Memory and Reference Counted Objects
//


class SharedObject
	/// SharedObject is the base class for objects that  
	/// employ reference counting based garbage collection.
	///
	/// Reference-counted objects inhibit construction by
	/// copying and assignment.
{
public:
	SharedObject(bool deferred = false) : 
		count(1), deferred(deferred)
		// Creates the SharedObject with an 
		// initial reference count of one.
	{
	}
	
	void duplicate()
		// Increment the object's reference count.
	{
		std::atomic_fetch_add_explicit(&count, 1u, std::memory_order_relaxed);
	}
		
	void release()
		// Decrement the object's reference count and
		// calls delete if the count reaches zero.
	{
		if (std::atomic_fetch_sub_explicit(&count, 1u, std::memory_order_release) == 1) {
			std::atomic_thread_fence(std::memory_order_acquire);
			freeMemory(); 
		}
	}
		
	unsigned refCount() const
	{
		return count;
	}

protected:
	virtual void freeMemory()
		// Deletes the instance when the reference count reaches zero.
		// This method can be overridden for different deletion strategies.
	{
		if (deferred)
			deleteLater<SharedObject>(this);
		else
			delete this;
	}

	virtual ~SharedObject() {}
		// Destroys the SharedObject.
		// The destructor should never be called directly.

	SharedObject(const SharedObject&);
	SharedObject& operator = (const SharedObject&);
	
	friend struct std::default_delete<SharedObject>;
	//friend struct deleter::Deferred<SharedObject>;
	
	std::atomic<unsigned> count;
	bool deferred;
};


template <class C>
class SharedPtr	
	/// SharedPtr manages a pointer to reference counted object.
	///
	/// The template class must implement duplicate() and
	/// release() methods, such as SharedObject.
	///
	/// Note: Depreciated in favour of std::smart_handle 
{
public:
	SharedPtr() : _handle(nullptr)
	{
	}

	SharedPtr(C* ptr) : _handle(ptr)
	{
	}

	SharedPtr(C* ptr, bool shared) : _handle(ptr)
	{
		if (shared && _handle) _handle->duplicate();
	}

	SharedPtr(const SharedPtr& ptr) : _handle(ptr._handle)
	{
		if (_handle) _handle->duplicate();
	}

	~SharedPtr()
	{
		if (_handle) _handle->release();
	}
	
	SharedPtr& assign(C* ptr)
	{
		if (_handle != ptr)
		{
			if (_handle) _handle->release();
			_handle = ptr;
		}
		return *this;
	}

	SharedPtr& assign(C* ptr, bool shared)
	{
		if (_handle != ptr)
		{
			if (_handle) _handle->release();
			_handle = ptr;
			if (shared && _handle) _handle->duplicate();
		}
		return *this;
	}
	
	SharedPtr& assign(const SharedPtr& ptr)
	{
		if (&ptr != this)
		{
			if (_handle) _handle->release();
			_handle = ptr._handle;
			if (_handle) _handle->duplicate();
		}
		return *this;
	}

	SharedPtr& operator = (C* ptr)
	{
		return assign(ptr);
	}

	SharedPtr& operator = (const SharedPtr& ptr)
	{
		return assign(ptr);
	}

	C* operator -> ()
	{
		if (_handle)
			return _handle;
		else
			throw std::runtime_error("Null pointer");
	}

	const C* operator -> () const
	{
		if (_handle)
			return _handle;
		else
			throw std::runtime_error("Null pointer");
	}

	C& operator * ()
	{
		if (_handle)
			return *_handle;
		else
			throw std::runtime_error("Null pointer");
	}

	const C& operator * () const
	{
		if (_handle)
			return *_handle;
		else
			throw std::runtime_error("Null pointer");
	}

	C* get()
	{
		return _handle;
	}

	const C* get() const
	{
		return _handle;
	}

	operator C* ()
	{
		return _handle;
	}
	
	operator const C* () const
	{
		return _handle;
	}
	
	bool operator ! () const
	{
		return _handle == nullptr;
	}

	bool isNull() const
	{
		return _handle == nullptr;
	}
	
	C* duplicate()
	{
		if (_handle) _handle->duplicate();
		return _handle;
	}

	bool operator == (const SharedPtr& ptr) const
	{
		return _handle == ptr._handle;
	}

	bool operator == (const C* ptr) const
	{
		return _handle == ptr;
	}

	bool operator == (C* ptr) const
	{
		return _handle == ptr;
	}

	bool operator != (const SharedPtr& ptr) const
	{
		return _handle != ptr._handle;
	}

	bool operator != (const C* ptr) const
	{
		return _handle != ptr;
	}

	bool operator != (C* ptr) const
	{
		return _handle != ptr;
	}

	bool operator < (const SharedPtr& ptr) const
	{
		return _handle < ptr._handle;
	}

	bool operator < (const C* ptr) const
	{
		return _handle < ptr;
	}

	bool operator < (C* ptr) const
	{
		return _handle < ptr;
	}

	bool operator <= (const SharedPtr& ptr) const
	{
		return _handle <= ptr._handle;
	}

	bool operator <= (const C* ptr) const
	{
		return _handle <= ptr;
	}

	bool operator <= (C* ptr) const
	{
		return _handle <= ptr;
	}

	bool operator > (const SharedPtr& ptr) const
	{
		return _handle > ptr._handle;
	}

	bool operator > (const C* ptr) const
	{
		return _handle > ptr;
	}

	bool operator > (C* ptr) const
	{
		return _handle > ptr;
	}

	bool operator >= (const SharedPtr& ptr) const
	{
		return _handle >= ptr._handle;
	}

	bool operator >= (const C* ptr) const
	{
		return _handle >= ptr;
	}

	bool operator >= (C* ptr) const
	{
		return _handle >= ptr;
	}

private:
	C* _handle;
};


} // namespace scy


#endif // SCY_Memory_H



	
	//void close();
		// Closes the internal timer and frees all 
		// scheduled pointers now.
	//UVEmptyStatusCallback(GarbageCollector, onTimer, uv_timer_t);
/*
template <class C>
class Deleter: public DeferredDeleter
	/// Deleter is the base deleter template  
	/// from which all others derive.
{
public:
    typedef void (*Func)(C*);
	Func func;

	Deleter(C* p, Func f) : 
		DeferredDeleter(p), func(f)
	{
	}

	~Deleter()
	{
		if (ptr) invoke();
	}

    void invoke()
    {
		assert(ptr);
		//C* p = reinterpret_cast<C*>(ptr);
		C* p = (C*)ptr;
		ptr = nullptr;
        func(p);
    }
};


template <class C>
class DefaultDeleter: public Deleter<C>
	/// DefaultDeleter calls the standard delete 
	/// operator to free pointer memory.
{
public:
	DefaultDeleter(C* p = nullptr) : 
		Deleter<C>(p, &DefaultDeleter<C>::func)
	{
	}
				
	static void func(C* p)
	{
		delete p;
	}
};


template <class C>
class DeferredDeleter: public Deleter<C>
	/// DeferredDeleter schedules a pointer for 
	/// deferred deletion by the GarbageCollector.
{
public:
	DeferredDeleter(C* p = nullptr) : 
		Deleter<C>(p, &DeferredDeleter<C>::func)
	{
	}
				
	static void func(C* p)
	{
		scy::deleteLater(p);
	}
};
*/
    //typedef void (*Func)(Type*);
	//Func func;, Func f
		//C* p = reinterpret_cast<C*>(ptr);
        //func(p);//, func(f)//<Type>
		//Type* p = (Type*)ptr;
		//ptr = nullptr;
		//DeferredDeleter<Type>::


	//: public MemoryObject//<deleter_t>//DeferredDeleter* deleter = new DefaultDeleter<SharedObject>()MemoryObject(deleter), 
			//MemoryObject::
		//deleter(this);
		//Deleter::();
		//deleter->invoke();
	//friend struct std::default_delete<SharedObject>;	
	//friend class DeferredDeleter<SharedObject>;

/*
//template <class deleter_t>
class MemoryObject//: public 
	/// MemoryObject is the base class for LibSourcey objects
	/// which employ different memory management strategies.
{	
public:
	MemoryObject(): //DeferredDeleter* deleter = new DefaultDeleter<MemoryObject>()
		//deleter(deleter)
	{
		//if (deleter->ptr == nullptr)
		//	deleter->ptr = this;
	}

	virtual ~MemoryObject()
	{
	}

	virtual void freeMemory()
	{
		//deleter(this);
		//Deleter::();
		//deleter->invoke();
	}

protected:
	MemoryObject(const MemoryObject&) {};
	MemoryObject& operator = (const MemoryObject&) {};
	
	//friend struct scy::DeferredDelete/friend class PointerDeleter/<MemoryObject>;	
	//friend class DeferredDeleter<MemoryObject>;

	//deleter_t deleter;
	//std::unique_handle<DeferredDeleter> deleter;
};
//template <class deleter_t>
*/



/*
template <class C>
class GCDeleter: public Deleter<C>
	/// GCDeleter is used by the GarbageCollector
	/// to free managed pointer memory.
{
public:
	GCDeleter(C* p = nullptr) : 
		Deleter<C>(p, &GCDeleter<C>::func)
	{
	}
				
	static void func(C* p)
	{
		delete p;
	}
};


template <class C>
class DestroyMethodDeleter: public Deleter<C>
	/// DestroyMethodDeleter calls the destroy() method 
	/// on an object to begin the deletion sequence.
{
public:
	DestroyMethodDeleter(C* p = nullptr) : 
		Deleter<C>(p, &DestroyMethodDeleter<C>::func)
	{
	}
				
	static void func(C* p)
	{
		p->destroy();
	}
};
*/


/*
/// Reference-counted object base class
/// By default the reference count is initialized to 1
template<typename T> struct RefCountedBase
{
	std::atomic<unsigned> ndelegates;

	explicit RefCountedBase(unsigned int count = 1) : ndelegates(count) {}

	void increment(unsigned int count = 1)
	{
		ndelegates.fetch_add(count, std::memory_order_relaxed);
	}

	void release(unsigned int count = 1)
	{
		if (ndelegates.fetch_sub(count, std::memory_order_release) == count) {
			std::atomic_thread_fence(std::memory_order_acquire);
			delete static_cast<T*>(this);
		}
	}
};
*/
