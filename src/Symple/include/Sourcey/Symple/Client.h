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


#include "Sourcey/SocketIO/Client.h"
#include "Sourcey/Net/Reactor.h"
#include "Sourcey/Net/WebSocket.h"
#include "Sourcey/Util/TimedManager.h"
#include "Sourcey/Symple/Message.h"
#include "Sourcey/Symple/Command.h"
#include "Sourcey/Symple/Presence.h"
#include "Sourcey/Symple/Form.h"
#include "Sourcey/Symple/Peer.h"
#include "Sourcey/Symple/Roster.h"


namespace Sourcey {
namespace Symple {

	
typedef TimedManager<std::string, Message> PersistenceT;


// ---------------------------------------------------------------------
//
class Client: public SocketIO::Client
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
	Client(Net::IWebSocket& socket, Runner& runner, const Options& options = Options());
	virtual ~Client();

	void connect();
	void close();

	virtual int send(const std::string data);
	virtual int send(Message& message, bool ack = false);

	virtual int respond(Message& message);
		/// Responds to an incoming message and sends it.
		/// NOTE: Internal message data is modified.

	virtual int sendPresence(bool probe = false);
		/// Broadcasts presence to the user group scope.

	virtual int sendPresence(const Address& to, bool probe = false);
		/// Sends directed presence to the given peer.
	
	virtual std::string ourID() const;
		/// Returns the session ID of our peer object.

    virtual Peer& ourPeer();
		/// Returns the peer object that controls the
		/// current session or throws an exception.

	virtual Roster& roster();
		/// Returns the roster which stores all online
		/// peers.

	virtual PersistenceT& persistence();
		/// Returns the persistence manager which stores
		/// long lived messages.

	virtual Client::Options& options();
		/// Returns a reference to the options object.

	virtual Client& operator >> (Message& message);	
		/// Stream operator alias for send().

	Signal<int&> Announce;
		/// Notifies the outside application about the 
		/// response status code of our announce() call.
		/// Possible status codes are:
		///		- 200: Authentication success
		///		- 401: Authentication failed
		///		- 400: Bad request data
		///		- 500: Server not found

	Signal<Peer&> UpdatePresenceData;
		/// Called by createPresence() so outside classes
		/// can modify the outgoing Peer object.

	virtual const char* className() const { return "Symple::Client"; }
	
protected:	
	virtual int announce();
		/// Called when a new connection is established
		/// to announce and authenticate the peer on the
		/// server.

	virtual void reset();
		/// Resets variables and data at the beginning  
		/// and end of each session.

	virtual void createPresence(Presence& p);
		/// Creates a Presence object.
	
	virtual void onOnline();
	virtual void onClose();

	virtual void onAnnounce(void* sender, TransactionState& state, const TransactionState&);
	virtual void onPacket(SocketIO::Packet& packet);

protected:	
	mutable Poco::FastMutex	_mutex;

	Roster _roster;
	std::string _ourID;
	PersistenceT _persistence;
	Client::Options _options;
	int _announceStatus;
};


// ---------------------------------------------------------------------
//
template <class WebSocketBaseT>
class ClientBase: public Client
{
public:
	ClientBase(Net::Reactor& reactor, Runner& runner = Runner::getDefault(), const Client::Options& options = Client::Options()) :
		_socket(reactor),
		Client(_socket, runner, options)
	{
	}

protected:
	WebSocketBaseT _socket;
};


// ---------------------------------------------------------------------
//
typedef Symple::ClientBase< 
	Net::WebSocketBase< 
		Net::StatefulSocketBase< 
			Net::SocketBase< Poco::Net::StreamSocket, Net::TCP, Net::IWebSocket >
		> 
	> 
> TCPClient;


// ---------------------------------------------------------------------
//
typedef Symple::ClientBase< 
	Net::WebSocketBase< 
		Net::StatefulSocketBase< 
			Net::SocketBase< Poco::Net::SecureStreamSocket, Net::SSLTCP, Net::IWebSocket >
		> 
	> 
> SSLClient;


// ---------------------------------------------------------------------
//
enum FilterFlags 
{
	AcceptResquests		= 0x01, 
	AcceptResponses		= 0x02
};


struct Filter: public Flags
{	
	Filter(const std::string& path, unsigned flags = 0) : 
		Flags(flags), path(path) {}
	
