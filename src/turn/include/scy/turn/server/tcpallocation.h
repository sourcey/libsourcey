///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup turn
/// @{


#ifndef SCY_TURN_SERVER_TCPAllocation_H
#define SCY_TURN_SERVER_TCPAllocation_H


#include "scy/collection.h"
#include "scy/net/tcpsocket.h"
#include "scy/turn/server/serverallocation.h"
#include "scy/turn/server/tcpconnectionpair.h"


namespace scy {
namespace turn {


class TURN_API Server;


typedef PointerCollection<uint32_t, TCPConnectionPair>
    TCPConnectionPairMap;


class TURN_API TCPAllocation : public ServerAllocation
{
public:
    TCPAllocation(Server& server, const net::Socket::Ptr& control,
                  const FiveTuple& tuple, const std::string& username,
                  const uint32_t& lifetime);
    virtual ~TCPAllocation();

    bool handleRequest(Request& request);
    void handleConnectRequest(Request& request);
    void handleConnectionBindRequest(Request& request);

    /// Sends a Connect request response to control.
    void sendPeerConnectResponse(TCPConnectionPair* pair, bool success);

    int sendToControl(stun::Message& message);

    net::TCPSocket& control();
    net::Address relayedAddress() const;
    TCPConnectionPairMap& pairs();

    bool onTimer();

    /// Accepts incoming peer sockets for ConnectionBind requests.
    void onPeerAccept(const net::TCPSocket::Ptr& sock);

    /// Callback for handling controll connection destruction.
    /// The allocation will be deleted.
    void onControlClosed(net::Socket& socket);

protected:
    net::SocketEmitter _control; // net::TCPSocket
    net::SocketEmitter _acceptor; // net::TCPSocket
    TCPConnectionPairMap _pairs;
};


} } //  namespace scy::turn


#endif // SCY_TURN_SERVER_TCPAllocation_H


/// @\}
