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


#include "Sourcey/ICE/CheckList.h"
#include "Sourcey/ICE/MediaStream.h"
#include "Sourcey/ICE/State.h"
#include "Sourcey/ICE/Agent.h"
#include "Sourcey/ICE/PeerReflexiveCandidate.h"
#include "Sourcey/Timer.h"


using namespace std;


namespace Sourcey {
namespace ICE {


CheckList::CheckList(MediaStream& stream) : 
	_stream(stream) 
{ 
	setState(CheckList::Running);
}


CheckList::~CheckList() 
{
	Timer::getDefault().stop(TimerCallback<CheckList>(this, &CheckList::onTriggeredCheck));
	deleteCandidatePairs();
}


void CheckList::deleteCandidatePairs()
{	
	Util::ClearList(_pairs);
	//for (CandidatePairList::const_iterator it = _pairs.begin(); it != _pairs.end(); ++it) {
	//	delete *it;
	//}
}


void CheckList::printLog(ostream& ost) const 
{
	ost << "["
		<< className()
		<< ":"
		<< this
		<< "] ";

	//return _stream.printLog(ostream& ost) + "[CheckList]";
}


void CheckList::setState(CheckList::State state) 
{	
	Poco::FastMutex::ScopedLock lock(_mutex); 	
	assert(_state != state);
	_state = state;

	switch (state) {
	case CheckList::Running:
		Log("debug", this) << "Set to RUNNING" << endl;
		break;
	case CheckList::Completed:
		Log("debug", this) << "Set to COMPLETED" << endl;
		Timer::getDefault().stop(TimerCallback<CheckList>(this, &CheckList::onTriggeredCheck));
		//stopTriggeredChecks();
		break;
	case CheckList::Failed:
		Log("debug", this) << "Set to FAILED" << endl;
		_stream.agent().onMediaStreamCheckingFailed(&_stream);
		Timer::getDefault().stop(TimerCallback<CheckList>(this, &CheckList::onTriggeredCheck));
		//stopTriggeredChecks();
		assert(false);
		break;
	default:
		assert(false);
		break;
	}
}


bool CheckList::isRunning() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);	
	return _state == CheckList::Running;
}


bool CheckList::isCompleted() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);	
	return _state == CheckList::Completed;
}


bool CheckList::isFailed() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);	
	return _state == CheckList::Failed;
}


void CheckList::startTriggeredChecks() 
{
	Poco::FastMutex::ScopedLock lock(_mutex); 	
	Log("debug", this) << "Starting Triggered Checks: " << _stream.agent().options().triggeredCheckDelay << endl;

	Timer::getDefault().start(TimerCallback<CheckList>(this, &CheckList::onTriggeredCheck, 
		_stream.agent().options().triggeredCheckDelay, 
		_stream.agent().options().triggeredCheckDelay));
}


void CheckList::stopTriggeredChecks() 
{
	Poco::FastMutex::ScopedLock lock(_mutex); 	
	Log("debug", this) << "Stopping Triggered Checks: " << _stream.agent().options().triggeredCheckDelay << endl;

	Timer::getDefault().stop(TimerCallback<CheckList>(this, &CheckList::onTriggeredCheck));
}


// ---------------------------------------------------------------------
//
// Connectivity Checking
//
// ---------------------------------------------------------------------
void CheckList::startConnectivityChecks() 
{
	Log("debug", this) << "Starting Connectivity Checks" << endl;	

	assert(_stream.state().id() == ICE::State::CheckingConnectivity);
	//assert(_queue);
	//if (_queue) {
		
		// An agent performs ordinary checks and triggered checks. The
		// generation of both checks is governed by a timer that fires
		// periodically for each media stream. The agent maintains a FIFO
		// queue, called the triggered check queue, which contains candidate
		// pairs for which checks are to be sent at the next available
		// opportunity. When the timer fires, the agent removes the top pair
		// from the triggered check queue, performs a connectivity check on that
		// pair, and sets the state of the candidate pair to In-Progress. If
		// there are no pairs in the triggered check queue, an ordinary check is
		// sent.

		// Once the agent has computed the check lists as described in
		// Section 5.7, it sets a timer for each active check list. The timer
		// fires every Ta*N seconds, where N is the number of active check lists
		// (initially, there is only one active check list). Implementations
		// MAY set the timer to fire less frequently than this. Implementations
		// SHOULD take care to spread out these timers so that they do not fire
		// at the same time for each media stream. Ta and the retransmit timer
		// RTO are computed as described in Section 16. Multiplying by N allows
		// this aggregate check throughput to be split between all active check
		// lists. The first timer fires immediately, so that the agent performs
		// a connectivity check the moment the offer/answer exchange has been
		// done, followed by the next check Ta seconds later (since there is
		// only one active check list).				
		createCandidatePairs();
		assert(_pairs.size());

		// Each candidate pair in the check list has a foundation and a state.
		// The foundation is the combination of the foundations of the local and
		// remote candidates in the pair. 
		// 
		// The initial states for each pair in a check list are computed by
		// performing the following sequence of steps:
		// 
		// 1. The agent sets all of the pairs in each check list to the Frozen
		//    state.
		//
		// 2. The agent examines the check list for the first media stream (a
		//    media stream is the first media stream when it is described by
		//    the first m line in the SDP offer and answer). For that media
		//    stream:
		//
		//    *  For all pairs with the same foundation, it sets the state of
		// 		 the pair with the lowest component ID to Waiting. If there is
		// 		 more than one such pair, the one with the highest priority is
		// 		 used.
		//
		CandidatePair* currPair;
		CandidatePair* compPair;
		map<string, CandidatePair*> matches;
		for (CandidatePairList::const_iterator it = _pairs.begin(); it != _pairs.end(); ++it) {
		//for (int idx = 0; idx < _pairs.size(); idx++) {
			currPair = *it;

			// TODO: Should we skip if the pair state is not Frozen?
		
			// Loop through other local candidate pairs to determine if there
			// are any others the same foundation.
			for (CandidatePairList::const_iterator it1 = _pairs.begin(); it1 != _pairs.end(); ++it1) {
			//for (int i = 0; i < _pairs.size(); i++) {
				if (*currPair == **it1) continue;
				compPair = *it1;

				// Remember pairs with requesting foundations.
				assert(currPair->valid());
				assert(compPair->valid());
				if (currPair->foundation() == compPair->foundation()) {
					matches[compPair->foundation()] = compPair;
				}
			}

			// If there were matches others then we need to find the pair 
			// with the lowest component ID.
			if (matches.size()) {
				int lowest = 100;
				string selectedID;
				for (map<string, CandidatePair*>::const_iterator it = matches.begin(); it != matches.end(); ++it) {
					if (lowest > it->second->local()->componentID()) {
						lowest = it->second->local()->componentID();
						selectedID = it->second->foundation();
					}
				}
				if (selectedID.size())
					currPair = matches[selectedID];
			}

			// Finally, set the selected pair's state to waiting.	 
			assert(currPair);
			currPair->setState(CandidatePair::Waiting);
		}

		// Kick off the timer!
		startTriggeredChecks();
		//startConnectivityChecks();
	//}
}

/*
void CheckList::stopTriggeredChecks() {
	Poco::FastMutex::ScopedLock lock(_mutex); 	
	Log("debug", this) << "Stopping Triggered Checks" << endl;

	//Notifier::getDefault().removeObserver(Observer<CheckList, TimerNotification>(this, &CheckList::onTriggeredCheck));
	Timer::getDefault().stop(TimerCallback<CheckList>(this, &CheckList::onTriggeredCheck, _delay, 100));
}
*/



void CheckList::pushTriggeredPair(CandidatePair* pair) 
{	
	//FastMutex::ScopedLock lock(_mutex); 	

	// Filter duplicates.
	for (TirggeredCheckQueue::const_iterator it = _queue.begin(); it != _queue.end(); ++it) {	
		if (**it == *pair) {
			Log("debug", this) << "Unable to add duplicate TRIGGERED PAIR: " << _queue.size() << endl;
			return;
		}
	}

	// The pair is also maintained the our regular pair list,
	// which will be responsible for it's destruction.
	_queue.push_back(pair);
	_queue.sort(CandidatePair::CompareProiroty); 

	Log("debug", this) << "Added TRIGGERED PAIR: " << _queue.size() << endl;

	// Restart checks when an item is added and queue is inactive?
	if (!isRunning())
		startTriggeredChecks();
}


