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


#include "Sourcey/Symple/Peer.h"
#include "Sourcey/Logger.h"
#include "assert.h"


using namespace std;
using namespace Poco;


namespace Sourcey {
namespace Symple {


Peer::Peer() 
{
	(*this)["type"] = "Peer";
}


Peer::Peer(const Peer& r) :
	JSON::Value(r)
{
	(*this)["type"] = "Peer";
}


Peer::Peer(const JSON::Value& r) :
	JSON::Value(r)
{
	(*this)["type"] = "Peer";
}


Peer::~Peer() 
{
}

	
void Peer::print(std::ostream& os) const
{
	JSON::StyledWriter writer;
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


string Peer::id() const 
{
	return get("id", "").asString();
}


string Peer::user() const 
{
	return get("user", "").asString();
}


string Peer::name() const 
{
	return get("name", "").asString();
}


string Peer::group() const 
{
	return get("group", "").asString();
}


string Peer::type() const 
{
	return get("type", "").asString();
}


string Peer::host() const 
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


} // namespace Symple 
} // namespace Sourcey