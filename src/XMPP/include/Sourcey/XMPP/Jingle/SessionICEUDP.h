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


#ifndef SOURCEY_XMPP_JINGLE_SessionICEUDP_H
#define SOURCEY_XMPP_JINGLE_SessionICEUDP_H

#ifdef HAVE_LIBSOURCEY_ICE


#include "Sourcey/XML/XML.h"
#include "Sourcey/XMPP/Jingle/ISession.h"
#include "Sourcey/XMPP/Jingle/Jingle.h"
#include "Sourcey/ICE/Agent.h"
#include "Sourcey/PacketEmitter.h"
#include "Sourcey/Util/UserManager.h"
#include "Sourcey/Signal.h"

#include <assert.h>


namespace Scy {
namespace XMPP {
namespace Jingle {


class SessionICEUDP: public ISession, public ICE::IAgentObserver
{
public:
	struct JingleICEUDP: public XML::Template
		// The XML template for Jingle ICE-UDP stanzas.
	{
		JingleICEUDP(SessionICEUDP* session, const std::string& action);
	};

public:
	SessionICEUDP(SessionManager& manager,		
				  const std::string& initiator = "", 
				  const std::string& responder = "", 
				  const std::string& sid = "");
	SessionICEUDP(SessionManager& manager, 
				  const Jingle& j);
	
	virtual ICE::Agent* createICEAgent(const ICE::Options& options);
		// Creates and sets the ICE agent instance with the given
		// options Deletes the current .

	virtual void setICEAgent(ICE::Agent* ice);
		// Sets the ICE agent instance for us to use. This MUST be
		// done before initiating the session, generally via the
		// Session Manager's SessionCreated callback.
		// NOTE: A PacketEmitter should be added for each ICE
		// Media Stream with corresponding indexes.

	virtual void initiate();
		// Initiates ICE processing and candidate gathering as the 
		// controlling agent. A media source (unless recvonly) and
		// ICE agent must already be associated with the session.

	virtual void terminate(const std::string& reason = "success");
		// Terminates the current session and notifies the peer that
		// the session has been terminated.
		
	virtual bool process(IQ& iq);
		// Processed data received from the outside application, and
		// sends it to the internal ICE ice for processing. The data
		// may be a STUN message, media data, or an XMPP stanza.
	
	virtual PacketEmitter* geStreamMediaSource(const ICE::MediaStream& stream);
	virtual void removeMediaSources();

	//
	// ICE Handler Events
	//
	virtual bool onMediaStreamCreated(ICE::Agent& agent, ICE::MediaStream& stream);
		// Called when a media stream had been created. This is useful
		// for controlled agents.

	virtual const IUser* authenticateBindingRequest(ICE::Agent& agent, const std::string& username);

	virtual void onMediaStreamValidPairsChanged(ICE::Agent& agent, ICE::MediaStream& stream, const ICE::CandidatePairList& validPairs);
		// Once candidate pairs for each component of a media stream
		// enter the valid list, the answerer can begin sending media 
		// on that media stream.

	virtual void onSelectedPairsChanged(ICE::Agent& agent, ICE::MediaStream& stream, ICE::Component& component, 
		const ICE::CandidatePair* selectedPair, const ICE::CandidatePair* oldPair);
		// Called as when the selected candidate pair for the given
		// Component is changed. The implementation should change the
		// media destination to the new selected pair straight away.

	virtual void onMediaOnData(ICE::Agent& agent, ICE::MediaStream& stream, ICE::Component& component, 
		const DataPacket& packet, const Net::Address& localAddress, const Net::Address& remoteAddr);
		// Called when a media packet is received from a peer.

	virtual void onOutgoingSDP(ICE::Agent& agent, SDP::Message& sdp);
		// Called when outgoing SDP::Message packet is ready to be sent to the
		// peer.

	virtual void onICEStateChange(ICE::Agent& agent, ICE::State& state);
		// Called when the ICE agent state had changed.

	//
	// Jingle/SDP Helpers
	//
	bool sdpToJingle(IQ& iq, const SDP::Message& sdp);
	bool jingleToSDP(SDP::Message& sdp, const Jingle& j);

	//
	// Events
	//
	Signal2<const std::string&, const IUser*> AuthenticateICEConnectivityCheck;
		// Called when a STUN binding request is received from a peer.
		// The implementation should implement an Authenticator for
		// authenticating against the provided username and returning
		// User objects. If a User object is NULL the request is
		// considered not authenticated.	

	/*
	virtual PacketEmitter*& associateMediaSource(Agent& agent, MediaStream& stream) = 0;
		// Called when it is time to associate a PacketEmitter with a
		// media stream. If we are the controlling agent and a
		// MediaSource is already associated with the media stream then
		// this method will not be called. If the PacketEmitter object is
		// NULL the Media  Stream is set to Failed. The media stream is
		// NOT responsible for freeing the PacketEmitter.
		*/

	//Signal<const DataPacket&> OnData;
		// Called once the session has been established and a media
		// packet is received from a peer.

protected:
	virtual ~SessionICEUDP();

private:
	Poco::FastMutex		_mutex;
	ICE::Agent*			_ice;

	friend class SessionManager;
};


} // namespace Jingle
} // namespace XMPP 
} // namespace Scy 


#endif // HAVE_LIBSOURCEY_ICE

#endif // SOURCEY_XMPP_JINGLE_SessionICEUDP_H