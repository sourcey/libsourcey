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


#ifndef SOURCEY_Types_H
#define SOURCEY_Types_H


#include "Sourcey/Base.h"
#include "Poco/Foundation.h"
#include "Poco/Exception.h"
#include "Poco/Mutex.h"

#include <vector>
#include <map>


//
/// Cross platform normalization
//
#ifdef WIN32
#define strncasecmp strnicmp
#define strcasecmp stricmp
#endif 


namespace scy {

	
//
// Reduce namespace pollution by forwarding some 
// extensively used basic Poco types to our namespace.
//
using Poco::Exception;
using Poco::Mutex;

using Poco::Int8;
using Poco::UInt8;
using Poco::Int16;
using Poco::UInt16;
using Poco::Int32;
using Poco::UInt32;
using Poco::Int32;
using Poco::UInt32;
using Poco::Int64;
using Poco::UInt64;


typedef std::vector<std::string>			StringVec;
typedef std::map<std::string, std::string>	StringMap;


struct Error 
	/// Simple error type 
{
	std::string message;
		/// Error message (set by application)

	int syserr;
		/// System error code

	int uverr;
		/// Libuv error code

	Error()
	{
		reset();
	}

	Error(const std::string& msg)
	{
		reset();
		message = msg;
	}

	bool any() 
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


#endif // SOURCEY_Types_H