CandidatePair* CheckList::popNextTriggeredPair() 
{
	//FastMutex::ScopedLock lock(_mutex); 	

	if (_queue.empty())
		return NULL;

	// TODO: Should we be filtering states here?
	CandidatePair* pair = NULL;
	while (true) {
		pair = _queue.front();
		_queue.pop_front();
		if (pair &&
			pair->state() != CandidatePair::Succeeded && 
			pair->state() != CandidatePair::InProgress) {
			break;
		} else if (_queue.empty()) {
			pair = NULL;
			break;
		}
	}

	return pair;
}


void CheckList::removeTriggeredPair(CandidatePair* pair) 
{
	for (TirggeredCheckQueue::iterator it = _queue.begin(); it != _queue.end(); ++it) {	
		if (**it == *pair) {
			_queue.erase(it);
			return;
		}
	}
}


void CheckList::onTriggeredCheck(TimerCallback<CheckList>& timer) 
{
	Log("debug", this) << "Running Scheduled Check" << endl;

	//FastMutex::ScopedLock lock(_mutex);

	// Pop the next pair from the triggered check queue.
	CandidatePair* pair = popNextTriggeredPair();
	if (pair) {
		Log("debug", this) << "Checking next TRIGGERED PAIR: " << pair->toString() << endl;
	}

	// When the timer fires and there is no triggered check to be sent, the
	// agent MUST choose an ordinary check as follows:
	//
	// o  Find the highest-priority pair in that check list that is in the
	//    Waiting state.
	if (!pair) {
		pair = getNextCandidatePair(CandidatePair::Waiting);
		if (pair)
			Log("debug", this) << "Checking next WAITING PAIR" << endl;
	} 

	// o  If there is no such pair:
	//
	//    *  Find the highest-priority pair in that check list that is in
	//       the Frozen state.
	//
	//    *  If there is such a pair:
	//
	//       +  Unfreeze the pair.
	//
	//       +  Perform a check for that pair, causing its state to
	//          transition to In-Progress.
	if (!pair) {
		pair = getNextCandidatePair(CandidatePair::Frozen);
		if (pair) {
			Log("debug", this) << "Checking next FROZEN PAIR" << endl;
			pair->setState(CandidatePair::Waiting);	
		}
	}

	// o  If there is such a pair:
	//
	//    *  Send a STUN check from the local candidate of that pair to the
	//       remote candidate of that pair. The procedures for forming the
	//       STUN request for this purpose are described in Section 7.1.2.
	//
	//    *  Set the state of the candidate pair to In-Progress.
	//
	if (pair) {

		// Meer reflexive candidates must always be represented by their base.
		assert(pair->local()->type() != Candidate::PeerReflexive);

		if (pair->sendConnectivityCheck()) {

			// Request sent successfuly, update our state accordingly.			
			if (pair->state() == CandidatePair::Waiting)
				pair->setState(CandidatePair::InProgress);	

			// Occasionally race conditions occur where the pair's response has 
			// already been recieved and the state updated.
			else
				Log("debug", this) << "Race condition encountered for pair. " 
					<< "Sending extra request: " << pair->toString() << endl;

			// NOTE: STUN responses from peers will be handled via the MediaStream.
		} else {

			// Request failed, update our state accordingly.
			pair->setState(CandidatePair::Failed);
		}
	} 
	
	//    *  If there is no such pair:
	//
	//       +  Terminate the timer for that check list.
	else {
		//Log("debug", this) << "No pairs to process." << endl;
		//return false;
		timer.cancel();
		//stopTriggeredChecks();
	}
}


// ---------------------------------------------------------------------
//
// Candidates
//
// ---------------------------------------------------------------------
void CheckList::createCandidatePairs() 
{	
	Log("debug", this) << "Forming Candidate Pairs: " 
		 << _stream.localCandidates().size() << " local, " << _stream.remoteCandidates().size() << " remote" << endl;

	// Forming Candidate Pairs
	//
	// In order to prevent the attacks described in Section 18.5.2, 
	// agents MAY limit the number of candidates they'll accept in an offer
	// or answer. A local candidate is paired with a remote candidate if
	// and only if the two candidates have the same component ID and have
	// the same IP address version. It is possible that some of the local
	// candidates won't get paired with remote candidates, and some of the
	// remote candidates won't get paired with local candidates. This can
	// happen if one agent doesn't include candidates for the all of the
	// components for a media stream. If this happens, the number of
	// components for that media stream is effectively reduced, and
	// considered to be equal to the minimum across both agents of the
	// maximum component ID provided by each agent across all components for
	// the media stream.
	//
	LocalCandidateList localCandidates = _stream.localCandidates();
	RemoteCandidateList remoteCandidates = _stream.remoteCandidates();
	for (LocalCandidateList::iterator lit = localCandidates.begin(); lit != localCandidates.end();) {	
		
		Log("debug", this) << "Finding match for Local Candidate: " << (*lit)->toString() << endl;	
			
		// Filter redundant local candidates.
		if (!(*lit)->stateEquals(CandidateState::Success)) {
			Log("debug", this) << "SKIPPED: " << (*lit)->toString() << endl;	
			lit++;
			continue;
		}

		/*
		// Filter unwanted types.
		if (((*lit)->type() == Candidate::Host && _stream.agent().options().disableHost) ||
			((*lit)->type() == Candidate::ServerReflexive && _stream.agent().options().disableSTUN) ||
			((*lit)->type() == Candidate::Relayed && _stream.agent().options().disableRelay)) {
			Log("debug", this) << "SKIPPED: " << (*lit)->toString() << endl;	
			lit++;
			continue;
		}
		*/

		bool hasMatch = false;

		for (RemoteCandidateList::iterator rit = remoteCandidates.begin(); rit != remoteCandidates.end();) {
			Log("debug", this) << "Trying to pair with Remote Candidate: " << (*rit)->toString() << endl;	

			// Pair our local candidate with the remote candidate if the two 
			// candidates have the same component ID and IP address version.
			if ((*lit)->componentID() == (*rit)->componentID() && 
				(*lit)->transportAddr().family() == (*rit)->transportAddr().family()) {
				CandidatePair* pair = new CandidatePair(*this, *lit, *rit);
				if (tryAddCandidatePair(pair))
					hasMatch = true;		

				remoteCandidates.erase(rit);
				break;
			}
			else {
				rit++;
			}
		}

		if (hasMatch) {
			lit = localCandidates.erase(lit);
		}
		else lit++;

		//if (hasMatch) {
		//	if (lit != localCandidates.end()) {
		//		localCandidates.erase(lit);
		//		lit++;
		//	}
		//}
	}
	
	Log("debug", this)  
		<< " A total of  " << localCandidates.size() 
		<< " local candidates and " << remoteCandidates.size() 
		<< " remote candidates have not been paired" << endl;	
}


void CheckList::updateSelectedPairs() 
{
	// A selected pair is equal to the highest-priority nominated pair
	// for that component in the valid list if the state of the check 
	// list is Completed.
	// TODO: Calculate selected pairs based on latency.	
	ComponentMap components = _stream.components();
	for (ComponentMap::const_iterator it = components.begin(); it != components.end(); ++it) {	
		CandidatePairList componentPairs = pairs(it->first);
		for (CandidatePairList::const_iterator ait = componentPairs.begin(); ait != componentPairs.end(); ++ait) {	
			CandidatePair* pair = *ait;
			pair->removeFlag(CandidatePair::Selected);
			if (pair->hasFlag(CandidatePair::Nominated)) {
				pair->setSelected();
				it->second->setSelectedCandidatePair(pair);			
			}
		}
	}
}


