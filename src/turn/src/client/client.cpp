///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup turn
/// @{


#include "scy/turn/client/client.h"
#include "scy/application.h"
#include "scy/crypto/hash.h"
#include "scy/hex.h"
#include "scy/logger.h"
#include "scy/net/tcpsocket.h"
#include "scy/net/udpsocket.h"

#include <algorithm>
#include <assert.h>
#include <iostream>


using namespace std;


namespace scy {
namespace turn {


Client::Client(ClientObserver& observer, const Options& options, const net::Socket::Ptr& socket)
    : _observer(observer)
    , _options(options)
    , _socket(socket)
{
}


Client::~Client()
{
    LTrace("Destroy")
    shutdown();
    // assert(_socket->/*base().*/refCount() == 1);
    // assert(closed());
}


void Client::initiate()
{
    LDebug("TURN client connecting to ", _options.serverAddr)

    assert(!_permissions.empty() && "must set permissions");
    assert(_socket.impl && "must set socket");

    auto udpSocket = dynamic_cast<net::UDPSocket*>(_socket.impl.get());
    if (udpSocket) {
        udpSocket->bind(net::Address("0.0.0.0", 0));
        // udpSocket->setBroadcast(true);
    }

    _socket.Recv += slot(this, &Client::onSocketRecv, -1, -1);
    _socket.Connect += slot(this, &Client::onSocketConnect);
    _socket.Close += slot(this, &Client::onSocketClose);
    _socket->connect(_options.serverAddr);
}


void Client::shutdown()
{
    _timer.stop();

    for (auto it = _transactions.begin(); it != _transactions.end();) {
        LTrace("Shutdown base: Delete transaction: ", *it)
        (*it)->StateChange -= slot(this, &Client::onTransactionProgress);
        // delete *it;
        (*it)->dispose();
        it = _transactions.erase(it);
    }

    _socket.Connect -= slot(this, &Client::onSocketConnect);
    _socket.Recv -= slot(this, &Client::onSocketRecv);
    //_socket->Error -= slot(this, &Client::onSocketError);
    _socket.Close -= slot(this, &Client::onSocketClose);
    if (!_socket->closed()) {
        _socket->close();
    }
}


void Client::onSocketConnect(net::Socket& socket)
{
    LTrace("Client connected")
    _socket.Connect -= slot(this, &Client::onSocketConnect);

    _timer.setInterval(_options.timerInterval);
    _timer.start(std::bind(&Client::onTimer, this));

    sendAllocate();
}


void Client::onSocketRecv(net::Socket& socket, const MutableBuffer& buffer, const net::Address& peerAddress)
{
    LTrace("Control socket recv: ", buffer.size())

    stun::Message message;
    // auto socket = reinterpret_cast<net::Socket*>(sender);
    char* buf = bufferCast<char*>(buffer);
    size_t len = buffer.size();
    size_t nread = 0;
    while (len > 0 && (nread = message.read(constBuffer(buf, len))) > 0) {
        handleResponse(message);
        buf += nread;
        len -= nread;
    }
    if (len == buffer.size())
        LWarn("Non STUN packet received")

#if 0
    stun::Message message;
    if (message.read(constBuffer(packet.data(), packet.size())))
        handleResponse(message);
    else
        LWarn("Non STUN packet received")
#endif
}


void Client::onSocketClose(net::Socket& socket)
{
    assert(&socket == _socket.impl.get());
    LTrace("Control socket closed")
    assert(_socket->closed());
    shutdown();
    setError(_socket->error());
}


void Client::sendRefresh()
{
    // 7. Refreshing an Allocation
    //
    // A Refresh transaction can be used to either (a) refresh an existing
    // allocation and update its time-to-expiry or (b) delete an existing
    // allocation.
    //
    // If a client wishes to continue using an allocation, then the client
    // MUST refresh it before it expires. It is suggested that the client
    // refresh the allocation roughly 1 minute before it expires. If a
    // client no longer wishes to use an allocation, then it SHOULD
    // explicitly delete the allocation. A client MAY refresh an allocation
    // at any time for other reasons.

    LTrace("Send refresh allocation request")

    auto transaction = createTransaction();
    transaction->request().setClass(stun::Message::Request);
    transaction->request().setMethod(stun::Message::Refresh);

    stun::Lifetime* lifetimeAttr = new stun::Lifetime;
    lifetimeAttr->setValue((uint32_t)_options.lifetime / 1000);
    transaction->request().add(lifetimeAttr);

    sendAuthenticatedTransaction(transaction);
}


void Client::handleRefreshResponse(const stun::Message& response)
{
    LTrace("Received a Refresh Response: ", response.toString())

    assert(response.methodType() == stun::Message::Refresh);

    // 7.3. Receiving a Refresh Response
    //
    //
    // If the client receives a success response to its Refresh request with
    // a non-zero lifetime, it updates its copy of the allocation data
    // structure with the time-to-expiry value contained in the response.
    //
    // If the client receives a 437 (Allocation Mismatch) error response to
    // a request to delete the allocation, then the allocation no longer
    // exists and it should consider its request as having effectively
    // succeeded.
    auto errorAttr = response.get<stun::ErrorCode>();
    if (errorAttr) {
        assert(errorAttr->errorCode() == 437);
        return;
    }

    auto lifetimeAttr = response.get<stun::Lifetime>();
    if (!lifetimeAttr) {
        assert(0);
        return;
    }

    setLifetime(lifetimeAttr->value());

    LTrace("Refreshed allocation expires in: ", timeRemaining())

    // If lifetime is 0 the allocation will be cleaned up by garbage collection.
}


bool Client::removeTransaction(stun::Transaction* transaction)
{
    LTrace("Removing transaction: ", transaction)


    for (auto it = _transactions.begin(); it != _transactions.end(); ++it) {
        if (*it == transaction) {
            (*it)->StateChange -= slot(this, &Client::onTransactionProgress);
            _transactions.erase(it);
            return true;
        }
    }
    assert(0 && "unknown transaction");
    return false;
}


void Client::authenticateRequest(stun::Message& request)
{
    // Authenticate messages once the server provides us with realm and noonce
    if (_realm.empty())
        return;

    if (_options.username.size()) {
        auto usernameAttr = new stun::Username;
        usernameAttr->copyBytes(_options.username.c_str(),
                                _options.username.size());
        request.add(usernameAttr);
    }

    if (_realm.size()) {
        auto realmAttr = new stun::Realm;
        realmAttr->copyBytes(_realm.c_str(), _realm.size());
        request.add(realmAttr);
    }

    if (_nonce.size()) {
        auto nonceAttr = new stun::Nonce;
        nonceAttr->copyBytes(_nonce.c_str(), _nonce.size());
        request.add(nonceAttr);
    }

    if (_realm.size() && _options.password.size()) {
        crypto::Hash engine("md5");
        engine.update(_options.username + ":" + _realm + ":" +
                      _options.password);
        // return hex::encode(engine.digest());
        // std::string key(crypto::hash("MD5", _options.username + ":" + _realm
        // + ":" + _options.password));
        STrace << "Generating HMAC: data="
               << (_options.username + ":" + _realm + ":" + _options.password)
               << ", key=" << engine.digestStr() << endl;
        auto integrityAttr = new stun::MessageIntegrity;
        integrityAttr->setKey(engine.digestStr());
        request.add(integrityAttr);
    }
}


bool Client::sendAuthenticatedTransaction(stun::Transaction* transaction)
{
    authenticateRequest(transaction->request());
    STrace << "Send authenticated transaction: "
           << transaction->request().toString() << endl;
    return transaction->send();
}


stun::Transaction* Client::createTransaction(const net::Socket::Ptr& socket)
{

    // socket = socket ? socket : _socket;
    // assert(socket && !socket->isNull());
    auto transaction = new stun::Transaction(
        socket ? socket : _socket.impl, _options.serverAddr, _options.timeout, 1);
    transaction->StateChange += slot(this, &Client::onTransactionProgress);
    _transactions.push_back(transaction);
    return transaction;
}


bool Client::handleResponse(const stun::Message& response)
{
    LTrace("Handle response: ", response.toString())

    // Send this response to the appropriate handler.
    if (response.methodType() == stun::Message::Allocate) {
        if (response.classType() == stun::Message::SuccessResponse)
            handleAllocateResponse(response);

        else if (response.classType() == stun::Message::ErrorResponse)
            handleAllocateErrorResponse(response);

        // Must be a Transaction response
        else
            assert(0 && "no response state");
    }

    else if (response.methodType() == stun::Message::Refresh)
        handleRefreshResponse(response);

    // else if (response.methodType() ==  stun::Message::Refresh &&
    //    response.classType() == stun::Message::ErrorResponse)&&
    //    response.classType() == stun::Message::SuccessResponse
    //    handleRefreshErrorResponse(response);

    else if (response.methodType() == stun::Message::CreatePermission &&
             response.classType() == stun::Message::SuccessResponse)
        handleCreatePermissionResponse(response);

    else if (response.methodType() == stun::Message::CreatePermission &&
             response.classType() == stun::Message::ErrorResponse)
        handleCreatePermissionErrorResponse(response);

    else if (response.methodType() == stun::Message::DataIndication)
        handleDataIndication(response);

    else
        return false;

    return true;
}


void Client::sendAllocate()
{
    LTrace("Send allocation request")

    assert(!_options.username.empty());
    assert(!_options.password.empty());
    // assert(_options.lifetime);
    //_lifetime = _options.lifetime;

    // The client forms an Allocate request as follows.
    //
    // The client first picks a host transport address. It is RECOMMENDED
    // that the client pick a currently unused transport address, typically
    // by allowing the underlying OS to pick a currently unused port for a
    // new socket.
    //
    // The client then picks a transport protocol to use between the client
    // and the server. The transport protocol MUST be one of UDP, TCP, or
    // TLS-over-TCP. Since this specification only allows UDP between the
    // server and the peers, it is RECOMMENDED that the client pick UDP
    // unless it has a reason to use a different transport. One reason to
    // pick a different transport would be that the client believes, either
    // through configuration or by experiment, that it is unable to contact
    // any TURN server using UDP. See Section 2.1 for more discussion.
    //
    // The client also picks a server transport address, which SHOULD be
    // done as follows. The client receives (perhaps through configuration)
    // a domain name for a TURN server. The client then uses the DNS
    // procedures described in [RFC5389], but using an SRV service name of
    // "turn" (or "turns" for TURN over TLS) instead of "stun" (or "stuns").
    // For example, to find servers in the example.com domain, the client
    // performs a lookup for '_turn._udp.example.com',
    // '_turn._tcp.example.com', and '_turns._tcp.example.com' if the client
    // wants to communicate with the server using UDP, TCP, or TLS-over-TCP,
    // respectively.
    //
    auto transaction = createTransaction();
    // stun::Message request(stun::Message::Request, stun::Message::Allocate);
    transaction->request().setClass(stun::Message::Request);
    transaction->request().setMethod(stun::Message::Allocate);

    // The client MUST include a REQUESTED-TRANSPORT attribute in the
    // request. This attribute specifies the transport protocol between the
    // server and the peers (note that this is NOT the transport protocol
    // that appears in the 5-tuple).
    //
    auto transportAttr = new stun::RequestedTransport;
    transportAttr->setValue(transportProtocol() << 24);
    transaction->request().add(transportAttr);

    // If the client wishes the server to initialize the time-to-expiry
    // field of the allocation to some value other than the default
    // lifetime, then it MAY include a LIFETIME attribute specifying its
    // desired value. This is just a request, and the server may elect to
    // use a different value. Note that the server will ignore requests to
    // initialize the field to less than the default value.
    //
    if (_options.lifetime) {
        auto lifetimeAttr = new stun::Lifetime;
        lifetimeAttr->setValue((uint32_t)_options.lifetime / 1000);
        transaction->request().add(lifetimeAttr);
    }

    // If the client wishes to later use the DONT-FRAGMENT attribute in one
    // or more Send indications on this allocation, then the client SHOULD
    // include the DONT-FRAGMENT attribute in the Allocate
    // transaction->request(). This
    // allows the client to test whether this attribute is supported by the
    // server.
    //
    // If the client requires the port number of the relayed transport
    // address be even, the client includes the EVEN-PORT attribute. If
    // this attribute is not included, then the port can be even or odd. By
    // setting the R bit in the EVEN-PORT attribute to 1, the client can
    // request that the server reserve the next highest port number (on the
    // same IP address) for a subsequent allocation. If the R bit is 0, no
    // such request is made.
    //
    // The client MAY also include a RESERVATION-TOKEN attribute in the
    // request to ask the server to use a previously reserved port for the
    // allocation. If the RESERVATION-TOKEN attribute is included, then the
    // client MUST omit the EVEN-PORT attribute.
    //
    // Once constructed, the client sends the Allocate request on the
    // 5-tuple.
    //

    sendAuthenticatedTransaction(transaction);
}


//  At this point the response has already been authenticated, but we
//  have not checked for existing allocations on this 5 tuple.
void Client::handleAllocateResponse(const stun::Message& response)
{
    LTrace("Allocate success response")
    assert(response.methodType() == stun::Message::Allocate);

    // If the client receives an Allocate success response, then it MUST
    // check that the mapped address and the relayed transport address are
    // in an address family that the client understands and is prepared to
    // handle. This specification only covers the case where these two
    // addresses are IPv4 addresses. If these two addresses are not in an
    // address family which the client is prepared to handle, then the
    // client MUST delete the allocation (Section 7) and MUST NOT attempt to
    // create another allocation on that server until it believes the
    // mismatch has been fixed.
    //
    //    The IETF is currently considering mechanisms for transitioning
    //    between IPv4 and IPv6 that could result in a client originating an
    //    Allocate request over IPv6, but the request would arrive at the
    //    server over IPv4, or vice versa.
    //
    // Otherwise, the client creates its own copy of the allocation data
    // structure to track what is happening on the server. In particular,
    // the client needs to remember the actual lifetime received back from
    // the server, rather than the value sent to the server in the request.
    //
    auto lifetimeAttr = response.get<stun::Lifetime>();
    if (!lifetimeAttr) {
        assert(0);
        return;
    }

    auto mappedAttr = response.get<stun::XorMappedAddress>();
    if (!mappedAttr || mappedAttr->family() != 1) {
        assert(0);
        return;
    }
    _mappedAddress = mappedAttr->address();

    // The client must also remember the 5-tuple used for the request and
    // the username and password it used to authenticate the request to
    // ensure that it reuses them for subsequent messages. The client also
    // needs to track the channels and permissions it establishes on the
    // server.
    //
    // The client will probably wish to send the relayed transport address
    // to peers (using some method not specified here) so the peers can
    // communicate with it. The client may also wish to use the server-
    // reflexive address it receives in the XOR-MAPPED-ADDRESS attribute in
    // its ICE processing.
    //
    auto relayedAttr = response.get<stun::XorRelayedAddress>();
    if (!relayedAttr || (relayedAttr && relayedAttr->family() != 1)) {
        assert(0);
        return;
    }

    if (relayedAttr->address().host() == "0.0.0.0") {
        assert(0 && "invalid loopback address");
        return;
    }

    // Use the relay server host and relayed port
    _relayedAddress = net::Address(
        relayedAttr->address().host(),
        relayedAttr->address().port()); //_options.serverAddr.host()

    STrace << "Allocation created:"
           << "\n\tRelayed address: " << _relayedAddress //.toString()
           << "\n\tMapped address: " << _mappedAddress   //.toString()
           << "\n\tLifetime: " << lifetimeAttr->value() << endl;

    // Once the allocation is created we transition to Authorizing while
    // peer permissions are created.
    setState(this, ClientState::Authorizing);

    // If the permission list has entries create them now.
    // A successful response here will set the client state to Success.
    if (!closed())
        sendCreatePermission();
}


void Client::handleAllocateErrorResponse(const stun::Message& response)
{
    LTrace("Allocate error response")

    assert(response.methodType() == stun::Message::Allocate &&
           response.classType() == stun::Message::ErrorResponse);

    auto errorAttr = response.get<stun::ErrorCode>();
    if (!errorAttr) {
        assert(0);
        return;
    }

    STrace << "Allocation error response: " << errorAttr->errorCode() << ": "
           << errorAttr->reason() << endl;

    // If the client receives an Allocate error response, then the
    // processing depends on the actual error code returned:

    // o  (Request timed out): There is either a problem with the server, or
    //    a problem reaching the server with the chosen transport. The
    //    client considers the current transaction as having failed but MAY
    //    choose to retry the Allocate request using a different transport
    //    (e.g., TCP instead of UDP).

    switch (errorAttr->errorCode()) {
        // 300 (Try Alternate): The server would like the client to use the
        // server specified in the ALTERNATE-SERVER attribute instead. The
        // client considers the current transaction as having failed, but
        // SHOULD try the Allocate request with the alternate server before
        // trying any other servers (e.g., other servers discovered using the
        // SRV procedures). When trying the Allocate request with the
        // alternate server, the client follows the ALTERNATE-SERVER
        // procedures specified in [RFC5389].
        case 300:
            // Return the error to the client.
            assert(0);
            break;

        // 400 (Bad Request): The server believes the client's request is
        // malformed for some reason. The client considers the current
        // transaction as having failed. The client MAY notify the client or
        // operator and SHOULD NOT retry the request with this server until
        // it believes the problem has been fixed.
        case 400:
            // Return the error to the client.
            assert(0);
            break;

        // 401 (NotAuthorized): If the client has followed the procedures of
        // the long-term credential mechanism and still gets this error, then
        // the server is not accepting the client's credentials. In this
        // case, the client considers the current transaction as having
        // failed and SHOULD notify the client or operator. The client SHOULD
        // NOT send any further requests to this server until it believes the
        // problem has been fixed.
        case 401: {

            if (_realm.empty() || _nonce.empty()) {

                // REALM
                const stun::Realm* realmAttr = response.get<stun::Realm>();
                if (realmAttr) {
                    _realm = realmAttr->asString();
                }

                // NONCE
                const stun::Nonce* nonceAttr = response.get<stun::Nonce>();
                if (nonceAttr) {
                    _nonce = nonceAttr->asString();
                }

                // Now that our realm and nonce are set we can re-send the
                // allocate request.
                if (_realm.size() && _nonce.size()) {
                    LTrace("Resending authenticated allocation request")
                    sendAllocate();
                    return;
                }
            }
        }

        break;

        // 403 (Forbidden): The request is valid, but the server is refusing
        // to perform it, likely due to administrative restrictions. The
        // client considers the current transaction as having failed. The
        // client MAY notify the client or operator and SHOULD NOT retry the
        // same request with this server until it believes the problem has
        // been fixed.
        case 403:
            // Return the error to the client.
            assert(0);
            break;

        // 420 (Unknown Attribute): If the client included a DONT-FRAGMENT
        // attribute in the request and the server rejected the request with
        // a 420 error code and listed the DONT-FRAGMENT attribute in the
        // UNKNOWN-ATTRIBUTES attribute in the error response, then the
        // client now knows that the server does not support the DONT-
        // FRAGMENT attribute. The client considers the current transaction
        // as having failed but MAY choose to retry the Allocate request
        // without the DONT-FRAGMENT attribute.
        case 420:
            assert(0);
            break;

        // 437 (Allocation Mismatch): This indicates that the client has
        // picked a 5-tuple that the server sees as already in use. One way
        // this could happen is if an intervening NAT assigned a mapped
        // transport address that was used by another client that recently
        // crashed. The client considers the current transaction as having
        // failed. The client SHOULD pick another client transport address
        // and retry the Allocate request (using a different transaction id).
        // The client SHOULD try three different client transport addresses
        // before giving up on this server. Once the client gives up on the
        // server, it SHOULD NOT try to create another allocation on the
        // server for 2 minutes.
        case 437:
            assert(0);
            break;

        // 438 (Stale Nonce): See the procedures for the long-term credential
        // mechanism [RFC5389].
        case 438:
            assert(0);
            break;

        // 441 (Wrong Credentials): The client should not receive this error
        // in response to a Allocate request. The client MAY notify the client
        // or operator and SHOULD NOT retry the same request with this server
        // until it believes the problem has been fixed.
        case 441:
            assert(0);
            break;

        // 442 (Unsupported Transport Address): The client should not receive
        // this error in response to a request for a UDP allocation. The
        // client MAY notify the client or operator and SHOULD NOT reattempt
        // the request with this server until it believes the problem has
        // been fixed.
        case 442:
            assert(0);
            break;

        // 486 (Allocation Quota Reached): The server is currently unable to
        // create any more allocations with this username. The client
        // considers the current transaction as having failed. The client
        // SHOULD wait at least 1 minute before trying to create any more
        // allocations on the server.
        case 486:
            // controlConn.disconnect();
            // controlConn.startConnectionTimer(60000);
            // assert(0);
            break;

        // 508 (Insufficient Capacity): The server has no more relayed
        // transport addresses available, or has none with the requested
        // properties, or the one that was reserved is no longer available.
        // The client considers the current operation as having failed. If
        // the client is using either the EVEN-PORT or the RESERVATION-TOKEN
        // attribute, then the client MAY choose to remove or modify this
        // attribute and try again immediately. Otherwise, the client SHOULD
        // wait at least 1 minute before trying to create any more
        // allocations on this server.
        case 508:
            // controlConn.disconnect();
            // controlConn.startConnectionTimer(60000);
            // assert(0);
            break;

        // An unknown error response MUST be handled as described in [RFC5389].
        default:
            assert(0);
            break;
    }

    setError(util::format("(%d) %s", (int)errorAttr->errorCode(),
                          errorAttr->reason().c_str()));

    if (!closed()) {
        _observer.onAllocationFailed(
            *this, errorAttr->errorCode(),
            errorAttr->reason()); // may result in instance deletion
    }
}


void Client::setError(const scy::Error& error)
{
    _error = error;
    setState(this, ClientState::Failed);
}


void Client::addPermission(const IPList& peerIPs)
{
    for (auto it = peerIPs.begin(); it != peerIPs.end(); ++it) {
        addPermission(*it);
    }
}


void Client::addPermission(const std::string& peerIP)
{
    IAllocation::addPermission(peerIP);
}


void Client::sendCreatePermission()
{
    LTrace("Send Create Permission Request")

    assert(!_permissions.empty());

    // The client who wishes to install or refresh one or more permissions
    // can send a CreatePermission request to the server.
    //
    // When forming a CreatePermission request, the client MUST include at
    // least one XOR-PEER-ADDRESS attribute, and MAY include more than one
    // such attribute. The IP address portion of each XOR-PEER-ADDRESS
    // attribute contains the IP address for which a permission should be
    // installed or refreshed. The port portion of each XOR-PEER-ADDRESS
    // attribute will be ignored and can be any arbitrary value. The
    // various XOR-PEER-ADDRESS attributes can appear in any order.

    auto transaction = createTransaction();
    // stun::Message request(stun::Message::Request, stun::Message::Allocate);
    transaction->request().setClass(stun::Message::Request);
    transaction->request().setMethod(stun::Message::CreatePermission);

    for (auto it = _permissions.begin(); it != _permissions.end(); ++it) {
        LTrace("Create permission request: ", (*it).ip)
        auto peerAttr = new stun::XorPeerAddress;
        peerAttr->setAddress(net::Address((*it).ip, 0));
        // peerAttr->setFamily(1);
        // peerAttr->setPort(0);
        // peerAttr->setIP((*it).ip);
        transaction->request().add(peerAttr);
    }

    sendAuthenticatedTransaction(transaction);
}


void Client::handleCreatePermissionResponse(const stun::Message& /* response */)
{
    // If the client receives a valid CreatePermission success response,
    // then the client updates its data structures to indicate that the
    // permissions have been installed or refreshed.
    LTrace("Permission created")

    // Send all queued requests...
    // TODO: To via onStateChange Success callback
    {

        while (!_pendingIndications.empty()) {
            _socket->sendPacket(_pendingIndications.front());
            _pendingIndications.pop_front();
        }
    }

    if (!closed()) {
        _observer.onAllocationPermissionsCreated(*this, _permissions);

        // auto transaction =
        // reinterpret_cast<stun::Transaction*>(response.opaque);
        // for (int i = 0; i < 100; i++) {
        //     auto peerAttr = transaction->request().get<stun::XorPeerAddress>(i);
        //     if (!peerAttr || (peerAttr && peerAttr->family() != 1))
        //         break;
        //     _observer.onAllocationPermissionsCreated(*this,
        // std::string(peerAttr->address().host()));
        // }
    }

    // Once permissions have been created the allocation
    // process is considered a success.
    LTrace("Allocation Success")
    setState(this, ClientState::Success);
}


void Client::handleCreatePermissionErrorResponse(const stun::Message& /* response */)
{
    LWarn("Permission Creation Failed")

    removeAllPermissions();

    setError("Cannot create server permissions.");
}


void Client::sendChannelBind(const std::string& /* peerIP */)
{
    // A channel binding is created or refreshed using a ChannelBind
    // transaction. A ChannelBind transaction also creates or refreshes a
    // permission towards the peer (see Section 8).
    //
    // To initiate the ChannelBind transaction, the client forms a
    // ChannelBind request. The channel to be bound is specified in a
    // CHANNEL-NUMBER attribute, and the peer's transport address is
    // specified in an XOR-PEER-ADDRESS attribute. Section 11.2 describes
    // the restrictions on these attributes.
    //
    // Rebinding a channel to the same transport address that it is already
    // bound to provides a way to refresh a channel binding and the
    // corresponding permission without sending data to the peer. Note
    // however, that permissions need to be refreshed more frequently than
    // channels.
    assert(0 && "not implemented");
}


void Client::sendData(const char* data, size_t size, const net::Address& peerAddress)
{
    LTrace("Send Data Indication to peer: ", peerAddress)

    // auto request = new stun::Message;
    stun::Message request;
    request.setClass(stun::Message::Indication);
    request.setMethod(stun::Message::SendIndication);

    // The client can use a Send indication to pass data to the server for
    // relaying to a peer. A client may use a Send indication even if a
    // channel is bound to that peer. However, the client MUST ensure that
    // there is a permission installed for the IP address of the peer to
    // which the Send indication is being sent; this prevents a third party
    // from using a TURN server to send data to arbitrary destinations.
    //
    // When forming a Send indication, the client MUST include an XOR-PEER-
    // ADDRESS attribute and a DATA attribute. The XOR-PEER-ADDRESS
    // attribute contains the transport address of the peer to which the
    // data is to be sent, and the DATA attribute contains the actual
    // application data to be sent to the peer.
    //
    // The client MAY include a DONT-FRAGMENT attribute in the Send
    // indication if it wishes the server to set the DF bit on the UDP
    // datagram sent to the peer.

    auto peerAttr = new stun::XorPeerAddress;
    peerAttr->setAddress(peerAddress);
    // peerAttr->setFamily(1);
    // peerAttr->setPort(peerAddress.port());
    // peerAttr->setIP(peerAddress.host());
    request.add(peerAttr);

    auto dataAttr = new stun::Data;
    dataAttr->copyBytes(data, size);
    request.add(dataAttr);

    // Ensure permissions exist for the peer.
    if (!hasPermission(peerAddress.host())) {
        // delete request;
        throw std::runtime_error("No permission exists for peer IP: " + peerAddress.host());
    }

    // If permission exists and is currently being negotiated with
    // the server then queue the outgoing request.
    // Queued requests will be sent when the CreatePermission
    // callback is received from the server.
    else if (stateEquals(ClientState::Authorizing)) {
        LTrace("Queueing outgoing request: ", request.toString())

        _pendingIndications.push_back(request);
        assert(_pendingIndications.size() < 100); // something is wrong...
    }

    // If a permission exists on server and client send our data!
    else {
        _socket->sendPacket(request, _options.serverAddr);
        // delete request;
    }
}


void Client::handleDataIndication(const stun::Message& response)
{
    // When the client receives a Data indication, it checks that the Data
    // indication contains both an XOR-PEER-ADDRESS and a DATA attribute,
    // and discards the indication if it does not. The client SHOULD also
    // check that the XOR-PEER-ADDRESS attribute value contains an IP
    // address with which the client believes there is an active permission,
    // and discard the Data indication otherwise. Note that the DATA
    // attribute is allowed to contain zero bytes of data.
    //
    //    NOTE: The latter check protects the client against an attacker who
    //    somehow manages to trick the server into installing permissions
    //    not desired by the client.
    //
    // If the Data indication passes the above checks, the client delivers
    // the data octets inside the DATA attribute to the application, along
    // with an indication that they were received from the peer whose
    // transport address is given by the XOR-PEER-ADDRESS attribute.

    auto peerAttr = response.get<stun::XorPeerAddress>();
    if (!peerAttr || (peerAttr && peerAttr->family() != 1)) {
        assert(0);
        return;
    }

    auto dataAttr = response.get<stun::Data>();
    if (!dataAttr) {
        assert(0);
        return;
    }

    LTrace("Handle Data indication: ", response.toString())

    if (!closed()) {
        _observer.onRelayDataReceived(*this, dataAttr->bytes(), dataAttr->size(), peerAttr->address());
    }
}


void Client::onTransactionProgress(void* sender, TransactionState& state, const TransactionState&)
{
    LTrace("Transaction state change: ", sender, ": ", state)

    auto transaction = reinterpret_cast<stun::Transaction*>(sender);
    transaction->response().opaque = transaction;

    if (!closed())
        _observer.onTransactionResponse(*this, *transaction);

    switch (state.id()) {
        case TransactionState::Running:
            return;

        case TransactionState::Success: {
            STrace << "STUN transaction success:"
                   << "\n\tState: " << state.toString()
                   << "\n\tFrom: " << transaction->peerAddress().toString()
                   << "\n\tRequest: " << transaction->request().toString()
                   << "\n\tResponse: " << transaction->response().toString()
                   << endl;

            if (removeTransaction(transaction)) {
                if (!handleResponse(transaction->response())) {
                    STrace << "Unhandled STUN response: "
                           << transaction->response().toString() << endl;
                }
            }
        } break;

        case TransactionState::Failed:
            SWarn << "STUN transaction error:"
                  << "\n\tState: " << state.toString()
                  << "\n\tFrom: " << transaction->peerAddress().toString()
                  << "\n\tData: " << transaction->response().toString() << endl;

            // TODO: More flexible response error handling
            if (removeTransaction(transaction)) {
                setError("Transaction failed");
            }
            break;
    }
}


void Client::onTimer()
{
    if (expired())
        // Attempt to re-allocate
        sendAllocate();

    else if (timeRemaining() < lifetime() * 0.33)
        sendRefresh();

    _observer.onTimer(*this);
}


void Client::onStateChange(ClientState& state, const ClientState& oldState)
{
    _observer.onClientStateChange(*this, state, oldState);
}


int Client::transportProtocol()
{
    return 17; // UDP
}


bool Client::closed() const
{
    return stateEquals(ClientState::None) || stateEquals(ClientState::Failed);
}


Client::Options& Client::options()
{
    return _options;
}


net::Address Client::mappedAddress() const
{
    return _mappedAddress;
}


net::Address Client::relayedAddress() const
{
    return _relayedAddress;
}


} } // namespace scy::turn


/// @\}
