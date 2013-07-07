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


#include "Sourcey/Timer.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Types.h"
#include <vector>
//#include <atomic>


namespace scy {
	
	
template <class C>
class Deleter 
	/// Simple garbage collector for delayed pointer deletion.
{
public:
	C* ptr;

	Deleter(C* ptr) : ptr(ptr)
	{
		traceL("Deleter", this) << "Queueing: " << ptr << std::endl;
		uv_idle_t* idler = new uv_idle_t;
		idler->data = this;
		uv_idle_init(uv_default_loop(), idler);
		uv_idle_start(idler, Deleter::onNextIteration);
	}

	~Deleter()
	{
		traceL("Deleter", this) << "Deleting: " << ptr << std::endl;
		delete ptr;
	}

	static void onNextIteration(uv_idle_t* handle, int status)
	{
		traceL("Deleter") << "On next iteration: " << status << std::endl;
		assert(status == 0);
		traceL("Deleter") << "On next iteration: Stopping" << std::endl;
		uv_idle_stop(handle);
		delete reinterpret_cast<Deleter*>(handle->data);
		traceL("Deleter") << "On next iteration: Stopping: OK" << std::endl;
		
		//delete handle; // FIXME: Causing crash???
	}
};


template <class C>
inline Deleter<C>* deleteLater(C* ptr)
	/// Inline helper for async garbage deletion.
{
	return new Deleter<C>(ptr);
}


// -------------------------------------------------------------------
//
class CountedObject
	/// A base class for objects that employ reference
	/// counting based garbage collection.
	///
	/// Reference-counted objects inhibit construction
	/// by copying and assignment.
	///
	/// TODO: Use atomic integer for MT goodness:
	/// http://www.jaggersoft.com/pubs/oload25.html
{
public:
	CountedObject() 
		/// Creates the CountedObject.
		/// The initial reference count is one.
	{
		count = 1;
	};
	
	void duplicate()
		/// Increments the object's reference count.
	{
		++count;
		traceL("CountedObject", this) << "Duplicated: " << count << std::endl;
	}
		
	void release()
		/// Decrements the object's reference count
		/// and deletes the object if the count
		/// reaches zero.
	{
		--count;
		traceL("CountedObject", this) << "Released: " << count << std::endl;
			
		if (count == 0)
			destroy();
	}
		
	int refCount() const
		/// Returns the reference count.
	{
		return count;
	}

protected:
	virtual void destroy()
	{
		traceL("CountedObject", this) << "Destroy: " << count << std::endl;
		// Destuctor logic may be overridden.
		// Could also use deleteLater<>
		delete this; 
	}

	virtual ~CountedObject()
		/// Destroys the CountedObject.
	{
		traceL("CountedObject", this) << "Destroying: " << refCount() << std::endl;
		//assert(refCount() == 0);
	}

protected:
	CountedObject(const CountedObject&) {};
	CountedObject& operator = (const CountedObject&) {};
	
	//std::atomic<int> count;
	int count; // TODO: sync
};


template <class C>
class Handle	
	/// Handle is a reference counted pointer similar to
	/// std::shared_ptr but without the bloat.
	///
	/// The template argument must implement duplicate()
	/// and release() methods. See CountedObject.
{
public:
	Handle() : _ptr(0)
	{
	}

	Handle(C* ptr) : _ptr(ptr)
	{
	}

	Handle(C* ptr, bool shared) : _ptr(ptr)
	{
		if (shared && _ptr) _ptr->duplicate();
	}

	Handle(const Handle& ptr) : _ptr(ptr._ptr)
	{
		if (_ptr) _ptr->duplicate();
	}

	~Handle()
	{
		if (_ptr) _ptr->release();
	}
	
	Handle& assign(C* ptr)
	{
		if (_ptr != ptr)
		{
			if (_ptr) _ptr->release();
			_ptr = ptr;
		}
		return *this;
	}

	Handle& assign(C* ptr, bool shared)
	{
		if (_ptr != ptr)
		{
			if (_ptr) _ptr->release();
			_ptr = ptr;
			if (shared && _ptr) _ptr->duplicate();
		}
		return *this;
	}
	
	Handle& assign(const Handle& ptr)
	{
		if (&ptr != this)
		{
			if (_ptr) _ptr->release();
			_ptr = ptr._ptr;
			if (_ptr) _ptr->duplicate();
		}
		return *this;
	}

	Handle& operator = (C* ptr)
	{
		return assign(ptr);
	}

	Handle& operator = (const Handle& ptr)
	{
		return assign(ptr);
	}

	C* operator -> ()
	{
		if (_ptr)
			return _ptr;
		else
			throw Poco::NullPointerException();
	}

	const C* operator -> () const
	{
		if (_ptr)
			return _ptr;
		else
			throw Poco::NullPointerException();
	}

	C& operator * ()
	{
		if (_ptr)
			return *_ptr;
		else
			throw Poco::NullPointerException();
	}

	const C& operator * () const
	{
		if (_ptr)
			return *_ptr;
		else
			throw Poco::NullPointerException();
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
		return _ptr == 0;
	}

	bool isNull() const
	{
		return _ptr == 0;
	}
	
	C* duplicate()
	{
		if (_ptr) _ptr->duplicate();
		return _ptr;
	}

	bool operator == (const Handle& ptr) const
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

	bool operator != (const Handle& ptr) const
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

	bool operator < (const Handle& ptr) const
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

	bool operator <= (const Handle& ptr) const
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

	bool operator > (const Handle& ptr) const
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

	bool operator >= (const Handle& ptr) const
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