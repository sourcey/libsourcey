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


#include "Sourcey/ICE/Agent.h"
#include "Sourcey/CryptoProvider.h"
//#include "Sourcey/ICE/IAgentObserver.h"


//using namespace Sourcey::Util;
////using namespace Poco;
//using namespace Poco::Net;
using namespace std;


namespace Sourcey {
namespace ICE {


Agent::Agent(IAgentObserver& observer, const Options& options, Net::Reactor& reactor) : //, Runner& runner
	_observer(observer),
	_options(options), 
	_role(Controlled), 
	_reactor(reactor)//,
	//_runner(runner)
{
	_sessionID = CryptoProvider::generateRandomKey(10);
	_tieBreaker = CryptoProvider::generateRandomNumber(8);
	//setState(State::Waiting);
}


Agent::~Agent() 
{
	//Log("debug", this) << "Destroying" << endl;
	//Timer::getDefault().stop(TimerCallback<Agent>(this, &Agent::onICETimeout));
	//Util::ClearMap(_streams);

	assert(stateEquals(State::Terminating));
	//terminate();
	Log("debug", this) << "Destroying: OK" << endl;
}


void Agent::initiate() 
{
	assert(!_streams.empty());
	Log("debug", this) << "Initiating ICE" << endl;
		
	// As the initiator we take on the controlling role.
	_role = Agent::Controlling;

	// Begin ICE processing! Loop through our media streams and
	// begin gathering local candidates.	
	for (MediaStreamMap::const_iterator it = _streams.begin(); it != _streams.end(); ++it) {
		it->second->tryTransitionState(State::GatheringCandidates);
	}
}


void Agent::terminate() 
{
	Log("debug", this) << "Terminating ICE" << endl;	
	//assert(!stateEquals(State::Terminating));
	tryTransitionState(State::Terminating);
	Log("debug", this) << "Terminating ICE: OK" << endl; // ILoggable instance may be NULL
}


void Agent::restart() 
{
	// TODO: Implement me
	assert(false);
}


bool Agent::tryTransitionState(UInt32 id) 
{
	return setState(id);
}


bool Agent::canTransitionState(UInt32 id)
{
	Log("debug", this) << "Attempting to transition state from " << state() << " to " << state().str(id) << endl;

	// Ensure the proposed state is not already active.
	if (stateEquals(id)) {
		Log("debug", this) << "ICE is already " << state() << endl;	
		return false;
	}

	// We can terminate at any time.
	if (id == State::Terminating)
		return true;	

	// When states for all media streams are matching
	// we can transition the agent state.
	for (MediaStreamMap::const_iterator it = _streams.begin(); it != _streams.end(); ++it) {
		if (it->second->state().id() != id) {
			Log("debug", this) << "Unable to transition state to: " << id << endl;
			return false;
		}
	}
	
	// When local candidates from ALL our media streams are ready 
	// we can create the SDP and send it to our peer.
	if (id == State::SendingSDP) {	
		for (MediaStreamMap::const_iterator it = _streams.begin(); it != _streams.end(); ++it) {
			if (!it->second->hasFlag(MediaStream::LocalCandidatesGathered)) {
				Log("debug", this) << "Unable to transition state to: " << id << endl;	
				return false;
			}
		}
	}

	return true;
}

/*
*/


void Agent::onStateChange(State& state, const State& oldState)
{	
	//FastMutex::ScopedLock lock(_mutex);

	Log("debug", this) << "ICE state changed from " << state << " to " << oldState << endl;
		
	_observer.onICEStateChange(*this, state);

	switch (state.id()) {	
	case State::GatheringCandidates:
		Timer::getDefault().start(TimerCallback<Agent>(this, &Agent::onICETimeout, _options.timeout));
		for (MediaStreamMap::const_iterator it = _streams.begin(); it != _streams.end(); ++it) {
			it->second->startGatheringLocalCandidates();
		}
		break;

	case State::SendingSDP:
		{
			//Timer::getDefault().reset(TimerCallback<Agent>(this, &Agent::onICETimeout));

			Log("debug", this) << "Sending Local Candidates..." << endl;
			//if (_observer) {
				SDP::Message sdp;
				generateSDP(sdp);
				_observer.onOutgoingSDP(*this, sdp);	
				setFlag(LocalCandidatesSent);
			//}
						
			// If we are the controlled agent we can begin connectivity,
			// checking otherwise the state will transition after we receive 
			// the remote answer.
			if (!isControlling()) {
				for (MediaStreamMap::const_iterator it = _streams.begin(); it != _streams.end(); ++it) {
					it->second->tryTransitionState(State::CheckingConnectivity);
				}
			}
		}
		break;

	case State::CheckingConnectivity:
		//Timer::getDefault().reset(TimerCallback<Agent>(this, &Agent::onICETimeout));
		for (MediaStreamMap::const_iterator it = _streams.begin(); it != _streams.end(); ++it) {
			it->second->queue().startConnectivityChecks();
		}
		break;

	case State::Success:
		Timer::getDefault().stop(TimerCallback<Agent>(this, &Agent::onICETimeout));
		sendUpdatedOffers();
		break;	

	case State::Failed:
		Timer::getDefault().stop(TimerCallback<Agent>(this, &Agent::onICETimeout));

		// The observer should listen for failed state.
		// Failed state automatically sets Terminating state,
		// which removes all streams.
		terminate();
		break;	

	case State::Terminating:
		Timer::getDefault().stop(TimerCallback<Agent>(this, &Agent::onICETimeout));			
		Util::ClearMap(_streams);
		/*
		for (MediaStreamMap::const_iterator it = _streams.begin(); it != _streams.end(); ++it) {
			_runner.deleteLater<MediaStream>(it->second);
		}
		*/
		break;	
	}

		/*
	if (Stateful<State>::setState(this, id)) {

		return true;
	}
	return false;
	*/
}


void Agent::setFailed()
{
	Timer::getDefault().stop(TimerCallback<Agent>(this, &Agent::onICETimeout));
	setState(ICE::State::Failed);
}


void Agent::onMediaStreamStateChange(MediaStream* stream) 
{
	Log("debug", this) << "Media Stream State Changed: " << stream->state().toString() << endl;	
	tryTransitionState(stream->state().id());
}


bool Agent::processSDP(const SDP::Message& sdp) 
{
	//assert(!sdp.streams.empty());
	Log("debug", this) << "Processsing SDP: " << &sdp << endl;
		
	bool peerIsLite = sdp.hasSessionLevelAttribute("ice-lite");	

	// The agent will proceed with the ICE procedures defined in this
	// specification if, for each media stream in the SDP it received, the
	// default destination for each component of that media stream appears
	// in a candidate attribute. For example, in the case of RTP, the IP
	// address and port in the c and m lines, respectively, appear in a
	// candidate attribute and the value in the rtcp attribute appears in a
	// candidate attribute.
	//
	if (!sdp.isICESupported()) {

		// If this condition is not met, the agent MUST process the SDP based on
		// normal RFC 3264 procedures, without using any of the ICE mechanisms
		// described in the remainder of this specification with the following
		// exceptions:
		//
		// 1. The agent MUST follow the rules of Section 10, which describe
		//    keepalive procedures for all agents.
		//
		// 2. If the agent is not proceeding with ICE because there were
		//    a= candidate attributes, but none that matched the default
		//    destination of the media stream, the agent MUST include an a=ice-
		//    mismatch attribute in its answer.
		//
		// 3. If the default candidates were relayed candidates learned through
		//    a TURN server, the agent MUST create permissions in the TURN
		//    server for the IP addresses learned from its peer in the SDP it
		//    just received. If this is not done, initial packets in the media
		//    stream from the peer may be lost.

		Log("debug") << "Peer DOESN'T support ICE" << endl;

		// TODO: Handle this properly.
		return false;
	} 

	// Case where the external peer is the initiator. We need to parse the
	// peer's initial offer SDP to create our media streams.
	if (state().id() == State::Waiting && 
		!hasFlag(LocalCandidatesSent) && 
		!hasFlag(RemoteCandidatesReceived)) {
		setFlag(RemoteCandidatesReceived);

		// Since the offerer is the initiator, we will take on the controlled 
		// role unless the offerer is a lite implementation.
		if (peerIsLite)
			_role = Agent::Controlling;
		else
			_role = Agent::Controlled;

		// When an agent receives an initial offer, it will check if the offerer
		// supports ICE, determine its own role, gather candidates, prioritize
		// them, choose default candidates, encode and send an answer, and for
		// full implementations, form the check lists and begin connectivity
		// checks.

		// Create Media Streams and parse Candidates for each m= line in the SDP.
		return processOfferSDP(sdp);
	} 
	
	// Case where we are the initiator. We need to parse the peer's initial
	// answer to create remote candidates for our media streams.
	else if (state().id() == State::SendingSDP && //GatheringCandidates && 
		hasFlag(LocalCandidatesSent) && !hasFlag(RemoteCandidatesReceived)) {
		setFlag(RemoteCandidatesReceived);

		// This section describes the procedures that an agent follows when it
		// receives the answer from the peer. It verifies that its peer
		// supports ICE, determines its role, and for full implementations,
		// forms the check list and begins performing ordinary checks.

		// Create media streams and parse candidates for each m= line in the 
		// SDP answer.
		return processAnswerSDP(sdp);
	} 

	// Case where we have already exchanged candidates, are are receiving an 
	// updated offer from the peer.
	else if (/*state().id() == State::Success && _role == Controlling && */ 
		hasFlag(LocalCandidatesSent) && hasFlag(RemoteCandidatesReceived)) {
		//	 && 
		//hasFlag(SentUpdatedOffer)
		setFlag(ReceivedUpdatedOffer);		
		return processUpdatedOfferSDP(sdp);
	}

	// Something went wrong...
	else {
		assert(false);
	}

	return false;	
}


// Creates media streams and parses candidates for each m= line in an SDP.
bool Agent::processOfferSDP(const SDP::Message& sdp) 
{
	Log("debug", this) << "Processsing Remote Offer SDP: " << &sdp << endl;
	assert(!isControlling());
	assert(_streams.empty());

	vector<SDP::M*> lines = sdp.lines<SDP::M>();
	for (vector<SDP::M*>::const_iterator it = lines.begin(); it != lines.end(); ++it) {
		SDP::M* m = *it;
		MediaStream* stream = MediaStream::createFromSDP(*this, sdp, *m);
		assert(stream);
		if (!stream || !tryAddMediaStream(stream)) {
			assert(false);
			return false;
		}
	}
	
	// Initialize our new streams
	for (MediaStreamMap::const_iterator it = _streams.begin(); it != _streams.end(); ++it) {
		it->second->tryTransitionState(State::GatheringCandidates);
	}

	return !_streams.empty();
}


bool Agent::processAnswerSDP(const SDP::Message& sdp) 
{
	Log("debug", this) << "Processsing Remote Answer SDP" << endl;
	assert(!_streams.empty());

	int id = 1;	
	vector<SDP::M*> lines = sdp.lines<SDP::M>();
	for (vector<SDP::M*>::const_iterator it = lines.begin(); it != lines.end(); ++it) {
		SDP::M* m = *it;
		MediaStream* stream = getMediaStream(id);
		if (!stream || !stream->processAnswerSDP(sdp, *m)) {
			assert(false);
			return false;
			break;
		}
		id++;
	}

	assert(!_streams.empty());
	return !_streams.empty();
}


bool Agent::processUpdatedOfferSDP(const SDP::Message& sdp) 
{
	Log("debug", this) << "Processsing Udated Offer SDP" << endl;

	assert(!_streams.empty());
	//assert(!sdp.streams.empty());

	int id = 1;	
	vector<SDP::M*> lines = sdp.lines<SDP::M>();
	for (vector<SDP::M*>::const_iterator it = lines.begin(); it != lines.end(); ++it) {
		SDP::M* m = *it;
		MediaStream* stream = getMediaStream(id);
		if (!stream || !stream->processUpdatedOfferSDP(sdp, *m)) {
			assert(false);
			return false;
			break;
		}
		id++;
	}

	assert(!_streams.empty());
	return !_streams.empty();
}


