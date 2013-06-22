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


#include "Sourcey/TURN/server/TCPPeerConnection.h"
#include "Sourcey/TURN/server/TCPAllocation.h"
#include "Sourcey/CryptoProvider.h"


using namespace std;
using namespace Poco;
using namespace Poco::Net;


namespace Scy {
namespace TURN {
	

TCPPeerConnection::TCPPeerConnection(TCPAllocation& allocation, Net::Reactor& reactor) : 
	Net::TCPPacketSocket(reactor),
	_allocation(allocation),
	_timeout(PEER_CONNECTION_TIMEOUT * 1000),
	_connectionID(CryptoProvider::generateRandomNumber(8) & 0xFFFFFFFF),
	_earlyDataBuffer((size_t)0),
	_client(NULL)
{
	while (_allocation.peers().exists(_connectionID))
		_connectionID = CryptoProvider::generateRandomNumber(8) & 0xFFFFFFFF;
	_allocation.peers().add(_connectionID, this);

	setDeleteOnClose(true);
}


TCPPeerConnection::TCPPeerConnection(TCPAllocation& allocation, const Poco::Net::StreamSocket& socket, Net::Reactor& reactor) :
	Net::TCPPacketSocket(socket, reactor), //, true
	_allocation(allocation),
	_timeout(PEER_CONNECTION_TIMEOUT * 1000),
	_connectionID(CryptoProvider::generateRandomNumber(8) & 0xFFFFFFFF),
	_earlyDataBuffer((size_t)0),
	_client(NULL)
{
	LogTrace("TCPPeerConnection", this) << "Creating" << endl;
	while (_allocation.peers().exists(_connectionID))
		_connectionID = CryptoProvider::generateRandomNumber(8) & 0xFFFFFFFF;
	_allocation.peers().add(_connectionID, this);

	setDeleteOnClose(true);
}

	
TCPPeerConnection::~TCPPeerConnection()
{
	LogTrace("TCPPeerConnection", this) << "Destroying" << endl;
	assert(!isConnected());
	if (_client)
		_client->bindWith(this);
	_allocation.peers().remove(this);
	LogTrace("TCPPeerConnection", this) << "Destroying: OK" << endl;
}


void TCPPeerConnection::recv(Buffer& buffer)
{
	LogTrace("TCPPeerConnection", this) << "Received Data: " << buffer.size() << endl;

	try {	
		FastMutex::ScopedLock lock(_mutex);

		if (_client) {		
			_allocation.updateUsage(buffer.size());
			if (!_allocation.isOK())
				throw Exception("Allocation expired");

			_client->send(buffer.data(), buffer.size());
		}

		// TODO: Handle Flash Policy elsewhere? Bloody flash...
		else if (buffer.size() == 23 && (strcmp(buffer.bytes(), "<policy-file-request/>") == 0)) {
			LogTrace("TCPPeerConnection", this) << "Handle Flash Policy" << endl;
			string policy("<?xml version=\"1.0\"?><cross-domain-policy><allow-access-from domain=\"*\" to-ports=\"*\" /></cross-domain-policy>");
			sendBytes(policy.data(), policy.length() + 1);
			//Net::TCPPacketSocket::close();
		}
		else {
			LogDebug("TCPPeerConnection", this) << "Buffering early data: " << buffer.size() << endl;
			if (buffer.size() > 4096)
				throw Exception("Dropping early media: Oversize packet");
			if (_earlyDataBuffer.size() > 4096)
				throw Exception("Dropping early media: Buffer at capacity");

			_earlyDataBuffer.writeBytes(buffer.data(), buffer.size());
		}
	}
	catch (Exception& exc) {
		LogError("TCPPeerConnection", this) << "RECV Error: " << exc.displayText() << endl;
		setError(exc.displayText());
	}
}


void TCPPeerConnection::onClientDisconnect(void*)
{
	LogTrace("TCPPeerConnection", this) << "On Our Client Disconnected" << endl;
	{		
		FastMutex::ScopedLock lock(_mutex);	
		_client = NULL;
	}
	
	// will result in destruction
	Net::TCPPacketSocket::close();
}


void TCPPeerConnection::bindWith(TCPClientConnection* client)
{
	{
		FastMutex::ScopedLock lock(_mutex);
		assert(!client || _connectionID == client->connectionID());
		if (_client)
			_client->Closed -= delegate(this, &TCPPeerConnection::onClientDisconnect);	
		_client = client;
		if (_client != NULL) {	
			LogTrace("TCPPeerConnection", this) << "Binding With Client: " << client->address() << endl;
			_client->Closed += delegate(this, &TCPPeerConnection::onClientDisconnect);
			_timeout.stop();
		}
		
		// Send buffered early media from the 
		// peer when a client is associated.
		try {
			LogTrace("TCPPeerConnection", this) << "Sending Early Media: " << _earlyDataBuffer.size() << endl;
			if (_earlyDataBuffer.size()) {
				_client->send(_earlyDataBuffer.data(), _earlyDataBuffer.size());
				_earlyDataBuffer.clear();
			}
		}
		catch (Exception& exc) {
			LogError("TCPPeerConnection", this) << "Send Early Data Error: " << exc.displayText() << endl;
			setError(exc.displayText());
		}
	}
	if (!client) {
		LogTrace("TCPPeerConnection", this) << "Closing Because Client Went Offline" << endl;
		Net::TCPPacketSocket::close();
	}
}


TCPClientConnection* TCPPeerConnection::client() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _client;
}


void TCPPeerConnection::setTransactionID(const STUN::TransactionID& id)
{
	FastMutex::ScopedLock lock(_mutex);
	_transactionID = id;
}


STUN::TransactionID	TCPPeerConnection::transactionID() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _transactionID;
}


UInt32 TCPPeerConnection::connectionID() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _connectionID;
}


void TCPPeerConnection::startTimeout()
{
	FastMutex::ScopedLock lock(_mutex);
	_timeout.reset();
}


bool TCPPeerConnection::expired() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _timeout.running() 
		&& _timeout.expired();
}


} } // namespace Scy::TURN




/*	
void TCPPeerConnection::onClose()
{
	LogTrace("TCPPeerConnection", this) << "On Close" << endl;	
		
	Net::TCPPacketSocket::onClose();
	LogTrace("TCPPeerConnection", this) << "On Close: OK" << endl;	
	
	// Close the associated client connection.
	FastMutex::ScopedLock lock(_mutex);	
	TCPClientConnection* client = _client;
	if (client) {
		client->Closed -= delegate(this, &TCPPeerConnection::onClientDisconnect);
		LogTrace("TCPPeerConnection", this) << "On Close 1" << endl;	
		_client = NULL;
		client->bindWith(NULL);
		//client->close();
	}
	TCPClientConnection* client = peer->client();
	if (client) {
		client->Closed -= delegate(this, &TCPAllocation::onClientDisconnect);
		_client = NULL;
		client->bindWith(NULL);
		//delete client;
	}
	LogTrace("TCPPeerConnection", this) << "On Close 2" << endl;
	*/
	
	/*
	TCPClientConnection* client = peer->client();
	if (client) {
		client->Closed -= delegate(this, &TCPAllocation::onClientDisconnect);
		client->close();
		//delete client;
	}
}
	*/