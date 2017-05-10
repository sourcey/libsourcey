///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup turn
/// @{


#ifndef SCY_TURN_SERVER_UDPAllocation_H
#define SCY_TURN_SERVER_UDPAllocation_H


#include "scy/net/packetsocket.h"
#include "scy/net/udpsocket.h"
#include "scy/turn/server/serverallocation.h"
#include "scy/turn/types.h"


namespace scy {
namespace turn {


class TURN_API Server;
class TURN_API IConnection;


class TURN_API UDPAllocation : public ServerAllocation
{
public:
    UDPAllocation(Server& server, const FiveTuple& tuple,
                  const std::string& username, const uint32_t& lifetime);
    virtual ~UDPAllocation();

    // void onPacketReceived(void* sender, RawPacket& packet);
    void onPeerDataReceived(net::Socket& socket, const MutableBuffer& buffer,
                            const net::Address& peerAddress);

    bool handleRequest(Request& request);
    void handleSendIndication(Request& request);

    ssize_t send(const char* data, size_t size,
                 const net::Address& peerAddress);

    net::Address relayedAddress() const;

private:
    net::SocketEmitter _relaySocket; // net::UDPSocket
};
}
} //  namespace scy::turn


#endif // SCY_TURN_SERVER_UDPAllocation_H


/// @\}
