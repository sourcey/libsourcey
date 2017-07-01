///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup turn
/// @{


#include "scy/turn/server/tcpallocation.h"
#include "scy/logger.h"
#include "scy/turn/server/server.h"


using namespace std;


namespace scy {
namespace turn {


TCPAllocation::TCPAllocation(Server& server, const net::Socket::Ptr& control,
                             const FiveTuple& tuple,
                             const std::string& username,
                             const uint32_t& lifetime)
    : ServerAllocation(server, tuple, username, lifetime)
    , _control(std::dynamic_pointer_cast<net::TCPSocket>(control))
    , _acceptor(std::make_shared<net::TCPSocket>())
{
    // Bind a socket acceptor for incoming peer connections.
    _acceptor->bind(net::Address(server.options().listenAddr.host(), 0));
    _acceptor->listen();
    _acceptor.as<net::TCPSocket>()->AcceptConnection += slot(this, &TCPAllocation::onPeerAccept);

    // The allocation will be deleted if the control connection is lost.
    _control.Close += slot(this, &TCPAllocation::onControlClosed);

    LTrace("Initializing on ", _acceptor->address())
}


TCPAllocation::~TCPAllocation()
{
    LTrace("Destroy TCP allocation")

   
    _acceptor.as<net::TCPSocket>()->AcceptConnection -= slot(this, &TCPAllocation::onPeerAccept);
    _acceptor->close();

    _control.Close -= slot(this, &TCPAllocation::onControlClosed);
    _control->close();

    auto pairs = this->pairs().map();
    for (auto it = pairs.begin(); it != pairs.end(); ++it) {
        // The allocation will be removed via callback
        delete it->second;
    }
    assert(this->pairs().empty());

    LTrace("Destroy TCP allocation: OK")
}


void TCPAllocation::onPeerAccept(const net::TCPSocket::Ptr& socket)
{
    LTrace("Peer connection accepted: ", socket->peerAddress())

    // 5.3. Receiving a TCP Connection on a Relayed Transport Address
    //
    // When a server receives an incoming TCP connection on a relayed
    // transport address, it processes the request as follows.
    //
    // The server MUST accept the connection. If it is not successful,
    // nothing is sent to the client over the control connection.
    //
    // If the connection is successfully accepted, it is now called a peer
    // data connection.  The server MUST buffer any data received from the
    // peer.  The server adjusts its advertised TCP receive window to
    // reflect the amount of empty buffer space.
    //
    // If no permission for this peer has been installed for this
    // allocation, the server MUST close the connection with the peer
    // immediately after it has been accepted.
    //
    if (!hasPermission(socket->peerAddress().host())) {
        LTrace("No permission for peer: ", socket->peerAddress())
        return;
    }
    LTrace("Has permission for: ", socket->peerAddress())

    // Otherwise, the server sends a ConnectionAttempt indication to the
    // client over the control connection. The indication MUST include an
    // XOR-PEER-ADDRESS attribute containing the peer's transport address,
    // as well as a CONNECTION-ID attribute uniquely identifying the peer
    // data connection.
    //
    auto pair = new TCPConnectionPair(*this);
    // assert(socket->/*base().*/refCount() == 1);
    pair->setPeerSocket(socket);
    // assert(socket->/*base().*/refCount() == 2);

    stun::Message response(stun::Message::Indication,
                           stun::Message::ConnectionAttempt);
    // stun::Message response;
    // response.setType(stun::Message::ConnectionAttempt);

    auto addrAttr = new stun::XorPeerAddress;
    addrAttr->setAddress(socket->peerAddress());
    // addrAttr->setFamily(1);
    // addrAttr->setPort(socket->peerAddress().port());
    // addrAttr->setIP(socket->peerAddress().host());
    response.add(addrAttr);

    auto connAttr = new stun::ConnectionID;
    connAttr->setValue(pair->connectionID);
    response.add(connAttr);

    sendToControl(response);

    STrace << "Peer connection accepted with ID: " << pair->connectionID
           << endl;
}


bool TCPAllocation::handleRequest(Request& request)
{
    LTrace("Handle request")

    if (!ServerAllocation::handleRequest(request)) {
        if (request.methodType() == stun::Message::Connect)
            handleConnectRequest(request);
        else if (request.methodType() == stun::Message::ConnectionBind)
            handleConnectionBindRequest(request);
        else
            return false;
    }

    return true;
}


bool TCPAllocation::onTimer()
{
    LTrace("On timer")

    // Clean up any expired Connect request peer connections.
    auto pairs = this->pairs().map();
    for (auto it = pairs.begin(); it != pairs.end(); ++it) {
        if (it->second->expired()) {
            LTrace("On timer: Removing expired peer")
            this->pairs().free(it->first);
        }
    }

    return ServerAllocation::onTimer();
}


void TCPAllocation::handleConnectRequest(Request& request)
{
    LTrace("Handle Connect request")

    // 5.2. Receiving a Connect Request
    //
    // When the server receives a Connect request, it processes the request
    // as follows.
    //
    // If the request is received on a TCP connection for which no
    // allocation exists, the server MUST return a 437 (Allocation Mismatch)
    // error.
    //
    // If the server is currently processing a Connect request for this
    // allocation with the same XOR-PEER-ADDRESS, it MUST return a 446
    // (Connection Already Exists) error.
    //
    // If the server has already successfully processed a Connect request
    // for this allocation with the same XOR-PEER-ADDRESS, and the resulting
    // client and peer data connections are either pending or active, it
    // MUST return a 446 (Connection Already Exists) error.
    //
    // If the request does not contain an XOR-PEER-ADDRESS attribute, or if
    // such attribute is invalid, the server MUST return a 400 (Bad Request)
    // error.
    //
    // If the new connection is forbidden by local policy, the server MUST
    // reject the request with a 403 (Forbidden) error.
    //
    auto peerAttr = request.get<stun::XorPeerAddress>();
    if (!peerAttr || (peerAttr && peerAttr->family() != 1)) {
        server().respondError(request, 400, "Bad Request");
        return;
    }

    // Otherwise, the server MUST initiate an outgoing TCP connection.
    // The local endpoint is the relayed transport address associated with
    // the allocation.  The remote endpoint is the one indicated by the
    // XOR-PEER-ADDRESS attribute.  If the connection attempt fails or times
    // out, the server MUST return a 447 (Connection Timeout or Failure)
    // error.  The timeout value MUST be at least 30 seconds.
    //
    auto pair = new TCPConnectionPair(*this);
    pair->transactionID = request.transactionID();
    pair->doPeerConnect(peerAttr->address());
}


void TCPAllocation::handleConnectionBindRequest(Request& request)
{
    LTrace("Handle ConnectionBind Request")

    assert(request.methodType() == stun::Message::ConnectionBind);
    TCPConnectionPair* pair = nullptr;
    auto socket = _server.getTCPSocket(request.remoteAddress);
    try {
        if (!socket)
            throw std::runtime_error("Invalid TCP socket");

        // 5.4. Receiving a ConnectionBind Request
        //
        // When a server receives a ConnectionBind request, it processes the
        // request as follows.
        //
        // If the client connection transport is not TCP or TLS, the server MUST
        // return a 400 (Bad Request) error.
        //
        if (request.transport != net::TCP) // TODO: TLS!!
            throw std::runtime_error(
                "TLS not supported"); // easy to implement, fixme!

        // If the request does not contain the CONNECTION-ID attribute, or if
        // this attribute does not refer to an existing pending connection, the
        // server MUST return a 400 (Bad Request) error.
        //
        auto connAttr = request.get<stun::ConnectionID>();
        if (!connAttr)
            throw std::runtime_error(
                "ConnectionBind missing CONNECTION-ID attribute");

        // Otherwise, the client connection is now called a client data
        // connection.  Data received on it MUST be sent as-is to the associated
        // peer data connection.
        //
        // Data received on the associated peer data connection MUST be sent
        // as-is on this client data connection.  This includes data that was
        // received after the associated Connect or request was successfully
        // processed and before this ConnectionBind request was received.
        //
        pair = pairs().get(connAttr->value(), false);
        if (!pair) {
            throw std::runtime_error("No client for ConnectionBind request: " +
                                     util::itostr(connAttr->value()));
        }

        if (pair->isDataConnection) {
            assert(0);
            throw std::runtime_error("Already a peer data connection: " +
                                     util::itostr(connAttr->value()));
        }

        stun::Message response(stun::Message::SuccessResponse,
                               stun::Message::ConnectionBind);
        response.setTransactionID(request.transactionID());

        // Send the response back over the client connection
        socket->sendPacket(response);

        // Reassign the socket base instance to the client connection.
        pair->setClientSocket(socket);
        if (!pair->makeDataConnection()) {
            // Must have a client and peer by now
            throw std::runtime_error("BUG: Data connection binding failed");
        }

        assert(pair->isDataConnection);
    } catch (std::exception& exc) {
        LError("ConnectionBind error: ", exc.what())
        server().respondError(request, 400, "Bad Request");

        if (pair && !pair->isDataConnection) {
            delete pair;
        }

        // Close the incoming connection
        socket->close();
    }
}


void TCPAllocation::sendPeerConnectResponse(TCPConnectionPair* pair, bool success)
{
    LTrace("Send peer Connect response: ", success)

    assert(!pair->transactionID.empty());

    // If the connection is successful, it is now called a peer data
    // connection. The server MUST buffer any data received from the
    // client. The server adjusts its advertised TCP receive window to
    // reflect the amount of empty buffer space.
    //
    // The server MUST include the CONNECTION-ID attribute in the Connect
    // success response. The attribute's value MUST uniquely identify the
    // peer data connection.
    //
    stun::Message response(stun::Message::SuccessResponse,
                           stun::Message::Connect);
    response.setTransactionID(pair->transactionID);

    if (success) {
        auto connAttr = new stun::ConnectionID;
        connAttr->setValue(pair->connectionID);
        response.add(connAttr);
    } else {
        auto errorCodeAttr = new stun::ErrorCode();
        errorCodeAttr->setErrorCode(447);
        errorCodeAttr->setReason("Connection Timeout or Failure");
        response.add(errorCodeAttr);
    }

    sendToControl(response);
}


int TCPAllocation::sendToControl(stun::Message& message)
{
    LTrace("Send to control: ", message)
    return _control->sendPacket(message, 0);
}


void TCPAllocation::onControlClosed(net::Socket& socket)
{
    LTrace("Control socket disconnected")

    // The allocation will be destroyed on the
    // next timer call to IAllocation::deleted()
    _deleted = true;
}


net::TCPSocket& TCPAllocation::control()
{
    return *_control.as<net::TCPSocket>();
}


TCPConnectionPairMap& TCPAllocation::pairs()
{
    return _pairs;
}


net::Address TCPAllocation::relayedAddress() const
{
    return _acceptor->address();
}


} } //  namespace scy::turn


/// @\}
