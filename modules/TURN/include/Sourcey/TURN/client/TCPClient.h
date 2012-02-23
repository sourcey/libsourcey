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


#ifndef SOURCEY_TURN_TCPClient_H
#define SOURCEY_TURN_TCPClient_H


#include "Sourcey/TURN/client/Client.h"
#include "Sourcey/BasicManager.h"
#include "Sourcey/Net/TCPClientSocket.h"

#include <string>
#include <deque>


namespace Sourcey {
namespace TURN {
	

typedef BasicManager<Net::Address, Net::TCPClientSocket>	ConnectionManager;
typedef ConnectionManager::Map								ConnectionManagerMap;


class TCPClient;


struct ITCPClientObserver: public IClientObserver 
{
	virtual void onClientConnectionCreated(TCPClient& client, Net::TCPClientSocket* socket, const Net::Address& peerAddress) {};
	virtual void onClientConnectionClosed(TCPClient& client, Net::TCPClientSocket* socket, const Net::Address& peerAddress) {};
	virtual void onClientConnectionState(TCPClient& client, Net::TCPClientSocket* socket,
		Net::ClientState& state, const Net::ClientState& oldState) {};

	virtual void onClientConnectionBindingFailed(TCPClient& client, const Net::Address& peerAddress) {};

	virtual bool onPeerConnectionAttempt(TCPClient& client, const Net::Address& peerAddress) { return true; };
};


class TCPClient: public Client
{
public:
	TCPClient(ITCPClientObserver& observer,
			  const Client::Options& options = Client::Options(), 
			  Net::Reactor& reactor = Net::Reactor::getDefault()//, 
			  // Runner& runner = Runner::getDefault()
			  );
	virtual ~TCPClient();

	//virtual void onTimer(TimerCallback<Client>& timer);

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
	virtual void onClientConnectionStateChange(void* sender, Net::ClientState& state, const Net::ClientState& oldState);	
	virtual void onDataPacketReceived(void* sender, DataPacket& packet);

	virtual Net::ISocket* createSocket();
	virtual int transportProtocol();
	
	bool getPeerAddress(Net::TCPClientSocket* conn, Net::Address& peerAddress);
	ConnectionManager& connections();
	
protected:	
	ITCPClientObserver& _observer;
	ConnectionManager _connections;
};


} } //  namespace Sourcey::TURN


#endif // SOURCEY_TURN_TCPClient_H