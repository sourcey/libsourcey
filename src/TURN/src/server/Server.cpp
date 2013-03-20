//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is is distributed under a dual license that allows free, 
// open source use and closed source use under a standard commercial
// license.
//
// Non-Commercial Use:
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// Commercial Use:
// Please contact mail@sourcey.com
//


#include "Sourcey/TURN/server/Server.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Buffer.h"


using namespace std;
using namespace Poco;
using namespace Sourcey::Net;


namespace Sourcey {
namespace TURN {


Server::Server(IServerObserver& observer, Net::Reactor& reactor, Runner& runner, const Options& options) : 
	_socketUDP(reactor, runner),
	_socketTCP(reactor),
	_observer(observer),
	_options(options),
	_reactor(reactor),
	_runner(runner),
	_timer(new TimerTask(runner, 5000, 5000))
{
	log("trace") << "Creating" << endl;
}


Server::~Server() 
{
	////Timer::getDefault().stop(TimerCallback<Server>(this, &ServerAllocation::onTimer));
	//_socketUDP.detach(packetDelegate<Server, STUN::Message>(this, &Server::onPacketReceived, 1));
	log("trace") << "Destroying" << endl;	
	stop();	
	_timer->destroy();	
	log("trace") << "Destroying: OK" << endl;
}


void Server::start()
{
	log("trace") << "Binding" << endl;	

	// Initialize UDP
	_socketUDP.registerPacketType<STUN::Message>(1);
	_socketUDP.attach(packetDelegate<Server, STUN::Message>(this, &Server::onPacketReceived, 1));
	_socketUDP.bind(_options.listenAddr);

	log("info") << "Listening on " << _socketUDP.address().toString() << endl;
	
	// Initialize TCP
	if (_options.enableTCP) {
		_socketTCP.bind(_options.listenAddr);
		_socketTCP.SocketAccepted += delegate(this, &Server::onTCPConnectionAccepted);
	}	
	
	//Timer::getDefault().start(TimerCallback<Server>(this, &Server::onTimer, 5000, 5000));
	//TimerTask* timer = new TimerTask(_runner, 5000, 5000);
	_timer->Timeout += delegate(this, &Server::onTimer);
	_timer->start();
}


void Server::stop()
{
	log("trace") << "Stopping" << endl;	
	//Timer::getDefault().stop(TimerCallback<Server>(this, &Server::onTimer));	
	_timer->cancel();

	if (_options.enableTCP) {
		_socketTCP.close();
	}
	_socketUDP.close();
}


void Server::onTimer(void*) //TimerCallback<Server>& timer
{
	//log("trace") << "On Timer" << endl;	

	ServerAllocationMap allocations = this->allocations();
	for (ServerAllocationMap::iterator it = allocations.begin(); it != allocations.end(); ++it) {
		if (!it->second->onTimer())
			delete it->second;
	}
}


void Server::onTCPConnectionAccepted(void* sender, Poco::Net::StreamSocket& sock, Net::Reactor& reactor)
{
	log("trace") << "TCP Connection Accepted: " << sock.peerAddress().toString() << endl;	
	
	TCPPacketSocket* socket = new TCPPacketSocket(sock, reactor); //, reactor()
	socket->setDeleteOnClose(true);
	socket->registerPacketType<STUN::Message>(1);
	socket->attach(packetDelegate(this, &Server::onPacketReceived));
	//socket->Closed += delegate(this, &Server::onTCPConnectionClosed, -1); // lowest priority
}


/*
void Server::onTCPConnectionClosed(void* sender)
{
	TCPPacketSocket* socket = reinterpret_cast<TCPPacketSocket*>(sender);

	log("trace") << "TCP Connection Closed: " << socket->peerAddress() << endl;	
	
	// NOTE: Terminate associated allocation
	//FiveTuple tuple(socket->peerAddress(), socket->localAddress(), socket->transport());
	//ServerAllocation* allocation = getAllocation(tuple);
	//if (allocation) {
	//}	

	delete socket;
}
*/


void Server::onPacketReceived(void* sender, STUN::Message& message) 
{
	log("trace") << "STUN Packet Received: " << message.toString() << endl;	

	assert(message.state() == STUN::Message::Request);
	
	Net::PacketInfo* source = reinterpret_cast<Net::PacketInfo*>(message.info);
	assert(source);
	if (!source)
		return;

	Request request(source->socket, message, source->localAddress, source->peerAddress);
	AuthenticationState state = _observer.authenticateRequest(this, request);
	handleRequest(request, state);
	
	/*, state
	//handleRequest(request);
	_observer.authenticateRequest(this, request)
	if (state != Authenticating)
	// NOTE: It is the job of the Server Observer to call the
	// handleAuthorizedRequest method.
	AuthenticationState result = _observer.authenticateRequest(this, request);
	switch (result) {
		case Authenticating: 
			log("trace") << "STUN Request Authenticating" << endl;
			break;

		case Authorized: 
			log("trace") << "STUN Request Authorized" << endl;
			handleAuthorizedRequest(request);
			break;

		case Unauthorized: 
			log("trace") << "Unauthorized STUN Request" << endl;
			sendError(request, 401, "Unauthorized");
			break;
	}

	if (_observer->authenticate(request) == Unauthorized)
		return;
	}
	else {
		log("trace") << "Unauthorized STUN Request" << endl;
		sendError(request, 401, "Unauthorized");
		return;
	}
	*/
}


void Server::handleRequest(const Request& request, AuthenticationState state)
{	
	log("trace") << "STUN Request Received:\n" 
		<< "\tFrom: " << request.remoteAddr.toString() << "\n"
		<< "\tData: " << request.toString()
		<< endl;

	switch (state) {
		case Authenticating: 
			// await async response
			break;

		case Authorized: 
			handleAuthorizedRequest(request);
			break;

		case QuotaReached:
			sendError(request, 486, "Allocation Quota Reached");
			break;

		case Unauthorized: 
			sendError(request, 401, "Unauthorized");
			break;
	}
}


void Server::handleAuthorizedRequest(const Request& request) //, AuthenticationState state
{		
	// All requests after the initial Allocate must use the same username as
	// that used to create the allocation, to prevent attackers from
	// hijacking the client's allocation.  Specifically, if the server
	// requires the use of the long-term credential mechanism, and if a non-
	// Allocate request passes authentication under this mechanism, and if
	// the 5-tuple identifies an existing allocation, but the request does
	// not use the same username as used to create the allocation, then the
	// request MUST be rejected with a 441 (Wrong Credentials) error.
	// 
	// When a TURN message arrives at the server from the client, the server
	// uses the 5-tuple in the message to identify the associated
	// allocation.  For all TURN messages (including ChannelData) EXCEPT an
	// Allocate request, if the 5-tuple does not identify an existing
	// allocation, then the message MUST either be rejected with a 437
	// Allocation Mismatch error (if it is a request) or silently ignored
	// (if it is an indication or a ChannelData message).  A client
	// receiving a 437 error response to a request other than Allocate MUST
	// assume the allocation no longer exists.

	switch (request.type()) {
		case STUN::Message::Binding: 
			handleBindingRequest(request);
			break;

		case STUN::Message::Allocate: 
			handleAllocateRequest(request);
			break;

		case STUN::Message::ConnectionBind: 
			handleConnectionBindRequest(request);
			break;

		default: {
			FiveTuple tuple(request.remoteAddr, request.localAddress, request.socket.transport());
			ServerAllocation* allocation = getAllocation(tuple); //reinterpret_cast<ServerAllocation*>();
			if (!allocation)  {
				sendError(request, 437, "Allocation Mismatch");
				return;
			}			

			log("trace") << "Obtained allocation: " 
				<< allocation->tuple().toString() << endl;
					
			if (!allocation->handleRequest(request))
				sendError(request, 600, "Operation Not Supported");
		}
	}
}


void Server::handleConnectionBindRequest(const Request& request)
{
	const STUN::ConnectionID* connAttr = request.get<STUN::ConnectionID>();
	if (!connAttr) {		
		sendError(request, 400, "Bad Request");
	}

	TCPAllocation* alloc = getTCPAllocation(connAttr->value());
	if (!connAttr) {
		sendError(request, 400, "Bad Request");
	}

	if (alloc)
		alloc->handleConnectionBindRequest(request);
}


void Server::handleBindingRequest(const Request& request) 
{
	log("trace") << "Handle Binding Request" << endl;

	assert(request.type() == STUN::Message::Binding);
	assert(request.state() == STUN::Message::Request);

	STUN::Message response;
	response.setType(STUN::Message::Binding);
	response.setTransactionID(request.transactionID());

	// XOR-MAPPED-ADDRESS
	STUN::XorMappedAddress* addrAttr = new STUN::XorMappedAddress;
	addrAttr->setFamily(1);
	addrAttr->setPort(request.remoteAddr.port());
	addrAttr->setIP(request.remoteAddr.host().toString());
	response.add(addrAttr);
  
	request.socket.send(response, request.remoteAddr);
}


void Server::handleAllocateRequest(const Request& request) 
{
	log("trace") << "Handle Allocate Request" << endl;

	assert(request.type() == STUN::Message::Allocate);
	assert(request.state() == STUN::Message::Request);

	// When the server receives an Allocate request, it performs the
	// following checks:
	// 
	// 1.  The server MUST require that the request be authenticated.  This
	//     authentication MUST be done using the long-term credential
	//     mechanism of [RFC5389] unless the client and server agree to use
	//     another mechanism through some procedure outside the scope of
	//     this document.
	// 
	const STUN::Username* usernameAttr = request.get<STUN::Username>();
	if (!usernameAttr) {
		log("trace") << "Unauthorized STUN Request" << endl;
		sendError(request, 401, "Unauthorized");
		return;
	}

	string username(usernameAttr->asString());	

	// 2.  The server checks if the 5-tuple is currently in use by an
	//     existing allocation.  If yes, the server rejects the request with
	//     a 437 (ServerAllocation Mismatch) error.

	// 3.  The server checks if the request contains a REQUESTED-TRANSPORT
	//     attribute.  If the REQUESTED-TRANSPORT attribute is not included
	//     or is malformed, the server rejects the request with a 400 (Bad
	//     Request) error.  Otherwise, if the attribute is included but
	//     specifies a protocol other that UDP, the server rejects the
	//     request with a 442 (Unsupported Transport Protocol) error.
	// 
	const STUN::RequestedTransport* transportAttr = request.get<STUN::RequestedTransport>();
	if (!transportAttr) {
		LogError() << "No Requested Transport" << endl;
		sendError(request, 400, "Bad Request");
		return;
	}
		
	if (transportAttr->value() != 6 &&
		transportAttr->value() != 17) {
		LogError() << "Requested Transport is neither TCP or UDP: " << transportAttr->value() << endl;
		sendError(request, 422, "Unsupported Transport Protocol");
		return;
	}

	FiveTuple tuple(request.remoteAddr, request.localAddress, transportAttr->value() == 17 ? Net::UDP : Net::TCP);
	if (getAllocation(tuple))  {
		sendError(request, 437, "Allocation Mismatch");
		return;
	} 

	// 4.  The request may contain a DONT-FRAGMENT attribute.  If it does,
	//     but the server does not support sending UDP datagrams with the DF
	//     bit set to 1 (see Section 12), then the server treats the DONT-
	//     FRAGMENT attribute in the Allocate request as an unknown
	//     comprehension-required attribute.

	// 5.  The server checks if the request contains a RESERVATION-TOKEN
	//     attribute.  If yes, and the request also contains an EVEN-PORT
	//     attribute, then the server rejects the request with a 400 (Bad
	//     Request) error.  Otherwise, it checks to see if the token is
	//     valid (i.e., the token is in range and has not expired and the
	//     corresponding relayed transport address is still available).  If
	//     the token is not valid for some reason, the server rejects the
	//     request with a 508 (Insufficient Capacity) error.

	// 6.  The server checks if the request contains an EVEN-PORT attribute.
	//     If yes, then the server checks that it can satisfy the request
	//     (i.e., can allocate a relayed transport address as described
	//     below).  If the server cannot satisfy the request, then the
	//     server rejects the request with a 508 (Insufficient Capacity)
	//     error.

	// 7.  At any point, the server MAY choose to reject the request with a
	//     486 (ServerAllocation Quota Reached) error if it feels the client is
	//     trying to exceed some locally defined allocation quota.  The
	//     server is free to define this allocation quota any way it wishes,
	//     but SHOULD define it based on the username used to authenticate
	//     the request, and not on the client's transport address.

	// 8.  Also at any point, the server MAY choose to reject the request
	//     with a 300 (Try Alternate) error if it wishes to redirect the
	//     client to a different server.  The use of this error code and
	//     attribute follow the specification in [RFC5389].

	// Compute the appropriate LIFETIME for this allocation.
	UInt32 lifetime = min(options().allocationMaxLifetime / 1000, options().allocationDefaultLifetime / 1000);
	const STUN::Lifetime* lifetimeAttr = request.get<STUN::Lifetime>();
	if (lifetimeAttr)
		lifetime = min(lifetime, lifetimeAttr->value());

	ServerAllocation* allocation = NULL;

	// Protocol specific allocation handling. 6 = TCP, 17 = UDP.
	if (transportAttr->value() == 17) {		// UDP

		// If all the checks pass, the server creates the allocation.  The
		// 5-tuple is set to the 5-tuple from the Allocate request, while the
		// list of permissions and the list of channels are initially empty.

		// The server chooses a relayed transport address for the allocation as
		// follows:

		// o  If the request contains a RESERVATION-TOKEN, the server uses the
		//    previously reserved transport address corresponding to the
		//    included token (if it is still available).  Note that the
		//    reservation is a server-wide reservation and is not specific to a
		//    particular allocation, since the Allocate request containing the
		//    RESERVATION-TOKEN uses a different 5-tuple than the Allocate
		//    request that made the reservation.  The 5-tuple for the Allocate
		//    request containing the RESERVATION-TOKEN attribute can be any
		//    allowed 5-tuple; it can use a different client IP address and
		//    port, a different transport protocol, and even different server IP
		//    address and port (provided, of course, that the server IP address
		//    and port are ones on which the server is listening for TURN
		//    requests).

		// o  If the request contains an EVEN-PORT attribute with the R bit set
		//    to 0, then the server allocates a relayed transport address with
		//    an even port number.

		// o  If the request contains an EVEN-PORT attribute with the R bit set
		//    to 1, then the server looks for a pair of port numbers N and N+1
		//    on the same IP address, where N is even.  Port N is used in the
		//    current allocation, while the relayed transport address with port
		//    N+1 is assigned a token and reserved for a future allocation.  The
		//    server MUST hold this reservation for at least 30 seconds, and MAY
		//    choose to hold longer (e.g., until the allocation with port N
		//    expires).  The server then includes the token in a RESERVATION-
		//    TOKEN attribute in the success response.

		// o  Otherwise, the server allocates any available relayed transport
		//    address.		

		// In all cases, the server SHOULD only allocate ports from the range
		// 49152 - 65535 (the Dynamic and/or Private Port range [Port-Numbers]),
		// unless the TURN server application knows, through some means not
		// specified here, that other applications running on the same host as
		// the TURN server application will not be impacted by allocating ports
		// outside this range.  This condition can often be satisfied by running
		// the TURN server application on a dedicated machine and/or by
		// arranging that any other applications on the machine allocate ports
		// before the TURN server application starts.  In any case, the TURN
		// server SHOULD NOT allocate ports in the range 0 - 1023 (the Well-
		// Known Port range) to discourage clients from using TURN to run
		// standard services.

		//    NOTE: The IETF is currently investigating the topic of randomized
		//    port assignments to avoid certain types of attacks (see
		//    [TSVWG-PORT]).  It is strongly recommended that a TURN implementor
		//    keep abreast of this topic and, if appropriate, implement a
		//    randomized port assignment algorithm.  This is especially
		//    applicable to servers that choose to pre-allocate a number of
		//    ports from the underlying OS and then later assign them to
		//    allocations; for example, a server may choose this technique to
		//    implement the EVEN-PORT attribute.

		// The server determines the initial value of the time-to-expiry field
		// as follows.  If the request contains a LIFETIME attribute, then the
		// server computes the minimum of the client's proposed lifetime and the
		// server's maximum allowed lifetime.  If this computed value is greater
		// than the default lifetime, then the server uses the computed lifetime
		// as the initial value of the time-to-expiry field.  Otherwise, the
		// server uses the default lifetime.  It is RECOMMENDED that the server
		// use a maximum allowed lifetime value of no more than 3600 seconds (1
		// hour).  Servers that implement allocation quotas or charge clients for
		// allocations in some way may wish to use a smaller maximum allowed
		// lifetime (perhaps as small as the default lifetime) to more quickly
		// remove orphaned allocations (that is, allocations where the
		// corresponding client has crashed or isTerminated or the client
		// IConnection has been lost for some reason).  Also, note that the time-
		// to-expiry is recomputed with each successful Refresh request, and
		// thus the value computed here applies only until the first refresh.

		// Find or create the allocation matching the 5-TUPLE. If the allocation
		// already exists then send an error.
		allocation = new UDPAllocation(*this, tuple, username, lifetime);
	} 
	
	else if (transportAttr->value() == 6) {	// TCP

		// 5.1. Receiving a TCP Allocate Request
		// 
		// 
		// The process is similar to that defined in [RFC5766], Section 6.2,
		// with the following exceptions:
		// 
		// 1.  If the REQUESTED-TRANSPORT attribute is included and specifies a
		//     protocol other than UDP or TCP, the server MUST reject the
		//     request with a 442 (Unsupported Transport Protocol) error.  If
		//     the value is UDP, and if UDP transport is allowed by local
		//     policy, the server MUST continue with the procedures of [RFC5766]
		//     instead of this document.  If the value is UDP, and if UDP
		//     transport is forbidden by local policy, the server MUST reject
		//     the request with a 403 (Forbidden) error.
		// 
		// 2.  If the client connection transport is not TCP or TLS, the server
		//     MUST reject the request with a 400 (Bad Request) error.
		// 
		// 3.  If the request contains the DONT-FRAGMENT, EVEN-PORT, or
		//     RESERVATION-TOKEN attribute, the server MUST reject the request
		//     with a 400 (Bad Request) error.
		// 
		// 4.  A TCP relayed transport address MUST be allocated instead of a
		//     UDP one.
		// 
		// 5.  The RESERVATION-TOKEN attribute MUST NOT be present in the
		//     success response.
		// 
		// If all checks pass, the server MUST start accepting incoming TCP
		// connections on the relayed transport address.  Refer to Section 5.3
		// for details.

		//Net::TCPPacketSocket& socket = static_cast<Net::TCPPacketSocket&>(request.socket);
		allocation = new TCPAllocation(*this, static_cast<Net::TCPPacketSocket*>(&request.socket), tuple, username, lifetime);
	} 

	// Once the allocation is created, the server replies with a success
	// response.  The success response contains:

	STUN::Message response;
	response.setType(STUN::Message::Allocate);
	response.setTransactionID(request.transactionID());

	// o  An XOR-RELAYED-ADDRESS attribute containing the relayed transport
	//    address.
	STUN::XorRelayedAddress* relayAddrAttr = new STUN::XorRelayedAddress;
	relayAddrAttr->setFamily(1);
	relayAddrAttr->setIP(options().listenAddr.host().toString());
	//relayAddrAttr->setIP(allocation->relayedAddress().host().toString());
	relayAddrAttr->setPort(allocation->relayedAddress().port());
	response.add(relayAddrAttr);

	// o  A LIFETIME attribute containing the current value of the time-to-
	//    expiry timer.
	STUN::Lifetime* resLifetimeAttr = new STUN::Lifetime;
	resLifetimeAttr->setValue(lifetime); // / 1000
	response.add(resLifetimeAttr);

	// o  A RESERVATION-TOKEN attribute (if a second relayed transport
	//    address was reserved).

	// o  An XOR-MAPPED-ADDRESS attribute containing the client's IP address
	//    and port (from the 5-tuple).

	//    NOTE: The XOR-MAPPED-ADDRESS attribute is included in the response
	//    as a convenience to the client.  TURN itself does not make use of
	//    this value, but clients running ICE can often need this value and
	//    can thus avoid having to do an extra Binding transaction with some
	//    STUN server to learn it. 
	STUN::XorMappedAddress* mappedAddressAttr = new STUN::XorMappedAddress;
	mappedAddressAttr->setFamily(1);
	mappedAddressAttr->setIP(request.remoteAddr.host().toString());
	mappedAddressAttr->setPort(request.remoteAddr.port());
	response.add(mappedAddressAttr);
	
	// The response (either success or error) is sent back to the client on
	// the 5-tuple.
	request.socket.send(response, request.remoteAddr);

	//    NOTE: When the Allocate request is sent over UDP, section 7.3.1 of
	//    [RFC5389] requires that the server handle the possible
	//    retransmissions of the request so that retransmissions do not
	//    cause multiple allocations to be created.  Implementations may
	//    achieve this using the so-called "stateless stack approach" as
	//    follows.  To detect retransmissions when the original request was
	//    successful in creating an allocation, the server can store the
	//    transaction id that created the request with the allocation data
	//    and compare it with incoming Allocate requests on the same
	//    5-tuple.  Once such a request is detected, the server can stop
	//    parsing the request and immediately generate a success response.
	//    When building this response, the value of the LIFETIME attribute
	//    can be taken from the time-to-expiry field in the allocate state
	//    data, even though this value may differ slightly from the LIFETIME
	//    value originally returned.  In addition, the server may need to
	//    store an indication of any reservation token returned in the
	//    original response, so that this may be returned in any
	//    retransmitted responses.

	//    For the case where the original request was unsuccessful in
	//    creating an allocation, the server may choose to do nothing
	//    special.  Note, however, that there is a rare case where the
	//    server rejects the original request but accepts the retransmitted
	//    request (because conditions have changed in the brief intervening
	//    time period).  If the client receives the first failure response,
	//    it will ignore the second (success) response and believe that an
	//    allocation was not created.  An allocation created in this matter
	//    will eventually timeout, since the client will not refresh it.
	//    Furthermore, if the client later retries with the same 5-tuple but
	//    different transaction id, it will receive a 437 (ServerAllocation
	//    Mismatch), which will cause it to retry with a different 5-tuple.
	//    The server may use a smaller maximum lifetime value to minimize
	//    the lifetime of allocations "orphaned" in this manner.

	log("trace") << "Handle Allocation Request: OK" << endl;
}

				   
void Server::sendError(const Request& request, int errorCode, const char* errorDesc) 
{
	log("trace") << "Sending STUN error: " << errorCode << ":" << errorDesc << endl;
	
	FastMutex::ScopedLock lock(_mutex);

	STUN::Message errorMsg;
	errorMsg.setType(request.type());
	errorMsg.setTransactionID(request.transactionID());

	// SOFTWARE
	STUN::Software* softwareAttr = new STUN::Software;
	softwareAttr->copyBytes(_options.software.c_str(), _options.software.size());
	errorMsg.add(softwareAttr);

	// REALM
	STUN::Realm* realmAttr = new STUN::Realm;
	realmAttr->copyBytes(_options.realm.c_str(), _options.realm.size());
	errorMsg.add(realmAttr);

	// NONCE
	STUN::Nonce* nonceAttr = new STUN::Nonce;
	string noonce = CryptoProvider::generateRandomKey(32);
	nonceAttr->copyBytes(noonce.c_str(), noonce.size());
	errorMsg.add(nonceAttr);

	// ERROR-CODE
	STUN::ErrorCode* errorCodeAttr = new STUN::ErrorCode();
	errorCodeAttr->setErrorCode(errorCode);
	errorCodeAttr->setReason(errorDesc);
	errorMsg.add(errorCodeAttr);
	
	request.socket.send(errorMsg, request.remoteAddr);
}

	
Net::UDPPacketSocket& Server::socketUDP()
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _socketUDP; 
}


IServerObserver& Server::observer() 
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _observer; 
}


