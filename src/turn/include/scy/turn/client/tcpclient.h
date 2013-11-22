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


#ifndef SCY_TURN_TCPClient_H
#define SCY_TURN_TCPClient_H


#include "scy/turn/client/client.h"
#include "scy/net/tcpsocket.h"
#include "scy/collection.h"

#include <string>
#include <deque>


namespace scy {
namespace turn {


class TCPClient;


struct TCPClientObserver: public ClientObserver 
{
	virtual void onRelayConnectionCreated(TCPClient& client, const net::TCPSocket& socket, const net::Address& peerAddress) {};
	virtual void onRelayConnectionError(TCPClient& client, const net::TCPSocket& socket, const net::Address& peerAddress) {};
	virtual void onRelayConnectionClosed(TCPClient& client, const net::TCPSocket& socket, const net::Address& peerAddress) {};
	virtual void onRelayConnectionBindingFailed(TCPClient& client, const net::Address& peerAddress) {};
	virtual bool onPeerConnectionAttempt(TCPClient& client, const net::Address& peerAddress) { return true; };
};


struct RelayConnectionBinding
{
	UInt32 connectionID;
	net::Address peerAddress;
};


typedef PointerCollection<net::Address, net::TCPSocket>	ConnectionManager;
typedef ConnectionManager::Map						    ConnectionManagerMap;


class TCPClient: public Client
{
public:
	TCPClient(TCPClientObserver& observer, const Client::Options& options = Client::Options());
	virtual ~TCPClient();

	virtual void initiate();
	virtual void shutdown();
	
	virtual void sendConnectRequest(const net::Address& peerAddress);
	virtual void sendData(const char* data, int size, const net::Address& peerAddress);	
	
	virtual bool handleResponse(const stun::Message& response);
	virtual void handleConnectResponse(const stun::Message& response);
	virtual void handleConnectErrorResponse(const stun::Message& response);
	virtual void handleConnectionBindResponse(const stun::Message& response);
	virtual void handleConnectionBindErrorResponse(const stun::Message& response);
	virtual void handleConnectionAttemptIndication(const stun::Message& response);	

	virtual bool createAndBindConnection(UInt32 connectionID, const net::Address& peerAddress);
	//virtual void onRelayConnectionStateChange(void* sender, Net::SocketState& state, const Net::SocketState& oldState);	

	virtual void onRelayConnectionConnect(void* sender);
	virtual void onRelayDataReceived(void* sender, net::SocketPacket& packet);
	virtual void onRelayConnectionError(void* sender, const Error& error) ;	
	virtual void onRelayConnectionClosed(void* sender);	
		

	void freeConnection(net::TCPSocket* socket);	

	virtual int transportProtocol();
	ConnectionManager& connections();

	virtual const char* className() const { return "TURNTCPClient"; };

protected:	
	TCPClientObserver& _observer;
	ConnectionManager _connections;
};


} } //  namespace scy::turn


#endif // SCY_TURN_TCPClient_H