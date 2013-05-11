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


#ifndef SOURCEY_TURN_Client_H
#define SOURCEY_TURN_Client_H


#include "Sourcey/TURN/FiveTuple.h"
#include "Sourcey/TURN/Util.h"
#include "Sourcey/TURN/IAllocation.h"
#include "Sourcey/TURN/Types.h"
#include "Sourcey/STUN/Transaction.h"
#include "Sourcey/Stateful.h"
#include "Sourcey/Net/UDPSocket.h"

#include <deque>


namespace Scy {
namespace TURN {


struct ClientState: public State  
{
	enum Type 
	{
		Waiting					= 0x00, 
		Allocating				= 0x02, 
		Authorizing				= 0x04, 
		Success					= 0x08, 
		Terminated				= 0x10, 
		Failed					= 0x20
	};

	std::string toString() const 
	{ 
		switch(id()) {
		case Waiting:			return "Waiting";
		case Allocating:		return "Allocating";
		case Authorizing:		return "Authorizing";
		case Success:			return "Success";
		case Terminated:		return "Terminated";
		case Failed:			return "Failed";
		}
		return "undefined"; 
	};
};


class Client;


struct IClientObserver 
{
	virtual void onRelayStateChange(Client& client, ClientState& state, const ClientState& oldState) = 0;

	virtual void onRelayedData(Client& client, const char* data, int size, const Net::Address& peerAddress) = 0;

	virtual void onAllocationCreated(Client& client, const STUN::Transaction& transaction) {};
	virtual void onAllocationFailed(Client& client, int errorCode, const std::string& reason) {};
	virtual void onAllocationDeleted(Client& client, const STUN::Transaction& transaction) {};
	virtual void onAllocationPermissionsCreated(Client& client, const PermissionList& permissions) {};

	virtual void onTransactionResponse(Client& client, const STUN::Transaction& transaction) {};
		// All received transaction responses will be routed here after local
		// processing so the observer can easily implement extra functionality.
};


class Client: public Stateful<ClientState>, protected IAllocation
{
public:
	struct Options 
	{
		std::string software;
		std::string username;
		std::string password;
		long timeout;
		UInt32 lifetime;
		UInt32 timerInterval;
		Net::Address serverAddr;
		Options() {
			software				= "Sourcey STUN/TURN Client [rfc5766]";
			username				= CryptoProvider::generateRandomKey(4);
			password				= CryptoProvider::generateRandomKey(22);
			lifetime				= 5 * 60 * 1000; // 5 minutes
			timeout					= 10 * 1000;
			timerInterval			= 30 * 1000; // 30 seconds
			serverAddr				= Net::Address("127.0.0.1", 3478);
		}
	};

public:
	Client(IClientObserver& observer,
		   Net::Reactor& reactor,
		   Runner& runner,
		   const Options& options = Options());
	virtual ~Client();

	virtual void initiate();
		// Initiates the allocation sequence.

	virtual void terminate();
		// Terminates the client and destroys all allocations.

	virtual void sendAllocate();
		// Sends the allocation request.
	
	virtual void addPermission(const IPList& peerIPs);	
	virtual void addPermission(const Net::IP& ip);
		// Peer permissions should be added/created before we kick
		// off the allocation sequence, but may be added later.

	virtual void sendCreatePermission();
		// Sends a CreatePermission request including all hosts
		// added via addPermission();
		// A CreatePermission request will be sent as soon as the 
		// Allocation is created, and at timer x intervals.

	virtual void sendChannelBind(const Net::IP& peerIP);
	virtual void sendRefresh();
	virtual void sendData(const char* data, int size, const Net::Address& peerAddress);	
	
	virtual bool handleResponse(const STUN::Message& response);
	virtual void handleAllocateResponse(const STUN::Message& response);
	virtual void handleAllocateErrorResponse(const STUN::Message& response);
	virtual void handleCreatePermissionResponse(const STUN::Message& response);
	virtual void handleCreatePermissionErrorResponse(const STUN::Message& response);
	virtual void handleRefreshResponse(const STUN::Message& response);
	virtual void handleDataIndication(const STUN::Message& response);
	
	virtual int transportProtocol();
	virtual Net::IPacketSocket* createSocket();
	virtual STUN::Transaction* createTransaction(Net::IPacketSocket* socket = NULL);
	virtual void authenticateRequest(STUN::Message& request);
	virtual bool sendAuthenticatedTransaction(STUN::Transaction* transaction);
	virtual bool removeTransaction(STUN::Transaction* transaction);

	Net::Address mappedAddress() const;
	Net::Address relayedAddress() const;
	bool isTerminated() const;	
	
	IClientObserver& observer();
	Net::IPacketSocket& socket();
	Runner& runner();
	Net::Reactor& reactor();
	Options& options();	
	
	virtual void onReceiveSTUNMessage(void* sender, STUN::Message& message);
	virtual void onClientConnect(void* sender);
	virtual void onClientDisconnect(void* sender);
	virtual void onStateChange(ClientState& state, const ClientState& oldState);
	virtual void onTransactionStateChange(void* sender, TransactionState& state, const TransactionState&);	
	virtual void onTimer(void*);
	//virtual void onTimer(TimerCallback<Client>& timer);

protected:
	IClientObserver&	_observer;
	Runner&				_runner;
	Net::Reactor&		_reactor;
	Options				_options;
	Net::IPacketSocket*	_socket;
	TimerTask*          _timer;

	Net::Address		_mappedAddress;
	Net::Address		_relayedAddress;

	std::string			_realm;
	std::string			_nonce;
	
	std::deque<STUN::Message*> _pendingRequests;
		// A list of queued packets awaiting server permissions

	std::vector<STUN::Transaction*> _transactions;
		// A list containing currently active transactions


	mutable Poco::FastMutex _mutex;
};


} } //  namespace Scy::TURN


#endif // SOURCEY_TURN_Client_H