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


#ifndef SOURCEY_Async_H
#define SOURCEY_Async_H


#include "Sourcey/Types.h"
#include <stdexcept>
#include <atomic>
#include <functional>
#include <memory>


namespace scy {
namespace async {

		
class Runnable
	// A generic interface for classes that 
	// can be run and cancelled.
{
	std::atomic<bool> exit;

public:
	Runnable() : exit(false) {}
	virtual ~Runnable() {}
	
	virtual void run() = 0;
		// The run method will be called by the async context.
	
	virtual void cancel(bool flag = true)
		// Cancel the current task.
		// The run() method should return ASAP.
	{
		exit.store(flag, std::memory_order_release);
	}
	
	virtual bool cancelled() const
		// True when the task has been cancelled.
	{
		bool s = exit.load(std::memory_order_relaxed);
		if (s) std::atomic_thread_fence(std::memory_order_acquire);
		return s;
	};
};


//
// Runner Interface
//

	
class Runner
	/// This class is a virtual interface for implementing 
	/// asynchronous objects such as threads and futures.
{
public:	
	typedef std::shared_ptr<Runner> ptr;

	Runner();
	virtual ~Runner();
	
	virtual void start(async::Runnable& target);
	virtual void start(std::function<void()> target);	
	virtual void start(std::function<void(void*)> target, void* arg);
		// Starts the thread with the given target.
		// TODO: Use veradic templates when we have better windows support (vs2013).
	
	bool started() const;
		// Returns true if the async context has been started.
	
	bool running() const;
		// Returns true if the async context is currently running.
	
	void cancel();
		// Cancels the async context.
	
	bool cancelled() const;
		// True when the task has been cancelled.
		// It is up to the implementation to return at the
		// earliest possible time.
	
	virtual bool synced() const = 0;
		// Returns true if the implementation belongs to an event loop.
		
protected:
	Runner(const Runner&);
	Runner& operator = (const Runner&);
	
	struct Context
		// The context which we send to the thread context.
		// This allows us to garecefully handle late callbacks
		// and avoid the need for deferred destruction of Runner objects.
		// Implementation note: Only POD and atomic members can be safely
		// accessed from outside the Runner context.
	{
		typedef std::shared_ptr<Context> ptr;

		// Non thread-safe members
		// Should not be accessed once the Runner is started
		std::function<void()> target;
		std::function<void(void*)> target1;	
		void* arg;
		void* opaque; // unmanaged

		// Thread-safe POD members
		// May be accessed at any time
		bool started;
		bool running;
		std::atomic<bool> exit;
	
		void cancel();
			// Cancels the async context.
	
		bool cancelled() const;
			// True when the task has been cancelled.
			// It is up to the implementation to return at the
			// earliest possible time.

		void reset()
			// The implementation is responsible for resetting
			// the context if it is to be reused.
		{
			arg = nullptr;
			target = nullptr;
			target1 = nullptr;
			started = false;
			running = false;
			exit = false;
		}

		Context() { reset(); }
	};
	
	Context::ptr pContext;

	virtual void startAsync() = 0;
		// Start the context from the control thread.

	static void runAsync(Context::ptr context);
		// Run the context from the async thread.
};


//
// Concurrent Flag
//

	
class Flag 
	/// A concurrent flag which can be    
	/// used to request task cancellation.
{
	std::atomic<bool> state;
	
	// Non-copyable and non-movable
	Flag(const Flag&); // = delete;
	Flag(Flag&&); // = delete;
	Flag& operator=(const Flag&); // = delete;
	Flag& operator=(Flag&&); // = delete;

public:
	Flag() : state(false) {};

	bool get() const
	{
		bool s = state.load(std::memory_order_relaxed);
		if (s) std::atomic_thread_fence(std::memory_order_acquire);
		return s;
	}

	void set(bool flag)
	{
		state.store(flag, std::memory_order_release);
	}
};

	
typedef void (*Callable)(void*);
	// For C client data callbacks.


class Startable
	// A generic interface for a classes
	// that can be started and stopped.
{
public:
	virtual void start() = 0;
	virtual void stop() = 0;
};

		
class Sendable
	// A generic interface for classes
	// that can be sent and cancelled.
{
public:
	virtual bool send() = 0;
	virtual void cancel() {};
};
	 

} } // namespace scy::async


#endif // SOURCEY_Async_H


		//std::function<void()> onexit;

	//virtual void onexit(std::function<void()> target);	
		// Specify a function to be called when the async context exits.