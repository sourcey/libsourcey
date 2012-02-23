//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2002 Sourcey
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


#ifndef SOURCEY_Symple_ID_H
#define SOURCEY_Symple_ID_H


#include "Sourcey/JSON/JSON.h"


namespace Sourcey {
namespace Symple {


struct ID
{	
	std::string group;
	std::string user;
	std::string node;
	
	ID();
	ID(const std::string& id);
	
	bool parse(const std::string& id);
	void print(std::ostream& os) const;
	std::string toString() const;
	bool valid() const;
	
	bool operator == (ID& r);
	bool operator == (std::string& r);
	
    friend std::ostream& operator << (std::ostream& os, const ID& id)
	{
		os << id.toString();
		return os;
	}
};


} // namespace Symple 
} // namespace Sourcey


#endif // SOURCEY_Symple_ID_H
