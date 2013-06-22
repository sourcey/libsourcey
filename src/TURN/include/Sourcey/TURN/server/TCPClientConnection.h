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


#ifndef SOURCEY_TURN_TCPClientConnection_H
#define SOURCEY_TURN_TCPClientConnection_H


#include "Sourcey/BasicManager.h"
#include "Sourcey/Timeout.h"
#include "Sourcey/STUN/Message.h"
#include "Sourcey/Net/TCPSocket.h"
#include "Sourcey/Net/Reactor.h"


namespace Scy {
namespace TURN {

	
class TCPAllocation;
class TCPPeerConnection;

	
class TCPClientConnection: public Net::TCPPacketSocket
{
public:
	TCPClientConnection(TCPAllocation& allocation, const Poco::Net::StreamSocket& socket, TCPPeerConnection* peer, Net::Reactor& reactor/* = Net::Reactor::getDefault()*/);
	virtual ~TCPClientConnection();
	
	void bindWith(TCPPeerConnection* peer);
	TCPPeerConnection* peer() const;

	UInt32 connectionID() const;

	virtual const char* className() const { return "TCPClientConnection"; }
	
protected:
	void recv(Buffer& buffer);
	void onPeerDisconnect(void*);

	TCPAllocation&			_allocation;
	UInt32					_connectionID;
	TCPPeerConnection*		_peer;	
	mutable Poco::FastMutex _mutex;
};


typedef BasicManager<UInt32, TCPClientConnection>	ClientConnectionManager;
typedef ClientConnectionManager::Map				ClientConnectionMap;


} } // namespace Scy::TURN


#endif // SOURCEY_TURN_TCPClientConnection_H
