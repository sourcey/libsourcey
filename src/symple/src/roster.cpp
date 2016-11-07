///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup symple
/// @{


#include "scy/symple/roster.h"
#include "scy/logger.h"


using std::endl;


namespace scy {
namespace smpl {


Roster::Roster()
{
    //TraceS(this) << "Create" << endl;
}


Roster::~Roster()
{
    //TraceS(this) << "Destroy" << endl;
}


Peer* Roster::getByHost(const std::string& host)
{
    Mutex::ScopedLock lock(_mutex);
    for (auto it = _map.begin(); it != _map.end(); ++it) {
        if (it->second->host() == host)
            return it->second;
    }
    return NULL;
}


Roster::PeerMap Roster::peers() const
{
    Mutex::ScopedLock lock(_mutex);
    return _map;
}


void Roster::print(std::ostream& os) const
{
    Mutex::ScopedLock lock(_mutex);

    os << "Roster[";
    for (auto it = _map.begin(); it != _map.end(); ++it) {
        os << "\n\t" << it->second << ": " << it->first;
    }
    os << "\n]";
}


#if 0
void Roster::update(const json::Value& data, bool whiny)
{
    if (data.isObject() &&
        data.isMember("id") &&
        data.isMember("user") &&
        data.isMember("name") //&&
        //data.isMember("type")
        ) {
        TraceS(this) << "Updating: " << json::stringify(data, true) << endl;
        std::string id = data["id"].asString();
        Peer* peer = get(id, false);
        if (!peer) {
            peer = new Peer(data);
            add(id, peer);
        } else
            static_cast<json::Value&>(*peer) = data;
    }
    else if (data.isArray()) {
        for (auto it = data.begin(); it != data.end(); it++) {
            update(*it, whiny);
        }
    }
    else {
        std::string error("Bad presence data: " + json::stringify(data));
        ErrorS(this) << error << endl;
        if (whiny)
            throw std::runtime_error(error);
    }
}
#endif


} } // namespace scy::smpl

/// @\}
