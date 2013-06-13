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


#include "Sourcey/ICE/MediaStream.h"
#include "Sourcey/ICE/Agent.h"
#include "Sourcey/ICE/Util.h"
//#include "Sourcey/Callback.h"


using namespace std;
using namespace Sourcey;


namespace Sourcey {
namespace ICE {


MediaStream* MediaStream::createFromSDP(Agent& agent, const SDP::Message& sdp, const SDP::M& m) 
{
	Log("debug") << "Creating Media Stream from SDP" << endl;
	
	int id = m.index();
	size_t nComponents = m.numComponents();
	bool hasCodecs = !m.getAttribute("rtpmap").empty();
	bool hasCandidates = !m.getAttribute("candidate").empty();
	string mediaType = m.mediaType();
	string mediaProfile = m.transport();
	//string ufrag = sdp.getMediaOrSessionLevelAttribute(m, "ice-ufrag");
	//string password = sdp.getMediaOrSessionLevelAttribute(m, "ice-pwd");
	//SDP::C* connection = sdp.getMediaOrSessionLevelLine<SDP::C>(m, "c=");
	//Net::Address defaultDestination;
	//if (connection)
	//	defaultDestination = Net::Address(connection->address(), m.port());

	// A media session name will be set from the i= line of the media
	// stream. In canses where this in unavailable it will be set from
	// the session-level i= attribute.
	string name;
	SDP::I* information = sdp.getMediaOrSessionLevelLine<SDP::I>(m, "i=");
	if (information)
		name = information->sessionInformation();

	assert(id);	
	assert(nComponents);
	assert(mediaType.size());	
	assert(mediaProfile.size());
	assert(hasCodecs);
	assert(hasCandidates);
	//assert(ufrag.size());
	//assert(password.size());
	//assert(defaultDestination.valid());
	if (!id || !nComponents || !hasCodecs || !hasCandidates || mediaType.empty() || 
		mediaProfile.empty() //|| ufrag.empty() || password.empty() || !defaultDestination.valid()
		) {
		assert(false);
		return NULL;
	}

	// Create the Media Stream
	MediaStream* stream = new MediaStream(agent, id, name, nComponents, mediaType, mediaProfile);	
	if (!stream->processOfferSDP(sdp, m)) {
		delete stream;
		return NULL;
	}

	return stream;
}


MediaStream::MediaStream(Agent& agent, int index, const string& name, int nComponents, 
						 const string& mediaType, const string& mediaProfile) : 
	_agent(agent), 
	_queue(*this), 
	_index(index), 
	_name(name), 
	_mediaType(mediaType), 
	_mediaProfile(mediaProfile)
{
	assert(mediaProfile == "RTP/AVP"); // Only RTP/AVP is supported for now.
	
	// Set the stream name to a random string if unset.
	if (_name.empty())
		_name = CryptoProvider::generateRandomKey(6);

	// For media streams based on RTP, there are two components per 
	// media stream -- one for RTP, and one for RTCP.
	Component* component;
	for (int i = 0; i < nComponents; i++) {
		component = createComponent();
	}
}


MediaStream::~MediaStream() 
{	
	//Log("debug", this) << "Destroying" << endl;

	// Clean up all candidate pairs
	_queue.deleteCandidatePairs();

	// First clear any candidates that may be in the gathering 
	// phase from our components.
	for (ComponentMap::iterator it = _components.begin(); it != _components.end(); ++it)
		it->second->terminateGatheringCandidates();

	// Destroy candidates in reverse because we need to ensure
	// that all base candidates are destroyed last just in case
	// there are active transactions.
	for (LocalCandidateList::reverse_iterator it = _localCandidates.rbegin(); it != _localCandidates.rend(); ++it) {
		(*it)->terminate();
		delete *it;
	}

	Util::ClearList(_remoteCandidates);
	Util::ClearMap(_components);
	

	Log("debug", this) << "Destroying: OK" << endl;
}


bool MediaStream::canTransitionState(UInt32 id) 
{
	Log("debug") << "MediaStream[" << _name << "] Attempting to transition state from " << state().id() << " to " << state().str(id) << endl;

	if (stateEquals(id))
		return false;

	switch (id)
	{	
	case State::Waiting:
		return true; //setState(State::Waiting);

	case State::GatheringCandidates:
		assert(state().id() == State::Waiting);
		if (state().id() == State::Waiting)	{
			// Agent will call startGatheringLocalCandidates() when 
			// adjacent streams are matching our state.
			return true; //setState(State::GatheringCandidates);
		}
		break;

	case State::SendingSDP:
		assert(state().id() == State::GatheringCandidates);
		assert(!_localCandidates.empty());
		
		//Log("debug") << "MediaStream[" << _name << "] _agent.isControlling(): " << _agent.isControlling() << endl;
		//Log("debug") << "MediaStream[" << _name << "] hasFlag(LocalCandidatesGathered): " << hasFlag(LocalCandidatesGathered) << endl;
		//Log("debug") << "MediaStream[" << _name << "] hasFlag(RemoteCandidatesReceived): " << hasFlag(RemoteCandidatesReceived) << endl;

		if (state().id() == State::GatheringCandidates &&
			(_agent.isControlling() && hasFlag(LocalCandidatesGathered) && !hasFlag(RemoteCandidatesReceived)) ||
			(!_agent.isControlling() && hasFlag(LocalCandidatesGathered) && hasFlag(RemoteCandidatesReceived))) {
			return true; //setState(State::SendingSDP);
		}
		assert(false);
		break;

	case State::CheckingConnectivity:
		assert(state().id() == State::SendingSDP);
		assert(!_localCandidates.empty());
		assert(!_remoteCandidates.empty());
		if (state().id() == State::SendingSDP &&
			hasFlag(LocalCandidatesGathered) ||
			hasFlag(RemoteCandidatesReceived)) {					
			// Agent will call startConnectivityChecks() when 
			// adjacent streams are matching our current state.
			return true; //setState(State::CheckingConnectivity);
		}
		break;

	case State::Success:
		assert(state().id() == State::CheckingConnectivity);		
		if (_queue.hasNominatedPairsAllComponents()) {	
			_queue.setState(CheckList::Completed);	
			_queue.updateSelectedPairs();
			
			/*
			assert(false);
			// TODO: Send media over selected candidate

			// Start seding media over our component's selected candidate pair 
			// if sending is enabled.
	
			if (_agent.options().senders != "recvonly") {
				assert(_provider);	
				if (_provider) {
					for (ComponentMap::iterator it = _components.begin(); it != _components.end(); ++it)
						*_provider += callback(it->second, &Component::sendPacket);
					if (!_provider->isActive()) {
						_provider->start();
					}
				}
			}
			*/

			return true; //setState(State::Success);
		}

	case State::Failed:
		//if (_provider)
		//	_provider->stop();
		_queue.setState(CheckList::Failed);	
		return true; //setState(State::Failed);
	}
	
	assert(false);
	return false;
}



/*
bool MediaStream::tryTransitionState(UInt32 id) 
{
	Log("debug") << "MediaStream[" << _name << "] Attempting to transition state from " << state().id() << " to " << state().str(id) << endl;
	//assert(id != state().id());
	if (id == state().id())
		return false;

	switch (id)
	{	
	case State::Waiting:
		return setState(State::Waiting);

	case State::GatheringCandidates:
		assert(state().id() == State::Waiting);
		if (state().id() == State::Waiting)	{
			// Agent will call startGatheringLocalCandidates() when 
			// adjacent streams are matching our state.
			return setState(State::GatheringCandidates);
		}
		break;

	case State::SendingSDP:
		assert(state().id() == State::GatheringCandidates);
		assert(!_localCandidates.empty());
		
		//Log("debug") << "MediaStream[" << _name << "] _agent.isControlling(): " << _agent.isControlling() << endl;
		//Log("debug") << "MediaStream[" << _name << "] hasFlag(LocalCandidatesGathered): " << hasFlag(LocalCandidatesGathered) << endl;
		//Log("debug") << "MediaStream[" << _name << "] hasFlag(RemoteCandidatesReceived): " << hasFlag(RemoteCandidatesReceived) << endl;

		if (state().id() == State::GatheringCandidates &&
			(_agent.isControlling() && hasFlag(LocalCandidatesGathered) && !hasFlag(RemoteCandidatesReceived)) ||
			(!_agent.isControlling() && hasFlag(LocalCandidatesGathered) && hasFlag(RemoteCandidatesReceived))) {
			return setState(State::SendingSDP);
		}
		assert(false);
		break;

	case State::CheckingConnectivity:
		assert(state().id() == State::SendingSDP);
		assert(!_localCandidates.empty());
		assert(!_remoteCandidates.empty());
		if (state().id() == State::SendingSDP &&
			hasFlag(LocalCandidatesGathered) ||
			hasFlag(RemoteCandidatesReceived)) {					
			// Agent will call startConnectivityChecks() when 
			// adjacent streams are matching our current state.
			return setState(State::CheckingConnectivity);
		}
		break;

	case State::Success:
		assert(state().id() == State::CheckingConnectivity);		
		if (_queue.hasNominatedPairsAllComponents()) {	
			_queue.setState(CheckList::Completed);	
			_queue.updateSelectedPairs();

			return setState(State::Success);
		}

	case State::Failed:
		//if (_provider)
		//	_provider->stop();
		_queue.setState(CheckList::Failed);	
		return setState(State::Failed);
	}

	//Log("debug") << "MediaStream[" << _name << "] Unable to transition state to: " << id << endl;		
	assert(false);
	return false;
}
*/


void MediaStream::onStateChange(State& state, const State& oldState)
{
	_agent.onMediaStreamStateChange(this);
}


bool MediaStream::processOfferSDP(const SDP::Message& sdp, const SDP::M& m) 
{
	Log("debug", this) << "Processing Offer SDP" << endl;	
	assert(_localCandidates.empty());
	assert(_remoteCandidates.empty());

	bool hasCodecs = !m.getAttribute("rtpmap").empty();
	bool hasCandidates = !m.getAttribute("candidate").empty();
	string ufrag = m.getAttribute("ice-ufrag");
	if (ufrag.empty())
		ufrag = sdp.getSessionLevelAttribute("ice-ufrag");
	string password = m.getAttribute("ice-pwd");
	if (password.empty())
		password = sdp.getSessionLevelAttribute("ice-pwd");		
	SDP::C* connection = sdp.getMediaOrSessionLevelLine<SDP::C>(m, "c=");
	Net::Address defaultDestination;
	if (connection)
		defaultDestination = Net::Address(connection->address(), m.port());

	assert(hasCodecs);
	assert(hasCandidates);
	assert(ufrag.size());
	assert(password.size());
	assert(defaultDestination.valid());
	if (!hasCodecs || !hasCandidates || ufrag.empty() || password.empty() || !defaultDestination.valid()) {
		assert(false);
		return false;
	}

	vector<SDP::RTPMap*> formats = m.lines<SDP::RTPMap>();
	for (vector<SDP::RTPMap*>::const_iterator it = formats.begin(); it != formats.end(); ++it) {
		//mediaType() == "audio" ? 
			//addCodec(Util::SDPToCodec((*it)->attributeType())) : 
			//addCodec(Util::SDPToCodec((*it)->attributeType()));
		addCodec(Util::SDPToCodec((*it)->attributeType()));
	}

	vector<SDP::Candidate*> candidates = m.lines<SDP::Candidate>();
	for (vector<SDP::Candidate*>::const_iterator it = candidates.begin(); it != candidates.end(); ++it) {
		RemoteCandidate* candidate = RemoteCandidate::createFromSDP(*it);	
		if (candidate && candidate->valid()) {
			if (tryAddRemoteCandidate(candidate)) {  // Deletes the pointer if candidate was pruned.
				Component* component = getComponent(candidate->componentID());
				if (!component)
					component = createComponent(candidate->componentID());
				if (candidate->transportAddr() == defaultDestination) {
					component->setDefaultRemoteCandidate(candidate);
				}
			}
		} else {
			assert(false);
			return false;
		}
	}
	
	setRemoteUFrag(ufrag);
	setRemotePassword(password);
	setRemoteCandidatesReceived();
	return !_remoteCandidates.empty();
}


// Creates the media stream remote components and remote candidates from a 
// remote answer.
bool MediaStream::processAnswerSDP(const SDP::Message& sdp, const SDP::M& m) 
{
	Log("debug", this) << "Processing Answer SDP" << endl;	
	assert(!_components.empty());

	bool hasCodecs = !m.getAttribute("rtpmap").empty();
	bool hasCandidates = !m.getAttribute("candidate").empty();
	string ufrag = m.getAttribute("ice-ufrag");
	if (ufrag.empty())
		ufrag = sdp.getSessionLevelAttribute("ice-ufrag");
	string password = m.getAttribute("ice-pwd");
	if (password.empty())
		password = sdp.getSessionLevelAttribute("ice-pwd");		
	SDP::C* connection = sdp.getMediaOrSessionLevelLine<SDP::C>(m, "c=");
	Net::Address defaultDestination;
	if (connection)
		defaultDestination = Net::Address(connection->address(), m.port());

	assert(hasCodecs);
	assert(hasCandidates);
	assert(ufrag.size());
	assert(password.size());
	assert(defaultDestination.valid());
	if (!hasCodecs || !hasCandidates || 
		ufrag.empty() || password.empty() || 
		!defaultDestination.valid()) {
		return false;
	}

	vector<SDP::Candidate*> attrs = m.lines<SDP::Candidate>();
	for (vector<SDP::Candidate*>::const_iterator it = attrs.begin(); it != attrs.end(); ++it) {			
		// TODO: Check that remote codecs match local.

		// Add candidates and components to the media stream.
		SDP::Candidate* attr = reinterpret_cast<SDP::Candidate*>(*it);	

		// Create the candidate from the SDP::Message header.
		RemoteCandidate* candidate = RemoteCandidate::createFromSDP(attr);	
		if (candidate && candidate->valid()) {
		
			// Add the candidate to the media stream.
			// NOTE: This call deletes the pointer if candidate was pruned.
			if (tryAddRemoteCandidate(candidate)) {  
				Component* component = getComponent(candidate->componentID());
				if (!component) {
					// The component should already exist! 
					// ICE failed!
					assert(false);
					return false;
				}
				if (candidate->transportAddr() == defaultDestination) {
					component->setDefaultRemoteCandidate(candidate);
				}
			}
		} else {
			assert(false);
			return false;
		}
	}


	/*
	assert(!sdp.ufrag.empty());
	assert(!sdp.password.empty());
	//assert(!sdp.candidates.empty());
	assert(!sdp.components.empty());
	if (sdp.ufrag.empty() || 
		sdp.password.empty() || 
		//sdp.candidates.empty() || 
		sdp.components.empty()) {
		return false;
	}

	// TODO: Authenticate the peer's credentials sent
	// with the offer.
	setRemoteUFrag(sdp.ufrag);
	setRemotePassword(sdp.password);

	for (list<SessionDescription::Component>::const_iterator it = sdp.components.begin(); it != sdp.components.end(); ++it) {
		assert(!(*it).candidates.empty());
		Component* component = getComponent((*it).id);
		for (list<Candidate>::const_iterator ait = (*it).candidates.begin(); ait != (*it).candidates.end(); ++ait) {
			assert(!(*it).id == (*ait).componentID());
			if ((*ait).valid()) {
		
				// Add the candidate to the media stream.
				// NOTE: This call deletes the pointer if candidate was pruned.
				RemoteCandidate* candidate = new RemoteCandidate(*ait);
				if (tryAddRemoteCandidate(candidate)) {  
					if (!component) {
						// The component should already exist! 
						// ICE failed!
						assert(false);
						return false;
					}
					if ((*ait).transportAddr() == (*it).defaultDestination) {
						component->setDefaultRemoteCandidate(candidate);
					}
				}
			} else {
				assert(false);
				return false;
			}
		}
	}
	*/
	
	setRemoteUFrag(ufrag);
	setRemotePassword(password);
	setRemoteCandidatesReceived();
	return true;
}


bool MediaStream::processUpdatedOfferSDP(const SDP::Message& sdp, const SDP::M& m) 
{
	Log("debug", this) << "Processing Updated Offer SDP" << endl;	
	
	// 9.2.2.1.  Existing Media Streams with ICE Running and no remote-candidates
	// 
	// If ICE is running for a media stream, and the offer for that media
	// stream lacked the remote-candidates attribute, the rules for
	// construction of the answer are identical to those for the offerer as
	// described in Section 9.1.2.1.
	// 
	// 9.2.2.2.  Existing Media Streams with ICE Completed and no remote-candidates
	// 
	// If ICE is Completed for a media stream, and the offer for that media
	// stream lacked the remote-candidates attribute, the rules for
	// construction of the answer are identical to those for the offerer as
	// described in Section 9.1.2.2, except that the answerer MUST NOT
	// include the a=remote-candidates attribute in the answer.

	// 9.2.2.3. Existing Media Streams and remote-candidates
	//
	// A controlled agent will receive an offer with the a=remote-candidates
	// attribute for a media stream when its peer has concluded ICE
	// processing for that media stream.  This attribute is present in the
	// offer to deal with a race condition between the receipt of the offer,
	// and the receipt of the Binding response that tells the answerer the
	// candidate that will be selected by ICE.  See Appendix B.6 for an
	// explanation of this race condition.  Consequently, processing of an
	// offer with this attribute depends on the winner of the race.
	//
	// The agent forms a candidate pair for each component of the media
	// stream by:
	//
	// o  Setting the remote candidate equal to the offerer's default
	//    destination for that component (e.g., the contents of the m and c
	//    lines for RTP, and the a=rtcp attribute for RTCP)
	//
	// o  Setting the local candidate equal to the transport address for
	//    that same component in the a=remote-candidates attribute in the
	//    offer.
	//
	vector<SDP::Candidate*> candidates = m.lines<SDP::Candidate>();
	vector<SDP::RemoteCandidate*> remoteCandidates = m.lines<SDP::RemoteCandidate>();

	// TODO: Get Default Destination on a per component basis.
	Net::Address defaultDestination;
	SDP::C* connection = sdp.getMediaOrSessionLevelLine<SDP::C>(m, "c=");
	if (connection)
		defaultDestination = Net::Address(connection->address(), m.port());
	assert(defaultDestination.valid());

	for (ComponentMap::const_iterator it = _components.begin(); it != _components.end(); ++it) {

		// TODO: Obtain local candidate from component not create it here!!
		assert(false);
		/*
		LocalCandidate* local = new LocalCandidate(it->second, Candidate::Host);
		RemoteCandidate* remote = new RemoteCandidate();

		for (vector<SDP::Candidate*>::const_iterator it1 = candidates.begin(); it1 != candidates.end(); ++it1) {
			SDP::Candidate* attr = reinterpret_cast<SDP::Candidate*>(*it1);	
			if (attr->address() == defaultDestination.host().toString()) {
				remote->setFoundation(attr->foundation());
				remote->setComponentID(attr->componentID());
				remote->setTransport(attr->transport());
				remote->setPriority(attr->priority());
				remote->setTransportAddr(defaultDestination);
				remote->setType(attr->candidateType());
			}
		}

		for (vector<SDP::RemoteCandidate*>::const_iterator it1 = remoteCandidates.begin(); it1 != remoteCandidates.end(); ++it1) {
			SDP::RemoteCandidate* attr = reinterpret_cast<SDP::RemoteCandidate*>(*it1);	
			vector<SDP::RemoteCandidate::Type*> candidates = attr->candidates();
			for (vector<SDP::RemoteCandidate::Type*>::const_iterator it2 = candidates.begin(); it2 != candidates.end(); ++it2) {
				if ((*it2)->componentID == it->second->id()) {
					local->setBase(local);
					local->setTransportAddr(Net::Address((*it2)->address, (*it2)->port));		
					local->computePriority();		
					local->computeFoundation();
				}
			}
		}

		if (!local->valid() || !remote->valid()) {
			assert(local->valid());
			assert(remote->valid());
			delete local;
			delete remote;
			return false;
		}

		// TODO: Ensure these candidates are properly destroyed.
		CandidatePair* pair = new CandidatePair(_queue, local, remote);	
		pair->setFlag(CandidatePair::OwnsChildCandidates);
		_queue.addUpdatedPair(pair);
		*/
	}

	_queue.checkUpdatedPairs();
	return true;
}


Component* MediaStream::createComponent(int id, UInt16 localPort) 
{
	if (getComponent(id)) {
		assert(false);
		return NULL;	
	}

	//FastMutex::ScopedLock lock(_mutex);

	Log("debug", this) << "Creating COMPONENT: " << (id > 0 ? id : _components.size() + 1) << endl;

	// TODO: If local port is specified and socket binding fails
	// we need to delete the component and return NULL here.
	Component* c = new Component(*this, id ? id : _components.size() + 1, localPort);		
	_components[id ? id : _components.size() + 1] = c;
	return c;
}


Component* MediaStream::getComponent(int id) const 
{
	//FastMutex::ScopedLock lock(_mutex);
	ComponentMap::const_iterator it = _components.find(id);
	//assert(it != _components.end());
	if (it != _components.end())
		return it->second;
	return NULL;
}


/*
void MediaStream::addCodec(const AudioCodec& codec) {
	//FastMutex::ScopedLock lock(_mutex);

	assert(_mediaType == "audio");
	Log("debug", this) << "Adding: " << codec.toString() << endl;
	_audioCodecs.push_back(codec);
}


void MediaStream::addCodec(const VideoCodec& codec) {
	//FastMutex::ScopedLock lock(_mutex);

	assert(_mediaType == "video");
	Log("debug", this) << "Adding: " << codec.toString() << endl;
	_videoCodecs.push_back(codec);
}


PacketStream* MediaStream::provider() 
{ 
	//FastMutex::ScopedLock lock(_mutex);
	return _provider; 
}


void MediaStream::setProvider(PacketStream* provider) 
{ 
	//FastMutex::ScopedLock lock(_mutex);
	_provider = provider; 
}


void MediaStream::startSendingMedia() 
{ 
	assert(stateEquals(MediaStream::Success));
	assert(_provider);
	
	assert(false);
	// TODO: Send media over selected candidate

	// Start seding media over our component's selected candidate
	// pair if sending is enabled.
	
	if (_agent.options().senders != "recvonly") {
		assert(_provider);	
		if (_provider) {
			for (ComponentMap::iterator it = _components.begin(); it != _components.end(); ++it)
				*_provider += callback(it->second, &Component::sendPacket);
			if (!_provider->isActive()) {
				_provider->start();
			}
		}
	}
}


void MediaStream::stopSendingMedia() 
{ 
	if (_provider)
		_provider->stop(); // callbacks will be removed
}
*/


// ---------------------------------------------------------------------
// Local Candidate Gathering
//
void MediaStream::startGatheringLocalCandidates() 
{
	//FastMutex::ScopedLock lock(_mutex);

	Log("debug", this) << "Gathering Local Candidates" << endl;
	assert(state().id() == State::GatheringCandidates);

	for (ComponentMap::const_iterator it = _components.begin(); it != _components.end(); ++it) {	
		it->second->startGatheringLocalCandidates();
	}
}


LocalCandidateList MediaStream::getLocalCandidatesForComponent(int componentID) const 
{
	LocalCandidateList candidates;
	for (LocalCandidateList::const_iterator it = _localCandidates.begin(); it != _localCandidates.end(); ++it) {	
		if ((*it)->componentID() == componentID) {
			candidates.push_back(*it);
		}
	}	
	return candidates;
}


RemoteCandidateList MediaStream::getRemoteCandidatesForComponent(int componentID) const 
{
	RemoteCandidateList candidates;
	for (RemoteCandidateList::const_iterator it = _remoteCandidates.begin(); it != _remoteCandidates.end(); ++it) {	
		if ((*it)->componentID() == componentID) {
			candidates.push_back(*it);
		}
	}	
	return candidates;
}

	
LocalCandidateList MediaStream::localCandidates() const 
{
	return _localCandidates;
}
	

RemoteCandidateList MediaStream::remoteCandidates() const 
{
	return _remoteCandidates;
}


/*
LocalCandidateList MediaStream::getLocalCandidatesByState(LocalCandidate::State state) {
	LocalCandidateList candidates;
	for (LocalCandidateList::const_iterator it = _localCandidates.begin(); it != _localCandidates.end(); ++it) {	
		if ((*it)->state() == state) {
			candidates.push_back(*it);
		}
	}	
	return candidates;
}
*/


void MediaStream::onLocalCandidateStateChange(LocalCandidate* candidate) 
{
	Log("debug", this) << "Local Candidate State Changed: " << candidate->toString() << endl;
}


void MediaStream::onComponentLocalCandidatesGathered(Component* component)
{
	Log("debug", this) << "Component Local Candidates Gathered: " << component->id() << endl;
	
	assert(!hasFlag(MediaStream::LocalCandidatesGathered));
	assert(component->hasFlag(Component::LocalCandidatesGathered));
	
	bool iceFailed = false;
	for (ComponentMap::const_iterator it = _components.begin(); it != _components.end(); ++it) {
		if (!it->second->hasFlag(Component::LocalCandidatesGathered))
			return; // wait for all components to complete gathering

		else if (it->second->defaultLocalCandidate() == NULL) {
			iceFailed = true;
			break;
		}
	}
	if (iceFailed) {
		Log("error", this) << "Unable to assign default candidate for component " << component->id() << endl;
		setState(State::Failed);
	}
	else {
		Log("debug", this) << "Local Candidate Gathering Complete" << endl;
		setFlag(MediaStream::LocalCandidatesGathered);	
		tryTransitionState(State::SendingSDP);
	}
	
}


