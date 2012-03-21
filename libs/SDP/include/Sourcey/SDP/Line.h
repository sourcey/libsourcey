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


#ifndef SOURCEY_SDP_LINE_H
#define SOURCEY_SDP_LINE_H


#include <string>
#include <vector>


namespace Sourcey {
namespace SDP { 


class Line 
{
public:
	enum Type 
	{
		A,
		C,
		M,
		O,
		S,
		T,
		V,
		I,
		B,
	};

public:
    Line(int type, int priority) :
		_type(type), _priority(priority) {}
    virtual ~Line() {}

	virtual std::string toString() = 0;	
		// A SDP line is a one line string. The toString method
		// returns this string and all line subclasses MUST
		// implement it.
		// 
		// @return The string that will be one line line in the 
		// SDP message

	unsigned int priority() const
		// The lines in an SDP message will be ordered depending
		// based on a priority value. A lower value means higher
		// priority. The value MUST be in the interval [0..15]. 
		// Lines with any other value will be discarded and not
		// included in the SDP message.
		// 
		// @return An integer that determines where in the
		//         list of lines this line will be in the
		//         SDP message.
		{ return _priority; };

	unsigned int type()
		// All lines have a type identifier (integer) that is 
		// defined in Line.h. This method returns this
		// identifier, and can be used to determine which 
		// of the subclasses an object is.
		// @return Type id that is defined in Line.h
		{ return _type; };
	
	void setPriority(unsigned int priority) { _priority = priority; }	
	void setType(unsigned int type) { _type = type; }	

private:
	unsigned int _type;
	unsigned int _priority;
};


} // namespace Sourcey
} // namespace SDP 


#endif
