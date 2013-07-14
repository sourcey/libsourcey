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
#include "Sourcey/UV/UVPP.h"
//#include <atomic>

#include "Sourcey/Singleton.h"


namespace scy {


class AbstractDeleter;
class GarbageCollector: public uv::Base
	/// Simple garbage collector for deferred pointer deletion.
{
public:	
	GarbageCollector();	
	~GarbageCollector();
		
	static GarbageCollector& instance();
		/// Returns the GarbageCollector singleton.
	
	template <class C>
	void deleteLater(C* ptr);
		/// Schedules a pointer for deferred deletion.

	static void shutdown();
		/// Shuts down the garbage collector and deleted 
		/// the singleton instance.

	void close();
		/// Closes the internal timer and frees all 
		/// scheduled pointers now.

protected:	
	void onTimer();
		
	UVEmptyStatusCallback(GarbageCollector, onTimer, uv_timer_t);
	
	mutable Mutex	_mutex;
	std::vector<AbstractDeleter*> _pending;
	std::vector<AbstractDeleter*> _ready;
};


// -------------------------------------------------------------------
//
class AbstractDeleter
	/// AbstractDeleter provides an interface for 
	/// deleting pointer memory in various ways.
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
	/// Deleter is the base template class for 
	/// deleting a type cast pointer.
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
		C* p = reinterpret_cast<C*>(ptr);
		ptr = NULL;
        func(p);
    }
};


template <class C>
class DefaultDeleter: public Deleter<C>
	/// DefaultDeleter calls the standard delete 
	/// operator to free pointer memory.
{
public:
	DefaultDeleter(C* p = NULL) : 
		Deleter<C>(p, &DefaultDeleter<C>::func)
	{
	}
				
	static void func(C* p)
	{
		traceL("DefaultDeleter") << "Destroying: " << p << std::endl;
		delete p;
	}
};


template <class C>
class GCDeleter: public Deleter<C>
	/// GCDeleter is used by the GarbageCollector
	/// to free managed pointer memory.
{
public:
	GCDeleter(C* p = NULL) : 
		Deleter<C>(p, &GCDeleter<C>::func)
	{
	}
				
	static void func(C* p)
	{
		traceL("GCDeleter") << "Destroying: " << p << std::endl;
		delete p;
	}
};


template <class C>
class DeferredDeleter: public Deleter<C>
	/// DeferredDeleter schedules a pointer for 
	/// deferred deletion by the GarbageCollector.
{
public:
	DeferredDeleter(C* p = NULL) : 
		Deleter<C>(p, &DeferredDeleter<C>::func)
	{
	}
				
	static void func(C* p)
	{
		traceL("DeferredDeleter") << "Destroying later: " << p << std::endl;
		scy::deleteLater(p);
	}
};


template <class C>
class DestroyMethodDeleter: public Deleter<C>
	/// DestroyMethodDeleter calls the destroy() method 
	/// on an object to begin the deletion sequence.
{
public:
	DestroyMethodDeleter(C* p = NULL) : 
		Deleter<C>(p, &DestroyMethodDeleter<C>::func)
	{
	}
				
	static void func(C* p)
	{
		traceL("DestroyMethodDeleter") << "Destroying: " << p << std::endl;
		p->destroy();
	}
};


// -------------------------------------------------------------------
//
template <class C>
void GarbageCollector::deleteLater(C* ptr)
	/// Schedules a pointer for deferred deletion.
{ 
	traceL("GarbageCollector", this) << "Scheduling: " << ptr << std::endl;
		
	Mutex::ScopedLock lock(_mutex);
	_pending.push_back(new GCDeleter<C>(ptr));
}


template <class C>
inline void deleteLater(C* ptr)
	/// Convenience function for accessing GarbageCollector::deleteLater
{
	GCDeleter<C> del(ptr);
	//GarbageCollector::instance().deleteLater(ptr);
}


// -------------------------------------------------------------------
//
class ManagedObject
	/// ManagedObject is the base class for LibSourcey objects
	/// which employ different memory management strategies.
{	
public:
	ManagedObject(AbstractDeleter* deleter = new DefaultDeleter<ManagedObject>()) : 
		deleter(deleter)
	{
		if (deleter->ptr == NULL)
			deleter->ptr = this;
	}

	virtual ~ManagedObject()
	{
		traceL("ManagedObject", this) << "Destroying" << std::endl;
	}

	virtual void freeMemory()
	{
		traceL("ManagedObject", this) << "Freeing" << std::endl;
		deleter->invoke();
		traceL("ManagedObject", this) << "Freeing: OK" << std::endl;
	}

protected:
	ManagedObject(const ManagedObject&) {};
	ManagedObject& operator = (const ManagedObject&) {};
	
	friend class DefaultDeleter<ManagedObject>;	

	std::unique_ptr<AbstractDeleter> deleter;
};


// -------------------------------------------------------------------
//
class CountedObject: public ManagedObject
	/// CountedObject is the base class for objects that employ 
	/// reference counting based garbage collection.
	///
	/// Reference-counted objects inhibit construction by
	/// copying and assignment.
	///
	/// TODO: Use atomic integer for MT goodness:
	/// http://www.jaggersoft.com/pubs/oload25.html
{
public:
	CountedObject(AbstractDeleter* deleter = new DefaultDeleter<CountedObject>()) :
		ManagedObject(deleter)
		/// Creates the CountedObject.
		/// The initial reference count is one.
	{
		count = 1;
	};
	
	void duplicate()
		/// Increments the object's reference count.
	{
		++count;
	}
		
	void release()
		/// Decrements the object's reference count and
		/// calls destroy() if the count reaches zero.
	{
		--count;

		if (count == 0)
			ManagedObject::freeMemory();
	}
		
	int refCount() const
		/// Returns the reference count.
	{
		return count;
	}

protected:
	virtual ~CountedObject()
		/// Destroys the CountedObject.
		/// The destructor should never be called directly.
	{
		traceL("CountedObject", this) << "Destroying" << std::endl;
		assert(refCount() == 0);
	}

	CountedObject(const CountedObject&) {};
	CountedObject& operator = (const CountedObject&) {};
	
	friend class DefaultDeleter<CountedObject>;	
	friend class DeferredDeleter<CountedObject>;	
	
	int /* std::atomic<int> */ count; // TODO: sync
};


} // namespace scy


