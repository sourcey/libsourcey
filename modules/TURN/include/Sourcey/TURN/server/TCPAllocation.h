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


#ifndef SOURCEY_TURN_SERVER_TCPAllocation_H
#define SOURCEY_TURN_SERVER_TCPAllocation_H


#include "Sourcey/TURN/server/ServerAllocation.h"
#include "Sourcey/TURN/server/TCPClientConnection.h"
#include "Sourcey/TURN/server/TCPPeerConnection.h"
#include "Sourcey/Net/TCPSocket.h"
#include "Sourcey/Net/TCPSocketAcceptor.h"


namespace Sourcey {
namespace TURN {

	
class Server;
class TCPControlConnection;


class TCPAllocation: public ServerAllocation
{
public:
	TCPAllocation(Server& server, 
				  Net::TCPSocket& controlConn, 
				  const FiveTuple& tuple, 
				  const std::string& username, 
				  const UInt32& lifetime);
	virtual ~TCPAllocation();
	
	bool handleRequest(const Request& request);	
	void handleConnectRequest(const Request& request);
	void handleConnectionBindRequest(const Request& request);
	
	void onPeerAccepted(void* sender, Poco::Net::StreamSocket& sock, Net::Reactor& reactor);
	void onPeerConnectSuccess(void* sender); 
	void onPeerConnectError(void* sender, const std::string& error, int& errnum);
	void onPeerDisconnected(void* sender);
	void onClientDisconnect(void* sender);
	
	bool isOK();
	bool onTimer();
	
	Net::Address relayedAddress() const;
	Net::TCPSocket& control();
	ClientConnectionManager& clients();
	PeerConnectionManager& peers();

protected:
	Net::TCPSocket&	_control;
	ClientConnectionManager	_clients;
	PeerConnectionManager	_peers;
	Net::TCPSocketAcceptor	_acceptor;
	mutable Poco::FastMutex	_mutex;
};


} } //  namespace Sourcey::TURN


#endif // SOURCEY_TURN_SERVER_TCPAllocation_H