bool CheckList::tryAddCandidatePair(CandidatePair* pair) 
{
	//FastMutex::ScopedLock lock(_mutex); 	

	Log("debug", this) << "Adding CANDIDATE PAIR: " << _pairs.size() << endl;

	// Computing Pair Priority and Ordering Pairs
	//
	// Let G be the priority for the candidate provided by the controlling
	// agent. Let D be the priority for the candidate provided by the
	// controlled agent. The priority for a pair is computed as:
	//
	//   pair priority = 2^32*MIN(G, D) + 2*MAX(G, D) + (G>D?1:0)
	//
	// Where G>D?1:0 is an expression whose value is 1 if G is greater than
	// D, and 0 otherwise. Once the priority is assigned, the agent sorts
	// the candidate pairs in decreasing order of priority. If two pairs
	// have identical priority, the ordering amongst them is arbitrary.
	//
	if (!pair->priority()) {
		UInt32 G = pair->local()->priority();
		UInt32 D = pair->remote()->priority();
		pair->setPriority(2^32*min(G, D) + 2*max(G, D) + (G>D?1:0));
	}

	// 5.7.3. Pruning the Pairs
	//
	// This sorted list of candidate pairs is used to determine a sequence
	// of connectivity checks that will be performed. Each check involves
	// sending a request from a local candidate to a remote candidate.
	// Since an agent cannot send requests directly from a reflexive
	// candidate, but only from its base, the agent next goes through the
	// sorted list of candidate pairs. For each pair where the local
	// candidate is server reflexive, the server reflexive candidate MUST be
	// replaced by its base. 
	//	
	if (pair->local()->type() == Candidate::ServerReflexive) {
		assert(pair->local()->base());
		assert(pair->local()->base()->type() == Candidate::Host);
		Log("debug", this) << "Replacing server reflexive candidate with base:" 
			<< "\n\tCandidate: " << pair->local()->toString()
			<< "\n\tWith: " << pair->local()->base()->toString()
			<< endl;
		pair->setLocalCandidate(pair->local()->base());
	}

	/*
	if (pair->local()->type() == Candidate::ServerReflexive) {
		bool found = false;		
		LocalCandidateList localCandidates = _stream.localCandidates();
		for (LocalCandidateList::const_iterator it = localCandidates.begin(); it != localCandidates.end(); ++it) {
			Log("debug", this) << "PRUNING:" 
				<< "\n\tMatching: " << Candidate::typeToString((*it)->base()->type()) << "," << (*it)->base()->transportIP() << ":" << (*it)->base()->transportPort() 
				<< "\n\tWith: " << Candidate::typeToString(pair->local()->type()) << "," << pair->local()->transportIP() << ":" << pair->local()->transportPort() 
				<< endl;
			if ((*it)->transportAddr() == pair->local()->transportAddr() &&
				(*it)->type() == Candidate::Host) {
				pair->setLocalCandidate(*it);
				found = true;	
				break;
			}		
		}
		if (!found) {
			Log("error", this) << "Unable to replace server reflexive." << endl;
			assert(false);
			delete pair;
			return false;
		}
	}
	*/

	// Once this has been done, the agent MUST prune
	// the list. This is done by removing a pair if its local and remote
	// candidates are identical to the local and remote candidates of a pair
	// higher up on the priority list. The result is a sequence of ordered
	// candidate pairs, called the check list for that media stream.
	//
	int highest = 0;
	for (CandidatePairList::iterator it = _pairs.begin(); it != _pairs.end(); ++it) {
		if (**it == *pair) {
			// If the new pair's priority is higher than the old pair we
			// remove the old one, UNLESS the old pair is in an active state
			// and it is probably in use.
			if (pair->priority() > (*it)->priority() && 
				(*it)->state() != CandidatePair::InProgress &&
				(*it)->state() != CandidatePair::Succeeded) {
				Log("debug", this) << "Adding CANDIDATE PAIR: Priority is greater than old one, removing redundant." << endl;
				if ((*it)->flags().data())
					pair->setFlag((*it)->flags().data());
				//removeCandidatePair(*it);
				_pairs.erase(it);			
				delete *it;
				break;
			} 
			
			// Delete the new pair if it's priority is less or equal to the
			// existing pair.
			else if (pair->priority() <= (*it)->priority()) {
				//assert(false);
				Log("debug", this) << "Adding CANDIDATE PAIR: Priority is less than old one, pruning." << endl;
				if (pair->flags().data())
					(*it)->setFlag(pair->flags().data());
				delete pair;
				return false;
			}

			//else 
			//	++it;
		}
	}

/*
#ifdef DEBUG
	// Ensure we don't have any duplicates
	for (CandidatePairList::const_iterator it = _pairs.begin(); it != _pairs.end(); ++it) {
		//assert (*(*it)->local() != *pair->local());
		assert (*(*it)->remote() != *pair->remote());
	}		
#endif
*/
	_pairs.push_back(pair);
	_pairs.sort(CandidatePair::CompareProiroty); 
	
	// Handle Binding Requests received by the local candidate.
	// NOTE: Binding Responses will be handled frist by the 
	// Candidate Pair, and then passed here.
	// @see onConnectivityCheckSuccess/Failed() 
	pair->local()->STUNMessageReceived += delegate(this, &CheckList::onReceiveSTUNMessage, 100); // normal priority

	Log("debug", this) << "Added CANDIDATE PAIR: " << _pairs.size() << ": " << pair->toString() << endl;
	return true;
}


// Removes a pair from the normal and valid lists.
void CheckList::removeCandidatePair(CandidatePair* pair)
{
	//FastMutex::ScopedLock lock(_mutex); 	
	Log("debug", this) << "Removing CANDIDATE PAIR: " << pair->toString() << endl;

	if (pair->state() == CandidatePair::InProgress ||
		pair->state() == CandidatePair::Waiting ||
		pair->state() == CandidatePair::Succeeded)
		Log("debug") << "Removing an active Candidate Pair is unsafe." << endl;

	// Remove from the normal list.
	for (CandidatePairList::iterator it = _pairs.begin(); it != _pairs.end(); ++it) {
		if (**it == *pair) {
			_pairs.erase(it);
			Log("debug", this) << "Removing CANDIDATE PAIR: " << pair->toString() << "\n: REMOVED" << endl;
			break;
		}
	}	

	// Remove from the valid, nominated and selected lists (if present).
	//removeValidPair(pair);
	//removeNominatedPair(pair);
	//removeSelectedPair(pair);
}


CandidatePairList CheckList::pairs() const 
{ 
	//FastMutex::ScopedLock lock(_mutex); 	
	return _pairs; 
};


CandidatePairList CheckList::pairs(CandidatePair::Flags flag) const 
{	
	//FastMutex::ScopedLock lock(_mutex); 
	CandidatePairList pairs;
	for (CandidatePairList::const_iterator it = _pairs.begin(); it != _pairs.end(); ++it) {			
		if ((*it)->hasFlag(flag)) {	
			pairs.push_back(*it);
		}
	}
	return pairs;
}


CandidatePairList CheckList::pairs(int componentID) const 
{	
	//FastMutex::ScopedLock lock(_mutex); 
	CandidatePairList pairs;
	for (CandidatePairList::const_iterator it = _pairs.begin(); it != _pairs.end(); ++it) {	
		if ((*it)->local()->componentID() == componentID) {	
			pairs.push_back(*it);
		}
	}
	return pairs;
}


/*
CandidatePairList CheckList::validPairs() {	
	//FastMutex::ScopedLock lock(_mutex); 
	CandidatePairList pairs;
	for (CandidatePairList::const_iterator it = _pairs.begin(); it != _pairs.end(); ++it) {			
		if ((*it)->isValid()) {	
			pairs.push_back(*it);
		}
	}
	return pairs;
}


CandidatePairList CheckList::nominatedPairs() {	
	//FastMutex::ScopedLock lock(_mutex); 
	CandidatePairList pairs;
	for (CandidatePairList::const_iterator it = _pairs.begin(); it != _pairs.end(); ++it) {			
		if ((*it)->isNominated()) {	
			pairs.push_back(*it);
		}
	}
	return pairs;
}
*/


CandidatePair* CheckList::getCandidatePair(const Candidate* local, const Candidate* remote) 
{
	//FastMutex::ScopedLock lock(_mutex); 	
	for (CandidatePairList::const_iterator it = _pairs.begin(); it != _pairs.end(); ++it) {
		if (*(*it)->local() == *local &&
			*(*it)->remote() == *remote) {
			return *it;
		}
	}
	return NULL;
}


CandidatePair* CheckList::getCandidatePair(const Net::Address& local, const Net::Address& remote) 
{
	//FastMutex::ScopedLock lock(_mutex); 	
	for (CandidatePairList::const_iterator it = _pairs.begin(); it != _pairs.end(); ++it) {
		if ((*it)->local()->transportAddr().toString() == local.toString() &&
			(*it)->remote()->transportAddr().toString() == remote.toString()) {
			return *it;
		}
	}
	return NULL;
}



