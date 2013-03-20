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


#ifndef SOURCEY_XMPP_Router_H
#define SOURCEY_XMPP_Router_H


#include "Sourcey/XML/XML.h"
#include "Sourcey/XMPP/Types.h"
#include "Sourcey/XMPP/Stanza.h"
#include "Sourcey/XMPP/Command.h"
#include "Sourcey/Util/TimedManager.h"
#include "Sourcey/Router.h"
#include "Sourcey/Flaggable.h"


namespace Sourcey {
namespace XMPP {
		

typedef TimedManager<std::string, IPacket> PersistenceT;


class Router: public Sourcey::Router
	/// The XMPP router is responsibe for routing XMPP stanzas between
	/// the interface, application and remote clients. As such certain 
	/// guidelines must be adhered to during stanza creation.
	///
	/// Scoping:
	/// The XMPP router exists at the local application scope, therefore
	/// the 'incoming' stanzas originate from outside the application
	/// scope, and 'outgoing' stanzas outside from within the application 
	/// scope.
	///
	/// Local Stanza:
	/// A local stanza exists within the interface and application scope.
	/// Stanzas are considered local in the following cases:
	/// The 'to' attribute is the active JID, and the 'from' attribute is 
	/// empty.
	/// The 'from' and 'to' fields both equal the active JID.
	///
	/// Remote Stanza:
	/// A remote stanza exists within the interface and remote client
	/// scope.
	/// The 'from' attribute MUST be the active jid JID.
	/// The 'to' attribute is set to the remote client JID.
{
public:
	Router(Runner& runner, const std::string& jid = "");	
	virtual ~Router();

	Router& operator >> (const char* data);
	Router& operator >> (const std::string& data);
	Router& operator >> (Stanza& stanza);

	void respond(Stanza& stanza, bool stopPropagation = true);
		// Responds to an incoming XMPP stanza and sends it.
		// NOTE: Updates internal stanza data.

	//
	// Helpers to determine the origin of the stanza.
	//
	bool stanzaIsOutbound(const Stanza& stanza) const;
	bool stanzaIsInbound(const Stanza& stanza) const;
	bool stanzaIsLocal(const Stanza& stanza) const;
	bool stanzaIsInternal(const Stanza& stanza) const;
	bool stanzaIsRequestIQ(const Stanza& stanza) const;
	bool stanzaIsResponseIQ(const Stanza& stanza) const;

	void setJID(const std::string& jid);
	std::string jid() const;
	bool isLocalJID(const std::string& other, bool absolute = false) const;

	PersistenceT& persistence(); 
		// Returns the persistance queue for storing Stanzas
		// during asynchronous processing.

protected:
	std::string			_jid;
	PersistenceT		_persistence;
};


//
// XMPP Delegates
//


enum FilterFlags 
{
	AcceptInbound		= 0x01, 
	AcceptOutbound		= 0x02, 
	AcceptResquestIQ	= 0x04, 
	AcceptResponseIQ	= 0x08, 
	AcceptInternal		= 0x10
};


struct Filter: public Flags
{	
	Filter(const std::string& path, unsigned flags = 0) : 
		Flags(flags), path(path) {}
	
	Filter(unsigned flags = 0) : 
		Flags(flags) {}

	std::string path;
};


// ---------------------------------------------------------------------
//
struct XMPPDelegate: public PacketDelegateBase
{
	typedef Filter DataT;

	Filter filter;

	XMPPDelegate(const Filter& filter = Filter()) : filter(filter) {};
	XMPPDelegate(const XMPPDelegate& r) : filter(r.filter) {};
	
