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


#include "scy/turn/client/tcpclient.h"
#include "scy/logger.h"
#include "scy/net/tcpsocket.h"
//#include "Poco/Format.h"

#include <assert.h>
#include <iostream>
#include <algorithm>


using namespace std;


namespace scy {
namespace turn {


TCPClient::TCPClient(TCPClientObserver& observer, const Client::Options& options) : 
	Client(observer, options), 
	_observer(observer)
{
	TraceL << "Create" << endl;
	_socket.assign(new net::TCPBase, false);
}

	
TCPClient::~TCPClient() 
{
	TraceL << "Destroy" << endl;
	assert(_connections.empty());
}


void TCPClient::initiate()
{
	Client::initiate();
}


void TCPClient::shutdown()
{
	TraceL << "Shutdown" << endl;	

	//if (closed()) {
	//	TraceL << "Already closed" << endl;	
	//	return;
	//}

	// Destroy transactions and stop timer
	Client::shutdown();

	{		
		//Mutex::ScopedLock lock(_mutex);	
		auto connections = _connections.map();
		TraceL << "Shutdown: Active connections: " << connections.size() << endl;	
		for (auto it = connections.begin(); it != connections.end(); ++it) {
			assert(it->second->base().refCount() == 1);

			// The connection will be removed via onRelayConnectionClosed
			it->second->close();
		}
		//assert(_connections.empty());
	}
	
	TraceL << "Shutdown: OK" << endl;
}


void TCPClient::sendConnectRequest(const net::Address& peerAddress)
{
	// 4.3. Initiating a Connection
	// 
	// To initiate a TCP connection to a peer, a client MUST send a Connect
	// request over the control connection for the desired allocation.  The
	// Connect request MUST include an XOR-PEER-ADDRESS attribute containing
	// the transport address of the peer to which a connection is desired.
			
	TraceL << "Send Connect request" << endl;	

	auto transaction = createTransaction();
	//transaction->request().setType(stun::Message::Connect);
	//stun::Message request(stun::Message::Request, stun::Message::Allocate);
	transaction->request().setClass(stun::Message::Request);
	transaction->request().setMethod(stun::Message::Connect);

	auto peerAttr = new stun::XorPeerAddress;
	peerAttr->setAddress(peerAddress);
	//peerAttr->setFamily(1);
	//peerAttr->setPort(peerAddress.port());
	//peerAttr->setIP(peerAddress.host());
	transaction->request().add(peerAttr);
	
	sendAuthenticatedTransaction(transaction);
}


void TCPClient::sendData(const char* data, int size, const net::Address& peerAddress) 
{
	TraceL << "Send data to " << peerAddress << endl;

	// Ensure permissions exist for the peer.
	if (!hasPermission(peerAddress.host()))	
		throw std::runtime_error("No permission exists for peer: " + peerAddress.host());	

	auto conn = connections().get(peerAddress, false);
	if (!conn)	
		throw std::runtime_error("No peer exists for: " + peerAddress.toString());
	
	conn->send(data, size);
}


bool TCPClient::handleResponse(const stun::Message& response)
{
	if (!Client::handleResponse(response)) {
		if (response.methodType() ==  stun::Message::Connect &&
			response.classType() == stun::Message::SuccessResponse)	
			handleConnectResponse(response);
		
		else if (response.methodType() ==  stun::Message::ConnectionAttempt)	
			handleConnectionAttemptIndication(response);

		else if (response.methodType() ==  stun::Message::ConnectionBind &&
			response.classType() == stun::Message::SuccessResponse)	
			handleConnectionBindResponse(response);

		else if (response.methodType() ==  stun::Message::ConnectionBind &&
			response.classType() == stun::Message::ErrorResponse)	
			handleConnectionBindErrorResponse(response);

		else if (response.methodType() ==  stun::Message::Connect &&
			response.classType() == stun::Message::ErrorResponse)	
			handleConnectErrorResponse(response);

		else
			return false;
	}

	return true;
}


void TCPClient::handleConnectResponse(const stun::Message& response)
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
	
	auto transaction = reinterpret_cast<stun::Transaction*>(response.opaque);
	auto peerAttr = transaction->request().get<stun::XorPeerAddress>();
	if (!peerAttr || (peerAttr && peerAttr->family() != 1)) {
		assert(0);
		return;
	}

	auto connAttr = response.get<stun::ConnectionID>();
	if (!connAttr) {
		assert(0);
		return;
	}

	createAndBindConnection(connAttr->value(), peerAttr->address());
}


void TCPClient::handleConnectErrorResponse(const stun::Message& response)
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
		
	auto transaction = reinterpret_cast<stun::Transaction*>(response.opaque);
	auto peerAttr = transaction->request().get<stun::XorPeerAddress>();
	if (!peerAttr || (peerAttr && peerAttr->family() != 1)) {
		assert(0);
		return;
	}
	
