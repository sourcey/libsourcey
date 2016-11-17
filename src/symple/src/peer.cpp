///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup symple
/// @{


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
    return Address(user(), id()); //, group()
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


// std::string Peer::group() const
// {
//     return get("group", "").asString();
// }


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


// void Peer::setGroup(const std::string& group)
// {
//     (*this)["group"] = group;
// }


void Peer::setType(const std::string& type)
{
    (*this)["type"] = type;
}


void Peer::setHost(const std::string& host)
{
    (*this)["host"] = host;
}


} // namespace smpl
} // namespace scy

/// @\}
