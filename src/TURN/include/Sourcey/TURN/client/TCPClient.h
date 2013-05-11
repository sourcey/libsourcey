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


#ifndef SOURCEY_TURN_TCPClient_H
#define SOURCEY_TURN_TCPClient_H


#include "Sourcey/TURN/client/Client.h"
#include "Sourcey/BasicManager.h"
#include "Sourcey/Net/TCPSocket.h"
#include "Sourcey/Net/StatefulSocket.h"

#include <string>
#include <deque>


namespace Scy {
namespace TURN {
	

typedef BasicManager<Net::Address, Net::TCPStatefulPacketSocket>	ConnectionManager;
typedef ConnectionManager::Map								ConnectionManagerMap;


class TCPClient;


struct TCPClientObserver: public IClientObserver 
{
	virtual void onClientConnectionCreated(TCPClient& client, Net::IPacketSocket* socket, const Net::Address& peerAddress) {};
	virtual void onClientConnectionClosed(TCPClient& client, Net::IPacketSocket* socket, const Net::Address& peerAddress) {};
	virtual void onClientConnectionState(TCPClient& client, Net::IPacketSocket* socket,
		Net::SocketState& state, const Net::SocketState& oldState) {};

	virtual void onClientConnectionBindingFailed(TCPClient& client, const Net::Address& peerAddress) {};

	virtual bool onPeerConnectionAttempt(TCPClient& client, const Net::Address& peerAddress) { return true; };
};


class TCPClient: public Client
{
public:
	TCPClient(TCPClientObserver& observer,
			  Net::Reactor& reactor,
			  Runner& runner,
			  const Client::Options& options = Client::Options());
	virtual ~TCPClient();

	virtual void initiate();
	virtual void terminate();
	
	virtual void sendConnectRequest(const Net::Address& peerAddress);
	virtual void sendData(const char* data, int size, const Net::Address& peerAddress);	
	
	virtual bool handleResponse(const STUN::Message& response);
	virtual void handleConnectResponse(const STUN::Message& response);
	virtual void handleConnectErrorResponse(const STUN::Message& response);
	virtual void handleConnectionBindResponse(const STUN::Message& response);
	virtual void handleConnectionBindErrorResponse(const STUN::Message& response);
	virtual void handleConnectionAttemptIndication(const STUN::Message& response);	

	virtual bool createAndBindConnection(UInt32 connectionID, const Net::Address& peerAddress);
	virtual void onClientConnectionStateChange(void* sender, Net::SocketState& state, const Net::SocketState& oldState);	
	virtual void onDataPacketReceived(void* sender, DataPacket& packet);

	virtual Net::IPacketSocket* createSocket();
	virtual int transportProtocol();
	ConnectionManager& connections();
		
	bool getPeerAddress(Net::IPacketSocket* conn, Net::Address& peerAddress);
	//Net::TCPStatefulPacketSocket* getConnection(const Net::Address& peerAddress);
		/// Obtain the TCPStatefulPacketSocket instance from the IPacketSocket

protected:	
	TCPClientObserver& _observer;
	ConnectionManager _connections;
};


} } //  namespace Scy::TURN


#endif // SOURCEY_TURN_TCPClient_H