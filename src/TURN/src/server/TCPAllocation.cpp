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


#include "Sourcey/TURN/server/TCPAllocation.h"
#include "Sourcey/TURN/server/Server.h"
#include "Sourcey/Net/TCPAsyncConnector.h"
#include "Sourcey/Logger.h"


using namespace std;
using namespace Poco;


namespace Sourcey {
namespace TURN {


TCPAllocation::TCPAllocation(Server& server, Net::TCPSocket* control, const FiveTuple& tuple, const string& username, const UInt32& lifetime) : 
	ServerAllocation(server, tuple, username, lifetime),
	_acceptor(server.reactor()),
	_control(control)
{
	// Handle data from the relay socket directly from the
	// allocation. This will remove the need for allocation
	// lookups when receiving data from peers.
	_acceptor.bind(Net::Address(server.options().listenAddr.host(), 0));
	_acceptor.SocketAccepted += delegate(this, &TCPAllocation::onPeerAccepted);
	
	// The allocation will be closed if the control connection is lost.
	_control->Closed += delegate(this, &TCPAllocation::onControlDisconnect);

	Log("trace", this) << "[TCPAllocation:" << this << "]  " << this << ": Initializing on address: " << _acceptor.address().toString() << endl;
}
	

TCPAllocation::~TCPAllocation() 
{
	Log("trace", this) << "[TCPAllocation:" << this << "] Destroying" << endl;	
	
	_acceptor.SocketAccepted -= delegate(this, &TCPAllocation::onPeerAccepted);
	_acceptor.close();
	
	if (_control) {
		_control->Closed -= delegate(this, &TCPAllocation::onControlDisconnect);	
		_control->close();
	}
	
	PeerConnectionMap& peers = _peers.items();	
	for (PeerConnectionMap::iterator it = peers.begin(); it != peers.end(); ++it) {
		Log("trace", this) << "[TCPAllocation:" << this << "] Destroying: Peer: " << it->second << endl;	
		//it->second->Connected.detachAll(this);
		//it->second->Closed.detachAll(this);
		it->second->Closed -= delegate(this, &TCPAllocation::onPeerDisconnected);	
		it->second->close();
		//delete it->second;
	}

	ClientConnectionMap& clients = _clients.items();	
	for (ClientConnectionMap::iterator it = clients.begin(); it != clients.end(); ++it) {
		Log("trace", this) << "[TCPAllocation:" << this << "] Destroying: Client: " << it->second << endl;	
		it->second->Closed -= delegate(this, &TCPAllocation::onClientDisconnect);	
		it->second->close();
		//delete it->second;
	}

	Log("trace", this) << "[TCPAllocation:" << this << "] Destroying: OK" << endl;	
}


void TCPAllocation::onPeerAccepted(void* sender, Poco::Net::StreamSocket& socket, Net::Reactor& reactor)
{
	Log("trace", this) << "[TCPAllocation:" << this << "] Socket Accepted: " << socket.peerAddress().toString() << endl;
	
	// 5.3. Receiving a TCP Connection on a Relayed Transport Address
	// 
	// When a server receives an incoming TCP connection on a relayed
	// transport address, it processes the request as follows.
	// 
	// The server MUST accept the connection.  If it is not successful,
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
		Log("trace", this) << "[TCPAllocation:" << this << "] No permission for peer: " << socket.peerAddress().toString() << endl;
		return;
	}

	// Otherwise, the server sends a ConnectionAttempt indication to the
	// client over the control connection. The indication MUST include an
	// XOR-PEER-ADDRESS attribute containing the peer's transport address,
	// as well as a CONNECTION-ID attribute uniquely identifying the peer
	// data connection.
	// 	
	TCPPeerConnection* peer = new TCPPeerConnection(*this, socket, reactor);
	peer->Closed += delegate(this, &TCPAllocation::onPeerDisconnected);
	
	STUN::Message response;
	response.setType(STUN::Message::ConnectionAttempt);

	STUN::XorPeerAddress* addrAttr = new STUN::XorPeerAddress;
	addrAttr->setFamily(1);
	addrAttr->setPort(socket.peerAddress().port());
	addrAttr->setIP(socket.peerAddress().host().toString());
	response.add(addrAttr);

	STUN::ConnectionID* connAttr = new STUN::ConnectionID;
	connAttr->setValue(peer->connectionID());
	response.add(connAttr);
  
