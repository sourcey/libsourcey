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


#ifndef ANIONU_JINGLE_SESSION_H
#define ANIONU_JINGLE_SESSION_H


#include "Sourcey/XMPP/Jingle/Jingle.h"
#include "Sourcey/PacketEmitter.h"
#include "Sourcey/Stateful.h"
#include "Sourcey/Util/UserManager.h"
#include "Sourcey/RTP/Codec.h"

#include <assert.h>


namespace Scy { 
namespace XMPP {
namespace Jingle {


struct SessionState: public State 
{
	enum Type 
	{
		Initializing = 0,
		SentInitiate,       // sent initiate, waiting for Accept or Reject
		ReceivedInitiate,   // received an initiate. Call Accept or Reject
		SentAccept,         // sent accept. begin connecting transport
		ReceivedAccept,     // received accept. begin connecting transport
		SentModify,         // sent modify, waiting for Accept or Reject
		ReceivedModify,     // received modify, call Accept or Reject
		SentReject,         // sent reject after receiving initiate
		ReceivedReject,     // received reject after sending initiate
		SentRedirect,       // sent direct after receiving initiate
		SentTerminate,      // sent terminate (any time / either side)
		ReceivedTerminate,  // received terminate (any time / either side)
		InProgress,         // session accepted and in progress
		Terminating,        // session is being destroyed
	};

	bool isActive() const 
	{ 	
		return id() != Initializing 
			&& id() != Terminating; 
	}

	std::string str(unsigned int id) const 
	{ 	
		switch(id) {
		case SessionState::Initializing:		return "Initializing";
		case SessionState::SentInitiate:		return "SentInitiate";
		case SessionState::ReceivedInitiate:	return "ReceivedInitiate";
		case SessionState::SentAccept:			return "SentAccept";
		case SessionState::ReceivedAccept:		return "ReceivedAccept";
		case SessionState::SentModify:			return "SentModify";
		case SessionState::ReceivedModify:		return "ReceivedModify";
		case SessionState::SentReject:			return "SentReject";
		case SessionState::ReceivedReject:		return "ReceivedReject";
		case SessionState::SentRedirect:		return "SentRedirect";
		case SessionState::SentTerminate:		return "SentTerminate";
		case SessionState::ReceivedTerminate:	return "ReceivedTerminate";
		case SessionState::InProgress:			return "InProgress";
		case SessionState::Terminating:			return "Terminating";
		}
		return "undefined"; 
	}

	std::string action() const 
	{ 	
		switch(id()) {
		case SessionState::Initializing: return "session-initiate";
		case SessionState::SentInitiate:
		case SessionState::ReceivedInitiate: return "session-accept";
		//default: return "transport-info";
		}
		return "transport-info";
	}
};


class SessionManager;


class ISession: public StatefulSignal<SessionState>, public PacketEmitter
{
public:	
	ISession(SessionManager& manager,
			const std::string& initiator = "",
			const std::string& responder = "",
			const std::string& sid = "");
	ISession(SessionManager& manager,
			const Jingle& j);
	virtual ~ISession();

	virtual void initiate() = 0;
		// ISession initialization logic should be implemented here.

	virtual void terminate(const std::string& reason = "success") = 0;
		// ISession termination logic should be implemented here.
		// NOTE: ISessions are never deleted, only terminated.	

	virtual bool process(IQ& iq) = 0;
		// Process incoming Jingle IQ stanzas for this SID.

	virtual void setSID(const std::string& sid) { _sid = sid; };
	virtual void setInitiator(const std::string& initiator) { _initiator = initiator; };
	virtual void setResponder(const std::string& responder) { _responder = responder; };
	virtual void setDestroySources(bool flag) { _destroySources = flag; };

	virtual std::string sid() const { return _sid; };
	virtual std::string initiator() const { return _initiator; };
	virtual std::string responder() const { return _responder; };
	virtual SessionManager& manager() const { return _manager; };
	virtual PacketEmitterList sources() const { return _sources; };
	
	virtual void addMediaSource(PacketEmitter* source);
	virtual void removeMediaSources();
		// Depending on weather the destroySources flag is set
		// the sources may or may not be destroyed.

	virtual void print(std::ostream& ost);

protected:
	SessionManager& _manager;
	std::string _sid;
    std::string _initiator;
    std::string _responder;
	PacketEmitterList _sources;
	bool _destroySources;

	friend class SessionManager;
};


//typedef std::map<std::string,ISession*> ISessionMap;


} // namespace Jingle
} // namespace XMPP 
} // namespace Scy 


#endif // ANIONU_JINGLE_SESSION_H

/*
class SessionManager 
{
public:
	virtual bool onJingleISessionCreated(ISession* session, const Jingle& jingle) = 0;
		// Called when a session has been created, but has not yet been
		// added to the stack. Returning false here will prevent creation,
		// and the session pointer will be deleted.

	virtual PacketEmitter* onCreateMediaSource(ISession* session, const std::string& mediaType, 
		const std::string& mediaProfile, const RTP::Codec& codec) = 0;
		// Called when signaling has completed and a PacketEmitter can be
		// associated with the session. The ISession will take ownership of 
		// the PacketEmitter.

	virtual IUser* onAuthenticateICEConnectivityCheck(ISession* session, const std::string& username) = 0;
		// Called when a STUN binding request is received from a peer.
		// The implementation should implement an Authenticator for
		// authenticating against the provided username and returning
		// User objects. If a User object is NULL the request is
		// considered not authenticated.

	virtual void onJingleSessionStateChange(ISession* session, SessionState& state) = 0;
		// Called when the Jingle session state is updated.

	virtual void onOutgoingJingle(ISession* session, const IQ& iq) = 0;
		// Called when outgoing Jingle element is ready to be sent to the
		// peer.
};
*/