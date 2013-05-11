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


#include "Sourcey/XMPP/Jingle/SessionManager.h"
#include "Sourcey/XMPP/Jingle/Jingle.h"
#include "Sourcey/Util.h"
#include "Sourcey/Logger.h"
#include "Poco/Format.h"

#include <iostream>
#include <assert.h>


using namespace std;
using namespace Poco;


namespace Scy {
namespace XMPP {
namespace Jingle {


SessionManager::SessionManager(Router& router) :
	_router(router) 
{
	LogDebug() << "[JingleSessionManager] Creating" << endl;
	_router.attach(
		xpathDelegate(this, &SessionManager::onRecvJingle, //XPathDelegate<SessionManager, IQ>
			Filter("/iq/jingle", AcceptInbound) ///*AcceptRecvStanzas |*/ 
		)
	);
}


SessionManager::~SessionManager() 
{	
	LogDebug() << "[JingleSessionManager] Destroying" << endl;
	//FastMutex::ScopedLock lock(_mutex);

	_router.detach(this);	
	terminateAll();
}


void SessionManager::terminateAll()
{
	FastMutex::ScopedLock lock(_mutex);

	SessionMap::iterator it = _items.begin();
	SessionMap::iterator it2;
	while (it != _items.end()) {
		it2 = it++;		
		(*it2).second->StateChange -= delegate(this, &SessionManager::onJingleSessionStateChange);
		(*it2).second->terminate();
		_items.erase(it2);
	}
}


SessionMap SessionManager::sessions() const 
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _items;
}


void SessionManager::onRecvJingle(void*, IQ& iq)
{
	// Catch Jingle Initiate requests here and create new sessions.
	try	{
		process(iq);
	}
	catch (Exception& exc) {
		LogError() << "[JingleSessionManager] Jingle Error: " << exc.displayText() << endl;	

		// We failed to initiate the Jingle session, so send an
		// XMPP Error as per XEP-0166 #6.3.2
		IQ iq;			
		iq.clear();
		iq.setError(0, "cancel", "bad-request", exc.displayText());
		_router.respond(iq);
	}
}


bool SessionManager::process(IQ& iq) 
{
	Jingle j = iq.query<Jingle>();
	
	LogDebug() << "[JingleSessionManager] Recv Jingle: " << j.sid() << endl;
	//FastMutex::ScopedLock lock(_mutex);
	ISession* s = get(j.sid(), false);
	if (!s)
		s = createSession(j);
	assert(s);
	if (s)
		return s->process(iq);
	return false;
}

	
ISession* SessionManager::createSession(const Jingle& j, bool whiny) 
{
	LogDebug() << "[JingleSessionManager] Create: " << j.sid() << endl;

	ISession* s = NULL;
	try	
	{	
		{			
			FastMutex::ScopedLock lock(_mutex);
			SessionMapT::iterator it = _types.find(j.transport().attribute("xmlns").value());
			if (it == _types.end())
				throw Exception(format("Unrecognized Jingle: %s", string(j.transport().attribute("xmlns").value())));
			s = it->second(*this, j);
		}
		assert(s);
		SessionCreated.emit(this, s, j);
		addSession(s); //)
		//throw Exception("Jingle session already exists: " + s->sid());
	}
	catch (Exception& exc) {
		LogError() << "[JingleSessionManager] " << exc.displayText() << endl;
		if (s) {
			delete s;
			s = NULL;
		}
		if (whiny)	
			exc.rethrow();
	}

    return s;
}


bool SessionManager::addSession(ISession* session, bool whiny)
{
	session->StateChange += delegate(this, &SessionManager::onJingleSessionStateChange);
	return SessionBase::add(session->sid(), session, whiny);
}


ISession* SessionManager::removeSession(const string& sid)
{
	return SessionBase::remove(sid);
}


void SessionManager::onJingleSessionStateChange(void* sender, XMPP::Jingle::SessionState& state, const XMPP::Jingle::SessionState&)
{
	LogDebug() << "[JingleSessionManager] Session State Changed: " << state.toString() << endl;

	if (state.id() == XMPP::Jingle::SessionState::Terminating) {
		ISession* session = reinterpret_cast<ISession*>(sender);
		LogDebug() << "[JingleSessionManager] Removing Terminating: " << session->sid() << endl;
		session->StateChange -= delegate(this, &SessionManager::onJingleSessionStateChange);
		assert(SessionBase::remove(session)); // delete the pointer
	}
}


void SessionManager::print(ostream& ost)
{
	FastMutex::ScopedLock lock(_mutex);

	ost << "[SessionManager(";
	for (SessionMap::iterator it = _items.begin();
		it != _items.end(); ++it) {
		ost << "\t";
		it->second->print(ost);
	}
	ost << ")]\n";
}


SessionMapT SessionManager::types() const 
{
	FastMutex::ScopedLock lock(_mutex);
	return _types; 
}


Router& SessionManager::router() 
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _router; 
}


} // namespace Jingle
} // namespace XMPP 
} // namespace Scy