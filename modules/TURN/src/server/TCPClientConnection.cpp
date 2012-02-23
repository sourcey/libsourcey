//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2002 Sourcey
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
	Net::TCPSocket(socket, reactor, true),
	_allocation(allocation),
	_connectionID(peer->connectionID()),
	_peer(peer)
{
	Log("trace") << "[TCPClientConnection:" << this << "] Creating" << endl;
	_peer->bindWith(this);
	_allocation.clients().add(_connectionID, this);
}

	
TCPClientConnection::~TCPClientConnection()
{
	Log("trace") << "[TCPClientConnection:" << this << "] Destroying" << endl;
	assert(!isConnected());
	_allocation.clients().remove(this);
}


void TCPClientConnection::recv(Buffer& buffer)
{
	//Log("trace") << "[TCPClientConnection:" << this << "] Received Data: " << buffer.size() << endl;

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
		Log("debug") << "[TCPClientConnection:" << this << "] Send Error: " << exc.displayText() << endl;
		setError(exc.displayText());
	}
}


void TCPClientConnection::bindWith(TCPPeerConnection* peer)
{
	{
		FastMutex::ScopedLock lock(_mutex);
		assert(!peer || _connectionID == peer->connectionID());
		_peer = peer;
		if (_peer != NULL)
			Log("trace") << "[TCPPeerConnection:" << this << "] Binding With Peer: " << peer->address() << endl;
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