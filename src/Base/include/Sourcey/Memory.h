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
#include "Sourcey/Atomic.h"
#include "Sourcey/UV/UVPP.h"
#include "Sourcey/Singleton.h"


namespace scy {


class AbstractDeleter;
class GarbageCollector: public uv::Handle
	// Simple garbage collector for deferred pointer deletion.
{
public:	
	GarbageCollector();	
	~GarbageCollector();
		
	static GarbageCollector& instance();
		// Returns the GarbageCollector singleton.
	
	template <class C>
	void deleteLater(C* ptr);
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
// Deleter Methods
//


class AbstractDeleter
	// AbstractDeleter provides an interface for 
	// deleting pointer memory in various ways.
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


template <class C>
class Deleter: public AbstractDeleter
	// Deleter is the base template class for 
	// deleting a type cast pointer.
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
		ptr = NULL;
        func(p);
    }
};


template <class C>
class DefaultDeleter: public Deleter<C>
	// DefaultDeleter calls the standard delete 
	// operator to free pointer memory.
{
public:
	DefaultDeleter(C* p = NULL) : 
		Deleter<C>(p, &DefaultDeleter<C>::func)
	{
	}
				
	static void func(C* p)
	{
		//traceL("DefaultDeleter") << "Destroying: " << p << std::endl;
		delete p;
	}
};


template <class C>
class GCDeleter: public Deleter<C>
	// GCDeleter is used by the GarbageCollector
	// to free managed pointer memory.
{
public:
	GCDeleter(C* p = NULL) : 
		Deleter<C>(p, &GCDeleter<C>::func)
	{
	}
				
	static void func(C* p)
	{
		//traceL("GCDeleter") << "Destroying: " << p << std::endl;
		delete p;
	}
};


template <class C>
class DeferredDeleter: public Deleter<C>
	// DeferredDeleter schedules a pointer for 
	// deferred deletion by the GarbageCollector.
{
public:
	DeferredDeleter(C* p = NULL) : 
		Deleter<C>(p, &DeferredDeleter<C>::func)
	{
	}
				
	static void func(C* p)
	{
		//traceL("DeferredDeleter") << "Destroying later: " << p << std::endl;
		scy::deleteLater(p);
	}
};


template <class C>
class DestroyMethodDeleter: public Deleter<C>
	// DestroyMethodDeleter calls the destroy() method 
	// on an object to begin the deletion sequence.
{
public:
	DestroyMethodDeleter(C* p = NULL) : 
		Deleter<C>(p, &DestroyMethodDeleter<C>::func)
	{
	}
				
	static void func(C* p)
	{
		//traceL("DestroyMethodDeleter") << "Destroying: " << p << std::endl;
		p->destroy();
	}
};


//
// Garbage Collector Inlines
//


template <class C>
void GarbageCollector::deleteLater(C* ptr)
	// Schedules a pointer for deferred deletion.
{ 
	//traceL("GarbageCollector", this) << "Scheduling: " << ptr << std::endl;
		
	Mutex::ScopedLock lock(_mutex);
	_pending.push_back(new GCDeleter<C>(ptr));
}


template <class C>
inline void deleteLater(C* ptr)
	// Convenience function for accessing GarbageCollector::deleteLater
{
	GarbageCollector::instance().deleteLater(ptr);
}


//
// Memory Object
//


class MemoryObject
	// MemoryObject is the base class for LibSourcey objects
	// which employ different memory management strategies.
{	
public:
	MemoryObject(AbstractDeleter* deleter = new DefaultDeleter<MemoryObject>()) : 
		deleter(deleter)
	{
		if (deleter->ptr == NULL)
			deleter->ptr = this;
	}

	virtual ~MemoryObject()
	{
	}

	virtual void freeMemory()
	{
		//traceL("MemoryObject", this) << "Freeing" << std::endl;
		deleter->invoke();
	}

protected:
	MemoryObject(const MemoryObject&) {};
	MemoryObject& operator = (const MemoryObject&) {};
	
	friend class DefaultDeleter<MemoryObject>;	

	std::unique_ptr<AbstractDeleter> deleter;
};


//
// Counted Object
//


class CountedObject: public MemoryObject
	// CountedObject is the base class for objects that  
	// employ reference counting based garbage collection.
	//
	// Reference-counted objects inhibit construction by
	// copying and assignment.
{
public:
	CountedObject(AbstractDeleter* deleter = new DefaultDeleter<CountedObject>()) :
		MemoryObject(deleter)
		// Creates the CountedObject.
		// The initial reference count is one.
	{
		count = 1;
	}
	
	void duplicate()
		// Increments the object's reference count.
	{
		atomic::increment(&count);
	}
		
	void release()
		// Decrements the object's reference count and
		// calls destroy() if the count reaches zero.
	{
		if (atomic::decrement(&count) == 0)
			MemoryObject::freeMemory();
	}
		
	int refCount() const
		// Returns the reference count.
	{
		return count;
	}

protected:
	virtual ~CountedObject()
		// Destroys the CountedObject.
		// The destructor should never be called directly.
	{
		assert(refCount() == 0);
	}

	CountedObject(const CountedObject&) {};
	CountedObject& operator = (const CountedObject&) {};
	
	friend class DefaultDeleter<CountedObject>;	
	friend class DeferredDeleter<CountedObject>;	
	
	volatile long count;
};


} // namespace scy


#endif // SOURCEY_Memory_H