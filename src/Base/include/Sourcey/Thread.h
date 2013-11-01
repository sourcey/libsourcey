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
#include "Sourcey/Async.h"


namespace scy {
	

class Thread: public async::Runner
	/// This class implements a platform-independent
	/// wrapper around an operating system thread.
{
public:	
	typedef std::shared_ptr<Thread> ptr;

	Thread();
	Thread(async::Runnable& target);	
	Thread(std::function<void()> target);	
	Thread(std::function<void(void*)> target, void* arg);
	virtual ~Thread();
	
	void join();
		// Waits until the thread exits.
	 
	unsigned long id() const;
		// Returns the native thread ID.
	
	static unsigned long currentID();
 		// Returns the native thread ID of the current thread.

protected:
	Thread(const Thread&);
	Thread& operator = (const Thread&);
	
	virtual bool async() const;
	virtual void startAsync(); 

	uv_thread_t _handle;
};


//
// Runner Startable
//


template <class TStartable>
class AsyncStartable: public TStartable
	// Depreciated: This class is an invisible wrapper around a TStartable instance,
	// which provides asynchronous access to the TStartable start() and
	// stop() methods. TStartable is an instance of async::Startable.
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
			// errorL("AsyncStartable") << exc.what() << std::endl;
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



	
		// Starts the thread with the given target.
		// The given Runnable object must be valid for the entire 
		// lifetime of the thread.
	
	
	/*
	
	//bool _started;
	//bool _running;
	
	//bool started() const;
		// Returns true if the thread has been started.
	
	//bool running() const;
		// Returns true if the thread is running.
	//virtual void reset();
	virtual void start(async::Runnable& target);
		// Starts the thread with the given target.
		//
		// The given Runnable object must be valid for the entire 
		// lifetime of the thread.
	
	virtual void start(std::function<void()> target);	
	virtual void start(std::function<void(void*)> target, void* arg);
		// Starts the thread with the given target.
		*/
	/*
	virtual void startAsync(std::function<void(void*)> target, void* arg)
	{
		int err = uv_thread_create(&_handle, target.target(), arg);
		if (err < 0) throw runtime_error("System error: Cannot initialize thread");	
	}
	*/
	

		// Starts the thread with the given target.
		//
		// The given Runnable object must be valid for the entire 
		// lifetime of the thread.
	
		// Starts the thread with the given target.

	//async::Runner(async::Runnable& target);
	//async::Runner(std::function<void()> target);	
	//async::Runner(std::function<void(void*)> target, void* arg);
	/*
class async::Runner
{
public:	
	async::Runner() {};
	virtual ~async::Runner() {};
	virtual void startAsync(std::function<void(void*)> target, void* arg) = 0;
		
protected:
	async::Runner(const async::Runner&);
	async::Runner& operator = (const async::Runner&);
};
*/


/*
class Thread: public async::Runner
	/// This class implements a platform-independent
	/// wrapper around an operating system thread.
{
public:	
	Thread() {};
	virtual ~Thread() {};

	//Thread(async::Runnable& target);	
	//Thread(std::function<void()> target);	
	//Thread(std::function<void(void*)> target, void* arg)
	//{
		//assert(!_started);
		//_started = true;
	
		//context.self = this;
		//context.target1 = target;	
		//context.arg = arg;
		//startAsync([](void* arg) {
		//}, arg);
	//}

	//virtual ~Thread();
	
	//void join();
		// Waits until the thread exits.
	
	//unsigned long id() const;
		// Returns the native thread ID.
	
	//static unsigned long currentID();
 		// Returns the native thread ID of the current thread.
		
protected:		
	virtual void startAsync(std::function<void(void*)> target, void* arg)
	{
		int err = uv_thread_create(&_handle, target.target, arg);
		if (err < 0) throw runtime_error("System error: Cannot initialize thread");	
	}

	unsigned long _id;
	uv_thread_t _handle;
};
*/
	
	//void startAsync(basic::Callable target, void* arg);
		// Starts the thread with the given target and arg pointer.
	//static void runAsync(void* arg, bool hasArg);
	
	//bool _started;
	//bool _running;
	//mutable Mutex _mutex;
	//bool started() const;
		// Returns true if the thread has been started.
	
	//bool running() const;
		// Returns true if the thread is running.
	//void start(async::Runnable& target);
		// Starts the thread with the given target.
		//
		// The given Runnable object must be valid for the entire 
		// lifetime of the thread.
	
	//void start(std::function<void()> target);	
	//void start(std::function<void(void*)> target, void* arg);
		// Starts the thread with the given target.