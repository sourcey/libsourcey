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


#ifndef SCY_TURN_Client_H
#define SCY_TURN_Client_H


#include "scy/turn/fivetuple.h"
#include "scy/turn/util.h"
#include "scy/turn/iallocation.h"
#include "scy/turn/types.h"
#include "scy/stun/transaction.h"
#include "scy/stateful.h"
#include "scy/net/udpsocket.h"

#include <deque>


namespace scy {
namespace turn {


struct ClientState: public State  
{
	enum Type 
	{
		None					= 0x00, 
		Allocating				= 0x02, 
		Authorizing				= 0x04, 
		Success					= 0x08, 
		//Terminated				= 0x10, 
		Failed					= 0x10
	};

	std::string toString() const 
	{ 
		switch(id()) {
		case None:			return "None";
		case Allocating:		return "Allocating";
		case Authorizing:		return "Authorizing";
		case Success:			return "Success";
		//case Terminated:		return "Terminated";
		case Failed:			return "Failed";
		}
		return "undefined"; 
	};
};


class Client;


struct ClientObserver 
{
	virtual void onClientStateChange(Client& client, ClientState& state, const ClientState& oldState) = 0;
	
	virtual void onRelayDataReceived(Client& client, const char* data, int size, const net::Address& peerAddress) = 0;

	virtual void onAllocationCreated(Client& client, const stun::Transaction& transaction) {};
	virtual void onAllocationFailed(Client& client, int errorCode, const std::string& reason) {};
	virtual void onAllocationDeleted(Client& client, const stun::Transaction& transaction) {};
	virtual void onAllocationPermissionsCreated(Client& client, const PermissionList& permissions) {};

	virtual void onTransactionResponse(Client& client, const stun::Transaction& transaction) {};
		// All received transaction responses will be routed here after local
		// processing so the observer can easily implement extra functionality.

	virtual void onTimer(Client& client) {};
		// Fires after the client's internal timer callback.
		// Handy for performing extra async cleanup tasks.
};


class Client: public Stateful<ClientState>, protected IAllocation
{
public:
	struct Options 
	{
		std::string software;
		std::string username;
		std::string password;
		//std::string realm;
		long timeout;
		Int64 lifetime;
		Int64 timerInterval;
		net::Address serverAddr;
		Options() {
			software				= "Sourcey STUN/TURN Client [rfc5766]";
			username				= util::randomString(4);
			password				= util::randomString(22);
			//realm				    = "sourcey.com";
			lifetime				= 5 * 60 * 1000; // 5 minutes
			timeout					= 10 * 1000;
			timerInterval			= 30 * 1000; // 30 seconds
			serverAddr				= net::Address("127.0.0.1", 3478);
		}
	};
	
public:
	Client(ClientObserver& observer, const Options& options = Options()); //net::SocketBase* socket, 
	virtual ~Client();

	virtual void initiate();
		// Initiates the allocation sequence.

	virtual void shutdown();
		// Shutdown the client and destroy the active allocation.

	virtual void sendAllocate();
		// Sends the allocation request.
	
	virtual void addPermission(const IPList& peerIPs);	
	virtual void addPermission(const std::string& ip);
		// Peer permissions should be added/created before we kick
		// off the allocation sequence, but may be added later.

	virtual void sendCreatePermission();
		// Sends a CreatePermission request including all hosts
		// added via addPermission();
		// A CreatePermission request will be sent as soon as the 
		// Allocation is created, and at timer x intervals.

	virtual void sendChannelBind(const std::string& peerIP);
	virtual void sendRefresh();
	virtual void sendData(const char* data, int size, const net::Address& peerAddress);	
	
	virtual bool handleResponse(const stun::Message& response);
	virtual void handleAllocateResponse(const stun::Message& response);
	virtual void handleAllocateErrorResponse(const stun::Message& response);
	virtual void handleCreatePermissionResponse(const stun::Message& response);
	virtual void handleCreatePermissionErrorResponse(const stun::Message& response);
	virtual void handleRefreshResponse(const stun::Message& response);
	virtual void handleDataIndication(const stun::Message& response);
	
	virtual int transportProtocol();
	virtual stun::Transaction* createTransaction(net::Socket* socket = nullptr);
	virtual void authenticateRequest(stun::Message& request);
	virtual bool sendAuthenticatedTransaction(stun::Transaction* transaction);
	virtual bool removeTransaction(stun::Transaction* transaction);

	net::Address mappedAddress() const;
	net::Address relayedAddress() const;

	bool closed() const;	
	
	ClientObserver& observer();
	Options& options();	
	
	virtual void onSocketRecv(void* sender, net::SocketPacket& packet);
	virtual void onSocketConnect(void* sender);
	virtual void onSocketClose(void* sender);
	virtual void onTransactionProgress(void* sender, TransactionState& state, const TransactionState&);	
	virtual void onStateChange(ClientState& state, const ClientState& oldState);
	virtual void onTimer(void*);

	//virtual const char* className() const { return "TURNClient"; };

protected:
	ClientObserver&	_observer;
	net::Socket _socket;
	Options _options;
	Timer _timer;

	net::Address _mappedAddress;
	net::Address _relayedAddress;

	std::string _realm;
	std::string _nonce;
	
	std::deque<stun::Message> _pendingIndications;
		// A list of queued Send indication packets awaiting server permissions

	std::vector<stun::Transaction*> _transactions;
		// A list containing currently active transactions

	//mutable Mutex _mutex;
};


} } //  namespace scy::turn


#endif // SCY_TURN_Client_H