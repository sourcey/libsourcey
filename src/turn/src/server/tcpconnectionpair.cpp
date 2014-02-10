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


#include "scy/turn/server/tcpconnectionpair.h"
#include "scy/turn/server/tcpallocation.h"
#include "scy/turn/server/server.h"
#include "scy/crypto/crypto.h"


using namespace std;


namespace scy {
namespace turn {

	
TCPConnectionPair::TCPConnectionPair(TCPAllocation& allocation) :
	allocation(allocation), client(nullptr), peer(nullptr), earlyPeerData(0),
	connectionID(util::randomNumber()), isDataConnection(false)
{		
	while (!allocation.pairs().add(connectionID, this, false)) {
		connectionID = util::randomNumber();
	}
	TraceLS(this) << "Create: " << connectionID << endl;	
}


TCPConnectionPair::~TCPConnectionPair() 
{		
	TraceLS(this) << "Destroy: " << connectionID << endl;	

	if (client) {
		//assert(client->base().refCount() == 2);
		client->Recv -= sdelegate(this, &TCPConnectionPair::onClientDataReceived);
		client->Close -= sdelegate(this, &TCPConnectionPair::onConnectionClosed);
		client->close();
	}
	if (peer) {		
		//assert(peer->base().refCount() == 1);
		peer->Recv -= sdelegate(this, &TCPConnectionPair::onPeerDataReceived);
		peer->Connect -= sdelegate(this, &TCPConnectionPair::onPeerConnectSuccess);
		peer->Error -= sdelegate(this, &TCPConnectionPair::onPeerConnectError);
		peer->Close -= sdelegate(this, &TCPConnectionPair::onConnectionClosed);
		peer->close();
	}

	assert(allocation.pairs().exists(connectionID));
	allocation.pairs().remove(connectionID);
}


bool TCPConnectionPair::doPeerConnect(const net::Address& peerAddr)
{	
	try {
		assert(!transactionID.empty());
		peer = std::make_shared<net::TCPSocket>();
		peer->opaque = this;
		peer->Close += sdelegate(this, &TCPConnectionPair::onConnectionClosed);

		// Start receiving early media
		peer->Recv += sdelegate(this, &TCPConnectionPair::onPeerDataReceived);

		// Connect request specific events
		peer->Connect += sdelegate(this, &TCPConnectionPair::onPeerConnectSuccess);
		peer->Error += sdelegate(this, &TCPConnectionPair::onPeerConnectError);
	
		client->connect(peerAddr);
	} 
	catch (std::exception& exc) {
		ErrorLS(this) << "Peer connect error: " << exc.what() << endl;
		assert(0);
		return false;
	}
	return true;
}


void TCPConnectionPair::setPeerSocket(const net::TCPSocket::Ptr& socket)
{	
	TraceLS(this) << "Set peer socket: " 
		<< connectionID << ": " << socket->peerAddress() << endl;	
		//<< ": " << socket./*base().*/refCount() 

	assert(peer == nullptr);
	//assert(socket./*base().*/refCount() == 1);
	peer = socket;
	peer->Close += sdelegate(this, &TCPConnectionPair::onConnectionClosed);
	
	// Receive and buffer early media from peer
	peer->Recv += sdelegate(this, &TCPConnectionPair::onPeerDataReceived);	
	setServerSocketBufSize<uv_tcp_t>(*socket.get(), SERVER_SOCK_BUF_SIZE); // TODO: make option
}


void TCPConnectionPair::setClientSocket(const net::TCPSocket::Ptr& socket)
{
	TraceLS(this) << "Set client socket: "
		<< connectionID << ": " << socket->peerAddress()  << endl;	
		//<< ": " << socket./*base().*/refCount()
	assert(client == nullptr);
	//assert(socket./*base().*/refCount() == 2);
	client = socket;
	client->Close += sdelegate(this, &TCPConnectionPair::onConnectionClosed);
	setServerSocketBufSize<uv_tcp_t>(*socket.get(), SERVER_SOCK_BUF_SIZE); // TODO: make option
}


bool TCPConnectionPair::makeDataConnection()
{
	TraceLS(this) << "Make data connection: " << connectionID << endl;	
	if (!peer || !client)
		return false;

	peer->Recv += sdelegate(this, &TCPConnectionPair::onPeerDataReceived);
	client->Recv += sdelegate(this, &TCPConnectionPair::onClientDataReceived);	
	
	// Relase and unbind the client socket from the server.
	// The client socket instance, events and data will be
	// managed by the TCPConnectionPair from now on.
	allocation.server().releaseTCPSocket(client.get());
			
	// Send early data from peer to client
	if (earlyPeerData.size()) {
		TraceLS(this) << "Flushing early media: " << earlyPeerData.size() << endl;	
		client->send(earlyPeerData.data(), earlyPeerData.size());
		earlyPeerData.clear();
	}

	return (isDataConnection = true);
}


void TCPConnectionPair::onPeerDataReceived(void*, const MutableBuffer& buffer, const net::Address& peerAddress)
{
	TraceLS(this) << "Peer => Client: " << buffer.size() << endl;	
	//assert(pkt.buffer.position() == 0);
	//if (pkt.buffer.available() < 300)
	//	TraceLS(this) << "Peer => Client: " << pkt.buffer << endl;	
	//auto socket = reinterpret_cast<net::Socket*>(sender);		
	//char* buf = bufferCast<char*>(buf);
	
	//Buffer& buf = pkt.buffer;
	const char* buf = bufferCast<const char*>(buffer);
	std::size_t len = buffer.size();
	if (client) {	
		
		allocation.updateUsage(len);
		if (allocation.deleted())
			return;

		//assert(buf.position() == 0);
		client->send(buf, len);
	}

	// Flash policy requests
	// TODO: Handle elsewhere? Bloody flash...
	else if (len == 23 && (strcmp(buf, "<policy-file-request/>") == 0)) {
		TraceLS(this) << "Handle flash policy" << endl;
		std::string policy("<?xml version=\"1.0\"?><cross-domain-policy><allow-access-from domain=\"*\" to-ports=\"*\" /></cross-domain-policy>");
		//assert(peer->get() == pkt.info->socket);
		peer->send(policy.c_str(), policy.length() + 1);
		peer->close();
	}
	
	// Buffer early media
	// TODO: Make buffer size server option
	else {
		size_t maxSize = allocation.server().options().earlyMediaBufferSize;
		DebugLS(this) << "Buffering early data: " << len << endl;
#ifdef _DEBUG
		DebugLS(this) << "Printing early data: " << std::string(buf, len) << endl;
#endif
		if (len > maxSize)
			WarnL << "Dropping early media: Oversize packet: " << len << endl;
		if (earlyPeerData.size() > maxSize)
			WarnL << "Dropping early media: Buffer at capacity >= " << maxSize << endl;

		//earlyPeerData.append(static_cast<const char*>(pkt.data()), len);
		earlyPeerData.insert(earlyPeerData.end(), buf, buf + len); 
	}
}


void TCPConnectionPair::onClientDataReceived(void*, const MutableBuffer& buffer, const net::Address& peerAddress)
{
	TraceLS(this) << "Client => Peer: " << buffer.size() << endl;	
	//assert(packet.buffer.position() == 0);
	//if (packet.size() < 300)
	//	TraceLS(this) << "Client => Peer: " << packet.buffer << endl;	

	if (peer) {
		allocation.updateUsage(buffer.size());
		if (allocation.deleted())
			return;

		peer->send(bufferCast<char*>(buffer), buffer.size());
	}
}


void TCPConnectionPair::onPeerConnectSuccess(void* sender)
{
	TraceLS(this) << "Peer Connect request success" << endl;	
	assert(sender == &peer);
	peer->Connect -= sdelegate(this, &TCPConnectionPair::onPeerConnectSuccess);
	peer->Error -= sdelegate(this, &TCPConnectionPair::onPeerConnectError);
		
	// If no ConnectionBind request associated with this peer data
	// connection is received after 30 seconds, the peer data connection
	// MUST be closed.

	allocation.sendPeerConnectResponse(this, true);

	// TODO: Ensure this is implemented properly
	startTimeout();
}


void TCPConnectionPair::onPeerConnectError(void* sender, const Error& error)
{
	TraceLS(this) << "Peer Connect request error: " << error.message << endl;	
	assert(sender == &peer);
	allocation.sendPeerConnectResponse(this, false);

	// The TCPConnectionPair will be deleted on next call to onConnectionClosed
}
	

void TCPConnectionPair::onConnectionClosed(void* sender)
{
	TraceLS(this) << "Connection pair socket closed: " << connectionID << ": " << sender << endl;
	delete this; // fail
}


void TCPConnectionPair::startTimeout()
{
	//Mutex::ScopedLock lock(_mutex);
	timeout.reset();
}


bool TCPConnectionPair::expired() const
{
	//Mutex::ScopedLock lock(_mutex);
	return timeout.running() 
		&& timeout.expired();
}

} } // namespace scy::turn



	//TCPConnectionPair* pair = reinterpret_cast<TCPConnectionPair*>(sender);
	//pairs().remove(connectionID);
	//delete pair;





	/*
TCPConnectionPair::TCPConnectionPair(TCPAllocation& allocation, const net::TCPSocket::Ptr& socket, TCPPeerConnection* peer)
	_allocation(allocation),
	_connectionID(peer->connectionID()),
{
}
	

//TCPClientConnection::TCPClientConnection(TCPAllocation& allocation, const Poco::Net::StreamSocket& socket, TCPPeerConnection* peer, Net::Reactor& reactor) : 
TCPClientConnection::TCPClientConnection(TCPAllocation& allocation, const net::TCPSocket::Ptr& socket, TCPPeerConnection* peer) : 
	net::TCPSocket(socket), //, true, reactor
	_allocation(allocation),
	_connectionID(peer->connectionID()),
	_peer(nullptr)
{
	TraceLS(this) << "Create" << endl;
	bindWith(peer);
	_peer->bindWith(this);
	_allocation.clients().add(_connectionID, this);

	//setDeleteOnClose(true);
}

	
TCPClientConnection::~TCPClientConnection()
{
	TraceLS(this) << "Destroy" << endl;
	//assert(!isConnected());
	if (_peer)
		_peer->bindWith(this);
	_allocation.clients().remove(this);
	TraceLS(this) << "Destroy: OK" << endl;
}


void TCPClientConnection::recv(Buffer& buffer)
{
	TraceLS(this) << "Received Data: " << buffer.available() << endl;

	try {	
		//Mutex::ScopedLock lock(_mutex);

		if (_peer) {		
			_allocation.updateUsage(buffer.available());
			if (!_allocation.!IAllocation::deleted())
				throw std::runtime_error("Allocation expired");

			_peer->send(buffer.begin(), buffer.available());
		}
		else		
			// TODO: buffering early media
			TraceLS(this) << "Dropped early data" << endl;
	}
	catch (std::exception&/Exception&/ exc) {
		ErrorLS(this) << "RECV Error: " << exc.what()/message()/ << endl;
		//setError(exc.what()/message()/);
		net::TCPSocket::close();
	}
}


void TCPClientConnection::onPeerDisconnect(void*)
{
	TraceLS(this) << "On Peer Disconnected" << endl;
	{		
		//Mutex::ScopedLock lock(_mutex);	
		_peer = nullptr;
	}

	// will result in destruction
	//net::TCPSocket::close();
	//assert(closing);
	
	assert(net::TCPbase().refCount() == 1);
	//net::TCPSocket::destroy();
	//net::TCPSocket::close();
	assert(0 && "close socket");
}


void TCPClientConnection::bindWith(TCPPeerConnection* peer)
{
	{
		//Mutex::ScopedLock lock(_mutex);
		assert(!peer || _connectionID == peer->connectionID());
		if (_peer)
			_peer->base().Closed -= sdelegate(this, &TCPClientConnection::onPeerDisconnect);	
		_peer = peer;
		if (_peer != nullptr) {
			TraceLS(this) << "Binding With Peer: " << _peer->address() << endl;
			_peer->base().Closed += sdelegate(this, &TCPClientConnection::onPeerDisconnect);	
		}
	}
	if (!peer) {
		TraceLS(this) << "Closing Because Peer Went Offline" << endl;
		//net::TCPSocket::close();
		assert(0 && "close socket");
	}
}


TCPPeerConnection* TCPClientConnection::peer() const
{
	//Mutex::ScopedLock lock(_mutex);
	return _peer;
}


UInt32 TCPClientConnection::connectionID() const
{
	return _connectionID;
}
*/



	/*	
void TCPClientConnection::onClose()
{
	TraceLS(this) << "On close" << endl;
	net::TCPSocket::onClose();	
	
	// Close the associated client connection.
	//Mutex::ScopedLock lock(_mutex);	
	TCPPeerConnection* peer = _peer;
	if (peer) {
		peer->Close -= sdelegate(this, &TCPClientConnection::onPeerDisconnect);
		TraceLS(this) << "On close 1" << endl;	
		_peer = nullptr;
		peer->bindWith(nullptr);
		//peer->close();
	}

	// Close the associated peer connection.
	if (_peer) {
		_peer->Close -= sdelegate(this, &TCPClientConnection::onPeerDisconnect);	
		_peer->close();
		_peer = nullptr;
	}
}
	*/