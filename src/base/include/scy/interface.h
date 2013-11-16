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


#ifndef SCY_Interfaces_H
#define SCY_Interfaces_H


#include "scy/types.h"
#include <stdexcept>
#include <atomic>
#include <functional>
#include <memory>


namespace scy {

	
struct LogStream;
	// Forward the LogStream for Logger.h

	
namespace basic { // interface pieces

			
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


#endif // SCY_Interfaces_H


 //Callback
	 //basic::Callable target, void* context
	//Async(async::Runnable& target);	
	//virtual void start(async::Runnable& target);
		// Starts the thread with the given target.
		//
		// The Runnable object must remain valid for the  
		// entire lifetime of the async object.