Server::Options& Server::options() 
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _options; 
}


ServerAllocationMap Server::allocations() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _allocations;
}


Net::Reactor& Server::reactor()
{
	FastMutex::ScopedLock lock(_mutex);
	return _reactor;
}


Runner& Server::runner()
{
	FastMutex::ScopedLock lock(_mutex);
	return _runner;
}
/*
*/


void Server::addAllocation(ServerAllocation* alloc) 
{
	{
		FastMutex::ScopedLock lock(_mutex);

		assert(_allocations.find(alloc->tuple()) == _allocations.end());
		_allocations[alloc->tuple()] = alloc;

		log("info") << "Added an allocation: " 
			<< alloc->tuple().toString() << ": " 
			<< _allocations.size() << " total" << endl;
	}

	_observer.onServerAllocationCreated(this, alloc);
}


void Server::removeAllocation(ServerAllocation* alloc) 
{
	{
		FastMutex::ScopedLock lock(_mutex);

		ServerAllocationMap::iterator it = _allocations.find(alloc->tuple());
		assert(it != _allocations.end());
		_allocations.erase(it);

		log("info") << "Removed an allocation: " 
			<< alloc->tuple().toString() << ": " 
			<< _allocations.size() << " remaining" << endl;
	}

	_observer.onServerAllocationRemoving(this, alloc);
}


ServerAllocation* Server::getAllocation(const FiveTuple& tuple) 
{
	FastMutex::ScopedLock lock(_mutex);

	ServerAllocationMap::iterator it = _allocations.find(tuple);
	if (it != _allocations.end())
		return it->second;
	return NULL;
}


TCPAllocation* Server::getTCPAllocation(const UInt32& connectionID) 
{
	FastMutex::ScopedLock lock(_mutex);
	
	for (ServerAllocationMap::iterator it = _allocations.begin(); it != _allocations.end(); ++it) {
		TCPAllocation* alloc = dynamic_cast<TCPAllocation*>(it->second);
		if (alloc && alloc->peers().exists(connectionID))
			return alloc;
	}

	return NULL;
}


} } //  namespace Sourcey::TURN
