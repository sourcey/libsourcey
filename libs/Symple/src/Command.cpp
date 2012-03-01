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


#include "Sourcey/Symple/Command.h"
#include "Sourcey/Util.h"
#include "Sourcey/CryptoProvider.h"
#include "assert.h"


using namespace std;
using namespace Poco;


namespace Sourcey {
namespace Symple {


Command::Command() 
{
	(*this)["type"] = "command";
}


Command::Command(const Command& root) :
	Message(root)
{
	if (!isMember("type"))
		(*this)["type"] = "command";
}


Command::Command(const JSON::Value& root) :
	Message(root)
{
	if (!isMember("type"))
		(*this)["type"] = "command";
}


Command::~Command() 
{
}


bool Command::valid() const
{
	return Message::valid()
		&& isMember("node");
}


string Command::node() const 
{
	return get("node", "").asString();
}


string Command::action() const 
{
	return get("action", "execute").asString();
}


void Command::setNode(const std::string& node)
{
	(*this)["node"] = node;
}

	
void Command::setAction(const string& action) 
{
	(*this)["action"] = action;
}


string Command::param(int n) const 
{
	StringList params = Util::split(node(), ':');
	assert(params.size() >= n);
	if (params.size() < n)
		return "";
	return params[n-1].data();
}


StringList Command::params() 
{
	StringList params = Util::split(node(), ':');
	return params;
}


bool Command::matches(const string& xnode) const 
{
	return Util::matchNodes(node(), xnode, ":");
}


} // namespace Symple 
} // namespace Sourcey