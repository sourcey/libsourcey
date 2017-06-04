///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup symple
/// @{


#ifndef SCY_Symple_Roster_H
#define SCY_Symple_Roster_H


#include "scy/symple/symple.h"
#include "scy/symple/address.h"
#include "scy/symple/peer.h"
#include "scy/collection.h"


namespace scy {
namespace smpl {


/// The Roster provides a registry for active network
/// peers indexed by session ID.
class Symple_API Roster : public LiveCollection<std::string, Peer>
{
public:
    typedef LiveCollection<std::string, Peer> Manager;
    typedef Manager::Map PeerMap;

public:
    Roster();
    virtual ~Roster();

    /// Returns the first peer which matches the given host address.
    Peer* getByHost(const std::string& host);

    virtual PeerMap peers() const;

    virtual void print(std::ostream& os) const;

    virtual const char* className() const { return "Symple::Roster"; }
};


} // namespace smpl
} // namespace scy


#endif // SCY_Symple_Roster_H


/// @\}