	/*
bool Agent::processUpdatedAnswerSDP(const SDP::Message& sdp) {
	Log("debug", this) << "Processsing Udated Answer SDP" << endl;

	int id = 1;	
	for (list<SessionDescription::MediaStream>::const_iterator it = sdp.streams.begin(); it != sdp.streams.end(); ++it) {
		MediaStream* stream = getMediaStream(id);
		if (!stream && !stream->processUpdatedOfferSDP(*it)) {
			assert(false);
			return false;
			break;
		}
		id++;
	}

	bool res = false;
	int id = 1;
	SDP::M* m;
	MediaStream* stream;

	list<SDP::Line*> headers = sdp.lines();
	for (list<SDP::Line*>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
		if ((*it)->type() == SDP::Line::M) {
			m = reinterpret_cast<SDP::M*>(*it);
			//string mediaType = m->getMediaType();
			//string mediaProfile = m->transport();

			// Get the media stream associated with this m= element.
			stream = getMediaStream(id);			
			if (!stream) {
				assert(false);
				res = false;
				break;
			}
			if (!stream->processUpdatedOfferSDP(sdp, m)) {
				assert(false);
				res = false;
				break;
			}

			res = true;
			id++;
		}
	}

	return !_streams.empty();
}
	*/


// ---------------------------------------------------------------------
// SDP Parsing Functions
//
bool Agent::generateSDP(SDP::Message& sdp) 
{
	Log("debug", this) << "Generating Local SDP" << endl;
	/*
	v=0
	o=jdoe 2890844526 2890842807 IN IP4 $L-PRIV-1.IP
	s=
	c=IN IP4 $NAT-PUB-1.IP
	t=0 0
	a=ice-pwd:asd88fgpdd777uzjYhagZg
	a=ice-ufrag:8hhY
	m=audio $NAT-PUB-1.PORT RTP/AVP 0
	b=RS:0
	b=RR:0
	a=rtpmap:0 PCMU/8000
	a=candidate:1 1 UDP 2130706431 $L-PRIV-1.IP $L-PRIV-1.PORT typ
	host
	a=candidate:2 1 UDP 1694498815 $NAT-PUB-1.IP $NAT-PUB-1.PORT typ
	srflx raddr $L-PRIV-1.IP rport $L-PRIV-1.PORT

	v=0
	o=- 2890844526 2890842807 IN IP4 10.0.1.1
	s=
	c=IN IP4 192.0.2.3
	t=0 0
	a=ice-pwd:asd88fgpdd777uzjYhagZg
	a=ice-ufrag:8hhY
	m=audio 45664 RTP/AVP 0
	b=RS:0
	b=RR:0
	a=rtpmap:0 PCMU/8000
	a=candidate:1 1 UDP 2130706431 10.0.1.1 8998 typ host
	a=candidate:2 1 UDP 1694498815 192.0.2.3 45664 typ srflx raddr 10.0.1.1 rport 8998
    */

	// TODO: C lines and ice-pwd, ice-ufrag are handles in media stream;

    ostringstream ost;
	ost << "v=0\r\n" 
		<< "o=- " << _sessionID << " 2890842807 IN IP4 0.0.0.0\r\n"  // TODO: IP should match local address
		<< "s=0\r\n" 
		<< "t=0 0\r\n"
		<< "a=ice-pwd:" << _options.localPassword << "\r\n"
		<< "a=ice-ufrag:" << _options.localUFrag << "\r\n";

	for (MediaStreamMap::const_iterator it = _streams.begin(); it != _streams.end(); ++it) {
		 it->second->generateSDP(ost);
	}
	
	Log("debug", this) << "############ SDP: " << ost.str() << endl;
	sdp.read(ost.str());
	return sdp.isICESupported();
}


// ---------------------------------------------------------------------
// Media Stream Functions
//
MediaStream* Agent::createMediaStream(const std::string& title, int nComponents, const std::string& mediaType, const std::string& mediaProfile) 
{	
	MediaStream* stream = new MediaStream(*this, _streams.size() + 1, title, nComponents, mediaType, mediaProfile);
	tryAddMediaStream(stream);
	return stream;
}


bool Agent::tryAddMediaStream(MediaStream* stream) 
{
	if (getMediaStream(stream->index()) ||
		//_observer && 
		!_observer.onMediaStreamCreated(*this, *stream)) {
		assert(false);
		delete stream;
		stream = NULL;
		return false;
	}
	_streams[stream->index()] = stream;
	return true;
}


bool Agent::removeMediaStream(int index) 
{
	MediaStreamMap::iterator it = _streams.find(index);
	assert(it != _streams.end());
	if (it != _streams.end()) {
		delete it->second;
		_streams.erase(it);
		return true;
	}
	return false;
}


MediaStream* Agent::getMediaStream(int id) 
{
	MediaStreamMap::const_iterator it = _streams.find(id);
	//assert(it != _streams.end());
	if (it != _streams.end())
		return it->second;
	return NULL;
}


void Agent::onICETimeout(TimerCallback<Agent>& timer) 
{
	Log("debug", this) << "ICE Processing has timed out: " << _options.timeout << endl;
	setState(State::Failed);
	//return true;
}


void Agent::printLog(std::ostream& ost) const 
{
	ost << "["
		<< className()
		<< ":"
		<< this
		<< ":"
		<< _sessionID
		<< "] ";
}


// ---------------------------------------------------------------------
// Events & Callbacks
//

/*
void Agent::onMediaStreamLocalCandidatesGathered(MediaStream* stream, LocalCandidateList&) {
	Log("debug", this) << "Local Candidates Gathered" << endl;

	//FastMutex::ScopedLock lock(_mutex);

	Log("debug", this) << "Local Candidates Gathered 1" << endl;

	// When local candidates from ALL our media streams are ready 
	// we can create the SDP and send it to our peer.
	for (MediaStreamMap::const_iterator it = _streams.begin(); it != _streams.end(); ++it) {
		if (!it->second->hasFlag(LocalCandidatesSent)) {
			return;
		}
	}

	setFlag(LocalCandidatesSent);

	Log("debug", this) << "Sending Local Candidates..." << endl;
	_observer.onOutgoingSDP(*this, generateSDP());	
}
*/


//void Agent::onCandidateGatheringComplete(MediaStream* stream) {
//	Log("debug", this) << "Candidate Gathering Complete for " << stream->name() << endl;
//}


void Agent::onMediaStreamValidPairsCreated(MediaStream* stream, CandidatePairList& pairs) 
{	
	//FastMutex::ScopedLock lock(_mutex);
	Log("debug", this) << "Valid Pairs Created" << endl;

	// Once candidate pairs for each component of a media stream
    // enter the valid list, the answerer can begin sending media 
    // on that media stream.
	//
	_observer.onMediaStreamValidPairsChanged(*this, *stream, pairs);

	// When a media stream has a valid pair for each of it's components,
	// the agent examines the check list for each other media stream in 
	// turn:
	//
	// * If the check list is active, the agent changes the state of
	//   all Frozen pairs in that check list whose foundation matches a
	//   pair in the valid list under consideration to Waiting.
	//
	// * If the check list is frozen, and there is at least one pair in
	//   the check list whose foundation matches a pair in the valid
	//   list under consideration, the state of all pairs in the check
	//   list whose foundation matches a pair in the valid list under
	//   consideration is set to Waiting. This will cause the check
	//   list to become active, and ordinary checks will begin for it,
	//   as described in Section 5.8.
	//
	// * If the check list is frozen, and there are no pairs in the
	//   check list whose foundation matches a pair in the valid list
	//   under consideration, the agent
	//
	//   +  groups together all of the pairs with the same foundation,
	//      and
	//
	//   +  for each group, sets the state of the pair with the lowest
	//      component ID to Waiting. If there is more than one such
	//      pair, the one with the highest priority is used.
	//
	for (MediaStreamMap::const_iterator it = _streams.begin(); it != _streams.end(); ++it) {
		if (it->second != stream &&
			!it->second->queue().hasValidPairsAllComponents()) {
			it->second->queue().onAdjacentMediaStreamValidPairsCreated(pairs);
			return;
		}
	}

}


void Agent::sendUpdatedOffers() 
{	
	//FastMutex::ScopedLock lock(_mutex);
	assert(state().id() == State::Success);
	Log("debug", this) << "Sending Updated Offers..." << endl;

	/*
	// Once the state of each check list is Completed:
	//
	// *  The agent sets the state of ICE processing overall to
	//    Completed.
	//
	for (MediaStreamMap::const_iterator it = _streams.begin(); it != _streams.end(); ++it) {
		if (it->second->queue().state() != CheckList::Completed) {
			return;
		}
	}
	*/

	// *  If an agent is controlling, it examines the highest-priority
	//    nominated candidate pair for each component of each media
	//    stream. If any of those candidate pairs differ from the
	//    default candidate pairs in the most recent offer/answer
	//    exchange, the controlling agent MUST generate an updated offer
	//    as described in Section 9. If the controlling agent is using
	//    an aggressive nomination algorithm, this may result in several
	//    updated offers as the pairs selected for media change. An
	//    agent MAY delay sending the offer for a brief interval (one
	//    second is RECOMMENDED) in order to allow the selected pairs to
	//    stabilize.
	//
	if (_role == Controlling) {
		for (MediaStreamMap::const_iterator it = _streams.begin(); it != _streams.end(); ++it) {
			MediaStream* stream = it->second; 
			if (!stream->queue().isNominatedPairsMatchDefaultPairs()) {		
				Log("debug", this) << "Sending Updated Offer..." << endl;
				SDP::Message sdp; 
				generateSDP(sdp);
				//if (_observer)
				_observer.onOutgoingSDP(*this, sdp);	
				setFlag(SentUpdatedOffer);
			}
		}		
	}
}


void Agent::onMediaStreamCheckingFailed(MediaStream* stream) 
{
	//FastMutex::ScopedLock lock(_mutex);

	// o  If the state of the check list is Failed, ICE has not been able to
	//    complete for this media stream. The correct behavior depends on
	//    the state of the check lists for other media streams:
	//
	//    *  If all check lists are Failed, ICE processing overall is
	//       considered to be in the Failed state, and the agent SHOULD
	//       consider the session a failure, SHOULD NOT restart ICE, and the
	//       controlling agent SHOULD terminate the entire session.
	//
	//    *  If at least one of the check lists for other media streams is
	//       Completed, the controlling agent SHOULD remove the failed media
	//       stream from the session in its updated offer.
	//
	//    *  If none of the check lists for other media streams are
	//       Completed, but at least one is Running, the agent SHOULD let
	//       ICE continue.
	//	
	list<int> runningStreams;
	list<int> completedStreams;
	list<int> failedStreams;
	for (MediaStreamMap::const_iterator it = _streams.begin(); it != _streams.end(); ++it) {
		if (it->second->queue().state() == CheckList::Running)
			runningStreams.push_back(it->second->index());
		if (it->second->queue().state() == CheckList::Completed)
			completedStreams.push_back(it->second->index());
		if (it->second->queue().state() == CheckList::Failed)
			failedStreams.push_back(it->second->index());
	}
	if (failedStreams.size() == _streams.size()) {
		// ICE has failed, set our state causing termination.
		setState(State::Failed);
	}
	else if (!completedStreams.empty()) {
		// Remove any failed media streams.
		for (list<int>::const_iterator it = completedStreams.begin(); it != completedStreams.end(); ++it)
			removeMediaStream((*it));
	}
	if (!runningStreams.empty()) {
		// The ICE spec specifies is that only is none of the other
		// streams are completed are we to continue processing, but
		// we will just allow each stream to finnish...
	}
}


/*
void Agent::onMediaStreamNominatedPairsCreated(MediaStream* stream, CandidatePairList& nominatedPairs)
{

}
*/


void Agent::onMediaStreamSubsequentCheckingComplete(MediaStream* stream) 
{
	//FastMutex::ScopedLock lock(_mutex);
	assert(_role == Agent::Controlled);

	// Once there are no losing pairs, the agent can generate the answer.
	// It MUST set the default destination for media to the candidates in
	// the remote-candidates attribute from the offer (each of which will
	// now be the local candidate of a candidate pair in the valid list).
	// It MUST include a candidate attribute in the answer for each
	// candidate in the remote-candidates attribute in the offer.
	//
	SDP::Message sdp;
	generateSDP(sdp);
	//if (_observer)
	_observer.onOutgoingSDP(*this, sdp);	
	setFlag(SentUpdatedAnswer);
}


// ---------------------------------------------------------------------
// Accessors
//

Options& Agent::options() 
{ 
	//FastMutex::ScopedLock lock(_mutex);
	return _options; 
}


IAgentObserver& Agent::observer() 
{ 
	//FastMutex::ScopedLock lock(_mutex);
	return _observer; 
}


Net::Reactor& Agent::reactor()
{
	//FastMutex::ScopedLock lock(_mutex);
	return _reactor;
}


MediaStreamMap Agent::streams() const 
{ 
	//FastMutex::ScopedLock lock(_mutex);
	return _streams; 
};


/*

Runner& Agent::runner()
{
	//FastMutex::ScopedLock lock(_mutex);
	return _runner;
}
void Agent::setIAgentObserver(IAgentObserver& observer) { 
	//FastMutex::ScopedLock lock(_mutex);
	_observer = observer; 
}
*/


} // namespace Sourcey
} // namespace ICE 