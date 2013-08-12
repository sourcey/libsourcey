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


#ifndef SOURCEY_Polymorphic_H
#define SOURCEY_Polymorphic_H


#include "Sourcey/Types.h"


namespace scy {	
	
	
struct LogStream;
class Polymorphic
	// A base class for C++ callback polymorphism.
{
public:
	virtual ~Polymorphic() {};
		
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

	virtual LogStream& log(const char* level = "debug") const;

	virtual const char* className() const = 0;
};


typedef Polymorphic Module;
	// The LibSourcey base module type.
	// May become a class type in the future.


} // namespace scy


#endif