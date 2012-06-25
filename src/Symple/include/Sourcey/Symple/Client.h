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


#ifndef SOURCEY_Symple_Client_H
#define SOURCEY_Symple_Client_H


#include "Sourcey/TimedManager.h"
#include "Sourcey/Net/Reactor.h"
#include "Sourcey/Net/WebSocket.h"
#include "Sourcey/SocketIO/Socket.h"
#include "Sourcey/Symple/Message.h"
#include "Sourcey/Symple/Command.h"
#include "Sourcey/Symple/Presence.h"
#include "Sourcey/Symple/Form.h"
#include "Sourcey/Symple/Peer.h"
#include "Sourcey/Symple/Roster.h"


namespace Sourcey {
namespace Symple {
	
	
typedef TimedManager<std::string, Message> PersistenceT;


class Client: public SocketIO::Socket
	/// Maximum simplicity. Maximum scope.
{
public:
	struct Options 
	{		
		std::string token;
		std::string user;
		std::string group;
		std::string name;
		std::string type;
		Net::Address serverAddr;

		Options() {
			token		= "";
			user		= "";
			group		= "global";
			name		= "";
			type		= "peer";
			serverAddr	= Net::Address("127.0.0.1", 4000);
		}
	};

public:
	Client(Net::Reactor& reactor, Runner& runner, const Options& options = Options()); 
	virtual ~Client();
	
	virtual void connect();
	virtual void close();
	
	virtual int send(const std::string data);
	virtual int send(Message& message, bool ack = false);
	virtual int sendPresence(bool probe = false);
	virtual int sendPresence(const Address& to, bool probe = false);
	virtual void createPresence(Presence& presence);

	virtual int respond(Message& message);
		/// Responds to an incoming message and sends it.
		/// NOTE: Internal message data is modified.
	
	Client& operator >> (Message& message);
		/// Stream operator alias for send()

    virtual Peer& ourPeer();
		/// Returns the peer object that controls the
		/// current session or throws an exception.
	
	Options& options();
	Roster& roster();
	Runner& runner();
	PersistenceT& persistence();
	std::string ourID() const;
	int announceStatus() const;

	Signal<int&> Announce;
		/// Notifies the outside application about the 
		/// response status code of our announce() call.
		/// Possible status codes are:
		///		- 200: Authentication success
		///		- 401: Authentication failed
		///		- 404: Bad request data
		///		- 500: Server not found

	Signal<Peer&> UpdatePresenceData;
		/// Called by createPresence() so outside classes
		/// can modify the outgoing Peer object.

protected:	
	virtual int announce();
		/// Called when a new connection is established
		/// to announce and authenticate the peer on the
		/// server.

	virtual void cleanup();
		/// Resets variables and data at the beginning  
		/// and end of each session.

	virtual void onOnline();
	virtual void onClose();
	virtual bool onPacketCreated(IPacket* packet);
	virtual void onAnnounce(void* sender, TransactionState& state, const TransactionState&);
	
	mutable Poco::FastMutex	_mutex;

	Options _options;
	Roster _roster;
	Runner& _runner;
	std::string _ourID;
	PersistenceT _persistence;
	int _announceStatus;
};


// ---------------------------------------------------------------------
//
enum FilterFlags 
{
	AcceptResquests		= 0x01, 
	AcceptResponses		= 0x02
};


struct Filter: public FlagT
{	
	Filter(const std::string& path, unsigned flags = 0) : 
		FlagT(flags), path(path) {}
	
	Filter(unsigned flags = 0) : 
		FlagT(flags), path("*") {}

	std::string path;
};


// ---------------------------------------------------------------------
//
struct MessageDelegate: public PacketDelegateBase
{
	typedef Filter DataT;
	Filter filter;

	MessageDelegate(const Filter& filter = Filter()) : filter(filter) {};
	MessageDelegate(const MessageDelegate& r) : filter(r.filter) {};
	
	virtual bool accepts(void* sender, IPacket& data, Void, Void, Void) 
	{
		Message* packet = dynamic_cast<Message*>(&data);
		if (packet &&
			(!filter.has(AcceptResquests) || 
				(filter.has(AcceptResquests) && packet->isRequest())) &&
			(!filter.has(AcceptResponses) || 
				(filter.has(AcceptResponses) && !packet->isRequest()))) {
			return true;
		}	
		return false;
	}
};


DefinePolymorphicDelegateWithArg(messageDelegate, IPacket, MessageDelegate, const Filter&, Filter())


struct CommandDelegate: public MessageDelegate
{
	CommandDelegate(const Filter& filter = Filter()) : MessageDelegate(filter) {};
	CommandDelegate(const CommandDelegate& r) : MessageDelegate(r) {};

	virtual bool accepts(void* sender, IPacket& data, Void empty2, Void empty3, Void empty4) 
	{
		if (MessageDelegate::accepts(sender, data, empty2, empty3, empty4)) {
			Command* c = dynamic_cast<Command*>(&data);
			return c && c->matches(filter.path);
		}
		return false;
	}
};


DefinePolymorphicDelegateWithArg(commandDelegate, IPacket, CommandDelegate, const Filter&, Filter())


} } // namespace Sourcey::Symple


#endif //  SOURCEY_Symple_Client_H