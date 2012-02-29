//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2005 Sourcey
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


#ifndef SOURCEY_SDP_LINE_A_H
#define SOURCEY_SDP_LINE_A_H


#include "Sourcey/SDP/Line.h"

#include <string>


namespace Sourcey {
namespace SDP { 


class A: public Line {
public:
	A(const std::string& src);
	virtual ~A();

	void setAttributes(const std::string& attr);

	std::string attributes();
	std::string attributeType();
	std::string attributeValue();

	virtual std::string toString();

private:
	std::string _attributes;
};


} // namespace Sourcey
} // namespace SDP 

#endif
