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


#include "Sourcey/TURN/server/TCPPeerConnection.h"
#include "Sourcey/TURN/server/TCPAllocation.h"
#include "Sourcey/CryptoProvider.h"


using namespace std;
using namespace Poco;
using namespace Poco::Net;


namespace Sourcey {
namespace TURN {
	


TCPPeerConnection::TCPPeerConnection(TCPAllocation& allocation, Net::Reactor& reactor) : 
	Net::TCPPacketSocket(reactor), //, true
	_allocation(allocation),
	_timeout(PEER_CONNECTION_TIMEOUT * 1000),
	_connectionID(CryptoProvider::generateRandomNumber(8)),
	_client(NULL)
{
	while (_allocation.peers().exists(_connectionID))
		_connectionID = CryptoProvider::generateRandomNumber(8);
	_allocation.peers().add(_connectionID, this);

	setDeleteOnClose(true);
}


TCPPeerConnection::TCPPeerConnection(TCPAllocation& allocation, const Poco::Net::StreamSocket& socket, Net::Reactor& reactor) :
	Net::TCPPacketSocket(socket, reactor), //, true
	_allocation(allocation),
	_timeout(PEER_CONNECTION_TIMEOUT * 1000),
	_connectionID(CryptoProvider::generateRandomNumber(8)),
	_client(NULL)
{
	Log("trace") << "[TCPPeerConnection:" << this << "] Creating" << endl;
	while (_allocation.peers().exists(_connectionID))
		_connectionID = CryptoProvider::generateRandomNumber(8);
	_allocation.peers().add(_connectionID, this);

	setDeleteOnClose(true);
}

	
TCPPeerConnection::~TCPPeerConnection()
{
	Log("trace") << "[TCPPeerConnection:" << this << "] Destroying" << endl;
	assert(!isConnected());
	_allocation.peers().remove(this);
}


void TCPPeerConnection::recv(Buffer& buffer)
{
	try {	
		FastMutex::ScopedLock lock(_mutex);

		if (_client) {		
			_allocation.updateUsage(buffer.size());
			if (!_allocation.isOK())
				throw Exception("Allocation expired");

			_client->send(buffer.bytes(), buffer.size());
		}
		else		
			// TODO: buffering early media
			Log("warn") << "[TCPPeerConnection:" << this << "] Dropped early data" << endl;
	}
	catch (Exception& exc) {
		setError(exc.displayText());
	}
}


void TCPPeerConnection::bindWith(TCPClientConnection* client)
{
	{
		FastMutex::ScopedLock lock(_mutex);
		assert(!client || _connectionID == client->connectionID());
		_client = client;
		if (_client != NULL) {	
			Log("trace") << "[TCPPeerConnection:" << this << "] Binding With Client: " << client->address() << endl;
			_timeout.stop();
		}
	}
	if (!client)
		close();
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


} } // namespace Sourcey::TURN