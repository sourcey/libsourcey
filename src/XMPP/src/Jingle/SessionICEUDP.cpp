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


#ifdef HAVE_LIBSOURCEY_ICE


#include "Sourcey/XMPP/Jingle/SessionICEUDP.h"
#include "Sourcey/XMPP/Jingle/SessionManager.h"
#include "Sourcey/XMPP/Stanza.h"
#include "Sourcey/RTP/RTCP/Packet.h"

#include "Sourcey/Media/Format.h"
#include "Sourcey/Logger.h"
#include "Poco/Format.h"


using namespace std; 
using namespace Poco;
using namespace Scy::ICE;


namespace Scy {
namespace XMPP {
namespace Jingle {


SessionICEUDP::SessionICEUDP(SessionManager& manager,		
							 const string& initiator, 
							 const string& responder, 
							 const string& sid) : 
	ISession(manager, initiator, responder, sid),
	_ice(NULL) 
{
	LogDebug() << "ICEUDP Jingle Session Created:"		
		<< "\n\tPID: " << this
		<< "\n\tInitiator: " << this->_initiator
		<< "\n\tResponder: " << this->_responder
		<< "\n\tSID: " << this->_sid
		<< endl;
}


SessionICEUDP::SessionICEUDP(SessionManager& manager, const Jingle& j) :
	ISession(manager, j),
	_ice(NULL)
{
	LogDebug() << "ICEUDP Jingle Session Created:"		
		<< "\n\tPID: " << this
		<< "\n\tInitiator: " << this->_initiator
		<< "\n\tResponder: " << this->_responder
		<< "\n\tSID: " << this->_sid
		<< endl;
}


SessionICEUDP::~SessionICEUDP() 
{
	LogDebug() << "[SessionICEUDP:" << this << "] Destroying" << endl;

	// NOTE: The session will be removed from the manager
	// by the base implementation.
	if (_ice)
		delete _ice;

	LogDebug() << "[SessionICEUDP:" << this << "] Destroying: OK" << endl;
}


ICE::Agent* SessionICEUDP::createICEAgent(const ICE::Options& options)
{
	FastMutex::ScopedLock lock(_mutex);
	if (_ice)
		delete _ice;
	_ice = new ICE::Agent(*this, options);
	return _ice;
}


void SessionICEUDP::setICEAgent(ICE::Agent* ice) 
{
	FastMutex::ScopedLock lock(_mutex);
	_ice = ice;
}


void SessionICEUDP::initiate() 
{	
	FastMutex::ScopedLock lock(_mutex);
	assert(_ice);
	assert(!_ice->streams().empty());
	assert(!_sources.empty() || 
		    _ice->options().senders == "recvonly");

	/*	
	if (!stream.agent().isControlling() &&
		stream.agent().options().senders != "recvonly") {
		PacketEmitter* source = geStreamMediaSource(stream);
		if (!source) {
			assert(false);
			return false;
		}

	if (_ice->options().senders != "recvonly") {
		assert(!sources().empty());
		MediaStreamMap streams = _ice->streams();
		for (ICE::MediaStreamMap::iterator it = streams.begin(); it != streams.end(); ++it) {
			MediaStream* stream = it->second;
			if (!stream.source())
				stream.setMediaSource(getMediaSource(stream.name()));
			if (!stream.source())
				throw Exception("Media Stream has no PacketEmitter.");
		}
	}
	*/

	_ice->initiate(); 
}


void SessionICEUDP::terminate(const string& reason)
{
	LogDebug() << "[SessionICEUDP:" << this << "] Terminating: " << reason << endl;

	{
		//FastMutex::ScopedLock lock(_mutex);
	
		if (stateEquals(SessionState::Terminating)) {
			LogInfo() << "[SessionICEUDP:" << this << "] The session is already terminating!" << endl;
			return;
		}
	
		assert(!_initiator.empty());
		assert(!_responder.empty());
		assert(!_sid.empty());

		if (!stateEquals(SessionState::ReceivedTerminate) &&
			!stateEquals(SessionState::SentTerminate)) {
			LogDebug() << "[SessionICEUDP:" << this << "] Sending Terminate Stanza" << endl;

			IQ iq;
			iq.setType("set");
			iq.setTo(_ice->isControlling() ? _responder : _initiator);
			iq.setFrom(_ice->isControlling() ? _initiator : _responder);
			Jingle::Terminate t(
				reason, 
				_initiator, 
				_responder, 
				_sid);
			Jingle jingle(iq, t);

			_manager.router() >> iq;	
		}

		removeMediaSources();
		setState(this, SessionState::Terminating);
		if (_ice)
			_ice->terminate();	
	}
	delete this;
}

	
bool SessionICEUDP::process(IQ& iq) 
{	
	if (stateEquals(SessionState::Terminating))
		return false;

	Jingle j = iq.query<Jingle>();
	assert(!j.action().empty());
	LogTrace() << "[SessionICEUDP:" << this << "] Processing: " << j.sid() << ": " << j.action() << endl;	

	if (j.action() == "session-initiate") 
	{
		FastMutex::ScopedLock lock(_mutex);
		assert(_ice);
		assert(state().id() == SessionState::Initializing);
		assert(_ice->streams().empty());
		assert(!_ice->isControlling());
		setState(this, SessionState::ReceivedInitiate);

		// Process as Remote Offer SDP
		SDP::Message sdp;
		jingleToSDP(sdp, j);
		bool success = _ice->processSDP(sdp);
		assert(success);
		return success;
	}

	else if (j.action() == "session-accept") 
	{
		FastMutex::ScopedLock lock(_mutex);
		assert(_ice);
		assert(state().id() == SessionState::SentInitiate);
		assert(!_ice->streams().empty());
		assert(_ice->isControlling());
		setState(this, SessionState::ReceivedAccept);

		// Process as Remote Answer SDP
		SDP::Message sdp;
		jingleToSDP(sdp, j);
		bool success = _ice->processSDP(sdp);
		assert(success);
		return success;
	} 
	
	else if (j.action() == "session-terminate") 
	{
		setState(this, SessionState::ReceivedTerminate);
		terminate();
	}
	
	else if (j.action() == "transport-info") 
	{
		FastMutex::ScopedLock lock(_mutex);
		assert(_ice);
		assert(!_ice->streams().empty());

		// Process as Updated Offer SDP
		SDP::Message sdp;
		jingleToSDP(sdp, j);
		bool success = _ice->processSDP(sdp);
		assert(success);
		return success;
	} 

	return false;
}


void SessionICEUDP::removeMediaSources()
{			
	LogDebug() << "[SessionICEUDP:" << this << "] Removing Media Sources" << endl;

	// Stop all media callbacks.
	if (!_destroySources && _ice && _ice->options().senders != "recvonly") {

		MediaStreamMap streams = _ice->streams();
		for (ICE::MediaStreamMap::iterator it = streams.begin(); it != streams.end(); ++it) {
			MediaStream* stream = it->second;
			PacketEmitter* source = geStreamMediaSource(*stream);
			if (!source)
				continue;
					
			ComponentMap components = stream->components();
			if (components.size() >= 1)
				source->detach(packetDelegate(components[1], &Component::sendRTPPacket));
			if (components.size() >= 2)					
				source->detach(packetDelegate(components[2], &Component::sendRTCPPacket));
		}
	}
	
	ISession::removeMediaSources();
	LogDebug() << "[SessionICEUDP:" << this << "] Removing Media Sources: OK" << endl;
}


PacketEmitter* SessionICEUDP::geStreamMediaSource(const ICE::MediaStream& stream)
{
	if (static_cast<int>(_sources.size()) < stream.index()) {
		return false;
	}

	LogDebug() << "[SessionICEUDP:" << this << "] Media Stream Source: " << stream.index() << endl;

	return _sources[stream.index() - 1];
}


// ---------------------------------------------------------------------
// ICE Events
//
bool SessionICEUDP::onMediaStreamCreated(ICE::Agent& agent, ICE::MediaStream& stream) 
{
	LogDebug() << "[SessionICEUDP:" << this << "] Media Stream Created" << endl;
	FastMutex::ScopedLock lock(_mutex);

	// The the agent is controlled and we are sending media then
	// we must have a data source by this stage or the session 
	// will fail...
	if (!stream.agent().isControlling() &&
		stream.agent().options().senders != "recvonly") {
		if (!geStreamMediaSource(stream)) {
			assert(false);
			return false;
		}
	}
	LogDebug() << "[SessionICEUDP:" << this << "] Media Stream Created: OK" << endl;
	return true;
}


const IUser* SessionICEUDP::authenticateBindingRequest(ICE::Agent& agent, const string& username) 
{
	FastMutex::ScopedLock lock(_mutex);

	// The request username must equal the Jingle peer's JID node.
	const IUser* user = NULL;

	string peerUsername = _ice->isControlling() ? _responder : _initiator;
	string::size_type index = peerUsername.find("@");
	peerUsername = peerUsername.substr(0, index);
	if (username == peerUsername) {
		// If it is then we request authentication.
		AuthenticateICEConnectivityCheck.emit(this, username, user);
	} else
		LogError() << "[SessionICEUDP:" << this << "] ICE Binding request not equal to peer: " << username << ": " << peerUsername << endl;
	return user;
}


void SessionICEUDP::onMediaStreamValidPairsChanged(ICE::Agent& agent, ICE::MediaStream& stream, const CandidatePairList& validPairs) 
{
	LogDebug() << "[SessionICEUDP:" << this << "] Media Stream Valid Pairs Changed" << endl;
}


void SessionICEUDP::onSelectedPairsChanged(Agent& agent, MediaStream& stream, Component& component, 
	const CandidatePair* selectedPair, const CandidatePair* oldPair)
{
	/*
	// change our media destination to the new selected pair	
	if (_ice->options().senders != "recvonly") {
		PacketEmitter* source = geStreamMediaSource(stream);
		if (source) {
			source->attach(packetDelegate(this, &Component::onAudioCapture));


			_audio->attach(audioDelegate(this, &CaptureRecorder::onAudioCapture));
			ComponentMap 
			for (ComponentMap::iterator it = stream.components.begin(); it != _components.end(); ++it)
				*source += callback(it->second, &Component::sendPacket);
			if (!source->isActive()) {
				source->start();
			}
		}
	}
	*/
}


void SessionICEUDP::onMediaOnData(ICE::Agent& agent, ICE::MediaStream& stream, Component& component, 
										const DataPacket& packet, const Net::Address& localAddress, const Net::Address& remoteAddr) 
{
	LogTrace() << "[SessionICEUDP:" << this << "] Media Data Received: " << packet.size() << endl;

	// Delegates may wish to differentiate RTP from RTCP
	// packets by creating a callback delegate for each.
	emit(this, (IPacket&)packet);
}


void SessionICEUDP::onOutgoingSDP(ICE::Agent& agent, SDP::Message& sdp) 
{
	LogDebug() << "[SessionICEUDP:" << this << "] Outgoing SDP: Session State: " << state() << endl;
	FastMutex::ScopedLock lock(_mutex);

	assert(_ice == &agent);
	assert(!agent.streams().empty());
	//assert(!sources().empty());

	IQ iq;
	sdpToJingle(iq, sdp);
	if (state().id() == SessionState::Initializing) {
		setState(this, agent.isControlling() ? 
			SessionState::SentInitiate : 
			SessionState::SentAccept);
	}

	_manager.router() >> iq;
}


void SessionICEUDP::onICEStateChange(ICE::Agent& agent, ICE::State& state) 
{
	LogDebug() << "[SessionICEUDP:" << this << "] ICE State Changed: " << state.toString() << endl;

	switch(state.id()) {	
	case ICE::State::GatheringCandidates:
		break;

	case ICE::State::SendingSDP:
		break;

	case ICE::State::CheckingConnectivity:
		break;

	case ICE::State::Success:
		
		try	{
			FastMutex::ScopedLock lock(_mutex);

			// Start seding media if enabled.
			if (agent.options().senders != "recvonly") {
				MediaStreamMap streams = agent.streams();

				for (ICE::MediaStreamMap::iterator it = streams.begin(); it != streams.end(); ++it) {
					MediaStream* stream = it->second;

					PacketEmitter* source = geStreamMediaSource(*stream);
					if (!source)
						throw Exception("No media source for stream: " + stream->name());
					
					ComponentMap components = stream->components();
					if (components.size() >= 1) {
						LogDebug() << "[SessionICEUDP:" << this << "] Sending media:"
							<< "\n\tStream: " << stream->index() << ": " << stream->name() 
							<< "\n\tComponent: " << components[1]->id()
							<< endl;
						source->attach(packetDelegate(components[1], &Component::sendRTPPacket));
					}
					if (components.size() >= 2) {						
						LogDebug() << "[SessionICEUDP:" << this << "] Sending media:"
							<< "\n\tStream: " << stream->index() << ": " << stream->name() 
							<< "\n\tComponent: " << components[2]->id()
							<< endl;

						// If our stream has 2 components set up a delegate 
						// for receiving RTCP packet from the dispatcher.
						source->attach(packetDelegate(components[2], &Component::sendRTCPPacket));
					}
				}
			}
			
			setState(this, SessionState::InProgress);			
		}
		catch (Exception& exc) {
			// Set ICE processing to Failed, in turn terminating the Session.
			agent.setFailed();
		}

		break;	

	case ICE::State::Failed:
		{
			/*
			// TODO: In the unlikely event that one of the parties determines
			// that it cannot establish connectivity even after sending and
			// checking lower-priority candidates, it SHOULD terminate the 
			// session as described in XEP-0166.

			IQ iq("set", 
				_ice->isControlling() ? _responder : _initiator, 
				_ice->isControlling() ? _initiator : _responder);
			Jingle::create(iq, Jingle::Terminate("media-error", _initiator, _responder, _sid));
			_manager.router() >> iq;

			LogError() << "Jingle ICE UDP Session failed: " << _sid << endl;
			*/
		}
		break;	

	case ICE::State::Terminating:
		if (!stateEquals(SessionState::Terminating)) {
			terminate();
		}
		break;	
	}
}




// ---------------------------------------------------------------------
// Helpers
//
//
// <iq from='juliet@capulet.lit/balcony'
//     id='yd71f495'
//     to='romeo@montague.lit/orchard'
//     type='set'>
//   <jingle xmlns='urn:xmpp:jingle:1'
//           action='session-accept'
//           responder='juliet@capulet.lit/balcony'
//           sid='a73sjjvkla37jfea'>
//     <content creator='initiator' name='voice'>
//       <description xmlns='urn:xmpp:jingle:apps:rtp:1' media='audio'>
//         <payload-type id='97' name='speex' clockrate='8000'/>
//         <payload-type id='18' name='G729'/>
//       </description>
//       <transport xmlns='urn:xmpp:jingle:transports:ice-udp:1'>
//         <candidate component='1'
//                    foundation='2'
//                    generation='0'
//                    id='y3s2b30v3r'
//                    ip='192.0.2.3'
//                    network='1'
//                    port='45664'
//                    priority='1694498815'
//                    protocol='udp'
//                    rel-addr='10.0.1.1'
//                    rel-port='8998'
//                    type='srflx'/>
//       </transport>
//     </content>
//   </jingle>
// </iq>
//
// v=0
// o=jdoe 2890844526 2890842807 IN IP4 10.0.1.1
// s=
// c=IN IP4 192.0.2.3
// t=0 0
// a=ice-pwd:asd88fgpdd777uzjYhagZg
// a=ice-ufrag:8hhY
// m=audio 45664 RTP/AVP 0
// b=RS:0
// b=RR:0
// a=rtpmap:0 PCMU/8000
// a=candidate:1 1 UDP 2130706431 10.0.1.1 8998 typ host
// a=candidate:2 1 UDP 1694498815 192.0.2.3 45664 typ srflx raddr
// 10.0.1.1 rport 8998
//
bool SessionICEUDP::jingleToSDP(SDP::Message& sdp, const Jingle& jingle) 
{
	LogDebug() << "[SessionICEUDP:" << this << "] Jingle to SDP: " << jingle.sid() << endl;

	//jingle.print(cout);
	FastMutex::ScopedLock lock(_mutex);

	// TODO: Support multiple <content/> elements (media streams)

	assert(sdp.empty());
	assert(jingle.valid());
	Jingle::Transport transport = jingle.transport();
	Jingle::Candidate highestPriorityCandidate = jingle.highestPriorityCandidate();
	Jingle::Description description = jingle.description();

	string payloadIDs;
	for (XML::Node node = description.child("payload-type"); 
		node; node = node.next_sibling("payload-type")) {
		payloadIDs += node.attribute("id").value();
    }
	
	//assert(!c.empty());
    ostringstream ost;
	ost << "v=0\r\n" 
		<< "o=- " << jingle.sid() << " 2890842807 IN IP4 " << highestPriorityCandidate.ip() << "\r\n"  // TODO: IP should match local address
		<< "s=0\r\n" 
		<< "c=IN IP4 " << highestPriorityCandidate.ip() << "\r\n"
		<< "t=0 0\r\n"
		<< "a=ice-pwd:" << jingle.pwd() << "\r\n"
		<< "a=ice-ufrag:" << jingle.ufrag() << "\r\n"
		<< "i=" << jingle.content().name() << "\r\n"
		<< "m=" << description.mediaType() << " " << highestPriorityCandidate.port() << " RTP/AVP " << payloadIDs << "\r\n";

	// a=rtpmap
	for (XML::Node node = description.child("payload-type"); 
		node; node = node.next_sibling("payload-type")) {
		Jingle::PayloadType payload(node);
		ost << "a=rtpmap:" << payload.id() << " " << payload.name() << "/" << payload.clockrate() << "\r\n" ;
    }

	// a=candidate
	for (XML::Node node = transport.child("candidate"); 
		node; node = node.next_sibling("candidate")) {
		Jingle::Candidate candidate(node);
		ost << "a=candidate:" 
			<< candidate.foundation() << " " << candidate.component() << " UDP " << candidate.priority() << " " 
			<< candidate.ip() << " " << candidate.port() << " typ " << candidate.type();
		if (candidate.type() != "host")
			ost << " raddr "  << candidate.relAddr() << " rport " << candidate.relPort();
		ost << "\r\n";
	}

	// a=remote-candidates
	if (!transport.child("remote-candidate").empty()) {
		int count = 0;
		ost << "a=remote-candidates:";
		for (XML::Node node = transport.child("remote-candidate"); 
			node; node = node.next_sibling("remote-candidate")) {
			Jingle::RemoteCandidate candidate(node);
			if (count > 0) ost << " ";
			ost << candidate.component() << " " << candidate.ip() << " " << candidate.port();
			count++;
		}
		ost << "\r\n";
	}

	sdp.read(ost.str());
    return sdp.isICESupported();
}


bool SessionICEUDP::sdpToJingle(IQ& iq, const SDP::Message& sdp) 
{
	FastMutex::ScopedLock lock(_mutex);
	assert(_ice);
	assert(!_initiator.empty());
	assert(!_responder.empty());
	assert(!_sid.empty());
		
	LogDebug() << "[SessionICEUDP:" << this << "] SDP to Jingle:\n" << sdp.toString() << endl;
	assert(sdp.isICESupported());
		
	iq.setType("set");
	iq.setTo(_ice->isControlling() ? _responder : _initiator);
	iq.setFrom(_ice->isControlling() ? _initiator : _responder);
	JingleICEUDP t(this, state().action());
	Jingle jingle(iq, t);
	jingle.setUFrag(_ice->options().localUFrag);
	jingle.setPwd(_ice->options().localPassword);

	vector<SDP::M*> mLines = sdp.mediaLines();
	for (vector<SDP::M*>::const_iterator it = mLines.begin(); it != mLines.end(); ++it) {
		SDP::M* m = *it;

		// m=
		Jingle::Content content = jingle.content();		
		SDP::I* info = sdp.getMediaOrSessionLevelLine<SDP::I>(*m, "i=");
		if (info)
			content.setName(info->sessionInformation());

		Jingle::Description description = content.description();
		description.setMediaType(m->mediaType());

		StringVec payloadTypes = m->payloadTypes();
		for (size_t i = 0; i < payloadTypes.size(); ++i) {
			SDP::RTPMap* rtpmap = m->rtpmap(payloadTypes[i]);
			if (rtpmap) {
				Jingle::PayloadType payload = description.addPayloadType();
				payload.setId(stoi(rtpmap->payloadType().data()));
				payload.setName(rtpmap->encodingName());
				payload.setClockrate(rtpmap->clockRate());
				if (m->mediaType() == "audio") {
					payload.setChannels(stoi(rtpmap->encodingParameters().data()));
				}
			}
		}

		// a=candidate
		vector<SDP::Candidate*> candidates = m->candidates();
		for (vector<SDP::Candidate*>::const_iterator it = candidates.begin(); it != candidates.end(); ++it) {
			SDP::Candidate* candidate = *it;	
			Jingle::Candidate c = jingle.addCandidate();
			c.setComponent(candidate->componentID());
			c.setFoundation(candidate->foundation());
			//c.setGeneration
			//c.setId
			c.setIp(candidate->address());
			//c.setNetwork
			c.setPort(candidate->port());
			c.setPriority(candidate->priority());
			c.setType(candidate->candidateType());
			if (candidate->candidateType() != "host") {
				c.setRelAddr(candidate->relAddr());
				c.setRelPort(candidate->relPort());
			}
		}

		// a=remote-candidates
		vector<SDP::RemoteCandidate*> remoteCandidates = m->lines<SDP::RemoteCandidate>();
		for (vector<SDP::RemoteCandidate*>::const_iterator it = remoteCandidates.begin(); it != remoteCandidates.end(); ++it) {
			vector<SDP::RemoteCandidate::Type*> remoteCandidateTypes = (*it)->candidates();			
			for (vector<SDP::RemoteCandidate::Type*>::const_iterator ait = remoteCandidateTypes.begin(); ait != remoteCandidateTypes.end(); ++ait) {
				Jingle::RemoteCandidate c = jingle.transport().addRemoteCandidate();
				c.setComponent((*ait)->componentID);
				c.setIp((*ait)->address);
				c.setPort((*ait)->port);
			}
		}
	}

	return jingle.valid();
}


SessionICEUDP::JingleICEUDP::JingleICEUDP(
	SessionICEUDP* session,
	const string& action) 
{
	data = format(		
		"<jingle xmlns='urn:xmpp:jingle:1' action='%s' initiator='%s' responder='%s' sid='%s'>\n"
		"\t<content creator='initiator'>\n"
		"\t\t<description xmlns='urn:xmpp:jingle:apps:rtp:1' />\n"
		"\t\t<transport xmlns='urn:xmpp:jingle:transports:ice-udp:1' />\n"
		"\t</content>\n"
		"</jingle>\n", 
		action, 
		session->_initiator, 
		session->_responder, 
		session->_sid);
}


} // namespace Jingle
} // namespace XMPP 
} // namespace Scy 


#endif // HAVE_LIBSOURCEY_ICE