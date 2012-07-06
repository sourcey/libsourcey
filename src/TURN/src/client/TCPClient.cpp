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


#include "Sourcey/TURN/client/TCPClient.h"
#include "Sourcey/Logger.h"
#include "Poco/Format.h"

#include <assert.h>
#include <iostream>
#include <algorithm>


using namespace std;
using namespace Poco;
using namespace Poco::Net;


namespace Sourcey {
namespace TURN {


TCPClient::TCPClient(ITCPClientObserver& observer, Net::Reactor& reactor, Runner& runner, const Client::Options& options) : 
	Client(observer, reactor, runner, options), 
	_observer(observer)
{
	Log("trace") << "[TURN::TCPClient:" << this << "] Creating" << endl;
}


TCPClient::~TCPClient() 
{
	Log("trace") << "[TURN::TCPClient:" << this << "] Destroying" << endl;
	assert(_connections.empty());
	Log("trace") << "[TURN::TCPClient:" << this << "] Destroying: OK" << endl;
}


void TCPClient::initiate()
{
	Client::initiate();
}


void TCPClient::terminate()
{
	Log("trace") << "[TURN::TCPClient:" << this << "] Terminating" << endl;	

	Client::terminate();	
	Log("trace") << "[TURN::TCPClient:" << this << "] Terminating 1" << endl;
	{
		FastMutex::ScopedLock lock(_mutex);
		
		ConnectionManagerMap& items = _connections.items();
		ConnectionManagerMap::iterator it = items.begin();
		ConnectionManagerMap::iterator it2;
		Log("trace") << "[TURN::TCPClient:" << this << "] Terminating: Active Connections: " << items.size() << endl;	
		while (it != items.end()) {
			it2 = it++;		
			//(*it2).second->StateChange -= delegate(this, &SessionManager::onJingleSessionStateChange);
			//(*it2).second->terminate();
			Log("trace") << "[TURN::TCPClient:" << this << "] Terminating: Closing: " << it2->second << endl;	
			it2->second->detach(packetDelegate<TCPClient, DataPacket>(this, &TCPClient::onDataPacketReceived));
			it2->second->StateChange -= delegate(this, &TCPClient::onClientConnectionStateChange);	
			it2->second->close();
			//delete it2->second;
			items.erase(it2);
		}

		/*
		ConnectionManagerMap::iterator it = items.begin();
		Log("trace") << "[TURN::TCPClient:" << this << "] Terminating: Active Connections: " << items.size() << endl;	
		while (it != items.end()) {
			Log("trace") << "[TURN::TCPClient:" << this << "] Terminating: Closing: " << it->second << endl;	
			it->second->detach(packetDelegate<TCPClient, DataPacket>(this, &TCPClient::onDataPacketReceived));
			it->second->StateChange -= delegate(this, &TCPClient::onClientConnectionStateChange);	
			it->second->close();
			//delete it->second;
			it = items.erase(it);
		}
		*/
		assert(items.empty());
	}
	
	Log("trace") << "[TURN::TCPClient:" << this << "] Terminating: OK" << endl;
}


void TCPClient::sendConnectRequest(const Net::Address& peerAddress)
{
	// 4.3. Initiating a Connection
	// 
	// To initiate a TCP connection to a peer, a client MUST send a Connect
	// request over the control connection for the desired allocation.  The
	// Connect request MUST include an XOR-PEER-ADDRESS attribute containing
	// the transport address of the peer to which a connection is desired.
			
	Log("trace") << "[TURN::TCPClient:" << this << "] Sending Connect Request" << endl;	

	STUN::Transaction* transaction = createTransaction();
	transaction->request().setType(STUN::Message::Connect);

	STUN::XorPeerAddress* peerAttr = new STUN::XorPeerAddress;
	peerAttr->setFamily(1);
	peerAttr->setPort(peerAddress.port());
	peerAttr->setIP(peerAddress.host().toString());
	transaction->request().add(peerAttr);
	
	sendAuthenticatedTransaction(transaction);
}


void TCPClient::sendData(const char* data, int size, const Net::Address& peerAddress) 
{
	Log("trace") << "[TURN::TCPClient:" << this << "] Sending Data to peer: " 
		<< peerAddress.toString() << endl;

	// Ensure permissions exist for the peer.
	if (!hasPermission(peerAddress.host()))	
		throw Exception("No permission exists for peer: " + peerAddress.host().toString());	

	Net::TCPSocket* conn = connections().get(peerAddress);
	if (!conn)	
		throw Exception("No peer exists for: " + peerAddress.toString());

	conn->send(data, size);

	Log("trace") << "[TURN::TCPClient:" << this << "] Sending Data to peer: OK" << endl;
}


bool TCPClient::handleResponse(const STUN::Message& response)
{
	if (!Client::handleResponse(response)) {
		if (response.type() == STUN::Message::Connect &&
			response.state() == STUN::Message::SuccessResponse)	
			handleConnectResponse(response);
		
		else if (response.type() == STUN::Message::ConnectionAttempt)	
			handleConnectionAttemptIndication(response);

		else if (response.type() == STUN::Message::ConnectionBind &&
			response.state() == STUN::Message::SuccessResponse)	
			handleConnectionBindResponse(response);

		else if (response.type() == STUN::Message::ConnectionBind &&
			response.state() == STUN::Message::ErrorResponse)	
			handleConnectionBindErrorResponse(response);

		else if (response.type() == STUN::Message::Connect &&
			response.state() == STUN::Message::ErrorResponse)	
			handleConnectErrorResponse(response);

		else
			return false;
	}

	return true;
}


void TCPClient::handleConnectResponse(const STUN::Message& response)
{
	// If the connection is successfully established, the client will
	// receive a success response.  That response will contain a
	// CONNECTION-ID attribute.  The client MUST initiate a new TCP
	// connection to the server, utilizing the same destination transport
	// address to which the control connection was established.  This
	// connection MUST be made using a different local transport address.
	// Authentication of the client by the server MUST use the same method
	// and credentials as for the control connection.  Once established, the
	// client MUST send a ConnectionBind request over the new connection.
	// That request MUST include the CONNECTION-ID attribute, echoed from
	// the Connect Success response.  When a response to the ConnectionBind
	// request is received, if it is a success, the TCP connection on which
	// it was sent is called the client data connection corresponding to the
	// peer.
	//
	
	STUN::Transaction* transaction = reinterpret_cast<STUN::Transaction*>(response.opaque);
	const STUN::XorPeerAddress* peerAttr = transaction->request().get<STUN::XorPeerAddress>();
	if (!peerAttr || peerAttr && peerAttr->family() != 1) {
		assert(false);
		return;
	}

	STUN::ConnectionID* connAttr = response.get<STUN::ConnectionID>();
	if (!connAttr) {
		assert(false);
		return;
	}

	createAndBindConnection(connAttr->value(), peerAttr->address());
}


void TCPClient::handleConnectErrorResponse(const STUN::Message& response)
{
	// If the result of the Connect request was an Error Response, and the
	// response code was 447 (Connection Timeout or Failure), it means that
	// the TURN server was unable to connect to the peer.  The client MAY
	// retry with the same XOR-PEER-ADDRESS attribute, but MUST wait at
	// least 10 seconds.
	// 
	// As with any other request, multiple Connect requests MAY be sent
	// simultaneously.  However, Connect requests with the same XOR-PEER-
	// ADDRESS parameter MUST NOT be sent simultaneously.
		
	STUN::Transaction* transaction = reinterpret_cast<STUN::Transaction*>(response.opaque);
	const STUN::XorPeerAddress* peerAttr = transaction->request().get<STUN::XorPeerAddress>();
	if (!peerAttr || peerAttr && peerAttr->family() != 1) {
		assert(false);
		return;
	}
	
	_observer.onClientConnectionBindingFailed(*this, peerAttr->address());
}


void TCPClient::handleConnectionAttemptIndication(const STUN::Message& response)
{
	// 4.4. Receiving a Connection
	// 
	// After an Allocate request is successfully processed by the server,
	// the client will start receiving a ConnectionAttempt indication each
	// time a peer for which a permission has been installed attempts a new
	// connection to the relayed transport address.  This indication will
	// contain CONNECTION-ID and XOR-PEER-ADDRESS attributes.  If the client
	// wishes to accept this connection, it MUST initiate a new TCP
	// connection to the server, utilizing the same destination transport
	// address to which the control connection was established.  This
	// connection MUST be made using a different local transport address.
	// Authentication of the client by the server MUST use the same method
	// and credentials as for the control connection.  Once established, the
	// client MUST send a ConnectionBind request over the new connection.
	// That request MUST include the CONNECTION-ID attribute, echoed from
	// the ConnectionAttempt indication.  When a response to the
	// ConnectionBind request is received, if it is a success, the TCP
	// connection on which it was sent is called the client data connection
	// corresponding to the peer.

	const STUN::XorPeerAddress* peerAttr = response.get<STUN::XorPeerAddress>();
	if (!peerAttr || peerAttr && peerAttr->family() != 1) {
		assert(false);
		return;
	}

	const STUN::ConnectionID* connAttr = response.get<STUN::ConnectionID>();
	if (!connAttr) {
		assert(false);
		return;
	}
	
	if (_observer.onPeerConnectionAttempt(*this, peerAttr->address())) //connAttr->value(), 
		createAndBindConnection(connAttr->value(), peerAttr->address());	
}


void TCPClient::handleConnectionBindResponse(const STUN::Message& response)
{
	Log("trace") << "[TURN::TCPClient:" << this << "] Connection Bind Success Response" << endl;	

	STUN::Transaction* transaction = reinterpret_cast<STUN::Transaction*>(response.opaque);
	Net::TCPStatefulSocket* conn = static_cast<Net::TCPStatefulSocket*>(transaction->socket());

	Net::Address peerAddress;
	if (!getPeerAddress(conn, peerAddress)) {
		assert(false);
		return;
	}

	// We no longer want to receive STUN packets.
	// Data will be transferred as-is to and from 
	// the peer.
	conn->unregisterPacketType<STUN::Message>();
	conn->registerPacketType<DataPacket>(1);
	conn->attach(packetDelegate<TCPClient, DataPacket>(this, &TCPClient::onDataPacketReceived));
	//conn->setState(Net::ClientState::Online);

	_observer.onClientConnectionCreated(*this, conn, peerAddress);
}


void TCPClient::handleConnectionBindErrorResponse(const STUN::Message& response)
{
	Log("trace") << "[TURN::TCPClient:" << this << "] Connection Bind Error" << endl;

	STUN::ConnectionID* connAttr = response.get<STUN::ConnectionID>();
	if (!connAttr) {
		assert(false);
		return;
	}
	
	STUN::Transaction* transaction = reinterpret_cast<STUN::Transaction*>(response.opaque);
	Net::TCPStatefulSocket* conn = static_cast<Net::TCPStatefulSocket*>(transaction->socket());
	conn->StateChange -= delegate(this, &TCPClient::onClientConnectionStateChange);	
	FastMutex::ScopedLock lock(_mutex);	
	if (!isTerminated()) {
		conn->close();
		_connections.remove(conn);
		//delete conn;
	}
}


bool TCPClient::createAndBindConnection(UInt32 connectionID, const Net::Address& peerAddress)
{
	FastMutex::ScopedLock lock(_mutex);
	if (isTerminated())
		return false;

	Net::TCPStatefulSocket* conn = new Net::TCPStatefulSocket(_reactor); //, false
	Log("trace") << "[TURN::TCPClient:" << this << "] @@@@@@@@@@@createAndBindConnection: conn: " << conn << endl;	
	try {
		// Temporarily accept STUN packets so we can 
		// verify the ConnectionBind response.
		conn->setDeleteOnClose(true);
		conn->registerPacketType<STUN::Message>(1);
		conn->StateChange += delegate(this, &TCPClient::onClientConnectionStateChange);	
		conn->connect(_options.serverAddr); // will throw on error
		
		STUN::Transaction* transaction = createTransaction(conn);
		transaction->request().setType(STUN::Message::ConnectionBind);

		STUN::ConnectionID* connAttr = new STUN::ConnectionID;
		connAttr->setValue(connectionID);
		transaction->request().add(connAttr);
		
		assert(transaction->socket() == conn);
		sendAuthenticatedTransaction(transaction);

		_connections.add(peerAddress, conn);

		return true;
	} 
	catch (Exception& exc) {	
		// instance deleted via state callback
		Log("error") << "[TURN::TCPClient" << this << "] Connection Bind Error: " << exc.displayText() << endl;
	}

	return false;
}


void TCPClient::onClientConnectionStateChange(void* sender, Net::ClientState& state, const Net::ClientState& oldState)
{	
	Net::TCPStatefulSocket* conn = reinterpret_cast<Net::TCPStatefulSocket*>(sender);	
	Log("trace") << "[TURN::TCPClient:" << this << "] Client Connection State Changed: " << state.toString() << endl;
	{
		FastMutex::ScopedLock lock(_mutex);
		if (isTerminated())
			return;
	}	
	
	if (state.id() == Net::ClientState::Disconnected) {	
		Net::Address peerAddress;
		if (getPeerAddress(conn, peerAddress)) {
			_observer.onClientConnectionClosed(*this, conn, peerAddress);
			connections().remove(conn);
		} else
			assert(false);
	}

	_observer.onClientConnectionState(*this, conn, state, oldState);
}


void TCPClient::onDataPacketReceived(void* sender, DataPacket& packet)
{
	Net::TCPStatefulSocket* conn = static_cast<Net::TCPStatefulSocket*>(sender);	
	Net::Address peerAddress;
	if (!isTerminated() && getPeerAddress(conn, peerAddress))
		_observer.onRelayedData(*this, (const char *)packet.data(), packet.size(), peerAddress);
	else
		Log("warn") << "[TURN::TCPClient" << this << "] No Peer: Data Packet Dropped" << endl;	
}


bool TCPClient::getPeerAddress(Net::TCPStatefulSocket* conn, Net::Address& peerAddress)
{
	ConnectionManagerMap conns = connections().items();
	for (ConnectionManagerMap::const_iterator it = conns.begin(); it != conns.end(); ++it) {
		if (it->second == conn) {
			peerAddress = it->first;
			return true;
		}
	}
	return false;
}


Net::ISocket* TCPClient::createSocket()
{
	FastMutex::ScopedLock lock(_mutex);
	return new Net::TCPStatefulSocket(_reactor);
}


ConnectionManager& TCPClient::connections()
{
	FastMutex::ScopedLock lock(_mutex);
	return _connections;
}


int TCPClient::transportProtocol()
{
	return 6; // TCP
}


} } //  namespace Sourcey::TURN