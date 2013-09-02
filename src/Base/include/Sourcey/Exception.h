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


#ifndef SOURCEY_Exception_H
#define SOURCEY_Exception_H


#include "Sourcey/Base.h"
#include <string>
#include <exception>


namespace scy { 


class Exception: public std::exception
	// This class extends the standard library exception object
	// with features relevent to the LibSourcey debug context.
	//
	// TDOD: Extend with file and linenumbers 
{
public:
	Exception(int code = 0);
	Exception(const std::string& msg, int code = 0);
	Exception(const std::string& msg, const std::string& ext, int code = 0);
		
	~Exception() throw();

	virtual const char* name() const throw();
		// Returns the exception type name.
		
	virtual const char* what() const throw();
		// Returns the message text char pointer.
			
	std::string message() const;
		// Returns the message text.
			
	int code() const;
		// Returns the exception code if defined.

	void setMessage(const std::string& msg);
		// Sets the message for the exception.

	void entendMessage(const std::string& ext);
		// Appends a message fragment to the exception.
	
	virtual void rethrow() const;
		// (Re)Throws the exception.
	
    friend std::ostream& operator << (std::ostream& stream, const Exception& exc) 
	{
		stream << exc.what();
		return stream;
    }

protected:		
	std::string _msg;
	int _code;
};


//
// Macro for declaring custom exception types
#define DECLARE_EXCEPTION(Class, Base, Name)										\
																					\
	class Class: public Base														\
	{																				\
	public:																			\
	Class::Class(int code = 0): Base(code = 0)										\
	{																				\
	}																				\
	Class::Class(const std::string& msg, int code = 0) :							\
		Base(msg, code)																\
	{																				\
	}																				\
	Class::Class(const std::string& msg, const std::string& ext, int code = 0) :	\
		Base(msg, ext, code)														\
	{																				\
	}																				\
	Class::~Class() throw()															\
	{																				\
	}																				\
	const char* Class::name() const throw()											\
	{																				\
		return Name;																\
	}																				\
	void Class::rethrow() const														\
	{																				\
		throw *this;																\
	}                                                                               \
};                                                                                  \


// All exceptions may be caught as a std::exception type.
// Rather than implement lots of exception types only basic
// exception types are used to hint at the nature of the error.
// More exceptions can be defined an the implementation required.
DECLARE_EXCEPTION(SystemException, Exception, "System error")
DECLARE_EXCEPTION(SyntaxException, Exception, "Syntax error")
DECLARE_EXCEPTION(NotFoundException, Exception, "Not found")
DECLARE_EXCEPTION(ExistsException, Exception, "Already exists")
DECLARE_EXCEPTION(ArgumentException, Exception, "Invalid argument")
DECLARE_EXCEPTION(IOException, Exception, "IO error")
DECLARE_EXCEPTION(FileException, IOException, "File error")


//
// Error type
//


struct Error 
{
	int syserr; // System error code
	int uverr;  // Libuv error code
	std::string message; // Error message (set by application)

	Error()
	{
		reset();
	}

	Error(const std::string& msg)
	{
		reset();
		message = msg;
	}

	Error(const char* msg)
	{
		//assert(msg);
		reset();
		message = msg;
	}

	bool any() const
	{
		return uverr != 0 || syserr != 0 || !message.empty();
	}

	void reset() 
	{
		uverr = 0;
		syserr = 0;
		message.clear();
	}
};


} // namespace scy


#endif // SOURCEY_Exception_H