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


#include "scy/symple/peer.h"
#include "scy/logger.h"
#include "assert.h"


using std::endl;
//


namespace scy {
namespace smpl {


Peer::Peer() 
{
	(*this)["type"] = "Peer";
}


Peer::Peer(const Peer& r) :
	json::Value(r)
{
	(*this)["type"] = "Peer";
}


Peer::Peer(const json::Value& r) :
	json::Value(r)
{
	(*this)["type"] = "Peer";
}


Peer::~Peer() 
{
}

	
void Peer::print(std::ostream& os) const
{
	json::StyledWriter writer;
	os << writer.write(*this);
}


bool Peer::valid() 
{
	return isMember("id") 
		&& isMember("user")
		&& isMember("type");
}


Address Peer::address() const 
{
	Address a;
	a.setID(id());
	a.setUser(user());
	a.setName(name());
	a.setGroup(group());
	return a;
}


std::string Peer::id() const 
{
	return get("id", "").asString();
}


std::string Peer::user() const 
{
	return get("user", "").asString();
}


std::string Peer::name() const 
{
	return get("name", "").asString();
}


std::string Peer::group() const 
{
	return get("group", "").asString();
}


std::string Peer::type() const 
{
	return get("type", "").asString();
}


std::string Peer::host() const 
{
	return get("host", "").asString();
}


void Peer::setID(const std::string& id) 
{
	(*this)["id"] = id;
}


void Peer::setUser(const std::string& user) 
{
	(*this)["user"] = user;
}


void Peer::setName(const std::string& name) 
{
	(*this)["name"] = name;
}

void Peer::setGroup(const std::string& group) 
{
	(*this)["group"] = group;
}

void Peer::setType(const std::string& type) 
{
	(*this)["type"] = type;
}


void Peer::setHost(const std::string& host) 
{
	(*this)["host"] = host;
}


} // namespace symple 
} // namespace scy