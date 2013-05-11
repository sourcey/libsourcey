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


#include "Sourcey/XMPP/Router.h"
#include "Sourcey/XMPP/Stanza.h"
#include "Sourcey/Logger.h"
#include "assert.h"


using namespace std; 
using namespace Poco;


namespace Scy {
namespace XMPP {


Router::Router(Runner& runner, const string& jid) :
	Scy::Router(runner),
	_jid(jid) 
{	
}


Router::~Router() 
{
}


void Router::setJID(const string& jid) 
{ 
	Mutex::ScopedLock lock(_mutex);
	LogDebug() << "[Router] Setting the local JID to: " << jid << endl;
	_jid = jid; 
}


string Router::jid() const 
{ 
	Mutex::ScopedLock lock(_mutex);
	return _jid; 
}


void Router::respond(Stanza& stanza, bool stopPropagation) 
{
	if (stanza.type() == "set" || 
		stanza.type() == "get")
		stanza.setType("result");
	stanza.setTo(stanza.from());
	stanza.setFrom(_jid);
	*this >> stanza;
	if (stopPropagation)
		throw StopPropagation();
}


PersistenceT& Router::persistence() 
{ 
	Mutex::ScopedLock lock(_mutex);
	return _persistence; 
}


bool Router::stanzaIsRequestIQ(const Stanza& stanza) const
{
	return stanza.name() == "iq" &&
		 ((strcmp(stanza.child("iq").attribute("type").value(), "get") == 0) ||
		  (strcmp(stanza.child("iq").attribute("type").value(), "set") == 0));
}


bool Router::stanzaIsResponseIQ(const Stanza& stanza) const
{
	return stanza.name() == "iq" &&
		 ((strcmp(stanza.child("iq").attribute("type").value(), "result") == 0) ||
		  (strcmp(stanza.child("iq").attribute("type").value(), "error") == 0));
}


bool Router::isLocalJID(const string& other, bool absolute) const
{
	// Consider equal if the other JID has an instance 
	// of @ and matches our local JID.
	return (other.find("@") != string::npos)
		&& (absolute ? 
				(other == _jid) : 
				(strncmp(_jid.data(), other.data(), other.size()) == 0));
}


bool Router::stanzaIsLocal(const Stanza& stanza) const 
{
	// Stanzas are considered local when they are either
	// Inbound or Internal.
	return stanzaIsInternal(stanza) 
		|| stanzaIsInbound(stanza);		
}


bool Router::stanzaIsInternal(const Stanza& stanza) const 
{
	// Stanzas are considered internal in the following cases:
	//	The stanza is not a presence type and the 'to' and 'from' attributes are empty.
	//	The 'to' and 'from' attributes both equal the active JID.
	return (stanza.name() != "presence"
		&&  stanza.to().empty() 
		&&  stanza.from().empty()) 
		|| (isLocalJID(stanza.to(), false)
		&&  isLocalJID(stanza.from(), false));	
}


bool Router::stanzaIsInbound(const Stanza& stanza) const 
{
	// Stanzas are considered inbound in the following cases:
	//	The 'to' attribute equals the active JID.
	//	The 'from' attribute does not match the active JID.
	return isLocalJID(stanza.to(), false)
		&& !isLocalJID(stanza.from(), false);
}


bool Router::stanzaIsOutbound(const Stanza& stanza) const 
{
	// Stanzas are considered outbound in the following cases:
	//	The 'to' attribute is empty (server bound) or doesn't equal the active JID.
	//	The 'to' and 'from' attributes do not match or the 'to' attribute is empty.
	//	The stanza is a presence type and the 'to' and 'from' attributes are empty.
	return !stanzaIsInbound(stanza) 
		&& (stanza.to() != stanza.from()
		||  stanza.to().empty()) 
		|| (stanza.name() == "presence"
		&&  stanza.to().empty() 
		&&  stanza.from().empty());
}


Router& Router::operator >> (const char* data) 
{
	Stanza* s = Stanza::create(data);
	if (s) {
		send(*s);
		delete s;
	}
	else
		LogError() << "[Router:" << this << "] Dropped invalid XMPP stanza" << endl;
	return *this;
}


Router& Router::operator >> (const string& data) 
{
	return (*this >> data.data());
}


Router& Router::operator >> (Stanza& stanza) 
{
	send(stanza);
	return *this;
}


} // namespace XMPP 
} // namespace Scy