	_observer.onRelayConnectionBindingFailed(*this, peerAttr->address());
}


void TCPClient::handleConnectionAttemptIndication(const stun::Message& response)
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

	auto peerAttr = response.get<stun::XorPeerAddress>();
	if (!peerAttr || (peerAttr && peerAttr->family() != 1)) {
		assert(0);
		return;
	}

	auto connAttr = response.get<stun::ConnectionID>();
	if (!connAttr) {
		assert(0);
		return;
	}
	
	if (_observer.onPeerConnectionAttempt(*this, peerAttr->address())) //connAttr->value(), 
		createAndBindConnection(connAttr->value(), peerAttr->address());	
}


void TCPClient::handleConnectionBindResponse(const stun::Message& response)
{
	TraceL << "ConnectionBind success response" << endl;	

	auto transaction = reinterpret_cast<stun::Transaction*>(response.opaque);
	auto req = reinterpret_cast<RelayConnectionBinding*>(transaction->socket->base().opaque);
	
	auto conn = connections().get(req->peerAddress, false);
	if (!conn) {
		assert(0);
		return;
	}
	
	// Data will now be transferred as-is to and from the peer.
	conn->Recv += delegate(this, &TCPClient::onRelayDataReceived);
	_observer.onRelayConnectionCreated(*this, *conn, req->peerAddress);

	TraceL << "ConnectionBind success response: OK" << endl;
}


void TCPClient::handleConnectionBindErrorResponse(const stun::Message& response)
{
	TraceL << "ConnectionBind error response" << endl;
	
	auto transaction = reinterpret_cast<stun::Transaction*>(response.opaque);
	auto req = reinterpret_cast<RelayConnectionBinding*>(transaction->socket->base().opaque);

	//auto connAttr = response.get<stun::ConnectionID>();
	//if (!connAttr) {
	//	assert(0);
	//	return;
	//}

	auto conn = connections().get(req->peerAddress, false);
	if (!conn) {
		assert(0);
		return;
	}

	freeConnection(conn);
		
	/*
	conn->Connect += delegate(this, &TCPClient::onRelayConnectionConnect);
	conn->Error += delegate(this, &TCPClient::onRelayConnectionError);
	conn->Close += delegate(this, &TCPClient::onRelayConnectionClosed);
	
	conn->close();
	_connections.remove(conn);
	*/

	//assert(0 && "redo");

	/*	
	auto transaction = reinterpret_cast<stun::Transaction*>(response.opaque);
	//auto socket = reinterpret_cast<net::TCPSocket*>(sender);
	net::TCPSocket& conn = connections().get(transaction->socket()->address());
	//net::TCPSocket& conn = static_cast<net::TCPStatefulSocketBase*>(transaction->socket());
	conn->StateChange -= delegate(this, &TCPClient::onRelayConnectionStateChange);	
	//Mutex::ScopedLock lock(_mutex);	
	if (!closed()) {
		conn->close();
		_connections.remove(conn);
		//delete conn;
	}
	*/
}


bool TCPClient::createAndBindConnection(UInt32 connectionID, const net::Address& peerAddress)
{
	//assert (!closed());
	//Mutex::ScopedLock lock(_mutex);

	TraceL << "Create and bind connection: " << peerAddress << endl;	
	
	net::TCPSocket* conn = nullptr;
	try {
		conn = new net::TCPSocket; 
		conn->Connect += delegate(this, &TCPClient::onRelayConnectionConnect);
		conn->Error += delegate(this, &TCPClient::onRelayConnectionError);
		conn->Close += delegate(this, &TCPClient::onRelayConnectionClosed);

		auto req = new RelayConnectionBinding;
		req->connectionID = connectionID;
		req->peerAddress = peerAddress;
		conn->base().opaque = req;	

		conn->connect(_options.serverAddr); // will throw on error
		
		_connections.add(peerAddress, conn);
		return true;
	} 
	catch (std::exception& exc) {	
		// Socket instance deleted via state callback
		ErrorL << "ConnectionBind Error: " << exc.what() << endl;
		delete conn;
	}

	return false;
}


void TCPClient::onRelayConnectionConnect(void* sender)
{		
	TraceL << "onRelayConnectionConnect" << endl;	
	
	auto conn =reinterpret_cast<net::TCPSocket*>(sender);
	conn->Connect -= delegate(this, &TCPClient::onRelayConnectionConnect);
	auto req = reinterpret_cast<RelayConnectionBinding*>(conn->base().opaque);
	assert(_connections.exists(req->peerAddress));

	// TODO: How to get peerAddress here?
	//net::TCPSocket& socket = _connections.get(peerAddress, conn);

	auto transaction = createTransaction(conn);
	transaction->request().setClass(stun::Message::Request);
	transaction->request().setMethod(stun::Message::ConnectionBind);

	auto connAttr = new stun::ConnectionID;
	connAttr->setValue(req->connectionID);
	transaction->request().add(connAttr);
		
	//assert(transaction->socket() == &conn);
	sendAuthenticatedTransaction(transaction);
}


