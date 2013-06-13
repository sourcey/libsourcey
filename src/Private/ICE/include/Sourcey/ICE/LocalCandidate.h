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


#ifndef SOURCEY_ICE_LOCAL_CANDIDATE_H
#define SOURCEY_ICE_LOCAL_CANDIDATE_H


#include "Sourcey/ICE/Candidate.h"
#include "Sourcey/STUN/Message.h"
#include "Sourcey/STUN/Transaction.h"
#include "Sourcey/Net/ISocket.h"


namespace Sourcey {
namespace ICE { 


struct CandidateState: public StateT  
{
	enum Type 
	{
		Waiting				= 0x00, 
		Resolving			= 0x01, 
		Success				= 0x02, 
		Redundant			= 0x04, 
		Failed				= 0x08, 
		Terminating			= 0x10
	};

	std::string toString() const 
	{ 
		switch(id()) {
		case Waiting:		return "Waiting";
		case Resolving:		return "Resolving";
		case Success:		return "Success";
		case Redundant:		return "Redundant";			
		case Failed:		return "Failed";
		case Terminating:	return "Terminating";
		}
		return "undefined"; 
	};
};


class Component;


class LocalCandidate: public Candidate, public StatefulSignal<CandidateState>
	/// LocalCandidate's are obtained by an agent for every stream
	/// component and are then included in outgoing offers or answers.  
{
public:
	LocalCandidate(Component& component, Candidate::Type type, LocalCandidate* base = NULL); //, State state = Waiting
	
	virtual ~LocalCandidate();
		// Candidates are terminated(), complete with eco garbage collection!

	virtual void resolve() = 0;
		// This method will be called by the parent component to initialize
		// the candidate gathering sequence. Overriding methods MUST set the
		// candidate state to Resolving.

	virtual void terminate() = 0;
		// This method will be called if a candidate fails or gets pruned
		// and is scheduled for deletion. All callbacks from this candidate
		// should immediately cease. Overriding methods MUST set the state
		// to Terminating.

	virtual void makeRedundant() = 0;
		// This method will be called if a candidate is found to be redundant
		// as per rfc5245 section 4.1.3. This will not delete the internal 
		// candidate as it may still be the base of another candidate with a
		// higher priority. Overriding methods MUST set the state to Redundant.

	virtual int send(const char* data, int size, const Net::Address& peerAddress) = 0;
	virtual int send(const IPacket& packet, const Net::Address& peerAddress) = 0;
	//virtual bool send(STUN::Transaction* transaction) = 0;
	//virtual void sendError(const STUN::Message& request, const Net::Address& peerAddress, int errorCode, const char* errorDesc) = 0;

	//virtual void cancelTransaction();

	virtual void setBase(LocalCandidate* base) { _base = base; };
		// The base of a server reflexive candidate is the host candidate       
		// from which it was derived. A host candidate is also said to have       
		// a base, equal to that candidate itself. Similarly, the base of a       
		// relayed candidate is that candidate itself.

	virtual LocalCandidate* base() { return _base; };
	virtual Component& component() const { return _component; };	
	//virtual STUN::Transaction* transaction() const { return _transaction; };
	//virtual State state() const { return _state; };	
	//virtual bool setState(State state);
		// Returns false if the candidate was pruned.
	
	virtual int preference() const;
		// Returns the preference order for this candidate type.

	virtual std::string toSDP() const;
	virtual void computeFoundation();
	virtual void computePriority();
	static std::string computeFoundation(int type, const Net::IP& ip);
	static UInt32 computePriority(int typePreference, int componentID, int localPreference = 65535);
	
	Signal2<const STUN::Message&, const Net::Address&> STUNMessageReceived;
	//Signal2<const DataPacket&, const Net::Address&> OnData;
	//Signal<STUN::Transaction&> ConnectivityCheckResponse;
	//Signal<STUN::Transaction&> ConnectivityCheckFailed;

protected:

	//virtual void onTransactionStateChange(void* sender, TransactionState& state, const TransactionState&);
	//virtual void onSTUNTransactionComplete(STUN::Transaction* transaction, Net::ISocket* socket);	
	//virtual void onSTUNTransactionTimeout(STUN::Transaction* transaction);

protected:
	Component&			_component;
	LocalCandidate*		_base;
	//STUN::Transaction*	_transaction;
	//State				_state;
};


typedef std::list<LocalCandidate*> LocalCandidateList;


} // namespace Sourcey
} // namespace ICE 


#endif // SOURCEY_ICE_LOCAL_CANDIDATE_H