CandidatePair* CheckList::getNextCandidatePair(CandidatePair::State state) 
{
	//FastMutex::ScopedLock lock(_mutex); 	
	int highest = 0;
	CandidatePair* pair = NULL;
	for (CandidatePairList::const_iterator it = _pairs.begin(); it != _pairs.end(); ++it) {
		if ((*it)->state() == state &&
			highest < (*it)->priority()) {
			highest = (*it)->priority();
			pair = *it;
		}
	}	
	return pair;
}


/*
void CheckList::addValidPair(CandidatePair* pair) {	
	//FastMutex::ScopedLock lock(_mutex); 	

	if (!pair->isValid())
		pair->setValid();

	_validPairs.push_back(pair);
	_validPairs.sort(CandidatePair::CompareProiroty); 
	Log("debug", this) << "Added VALID PAIR: " << _validPairs.size() << endl;
	
	// If there is a pair in the valid list for every component of this
	// media stream (where this is the actual number of components being
	// used, in cases where the number of components signaled in the SDP
	// differs from offerer to answerer), the success of this check may
	// unfreeze checks for other media streams.
	if (hasValidPairsAllComponents()) {
		agent().onMediaStreamValidPairsCreated(_stream, _validPairs);
	}
}


// Removes a pair from the selected list.
bool CheckList::removeValidPair(CandidatePair* pair) {
	if (pair->state() == CandidatePair::InProgress ||
		pair->state() == CandidatePair::Waiting ||
		pair->state() == CandidatePair::Succeeded)
		Log("debug") << "Removing an active valid candidate pair is unsafe." << endl;

	// Remove from the valid list.
	for (CandidatePairList::const_iterator it = _validPairs.begin(); it != _validPairs.end(); ++it) {
		if (**it == *pair) {
			_validPairs.erase(it);
			return true;
			break;
		}
	}	
	return false;
}
*/


CandidatePair* CheckList::getValidPair(const Candidate* local, const Candidate* remote) const 
{
	//FastMutex::ScopedLock lock(_mutex); 	
	for (CandidatePairList::const_iterator it = _pairs.begin(); it != _pairs.end(); ++it) {
		if ((*it)->isValid() &&
			*(*it)->local() == *local &&
			*(*it)->remote() == *remote) 
			return *it;
	}
	return NULL;
}


bool CheckList::hasValidPairsAllComponents() const 
{	
	//FastMutex::ScopedLock lock(_mutex); 	
	bool hasValidPairs = false;
	for (CandidatePairList::const_iterator it = _pairs.begin(); it != _pairs.end(); ++it) {			
		if ((*it)->isValid()) {	
			ComponentMap components = _stream.components();
			ComponentMap::const_iterator it1 = components.find((*it)->local()->componentID());
			if (it1 != components.end()) {
				hasValidPairs = true;
			} else {
				hasValidPairs = false;
				break;
			}

		}
	}
	return hasValidPairs;
}


/*
void CheckList::addNominatedPair(CandidatePair* pair) {
	//FastMutex::ScopedLock lock(_mutex); 	

	if (!pair->isNominated())
		pair->setNominated();

	_nominatedPairs.push_back(pair);
	_nominatedPairs.sort(CandidatePair::CompareProiroty); 
	Log("debug", this) << "Added NOMINATED PAIR: " << _nominatedPairs.size() << endl;
}


// Removes a pair from the selected list.
bool CheckList::removeNominatedPair(CandidatePair* pair) {
	if (pair->state() == CandidatePair::InProgress ||
		pair->state() == CandidatePair::Waiting ||
		pair->state() == CandidatePair::Succeeded)
		Log("debug") << "Removing an active nominated candidate pair is unsafe." << endl;

	// Remove from the nominated list.
	for (CandidatePairList::const_iterator it = _nominatedPairs.begin(); it != _nominatedPairs.end(); ++it) {
		if (**it == *pair) {
			_nominatedPairs.erase(it);
			return true;
			break;
		}
	}	
	return false;
}
*/


bool CheckList::hasNominatedPairs() const 
{
	for (CandidatePairList::const_iterator it = _pairs.begin(); it != _pairs.end(); ++it) {	
		if ((*it)->isNominated()) {
			return true;
		}
	}
	return false;
}


//bool CheckList::removeWaitingPairsMatchingNominatedPairs() {
//}


bool CheckList::hasNominatedPairsAllComponents() const 
{	
	//FastMutex::ScopedLock lock(_mutex); 	
	bool hasNominatedPairs = false;
	for (CandidatePairList::const_iterator it = _pairs.begin(); it != _pairs.end(); ++it) {	
		if ((*it)->isNominated()) {
			ComponentMap components = _stream.components();
			ComponentMap::const_iterator it1 = components.find((*it)->local()->componentID());
			if (it1 != components.end()) {
				hasNominatedPairs = true;
			} else {
				hasNominatedPairs = false;
				break;
			}
		}
	}
	return hasNominatedPairs;
}


bool CheckList::isNominatedPairsMatchDefaultPairs() const 
{	
	//FastMutex::ScopedLock lock(_mutex); 	

	Log("debug", this) << "Matching Nominated Pair with Default Candidates" << endl;
	//assert(_nominatedPairs.size());

	int matches = 0;
	ComponentMap components = _stream.components();
	assert(components.size());
	for (ComponentMap::const_iterator it = components.begin(); it != components.end(); ++it) {
		Component* component = it->second;

		for (CandidatePairList::const_iterator it1 = _pairs.begin(); it1 != _pairs.end(); ++it1) {	
			if ((*it1)->isNominated()) {
				//Log("debug", this) << "Matching Nominated Pair with Default Candidates:\n" 
				//	<< "\t(*it1)->local()->componentID(): " << (*it1)->local()->componentID() << "\n"
				//	<< "\tid: " << id << "\n"
				//	<< endl;
				if ((*it1)->local()->componentID() == component->id()) {
					CandidatePair* nominatedPair = *it1;
					assert(component->defaultLocalCandidate());
					assert(component->defaultRemoteCandidate());

					Log("debug", this) << "Component " << component->id() << " Matching Nominated Pair with Default Candidates:" 
						<< "\n\tNominatedPair: " << nominatedPair->toString()
						<< "\n\tComponentLocalDefault: " << component->defaultLocalCandidate()->toString()
						<< "\n\tComponentRemoteDefault: " << component->defaultRemoteCandidate()->toString()
						<< endl;
					if (*nominatedPair->local() == *component->defaultLocalCandidate() &&
						*nominatedPair->remote() == *component->defaultRemoteCandidate()) {
						Log("debug", this) << "Matching Nominated Pair with Default Candidates: Is Match" << endl;
						matches++;
						break;		
					}
				}
			}
		}
	}

	Log("debug", this) << "Matching Nominated Pair with Default Candidates: OK" << endl;
	return matches >= components.size();
}


// An updated pair represents a candidate pair generated by a peer offer
// including the remote-candidates attribute.
void CheckList::addUpdatedPair(CandidatePair* pair) 
{	
	//FastMutex::ScopedLock lock(_mutex); 	

	// Only add the updated pair if they don't exist on our valid list.
	//if (!getValidPair(pair->local(), pair->remote())) {
	_updatedPairs.push_back(pair);
	_updatedPairs.sort(CandidatePair::CompareProiroty); 
	Log("debug", this) << "Added UPDATED PAIR: " << pair->toString() << endl;
	//} else
	//	delete pair;
}