void TCPClient::onRelayConnectionError(void* sender, const Error& /* error */) 
{
	auto socket = reinterpret_cast<net::TCPSocket*>(sender);
	auto req = reinterpret_cast<RelayConnectionBinding*>(socket->base().opaque);

	TraceL << "Relay connection error: " << req->peerAddress << endl;	
	assert(_connections.exists(req->peerAddress));

	_observer.onRelayConnectionError(*this, *socket, req->peerAddress);
}


void TCPClient::onRelayConnectionClosed(void* sender)
{	
	auto socket = reinterpret_cast<net::TCPSocket*>(sender);
	auto req = reinterpret_cast<RelayConnectionBinding*>(socket->base().opaque);
	
	TraceL << "Relay connection closed: " << req->peerAddress << endl;	
	assert(_connections.exists(req->peerAddress));
	
	_observer.onRelayConnectionClosed(*this, *socket, req->peerAddress);
	freeConnection(socket);
}


void TCPClient::onRelayDataReceived(void* sender, net::SocketPacket& packet)
{
	auto socket = reinterpret_cast<net::TCPSocket*>(sender);
	auto req = reinterpret_cast<RelayConnectionBinding*>(socket->base().opaque);
	assert(_connections.exists(req->peerAddress));
	
	_observer.onRelayDataReceived(*this, packet.data(), packet.size(), req->peerAddress);
}


void TCPClient::freeConnection(net::TCPSocket* socket)
{
	TraceL << "Freeing TCP connection: " << socket << endl;	
	socket->Recv -= delegate(this, &TCPClient::onRelayDataReceived);
	socket->Connect -= delegate(this, &TCPClient::onRelayConnectionConnect);
	socket->Error -= delegate(this, &TCPClient::onRelayConnectionError);
	socket->Close -= delegate(this, &TCPClient::onRelayConnectionClosed);

	assert(socket->base().refCount() == 1);
	assert(connections().exists(socket));
	connections().remove(socket);

	delete reinterpret_cast<RelayConnectionBinding*>(socket->base().opaque);
	//delete socket;
	deleteLater<net::TCPSocket>(socket); // deferred
}


ConnectionManager& TCPClient::connections()
{
	//Mutex::ScopedLock lock(_mutex);
	return _connections;
}