	virtual bool accepts(void* sender, IPacket& data, Void, Void, Void) 
	{
		Router* router = reinterpret_cast<Router*>(sender);
		Stanza* stanza = dynamic_cast<Stanza*>(&data);
		if (!router || !stanza) {
			return false;
		}

		//LogDebug() << "[XMPPDelegate] Checking Stanza: " 
		//	<< "\n\tstanzaIsOutbound: " << router->stanzaIsOutbound(*stanza)
		//	<< "\n\tstanzaIsInbound: " << router->stanzaIsInbound(*stanza)
		//	<< "\n\tstanzaIsInternal: " << router->stanzaIsInternal(*stanza)
		//	<< "\n\tstanzaIsRequestIQ: " << router->stanzaIsRequestIQ(*stanza)
		//	<< "\n\tstanzaIsResponseIQ: " << router->stanzaIsResponseIQ(*stanza)
		//	<< "\n\tstanzaIsOutbound: " << router->stanzaIsOutbound(*stanza) 
		//	<< endl;

		//LogDebug() << "[XMPPDelegate] Checking Flags: " 
		//	<< "\n\tAcceptOutbound: " << hasFlag(AcceptOutbound)
		//	<< "\n\tAcceptInbound: " << hasFlag(AcceptInbound)
		//	<< "\n\tAcceptInternal: " << hasFlag(AcceptInternal)
		//	<< "\n\tAcceptResquestIQ: " << hasFlag(AcceptResquestIQ)
		//	<< "\n\tAcceptResponseIQ: " << hasFlag(AcceptResponseIQ) 
		//	<< endl;

		if (//(	// Match presence flags (not restricted by scope)
			//	hasFlag(AcceptPresenceStanzas) && stanza->name() == "presence"
			//) 
			//||
			(	// Match any of the scoping flags
				(!filter.has(AcceptOutbound) && 
					!filter.has(AcceptInbound) && 
					!filter.has(AcceptInternal)) ||				
					(filter.has(AcceptOutbound)  && router->stanzaIsOutbound(*stanza))  ||
					(filter.has(AcceptInbound)	&& router->stanzaIsInbound(*stanza))	||
					(filter.has(AcceptInternal)  && router->stanzaIsInternal(*stanza))
			) 
			&&
			(	// Filter specific IQ types
				(!filter.has(AcceptResquestIQ) || 
					(filter.has(AcceptResquestIQ) && router->stanzaIsRequestIQ(*stanza))) &&
				(!filter.has(AcceptResponseIQ) || 
					(filter.has(AcceptResponseIQ) && router->stanzaIsResponseIQ(*stanza)))
			)) {
			return true;
		}
		
		return false;
	}
};


DefinePolymorphicDelegateWithArg(xmppDelegate, IPacket, XMPPDelegate, const Filter&, Filter())


// ---------------------------------------------------------------------
//
struct CommandDelegate: public XMPPDelegate
{
	CommandDelegate(const Filter& filter = Filter()) : XMPPDelegate(filter) {};
	CommandDelegate(const CommandDelegate& r) : XMPPDelegate(r) {};

	virtual bool accepts(void* sender, IPacket& data, Void empty2, Void empty3, Void empty4) 
	{
		if (XMPPDelegate::accepts(sender, data, empty2, empty3, empty4)) {

			IQ* iq = dynamic_cast<IQ*>(&data);
			if (!iq) {
				assert(false);
				return false;
			}

			Command c = iq->query<Command>();
			if (c.matches(filter.path)) {
				return true;
			}
		}

		return false;
	}
};


DefinePolymorphicDelegateWithArg(commandDelegate, IPacket, CommandDelegate, const Filter&, Filter())


// ---------------------------------------------------------------------
//
struct XPathDelegate: public XMPPDelegate
{
	XPathDelegate(const Filter& filter = Filter()) : XMPPDelegate(filter) {};
	XPathDelegate(const XPathDelegate& r) : XMPPDelegate(r) {};

	virtual bool accepts(void* sender, IPacket& data, Void empty2, Void empty3, Void empty4) 
	{		
		if (XMPPDelegate::accepts(sender, data, empty2, empty3, empty4)) {
			Stanza* stanza = dynamic_cast<Stanza*>(&data);
			if (stanza && !stanza->select_single_node(filter.path.data()).node().empty()) {
				return true;
			}
		}
		return false;
	}
};


DefinePolymorphicDelegateWithArg(xpathDelegate, IPacket, XPathDelegate, const Filter&, Filter())


} // namespace XMPP
} // namespace Sourcey


#endif // SOURCEY_XMPP_Router_H