	sendToControl(response);
}


bool TCPAllocation::handleRequest(const Request& request) 
{	
	Log("trace", this) << "[TCPAllocation:" << this << "] Handle Request" << endl;	

	if (!ServerAllocation::handleRequest(request)) {
		if (request.type() == STUN::Message::Connect)
			handleConnectRequest(request);
		else if (
			request.type() == STUN::Message::ConnectionBind)
			handleConnectionBindRequest(request);
		else
			return false;
	}
	
	return true; 
}


bool TCPAllocation::isOK()
{	
	// Kill the allocation if our control connection goes down.
	if (!control())
		return false;

	return ServerAllocation::isOK();
}


bool TCPAllocation::onTimer() 
{
	//Log("trace", this) << "[TCPAllocation:" << this << "] On Timer" << endl;

	// Here we clean up expired peer connections, 
	// nothing else. All other connection types
	// are self maintaining.		
	PeerConnectionMap peers = this->peers().copy();	
	for (PeerConnectionMap::iterator it = peers.begin(); it != peers.end(); ++it) {
		if (it->second->expired()) {
			assert(it->second->client() == NULL);
			Log("trace", this) << "[TCPAllocation:" << this << "] Destroying: Peer: " << it->second << endl;	
			it->second->Closed -= delegate(this, &TCPAllocation::onPeerDisconnected);
			it->second->close();
			//delete it->second;
		}
	}

	return ServerAllocation::onTimer();
}		


void TCPAllocation::handleConnectRequest(const Request& request)
{
	Log("trace", this) << "[TCPAllocation:" << this << "] Handle Connect Request" << endl;

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
	const STUN::XorPeerAddress* peerAttr = request.get<STUN::XorPeerAddress>();
	if (!peerAttr || peerAttr && peerAttr->family() != 1) {
		_server.sendError(request, 400, "Bad Request");
		return;
	}

	// Otherwise, the server MUST initiate an outgoing TCP connection.  The
	// local endpoint is the relayed transport address associated with the
	// allocation.  The remote endpoint is the one indicated by the XOR-
	// PEER-ADDRESS attribute.  If the connection attempt fails or times
	// out, the server MUST return a 447 (Connection Timeout or Failure)
	// error.  The timeout value MUST be at least 30 seconds.
	// 	
	TCPPeerConnection* peer = new TCPPeerConnection(*this, _server.reactor());
	peer->setTransactionID(request.transactionID());
	peer->Connected += delegate(this, &TCPAllocation::onPeerConnectSuccess);
	peer->Error += delegate(this, &TCPAllocation::onPeerConnectError);	
	(void*)new Net::TCPAsyncConnector<TCPPeerConnection>(*peer, 
		peerAttr->address(), PEER_CONNECTION_TIMEOUT);
}


void TCPAllocation::onPeerConnectSuccess(void* sender)
{
	Log("trace", this) << "[TURNClient:" << this << "] Peer Connect Success" << endl;

	TCPPeerConnection* peer = reinterpret_cast<TCPPeerConnection*>(sender);	
	peer->Connected -= delegate(this, &TCPAllocation::onPeerConnectSuccess);
	peer->Error -= delegate(this, &TCPAllocation::onPeerConnectError);		
	peer->Closed += delegate(this, &TCPAllocation::onPeerDisconnected);	
	
	// If the connection is successful, it is now called a peer data
	// connection.  The server MUST buffer any data received from the
	// client.  The server adjusts its advertised TCP receive window to
	// reflect the amount of empty buffer space.
	// 
	// The server MUST include the CONNECTION-ID attribute in the Connect
	// success response. The attribute's value MUST uniquely identify the
	// peer data connection.
	// 
	STUN::Message response;
	response.setType(STUN::Message::Connect);
	response.setTransactionID(peer->transactionID());

	STUN::ConnectionID* connAttr = new STUN::ConnectionID;
	connAttr->setValue(peer->connectionID());
	response.add(connAttr);
  
	sendToControl(response);
	
	// If no ConnectionBind request associated with this peer data
	// connection is received after 30 seconds, the peer data connection
	// MUST be closed.
	//
	peer->startTimeout();
}


void TCPAllocation::onPeerConnectError(void* sender, int& errnum, const string& error)
{
	Log("trace", this) << "[TURNClient:" << this << "] Peer Connect Error" << endl;

	TCPPeerConnection* peer = reinterpret_cast<TCPPeerConnection*>(sender);	
	peer->Connected -= delegate(this, &TCPAllocation::onPeerConnectSuccess);
	peer->Error -= delegate(this, &TCPAllocation::onPeerConnectError);	
	
	STUN::Message response;
	response.setType(STUN::Message::Connect);
	response.setTransactionID(peer->transactionID());			

	STUN::ErrorCode* errorCodeAttr = new STUN::ErrorCode();
	errorCodeAttr->setErrorCode(447);
	errorCodeAttr->setReason("Connection Timeout or Failure");
	response.add(errorCodeAttr);

	sendToControl(response);

	delete peer;
}


void TCPAllocation::handleConnectionBindRequest(const Request& request) 
{
	Log("trace", this) << "[TCPAllocation:" << this << "] Handle ConnectionBind Request" << endl;
	
	assert(request.type() == STUN::Message::ConnectionBind);

	do {
		// 5.4. Receiving a ConnectionBind Request
		// 
		// 
		// When a server receives a ConnectionBind request, it processes the
		// request as follows.
		// 
		// If the client connection transport is not TCP or TLS, the server MUST
		// return a 400 (Bad Request) error.
		// 
		if (request.socket.transport() != Net::TCP) // TODO: TLS!!
			break;

		// If the request does not contain the CONNECTION-ID attribute, or if
		// this attribute does not refer to an existing pending connection, the
		// server MUST return a 400 (Bad Request) error.
		// 
		const STUN::ConnectionID* connAttr = request.get<STUN::ConnectionID>();
		if (!connAttr)
			break;

		// Otherwise, the client connection is now called a client data
		// connection.  Data received on it MUST be sent as-is to the associated
		// peer data connection.
		// 
		// Data received on the associated peer data connection MUST be sent
		// as-is on this client data connection.  This includes data that was
		// received after the associated Connect or request was successfully
		// processed and before this ConnectionBind request was received.
		//
		TCPPeerConnection* peer = _peers.get(connAttr->value());
		if (!peer)
			break;

		STUN::Message response;
		response.setType(STUN::Message::ConnectionBind);
		response.setTransactionID(request.transactionID());
		
		// Send the response back over the client connection
		request.socket.send(response);
				
		// Reassign the underlying socket to the client connection and
		// delete the old socket pointer.
		Net::TCPSocket* socket = static_cast<Net::TCPSocket*>(&request.socket);
		TCPClientConnection* client = new TCPClientConnection(*this, *socket, peer, _server.reactor());
		client->Closed += delegate(this, &TCPAllocation::onClientDisconnect);
		delete socket;
		return;

	} while(0);
	
	_server.sendError(request, 400, "Bad Request");

	// Close the incoming connection
	request.socket.close();
}


void TCPAllocation::sendToControl(STUN::Message& message)
{
	FastMutex::ScopedLock lock(_mutex);
	if (_control)
		_control->send(message);
}


void TCPAllocation::onControlDisconnect(void* sender)
{
	FastMutex::ScopedLock lock(_mutex);
	Log("trace", this) << "Control Socket Disconnected: " << _control->peerAddress() << endl;
	assert(_control == sender);
	_control = NULL;
}


void TCPAllocation::onPeerDisconnected(void* sender)
{
	TCPPeerConnection* peer = reinterpret_cast<TCPPeerConnection*>(sender);
	Log("trace", this) << "Peer Disconnected: " << peer << endl;
	
	// Close the associated client connection.
	TCPClientConnection* client = peer->client();
	if (client) {
		client->Closed -= delegate(this, &TCPAllocation::onClientDisconnect);
		client->close();
		//delete client;
	}
	//delete peer;
}


void TCPAllocation::onClientDisconnect(void* sender)
{
	TCPClientConnection* client = reinterpret_cast<TCPClientConnection*>(sender);
	Log("trace", this) << "Client Disconnected: " << client << endl;	

	// Close the associated peer connection.
	TCPPeerConnection* peer = client->peer();
	if (peer) {
		peer->Closed -= delegate(this, &TCPAllocation::onPeerDisconnected);
		peer->close();
		//delete peer;
	}
	//delete client;
}


Net::TCPSocket* TCPAllocation::control()
{
	FastMutex::ScopedLock lock(_mutex);
	return _control;
}


ClientConnectionManager& TCPAllocation::clients()
{
	FastMutex::ScopedLock lock(_mutex);
	return _clients;
}


PeerConnectionManager& TCPAllocation::peers()
{
	FastMutex::ScopedLock lock(_mutex);
	return _peers;

}


Net::Address TCPAllocation::relayedAddress() const 
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _acceptor.address();
}


} } //  namespace Sourcey::TURN