void CheckList::checkUpdatedPairs() 
{
	Log("debug", this) << "Checking updated pairs: " << _updatedPairs.size() << endl;

	// The agent then sees if each of these candidate pairs is present in
	// the valid list. If a particular pair is not in the valid list, the
	// check num "lost" the race.  Call such a pair a "losing pair".
	//
	CandidatePairList loosingPairs;
	for (CandidatePairList::const_iterator it = _updatedPairs.begin(); it != _updatedPairs.end(); ++it) {	
		if (!getValidPair((*it)->local(), (*it)->remote())) {
			loosingPairs.push_back(*it);	
		}
	}

	// The agent finds all the pairs in the check list whose remote
	// candidates equal the remote candidate in the losing pair:
	//
	if (loosingPairs.size()) {
		int numInProgress = 0;
		int numFailed = 0;
		for (CandidatePairList::const_iterator it = _updatedPairs.begin(); it != _updatedPairs.end(); ++it) {	
			for (CandidatePairList::const_iterator it1 = _pairs.begin(); it1 != _pairs.end(); ++it1) {	
				if (*(*it)->remote() == *(*it1)->remote()) {
					// NOTE: Waiting state is included in this condition
					// even though it is not stated in the spec.
					if ((*it1)->state() == CandidatePair::Waiting ||
						(*it1)->state() == CandidatePair::InProgress) {
						(*it1)->setFlag(CandidatePair::UpdateLoosingOnResponse);
						numInProgress++;
					} if ((*it1)->state() == CandidatePair::Failed)
						numFailed++;
				}
			}
		}

		// o  If none of the pairs are In-Progress, and at least one is Failed,
		//    it is most likely that a network failure, such as a network
		//    partition or serious packet loss, num occurred.  The agent SHOULD
		//    generate an answer for this media stream as if the remote-
		//    candidates attribute had not been present, and then restart ICE
		//    for this stream.
		//
		if (!numInProgress && numFailed) {
			// TODO: Restart ICE and send check.
			assert(false);
			setState(CheckList::Failed);
		}

		// o  If at least one of the pairs is In-Progress, the agent SHOULD wait
		//    for those checks to complete, and as each completes, redo the
		//    processing in this section until there are no losing pairs.
		// 
		else if (numInProgress) {
			// UpdateLoosingOnResponse flag set above.
			return;
		}	

		assert(false);
	}

	// Once there are no losing pairs, the agent can generate the answer.
	// It MUST set the default destination for media to the candidates in
	// the remote-candidates attribute from the offer (each of which will
	// now be the local candidate of a candidate pair in the valid list).
	// It MUST include a candidate attribute in the answer for each
	// candidate in the remote-candidates attribute in the offer.
	//
	else {
		// TODO: Ensure the remote-candidate is in our pairs list.
		for (CandidatePairList::const_iterator it = _updatedPairs.begin(); it != _updatedPairs.end(); ++it) {	
			for (CandidatePairList::const_iterator it1 = _pairs.begin(); it1 != _pairs.end(); ++it1) {	
				if (*(*it)->local() == *(*it1)->local()) {
					Component* component = _stream.getComponent((*it1)->local()->componentID());
					assert(component);
					if (component) {
						component->setDefaultLocalCandidate((*it1)->local());
						component->setDefaultRemoteCandidate((*it1)->remote());
					}
				}
			}
		}

		// Clear updated pairs list. This will clear candidate pointers
		// too since UpdateLoosingOnResponse has been set.
		Util::ClearList(_updatedPairs);
		
		Log("debug", this) << "Checking Updated Pairs: Subsequent Checking Complete" << endl;		
		_stream.agent().onMediaStreamSubsequentCheckingComplete(&_stream);
	}
}


bool CheckList::doUpdateStates() 
{
	Log("debug", this) << "Updating States" << endl;	
	
	// For both controlling and controlled agents, the state of ICE
	// processing depends on the presence of nominated candidate pairs in
	// the valid list and on the state of the check list. Note that, at any
	// time, more than one of the following cases can apply:
	//
	// o  If there are no nominated pairs in the valid list for a media
	//    stream and the state of the check list is Running, ICE processing
	//    continues.
	//
	if (!hasNominatedPairs()) {
		Log("debug", this) << "Updating States: No nominated pairs." << endl;
		return false;
	}

	// o  If there is at least one nominated pair in the valid list for a
	//    media stream and the state of the check list is Running:
	//		
	//    *  The agent MUST remove all Waiting and Frozen pairs in the check
	//       list and triggered check queue for the same component as the
	//       nominated pairs for that media stream.
	//		
	//    *  If an In-Progress pair in the check list is for the same
	//       component as a nominated pair, the agent SHOULD cease
	//       retransmissions for its check if its pair priority is lower
	//       than the lowest-priority nominated pair for that component.
	//		 (NOT IMPLIMENTED)
	//
	else if (state() == CheckList::Running) {
		Log("debug", this) << "Updating States: Removing matching pairs." << endl;
		for (CandidatePairList::const_iterator it = _pairs.begin(); it != _pairs.end(); ++it) {
			if ((*it)->isNominated()) {
				for (TirggeredCheckQueue::const_iterator ait = _queue.begin(); ait != _queue.end();) {
					if (((*ait)->local()->componentID() == (*it)->local()->componentID()) &&
						((*ait)->state() == CandidatePair::Frozen ||
						 (*ait)->state() == CandidatePair::Waiting)) {
						//delete *ait;
						//queue().erase(ait);
						removeTriggeredPair(*it);
					}
					else
						++ait;
				}
			}
		}
	}

	// o  Once there is at least one nominated pair in the valid list for
	//    every component of at least one media stream and the state of the
	//    check list is Running:
	//		
	//    *  The agent MUST change the state of processing for its check
	//       list for that media stream to Completed.
	//		
	//    *  The agent MUST continue to respond to any checks it may still
	//       receive for that media stream, and MUST perform triggered
	//       checks if required by the processing of Section 7.2.
	//		
	//    *  The agent MUST continue retransmitting any In-Progress checks
	//       for that check list.
	//		
	//    *  The agent MAY begin transmitting media for this media stream as
	//       described in Section 11.1.
	//
	return _stream.tryTransitionState(ICE::State::Success);	
}


// ---------------------------------------------------------------------
//
// Events
//
// ---------------------------------------------------------------------
void CheckList::onReceiveSTUNMessage(void* sender, const STUN::Message& message, const Net::Address& peerAddress)
{
	Log("debug", this) << "Received: " << message.toString() << endl;
	LocalCandidate* local = reinterpret_cast<LocalCandidate*>(sender);	
	onConnectivityCheckRequest(local, message, peerAddress);
}


