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


#include "scy/turn/server/tcpallocation.h"
#include "scy/turn/server/server.h"
#include "scy/logger.h"


using namespace std;


namespace scy {
namespace turn {


TCPAllocation::TCPAllocation(Server& server, const net::Socket& control, const FiveTuple& tuple, const std::string& username, const UInt32& lifetime) : 
	ServerAllocation(server, tuple, username, lifetime),
	_control(control)
{
	// Bind a socket acceptor for incoming peer connections.
	_acceptor.bind(net::Address(server.options().listenAddr.host(), 0));
	_acceptor.listen();
	_acceptor.base().AcceptConnection += delegate(this, &TCPAllocation::onPeerAccept);
	
	// The allocation will be deleted if the control connection is lost.
	_control.Close += delegate(this, &TCPAllocation::onControlClosed);

	log("trace") << "Initializing on " << _acceptor.address() << endl;
}
	

TCPAllocation::~TCPAllocation() 
{
	log("trace") << "Destroy TCP allocation" << endl;	
	
	//Mutex::ScopedLock lock(_mutex);
	assert(_acceptor.base().refCount() == 1);
	_acceptor.base().AcceptConnection -= delegate(this, &TCPAllocation::onPeerAccept);
	_acceptor.close();
	
	assert(_acceptor.base().refCount() == 1);
	_control.Close -= delegate(this, &TCPAllocation::onControlClosed);	
	_control.close();

	auto pairs = this->pairs().map();	
	for (auto it = pairs.begin(); it != pairs.end(); ++it) {
		// The allocation will be removed via callback
		delete it->second;
	}
	assert(this->pairs().empty());
	
	log("trace") << "Destroy TCP allocation: OK" << endl;	
}


void TCPAllocation::onPeerAccept(void* sender, const net::TCPSocket& socket)
{
	log("trace") << "Peer connection accepted: " << socket.peerAddress() << endl;
	
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
	if (!hasPermission(socket.peerAddress().host())) {
		log("trace") << "No permission for peer: " << socket.peerAddress() << endl;
		return;
	}
	log("trace") << "Has permission for: " << socket.peerAddress() << endl;

	// Otherwise, the server sends a ConnectionAttempt indication to the
	// client over the control connection. The indication MUST include an
	// XOR-PEER-ADDRESS attribute containing the peer's transport address,
	// as well as a CONNECTION-ID attribute uniquely identifying the peer
	// data connection.
	// 				
	auto pair = new TCPConnectionPair(*this);
	assert(socket.base().refCount() == 1);
	pair->setPeerSocket(socket);
	assert(socket.base().refCount() == 2);
	
	stun::Message response(stun::Message::Indication, stun::Message::ConnectionAttempt);
	//stun::Message response;
	//response.setType(stun::Message::ConnectionAttempt);

	auto addrAttr = new stun::XorPeerAddress;	
	addrAttr->setAddress(socket.peerAddress());
	//addrAttr->setFamily(1);
	//addrAttr->setPort(socket.peerAddress().port());
	//addrAttr->setIP(socket.peerAddress().host());
	response.add(addrAttr);
	
	auto connAttr = new stun::ConnectionID;
	connAttr->setValue(pair->connectionID);
	response.add(connAttr);
  
	sendToControl(response);
	
	log("trace") << "Peer connection accepted with ID: " << pair->connectionID << endl;
}


bool TCPAllocation::handleRequest(Request& request) 
{	
	log("trace") << "Handle request" << endl;	

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
	log("trace") << "TCPAllocation: On timer" << endl;
	
	// Clean up any expired Connect request peer connections.
	auto pairs = this->pairs().map();	
	for (auto it = pairs.begin(); it != pairs.end(); ++it) {
		if (it->second->expired()) {			
			log("trace") << "TCPAllocation: On timer: Removing expired peer" << endl;
			this->pairs().free(it->first);
		}
	}
	
	return ServerAllocation::onTimer();
}		


void TCPAllocation::handleConnectRequest(Request& request)
{
	log("trace") << "Handle Connect request" << endl;

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
	log("trace") << "Handle ConnectionBind Request" << endl;
	
	assert(request.methodType() == stun::Message::ConnectionBind);
	TCPConnectionPair* pair = nullptr;
	try {
		// 5.4. Receiving a ConnectionBind Request
		// 
		// When a server receives a ConnectionBind request, it processes the
		// request as follows.
		// 
		// If the client connection transport is not TCP or TLS, the server MUST
		// return a 400 (Bad Request) error.
		// 
		if (request.socket.transport() != net::TCP) // TODO: TLS!!
			throw std::runtime_error("TLS not supported"); // easy to implement, fixme!

		// If the request does not contain the CONNECTION-ID attribute, or if
		// this attribute does not refer to an existing pending connection, the
		// server MUST return a 400 (Bad Request) error.
		// 
		auto connAttr = request.get<stun::ConnectionID>();
		if (!connAttr)
			throw std::runtime_error("ConnectionBind missing CONNECTION-ID attribute");

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
			throw std::runtime_error("No client for ConnectionBind request: " + util::itostr(connAttr->value()));
		}

		if (pair->isDataConnection) {
			assert(0);
			throw std::runtime_error("Already a peer data connection: " + util::itostr(connAttr->value()));
		}
		
		stun::Message response(stun::Message::SuccessResponse, stun::Message::ConnectionBind);
		response.setTransactionID(request.transactionID());
		
		// Send the response back over the client connection
		request.socket.send(response);

		// Reassign the socket base instance to the client connection.		
		pair->setClientSocket(request.socket);
		if (!pair->makeDataConnection()) {
			// Must have a client and peer by now
			throw std::runtime_error("BUG: Data connection binding failed");
		}

		assert(pair->isDataConnection);			
	} 
	catch (std::exception& exc) {
		log("error") << "ConnectionBind error: " << exc.what() << endl;
		server().respondError(request, 400, "Bad Request");
		
		if (pair && !pair->isDataConnection) {
			delete pair;

			// Close the incoming connection
			request.socket.close();
		}
	}
}


void TCPAllocation::sendPeerConnectResponse(TCPConnectionPair* pair, bool success)
{
	log("trace") << "Send peer Connect response: " << success << endl;
	
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
	stun::Message response(stun::Message::SuccessResponse, stun::Message::Connect);
	response.setTransactionID(pair->transactionID);

	if (success) {
		auto connAttr = new stun::ConnectionID;
		connAttr->setValue(pair->connectionID);
		response.add(connAttr);
	}
	else {
		auto errorCodeAttr = new stun::ErrorCode();
		errorCodeAttr->setErrorCode(447);
		errorCodeAttr->setReason("Connection Timeout or Failure");
		response.add(errorCodeAttr);
	}
  
	sendToControl(response);
}


int TCPAllocation::sendToControl(stun::Message& message)
{
	//Mutex::ScopedLock lock(_mutex);
	log("trace") << "Send to control: " << message << endl;
	return _control.send(message);
}


void TCPAllocation::onControlClosed(void* sender)
{
	//Mutex::ScopedLock lock(_mutex);
	log("trace") << "Control socket disconnected" << endl;

	// The allocation will be destroyed on the  
	// next timer call to IAllocation::deleted()
	_deleted = true;
}


net::TCPSocket& TCPAllocation::control()
{
	//Mutex::ScopedLock lock(_mutex);
	return _control;
}


TCPConnectionPairMap& TCPAllocation::pairs()
{
	//Mutex::ScopedLock lock(_mutex);
	return _pairs;
}


net::Address TCPAllocation::relayedAddress() const 
{ 
	//Mutex::ScopedLock lock(_mutex);
	return _acceptor.address();
}


} } //  namespace scy::turn



		//pair->client.Close += delegate(this, &TCPAllocation::onConnectionClosed);
	/*
	TCPPeerConnection* peer = reinterpret_cast<TCPPeerConnection*>(sender);	
	peer->get()->Connected -= delegate(this, &TCPAllocation::onPeerConnectSuccess);
	peer->get()->Error -= delegate(this, &TCPAllocation::onPeerConnectError);	
	//peer->Closed += delegate(this, &TCPAllocation::onPeerDisconnected);	

	auto socket = reinterpret_cast<net::TCPSocket*>(sender);
	TCPConnectionPair* pair = reinterpret_cast<TCPConnectionPair*>(socket->base().opaque);
	*/
		//if (it->second->expired()) {
			//this->pairs().free(it->first);
			//pair->peer.Close += delegate(this, &TCPAllocation::onConnectionClosed);
			//pair->peer.Recv += delegate(pair, &TCPConnectionPair::onRelayDataReceived);
		//}

	
	/*
			traceL() << "TCPAllocation: On timer: Removing expired peer" << endl;

	// Here we clean up expired peer connections, nothing else. 
	// All other connection types are self maintaining.		
	this->pairs().clear();

	auto pairs = this->pairs().map();	
	auto it = pairs.begin();
	while (it != pairs.end()) {
		if (it->second->expired())
		   this->pairs().free(it->first);
		else
		   ++it;
	}
	*/
	
	/*
	ClientConnectionMap clients = this->clients().map();
	for (ClientConnectionMap::const_iterator it = clients.begin(); it != clients.end(); ++it) {
		log("trace") << "Destroy: Closing Client: " << it->second << endl;	
		it->second->close();
	}
	
	//assert(peers().empty()); // should have been cleared when clients closed
	PeerConnectionMap peers = this->peers().map();		
	for (PeerConnectionMap::const_iterator it = peers.begin(); it != peers.end(); ++it) {
		log("trace") << "Destroy: Closing Peer: " << it->second << endl;	
		it->second->close();
	}
	*/


	/*
void TCPAllocation::onPeerConnectError(TCPConnectionPair* pair)  //, const std::string& error //void* sender, const Error& error
{
	log("trace") << "Peer Connect Error" << endl;
	
	assert(0 && "a");

	
	auto socket = reinterpret_cast<net::TCPSocket*>(sender);
	socket->Connect -= delegate(this, &TCPAllocation::onPeerConnectSuccess);
	socket->Error -= delegate(this, &TCPAllocation::onPeerConnectError);

	TCPConnectionPair* pair = reinterpret_cast<TCPConnectionPair*>(socket->base().opaque);
	//assert(socket->base().refCount() == 1); // ensure the socket base will be destroyed
	//pairs().free(pair->connectionID);

	TCPPeerConnection* peer = reinterpret_cast<TCPPeerConnection*>(sender);	
	peer->get()->Connected -= delegate(this, &TCPAllocation::onPeerConnectSuccess);
	peer->get()->Error -= delegate(this, &TCPAllocation::onPeerConnectError);	
	
	stun::Message response;
	response.setType(stun::Message::Connect);
	response.setTransactionID(peer->transactionID());			

	sendToControl(response);

	delete peer;
}
	*/
		
		 //.assign(static_cast<net::TCPSocket*>(&request.socket));
		 //static_cast<net::TCPBase*>()
		//UInt32 connectionID = util::randomNumber();
		//while (pairs().exists(connectionID))
		//	connectionID = util::randomNumber();
		//net::SocketBase* socket = static_cast<net::SocketBase*>(&request.socket);
		//TCPClientConnection* client = new TCPClientConnection(*this, *socket, peer); //, server().reactor()
		//client->Closed += delegate(this, &TCPAllocation::onClientDisconnect);
		//delete socket;
		//peers().exists(connAttr->value())

		/*
		TCPPeerConnection* peer = peers().get(connAttr->value()); //, false
		if (!peer)
			break;

		stun::Message response;
		response.setType(stun::Message::ConnectionBind);
		response.setTransactionID(request.transactionID());
		
		// Send the response back over the client connection
		request.socket.send(response);
				
		// Reassign the underlying socket implementation to the 
		// client connection and free the old socket pointer.
		
		//net::SocketBase* socket = static_cast<net::SocketBase*>(&request.socket);
		//TCPClientConnection* client = new TCPClientConnection(*this, *socket, peer); //, server().reactor()
		//client->Closed += delegate(this, &TCPAllocation::onClientDisconnect);
		//delete socket;
		*/
		
		 //.assign(static_cast<net::TCPSocket*>(&request.socket));
		 //static_cast<net::TCPBase*>()
		//UInt32 connectionID = util::randomNumber();
		//while (pairs().exists(connectionID))
		//	connectionID = util::randomNumber();
		//net::SocketBase* socket = static_cast<net::SocketBase*>(&request.socket);
		//TCPClientConnection* client = new TCPClientConnection(*this, *socket, peer); //, server().reactor()
		//client->Closed += delegate(this, &TCPAllocation::onClientDisconnect);
		//delete socket;
		//peers().exists(connAttr->value())

		/*
		TCPPeerConnection* peer = peers().get(connAttr->value()); //, false
		if (!peer)
			break;

		stun::Message response;
		response.setType(stun::Message::ConnectionBind);
		response.setTransactionID(request.transactionID());
		
		// Send the response back over the client connection
		request.socket.send(response);
				
		// Reassign the underlying socket implementation to the 
		// client connection and free the old socket pointer.
		
		//net::SocketBase* socket = static_cast<net::SocketBase*>(&request.socket);
		//TCPClientConnection* client = new TCPClientConnection(*this, *socket, peer); //, server().reactor()
		//client->Closed += delegate(this, &TCPAllocation::onClientDisconnect);
		//delete socket;
		*/

	
	/*

	auto pairs = this->pairs().map();	
	for (auto it = pairs.begin(); it != pairs.end(); ++it) {
		if (it->second->expired())
		   this->pairs().free(it->first);
	}

	// Here we clean up expired peer connections, nothing else. 
	// All other connection types are self maintaining.		
	PeerConnectionMap peers = this->peers().map();	
	for (PeerConnectionMap::iterator it = peers.begin(); it != peers.end(); ++it) {
		if (it->second->expired()) {
			assert(it->second->client() == nullptr);
			log("trace") << "Closing Expired Peer Connection: " << it->second << endl;	
			//it->second->Closed -= delegate(this, &TCPAllocation::onPeerDisconnected);
			it->second->close();
		}
	}

	// Here we clean up expired peer connections, nothing else. 
	// All other connection types are self maintaining.			
	auto& pairs = this->pairs().map();	
	auto it = pairs.begin();
	while (it != pairs.end()) {
		if (it->second.expired())
		   pairs.erase(it++);
		else
		   ++it;
	}

	TCPConnectionPairMap& pairs = this->pairs().map();	
	for (TCPConnectionPairMap::iterator it = pairs.begin(); it != pairs.end(); ++it) {
		if (it->second.expired()) {
			//assert(it->second->client() == nullptr);
			log("trace") << "Closing Expired Peer Connection: " << &it->second << endl;	
			it->second->Closed -= delegate(this, &TCPAllocation::onConnectionClosed);
			//pairs
			//it->second->close();
		}
	}
	*/



