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


#include "Sourcey/Symple/Address.h"
#include "Sourcey/Util.h"
#include "sstream"
#include "assert.h"


using namespace std;


namespace scy {
namespace smple {


Address::Address() :
	json::Value(Json::objectValue)
{
}


Address::Address(const Address& r) :
	json::Value(r)
{
}


Address::Address(const json::Value& r) :
	json::Value(r)
{
}


Address::~Address()
{
}


/*
Address::Address(const string& id)
{
	parse(id);
}
	

bool Address::parse(const string& id)
{
	StringVec elems;
	util::split(id, ':', elems);
	if (elems.size() >= 1)
		group = elems[0];
	if (elems.size() >= 2)
		user = elems[1];
	if (elems.size() >= 3)
		id = elems[2];
	return valid();
}
*/


string Address::id() const 
{
	return get("id", "").asString();
}


string Address::user() const 
{
	return get("user", "").asString();
}


string Address::name() const 
{
	return get("name", "").asString();
}


string Address::group() const 
{
	return get("group", "").asString();
}


void Address::setID(const std::string& id) 
{
	(*this)["id"] = id;
}


void Address::setUser(const std::string& user) 
{
	(*this)["user"] = user;
}


void Address::setName(const std::string& name) 
{
	(*this)["name"] = name;
}


void Address::setGroup(const std::string& group) 
{
	(*this)["group"] = group;
}


bool Address::valid() const
{
	return !id().empty()
		&& !user().empty();
}


void Address::print(ostream& os) const
{
	os << group();
	os << ":";
	os << user();
	os << "(";
	os << name();
	os << ")";
	os << ":";
	os << id();
}


/*
string Address::toString() const
{
	ostringstream os;
	print(os);
	return os.str(); 
}
*/


bool Address::operator == (const Address& r)
{
	return group() == r.group()
		&& user() == r.user()
		&& id() == r.id();
}


/*
bool Address::operator == (string& r)
{
	return toString() == r;
}
*/


} // namespace smple 
} // namespace scy