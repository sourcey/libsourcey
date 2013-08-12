#ifndef SOURCEY_Singleton_H
#define SOURCEY_Singleton_H


#include "Sourcey/Types.h"


namespace scy {


template <class S>
class Singleton
	// This is a helper template class for managing
	// singleton objects allocated on the heap.
{
public:
	Singleton() :
		_ptr(0)
		// Creates the Singleton wrapper.
	{
	}
	
	~Singleton()
		// Destroys the Singleton wrapper and the 
		// managed singleton instance it holds.
	{
		if (_ptr)
			delete _ptr;
	}
	
	S* get()
		// Returns a pointer to the singleton object
		// hold by the Singleton. The first call
		// to get on a NULL singleton will instantiate
		// the singleton.
	{
		Mutex::ScopedLock lock(_m);
		if (!_ptr) 
			_ptr = new S;
		return _ptr;
	}
	
	S* swap(S* newPtr)
		// Swaps the old pointer with the new one and 
		// returns the old instance.
	{
		Mutex::ScopedLock lock(_m);
		S* oldPtr = _ptr;
		_ptr = newPtr;
		return oldPtr;
	}

	void destroy()
		// Destroys the managed singleton instance.
	{
		Mutex::ScopedLock lock(_m);
		if (_ptr)
			delete _ptr;
		_ptr = nil;
	}
	
private:
	S* _ptr;
	Mutex _m;
};


} // namespace scy


#endif // SOURCEY_Singleton_H
