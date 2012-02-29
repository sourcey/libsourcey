//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2005 Sourcey
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


#ifndef SOURCEY_TURN_Server_H
#define SOURCEY_TURN_Server_H


#include "Sourcey/STUN/Message.h"
#include "Sourcey/TURN/server/IServerObserver.h"
#include "Sourcey/TURN/server/ServerAllocation.h"
#include "Sourcey/TURN/server/UDPAllocation.h"
#include "Sourcey/TURN/server/TCPAllocation.h"
#include "Sourcey/TURN/Util.h"

#include "Sourcey/Net/TCPServer.h"
#include "Sourcey/Net/UDPSocket.h"

#include <assert.h>
#include <string>
#include <iostream>
#include <algorithm>


namespace Sourcey {
namespace TURN {


class Server: public ILoggable
{
public:
	struct Options 
	{
		std::string software;
		std::string realm;

		UInt32 allocationDefaultLifetime;		// By default, we require a ping every 90 seconds.
		UInt32 allocationMaxLifetime;	
		UInt32 allocationMaxPermissions;	

		//UInt32 permissionLifetime;			// The default Permission Lifetime is 300 seconds (= 5 minutes).
		Net::Address listenAddr;
		bool enableTCP;

		Options() {
			software							= "Sourcey STUN/TURN Server [rfc5766]";
			realm								= "anionu.com";
			allocationDefaultLifetime			= 90 * 1000;
			allocationMaxLifetime				= 15 * 60 * 1000;
			allocationMaxPermissions			= 10;
			//permissionLifetime					= 3 * 60 * 1000;
			listenAddr							= Net::Address("0.0.0.0", 3478);
			enableTCP							= true;
		}
	};

public:
	Server(IServerObserver& observer, 
		   const Options& options, 
		   //Runner& runner = Runner::getDefault(), 
		   Net::Reactor& reactor = Net::Reactor::getDefault());
	~Server();

	virtual void start();
	virtual void stop();
	
	//void handleRequest(const Request& request);
	void handleRequest(const Request& request, AuthenticationState state);
	void handleAuthorizedRequest(const Request& request);
	void handleBindingRequest(const Request& request);
	void handleAllocateRequest(const Request& request);
	void handleConnectionBindRequest(const Request& request);

	void sendError(const Request& request, int errorCode, const char* errorDesc);
	
	ServerAllocationMap allocations() const;
	void addAllocation(ServerAllocation* alloc);
	void removeAllocation(ServerAllocation* alloc);
	ServerAllocation* getAllocation(const FiveTuple& tuple);
	TCPAllocation* getTCPAllocation(const UInt32& connectionID);
	
	Net::UDPSocket& socketUDP();
	IServerObserver& observer();
	Net::Reactor& reactor();
	//Runner& runner();
	Options& options();
	
	void onTCPConnectionCreated(void* sender, Net::TCPSocket* sock);
	void onPacketReceived(void* sender, STUN::Message& message);
	void onTimer(TimerCallback<Server>& timer);

	virtual const char* className() const { return "TURNServer"; };

private:	
	//Runner&				_runner;
	Net::Reactor&			_reactor;
	Net::UDPSocket	_socketUDP;
	Net::TCPPacketServer	_socketTCP;
	IServerObserver&		_observer;
	Options					_options;
	ServerAllocationMap		_allocations;
	mutable Poco::FastMutex	_mutex;
};


} } //  namespace Sourcey::TURN


#endif // SOURCEY_TURN_Server_H