///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup turn
/// @{


#include "scy/turn/server/tcpconnectionpair.h"
#include "scy/turn/server/server.h"
#include "scy/turn/server/tcpallocation.h"
#include "scy/net/util.h"
#include "scy/crypto/crypto.h"


using namespace std;


namespace scy {
namespace turn {


TCPConnectionPair::TCPConnectionPair(TCPAllocation& allocation)
    : allocation(allocation)
    , client(nullptr)
    , peer(nullptr)
    , earlyPeerData(0)
    , connectionID(util::randomNumber())
    , isDataConnection(false)
{
    while (!allocation.pairs().add(connectionID, this, false)) {
        connectionID = util::randomNumber();
    }
    LTrace("Create: ", connectionID)
}


TCPConnectionPair::~TCPConnectionPair()
{
    LTrace("Destroy: ", connectionID)

    if (client.impl) {
        client.Recv -= slot(this, &TCPConnectionPair::onClientDataReceived);
        client.Close -= slot(this, &TCPConnectionPair::onConnectionClosed);
        client->close();
    }
    if (peer.impl) {
        peer.Recv -= slot(this, &TCPConnectionPair::onPeerDataReceived);
        peer.Connect -= slot(this, &TCPConnectionPair::onPeerConnectSuccess);
        peer.Error -= slot(this, &TCPConnectionPair::onPeerConnectError);
        peer.Close -= slot(this, &TCPConnectionPair::onConnectionClosed);
        peer->close();
    }

    assert(allocation.pairs().exists(connectionID));
    allocation.pairs().remove(connectionID);
}


bool TCPConnectionPair::doPeerConnect(const net::Address& peerAddr)
{
    try {
        assert(!transactionID.empty());
        peer.swap(std::make_shared<net::TCPSocket>());
        peer.impl->opaque = this;
        peer.Close += slot(this, &TCPConnectionPair::onConnectionClosed);

        // Start receiving early media
        peer.Recv += slot(this, &TCPConnectionPair::onPeerDataReceived);

        // Connect request specific events
        peer.Connect += slot(this, &TCPConnectionPair::onPeerConnectSuccess);
        peer.Error += slot(this, &TCPConnectionPair::onPeerConnectError);

        client->connect(peerAddr);
    } catch (std::exception& exc) {
        LError("Peer connect error: ", exc.what())
        assert(0);
        return false;
    }
    return true;
}


void TCPConnectionPair::setPeerSocket(const net::TCPSocket::Ptr& socket)
{
    STrace << "Set peer socket: " << connectionID << ": "
                 << socket->peerAddress() << endl;

    assert(!peer.impl);
    peer.swap(socket);
    peer.Close += slot(this, &TCPConnectionPair::onConnectionClosed);

    // Receive and buffer early media from peer
    peer.Recv += slot(this, &TCPConnectionPair::onPeerDataReceived);

    net::setServerSocketRecvBufSize<uv_tcp_t>(
        *socket.get(), SERVER_SOCK_BUF_SIZE); // TODO: make option
}


void TCPConnectionPair::setClientSocket(const net::TCPSocket::Ptr& socket)
{
    STrace << "Set client socket: " << connectionID << ": "
                 << socket->peerAddress() << endl;

    assert(!client.impl);
    client.swap(socket);
    client.Close += slot(this, &TCPConnectionPair::onConnectionClosed);

    net::setServerSocketRecvBufSize<uv_tcp_t>(
        *socket.get(), SERVER_SOCK_BUF_SIZE); // TODO: make option
}


bool TCPConnectionPair::makeDataConnection()
{
    LTrace("Make data connection: ", connectionID)
    if (!peer.impl || !client.impl)
        return false;

    peer.Recv += slot(this, &TCPConnectionPair::onPeerDataReceived);
    client.Recv += slot(this, &TCPConnectionPair::onClientDataReceived);

    // Relase and unbind the client socket from the server.
    // The client socket instance, events and data will be
    // managed by the TCPConnectionPair from now on.

    allocation.server().releaseTCPSocket(*client.impl.get());

    // Send early data from peer to client
    if (earlyPeerData.size()) {
        LTrace("Flushing early media: ", earlyPeerData.size())
        client->send(earlyPeerData.data(), earlyPeerData.size());
        earlyPeerData.clear();
    }

    return (isDataConnection = true);
}


void TCPConnectionPair::onPeerDataReceived(net::Socket&,
                                           const MutableBuffer& buffer,
                                           const net::Address& peerAddress)
{
    LTrace("Peer > Client: ", buffer.size())
    // assert(pkt.buffer.position() == 0);
    // if (pkt.buffer.available() < 300)
    //    LTrace("Peer => Client: ", pkt.buffer)
    // auto socket = reinterpret_cast<net::Socket*>(sender);
    // char* buf = bufferCast<char*>(buf);

    // Buffer& buf = pkt.buffer;
    const char* buf = bufferCast<const char*>(buffer);
    size_t len = buffer.size();
    if (client.impl) {

        allocation.updateUsage(len);
        if (allocation.deleted())
            return;

        // assert(buf.position() == 0);
        client->send(buf, len);
    }

#if 0
    // Flash policy requests
    // TODO: Handle elsewhere? Bloody flash...
    else if (len == 23 && (strcmp(buf, "<policy-file-request/>") == 0)) {
        LTrace("Handle flash policy")
        std::string policy(
            "<?xml version=\"1.0\"?><cross-domain-policy><allow-access-from "
            "domain=\"*\" to-ports=\"*\" /></cross-domain-policy>");
        // assert(peer->get() == pkt.info->socket);
        peer->send(policy.c_str(), policy.length() + 1);
        peer->close();
    }
#endif

    // Buffer early media
    // TODO: Make buffer size server option
    else {
        size_t maxSize =
            allocation.server().options().earlyMediaBufferSize;
        LDebug("Buffering early data: ", len)
        //#ifdef _DEBUG
        //    LDebug("Printing early data: ", std::string(buf, len))
        //#endif
        if (len > maxSize)
            LWarn("Dropping early media: Oversize packet: ", len)
        if (earlyPeerData.size() > maxSize)
            LWarn("Dropping early media: Buffer at capacity >= ", maxSize)

        // earlyPeerData.append(static_cast<const char*>(pkt.data()), len);
        earlyPeerData.insert(earlyPeerData.end(), buf, buf + len);
    }
}


void TCPConnectionPair::onClientDataReceived(net::Socket&,
                                             const MutableBuffer& buffer,
                                             const net::Address& peerAddress)
{
    LTrace("Client > Peer: ", buffer.size())
    // assert(packet.buffer.position() == 0);
    // if (packet.size() < 300)
    //    LTrace("Client > Peer: ", packet.buffer)

    if (peer.impl) {
        allocation.updateUsage(buffer.size());
        if (allocation.deleted())
            return;

        peer->send(bufferCast<char*>(buffer), buffer.size());
    }
}


void TCPConnectionPair::onPeerConnectSuccess(net::Socket& socket)
{
    LTrace("Peer Connect request success")
    assert(&socket == peer.impl.get());
    peer.Connect -= slot(this, &TCPConnectionPair::onPeerConnectSuccess);
    peer.Error -= slot(this, &TCPConnectionPair::onPeerConnectError);

    // If no ConnectionBind request associated with this peer data
    // connection is received after 30 seconds, the peer data connection
    // MUST be closed.

    allocation.sendPeerConnectResponse(this, true);

    // TODO: Ensure this is implemented properly
    startTimeout();
}


void TCPConnectionPair::onPeerConnectError(net::Socket& socket, const Error& error)
{
    LTrace("Peer Connect request error: ", error.message)
    assert(&socket == peer.impl.get());
    allocation.sendPeerConnectResponse(this, false);

    // The TCPConnectionPair will be deleted on next call to onConnectionClosed
}


void TCPConnectionPair::onConnectionClosed(net::Socket& socket)
{
    LTrace("Connection pair socket closed: ",  connectionID,  ": ", &socket)
    delete this; // kill
}


void TCPConnectionPair::startTimeout()
{
    timeout.reset();
}


bool TCPConnectionPair::expired() const
{
    return timeout.running() && timeout.expired();
}


} // namespace turn
} // namespace scy


/// @\}
