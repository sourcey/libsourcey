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


#ifndef SOURCEY_ICE_CANDIDATE_PAIR_H
#define SOURCEY_ICE_CANDIDATE_PAIR_H


#include "Sourcey/ICE/LocalCandidate.h"
#include "Sourcey/ICE/RemoteCandidate.h"
#include "Sourcey/Net/Address.h"
#include "Sourcey/Flaggable.h"


namespace Sourcey {
namespace ICE {


class CheckList;


class CandidatePair: public Flaggable, public ILoggable  
	/// +-------------------------------+
	/// |                               |
	/// | Local     Remote              |
	/// | +----+    +----+   +Default?  |
	/// | |Cand|    |Cand|   +Valid?    |
	/// | +----+    +----+   +Nominated?|
	/// |                    +State     |
	/// |                               |
	/// |                               |
	/// |          Candidate Pair       |
	/// +-------------------------------+
{
public:
	enum State 
	{
		Frozen		= 0x01,
			// A check for this pair hasn't been performed, and it can't
			// yet be performed until some other check succeeds, allowing this
			// pair to unfreeze and move into the Waiting state.

		Waiting		= 0x02,
			// A check has not been performed for this pair, and can be
			// performed as soon as it is the highest-priority Waiting pair on
			// the check list.

		InProgress	= 0x04,
			// A check has been sent for this pair, but the
			// transaction is in progress.

		Succeeded	= 0x08, 
			// A check for this pair was already done and produced a
			// successful result.

		Failed		= 0x10,
			// A check for this pair was already done and failed, either
			// never producing any response or producing an unrecoverable failure
			// response.
	};

	enum Flags 
	{
		Default					= 0x01,
			// The candidate pairs whose local and remote candidates are both the
			// default candidates for a particular component is called,
			// unsurprisingly, the default candidate pair for that component. This
			// is the pair that would be used to transmit media if both agents had
			// not been ICE aware.

		Valid					= 0x02,
			// A valid pair is a candidate pair that has been validated by a STUN
			// connectivity check.

		Nominated				= 0x04, 
			// If a valid candidate pair has its nominated flag set, it
			// means that it may be selected by ICE for sending and receiving
			// media.

		Selected				= 0x08,
			// Selected Pair, Selected Candidate:  The candidate pair selected by
			// ICE for sending and receiving media is called the selected pair,
			// and each of its candidates is called the selected candidate.

		NominateOnSuccess		= 0x10,
			// @see: Updating the Nominated Flag
			// http://tools.ietf.org/html/rfc5245#section-7.2.1.5

		UpdateLoosingOnResponse	= 0x20,
			// @see: Existing Media Streams and remote-candidates
			// http://tools.ietf.org/html/rfc5245#section-9.2.2.3

		OwnsChildCandidates		= 0x40
			// Takes ownership of child candidates, deleting them when the
			// candidate pair is deleted.
	};

	//CandidatePair();
	CandidatePair(CheckList& queue, LocalCandidate* local, RemoteCandidate* remote);
	//CandidatePair(const CandidatePair& r);
	~CandidatePair();

	void setLocalCandidate(LocalCandidate* local); // { _local = local; };
	void setRemoteCandidate(RemoteCandidate* remote); // { _remote = remote; };
	void setPriority(UInt32 priority) { _priority = priority; };
	void setState(const State& state);
	void setDefault() { setFlag(Default); };
	void setValid() { setFlag(Valid); };
	void setNominated() { setFlag(Nominated); };
	void setSelected() { setFlag(Selected); };
	void setFlag(UInt32 flag);

	LocalCandidate* local() const { return _local; };	
	RemoteCandidate* remote() const { return _remote; };	
	bool isDefault() const { return hasFlag(Default); };
	bool isValid() const { return hasFlag(Valid); };
	bool isNominated() const { return hasFlag(Nominated); };
	bool isSelected() const { return hasFlag(Selected); };
	UInt32 priority() const { return _priority; };
	std::string foundation() const { return _local->foundation() + _remote->foundation(); };
	State state() const { return _state; };	

	bool valid();

	std::string toString() const;
	void printLog(std::ostream& ost) const;

	static std::string stateToString(UInt32 state);
	static std::string flagToString(UInt32 flag);
	std::string stateString() const;
	std::string flagsString() const;

	//CandidatePair& operator = (const CandidatePair& r);
	bool operator == (const CandidatePair& r) const;
	bool operator != (const CandidatePair& r) const;
	
	// For stl::list:sort operations.
	static bool CompareProiroty(const CandidatePair* l, const CandidatePair* r) {
		return l->priority() > r->priority();
	}
	
	bool sendConnectivityCheck();
	bool sendConnectivityCheckResponse(const STUN::Message& request, const Net::Address& peerAddress); //const Net::Address& localAddress, 
	void cancelConnectivityCheck();
	void clearTransaction(STUN::Transaction* transaction);
	void clearAllTransactions();

	//void onConnectivityCheckSuccess(const void* sender, STUN::MessageIPacket& response);
	//void onConnectivityCheckFailed(const void* sender, STUN::TransactionIPacket& request);
	
	//bool onPotentialConnectivityCheckResponse(const STUN::Message& message, const Net::Address& peerAddress);
		// Returns true is this is our response, false otherwise.

	void onReceiveSTUNMessage(void* sender, const STUN::Message& message, const Net::Address& peerAddress);	
	void onTransactionStateChange(void* sender, TransactionState& state, const TransactionState&);
	void onCandidateStateChange(void* sender, CandidateState& state, const CandidateState&);
	//void onConnectivityCheckResponse(void* sender, STUN::Transaction& transaction);	
	//void onConnectivityCheckFailed(void* sender, STUN::Transaction& transaction);
	
	virtual const char* className() const { return "CandidatePair"; }
	
//private:	
	int send(const char* data, int size);
	int send(const IPacket& packet);
	//int send(STUN::Transaction* transaction);	
		
private:
	LocalCandidate* _local;
	RemoteCandidate* _remote;
	STUN::Transaction* _transaction;
	STUN::Transaction* _cancelledTransaction;
	CheckList& _queue;
	State _state;
	//int _failedCount;
	UInt32 _priority;
};


typedef std::list<CandidatePair*> CandidatePairList;


} // namespace Sourcey
} // namespace ICE 


#endif // SOURCEY_ICE_CANDIDATE_PAIR_H
