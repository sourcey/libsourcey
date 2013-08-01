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
		ptr(0)
		// Creates the Singleton wrapper.
	{
	}
	
	~Singleton()
		// Destroys the Singleton wrapper and the 
		// managed singleton instance it holds.
	{
		if (ptr)
			delete ptr;
	}
	
	S* get()
		// Returns a pointer to the singleton object
		// hold by the Singleton. The first call
		// to get on a NULL singleton will instantiate
		// the singleton.
	{
		ScopedLock lock(_m);
		if (!ptr) 
			ptr = new S;
		return ptr;
	}

	void destroy()
		// Destroys the managed singleton instance.
	{
		ScopedLock lock(_m);
		if (ptr) {
			delete ptr;
		}
		ptr = 0;
	}
	
private:
	S* ptr;
	Mutex _m;
};


} // namespace scy


#endif // SOURCEY_Singleton_H
