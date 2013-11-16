#ifndef SCY_Mutex_H
#define SCY_Mutex_H


#include "scy/uv/uvpp.h"


namespace scy {


template <class T>
class ScopedLock
	// ScopedLock simplifies thread synchronization 
	// with a Mutex or similar lockable object.
	// The given Mutex is locked in the constructor,
	// and unlocked it in the destructor.
	// T can be any class with lock() and unlock() functions.
{
public:
	explicit ScopedLock(T& m) : _m(m)
	{
		_m.lock();
	}
	
	~ScopedLock()
	{
		_m.unlock();
	}

private:
	ScopedLock();
	ScopedLock(const ScopedLock&);
	ScopedLock& operator = (const ScopedLock&);

	T& _m;
};


class Mutex
	// This class is a wrapper around uv_mutex_t.
	//
	// A Mutex (mutual exclusion) is a synchronization mechanism
	// used to control access to a shared resource in a concurrent
	// (multithreaded) scenario.
	//
	// The ScopedLock class is usually used to obtain a Mutex lock, 
	// since it makes locking exception-safe.
{
public:
	typedef scy::ScopedLock<Mutex> ScopedLock;

	Mutex();
	~Mutex();

	void lock();
		// Locks the mutex.
		// Blocks if the mutex is held by another thread.

	bool tryLock();
		// Tries to lock the mutex. Returns false if the 
		// mutex is already held by another thread.
		// Returns true if the mutex was successfully locked.

	void unlock();
		// Unlocks the mutex so that it can be acquired by
		// other threads.
	
private:
	Mutex(const Mutex&);
	Mutex& operator = (const Mutex&);

	uv_mutex_t _mx;
};


// TODO: RwLock
// TODO: Condition


} // namespace scy


#endif // SCY_Mutex_H
