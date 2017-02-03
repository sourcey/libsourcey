///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:    LGPL-2.1+
//
/// @addtogroup symple
/// @{


#include "scy/symple/command.h"
#include "assert.h"
#include "scy/util.h"


using std::endl;


namespace scy {
namespace smpl {


Command::Command()
{
    (*this)["type"] = "command";
}


Command::Command(const Command& root)
    : Message(root)
{
    if (!isMember("type"))
        (*this)["type"] = "command";
}


Command::Command(const json::Value& root)
    : Message(root)
{
    if (!isMember("type"))
        (*this)["type"] = "command";
}


Command::~Command()
{
}


bool Command::valid() const
{
    return Message::valid() && isMember("node");
}


std::string Command::node() const
{
    return get("node", "").asString();
}


std::string Command::action() const
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


std::string Command::param(int n) const
{
    std::vector<std::string> params = util::split(node(), ':');
    assert(int(params.size()) >= n);
    if (int(params.size()) < n)
        return "";
    return params[n - 1].c_str();
}


std::vector<std::string> Command::params()
{
    return util::split(node(), ':');
}


bool Command::matches(const std::string& xnode) const
{
    return util::matchNodes(node(), xnode, ":");
}


} // namespace smpl
} // namespace scy


/// @\}
