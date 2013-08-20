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


#ifndef SOURCEY_Interfaces_H
#define SOURCEY_Interfaces_H


#include "Sourcey/Types.h"
#include <atomic>


namespace scy {

	
struct LogStream;
	// Forward the LogStream for Logger.h

	
namespace basic { // interface pieces

		
class Runnable
	// A generic interface for classes that 
	// can be run and cancelled.
{
	std::atomic<bool> flag;

public:
	Runnable() : flag(false) {}
	virtual ~Runnable() {}
	
	virtual void run() = 0;
		// The run method will be called repeatedly by the  
		// managing context until this method return false.
	
	virtual void cancel()
		// Cancels the current task.
	{
		flag.store(true, std::memory_order_release);
	}
	
	virtual bool cancelled() const
		// True when the task has been cancelled.
		// The run() method should return ASAP.
	{
		bool s = flag.load(std::memory_order_relaxed);
		if (s) std::atomic_thread_fence(std::memory_order_acquire);
		return s;
	};
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
	
	
class Decoder 
{	
public:
	Decoder() {}
	virtual ~Decoder() {}
	virtual std::size_t decode(const char* inbuf, std::size_t nread, char* outbuf) = 0;
	virtual std::size_t finalize(char* /* outbuf */) { return 0; }
};
	

class Encoder 
{	
public:
	Encoder() {}
	virtual ~Encoder() {}
	virtual std::size_t encode(const char* inbuf, std::size_t nread, char* outbuf) = 0;
	virtual std::size_t finalize(char* /* outbuf */) { return 0; }
};


class Polymorphic
	// A base module class for C++ callback polymorphism.
{
public:
	virtual ~Polymorphic() {};
		
	template<class T>
	bool is() {
		return dynamic_cast<T*>(this) != nullptr;
	};

	template<class T>
	T* as(bool whiny = false) {
		T* self = dynamic_cast<T*>(this);
		if (self == nullptr && whiny)
			throw std::runtime_error("Polymorphic cast failed");
		return self;
	};

	scy::LogStream& log(const char* level = "debug") const; // depreciated

	virtual const char* className() const = 0;
};


typedef Polymorphic Module;
	// The LibSourcey base module type.
	// May become a class type in the future.
	// depreciated
	 

} } // namespace scy::basic


#endif // SOURCEY_Interfaces_H