void CheckList::onConnectivityCheckRequest(LocalCandidate* local, const STUN::Message& message, 
											//const Net::Address& localAddress, 
											const Net::Address& peerAddress) 
{	
	//FastMutex::ScopedLock lock(_mutex);

	//Log("debug", this) << "Received Connectivity Check Request" << endl;
	
	Log("debug", this) << "Received Connectivity Check Request on:\n\t" << local->toSDP() << endl;
	assert(message.type() == STUN::Message::Binding);


	// After the agent has done the basic processing of a message, the agent
	// performs the checks listed below in order specified:

	// o  If the message does not contain both a MESSAGE-INTEGRITY and a
	//    USERNAME attribute:

	//    *  If the message is a request, the server MUST reject the request
	//       with an error message. This response MUST use an error code
	//       of 400 (Bad Request).

	//    *  If the message is an indication, the agent MUST silently
	//       discard the indication.	
	//
	const STUN::Username* usernameAttr = message.get<STUN::Username>();
	const STUN::MessageIntegrity* integrityAttr = message.get<STUN::MessageIntegrity>();
	if (!usernameAttr || !integrityAttr) {	
		Log("debug", this) << "No USERNAME || MESSAGE-INTEGRITY" << endl;			
		sendError(local, message, peerAddress, 400, "Bad Request");
		assert(false);
		return;
	}

	// o  If the USERNAME does not contain a username value currently valid
	//    within the server:
	//
	//    *  If the message is a request, the server MUST reject the request
	//       with an error message. This response MUST use an error code
	//       of 401 (Unauthorized).
	//
	//    *  If the message is an indication, the agent MUST silently
	//       discard the indication.
	//
	string ufrag = usernameAttr->asString();
	string ourUFrag;
	string peerUFrag;			
	string::size_type index = ufrag.find(":");
	if (index != string::npos) {
		ourUFrag = ufrag.substr(0, index);
		peerUFrag = ufrag.substr(index+1, ufrag.length());
	}
	assert(!ourUFrag.empty());
	assert(!peerUFrag.empty());
	Log("debug", this) << "Authenticating Binding Request: " << peerUFrag << endl;	
	const IUser* user = _stream.agent().observer().authenticateBindingRequest(_stream.agent(), peerUFrag);
	//const IUser* user = _stream.agent().authenticator()->getUser(message);
	if (!user) {			
		Log("debug", this) << "Unable to Authenticate User" << endl;	
		sendError(local, message, peerAddress, 401, "Unauthorized");
		return;
	}

	// o  Using the password associated with the username, compute the value
	//    for the message integrity as described in Section 15.4. If the
	//    resulting value does not match the contents of the MESSAGE-
	//    INTEGRITY attribute:

	//    *  If the message is a request, the server MUST reject the request
	//       with an error message. This response MUST use an error code
	//       of 401 (Unauthorized).

	//    *  If the message is an indication, the agent MUST silently
	//       discard the indication.
		
	if (!integrityAttr->verifyHmac(user->password())) {	
		Log("debug", this) << "Unable to Verify HMAC with Key: " << user->password() << endl;	
		sendError(local, message, peerAddress, 401, "Unauthorized");
		return;
	}


	/*
	// Match the incoming request with it's remote candidate,
	// or silently disgard it.
	LocalCandidate* local = NULL;
	LocalCandidateList localCandidates = _stream.localCandidates();
	for (LocalCandidateList::const_iterator it = localCandidates.begin(); it != localCandidates.end(); ++it) {	
		if ((*it)->transportAddr().toString() == localAddress.toString() && 
			((*it)->type() == Candidate::Host ||
			 (*it)->type() == Candidate::Relayed)) {
			local = (*it);
		}
	}
	if (!local) {
		Log("error", this) << "No Local Candidate for BINDING-REQUEST" << endl;
		assert(false);
		return;
	}
	*/

	const STUN::ICEPriority* priorityAttr = message.get<STUN::ICEPriority>();
	if (!priorityAttr) {
		Log("warn", this) << "Binding request has no PRIORITY attribute. Discarding probable cancelled transaction." << endl;
		return;
	}

	// 7.2.1.3. Learning Peer Reflexive Candidates
	// 
	// If the source transport address of the request does not match any
	// existing remote candidates, it represents a new peer reflexive remote
	// candidate. This candidate is constructed as follows:
	// 
	// o  The priority of the candidate is set to the PRIORITY attribute
	//    from the request.
	// 
	// o  The type of the candidate is set to peer reflexive.
	// 
	// o  The foundation of the candidate is set to an arbitrary value,
	//    different from the foundation for all other remote candidates. If
	//    any subsequent offer/answer exchanges contain this peer reflexive
	//    candidate in the SDP, it will signal the actual foundation for the
	//    candidate.
	// 
	// o  The component ID of this candidate is set to the component ID for
	//    the local candidate to which the request was sent.
	// 
	// This candidate is added to the list of remote candidates. However,
	// the agent does not pair this candidate with any local candidates.
	//
	RemoteCandidate* remote = _stream.getRemoteCandidateByAddress(peerAddress);
	if (!remote) {
		remote = new RemoteCandidate;
		remote->setType(Candidate::PeerReflexive);
		remote->setFoundation(CryptoProvider::generateRandomKey(8));	
		remote->setTransport(Candidate::Udp);			
		//remote->setPriority(Candidate::computePriority(Candidate::PeerReflexive, component->id()));
		remote->setTransportAddr(peerAddress);
		//remote->setBaseAddr(peerAddress);
		remote->setComponentID(local->component().id());
		remote->setPriority(priorityAttr->value());
		_stream.tryAddRemoteCandidate(remote);
	}
	assert(remote);


	// 7.2.1.4. Triggered Checks
	//
	// Next, the agent constructs a pair whose local candidate is equal to
	// the transport address on which the STUN request was received, and a
	// remote candidate equal to the source transport address where the
	// request came from (which may be the peer reflexive remote candidate
	// that was just learned). The local candidate will either be a host
	// candidate (for cases where the request was not received through a
	// relay) or a relayed candidate (for cases where it is received through
	// a relay). The local candidate can never be a server reflexive
	// candidate. Since both candidates are known to the agent, it can
	// obtain their priorities and compute the candidate pair priority.
	// This pair is then looked up in the check list. There can be one of
	// several outcomes:
	//
	CandidatePair* pair = getCandidatePair(local, remote);
	CandidatePair::State state = CandidatePair::Frozen;
	if (pair) {	
		Log("debug", this) << "Connectivity Check Request: Obtained Existing Pair:\n" 
			<< pair->toString() << endl;

		state = pair->state();
		switch (state) {

		// o  If the pair is already on the check list:
		//
		//    *  If the state of that pair is Waiting or Frozen, a check for
		//       that pair is enqueued into the triggered check queue if not
		//       already present.
		//
		case CandidatePair::Waiting:
		case CandidatePair::Frozen:
			pushTriggeredPair(pair);
			break;

		//    *  If the state of that pair is In-Progress, the agent cancels the
		//       in-progress transaction. Cancellation means that the agent
		//       will not retransmit the request, will not treat the lack of
		//       response to be a failure, but will wait the duration of the
		//       transaction timeout for a response. In addition, the agent
		//       MUST create a new connectivity check for that pair
		//       (representing a new STUN Binding request transaction) by
		//       enqueueing the pair in the triggered check queue. The state of
		//       the pair is then changed to Waiting.
		// 
		case CandidatePair::InProgress:
			pair->cancelConnectivityCheck();
			pair->setState(CandidatePair::Waiting);
			pushTriggeredPair(pair);					
			break;

		//    *  If the state of the pair is Failed, it is changed to Waiting
		//       and the agent MUST create a new connectivity check for that
		//       pair (representing a new STUN Binding request transaction), by
		//       enqueueing the pair in the triggered check queue.
		//
		case CandidatePair::Failed:	
			pair->setState(CandidatePair::Waiting);
			pushTriggeredPair(pair);		
			break;

		//    *  If the state of that pair is Succeeded, nothing further is
		//       done.
		//
		case CandidatePair::Succeeded:	
			Log("debug", this) << "Connectivity Check Request: Discarding request from Succeeded pair." << endl;
			//return;
			break;
		}
	} 
	else {
		Log("debug", this) << "Connectivity Check Request: Creating New Pair:\n" << pair->toString() << endl;

		// o  If the pair is not already on the check list:
		//
		//    *  The pair is inserted into the check list based on its priority.
		//
		//    *  Its state is set to Waiting.
		//
		//    *  The pair is enqueued into the triggered check queue.
		//
		pair = new CandidatePair(*this, local, remote);
		pair->setState(CandidatePair::Waiting);
		tryAddCandidatePair(pair);			
		pushTriggeredPair(pair);
		state = pair->state();

		// When a triggered check is to be sent, it is constructed and processed
		// as described in Section 7.1.2. These procedures require the agent to
		// know the transport address, username fragment, and password for the
		// peer. The username fragment for the remote candidate is equal to the
		// part after the colon of the USERNAME in the Binding request that was
		// just received. Using that username fragment, the agent can check the
		// SDP messages received from its peer (there may be more than one in 
		// cases of forking), and find this username fragment. The corresponding
		// password is then selected.
	}

	// 7.1.3.2.4. Updating the Nominated Flag
	//
	// If the Binding request received by the agent had the USE-CANDIDATE
	// attribute set, and the agent is in the controlled role, the agent
	// looks at the state of the pair computed in Section 7.2.1.4:
	//
	const STUN::ICEUseCandidate* candidateAttr = message.get<STUN::ICEUseCandidate>();
	if (candidateAttr && !_stream.agent().isControlling()) {
		Log("debug", this) << "Updating the Nominated Flag:\n" << pair->toString() << endl;
		assert(pair);
		switch (state) {

		// o  If the state of this pair is Succeeded, it means that the check
		//    generated by this pair produced a successful response. This would
		//    have caused the agent to construct a valid pair when that success
		//    response was received (see Section 7.1.3.2.2). The agent now sets
		//    the nominated flag in the valid pair to true. This may end ICE
		//    processing for this media stream; see Section 8.
		//
		case CandidatePair::Succeeded:		
			Log("debug", this) << "Updating the Nominated Flag: Succeeded:\n" << pair->toString() << endl;
			//assert(!pair->isNominated());
			if (!pair->isNominated()) {
				pair->setNominated();
				doUpdateStates();
			}
			break;

		// o  If the state of this pair is In-Progress, if its check produces a
		//    successful result, the resulting valid pair has its nominated flag
		//    set when the response arrives. This may end ICE processing for
		//    this media stream when it arrives; see Section 8.
		//

		// NOTE: The above text is extremely confusing! Section 8.2.1. states
		// that a successful response to a binding request with the USE-CANDIDATE
		// will cause the candidate to be Nominated, so a specific In-Progress
		// state here should not be required.
		case CandidatePair::Waiting:
		case CandidatePair::InProgress:
			Log("debug", this) << "Updating the Nominated Flag: InProgress:\n" << pair->toString() << endl;
			pair->setFlag(CandidatePair::NominateOnSuccess);
			break;
		}
	}

	pair->sendConnectivityCheckResponse(message, peerAddress); //localAddress, 
}


