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


#include "Sourcey/Symple/Peer.h"
#include "Sourcey/Util.h"
#include "Sourcey/CryptoProvider.h"
#include "Poco/Format.h"
#include "assert.h"


using namespace std;
//using namespace Sourcey::XML;
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
		&& isMember("name")
		&& isMember("type");
}


ID Peer::id() const 
{
	return ID(get("id", "").asString());
}


string Peer::name() const 
{
	return get("name", "").asString();
}


string Peer::type() const 
{
	return get("type", "").asString();
}

string Peer::user() const 
{
	return get("user", "").asString();
}

string Peer::address() const 
{
	return get("address", "").asString();
}


void Peer::setID(const std::string& id) 
{
	(*this)["id"] = id;
}


void Peer::setType(const std::string& type) 
{
	(*this)["type"] = type;
}


void Peer::setName(const std::string& name) 
{
	(*this)["name"] = name;
}


void Peer::setUser(const std::string& user) 
{
	(*this)["user"] = user;
}


void Peer::setAddress(const std::string& addr) 
{
	(*this)["addr"] = addr;
}


} // namespace Symple 
} // namespace Sourcey