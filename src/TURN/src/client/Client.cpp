//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// LibSourcey is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//


#include "Sourcey/TURN/client/Client.h"
#include "Sourcey/Logger.h"
#include "Poco/Format.h"

#include <assert.h>
#include <iostream>
#include <algorithm>


using namespace std;
using namespace Poco;
using namespace Poco::Net;


namespace Scy {
namespace TURN {


Client::Client(IClientObserver& observer, const Options& options) : 
	_observer(observer),
	_options(options),
	_reactor(Net::Reactor::getDefault()),
	_runner(Runner::getDefault()),
	_socket(NULL),
	_timer(NULL)
{
}


Client::Client(IClientObserver& observer, Net::Reactor& reactor, Runner& runner, const Options& options) : 
	_observer(observer),
	_options(options),
	_reactor(reactor),
	_runner(runner),
	_socket(NULL),
	_timer(NULL)
{
}


Client::~Client() 
{
	log() << "Destroying" << endl;

	assert(isTerminated());
}


void Client::initiate() 
{
	log() << "TURN Client Connecting to " 
		<< _options.serverAddr.toString() 
		<< endl;
	
	assert(!_permissions.empty() && "must assign permissions");

	_socket = createSocket();
	_socket->registerPacketType<STUN::Message>(1);
	_socket->attach(packetDelegate<Client, STUN::Message>(this, &Client::onReceiveSTUNMessage, 0));
	_socket->Connected += delegate(this, &Client::onClientConnect);
	_socket->Closed += delegate(this, &Client::onClientDisconnect);
	_socket->connect(_options.serverAddr);
}


void Client::terminate()
{
	log() << "Terminating" << endl;	

	//Timer::getDefault().stop(TimerCallback<Client>(this, &Client::onTimer));	
	{
		FastMutex::ScopedLock lock(_mutex); 

		if (_timer) {
			_runner.destroy(_timer);
			_timer = NULL;
		}
			//_timer->destroy();	
	
		//Util::ClearVector(_transactions);
		for (vector<STUN::Transaction*>::iterator it = _transactions.begin(); it != _transactions.end(); ++it) {
			log() << "Terminating: Transaction: " << *it << endl;	
			(*it)->StateChange -= delegate(this, &Client::onTransactionStateChange);	
			(*it)->cancel();
			//delete *it;
		}
		_transactions.clear();

		if (_socket) {
			_socket->detach(packetDelegate<Client, STUN::Message>(this, &Client::onReceiveSTUNMessage, 0));
			_socket->Connected -= delegate(this, &Client::onClientConnect);
			_socket->Closed -= delegate(this, &Client::onClientDisconnect);
			_socket->close();
			delete _socket;
			_socket = NULL;
		}
	}
	
	setState(ClientState::Terminated);

	log() << "Terminating: OK" << endl;	
}


void Client::onClientConnect(void* sender)
{
	log() << "Client Connected" << endl;	
	//Timer::getDefault().start(TimerCallback<Client>(this, &Client::onTimer, _options.timerInterval, _options.timerInterval));
	assert(!_timer);
	_timer = new TimerTask(_options.timerInterval, _options.timerInterval);
	_timer->Timeout += delegate(this, &Client::onTimer);
	runner().start(_timer);
	//_timer->start();
	sendAllocate();
}


void Client::onClientDisconnect(void* sender)
{
	log() << "Client Disconnected" << endl;	
	terminate();
}


/*
void Client::onConnectStateChange(void* sender, Net::SocketState& state, const Net::SocketState&)
{
	log() << "Connection State Changed: " << state.toString() << endl;	
	
	switch (state.id()) {
	case Net::SocketState::Connected:
		Timer::getDefault().start(TimerCallback<Client>(this, &Client::onTimer, _options.timerInterval, _options.timerInterval));
		sendAllocate();
		break;
		
	case Net::SocketState::Disconnected: 
	case Net::SocketState::Error:
		terminate();
		break;
	}
}
*/


void Client::sendRefresh()
{
	// 7. Refreshing an Allocation
	// 
	// A Refresh transaction can be used to either (a) refresh an existing
	// allocation and update its time-to-expiry or (b) delete an existing
	// allocation.
	// 
	// If a client wishes to continue using an allocation, then the client
	// MUST refresh it before it expires.  It is suggested that the client
	// refresh the allocation roughly 1 minute before it expires.  If a
	// client no longer wishes to use an allocation, then it SHOULD
	// explicitly delete the allocation.  A client MAY refresh an allocation
	// at any time for other reasons.
		
	log() << "Sending refresh allocation request" << endl;	

	STUN::Transaction* transaction = createTransaction();
	transaction->request().setType(STUN::Message::Refresh);

	STUN::Lifetime* lifetimeAttr = new STUN::Lifetime;
	lifetimeAttr->setValue(_options.lifetime / 1000);
	transaction->request().add(lifetimeAttr);
	
	sendAuthenticatedTransaction(transaction);
	//transaction->send();
}


void Client::handleRefreshResponse(const STUN::Message& response) 
{
	log() << "Received a Refresh Response: " << response.toString() << endl;	

	assert(response.type() == STUN::Message::Refresh);	

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
	const STUN::ErrorCode* errorAttr = response.get<STUN::ErrorCode>();
	if (errorAttr) {
		assert(errorAttr->errorCode() == 437);
		return;
	}

	const STUN::Lifetime* lifetimeAttr = response.get<STUN::Lifetime>();
	if (!lifetimeAttr) {
		assert(false);
		return;
	}

	setLifetime(lifetimeAttr->value());
	
	log() << "Refreshed allocation expires in: " << timeRemaining() << endl;	

	// If lifetime is 0 the allocation will be cleaned up by garbage collection.
}


void Client::onReceiveSTUNMessage(void* sender, STUN::Message& message) 
{
	log() << "Received a STUN Message: " << message.toString() << endl;	
	
	handleResponse(message);
}


bool Client::removeTransaction(STUN::Transaction* transaction)
{
	log() << "Removing Transaction: " << transaction << endl;

	FastMutex::ScopedLock lock(_mutex); 
	
	for (vector<STUN::Transaction*>::iterator it = _transactions.begin(); it != _transactions.end(); ++it) {
		if (*it == transaction) {
			(*it)->StateChange -= delegate(this, &Client::onTransactionStateChange);	
			//if (free)
			//	delete *it;
			_transactions.erase(it);
			return true;
		}
	}
	return false;
}


void Client::authenticateRequest(STUN::Message& request)
{	
	FastMutex::ScopedLock lock(_mutex); 

	if (_options.username.size()) {
		STUN::Username* usernameAttr = new STUN::Username;
		usernameAttr->copyBytes(_options.username.c_str(), _options.username.size());
		request.add(usernameAttr);
	}

	if (_realm.size()) {
		STUN::Realm* realmAttr = new STUN::Realm;
		realmAttr->copyBytes(_realm.c_str(), _realm.size());
		request.add(realmAttr);
	}

	if (_nonce.size()) {
		STUN::Nonce* nonceAttr = new STUN::Nonce;		
		nonceAttr->copyBytes(_nonce.c_str(), _nonce.size());
		request.add(nonceAttr);
	}

	if (_options.password.size()) {
		STUN::MessageIntegrity* integrityAttr = new STUN::MessageIntegrity;
		integrityAttr->setKey(_options.password);
		request.add(integrityAttr);
	}
}


bool Client::sendAuthenticatedTransaction(STUN::Transaction* transaction)
{	
	authenticateRequest(transaction->request());
	log() << "Sending Authenticated Transaction: " << transaction->request().toString() << endl;
	return transaction->send();
}


STUN::Transaction* Client::createTransaction(Net::IPacketSocket* socket)
{
	FastMutex::ScopedLock lock(_mutex); 
	
	socket = socket ? socket : _socket;
	assert(socket);
	STUN::Transaction* transaction = new STUN::Transaction(_runner, socket, socket->address(), _options.serverAddr, _options.timeout, 1);
	transaction->StateChange += delegate(this, &Client::onTransactionStateChange);	
	_transactions.push_back(transaction);
	return transaction;
}


bool Client::handleResponse(const STUN::Message& response)
{
	// Send this response to the appropriate handler.	
	if (response.type() == STUN::Message::Allocate &&
		response.state() == STUN::Message::SuccessResponse)	
		handleAllocateResponse(response);

	else if (response.type() == STUN::Message::Allocate &&
		response.state() == STUN::Message::ErrorResponse)	
		handleAllocateErrorResponse(response);

	else if (response.type() == STUN::Message::Refresh)	
		handleRefreshResponse(response);

	//else if (response.type() == STUN::Message::Refresh &&
	//	response.state() == STUN::Message::ErrorResponse)&&
	//	response.state() == STUN::Message::SuccessResponse
	//	handleRefreshErrorResponse(response);

	else if (response.type() == STUN::Message::CreatePermission &&
		response.state() == STUN::Message::SuccessResponse)	
		handleCreatePermissionResponse(response);

	else if (response.type() == STUN::Message::CreatePermission &&
		response.state() == STUN::Message::ErrorResponse)	
		handleCreatePermissionErrorResponse(response);

	else if (response.type() == STUN::Message::DataIndication)	
		handleDataIndication(response);

	else 
		return false;

	return true;
}


void Client::sendAllocate() 
{
	log() << "Sending allocation request" << endl;

	assert(_options.username.size());
	assert(_options.password.size());
	//assert(_options.lifetime);
	//_lifetime = _options.lifetime;

	// The client forms an Allocate request as follows.
	// 
	// The client first picks a host transport address.  It is RECOMMENDED
	// that the client pick a currently unused transport address, typically
	// by allowing the underlying OS to pick a currently unused port for a
	// new socket.
	// 
	// The client then picks a transport protocol to use between the client
	// and the server.  The transport protocol MUST be one of UDP, TCP, or
	// TLS-over-TCP.  Since this specification only allows UDP between the
	// server and the peers, it is RECOMMENDED that the client pick UDP
	// unless it has a reason to use a different transport.  One reason to
	// pick a different transport would be that the client believes, either
	// through configuration or by experiment, that it is unable to contact
	// any TURN server using UDP.  See Section 2.1 for more discussion.
	// 
	// The client also picks a server transport address, which SHOULD be
	// done as follows.  The client receives (perhaps through configuration)
	// a domain name for a TURN server.  The client then uses the DNS
	// procedures described in [RFC5389], but using an SRV service name of
	// "turn" (or "turns" for TURN over TLS) instead of "stun" (or "stuns").
	// For example, to find servers in the example.com domain, the client
	// performs a lookup for '_turn._udp.example.com',
	// '_turn._tcp.example.com', and '_turns._tcp.example.com' if the client
	// wants to communicate with the server using UDP, TCP, or TLS-over-TCP,
	// respectively.
	// 	
	STUN::Transaction* transaction = createTransaction();
	transaction->request().setType(STUN::Message::Allocate);

	// The client MUST include a REQUESTED-TRANSPORT attribute in the
	// request.  This attribute specifies the transport protocol between the
	// server and the peers (note that this is NOT the transport protocol
	// that appears in the 5-tuple).
	// 
	STUN::RequestedTransport* transportAttr = new STUN::RequestedTransport;
	transportAttr->setValue(transportProtocol());
	transaction->request().add(transportAttr);

	// If the client wishes the server to initialize the time-to-expiry
	// field of the allocation to some value other than the default
	// lifetime, then it MAY include a LIFETIME attribute specifying its
	// desired value.  This is just a request, and the server may elect to
	// use a different value.  Note that the server will ignore requests to
	// initialize the field to less than the default value.
	// 
	if (_options.lifetime) {
		STUN::Lifetime* lifetimeAttr = new STUN::Lifetime;
		lifetimeAttr->setValue(_options.lifetime / 1000);
		transaction->request().add(lifetimeAttr);
	}

	// If the client wishes to later use the DONT-FRAGMENT attribute in one
	// or more Send indications on this allocation, then the client SHOULD
	// include the DONT-FRAGMENT attribute in the Allocate transaction->request().  This
	// allows the client to test whether this attribute is supported by the
	// server.
	// 
	// If the client requires the port number of the relayed transport
	// address be even, the client includes the EVEN-PORT attribute.  If
	// this attribute is not included, then the port can be even or odd.  By
	// setting the R bit in the EVEN-PORT attribute to 1, the client can
	// request that the server reserve the next highest port number (on the
	// same IP address) for a subsequent allocation.  If the R bit is 0, no
	// such request is made.
	// 
	// The client MAY also include a RESERVATION-TOKEN attribute in the
	// request to ask the server to use a previously reserved port for the
	// allocation.  If the RESERVATION-TOKEN attribute is included, then the
	// client MUST omit the EVEN-PORT attribute.
	// 
	// Once constructed, the client sends the Allocate request on the
	// 5-tuple.
	// 
	sendAuthenticatedTransaction(transaction);
}



//  At this point the response has already been authenticated, but we 
//  have not checked for existing allocations on this 5 tuple.
void Client::handleAllocateResponse(const STUN::Message& response) 
{
	assert(response.type() == STUN::Message::Allocate);	
	
	FastMutex::ScopedLock lock(_mutex);

	// If the client receives an Allocate success response, then it MUST
	// check that the mapped address and the relayed transport address are
	// in an address family that the client understands and is prepared to
	// handle.  This specification only covers the case where these two
	// addresses are IPv4 addresses.  If these two addresses are not in an
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
	// structure to track what is happening on the server.  In particular,
	// the client needs to remember the actual lifetime received back from
	// the server, rather than the value sent to the server in the request.
	// 	
	const STUN::Lifetime* lifetimeAttr = response.get<STUN::Lifetime>();
	if (!lifetimeAttr) {
		assert(false);
		return;
	}
	
	const STUN::XorMappedAddress* mappedAttr = response.get<STUN::XorMappedAddress>();
	if (!mappedAttr || mappedAttr->family() != 1) {
		assert(false);
		return;
	}
	_mappedAddress = Net::Address(mappedAttr->ipString(), mappedAttr->port());

	// The client must also remember the 5-tuple used for the request and
	// the username and password it used to authenticate the request to
	// ensure that it reuses them for subsequent messages.  The client also
	// needs to track the channels and permissions it establishes on the
	// server.
	// 
	// The client will probably wish to send the relayed transport address
	// to peers (using some method not specified here) so the peers can
	// communicate with it.  The client may also wish to use the server-
	// reflexive address it receives in the XOR-MAPPED-ADDRESS attribute in
	// its ICE processing.	
	// 
	const STUN::XorRelayedAddress* relayedAttr = response.get<STUN::XorRelayedAddress>();
	if (!relayedAttr || relayedAttr && relayedAttr->family() != 1) {
		assert(false);
		return;
	}
	_relayedAddress = Net::Address(relayedAttr->ipString(), relayedAttr->port());	

	log() << "Allocation Response:" 
		<< "\n\tRelayed Address: " << _relayedAddress.toString()
		<< "\n\tMapped Address: " << _mappedAddress.toString()
		<< "\n\tLifetime: " << lifetimeAttr->value()
		<< endl;
	
	// Once the allocation is created we transition to Authorizing while
	// peer permissions are created.
	setState(ClientState::Authorizing);

	// If the permission list has entries create them now.
	// A successful response here will set the client state to Success.
	if (!isTerminated())
		sendCreatePermission();	
}


void Client::handleAllocateErrorResponse(const STUN::Message& response) 
{	
	assert(response.type() == STUN::Message::Allocate &&
		response.state() == STUN::Message::ErrorResponse);	
	
	const STUN::ErrorCode* errorAttr = response.get<STUN::ErrorCode>();
	if (!errorAttr) {
		assert(false);
		return;
	}
	
	log() << "Allocation Error Response: " 
		<< errorAttr->errorCode() << endl;

	// If the client receives an Allocate error response, then the
	// processing depends on the actual error code returned:

	// o  (Request timed out): There is either a problem with the server, or
	//    a problem reaching the server with the chosen transport.  The
	//    client considers the current transaction as having failed but MAY
	//    choose to retry the Allocate request using a different transport
	//    (e.g., TCP instead of UDP).

	switch (errorAttr->errorCode()) {
		// 300 (Try Alternate): The server would like the client to use the
		// server specified in the ALTERNATE-SERVER attribute instead.  The
		// client considers the current transaction as having failed, but
		// SHOULD try the Allocate request with the alternate server before
		// trying any other servers (e.g., other servers discovered using the
		// SRV procedures).  When trying the Allocate request with the
		// alternate server, the client follows the ALTERNATE-SERVER
		// procedures specified in [RFC5389].
		case 300:
			// Return the error to the client.
			assert(false);
			break;						

		// 400 (Bad Request): The server believes the client's request is
		// malformed for some reason.  The client considers the current
		// transaction as having failed.  The client MAY notify the client or
		// operator and SHOULD NOT retry the request with this server until
		// it believes the problem has been fixed.
		case 400:
			// Return the error to the client.
			assert(false);
			break;

		// 401 (Unauthorized): If the client has followed the procedures of
		// the long-term credential mechanism and still gets this error, then
		// the server is not accepting the client's credentials.  In this
		// case, the client considers the current transaction as having
		// failed and SHOULD notify the client or operator.  The client SHOULD
		// NOT send any further requests to this server until it believes the
		// problem has been fixed.
		case 401:	
			{
				FastMutex::ScopedLock lock(_mutex); 
				if (_realm.empty() || _nonce.empty()) {
				
					// REALM
					const STUN::Realm* realmAttr = response.get<STUN::Realm>();
					if (realmAttr) {
						_realm = realmAttr->asString();
					}
				
					// NONCE
					const STUN::Nonce* nonceAttr = response.get<STUN::Nonce>();					
					if (nonceAttr) {
						_nonce = nonceAttr->asString();
					}
				
					// Now that our realm and nonce are set we can re-send the allocate request.
					if (_realm.size() && _nonce.size()) {					
						log() << "Re-sending allocation request" << endl;
						sendAllocate();
						return;
					}
				}
			}
			
			break;

		// 403 (Forbidden): The request is valid, but the server is refusing
		// to perform it, likely due to administrative restrictions.  The
		// client considers the current transaction as having failed.  The
		// client MAY notify the client or operator and SHOULD NOT retry the
		// same request with this server until it believes the problem has
		// been fixed.
		case 403:
			// Return the error to the client.
			assert(false);
			break;

		// 420 (Unknown Attribute): If the client included a DONT-FRAGMENT
		// attribute in the request and the server rejected the request with
		// a 420 error code and listed the DONT-FRAGMENT attribute in the
		// UNKNOWN-ATTRIBUTES attribute in the error response, then the
		// client now knows that the server does not support the DONT-
		// FRAGMENT attribute.  The client considers the current transaction
		// as having failed but MAY choose to retry the Allocate request
		// without the DONT-FRAGMENT attribute.
		case 420:
			assert(false);
			break;

		// 437 (Allocation Mismatch): This indicates that the client has
		// picked a 5-tuple that the server sees as already in use.  One way
		// this could happen is if an intervening NAT assigned a mapped
		// transport address that was used by another client that recently
		// crashed.  The client considers the current transaction as having
		// failed.  The client SHOULD pick another client transport address
		// and retry the Allocate request (using a different transaction id).
		// The client SHOULD try three different client transport addresses
		// before giving up on this server.  Once the client gives up on the
		// server, it SHOULD NOT try to create another allocation on the
		// server for 2 minutes.
		case 437:
			assert(false);
			break;

		// 438 (Stale Nonce): See the procedures for the long-term credential
		// mechanism [RFC5389].
		case 438:
			assert(false);
			break;

		// 441 (Wrong Credentials): The client should not receive this error
		// in response to a Allocate request.  The client MAY notify the client
		// or operator and SHOULD NOT retry the same request with this server
		// until it believes the problem has been fixed.
		case 441:
			assert(false);
			break;

		// 442 (Unsupported Transport Address): The client should not receive
		// this error in response to a request for a UDP allocation.  The
		// client MAY notify the client or operator and SHOULD NOT reattempt
		// the request with this server until it believes the problem has
		// been fixed.
		case 442:
			assert(false);
			break;

		// 486 (Allocation Quota Reached): The server is currently unable to
		// create any more allocations with this username.  The client
		// considers the current transaction as having failed.  The client
		// SHOULD wait at least 1 minute before trying to create any more
		// allocations on the server.
		case 486:
			// controlConn.disconnect();
			// controlConn.startConnectionTimer(60000);
			//assert(false);
			break;

		// 508 (Insufficient Capacity): The server has no more relayed
		// transport addresses available, or has none with the requested
		// properties, or the one that was reserved is no longer available.
		// The client considers the current operation as having failed.  If
		// the client is using either the EVEN-PORT or the RESERVATION-TOKEN
		// attribute, then the client MAY choose to remove or modify this
		// attribute and try again immediately.  Otherwise, the client SHOULD
		// wait at least 1 minute before trying to create any more
		// allocations on this server.
		case 508:
			// controlConn.disconnect();
			// controlConn.startConnectionTimer(60000);
			//assert(false);
			break;
			
		// An unknown error response MUST be handled as described in [RFC5389].
		default:
			assert(false);
			break;
	}

	if (!isTerminated())
		_observer.onAllocationFailed(*this, errorAttr->errorCode(), errorAttr->reason());
	
	setState(ClientState::Failed, format("(%d) %s", (int)errorAttr->errorCode(), errorAttr->reason()));
}


void Client::addPermission(const IPList& peerIPs)
{	
	for (IPList::const_iterator it = peerIPs.begin(); it != peerIPs.end(); ++it) {
		addPermission(*it);
	}
}


void Client::addPermission(const Net::IP& peerIP)
{
	IAllocation::addPermission(peerIP);
}


void Client::sendCreatePermission()
{
	log() << "Send Create Permission Request" << endl;

	assert(!_permissions.empty());

	// The client who wishes to install or refresh one or more permissions
	// can send a CreatePermission request to the server.
	// 
	// When forming a CreatePermission request, the client MUST include at
	// least one XOR-PEER-ADDRESS attribute, and MAY include more than one
	// such attribute.  The IP address portion of each XOR-PEER-ADDRESS
	// attribute contains the IP address for which a permission should be
	// installed or refreshed.  The port portion of each XOR-PEER-ADDRESS
	// attribute will be ignored and can be any arbitrary value.  The
	// various XOR-PEER-ADDRESS attributes can appear in any order.

	STUN::Transaction* transaction = createTransaction();
	transaction->request().setType(STUN::Message::CreatePermission);
	
	for (PermissionList::const_iterator it = _permissions.begin(); it != _permissions.end(); ++it) {
		log() << "Create Permission Request: " << (*it).ip.toString() << endl;
		STUN::XorPeerAddress* peerAttr = new STUN::XorPeerAddress;
		peerAttr->setFamily(1);
		peerAttr->setPort(0);
		peerAttr->setIP((*it).ip.toString());
		transaction->request().add(peerAttr);
	}

	sendAuthenticatedTransaction(transaction);
}


void Client::handleCreatePermissionResponse(const STUN::Message& response) 
{
	// If the client receives a valid CreatePermission success response,
	// then the client updates its data structures to indicate that the
	// permissions have been installed or refreshed.	
	log() << "Permission Created" << endl;
	
	// Send all queued requests...
	{
		FastMutex::ScopedLock lock(_mutex); 
		
		STUN::Message* request = NULL;
		while (!_pendingRequests.empty()) {
			request = _pendingRequests.front();
			_pendingRequests.pop_front();
			_socket->send(*request);
			delete request;
		}
	}
	
	if (!isTerminated()) {		
		_observer.onAllocationPermissionsCreated(*this, _permissions);
		
		/*
		STUN::Transaction* transaction = reinterpret_cast<STUN::Transaction*>(response.opaque);
		for (int i = 0; i < 100; i++) {
			const STUN::XorPeerAddress* peerAttr = transaction->request().get<STUN::XorPeerAddress>(i);
			if (!peerAttr || peerAttr && peerAttr->family() != 1)
				break;	
			_observer.onAllocationPermissionsCreated(*this, Net::IP(peerAttr->ipString()));
		}
		*/
	}

	// Once permissions have been created the allocation 
	// process is considered a success.
	setState(ClientState::Success);
}


void Client::handleCreatePermissionErrorResponse(const STUN::Message& response) 
{	
	LogWarn() << "Permission Creation Failed" << endl;

	removeAllPermissions();
	
	setState(ClientState::Failed, "Cannot create peer permissions on server.");
}


void Client::sendChannelBind(const Net::IP& peerIP) 
{
	// A channel binding is created or refreshed using a ChannelBind
	// transaction. A ChannelBind transaction also creates or refreshes a
	// permission towards the peer (see Section 8).
	// 
	// To initiate the ChannelBind transaction, the client forms a
	// ChannelBind request.  The channel to be bound is specified in a
	// CHANNEL-NUMBER attribute, and the peer's transport address is
	// specified in an XOR-PEER-ADDRESS attribute.  Section 11.2 describes
	// the restrictions on these attributes.
	// 
	// Rebinding a channel to the same transport address that it is already
	// bound to provides a way to refresh a channel binding and the
	// corresponding permission without sending data to the peer.  Note
	// however, that permissions need to be refreshed more frequently than
	// channels.
	assert(false);
}


void Client::sendData(const char* data, int size, const Net::Address& peerAddress) 
{
	log() << "Sending Data Indication to peer: " 
		<< peerAddress.toString() << endl;

	STUN::Message* request = new STUN::Message;
	request->setType(STUN::Message::SendIndication);

	// The client can use a Send indication to pass data to the server for
	// relaying to a peer.  A client may use a Send indication even if a
	// channel is bound to that peer.  However, the client MUST ensure that
	// there is a permission installed for the IP address of the peer to
	// which the Send indication is being sent; this prevents a third party
	// from using a TURN server to send data to arbitrary destinations.
	// 
	// When forming a Send indication, the client MUST include an XOR-PEER-
	// ADDRESS attribute and a DATA attribute.  The XOR-PEER-ADDRESS
	// attribute contains the transport address of the peer to which the
	// data is to be sent, and the DATA attribute contains the actual
	// application data to be sent to the peer.
	// 	
	// The client MAY include a DONT-FRAGMENT attribute in the Send
	// indication if it wishes the server to set the DF bit on the UDP
	// datagram sent to the peer.

	STUN::XorPeerAddress* peerAttr = new STUN::XorPeerAddress;
	peerAttr->setFamily(1);
	peerAttr->setPort(peerAddress.port());
	peerAttr->setIP(peerAddress.host().toString());
	request->add(peerAttr);

	STUN::Data* dataAttr = new STUN::Data;
	dataAttr->copyBytes(data, size);
	request->add(dataAttr);

	// Ensure permissions exist for the peer.
	if (!hasPermission(peerAddress.host())) {
		throw Exception("No permission exists for peer IP: " + peerAddress.host().toString());
	} 

	// If permission exists and is currently being negotiated with
	// the server then queue the outgoing request.
	// Queued requests will be sent when the CreatePermission
	// callback is received from the server.
	else if (stateEquals(ClientState::Authorizing)) {	
		log() << "Queueing outgoing request: " 
			<< request->toString() << endl;
		FastMutex::ScopedLock lock(_mutex);
		_pendingRequests.push_back(request);	
		assert(_pendingRequests.size() < 100); // something is wrong...
	}

	// If a permission exists on server and client send our data!
	else {
		_socket->send(*request, _options.serverAddr);
		delete request;
	}
}


void Client::handleDataIndication(const STUN::Message& response) 
{
	// When the client receives a Data indication, it checks that the Data
	// indication contains both an XOR-PEER-ADDRESS and a DATA attribute,
	// and discards the indication if it does not.  The client SHOULD also
	// check that the XOR-PEER-ADDRESS attribute value contains an IP
	// address with which the client believes there is an active permission,
	// and discard the Data indication otherwise.  Note that the DATA
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

	const STUN::XorPeerAddress* peerAttr = response.get<STUN::XorPeerAddress>();
	if (!peerAttr || peerAttr && peerAttr->family() != 1) {
		assert(false);
		return;
	}

	const STUN::Data* dataAttr = response.get<STUN::Data>();
	if (!dataAttr) {
		assert(false);
		return;
	}	

	log() << "Handle Data Indication: " << response.toString() << endl;

	if (!isTerminated())
		_observer.onRelayedData(*this, dataAttr->bytes(), dataAttr->size(), peerAttr->address());
		
	//OnData.emit(this, dataAttr->bytes(), dataAttr->size(), peerAttr->address());
	FastMutex::ScopedLock lock(_mutex); 
	//STUN::DataPacket packet(dataAttr->bytes(), dataAttr->size(), _socket->address(), peerAttr->address());
	//DataIndicationReceived.notify(this, packet);
}


void Client::onTransactionStateChange(void* sender, TransactionState& state, const TransactionState&) 
{
	log() << "Transaction State Changed: " << sender << ": " << state.toString() << endl;

	STUN::Transaction* transaction = reinterpret_cast<STUN::Transaction*>(sender);
	transaction->response().opaque = transaction;	
	
	if (!isTerminated())
		_observer.onTransactionResponse(*this, *transaction);

	switch (state.id()) {	
	case TransactionState::Running:
		return;

	case TransactionState::Success: 
		{	
			log() << "STUN Transaction Success Response:" 
				<< "\n\tState: " << state.toString()
				<< "\n\tFrom: " << transaction->peerAddress().toString()
				<< "\n\tRequest: " << transaction->request().toString()
				<< "\n\tResponse: " << transaction->response().toString()
				<< endl;
			
			if (removeTransaction(transaction)) {
				if (!handleResponse(transaction->response()))
					log() << "Unhandled STUN Response: " << transaction->response().toString() << endl;	
				//delete transaction;
			}
		}
		break;

	case TransactionState::Failed:
		LogWarn() << "STUN Transaction Error Response:" 
				<< "\n\tState: " << state.toString()
				<< "\n\tFrom: " << transaction->peerAddress().toString()
				<< "\n\tData: " << transaction->response().toString()
				<< endl;

		if (removeTransaction(transaction)) {

			// TODO: Be a little more flexible
			setState(ClientState::Failed, state.message());
			//delete transaction;
		}
		break;
	}
};


//void Client::onTimer(TimerCallback<Client>&) 
void Client::onTimer(void*)
{	
	FastMutex::ScopedLock lock(_mutex); 

	if (expired())
		// Attempt to re-allocate
		sendAllocate();

	else if (timeRemaining() < lifetime() * 0.33)
		sendRefresh();
}

	
void Client::onStateChange(ClientState& state, const ClientState& oldState) 
{
	_observer.onRelayStateChange(*this, state, oldState);
}
		

Net::IPacketSocket* Client::createSocket()
{
	return new Net::UDPPacketSocket(_reactor, _runner);
}


int Client::transportProtocol()
{
	return 17; // UDP
}


bool Client::isTerminated() const
{
	return stateEquals(ClientState::Terminated);
}

	
Net::IPacketSocket& Client::socket()
{ 
	FastMutex::ScopedLock lock(_mutex);
	return *_socket; 
}


Client::Options& Client::options() 
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _options; 
}


Net::Reactor& Client::reactor()
{
	FastMutex::ScopedLock lock(_mutex);
	return _reactor;
}


Runner& Client::runner()
{
	FastMutex::ScopedLock lock(_mutex);
	return _runner;
}


Net::Address Client::mappedAddress() const 
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _mappedAddress; 
}


Net::Address Client::relayedAddress() const 
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _relayedAddress; 
}


} } //  namespace Scy::TURN