int TCPClient::transportProtocol()
{
	return 6; // TCP
}


} } //  namespace scy::turn


	
	
	/*
	
	// TODO: catch and handle
	//net::TCPSocket& conn = connections().get(peerAddress);
	assert(_connections.exists(req->peerAddress));
	net::Address peerAddress;
	if (!getPeerAddress(socket, peerAddress)) {
		WarnL << "No connection for socket: " 
			<< socket << std::endl;
		// This can happen if the sockets were terminated 
		// suddenly on client destruction.
		assert(0);
		return;
	}
	*/
	/*
	// We no longer want to receive STUN packets on the peer connection.
	// Data will be transferred as-is to and from the peer.
	socket->Recv += delegate(this, &TCPClient::onRelayDataReceived);
	_observer.onRelayConnectionCreated(*this, *socket, req->peerAddress);

	// We no longer want to receive STUN packets.
	// Data will be transferred as-is to and from the peer.
	//conn->unregisterPacketType<stun::Message>();
	//conn->registerPacketType<RawPacket>(1);
	//conn->attach(packetDelegate<TCPClient, RawPacket>(this, &TCPClient::onRelayDataReceived));
	//conn->setState(Net::SocketState::Online);


	//assert(0 && "redo");
	//net::TCPSocket& conn = connections().get(peerAddress);
	
	// We no longer want to receive STUN packets on the peer connection.
	// Data will be transferred as-is to and from the peer.
	//conn.Recv += delegate(this, &TCPClient::onRelayDataReceived);
	//assert(conn);

	// We no longer want to receive STUN packets.
	// Data will be transferred as-is to and from 
	// the peer.
	//conn->unregisterPacketType<stun::Message>();
	//conn->registerPacketType<RawPacket>(1);
	//conn->attach(packetDelegate<TCPClient, RawPacket>(this, &TCPClient::onRelayDataReceived));
	//conn->setState(Net::SocketState::Online);

	//_observer.onRelayConnectionCreated(*this, conn, peerAddress);
	*/
	
	// Temporarily accept STUN packets so we can 
	// verify the ConnectionBind response.
	/*
	//conn->setDeleteOnClose(true);		
	//conn->registerPacketType<stun::Message>(1);
	//conn->StateChange += delegate(this, &TCPClient::onRelayConnectionStateChange);	
	//conn->Recv += delegate(this, &TCPClient::onConnectionBindSocketRecv);


	net::TCPSocket& conn = new net::TCPStatefulSocketBase(_reactor); 
	TraceL << "Create And Bind Connection: " << conn << endl;	
	try {
		// Temporarily accept STUN packets so we can 
		// verify the ConnectionBind response.
		conn->setDeleteOnClose(true);		
		conn->registerPacketType<stun::Message>(1);
		conn->StateChange += delegate(this, &TCPClient::onRelayConnectionStateChange);	
		conn->connect(_options.serverAddr); // will throw on error
		
		auto transaction = createTransaction(conn);
		transaction->request().setType(stun::Message::ConnectionBind);

		auto connAttr = new stun::ConnectionID;
		connAttr->setValue(connectionID);
		transaction->request().add(connAttr);
		
		assert(transaction->socket() == conn);
		sendAuthenticatedTransaction(transaction);

		_connections.add(peerAddress, conn);
		return true;
	} 
	catch (std::exception&/Exception&/ exc) {	
		// Socket instance deleted via state callback
		ErrorL << "Connection Bind Error: " << exc.what()/message()/ << endl;
	}
	*/
	/*
	net::Address peerAddress;
	if (getPeerAddress(socket, peerAddress)) {
		_observer.onRelayConnectionClosed(*this, socket, peerAddress);
		(connections().remove(peerAddress));
		assert(socket->base().refCount() == 1);
		socket->release();
	} else
		assert(0);
		*/
	/*
	net::Address peerAddress;
	if (!closed() && getPeerAddress(socket, peerAddress))
		_observer.onRelayDataReceived(*this, packet.data(), packet.size(), peerAddress);
	else
		WarnL << "No Peer: Data Packet Dropped" << endl;	
		*/

	
	
/*
TCPClient::TCPClient(TCPClientObserver& observer, Net::Reactor& reactor, uv::Loop* loop, const Client::Options& options) : 
	Client(observer, reactor, runner, options), 
	_observer(observer)
{
	TraceL << "Create" << endl;
}
*/

/*


bool TCPClient::getPeerAddress(net::TCPBase* socket, net::Address& peerAddress)
{	
	TraceL << "getPeerAddress: " << socket << endl;	
	ConnectionManagerMap conns = connections().map();
	for (ConnectionManagerMap::const_iterator it = conns.begin(); it != conns.end(); ++it) {
		if (it->second == socket) {
			peerAddress = it->first;
			return true;
		}
	}
	return false;
}


void TCPClient::onConnectionBindSocketRecv(void* sender, net::SocketPacket& packet)
{
	TraceL << "onConnectionBindSocketRecv" << endl;	
}
*/


/*
void TCPClient::onRelayConnectionStateChange(void* sender, Net::SocketState& state, const Net::SocketState& oldState)
{	
	TraceL << "Client connection state changed to " << state << " from " << oldState << endl;	

	if (state.id() == Net::SocketState::None)
		return;

	auto socket = reinterpret_cast<net::TCPSocket*>(sender);
	//net::TCPSocket& conn = connections().get(socket->address(), false);
	//TraceL << "Client Connection State Change: " << socket << endl;	
	{
		//Mutex::ScopedLock lock(_mutex);
		assert(socket);
		if (!socket || closed())
			return;
	}	
	
	if (state.id() == Net::SocketState::Disconnected) {	
		net::Address peerAddress;
		if (getPeerAddress(socket, peerAddress)) {
			_observer.onRelayConnectionClosed(*this, socket, peerAddress);
			(connections().remove(peerAddress));
		} else
			assert(0);
	}

	_observer.onRelayConnectionState(*this, socket, state, oldState);
}
*/

//Buffer& buffer
//void TCPClient::onRelayDataReceived(void* sender, RawPacket& packet)
		// TODO: Relay buffer directly (check base client impl)
		//_observer.onRelayDataReceived(*this, (const char *)buffer.begin(), buffer.available(), peerAddress);


	//TraceL << "getPeerAddress: " << socket->address() << endl;	
	//net::TCPSocket& conn = connections().get(socket->address());

/*
net::TCPBase* TCPClient::createSocketBase()
{
	//Mutex::ScopedLock lock(_mutex);
	//return new net::TCPStatefulSocketBase(_reactor);
	return new net::TCPBase;
}
*/

//net::TCPStatefulSocketBase* getPeerConnection(net::Address& peerAddress);