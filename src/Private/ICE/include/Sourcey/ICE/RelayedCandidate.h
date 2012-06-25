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


#ifndef SOURCEY_ICE_RELAYED_CANDIDATE_H
#define SOURCEY_ICE_RELAYED_CANDIDATE_H


#include "Sourcey/ICE/LocalCandidate.h"
#include "Sourcey/ICE/RemoteCandidate.h"

#include "Sourcey/TURN/client/Client.h"


namespace Sourcey {
namespace ICE { 


class RelayedCandidate: public LocalCandidate, TURN::IClientObserver
{
public:
	RelayedCandidate(Component& component, const TURN::Client::Options& options);

	void resolve();
	void terminate();
	void makeRedundant();
	
	int send(const char* data, int size, const Net::Address& peerAddress);
	int send(const IPacket& packet, const Net::Address& peerAddress);

	//bool send(STUN::Transaction* transaction);
	//void sendError(const STUN::Message& request, const Net::Address& peerAddress, int errorCode, const char* errorDesc);

	//void onAllocationCreated(STUN::Transaction* transaction);
	//void onAllocationDeleted(STUN::Transaction* transaction);
	//void onDataIndicationReceived(STUN::DataPacket& packet);
	//void onRelayedData(STUN::DataPacket& packet);
	
	/*
	void onAllocationCreated(Client* client, TURN::ClientAllocation* allocation) = 0;
	void onAllocationFailed(Client* client, int errorCode) = 0; //STUN::Transaction* transaction, 
	void onAllocationDeleted(Client* client, TURN::ClientAllocation* allocation) = 0; //STUN::Transaction* transaction
	//void onAllocationAllocationPermissionsCreated(Client* client, TURN::ClientAllocation* allocation) = 0; //, const PermissionList& permissions
	void onRelayedData(Client* client, const char* data, int size) = 0;

	void onTransactionResponse(Client* client, STUN::Transaction* transaction) = 0;

	void onAllocationCreated(TURN::Client* client, TURN::ClientAllocation* allocation);	
	void onAllocationFailed(TURN::Client* client, int errorCode);
	void onAllocationDeleted(TURN::Client* client, TURN::ClientAllocation* allocation);
	void onAllocationPermissionsCreated(TURN::Client* client, TURN::ClientAllocation* allocation);
	void onRelayedData(TURN::Client* client, const char* data, int size, const Net::Address& peerAddress);
	void onTransactionResponse(TURN::Client* client, const TransactionState& state, const STUN::Message& response);
	*/

	void onRelayStateChange(TURN::Client&, TURN::ClientState& state, const TURN::ClientState&);
	void onRelayedData(TURN::Client&, const char* data, int size, const Net::Address& peerAddress);

	void onRemoteCandidateAdded(void*, RemoteCandidate& candidate);

	virtual const char* className() const { return "RelayedCandidate"; }

protected:
	virtual ~RelayedCandidate();	
	void onSTUNTransactionComplete(STUN::Transaction* transaction, Net::ISocket* socket) {};	
	void onSTUNTransactionTimeout(STUN::Transaction* transaction) {};

	TURN::Client _client;
};


} // namespace Sourcey
} // namespace ICE 


#endif // SOURCEY_ICE_RELAYED_CANDIDATE_H