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


#ifndef SOURCEY_ICE_CHECKLIST
#define SOURCEY_ICE_CHECKLIST


#include "Sourcey/Timer.h"
#include "Sourcey/ICE/State.h"
#include "Sourcey/ICE/Candidate.h"
#include "Sourcey/ICE/CandidatePair.h"
#include "Sourcey/ICE/Component.h"

#include "Sourcey/Util.h"
#include "Sourcey/Logger.h"

#include <iostream>
#include <string>
#include <list>
#include <list>
#include <assert.h>


namespace Sourcey {
namespace ICE {


class MediaStream;
typedef std::list<CandidatePair*> TirggeredCheckQueue;


class CheckList: public ILoggable
{
	// The CheckList is responsible for processing and computing the
	// connectivity of Candiate Pairs from the Media Channel.
	// NOTE: The CheckList only supports Aggressive Nomination.
public:
	enum State 
	{
		Running, 
			// In this state, ICE checks are still in progress for this
			// media stream.

		Completed, 
			// In this state, ICE checks have produced nominated pairs
			// for each component of the media stream. Consequently, ICE has
			// succeeded and media can be sent.

		Failed
			// In this state, the ICE checks have not completed
			// successfully for this media stream.
	};

	CheckList(MediaStream& stream);
	~CheckList();

	//void startConnectivityChecks();

	//
	// State Machine
	//
	void setState(State state);
	State state() const { return _state; };
	bool isRunning() const;	
	bool isCompleted() const;	
	bool isFailed() const;	
	MediaStream& stream() const { return _stream; };
	TirggeredCheckQueue queue() const { return _queue; };
	
	//
	// Actions
	//
	void startConnectivityChecks();
	void startTriggeredChecks();
	void stopTriggeredChecks();
	bool doUpdateStates();
		// Update States 8.1.2

	void createCandidatePairs();
		// Creates candidate pairs when we enter the CheckingConnectivity
		// state.

	void updateSelectedPairs();
		// Selected pairs are generated when the check list is Completed.
		// A selected pair is equal to the highest-priority nominated pair
		// for that component in the valid list if the state of the check 
		// list is Completed.

	void checkUpdatedPairs();
		// Initializes connectivity checking for candidate pairs crated
		// as a result of the updated offer.

	//
	// Triggered Checks
	//
	void pushTriggeredPair(CandidatePair* pair);
	void removeTriggeredPair(CandidatePair* pair);
	CandidatePair* popNextTriggeredPair();
	CandidatePair* getNextCandidatePair(CandidatePair::State state = CandidatePair::Waiting);
	void onTriggeredCheck(TimerCallback<CheckList>& timer);

	bool tryAddCandidatePair(CandidatePair* pair);
	void removeCandidatePair(CandidatePair* pair);
	void deleteCandidatePairs();
	CandidatePair* getCandidatePair(const Candidate* local, const Candidate* remote);
	CandidatePair* getCandidatePair(const Net::Address& local, const Net::Address& remote);
	CandidatePair* getValidPair(const Candidate* local, const Candidate* remote) const;
	CandidatePairList pairs() const;	
	CandidatePairList pairs(CandidatePair::Flags flag) const;
	CandidatePairList pairs(int componentID) const;

	void addUpdatedPair(CandidatePair* pair);

	bool hasValidPairsAllComponents() const;
	bool hasNominatedPairs() const;
	bool hasNominatedPairsAllComponents() const;
	bool isNominatedPairsMatchDefaultPairs() const;

	//
	// Events
	//	
	void onReceiveSTUNMessage(void* sender, const STUN::Message& message, const Net::Address& peerAddress);	

	void onConnectivityCheckRequest(LocalCandidate* local, const STUN::Message& request, /*const Net::Address& localAddress,*/ const Net::Address& peerAddresst);

	void onConnectivityCheckSuccess(CandidatePair* pair, STUN::Message& response);
	void onConnectivityCheckFailed(CandidatePair* pair, STUN::Message& response);
	void onAdjacentMediaStreamValidPairsCreated(CandidatePairList& pairs);

	void printLog(std::ostream& ost) const;
	
	void sendError(LocalCandidate* local, const STUN::Message& request, const Net::Address& peerAddress, int errorCode, const char* errorDesc);
		
	virtual const char* className() const { return "CheckList"; }

private:
	State _state;
	MediaStream& _stream;
	TirggeredCheckQueue _queue;
	CandidatePairList _pairs;		
	CandidatePairList _updatedPairs;	
	
	mutable Poco::FastMutex _mutex;

	friend class MediaStream;
};


} // namespace Sourcey
} // namespace ICE 


#endif // SOURCEY_ICE_CHECKLIST
