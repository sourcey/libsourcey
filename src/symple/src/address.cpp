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


#include "scy/symple/address.h"
#include "scy/util.h"
#include "sstream"
#include "assert.h"


using std::endl;


namespace scy {
namespace smpl {


Address::Address()
{
}


Address::Address(const std::string& id)
{
	parse(id);
}


Address::Address(const std::string& user, const std::string& group, const std::string& id) :
	user(user), group(group), id(id)
{
}


Address::~Address()
{
}
	

bool Address::parse(const std::string& addr)
{	
	if (addr.empty())
		return false;
  
	// First find the ID part
	std::string::size_type slashpos = addr.find('/');
	id = (slashpos == std::string::npos ? "" : addr.substr(slashpos + 1));

	// Now look for the user
	std::string::size_type atpos = addr.find('@');
	std::string::size_type groupbegin;
	if (atpos < slashpos && atpos != std::string::npos) {
		user = addr.substr(0, atpos);
		groupbegin = atpos + 1;
	} else {
		groupbegin = 0;
	}

	// Now take what is left as the group
	std::string::size_type domainend = (slashpos == std::string::npos) ?
		(addr.length() - groupbegin) : (slashpos - groupbegin);
	group = addr.substr(groupbegin, domainend);

	return valid();
}

bool Address::valid() const
{
	return !user.empty()
		|| !group.empty()
		|| !id.empty();
}


void Address::print(std::ostream& os) const
{
	if (!user.empty())
		os << user;
	if (!group.empty()) {
		if (!user.empty())
			os << "@";
		os << group;
	}
	if (!id.empty()) {
		os << "/"; // always add slash to identify ID only addresses
		os << id;
	}
}


std::string Address::toString() const
{
	std::ostringstream os;
	print(os);
	return os.str(); 
}


bool Address::operator == (const Address& r)
{
	return group == r.group
		&& user == r.user
		&& id == r.id;
}


bool Address::operator == (std::string& r)
{
	return toString() == r;
}


#if 0 // JSON Address
Address::Address(const std::string& id)
{
	parse(id);
}
	

bool Address::parse(const std::string& id)
{
	std::vector<std::string> elems;
	util::split(id, ':', elems);
	if (elems.size() >= 1)
		group = elems[0];
	if (elems.size() >= 2)
		user = elems[1];
	if (elems.size() >= 3)
		id = elems[2];
	return valid();
}


std::string Address::id() const 
{
	return get("id", "").asString();
}


std::string Address::user() const 
{
	return get("user", "").asString();
}


std::string Address::name() const 
{
	return get("name", "").asString();
}


std::string Address::group() const 
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
#endif


} // namespace symple 
} // namespace scy


