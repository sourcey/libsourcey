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


#ifndef SOURCEY_XMPP_Client_H
#define SOURCEY_XMPP_Client_H


#include "Sourcey/XMPP/Router.h"
#include "Sourcey/XMPP/Stanza.h"
#include "Sourcey/Signal.h"
#include "Sourcey/Stateful.h"

#include "Poco/Thread.h"

#include <strophe.h>

#include <map>
#include <string>
#include <vector>


namespace Sourcey {
namespace XMPP {


struct ClientState: public StateT 
{
	enum Type 
	{
		None				= 0x00, //!< Nonexistent state. 
		Connecting			= 0x01, //!< Exchanging stream headers, authenticating and so on. 
		Connected			= 0x02, //!< Authenticated and bound.
		Disconnected		= 0x04	//!< Session closed, possibly due to error.
	};

	std::string str(unsigned int id) const 
	{ 
		switch(id) {
		case None:			return "None";
		case Connecting:	return "Connecting";
		case Connected:		return "Connected";
		case Disconnected:	return "Disconnected";
		}
		return "undefined"; 
	};
};


struct ClientCredentials
{	
	std::string		jid;
	std::string		password;	
	std::string		domain;	
	short			port;

	bool valid() {
		return jid.find("@") != std::string::npos
			&& !password.empty();
	}

	ClientCredentials(const std::string& jid = "", 
					  const std::string& password = "", 
					  const std::string& domain = "", 
					  short port = 5222) :
		jid(jid), 
		password(password), 
		domain(domain), 
		port(port) {} 
};


class XMPPTask;
typedef xmpp_stream_error_t StreamError;


// ---------------------------------------------------------------------
//
class Client: public StatefulSignal<ClientState>, public ILoggable
{
public:
	Client(Router& router);
	virtual ~Client();

	virtual void setCredentials(const std::string& jid, 
								const std::string& password, 
								const std::string& domain = "", 
								short port = 5222);
	virtual void login();
	virtual void logout();
	
	virtual void send(const Stanza& stanza);	
	virtual void send(const std::string& raw);	

	virtual Router& router() const;
	virtual ClientCredentials& credentials();
	virtual const StreamError* error() const;
	virtual std::string jid() const;
		// Returns the The JID bound by the server.
		// Empty if not connected.
	
	//virtual Presence geteratePresenceStanza(const std::string& to = "");

	virtual bool isConnected() const { return stateEquals(ClientState::Connected); };
	
	virtual const char* className() const { return "XMPP Client"; }
	
	//
	// WARNING: The following methods are exposed for C
	// callbacks and should not be used by the external
	// application.
	//	
	virtual void cleanup();
	virtual bool setState(unsigned int id);

protected:
	Router&				_router;
	ClientCredentials	_credentials;
	StreamError			_error;	
	XMPPTask*			_task;
	mutable Poco::FastMutex	_mutex;

	friend class XMPPTask;
};


// ---------------------------------------------------------------------
//
class XMPPTask: public ITask, public ILoggable
{
public:
	XMPPTask(Client& client, Runner& runner);
	
	virtual bool start();
	//virtual bool stop();
	
	virtual void send(const Stanza& stanza);
	virtual void send(const std::string& raw);
		// Sends raw XML data to the server. No checking
		// is provided so use with care.
	
	virtual Client& client();
	virtual const StreamError* error() const;
	//virtual std::string errorMessage() const;

	virtual std::string jid() const;
		// Returns the The JID bound by the server.
		// Empty if not connected.

	virtual void onRecvStanza(xmpp_stanza_t* const stanza);
		// Handles inbound remote stanzas. By default
		// they are sent to the router.

	virtual void onSendStanza(void*, Stanza& stanza);
		// Handles outgoing stanzas from the Router.

	virtual void run();	

	virtual const char* className() const { return "XMPP Client Task"; }
	
	//
	// WARNING: The following methods are exposed for C
	// callbacks and should not be used by the external
	// application.
	//	
    virtual xmpp_ctx_t*	context() const;
    virtual xmpp_conn_t* connection() const;
    virtual xmpp_log_t*	log() const;

protected:
	virtual ~XMPPTask();
	
	Client&			_client;
    xmpp_ctx_t*		_context;
    xmpp_conn_t*	_connection;
    xmpp_log_t*		_log;
	mutable Poco::FastMutex	_mutex;
};


} // namespace XMPP
} // namespace Sourcey


#endif  // SOURCEY_XMPP_Client_H