/*
void CheckList::onReceivedConnectivityCheckSuccess(const void* sender, STUN::MessageIPacket& response) {
	//FastMutex::ScopedLock lock(_mutex);

	//Component* component = const_cast<Component*>(reinterpret_cast<const Component*>(sender));

}
*/


void CheckList::onConnectivityCheckSuccess(CandidatePair* pair, STUN::Message& response) 
{	
	Log("debug") << pair->toString() << "\n: ReceivedConnectivityCheckSuccess" << endl;

	/*
	// If the pair has the DeleteOnResponse flag set we delete it here.
	if (pair->hasFlag(CandidatePair::DeleteOnResponse)) {
		Log("debug") << pair->toString() << "\n: DELETING CandidatePair with DeleteOnResponse flag" << endl;
		removeCandidatePair(pair);
		delete pair;
		return;
	} 

	// If the pair has the CurrentRequestCancalled flag set we ignore
	// the packet.message.
	if (pair->hasFlag(CandidatePair::CurrentRequestCancalled)) {
		Log("debug") << pair->toString() << "\n: IGNORING CandidatePair STUN response with CurrentRequestCancalled flag" << endl;
		pair->removeFlag(CandidatePair::CurrentRequestCancalled);
		return;
	} 
	*/

	// NOTE: No STUN backwards compatability for connectivity checks.
	const STUN::XorMappedAddress* addressAttr = response.get<STUN::XorMappedAddress>();
	if (!addressAttr || addressAttr->family() != 1) {
		Log("error") << pair->toString() << "\n: STUN Binding Response has no Mapped Address" << endl;
		assert(false);
		return;
	}
	Net::Address mappedAddress = addressAttr->address();

	// 7.1.3.2.1. Discovering Peer Reflexive Candidates
	//
	// The agent checks the mapped address from the STUN message. If the
	// transport address does not match any of the local candidates that the
	// agent knows about, the mapped address represents a new candidate -- a
	// peer reflexive candidate. Like other candidates, it has a type, 
	// base, priority, and foundation. They are computed as follows:
	//
	// o  Its type is equal to peer reflexive.
	//
	// o  Its base is set equal to the local candidate of the candidate pair
	//    from which the STUN check was sent.
	//
	// o  Its priority is set equal to the value of the PRIORITY attribute
	//    in the Binding request.
	//
	// o  Its foundation is selected as described in Section 4.1.1.3.
	//
	// This peer reflexive candidate is then added to the list of local
	// candidates for the media stream. Its username fragment and password
	// are the same as all other local candidates for that media stream.
	//
	// However, the peer reflexive candidate is not paired with other remote
	// candidates. This is not necessary; a valid pair will be generated
	// from it momentarily based on the procedures in Section 7.1.3.2.2. If
	// an agent wishes to pair the peer reflexive candidate with other
	// remote candidates besides the one in the valid pair that will be
	// generated, the agent MAY generate an updated offer which includes the
	// peer reflexive candidate. This will cause it to be paired with all
	// other remote candidates.
	//
	LocalCandidate* local = pair->local();
	if (_stream.getLocalCandidateByAddress(mappedAddress) == NULL) {
		PeerReflexiveCandidate* prflx = new PeerReflexiveCandidate(pair->local()->component());
		prflx->setTransport(Candidate::Udp);			
		prflx->setTransportAddr(mappedAddress);
		prflx->setBase(pair->local());
		prflx->setPriority(LocalCandidate::computePriority(Candidate::PeerReflexive, pair->local()->componentID())); // Calculated in the same way as the request
		prflx->computeFoundation();

		// If the new peer reflexive candidate is accepted we use it to
		// create the valid pair, otherwise use the Candiate Pair that 
		// initiated the Binding request.
		//
		Log("debug") << "******************** ABOUT TO REPLACE" << endl;
		if (_stream.tryAddLocalCandidate(prflx)) {
			local = prflx;
			//pair->setLocalCandidate(local);
		}
		Log("debug") << "******************** REPLACEDDDDDDDDDDDDDD" << endl;
	}
		
	// 7.1.3.2.2. Constructing a Valid Pair
	//
	// The agent constructs a candidate pair whose local candidate equals
	// the mapped address of the response, and whose remote candidate equals
	// the destination address to which the request was sent. This is
	// called a valid pair, since it has been validated by a STUN
	// connectivity check. The valid pair may equal the pair that generated
	// the check, may equal a different pair in the check list, or may be a
	// pair not currently on any check list. If the pair equals the pair
	// that generated the check or is on a check list currently, it is also
	// added to the VALID LIST, which is maintained by the agent for each
	// media stream. This list is empty at the start of ICE processing, and
	// fills as checks are performed, resulting in valid candidate pairs.
	//
	// It will be very common that the pair will not be on any check list.
	// Recall that the check list has pairs whose local candidates are never
	// server reflexive; those pairs had their local candidates converted to
	// the base of the server reflexive candidates, and then pruned if they
	// were redundant. When the response to the STUN check arrives, the
	// mapped address will be reflexive if there is a NAT between the two.
	// In that case, the valid pair will have a local candidate that doesn't
	// match any of the pairs in the check list.
	//
	CandidatePair* validPair = NULL;

	// Check if the pair which initiated the binging request can become valid.
	if (pair->local()->transportAddr().toString() == mappedAddress.toString()) {
		Log("debug") << pair->toString() << "\nVALID pair is INTITATOR" << endl;
		validPair = pair;
		validPair->setValid();
		assert(validPair->isValid());
	}

	// Otherwise try to find it in our candidate pair list.
	else if (validPair = getCandidatePair(mappedAddress, pair->remote()->transportAddr())) {
		assert(validPair->isValid());
		Log("debug") << pair->toString() << "\nVALID pair exists in LIST" << endl;
	}

	// Otherwise create a new object to be our valid pair.
	else {				
		// If the pair is not on any check list, the agent computes the priority
		// for the pair based on the priority of each candidate, using the
		// algorithm in Section 5.7. The priority of the local candidate
		// depends on its type. If it is not peer reflexive, it is equal to the
		// priority signaled for that candidate in the SDP. If it is peer
		// reflexive, it is equal to the PRIORITY attribute the agent placed in
		// the Binding request that just completed. The priority of the remote
		// candidate is taken from the SDP of the peer. If the candidate does
		// not appear there, then the check must have been a triggered check to
		// a new remote candidate. In that case, the priority is taken as the
		// value of the PRIORITY attribute in the Binding request that triggered
		// the check that just completed. The pair is then added to the VALID
		// LIST.		
		Log("debug") << validPair->toString() << "\nVALID pair doesn't exist in LIST, creating a new one." << endl;
		validPair = new CandidatePair(*this, local, pair->remote());
		validPair->setValid();
		bool success = tryAddCandidatePair(validPair);
		assert(success);
	}

	// 7.1.3.2.3.  Updating Pair States
	//
	// The agent sets the state of the pair that *generated* the check to
	// Succeeded.  Note that, the pair which *generated* the check may be
	// different than the valid pair constructed in Section 7.1.3.2.2 as a
	// consequence of the response.  The success of this check might also
	// cause the state of other checks to change as well.  The agent MUST
	// perform the following two steps:
	//	
	pair->setState(CandidatePair::Succeeded);

	// 1.  The agent changes the states for all other Frozen pairs for the
	//     same media stream and same foundation to Waiting.  Typically, but
	//     not always, these other pairs will have different component IDs.
	//
	for (CandidatePairList::const_iterator it = _pairs.begin(); it != _pairs.end(); ++it) {	
		if ((**it != *pair) &&
			((*it)->foundation() == pair->foundation())) {		
			 (*it)->setState(CandidatePair::Waiting);
		}
	}

	// 2.  If there is a pair in the valid list for every component of this
	//     media stream (where this is the actual number of components being
	//     used, in cases where the number of components signaled in the SDP
	//     differs from offerer to answerer), the success of this check may
	//     unfreeze checks for other media streams.  Note that this step is
	//     followed not just the first time the valid list under
	//     consideration has a pair for every component, but every
	//     subsequent time a check succeeds and adds yet another pair to
	//     that valid list.  The agent examines the check list for each
	//     other media stream in turn:
	//
	if (hasValidPairsAllComponents()) {
		CandidatePairList validPairs = pairs(CandidatePair::Valid);
		_stream.agent().onMediaStreamValidPairsCreated(&_stream, validPairs);
	}

	// 7.1.3.2.4.  Updating the Nominated Flag
	//
	switch (_stream.agent().role()) {

	// If the agent was a controlling agent, and it had included a USE-
	// CANDIDATE attribute in the Binding request, the valid pair generated
	// from that check has its nominated flag set to true. This flag
	// indicates that this valid pair should be used for media if it is the
	// highest-priority one amongst those whose nominated flag is set. This
	// may conclude ICE processing for this media stream or all media
	// streams; see Section 8.
	//
	case Agent::Controlling:
		assert(!validPair->isNominated());
		//_stream.addNominatedPair(validPair);
		//validPair->setFlag(CandidatePair::Nominated);
		validPair->setNominated();
		//tryAddCandidatePair(validPair);
		doUpdateStates();
		break;

	// If the agent is the controlled agent, the response may be the result
	// of a triggered check that was sent in response to a request that
	// itself had the USE-CANDIDATE attribute. This case is described in
	// Section 7.2.1.5, and may now result in setting the nominated flag for
	// the pair learned from the original request.
	//
	case Agent::Controlled:
		if (pair->hasFlag(CandidatePair::NominateOnSuccess)) {		
			//_stream.addNominatedPair(this);
			//setFlag(CandidatePair::Nominated);
			pair->setNominated();
			//tryAddCandidatePair(pair);
			doUpdateStates();
		}
		
		// Re-check this pair against the loosing pairs generated from the
		// remote-candidates updated offer.
		if (pair->hasFlag(CandidatePair::UpdateLoosingOnResponse)) {		
			checkUpdatedPairs();
		}				
		break;
	}

	if (pair->hasFlag(CandidatePair::UpdateLoosingOnResponse)) {
		//assert(false);
		checkUpdatedPairs();
	}
}


