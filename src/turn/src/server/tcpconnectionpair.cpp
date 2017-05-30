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
    STrace << "Create: " << connectionID << endl;
}


TCPConnectionPair::~TCPConnectionPair()
{
    STrace << "Destroy: " << connectionID << endl;

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
        SError << "Peer connect error: " << exc.what() << endl;
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
    STrace << "Make data connection: " << connectionID << endl;
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
        STrace << "Flushing early media: " << earlyPeerData.size() << endl;
        client->send(earlyPeerData.data(), earlyPeerData.size());
        earlyPeerData.clear();
    }

    return (isDataConnection = true);
}


void TCPConnectionPair::onPeerDataReceived(net::Socket&,
                                           const MutableBuffer& buffer,
                                           const net::Address& peerAddress)
{
    STrace << "Peer > Client: " << buffer.size() << endl;
    // assert(pkt.buffer.position() == 0);
    // if (pkt.buffer.available() < 300)
    //    STrace << "Peer => Client: " << pkt.buffer << endl;
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
        SDebug << "Buffering early data: " << len << endl;
        //#ifdef _DEBUG
        //    SDebug << "Printing early data: " << std::string(buf, len) << endl;
        //#endif
        if (len > maxSize)
            SWarn << "Dropping early media: Oversize packet: " << len << endl;
        if (earlyPeerData.size() > maxSize)
            SWarn << "Dropping early media: Buffer at capacity >= " << maxSize << endl;

        // earlyPeerData.append(static_cast<const char*>(pkt.data()), len);
        earlyPeerData.insert(earlyPeerData.end(), buf, buf + len);
    }
}


void TCPConnectionPair::onClientDataReceived(net::Socket&,
                                             const MutableBuffer& buffer,
                                             const net::Address& peerAddress)
{
    STrace << "Client > Peer: " << buffer.size() << endl;
    // assert(packet.buffer.position() == 0);
    // if (packet.size() < 300)
    //    STrace << "Client > Peer: " << packet.buffer << endl;

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
    STrace << "Peer Connect request error: " << error.message << endl;
    assert(&socket == peer.impl.get());
    allocation.sendPeerConnectResponse(this, false);

    // The TCPConnectionPair will be deleted on next call to onConnectionClosed
}


void TCPConnectionPair::onConnectionClosed(net::Socket& socket)
{
    STrace << "Connection pair socket closed: " << connectionID << ": " << &socket << endl;
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
