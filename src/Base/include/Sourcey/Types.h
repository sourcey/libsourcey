//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is is distributed under a dual license that allows free, 
// open source use and closed source use under a standard commercial
// license.
//
// Non-Commercial Use:
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// Commercial Use:
// Please contact mail@sourcey.com
//


#ifndef SOURCEY_Types_H
#define SOURCEY_Types_H


#include "Poco/Types.h"
#include "Poco/Exception.h"

#include <string>
#include <vector>
#include <map>


namespace Sourcey {	


//
// Forward some Poco types to Sourcey namespace
// to reduce namespace pollution.
//
typedef Poco::Int8 Int8;
typedef Poco::UInt8 UInt8;
typedef Poco::Int16 Int16;
typedef Poco::UInt16 UInt16;
typedef Poco::Int32 Int24;
typedef Poco::UInt32 UInt24;
typedef Poco::Int32 Int32;
typedef Poco::UInt32 UInt32;
typedef Poco::Int64 Int64;
typedef Poco::UInt64 UInt64;


typedef Poco::Exception Exception;


typedef std::vector<std::string>			StringList;
typedef std::map<std::string, std::string>	StringMap;
typedef std::pair<std::string, std::string> Metadata;


struct StopPropagation: public std::exception
	// An exception used to break the current scope.
{
	virtual ~StopPropagation() throw() {};
};


struct IPolymorphic
{
	virtual ~IPolymorphic() {};
		
	template<class T>
	bool is() {
		return dynamic_cast<T*>(this) != NULL;
	};

	template<class T>
	T* as(bool whiny = false) {
		T* self = dynamic_cast<T*>(this);
		if (self == NULL && whiny)
			throw Exception("Polymorphic cast failed");
		return self;
	};
};


} // namespace Sourcey


#endif