//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2005 Sourcey
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


#ifndef SOURCEY_XMPP_JINGLE_SessionManager_H
#define SOURCEY_XMPP_JINGLE_SessionManager_H


#include "Sourcey/BasicManager.h"
#include "Sourcey/XMPP/Jingle/ISession.h"
#include "Sourcey/XMPP/Jingle/Jingle.h"
#include "Sourcey/XMPP/Router.h"

#include <string>
#include <sstream>
#include <map>


namespace Sourcey {
namespace XMPP {
namespace Jingle {


template<typename T> 
ISession* createSessionT(SessionManager& manager, const Jingle& j) { return new T(manager, j); }
typedef std::map<std::string, ISession*(*)(SessionManager&, const Jingle&)> SessionMapT;


typedef BasicManager<std::string, ISession>	SessionBase;
typedef SessionBase::Map					SessionMap;


class SessionManager: protected BasicManager<std::string, ISession> 
	// The Session Manager maintains Jingle Sessions. It
	// parses incoming Jingle IQs and creates or returns
	// existing Sessions based on the SID attribute.
{
public:
	SessionManager(Router& router);
	virtual ~SessionManager();

	virtual void onRecvJingle(void*, IQ& iq);
	
	virtual bool process(IQ& iq);
		// Processes and incoming Jingle IQ stanza request,
		// and attempts to create the session if it does 
		// not exist.

    virtual ISession* createSession(const Jingle& j, bool whiny = true);
	virtual bool addSession(ISession* session, bool whiny = true);
	virtual ISession* removeSession(const std::string& sid);
		// Removes a session from the stack but does not 
		// delete the instance, which is done via a the
		// session's terminate() method.

	virtual void terminateAll();
		// Terminates all sessions gracefully.

	virtual void print(std::ostream& ost);
	virtual SessionMap sessions() const; 

	virtual void onJingleSessionStateChange(void*, XMPP::Jingle::SessionState& state, const XMPP::Jingle::SessionState&);

	//
	// Jingle Session Types
	//
	template<typename T>
    void registerType(const std::string& name)	
	{ 
		Poco::FastMutex::ScopedLock lock(_mutex);
		_types[name] = &createSessionT<T>; 
	}	

    void unregisterType(const std::string& s)	
	{ 
		Poco::FastMutex::ScopedLock lock(_mutex);
        SessionMapT::iterator it = _types.find(s);
        if (it == _types.end())
            return;
		_types.erase(it);
    }

    SessionMapT types() const;
    Router& router();

	Signal2<ISession*, const Jingle&> SessionCreated;
		// Called when a session has been created, but has not yet been
		// added to the stack. Throwing an exception here will prevent,
		// creation and cause the session pointer to be deleted. All 
		// session initialization login should be implemented from this
		// callback.

	Signal<ISession*> SessionRemoved;
		// Called when a session is removed from the stack.
	
protected:
    SessionMapT _types;
	Router&		_router;
	mutable Poco::FastMutex	_mutex;
};


} // namespace Jingle
} // namespace XMPP 
} // namespace Sourcey 


#endif // SOURCEY_XMPP_JINGLE_SessionManager_H