void CheckList::onConnectivityCheckFailed(CandidatePair* pair, STUN::Message& response) 
{	
	// Failure Cases
	// If the STUN transaction generates a 487 (Role Conflict) error
	// response, the agent checks whether it included the ICE-CONTROLLED or
	// ICE-CONTROLLING attribute in the Binding request. If the request
	// contained the ICE-CONTROLLED attribute, the agent MUST switch to the
	// controlling role if it has not already done so. If the request
	// contained the ICE-CONTROLLING attribute, the agent MUST switch to the
	// controlled role if it has not already done so. Once it has switched, 
	// the agent MUST enstream the candidate pair whose check generated the
	// 487 into the triggered check stream. The state of that pair is set to
	// Waiting. When the triggered check is sent, it will contain an ICE-
	// CONTROLLING or ICE-CONTROLLED attribute reflecting its new role.
	// Note, however, that the tie-breaker value MUST NOT be reselected.

	// A change in roles will require an agent to recompute pair priorities
	// (Section 5.7.2), since those priorities are a function of controlling
	// and controlled roles. The change in role will also impact whether
	// the agent is responsible for selecting nominated pairs and generating
	// updated offers upon conclusion of ICE.

	// Agents MAY support receipt of ICMP errors for connectionectivity checks.
	// If the STUN transaction generates an ICMP error, the agent sets the
	// state of the pair to Failed. If the STUN transaction generates a
	// STUN error response that is unrecoverable (as defined in [RFC5389])
	// or times out, the agent sets the state of the pair to Failed.

	// The agent MUST check that the source IP address and port of the
	// response equal the destination IP address and port to which the
	// Binding request was sent, and that the destination IP address and
	// port of the response request the source IP address and port from which
	// the Binding request was sent. In other words, the source and
	// destination transport addresses in the request and responses are
	// symmetric. If they are not symmetric, the agent sets the state of
	// the pair to Failed.

	pair->setState(CandidatePair::Failed);

	if (pair->hasFlag(CandidatePair::UpdateLoosingOnResponse))
		checkUpdatedPairs();
}



// When annother media stream check list aquires it's valid pairs,
// they sent to all other media streams to re-calculate states.
void CheckList::onAdjacentMediaStreamValidPairsCreated(CandidatePairList& pairs) 
{
	Log("debug", this) << "Adjacent Media Stream Valid Pairs Created" << endl;
	
	//FastMutex::ScopedLock lock(_mutex); 	

	CandidatePairList matches;
	for (CandidatePairList::const_iterator it = pairs.begin(); it != pairs.end(); ++it) {
		for (CandidatePairList::const_iterator it1 = _pairs.begin(); it1 != _pairs.end(); ++it1) {
			if ((*it)->foundation() == (*it1)->foundation()) {
				matches.push_back(*it1);
			}
		}
	}

	// If the check list is active, the agent changes the state of
	// all Frozen pairs in that check list whose foundation matches a
	// pair in the valid list under consideration to Waiting.
	if (isRunning()) {
		for (CandidatePairList::const_iterator it = matches.begin(); it != matches.end(); ++it) {
			if ((*it)->state() == CandidatePair::Frozen) {
				(*it)->setState(CandidatePair::Waiting);
			}
		}
	}

	// If the check list is frozen, and there is at least one pair in
	// the check list whose foundation matches a pair in the valid
	// list under consideration, the state of all pairs in the check
	// list whose foundation matches a pair in the valid list under
	// consideration is set to Waiting. This will cause the check
	// list to become active, and ordinary checks will begin for it,
	// as described in Section 5.8.
	else {
		if (matches.size()) {
			for (CandidatePairList::const_iterator it = matches.begin(); it != matches.end(); ++it) {
				(*it)->setState(CandidatePair::Waiting);
			}
		}

		// If the check list is frozen, and there are no pairs in the
		// check list whose foundation matches a pair in the valid list
		// under consideration, the agent
		//
		// +  groups together all of the pairs with the same foundation,
		//    and
		//
		// +  for each group, sets the state of the pair with the lowest
		//    component ID to Waiting. If there is more than one such
		//    pair, the one with the highest priority is used.
		//
		else {
			startConnectivityChecks();
		}
	}
}



// Constructs a STUN error response and sends it on the given connection.
void CheckList::sendError(LocalCandidate* local, const STUN::Message& request, const Net::Address& peerAddress, 
	int errorCode, const char* errorDesc) 
{
	Log("debug", this) << "Send STUN ERROR: (" << errorCode << ":" << errorDesc << ")" << endl;

	STUN::Message errMsg;
	errMsg.setType(request.type());
	errMsg.setTransactionID(request.transactionID());

	// SOFTWARE
	STUN::Software* softwareAttr = new STUN::Software;
	softwareAttr->copyBytes(_stream.agent().options().software.data(), _stream.agent().options().software.size());
	errMsg.add(softwareAttr);

	// REALM
	STUN::Realm* realmAttr = new STUN::Realm;
	softwareAttr->copyBytes(_stream.agent().options().realm.data(), _stream.agent().options().realm.size());
	errMsg.add(realmAttr);

	// NONCE
	STUN::Nonce* noonceAttr = new STUN::Nonce;
	string noonce = CryptoProvider::generateRandomKey(32);
	noonceAttr->copyBytes(noonce.c_str(), noonce.size());
	errMsg.add(noonceAttr);

	// ERROR-CODE
	STUN::ErrorCode* errAttr = new STUN::ErrorCode;
	errAttr->setErrorCode(errorCode);
	errAttr->setReason(errorDesc);
	errMsg.add(errAttr);

	local->send(errMsg, peerAddress);
}



} // namespace Sourcey
} // namespace ICE 
