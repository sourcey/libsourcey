///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup turn
/// @{


#ifndef SCY_TURN_TCPClientConnection_H
#define SCY_TURN_TCPClientConnection_H


#include "scy/turn/turn.h"
#include "scy/stun/message.h"
#include "scy/net/tcpsocket.h"
#include "scy/net/socketemitter.h"
#include "scy/collection.h"
#include "scy/util/timeout.h"


namespace scy {
namespace turn {


class TURN_API TCPAllocation;


class TURN_API TCPConnectionPair
{
public:
    TCPConnectionPair(TCPAllocation& allocation);
    virtual ~TCPConnectionPair();

    /// Initiates an outgoing TCP connection to the
    /// peer for Connect requests.
    bool doPeerConnect(const net::Address& peerAddr);

    /// Binds the client <> peer relay pipe once the
    /// ConnectionBind request is successfull.
    bool makeDataConnection();

    void setPeerSocket(const net::TCPSocket::Ptr& socket);
    void setClientSocket(const net::TCPSocket::Ptr& socket);

    /// Connection success callback for Connect request.
    void onPeerConnectSuccess(net::Socket& socket);

    /// Connection error callback for Connect request.
    void onPeerConnectError(net::Socket& socket, const Error& error);

    void onClientDataReceived(net::Socket& socket, const MutableBuffer& buffer,
                              const net::Address& peerAddress);
    void onPeerDataReceived(net::Socket& socket, const MutableBuffer& buffer,
                            const net::Address& peerAddress);

    /// Callback for handing either client or peer connections
    /// which result in the destruction of the TCPConnectionPair.
    void onConnectionClosed(net::Socket& socket);

    /// Starts the ConnectionBind request timeout.
    ///
    /// If no ConnectionBind request associated with this peer data
    /// connection is received after 30 seconds, the peer data
    /// connection MUST be closed.
    void startTimeout();

    TCPAllocation& allocation;

    /// The client socket, nullptr to start.
    net::SocketEmitter client; // net::TCPSocket

    /// The client socket, nullptr to start.
    net::SocketEmitter peer; // net::TCPSocket

    /// Return true if the peer `ConnectionBind` request timed out.
    bool expired() const;

    /// Stores early peer > client data.
    Buffer earlyPeerData;

    /// The unique connection ID.
    uint32_t connectionID;

    /// True when p2p relay is flowing.
    bool isDataConnection;

    /// The ConnectionBind request timeout counter.
    Timeout timeout;

    stun::TransactionID transactionID;

private:
    /// NonCopyable and NonMovable
    TCPConnectionPair(const TCPConnectionPair&) = delete;
    TCPConnectionPair& operator=(const TCPConnectionPair&) = delete;
};


} // namespace turn
} // namespace scy


#endif // SCY_TURN_TCPClientConnection_H


/// @\}
