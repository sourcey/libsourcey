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


#ifndef SCY_TURN_UDPClient_H
#define SCY_TURN_UDPClient_H


#include "scy/turn/client/client.h"
#include "scy/turn/util.h"
#include "scy/turn/iallocation.h"
#include "scy/turn/types.h"
#include "scy/stun/transaction.h"
#include "scy/stateful.h"
#include "scy/net/udpsocket.h"

#include <deque>


namespace scy {
namespace turn {


class UDPClient: public Client
{	
public:
	UDPClient(ClientObserver& observer, const Options& options = Options());
	virtual ~UDPClient();
};


} } //  namespace scy::turn


#endif // SCY_TURN_Client_H



	
	/*
	virtual void initiate();
		// Initiates the allocation sequence.

	virtual void terminate();
		// Terminates the client and destroys all allocations.

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
	virtual void sendData(const char* data, std::size_t size, const net::Address& peerAddress);	
	
	virtual bool handleResponse(const stun::Message& response);
	virtual void handleAllocateResponse(const stun::Message& response);
	virtual void handleAllocateErrorResponse(const stun::Message& response);
	virtual void handleCreatePermissionResponse(const stun::Message& response);
	virtual void handleCreatePermissionErrorResponse(const stun::Message& response);
	virtual void handleRefreshResponse(const stun::Message& response);
	virtual void handleDataIndication(const stun::Message& response);
	
	virtual int transportProtocol();
	virtual net::Socket* createSocketBase();
	virtual stun::Transaction* createTransaction(net::Socket* socket = nullptr);
	virtual void authenticateRequest(stun::Message& request);
	virtual bool sendAuthenticatedTransaction(stun::Transaction* transaction);
	virtual bool removeTransaction(stun::Transaction* transaction);

	net::Address mappedAddress() const;
	net::Address relayedAddress() const;
	bool isTerminated() const;	
	
	ClientObserver& observer();
	net::Socket& socket();
	//uv::Loop* loop();
	//Net::Reactor& reactor();
	Options& options();	
	
	virtual void onSTUNReceive(void* sender, stun::Message& message);
	virtual void onSocketConnect(void* sender);
	virtual void onSocketClose(void* sender, const Error& error) ;
	virtual void onStateChange(ClientState& state, const ClientState& oldState);
	virtual void onTransactionProgress(void* sender, TransactionState& state, const TransactionState&);	
	virtual void onTimer(void*);
	//virtual void onTimer(TimerCallback<Client>& timer);

	//virtual const char* className() const { return "TURNClient"; };

protected:
	//ClientObserver&	_observer;
	//Runner&				_runner;
	//Net::Reactor&		_reactor;Base
	//Options				_options;
	//net::Socket	_socket;
	//Timer*          _timer;

	net::Address		_mappedAddress;
	net::Address		_relayedAddress;

	std::string			_realm;
	std::string			_nonce;
	
	std::deque<stun::Message*> _pendingIndications;
		// A list of queued packets awaiting server permissions

	std::vector<stun::Transaction*> _transactions;
		// A list containing currently active transactions

	//mutable Mutex _mutex;
	*/