#endif // SOURCEY_Memory_H




/*
class IDeletable
	/// Abstract storage container for deleting
	/// a type cast pointer.
{
public:
	virtual ~IDeletable() 
	{
	}
};


template <class C>
class Deletable: public IDeletable
	/// Storage container implementation for deleting
	/// a type cast pointer.
{
public:
	C* ptr;

	Deletable(C* ptr) : ptr(ptr)
	{
	}

	virtual ~Deletable()
	{
		delete ptr;
	}
};
*/

	//{ 
	//	traceL("GarbageCollector", this) << "Scheduling: " << ptr << std::endl;
	//	
	//	Mutex::ScopedLock lock(_mutex);
	//	_pending.push_back(new Deletable<C>(ptr));
	//}
	
	/*
class ManagedObject;
	
	void deleteLater(ManagedObject* ptr);
class AbstractDeleter;
*/
	/*
	virtual void free() 
	{
		delete this; 
	}


	Deleter(Func f = &deleter::Default< C >::func) : 
		AbstractDeleter(NULL), func(f)
	{
	}
	*/
/*
// -------------------------------------------------------------------
//
class StandardObject	
	/// StandardObject is a memory base for objects which employ
	/// standard memory menagement ie. no special reference counting  
	/// or garbage colleaction.
{
	virtual ~StandardObject()
	{
	}
};


// -------------------------------------------------------------------
//
class MemBase	
{
};
*/


/*    void invoke(C* p)
    {
        func(p);
    }


template< class C >
struct Deleter
{
    typedef void (*Func)(C*);
 
    static void invoke(Func f, C const* p)
    {
        f(const_cast<C*>(p));
    }
};
	*/
/*


namespace deleter {
	/// Deleter functors which freeing pointer memory in various ways.
	///
	/// These methods are useful for passing to classes or templates
	/// that are oblivious about the memory handling method of their
	/// managed pointers.
 
    template< class C >
    struct Default
    {
		static void func(C* p)
		{
			delete p;
		}
    };

    template< class C >
    struct Destroy
    {
		static void func(C* p)
		{
			p->destroy();
		}
    };

    template< class C >
    struct Deferred
    {
		static void func(C* p)
		{
			scy::deleteLater(C* ptr);
		}
    };

    template< class C >
    struct ViaAutoPtr
    {		
		static void func(C* p)
		{
			std::auto_ptr<C>(p + 0);
		}
    };

    template< class C >
    struct None
    {
		static void func(C* p)
		{
		}
    };

}  // namespace deleter

*/
/*
*/

	/*
	ManagedObject(AbstractDeleter* deleter = new DeferredDeleter<ManagedObject>()) :
		CountedObject(deleter)
	{
	};
	*/

	/*
	virtual void destroy()
		/// Destroys the CountedObject when the count
		/// reaches zero.
		///
		/// This method can be overridden to implement
		/// destruction logic or garbage collection.
	{
		assert(refCount() == 0);
		delete this; 
	}
		Deleter<CountedObject>::invoke(&deleter::Default<CountedObject>::func, this);
	*/


	/* //<deleter::Deferred< C >>
		/// Creates the CountedObject.
		/// The initial reference count is one.

	virtual void destroy()
		/// Destroys the ManagedObject.
	{
		deleteLater<ManagedObject>(this); 
	}
	*/


//typedef ManagedObject

/*

	//Deleter::func(this);
	//typedef typename Deleter< Type >::Func  DeleterFunc;	
    //typedef void (*DeleteFunc)(C*);
	//DeleteFunc fn; 
//template< class Deleter = DefaultDeleter< CountedObject > >

template< class C = ManagedObject >
*/


	
/*
template <class C>
class AsyncDeleter 
	/// Simple garbage collector for delayed deletion of pointers.
{
public:
	C* ptr;

	AsyncDeleter(C* ptr) : ptr(ptr)
	{
		traceL("AsyncDeleter", this) << "Queueing: " << ptr << std::endl;

		// TODO: Use uv_async_t, or uv_timer_t?
		uv_idle_t* idler = new uv_idle_t;
		idler->data = this;
		uv_idle_init(uv_default_loop(), idler);
		uv_idle_start(idler, AsyncDeleter::onDestroyObject);
	}

	~AsyncDeleter()
	{
		traceL("AsyncDeleter", this) << "Deleting: " << ptr << std::endl;
		delete ptr;
	}

	static void onDestroyObject(uv_idle_t* idler, int status)
	{
		AsyncDeleter* self = reinterpret_cast<AsyncDeleter*>(idler->data);
		traceL("AsyncDeleter", self) << "On destroy" << std::endl;
		assert(status == 0);
		//uv_idle_stop(handle);		
		uv_close((uv_handle_t*)idler, afterClose);
		delete self;
	}

	static void afterClose(uv_handle_t* handle) 
	{	
		delete handle;
	}
};
*/