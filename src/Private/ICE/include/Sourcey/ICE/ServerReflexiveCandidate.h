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


#ifndef SOURCEY_ICE_SERVER_REFLEXIVE_CANDIDATE_H
#define SOURCEY_ICE_SERVER_REFLEXIVE_CANDIDATE_H


#include "Sourcey/ICE/LocalCandidate.h"
#include "Sourcey/STUN/Transaction.h"


namespace Sourcey {
namespace ICE { 


class ServerReflexiveCandidate: public LocalCandidate
{
public:
	ServerReflexiveCandidate(Component& component, LocalCandidate* base);

	void resolve();
	void terminate();
	void makeRedundant();

	int send(const char* data, int size, const Net::Address& peerAddress);
	int send(const IPacket& packet, const Net::Address& peerAddress);
	//bool send(STUN::Transaction* transaction);
	//void sendError(const STUN::Message& request, const Net::Address& peerAddress, int errorCode, const char* errorDesc);

	//void resolveServerReflexiveAddr();
	
	virtual const char* className() const { return "ServerReflexiveCandidate"; }

protected:
	~ServerReflexiveCandidate();
	void onTransactionStateChange(void* sender, TransactionState& state, const TransactionState& oldState);
	//void onSTUNTransactionComplete(STUN::Transaction* transaction, Net::ISocket* socket);	
	//void onSTUNTransactionTimeout(STUN::Transaction* transaction);
	STUN::Transaction* _transaction;
};


} // namespace Sourcey
} // namespace ICE 


#endif // SOURCEY_ICE_SERVER_REFLEXIVE_CANDIDATE_H