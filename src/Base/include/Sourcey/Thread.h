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
#include "Sourcey/Interfaces.h"


namespace scy {


class Thread
	// This class implements a platform-independent
	// wrapper to an operating system thread.
	//
	// Every Thread object gets a unique (within
	// its process) numeric thread ID.
	// Furthermore, a thread can be assigned a name.
	// The name of a thread can be changed at any time.
{
public:	
	Thread();		
	Thread(const std::string& name);		
	~Thread();

	void start(abstract::Runnable& target);
		// Starts the thread with the given target.
		//
		// The given Runnable object must be valid
		// for the entire lifetime of the thread.

	void start(Callable target, void* opaque = 0);
		// Starts the thread with the given target and 
		// opaque pointer.

	void join();
		// Waits until the thread exits.

	unsigned long id() const;
		// Returns the unique thread ID of the thread.

	std::string name() const;
		// Returns the name of the thread.

	bool running() const;
		// Returns true if the thread is currently running.
	
	unsigned long currentID() const;
 		// Returns the native thread ID of the currently active thread.
		
protected:
	Thread(const Thread&);
	Thread& operator = (const Thread&);

	static void runAsync(void* arg);
	
	bool _running;
	std::string _name;
	uv_thread_t _handle;
	unsigned long _id;
	mutable Mutex _mutex;
};


//
// Async Startable
//


template <class TStartable>
class AsyncStartable: public TStartable
	// This class is an invisible wrapper around a TStartable instance
	// which provides asynchronous access to the TStartable start() and
	// stop() methods. TStartable is an instance of abstract::Startable.
{
public:
	AsyncStartable() {};
	virtual ~AsyncStartable() {};

	static void runAsync(void* arg) {
		try {
			// Start the TStartable base once only, 
			// so this call must block.
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