/*
ClientConnectionManager& TCPAllocation::clients()
{
	//Mutex::ScopedLock lock(_mutex);
	return _clients;
}


PeerConnectionManager& TCPAllocation::peers()
{
	//Mutex::ScopedLock lock(_mutex);
	return _peers;
}
*/


/*
void TCPAllocation::onPeerDisconnected(TCPPeerConnection* peer)
{
	log("trace") << "Peer Disconnected: " << peer << endl;
	net::SocketBase* sock = reinterpret_cast<net::SocketBase*>(sender);
	log("trace") << "Peer Disconnected 0: " << sock1 << endl;
	log("trace") << "Peer Disconnected 01: " << sock1->error() << endl;

	net::TCPSocket* sock = static_cast<net::TCPSocket*>(sender);
	log("trace") << "Peer Disconnected: " << sock << endl;
	log("trace") << "Peer Disconnected 1: " << sock->error() << endl;

	TCPPeerConnection* peer = static_cast<TCPPeerConnection*>(sender);
	log("trace") << "#### Peer Disconnected: " << peer->error() << endl;
	
	//delete peer;
}


void TCPAllocation::onClientDisconnect(TCPClientConnection* client)
{
	//TCPClientConnection* client = reinterpret_cast<TCPClientConnection*>(sender);
	log("trace") << "Client Disconnected: " << client << endl;	

	// Close the associated peer connection.
	TCPPeerConnection* peer = client->peer();
	if (peer) {
		peer->Closed -= delegate(this, &TCPAllocation::onPeerDisconnected);
		peer->close();
		//delete peer;
	}
	//delete client;
}
	*/