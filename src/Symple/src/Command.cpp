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


#include "Sourcey/Symple/Command.h"
#include "Sourcey/Util.h"
////#include "Sourcey/Crypto/Crypto.h"
#include "assert.h"


using namespace std;



namespace scy {
namespace smpl {


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


Command::Command(const json::Value& root) :
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

	
void Command::setAction(const std::string& action) 
{
	(*this)["action"] = action;
}


string Command::param(int n) const 
{
	std::vector<std::string> params = util::split(node(), ':');
	assert(params.size() >= n);
	if (params.size() < n)
		return "";
	return params[n-1].c_str();
}


std::vector<std::string> Command::params() 
{
	std::vector<std::string> params = util::split(node(), ':');
	return params;
}


bool Command::matches(const std::string& xnode) const 
{
	return util::matchNodes(node(), xnode, ":");
}


} // namespace symple 
} // namespace scy