	/*
bool MediaStream::trySetLocalCandidatesGathered() 
{
	Log("debug", this) << "Trying to set Local Candidates Gathered." << endl;
	assert(state().id() == State::GatheringCandidates);
	assert(!hasFlag(LocalCandidatesGathered));

	bool isComplete = false;
	for (ComponentMap::const_iterator it = _components.begin(); it != _components.end(); ++it) {	

		// Continue looping if the component's LocalCandidatesGathered
		// flag is already set...
		if (it->second->hasFlag(Component::LocalCandidatesGathered)) {	
			Log("debug", this) << "Local Candidates Gathering complete on Component " << it->second->id() << endl;
			isComplete = true;
			continue;
		}			

		// Loop through all candidates and established if they are in a
		// completed state.
		int nCandidates = it->second->numGatheringCandidates(); //getLocalCandidatesForComponent(it->second->id()).size();
		int nCompletedCandidates = 0;		
		for (LocalCandidateList::const_iterator lit = _localCandidates.begin(); lit != _localCandidates.end(); ++lit) {	
			if ((*lit)->state() == LocalCandidate::Success ||
				(*lit)->state() == LocalCandidate::Failed) {
				nCompletedCandidates++;
			} 
		}

		Log("debug", this) << "Component " << it->second->id() 
			<< " gathered " << nCompletedCandidates << " of " << nCandidates 
			<< " candidates." << endl;
		
		// If all candidates have completed then we can set component
		// default candidates and send our SDP.
		if (nCandidates && 
			nCandidates == nCompletedCandidates) {
			isComplete = true;
			it->second->setFlag(Component::LocalCandidatesGathered);
			selectComponentDefaultLocalCandidate(it->second);
		} else {
			isComplete = false;
			break;
		}
	}
	if (isComplete) {
		Log("debug", this) << "Setting Local Candidates Gathered" << endl;
		setFlag(LocalCandidatesGathered);
		tryTransitionState(State::SendingSDP);
	}

	return isComplete;
}
	*/


bool MediaStream::tryAddLocalCandidate(LocalCandidate* candidate, bool deleteIfPruned) 
{
	Log("debug", this) << "Adding Local Candidate: " << candidate->toString() << endl;	

	// Compute foundation and priority if unset.
	if (candidate->foundation().empty())	
		candidate->computeFoundation();
	if (!candidate->priority())
		candidate->computePriority();

	// A candidate is redundant if its transport address equals another candidate, 
	// and its base equals the base of that other candidate. Note that two
	// candidates can have the same transport address yet have different
	// bases, and these would not be considered redundant. Frequently, a
	// server reflexive candidate and a host candidate will be redundant
	// when the agent is not behind a NAT. The agent SHOULD eliminate the
	// redundant candidate with the lower priority.
	//
	//LocalCandidateList::const_iterator it = _localCandidates.begin();
	//while (it != _localCandidates.end()) {

	// Disabled candidate types are always redundant.
	if ((candidate->type() == Candidate::Host && _agent.options().disableHost) ||
		(candidate->type() == Candidate::ServerReflexive && _agent.options().disableSTUN) ||
		(candidate->type() == Candidate::Relayed && _agent.options().disableRelay)) {
		 candidate->makeRedundant();
	}
	else {
		for (LocalCandidateList::const_iterator it = _localCandidates.begin(); it != _localCandidates.end(); ++it) {	
				
			// Check for matching transport addresses and bases.
			if (candidate->transportAddr().toString() == (*it)->transportAddr().toString() &&
				candidate->base() == (*it)->base()) {
				
				LocalCandidate* winning = candidate->priority() > (*it)->priority() ? candidate : *it;
				LocalCandidate* loosing = candidate != winning ? candidate : *it;
				assert(winning != loosing);
			
				Log("debug", this) << "Making Candidate Redundant: " 
					<< "\n\tWinning: " << winning->toString() 
					<< "\n\tLoosing: " << loosing->toString() 
					<< endl;

				loosing->makeRedundant();

				/*
				// Make the candidate with the lowest priority redundant.
				if (candidate->priority() > (*it)->priority()) {	
					Log("debug", this) << "Old Local Candidate is Redundant: " 
						<< "\n\tWinning: " << candidate->toString() 
						<< "\n\tLoosing: " << (*it)->toString()
						<< endl;	
					//delete *it;
					(*it)->terminate();
					//_agent.runner().deleteLater<LocalCandidate>(*it);
					//it = _localCandidates.erase(it);	
					//break;
				} 

				else {
					Log("debug", this) << "New Local Candidate is Redundant: " 
						<< "\n\tWinning: " << (*it)->toString()
						<< "\n\tLoosing: " << candidate->toString() 
						<< endl;	
					if (deleteIfPruned) {
						//_agent.runner().deleteLater<LocalCandidate>(candidate);
						candidate->terminate();
						//delete candidate;
						//candidate = NULL;
					}
					return false;
				}
				*/
			}
		}
	}

	Log("debug", this) << "Added Local Candidate:" 
		<< "\n\t: " << candidate->toString() << endl;

	_localCandidates.push_back(candidate);
	_localCandidates.sort(Candidate::CompareProiroty); 
	
	LocalCandidateAdded.emit(this, *candidate);
	return true;
}


Candidate* MediaStream::getLocalCandidateByAddress(const Net::Address& address) const 
{
	for (LocalCandidateList::const_iterator it = _localCandidates.begin(); it != _localCandidates.end(); ++it) {	
		if ((*it)->transportAddr().toString() == address.toString())
			return *it;
	}
	return NULL;
}


/*
Candidate* MediaStream::getLocalCandidateByType(UInt16 port, UInt32 type) {
	for (LocalCandidateList::const_iterator it = _localCandidates.begin(); it != _localCandidates.end(); ++it) {	
		if (type == (*it)->type() && (*it)->transportPort() == port)
			return *it;
	}
	assert(false);
	return NULL;
}


void MediaStream::processLocalCandidates() {	
	Log("debug", this) << "Processing Local Candidates" << endl;
	Log("debug", this) << "Computing Foundations" << endl;



	// Loop through each local candidate
	string foundation = CryptoProvider::generateRandomKey(9);
	Candidate* currCandidate;
	Candidate* compCandidate;
	for (int idx = 0; idx < _localCandidates.size(); idx++) {
        currCandidate = _localCandidates[idx];

		// If the foundation value of the current candidate has already been
		// set we can continue the itteration.
		if (currCandidate->foundation().size()) continue;
		currCandidate->setFoundation(foundation);
	
		// Loop through other local candidates to determine if they
		// are of the same foundation, otherwise incriment the foundation
		// counter and continue.
		for (int i = 0; i < _localCandidates.size(); i++) {
			if (i == idx) continue;
			compCandidate = _localCandidates[i];
			if (*compCandidate ^=  *currCandidate)
				compCandidate->setFoundation(foundation);
		}

		foundation = CryptoProvider::generateRandomKey(9);
	}

	Log("debug", this) << "Prioritizing Candidates" << endl;

	for (LocalCandidateList::const_iterator it = _localCandidates.begin(); it != _localCandidates.end(); ++it) {	
		//Log("debug", this) << "%%%%%%%%%%%%%%%%%%%%%%%%% Prioritizing Candidates" << endl;
		(*it)->setPriority(computePriority((*it)->type(), (*it)->componentID()));
	}
}
*/


void MediaStream::addCodec(const Media::Codec& codec) 
{
	//FastMutex::ScopedLock lock(_mutex);
	Log("debug", this) << "Adding: " << codec.toString() << endl;
	_codecs.push_back(codec);
}



// ---------------------------------------------------------------------
// Remote Candidate Parsing
//

bool MediaStream::tryAddRemoteCandidate(RemoteCandidate* candidate, bool deleteIfPruned) 
{

	/*
	// A candidate is redundant if its transport address equals another candidate, 
	// and its base equals the base of that other candidate. Note that two
	// candidates can have the same transport address yet have different
	// bases, and these would not be considered redundant. Frequently, a
	// server reflexive candidate and a host candidate will be redundant
	// when the agent is not behind a NAT. The agent SHOULD eliminate the
	// redundant candidate with the lower priority.
	//
	for (RemoteCandidateList::const_iterator it = _remoteCandidates.begin(); it != _remoteCandidates.end(); ++it) {	
		// Check for matching base
		if (candidate->transportAddr().toString() == (*it)->transportAddr().toString()) {
			if (candidate->priority() > (*it)->priority()) {
				// Discard the old candidate if the new candidate has a higher 
				// priority (unlikely).
				Log("debug", this) << "Pruning Old Remote Candidate: " 
				<< "\n\tLoosing" << (*it)->toString()
				<< "\n\tWinning" << candidate->toString() 
				<< endl;	
				
				//_agent.runner().deleteLater<RemoteCandidate>(*it);
				//(*it)->terminate();
				delete *it;
				// Set the poninter to NULL as other processes may be using it.
				_remoteCandidates.erase(it);
				break;
			} else {
				// delete the new candidate.
				Log("debug", this) << "Pruning New Remote Candidate: " 
					<< "\n\tLoosing" << candidate->toString() 
					<< "\n\tWinning" << (*it)->toString()
					<< endl;
				if (deleteIfPruned) {
					//_agent.runner().deleteLater<RemoteCandidate>(candidate);
					//candidate->terminate();
					delete candidate;
					//candidate = NULL;
				}
				return false;
			}
			break;
			return false;
		}
	}
	*/

	// In order to prevent the attacks described in Section 18.5.2, 
    // we will limit the number of candidates they'll accept in an offer
    // or answer.
	if (_remoteCandidates.size() > _agent.options().maxRemoteCandidates) {
		Log("debug", this) << "Pruning New Remote Candidate:" 
			<< "\n\t" << candidate->toString() << endl;
		if (deleteIfPruned)
			delete candidate;
		return false;
	}

	_remoteCandidates.push_back(candidate);
	_remoteCandidates.sort(Candidate::CompareProiroty); 
	
	Log("debug", this) << "Added Remote Candidate:" 
		<< "\n\t" << candidate->toString() << endl;	

	RemoteCandidateAdded.emit(this, *candidate);
	return true;
}


RemoteCandidate* MediaStream::getRemoteCandidateByAddress(const Net::Address& address) const 
{
	for (RemoteCandidateList::const_iterator it = _remoteCandidates.begin(); it != _remoteCandidates.end(); ++it) {	
		if ((*it)->transportAddr() == address// || 
			 // Match the base also since it will be used by the peer to send data.
			//(*it)->baseAddr() == address
			)
			return *it;
	}
	return NULL;
}


void MediaStream::setRemoteCandidatesReceived() 
{
	Log("debug", this) << "Setting Remote Candidates Received" << endl;	

	//_isRemoteCandidatesReceived = true;
	assert(!hasFlag(RemoteCandidatesReceived));

	setFlag(RemoteCandidatesReceived);

	// If we are the controlling agent try to transition 
	// our state and begin connectivity checking.
	if (_agent.role() == Agent::Controlling)
		tryTransitionState(State::CheckingConnectivity);


	/*
	if (isCandidateGatheringComplete()) {
		_agent.onCandidateGatheringComplete(this);

		// Once candidate gathering is complete we can initiate 
		// connectivity checks.
		startConnectivityChecks();
	}
	*/
}


bool MediaStream::generateSDP(ostream& ost) const 
{
	assert(_name.size());
	assert(_components.size());

	Log("debug", this) << "Generating Local SDP" << endl;

	LocalCandidate* defaultCandidate = getComponent(1)->defaultLocalCandidate();
	assert(defaultCandidate);
	ost << "m=" << _mediaType << " " << defaultCandidate->transportPort() << " " << _mediaProfile;
	assert(_codecs.size());
	for (Media::CodecList::const_iterator it = _codecs.begin(); it != _codecs.end(); ++it)
		ost << " " << (*it).id;
	ost << "\r\n"; // end of m= line.
	ost << "c=IN IP4 " << defaultCandidate->transportIP() << "\r\n";
	for (Media::CodecList::const_iterator it = _codecs.begin(); it != _codecs.end(); ++it)
		ost << "a=rtpmap:" 
			<< (*it).id << " " 
			<< (*it).name << "/" 
			<< (*it).sampleRate 
			<< "\r\n";

	// 9.1.2.1.  Existing Media Streams with ICE Running
	//
	// If an agent generates an updated offer including a media stream that
	// was previously established, and for which ICE checks are in the
	// Running state, the agent follows the procedures defined here.
	//
	// An agent MUST include candidate attributes for all local candidates
	// it had signaled previously for that media stream.  The properties of
	// that candidate as signaled in SDP -- the priority, foundation, type,
	// and related transport address -- SHOULD remain the same.  The IP
	// address, port, and transport protocol, which fundamentally identify
	// that candidate, MUST remain the same (if they change, it would be a
	// new candidate).  The component ID MUST remain the same.  The agent
	// MAY include additional candidates it did not offer previously, but
	// which it has gathered since the last offer/answer exchange, including
	// peer reflexive candidates.
	//
	// The agent MAY change the default destination for media.  As with
	// initial offers, there MUST be a set of candidate attributes in the
	// offer matching this default destination.
	//
	if (_queue.isRunning()) {
		for (LocalCandidateList::const_iterator it = _localCandidates.begin(); it != _localCandidates.end(); ++it) {			
			if (!(*it)->stateEquals(CandidateState::Success)) // skip redundant
				continue;

			ost << (*it)->toSDP();
		}
	}

	// 9.1.2.2.  Existing Media Streams with ICE Completed
	// 
	// If an agent generates an updated offer including a media stream that
	// was previously established, and for which ICE checks are in the
	// Completed state, the agent follows the procedures defined here.
	// 
	// The default destination for media (i.e., the values of the IP
	// addresses and ports in the m and c lines used for that media stream)
	// MUST be the local candidate from the highest-priority nominated pair
	// in the valid list for each component.  This "fixes" the default
	// destination for media to equal the destination ICE has selected for
	// media.
	// 
	// The agent MUST include candidate attributes for candidates matching
	// the default destination for each component of the media stream, and
	// MUST NOT include any other candidates.
	// 
	// In addition, if the agent is controlling, it MUST include the
	// a=remote-candidates attribute for each media stream whose check list
	// is in the Completed state.  The attribute contains the remote
	// candidates from the highest-priority nominated pair in the valid list
	// for each component of that media stream.  It is needed to avoid a
	// race condition whereby the controlling agent chooses its pairs, but
	// the updated offer beats the connectivity checks to the controlled
	// agent, which doesn't even know these pairs are valid, let alone
	// selected. See Appendix B.6 for elaboration on this race condition.
	//
	else if (_queue.isCompleted()) {

		ost << defaultCandidate->toSDP();

		if (_agent.isControlling()) {
			ost << "a=remote-candidates:";
			assert(_components.size());
			Component* component = NULL;
			for (ComponentMap::const_iterator it = _components.begin(); it != _components.end(); ++it) {
				component = it->second;	
				assert(component && component->defaultRemoteCandidate());
				ost << component->id() << " " 
					<< component->defaultRemoteCandidate()->transportIP() << " " 
					<< component->defaultRemoteCandidate()->transportPort();
			}
			ost << "\r\n";
		}
	}

	if (_name.size())
		ost << "i=" << _name << "\r\n";
	ost << "b=RS:0\r\n"; // end of b= line.
	ost << "b=RR:0\r\n"; // end of b= line.
	
	return true;
}


/*
bool MediaStream::sendPacket(int componentID, const IPacket& packet) 
{
	//Log("debug") << "MediaStream sendPacket" << endl;
	ComponentMap::const_iterator it = _components.find(componentID);
	assert(it != _components.end());
	if (it != _components.end())
		return it->second->send(packet) > 0;
	return false;
}
*/


void MediaStream::printLog(std::ostream& ost) const 
{	
	//ss << _agent.printLog(std::ostream& ost);
	ost << "["
		<< className()
		<< ":"
		<< this
		<< ":"
		<< _name
		<< "] ";
}




/*
void MediaStream::addSelectedPair(CandidatePair* pair) {
	Log("debug", this) << "Add Selected Pair" << endl;

	// A selected pair is equal to the highest-priority nominated pair
	// for that component in the valid list if the state of the check 
	// list is Completed.

	// TODO: Check uniqueness
	_selectedPairs.push_back(pair);
	
	// Ensure a component is set for the local and remote candidates
	// of this pair.
	assert(*pair->local()->component()->defaultLocalCandidate() == *pair->local());
	assert(pair->local()->component());
	assert(pair->remote()->component());
}


// Removes a pair from the selected list.
bool MediaStream::removeSelectedPair(CandidatePair* pair) {
	if (pair->state() == CandidatePair::InProgress ||
		pair->state() == CandidatePair::Waiting ||
		pair->state() == CandidatePair::Succeeded)
		Log("debug") << "Removing an active selected candidate pair is unsafe." << endl;

	// Remove from the selected list.
	for (CandidatePairList::const_iterator it = _selectedPairs.begin(); it != _selectedPairs.end(); ++it) {
		if (**it == *pair) {
			_selectedPairs.erase(it);
			return true;
			break;
		}
	}	
	return false;
}
*/




} // namespace Sourcey
} // namespace ICE 