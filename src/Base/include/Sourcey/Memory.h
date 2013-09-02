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


#ifndef SOURCEY_Memory_H
#define SOURCEY_Memory_H


#include "Sourcey/Logger.h"
#include "Sourcey/Types.h"
#include "Sourcey/Mutex.h"
#include "Sourcey/UV/UVPP.h"
#include "Sourcey/Singleton.h"
#include <exception>
#include <memory>
#include <atomic>
#include <vector>


namespace scy {

	
class AbstractDeleter;


class GarbageCollector: public uv::Handle
	/// Simple garbage collector for deferred pointer deletion.
{
public:	
	GarbageCollector();	
	~GarbageCollector();
		
	static GarbageCollector& instance();
		// Returns the GarbageCollector singleton.
	
	template <class C> void deleteLater(C* ptr);
		// Schedules a pointer for deferred deletion.
	
	static void shutdown();
		// Shuts down the garbage collector and deletes 
		// the singleton instance.
	
	void finalize();
		// Frees all scheduled pointers now.
	
	void close();
		// Closes the internal timer and frees all 
		// scheduled pointers now.

protected:	
	void onTimer();
		
	UVEmptyStatusCallback(GarbageCollector, onTimer, uv_timer_t);
	
	mutable Mutex _mutex;
	std::vector<AbstractDeleter*> _pending;
	std::vector<AbstractDeleter*> _ready;
};


//
/// Deleter Methods
//


#if 0 // use std::default_delete instead
template<class T> struct default_delete
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


template<class T> struct deferred_delete
{
	void operator()(T *ptr)
	{
		assert(ptr);		
		static_assert(0 < sizeof(T), 
			"can't delete an incomplete type");
		scy::deleteLater(ptr);
	}
};


template<class T> struct dispose_method
{
	void operator()(T *ptr)
	{
		assert(ptr);		
		static_assert(0 < sizeof(T), 
			"can't delete an incomplete type");
		ptr->dispose();
	}
};


//
/// Deleter classes
//


class AbstractDeleter
	/// AbstractDeleter provides an interface for 
	/// holding and deleting a pointer in various ways. 
	/// Do not use this class directly, use AsyncDeleter.
{
public:
	void* ptr;
	
	AbstractDeleter(void* p) : 
		ptr(p)
	{
	}

	virtual ~AbstractDeleter()
	{		
	}

	virtual void invoke() = 0;
};


template <class T, typename D = std::default_delete<T>>
class AsyncDeleter: public AbstractDeleter
	/// AsyncDeleter implements the AbstractDeleter interface  
	/// to provide different methods of deleting a pointer.
{
public:
	AsyncDeleter(T* p) : 
		AbstractDeleter(p)
	{
	}

	virtual ~AsyncDeleter()
	{
		if (ptr) invoke();
	}

    virtual void invoke()
    {
		D func;
		func((T*)ptr);
		ptr = nullptr;
    }
};


//
// Garbage Collector inlines
//


template <class C> inline void GarbageCollector::deleteLater(C* ptr)
	/// Schedules a pointer for deferred deletion.
{ 
	Mutex::ScopedLock lock(_mutex);
	_pending.push_back(new AsyncDeleter<C>(ptr));
}


template <class C> inline void deleteLater(C* ptr)
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
		deferred(deferred), count(1)
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
	friend struct scy::deferred_delete<SharedObject>;
	
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
	/// Note: Depreciated in favour of std::smart_ptr 
{
public:
	SharedPtr() : _ptr(nullptr)
	{
	}

	SharedPtr(C* ptr) : _ptr(ptr)
	{
	}

	SharedPtr(C* ptr, bool shared) : _ptr(ptr)
	{
		if (shared && _ptr) _ptr->duplicate();
	}

	SharedPtr(const SharedPtr& ptr) : _ptr(ptr._ptr)
	{
		if (_ptr) _ptr->duplicate();
	}

	~SharedPtr()
	{
		if (_ptr) _ptr->release();
	}
	
	SharedPtr& assign(C* ptr)
	{
		if (_ptr != ptr)
		{
			if (_ptr) _ptr->release();
			_ptr = ptr;
		}
		return *this;
	}

	SharedPtr& assign(C* ptr, bool shared)
	{
		if (_ptr != ptr)
		{
			if (_ptr) _ptr->release();
			_ptr = ptr;
			if (shared && _ptr) _ptr->duplicate();
		}
		return *this;
	}
	
	SharedPtr& assign(const SharedPtr& ptr)
	{
		if (&ptr != this)
		{
			if (_ptr) _ptr->release();
			_ptr = ptr._ptr;
			if (_ptr) _ptr->duplicate();
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
		if (_ptr)
			return _ptr;
		else
			throw std::runtime_error("Null pointer");
	}

	const C* operator -> () const
	{
		if (_ptr)
			return _ptr;
		else
			throw std::runtime_error("Null pointer");
	}

	C& operator * ()
	{
		if (_ptr)
			return *_ptr;
		else
			throw std::runtime_error("Null pointer");
	}

	const C& operator * () const
	{
		if (_ptr)
			return *_ptr;
		else
			throw std::runtime_error("Null pointer");
	}

	C* get()
	{
		return _ptr;
	}

	const C* get() const
	{
		return _ptr;
	}

	operator C* ()
	{
		return _ptr;
	}
	
	operator const C* () const
	{
		return _ptr;
	}
	
	bool operator ! () const
	{
		return _ptr == nullptr;
	}

	bool isNull() const
	{
		return _ptr == nullptr;
	}
	
	C* duplicate()
	{
		if (_ptr) _ptr->duplicate();
		return _ptr;
	}

	bool operator == (const SharedPtr& ptr) const
	{
		return _ptr == ptr._ptr;
	}

	bool operator == (const C* ptr) const
	{
		return _ptr == ptr;
	}

	bool operator == (C* ptr) const
	{
		return _ptr == ptr;
	}

	bool operator != (const SharedPtr& ptr) const
	{
		return _ptr != ptr._ptr;
	}

	bool operator != (const C* ptr) const
	{
		return _ptr != ptr;
	}

	bool operator != (C* ptr) const
	{
		return _ptr != ptr;
	}

	bool operator < (const SharedPtr& ptr) const
	{
		return _ptr < ptr._ptr;
	}

	bool operator < (const C* ptr) const
	{
		return _ptr < ptr;
	}

	bool operator < (C* ptr) const
	{
		return _ptr < ptr;
	}

	bool operator <= (const SharedPtr& ptr) const
	{
		return _ptr <= ptr._ptr;
	}

	bool operator <= (const C* ptr) const
	{
		return _ptr <= ptr;
	}

	bool operator <= (C* ptr) const
	{
		return _ptr <= ptr;
	}

	bool operator > (const SharedPtr& ptr) const
	{
		return _ptr > ptr._ptr;
	}

	bool operator > (const C* ptr) const
	{
		return _ptr > ptr;
	}

	bool operator > (C* ptr) const
	{
		return _ptr > ptr;
	}

	bool operator >= (const SharedPtr& ptr) const
	{
		return _ptr >= ptr._ptr;
	}

	bool operator >= (const C* ptr) const
	{
		return _ptr >= ptr;
	}

	bool operator >= (C* ptr) const
	{
		return _ptr >= ptr;
	}

private:
	C* _ptr;
};


} // namespace scy


#endif // SOURCEY_Memory_H



/*
template <class C>
class Deleter: public AbstractDeleter
	/// Deleter is the base deleter template  
	/// from which all others derive.
{
public:
    typedef void (*Func)(C*);
	Func func;

	Deleter(C* p, Func f) : 
		AbstractDeleter(p), func(f)
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
		//AbstractDeleter<Type>::


	//: public MemoryObject//<deleter_t>//AbstractDeleter* deleter = new DefaultDeleter<SharedObject>()MemoryObject(deleter), 
			//MemoryObject::
		//deleter(this);
		//Deleter::();
		//deleter->invoke();
	//friend struct std::default_delete/*friend class AsyncDeleter*/<SharedObject>;	
	//friend class DeferredDeleter<SharedObject>;

/*
//template <class deleter_t>
class MemoryObject//: public 
	/// MemoryObject is the base class for LibSourcey objects
	/// which employ different memory management strategies.
{	
public:
	MemoryObject() //: //AbstractDeleter* deleter = new DefaultDeleter<MemoryObject>()
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
	
	//friend struct scy::deferred_delete/friend class AsyncDeleter/<MemoryObject>;	
	//friend class DeferredDeleter<MemoryObject>;

	//deleter_t deleter;
	//std::unique_ptr<AbstractDeleter> deleter;
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
	std::atomic<unsigned> refCount;

	explicit RefCountedBase(unsigned int count = 1) : refCount(count) {}

	void increment(unsigned int count = 1)
	{
		refCount.fetch_add(count, std::memory_order_relaxed);
	}

	void release(unsigned int count = 1)
	{
		if (refCount.fetch_sub(count, std::memory_order_release) == count) {
			std::atomic_thread_fence(std::memory_order_acquire);
			delete static_cast<T*>(this);
		}
	}
};
*/