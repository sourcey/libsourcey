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


#include "Sourcey/Symple/ID.h"
#include "Sourcey/Util.h"
#include "sstream"
#include "assert.h"


using namespace std;


namespace Sourcey {
namespace Symple {


ID::ID() 
{
}


ID::ID(const string& id)
{
	parse(id);
}
	

bool ID::parse(const string& id)
{
	StringList elems;
	Util::split(id, ':', elems);
	if (elems.size() >= 1)
		group = elems[0];
	if (elems.size() >= 2)
		user = elems[1];
	if (elems.size() >= 3)
		node = elems[2];
	return valid();
}


bool ID::valid() const
{
	return !group.empty()
		&& !user.empty()
		&& !node.empty();
}


void ID::print(ostream& os) const
{
	os << group;
	os << ":";
	os << user;
	os << ":";
	os << node;
}


string ID::toString() const
{
	ostringstream os;
	print(os);
	return os.str(); 
}


bool ID::operator == (ID& r)
{
	return group == r.group
		&& user == r.user
		&& node == r.node;
}


bool ID::operator == (string& r)
{
	return toString() == r;
}


} // namespace Symple 
} // namespace Sourcey