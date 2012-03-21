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


#include "Sourcey/TURN/server/UDPAllocation.h"
#include "Sourcey/TURN/server/Server.h"
#include "Sourcey/Buffer.h"
#include "Sourcey/Logger.h"
#include "Poco/Net/SocketReactor.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>


using namespace std;
using namespace Poco;
using namespace Poco::Net;


namespace Sourcey {
namespace TURN {


UDPAllocation::UDPAllocation(Server& server,
							 const FiveTuple& tuple, 
							 const std::string& username, 
							 const UInt32& lifetime) : 
	ServerAllocation(server, tuple, username, lifetime),
	_relaySocket(server.reactor()) //, server.runner()
{
	// Handle data from the relay socket directly from the allocation.
	// This will remove the need for allocation lookups when receiving
	// data from peers.
	_relaySocket.bind(Net::Address("127.0.0.1", 0));
	_relaySocket.attach(packetDelegate(this, &UDPAllocation::onPacketReceived,1));

	Log("debug") << "[UDPAllocation:" << this << "]  " << this << ": Initializing on address: " << _relaySocket.address().toString() << std::endl;
}


UDPAllocation::~UDPAllocation() 
{
	Log("debug") << "[UDPAllocation:" << this << "] Destroying" << endl;	
	_relaySocket.detach(packetDelegate(this, &UDPAllocation::onPacketReceived));
	_relaySocket.close();
}


bool UDPAllocation::handleRequest(const Request& request) 
{	
	Log("debug") << "[UDPAllocation:" << this << "] Handle Request" << endl;	

	if (!ServerAllocation::handleRequest(request)) {
		if (request.type() == STUN::Message::SendIndication)
			handleSendIndication(request);
		else
			return false;
	}
	
	return true; 
}


void UDPAllocation::handleSendIndication(const Request& request) 
{	
	Log("debug") << "[UDPAllocation:" << this << "] Handle Send Indication" << endl;

	// The message is first checked for validity.  The Send indication MUST
	// contain both an XOR-PEER-ADDRESS attribute and a DATA attribute.  If
	// one of these attributes is missing or invalid, then the message is
	// discarded.  Note that the DATA attribute is allowed to contain zero
	// bytes of data.

	const STUN::XorPeerAddress* peerAttr = request.get<STUN::XorPeerAddress>();
	if (!peerAttr || peerAttr && peerAttr->family() != 1) {
		Log("error") << "[UDPAllocation:" << this << "] Send Indication Error: No Peer Address" << endl;
		// silently disgarded...
		return;
	}

	const STUN::Data* dataAttr = request.get<STUN::Data>();
	if (!dataAttr) {
		Log("error") << "[UDPAllocation:" << this << "] Send Indication Error: No Data Attribute" << endl;
		// silently disgarded...
		return;
	}

	// The Send indication may also contain the DONT-FRAGMENT attribute.  If
	// the server is unable to set the DF bit on outgoing UDP datagrams when
	// this attribute is present, then the server acts as if the DONT-
	// FRAGMENT attribute is an unknown comprehension-required attribute
	// (and thus the Send indication is discarded).

	// The server also checks that there is a permission installed for the
	// IP address contained in the XOR-PEER-ADDRESS attribute.  If no such
	// permission exists, the message is discarded.  Note that a Send
	// indication never causes the server to refresh the permission.

	// The server MAY impose restrictions on the IP address and port values
	// allowed in the XOR-PEER-ADDRESS attribute -- if a value is not
	// allowed, the server silently discards the Send indication.
	
	Net::Address peerAddress = peerAttr->address();
	if (!hasPermission(peerAddress.host())) {
		Log("error") << "[UDPAllocation:" << this << "] Send Indication Error: No permission: " << peerAddress.host().toString() << endl;
		// silently disgarded...
		return;
	}

	// If everything is OK, then the server forms a UDP datagram as follows:

	// o  the source transport address is the relayed transport address of
	//    the allocation, where the allocation is determined by the 5-tuple
	//    on which the Send indication arrived;

	// o  the destination transport address is taken from the XOR-PEER-
	//    ADDRESS attribute;

	// o  the data following the UDP header is the contents of the value
	//    field of the DATA attribute.

	// The handling of the DONT-FRAGMENT attribute (if present), is
	// described in Section 12.

	// The resulting UDP datagram is then sent to the peer.
	
	Log("debug") << "[UDPAllocation:" << this << "] Relaying Send Indication: " 
		<< "\r\tFrom: " << request.remoteAddr.toString()
		<< "\r\tTo: " << peerAttr->address().toString()
		<< endl;	

	if (send(dataAttr->bytes(), dataAttr->size(), peerAttr->address()) == -1) {
		_server.sendError(request, 486, "Allocation Quota Reached");
		delete this;
	}
}


void UDPAllocation::onPacketReceived(void* sender, DataPacket& packet) 
{	
	Net::PacketInfo* source = reinterpret_cast<Net::PacketInfo*>(packet.info);
	assert(source);
	if (!source)
		return;
	
	updateUsage(packet.size());
	
	// Check that we have not exceeded out lifetime and bandwidth quota.
	if (!isOK())
		return;

	Log("debug") << "[UDPAllocation:" << this << "] Received UDP Datagram from " << source->peerAddress.toString() << endl;	
	
	if (!hasPermission(source->peerAddress.host())) {
		Log("debug") << "[UDPAllocation:" << this << "] No Permission: " << source->peerAddress.host().toString() << endl;	
		return;
	}

	STUN::Message message;
	message.setType(STUN::Message::DataIndication);
	
	STUN::XorPeerAddress* peerAttr = new STUN::XorPeerAddress;
	peerAttr->setFamily(1);
	peerAttr->setPort(source->peerAddress.port());
	peerAttr->setIP(source->peerAddress.host().toString());
	message.add(peerAttr);

	STUN::Data* dataAttr = new STUN::Data;
	dataAttr->copyBytes(packet.data(), packet.size());
	message.add(dataAttr);
	
	FastMutex::ScopedLock lock(_mutex);

	Log("debug") << "[UDPAllocation:" << this << "] Sending Data Indication:" 
		<< "\n\tFrom: " << source->peerAddress.toString()
		<< "\n\tTo: " << _tuple.remote().toString()
		//<< "\tData: " << string(packet.data, packet.size)
		<< endl;
	
	_server.socketUDP().send(message, _tuple.remote());
}


int UDPAllocation::send(const char* data, int size, const Net::Address& peerAddress)
{
	updateUsage(size);
	
	// Check that we have not exceeded our lifetime and bandwidth quota.
	if (!isOK()) {
		return -1;
	}

	return _relaySocket.send(data, size, peerAddress);
}


Net::Address UDPAllocation::relayedAddress() const 
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _relaySocket.address();
}


} } //  namespace Sourcey::TURN