	Filter(unsigned flags = 0) : 
		Flags(flags), path("*") {}

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



	


/*
// ---------------------------------------------------------------------
//
typedef Symple::Client< 
	SocketIO::SocketBase< 
		Net::WebSocketBase< 
			Net::StatefulSocketBase< 
				Net::SocketBase< ::TCPContext, Symple::IClient > 
			> 
		> 
	> 
> Client;


typedef Symple::Client< 
	SocketIO::SocketBase< 
		Net::WebSocketBase< 
			Net::StatefulSocketBase< 
				Net::SocketBase< ::SSLContext, Symple::IClient > 
			> 
		> 
	> 
> SSLClient;
*/
	//virtual void onError();
	//virtual int announceStatus() const;
		/// Returns a reference to the options object.


	//Runner& _runner;
/*
// ---------------------------------------------------------------------
//
class IClient: public SocketIO::Client
	/// Maximum simplicity. Maximum scope.
{
public:

public:
	//Client(Net::Reactor& reactor, Runner& runner, const Options& options = Options()); 
	virtual ~IClient() {};
	
	virtual void connect() = 0;
	virtual void close() = 0;
	
	virtual int send(const std::string data) = 0;
	virtual int send(Message& message, bool ack = false) = 0;
	virtual int sendPresence(bool probe = false) = 0;
	virtual int sendPresence(const Address& to, bool probe = false) = 0;
	virtual void createPresence(Presence& presence) = 0;

	virtual int respond(Message& message) = 0;
		/// Responds to an incoming message and sends it.
		/// NOTE: Internal message data is modified.
	
	virtual IClient& operator >> (Message& message) = 0;
		/// Stream operator alias for send()

    virtual Peer& ourPeer() = 0;
		/// Returns the peer object that controls the
		/// current session or throws an exception.
	
	virtual Client::Options& options() = 0;
	virtual Roster& roster() = 0;
	virtual Runner& runner() = 0;
	virtual PersistenceT& persistence() = 0;
	virtual std::string ourID() const = 0;
	virtual int announceStatus() const = 0;

	Signal<int&> Announce;
		/// Notifies the outside application about the 
		/// response status code of our announce() call.
		/// Possible status codes are:
		///		- 200: Authentication success
		///		- 401: Authentication failed
		///		- 400: Bad request data
		///		- 500: Server not found

	Signal<Peer&> UpdatePresenceData;
		/// Called by createPresence() so outside classes
		/// can modify the outgoing Peer object.

protected:	
	virtual int announce() = 0;
		/// Called when a new connection is established
		/// to announce and authenticate the peer on the
		/// server.

	virtual void reset() = 0;
		/// Resets variables and data at the beginning  
		/// and end of each session.

	virtual void onOnline() = 0;
	virtual void onClose() = 0;
	virtual bool onPacketCreated(IPacket* packet) = 0;
	virtual void onAnnounce(void* sender, TransactionState& state, const TransactionState&) = 0;
};


// ---------------------------------------------------------------------
//
template <class SocketIO::Client>
class Client: public SocketIO::Client
{
public:
	Client(Net::Reactor& reactor, Runner& runner, const Client::Options& options = Client::Options()) : 
		SocketIO::Client(reactor),
		_runner(runner),
		_options(options),
		_announceStatus(500)
	{
		Log("trace") << "[Symple::Client:" << this << "] Creating" << std::endl;
	}


	virtual ~Client() 
	{
		Log("trace") << "[Symple::Client:" << this << "] Destroying" << std::endl;
		close();
		Log("trace") << "[Symple::Client:" << this << "] Destroying: OK" << std::endl;
	}


	void connect()
	{
		Log("trace") << "[Symple::Client:" << this << "] Connecting" << std::endl;
		
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			assert(!_options.user.empty());
			//assert(!_options.token.empty());
			_srvAddr = _options.serverAddr;
		}
		reset();
		SocketIO::Client::connect();
	}


	void close()
	{
		Log("trace") << "[Symple::Client:" << this << "] Closing" << std::endl;

		SocketIO::Client::close();
	}


	virtual int send(const std::string data)
	{
		assert(isOnline());
		return SocketIO::Client::send(SocketIO::Packet::Message, data, false);
	}


	virtual int send(Message& message, bool ack = false)
	{	
		assert(isOnline());
		message.setFrom(ourPeer().address());
		assert(message.valid());
		assert(message.to().id() != message.from().id());
		Log("trace") << "[Symple::Client:" << this << "] Sending Message: " 
			<< message.id() << ":\n" 
			<< JSON::stringify(message, true) << std::endl;
		return SocketIO::Client::send(message, false);
	}


	virtual void createPresence(Presence& p)
	{
		Log("trace") << "[Symple::Client:" << this << "] Creating Presence" << std::endl;

		Peer& peer = ourPeer();
		UpdatePresenceData.dispatch(this, peer);
		p["data"] = peer;
	}


	virtual int sendPresence(bool probe = false)
	{
		Log("trace") << "[Symple::Client:" << this << "] Broadcasting Presence" << std::endl;

		Presence p;
		createPresence(p);
		p.setProbe(probe);
		return send(p);
	}


	virtual int sendPresence(const Address& to, bool probe = false)
	{
		Log("trace") << "[Symple::Client:" << this << "] Sending Presence" << std::endl;
	
		Presence p;
		createPresence(p);
		p.setProbe(probe);
		p.setTo(to);
		return send(p);
	}


	virtual int respond(Message& message)
	{
		message.setTo(message.from());
		return send(message);
	}


	virtual Roster& roster() 
	{ 
		Poco::FastMutex::ScopedLock lock(_mutex);
		return _roster; 
	}


	virtual Runner& runner() 
	{ 
		Poco::FastMutex::ScopedLock lock(_mutex);
		return _runner; 
	}


	virtual PersistenceT& persistence() 
	{ 
		Poco::FastMutex::ScopedLock lock(_mutex);
		return _persistence; 
	}


	virtual Client::Options& options() 
	{ 
		Poco::FastMutex::ScopedLock lock(_mutex);
		return _options; 
	}


	virtual std::string ourID() const
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		return _ourID;
	}


	virtual int announceStatus() const
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		return _announceStatus;
	}


	virtual Peer& ourPeer()
	{	
		Poco::FastMutex::ScopedLock lock(_mutex);
		Log("trace") << "[Symple::Client:" << this << "] Getting Our Peer: " << _ourID << std::endl;
		if (_ourID.empty())
			throw Exception("No active peer session is available.");
		return *_roster.get(_ourID, true);
	}


	virtual IClient& operator >> (Message& message)
	{
		send(message);
		return *this;
	}
	

	virtual const char* className() const { return "Symple::Client"; }

	
protected:	
	virtual int announce()
	{
		JSON::Value data;
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			data["token"]	= _options.token;
			data["group"]	= _options.group;
			data["user"]	= _options.user;
			data["name"]	= _options.name;
			data["type"]	= _options.type;
		}	
		SocketIO::Packet p("announce", data, true);
		SocketIO::Transaction* txn = new SocketIO::Transaction(_runner, *this, p, 1, 5000);
		txn->StateChange += delegate(this, &Client::onAnnounce);
		return txn->send();
	}


	virtual void onAnnounce(void* sender, TransactionState& state, const TransactionState&) 
	{
		Log("trace") << "[Symple::Client:" << this << "] Announce Response: " << state.toString() << std::endl;
	
		SocketIO::Transaction* transaction = reinterpret_cast<SocketIO::Transaction*>(sender);
		switch (state.id()) {	
		case TransactionState::Success:
			try 
			{
				JSON::Value data = transaction->response().json()[(size_t)0];
				_announceStatus = data["status"].asInt();

				// Notify the outside application of the response 
				// status before we transition the client state.
				Announce.dispatch(this, _announceStatus);

				if (_announceStatus != 200)
					throw Exception(data["message"].asString()); //"Announce Error: " + 

				_ourID = data["data"]["id"].asString(); //Address();
				if (_ourID.empty())
					throw Exception("Invalid server response.");

				// Set our local peer data from the response or fail.
				_roster.update(data["data"], true);

				// Transition our state on Online.
				SocketIO::Client::onOnline();

				// Broadcast a presence probe to our network.
				sendPresence(true);
			}
			catch (Exception& exc)
			{
				// Set the error message and close the connection.
				setError(exc.displayText());
			}
			break;		

		case TransactionState::Failed:
			Announce.dispatch(this, _announceStatus);
			setError(state.message());
			break;
		}
	}


	virtual void onOnline()
	{
		Log("trace") << "[Symple::Client:" << this << "] On Online" << std::endl;

		// Override this method because we are not quite
		// ready to transition to Online yet - we still
		// need to announce our presence on the server.
		announce();
	}


	virtual bool onPacketCreated(IPacket* packet) 
	{
		Log("trace") << "[Symple::Client:" << this << "] Packet Created: " << packet->className() << std::endl;

		// Catch incoming messages here so we can parse
		// messages and handle presence updates.

		SocketIO::Packet* p = dynamic_cast<SocketIO::Packet*>(packet);
		if (p && (
			p->type() == SocketIO::Packet::Message || 
			p->type() == SocketIO::Packet::JSON)) {

			JSON::Value data = p->json();
			if (!data.isObject() || data.isNull()) {
				Log("warning") << "[Symple::Client:" << this << "] Packet is not a JSON object" << std::endl;
				return true; // continue propagation
			}
		
			string type(data["type"].asString());
			Log("trace") << "[Symple::Client:" << this << "] Packet Created: Symple Type: " << type << std::endl;
			if (type == "message") {
				Message m(data);
				dispatch(this, m);
				return false; // stop propagation
			}
			else if (type == "command") {
				Command c(data);
				dispatch(this, c);
				return false; // stop propagation
			}
			else if (type == "presence") {
				Presence p(data);
				if (p.isMember("data"))
					_roster.update(p["data"], false);
				dispatch(this, p);
				if (p.isProbe())
					sendPresence(p.from());
				return false; // stop propagation
			}
		}
		return true;
	}


	virtual void onClose()
	{
		Log("trace") << "[[Symple::Client:" << this << "] Closing" << std::endl;
		SocketIO::Client::onClose();
		reset();
	}


	virtual void onError()
	{
		SocketIO::Client::onClose();
		reset();
	}


	void reset()
	{
		_roster.clear();
		_announceStatus = 500;
		_ourID = "";
	}
	

protected:	
	mutable Poco::FastMutex	_mutex;

	Roster _roster;
	Runner& _runner;
	std::string _ourID;
	PersistenceT _persistence;
	Client::Options _options;
	int _announceStatus;
};


// ---------------------------------------------------------------------
//
typedef Symple::Client< 
	SocketIO::SocketBase< 
		Net::WebSocketBase< 
			Net::StatefulSocketBase< 
				Net::SocketBase< ::TCPContext, Symple::IClient > 
			> 
		> 
	> 
> Client;


typedef Symple::Client< 
	SocketIO::SocketBase< 
		Net::WebSocketBase< 
			Net::StatefulSocketBase< 
				Net::SocketBase< ::SSLContext, Symple::IClient > 
			> 
		> 
	> 
> SSLClient;
*/




//typedef Symple::Client<  > Client; //, Symple::IClient //Symple::SocketBase< ::TCPContext>
//typedef Symple::Client< SocketIO::SocketBase< Net::StatefulSocketBase< Net::SocketBase< ::SSLContext, Symple::IClient> > > > SSLClient; //, Symple::IClient //Symple::SocketBase< ::SSLContext>
//SocketIO::SSLSocket
// Net::SocketBase< ::SSLContext, SocketIO::Client>

 //, public Symple::IClient
//class Client: public SocketIO::Client
	/// Maximum simplicity. Maximum scope.//BaseT

/*

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
		///		- 400: Bad request data
		///		- 500: Server not found

	Signal<Peer&> UpdatePresenceData;
		/// Called by createPresence() so outside classes
		/// can modify the outgoing Peer object.
	
protected:	
	virtual int announce();
		/// Called when a new connection is established
		/// to announce and authenticate the peer on the
		/// server.

	virtual void reset();
		/// Resets variables and data at the beginning  
		/// and end of each session.

	virtual void onOnline();
	virtual void onClose();
	virtual bool onPacketCreated(IPacket* packet);
	virtual void onAnnounce(void* sender, TransactionState& state, const TransactionState&);
	*/


	/*
public:
	struct Options 
	{		
		std::string token;
		std::string user;
		std::string group;
		std::string name;
		std::string type;
		Net::Address serverAddr;
		bool secure;

		Options() {
			token		= "";
			user		= "";
			group		= "global";
			name		= "";
			type		= "peer";
			serverAddr	= Net::Address("127.0.0.1", 4000);
			secure		= false;
		}
	};
	*/