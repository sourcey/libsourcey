///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
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
    // LTrace("Create")
}


Roster::~Roster()
{
    // LTrace("Destroy")
}


Peer* Roster::getByHost(const std::string& host)
{
    std::lock_guard<std::mutex> guard(_mutex);
    for (auto it = _map.begin(); it != _map.end(); ++it) {
        if (it->second->host() == host)
            return it->second;
    }
    return NULL;
}


Roster::PeerMap Roster::peers() const
{
    std::lock_guard<std::mutex> guard(_mutex);
    return _map;
}


void Roster::print(std::ostream& os) const
{
    std::lock_guard<std::mutex> guard(_mutex);

    os << "Roster[";
    for (auto it = _map.begin(); it != _map.end(); ++it) {
        os << "\n\t" << it->second << ": " << it->first;
    }
    os << "\n]";
}


#if 0
void Roster::update(const json::value& data, bool whiny)
{
    if (data.is_object() &&
        data.isMember("id") &&
        data.isMember("user") &&
        data.isMember("name") //&&
        //data.isMember("type")
        ) {
        LTrace("Updating: ", json::stringify(data, true))
        std::string id = data["id"].get<std::string>();
        Peer* peer = get(id, false);
        if (!peer) {
            peer = new Peer(data);
            add(id, peer);
        } else
            static_cast<json::value&>(*peer) = data;
    }
    else if (data.isArray()) {
        for (auto it = data.begin(); it != data.end(); it++) {
            update(*it, whiny);
        }
    }
    else {
        std::string error("Bad presence data: " + json::stringify(data));
        LError(error, )
        if (whiny)
            throw std::runtime_error(error);
    }
}
#endif


} // namespace smpl
} // namespace scy


/// @\}
