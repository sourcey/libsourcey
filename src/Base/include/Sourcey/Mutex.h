#ifndef SOURCEY_Mutex_H
#define SOURCEY_Mutex_H


#include "Sourcey/UV/UVPP.h"


namespace scy {


template <class M>
class ScopedLock
	/// ScopedLock is a scoped mutex locker that is 
	/// initialized on the stack.
	/// The referenced mutex is locked in the constructor, 
	/// and unlocked in the destructor.
{
public:
	explicit ScopedLock(M& mutex) : _mutex(mutex)
	{
		_mutex.lock();
	}
	
	~ScopedLock()
	{
		_mutex.unlock();
	}

private:
	M& _mutex;

	ScopedLock();
	ScopedLock(const ScopedLock&);
	ScopedLock& operator = (const ScopedLock&);
};


class Mutex
	/// A Mutex (mutual exclusion) is a synchronization 
	/// mechanism used to control access to a shared resource
	/// in a concurrent (multithreaded) scenario.
	/// Mutexes are recursive, that is, the same mutex can be 
	/// locked multiple times by the same thread (but, of course,
	/// not by other threads).
	/// Using the ScopedLock class is the preferred way to automatically
	/// lock and unlock a mutex.
{
public:
	typedef ScopedLock<Mutex> ScopedLock;
	
	Mutex();
		/// Creates the Mutex.
		
	~Mutex();
		/// Destroys the Mutex.

	void lock();
		/// Locks the mutex. Blocks if the mutex
		/// is held by another thread.

	bool tryLock();
		/// Tries to lock the mutex. Returns false immediately
		/// if the mutex is already held by another thread.
		/// Returns true if the mutex was successfully locked.

	void unlock();
		/// Unlocks the mutex so that it can be acquired by
		/// other threads.
	
private:
	Mutex(const Mutex&);
	Mutex& operator = (const Mutex&);

	uv_mutex_t _mx;
};


// TODO: RwLock


} // namespace scy


#endif // SOURCEY_Mutex_H
