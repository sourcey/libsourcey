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


#ifndef SOURCEY_Thread_H
#define SOURCEY_Thread_H


#include "Sourcey/UV/UVPP.h"
#include "Sourcey/Mutex.h"
#include "Sourcey/Interface.h"

#include <functional>


namespace scy {
	

class Thread
	/// This class implements a platform-independent
	/// wrapper around an operating system thread.
{
public:	
	Thread();
	Thread(basic::Runnable& target);	
	Thread(std::function<void()> target);	
	Thread(std::function<void(void*)> target, void* opaque = nullptr);
	~Thread();
	
	void start(basic::Runnable& target);
		// Starts the thread with the given target.
		//
		// The given Runnable object must be valid for the entire 
		// lifetime of the thread.
	
	void start(std::function<void()> target);	
	void start(std::function<void(void*)> target, void* opaque = nullptr);
		// Starts the thread with the given target.
	
	void startC(basic::Callable target, void* opaque = nullptr);
		// Starts the thread with the given target and opaque pointer.
	
	void join();
		// Waits until the thread exits.
	
	bool running() const;
		// Returns true if the thread is running.
	
	unsigned long id() const;
		// Returns the native thread ID.
	
	static unsigned long currentID();
 		// Returns the native thread ID of the current thread.
		
protected:
	Thread(const Thread&);
	Thread& operator = (const Thread&);
	
	static void runAsync(void* arg, bool hasArg);
	
	bool _running;
	unsigned long _id;
	uv_thread_t _handle;
	mutable Mutex _mutex;
};


//
// Cancellation Flag
//

	
class CancellationFlag 
	/// A concurrent flag which can be    
	/// used to request task cancellation.
{
	std::atomic<bool> state;
	
	// Non-copyable and non-movable
	CancellationFlag(const CancellationFlag&); // = delete;
	CancellationFlag(CancellationFlag&&); // = delete;
	CancellationFlag& operator=(const CancellationFlag&); // = delete;
	CancellationFlag& operator=(CancellationFlag&&); // = delete;

public:
	CancellationFlag() : state(false) {};

	bool cancelled() const
	{
		bool s = state.load(std::memory_order_relaxed);
		if (s)
			std::atomic_thread_fence(std::memory_order_acquire);
		return s;
	}

	void cancel()
	{
		state.store(true, std::memory_order_release);
	}
};


//
// Async Startable
//


template <class TStartable>
class AsyncStartable: public TStartable
	// This class is an invisible wrapper around a TStartable instance
	// which provides asynchronous access to the TStartable start() and
	// stop() methods. TStartable is an instance of basic::Startable.
{
public:
	AsyncStartable() {};
	virtual ~AsyncStartable() {};

	static void runAsync(void* arg) {
		try {
			// Call the blocking start() function once only
			static_cast<TStartable*>(arg)->start();
		}
		catch (std::exception& exc) {
			errorL("AsyncStartable") << exc.what() << std::endl;
#ifdef _DEBUG
			throw exc;
#endif
		}
	}

	virtual bool start() 
	{
		_thread.start(*this);
		return true;
	}
	
	virtual void stop()
	{
		TStartable::stop();
		_thread.join();
	}

protected:
	Thread _thread;
};


} // namespace scy


#endif
