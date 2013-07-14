#ifndef SOURCEY_Singleton_H
#define SOURCEY_Singleton_H


#include "Sourcey/Types.h"
#include "Sourcey/Logger.h"


namespace scy {


template <class S>
class Singleton
	/// This is a helper template class for managing
	/// singleton objects allocated on the heap.
	/// The class ensures proper deletion (including
	/// calling of the destructor) of singleton objects
	/// when the application that created them terminates.
{
public:
	Singleton():
		ptr(0)
		/// Creates the Singleton wrapper.
	{
		//traceL("Singleton", this) << "Create: " << ptr << std::endl;
	}
	
	~Singleton()
		/// Destroys the Singleton and the singleton
		/// object that it holds.
	{
		//traceL("Singleton", this) << "Destroy: " << ptr << std::endl;
		if (ptr)
			delete ptr;
	}
	
	S* get()
		/// Returns a pointer to the singleton object
		/// hold by the Singleton. The first call
		/// to get will create the singleton.
	{
		//traceL("Singleton", this) << "Get: " << ptr << std::endl;

		Mutex::ScopedLock lock(_m);
		if (!ptr) 
			ptr = new S;
		return ptr;
	}

	void destroy()
	{
		traceL("Singleton", this) << "Kill: " << ptr << std::endl;

		Mutex::ScopedLock lock(_m);
		if (ptr) {
			traceL("Singleton", this) << "Kill: Deleting: " << ptr << std::endl;
			delete ptr;
		}
		ptr = 0;

		traceL("Singleton", this) << "Kill: OK: " << ptr << std::endl;
	}
	
private:
	S* ptr;
	Mutex _m;
};


} // namespace scy


#endif // SOURCEY_Singleton_H
