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


#include "Sourcey/TURN/server/TCPClientConnection.h"
#include "Sourcey/TURN/server/TCPAllocation.h"
#include "Sourcey/CryptoProvider.h"


using namespace std;
using namespace Poco;
using namespace Poco::Net;


namespace Sourcey {
namespace TURN {


TCPClientConnection::TCPClientConnection(TCPAllocation& allocation, const Poco::Net::StreamSocket& socket, TCPPeerConnection* peer, Net::Reactor& reactor) : 
	Net::TCPPacketSocket(socket, reactor), //, true
	_allocation(allocation),
	_connectionID(peer->connectionID()),
	_peer(NULL)
{
	Log("trace") << "[TCPClientConnection:" << this << "] Creating" << endl;
	bindWith(peer);
	_peer->bindWith(this);
	_allocation.clients().add(_connectionID, this);

	setDeleteOnClose(true);
}

	
TCPClientConnection::~TCPClientConnection()
{
	Log("trace") << "[TCPClientConnection:" << this << "] Destroying" << endl;
	assert(!isConnected());
	//assert(_peer == NULL);
	_allocation.clients().remove(this);
	Log("trace") << "[TCPClientConnection:" << this << "] Destroying: OK" << endl;
}


void TCPClientConnection::recv(Buffer& buffer)
{
	Log("trace") << "[TCPClientConnection:" << this << "] Received Data: " << buffer.size() << endl;

	try {	
		FastMutex::ScopedLock lock(_mutex);

		if (_peer) {		
			_allocation.updateUsage(buffer.size());
			if (!_allocation.isOK())
				throw Exception("Allocation expired");

			_peer->send(buffer.bytes(), buffer.size());
		}
		else		
			// TODO: buffering early media
			Log("trace") << "[TCPClientConnection:" << this << "] Dropped early data" << endl;
	}
	catch (Exception& exc) {
		Log("error") << "[TCPClientConnection:" << this << "] RECV Error: " << exc.displayText() << endl;
		setError(exc.displayText());
	}
}


	/*	
void TCPClientConnection::onClose()
{
	Log("trace") << "[TCPClientConnection:" << this << "] On Close" << endl;
	Net::TCPPacketSocket::onClose();	
	
	// Close the associated client connection.
	FastMutex::ScopedLock lock(_mutex);	
	TCPPeerConnection* peer = _peer;
	if (peer) {
		peer->Closed -= delegate(this, &TCPClientConnection::onPeerDisconnect);
		Log("trace") << "[TCPClientConnection:" << this << "] On Close 1" << endl;	
		_peer = NULL;
		peer->bindWith(NULL);
		//peer->close();
	}

	// Close the associated peer connection.
	if (_peer) {
		_peer->Closed -= delegate(this, &TCPClientConnection::onPeerDisconnect);	
		_peer->close();
		_peer = NULL;
	}
}
	*/


void TCPClientConnection::onPeerDisconnect(void*)
{
	Log("trace") << "[TCPClientConnection:" << this << "] On Peer Disconnected" << endl;
	{		
		FastMutex::ScopedLock lock(_mutex);	
		_peer = NULL;
	}
	close();
}


void TCPClientConnection::bindWith(TCPPeerConnection* peer)
{
	{
		FastMutex::ScopedLock lock(_mutex);
		assert(!peer || _connectionID == peer->connectionID());
		if (_peer)
			_peer->Closed -= delegate(this, &TCPClientConnection::onPeerDisconnect);	
		_peer = peer;
		if (_peer != NULL) {
			Log("trace") << "[TCPClientConnection:" << this << "] Binding With Peer: " << _peer->address() << endl;
			_peer->Closed += delegate(this, &TCPClientConnection::onPeerDisconnect);	
		}
	}
	if (!peer)
		close();
}


TCPPeerConnection* TCPClientConnection::peer() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _peer;
}


UInt32 TCPClientConnection::connectionID() const
{
	return _connectionID;
}


} } // namespace Sourcey::TURN