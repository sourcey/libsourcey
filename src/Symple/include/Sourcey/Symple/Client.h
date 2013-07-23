//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// LibSourcey is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//


#ifndef SOURCEY_Symple_Client_H
#define SOURCEY_Symple_Client_H


#include "Sourcey/Flaggable.h"
#include "Sourcey/SocketIO/Client.h"
#include "Sourcey/Net/TCPSocket.h"
#include "Sourcey/Net/SSLSocket.h"
#include "Sourcey/HTTP/WebSocket.h"
#include "Sourcey/Util/TimedManager.h"
#include "Sourcey/Symple/Roster.h"
#include "Sourcey/Symple/Message.h"
#include "Sourcey/Symple/Presence.h"
#include "Sourcey/Symple/Command.h"
#include "Sourcey/Symple/Event.h"
#include "Sourcey/Symple/Form.h"
#include "Sourcey/Symple/Peer.h"


namespace scy {
namespace smpl {

	
typedef TimedManager<std::string, Message> PersistenceT;


// ---------------------------------------------------------------------
//
class Client: public sockio::Client
{
public:
	struct Options 
	{				
		std::string host;
		UInt16 port;

		std::string token;
		std::string user;
		std::string group;
		std::string name;
		std::string type;
		//net::Address serverAddr;

		Options() {
			host = "127.0.0.1";
			port = 4000;

			token		= "";
			user		= "";
			group		= "global";
			name		= "";
			type		= "peer";
			//serverAddr	= net::Address("127.0.0.1", 4000);
		}
	};

public:
	Client(net::SocketBase* socket, const Options& options = Options(), uv::Loop& loop = uv::defaultLoop());
	virtual ~Client();

	void connect();
	void close();

	virtual int send(const std::string data, bool ack = false);
	virtual int send(Message& message, bool ack = false);
		/// Sends a message.

	virtual int respond(Message& message, bool ack = false);
		/// Responds to the given message.
		/// The 'to' and 'from' fields will be swapped.

	virtual int sendPresence(bool probe = false);
		/// Broadcasts presence to the user group scope.

	virtual int sendPresence(const Address& to, bool probe = false);
		/// Sends directed presence to the given peer.
	
	virtual std::string ourID() const;
		/// Returns the session ID of our peer object.

    virtual Peer& ourPeer();
		/// Returns the peer object that controls the
		/// current session, or throws an exception.

	virtual Roster& roster();
		/// Returns the roster which stores all online peers.

	virtual PersistenceT& persistence();
		/// Returns the persistence manager which stores
		/// long lived messages.

	virtual Client::Options& options();
		/// Returns a reference to the options object.

	virtual Client& operator >> (Message& message);	
		/// Stream operator alias for send().

	Signal<const int&> Announce;
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

	virtual const char* className() const { return "SympleClient"; }
	
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
	
	//virtual void onOnline();
	virtual void onClose();
	
	virtual void onSocketConnect(void*);
	virtual void onAnnounce(void* sender, TransactionState& state, const TransactionState&);
	virtual void onPacket(sockio::Packet& packet);

protected:	
	//mutable Mutex	_mutex;

	Roster _roster;
	std::string _ourID;
	PersistenceT _persistence;
	Client::Options _options;
	int _announceStatus;
};


// ---------------------------------------------------------------------
//
class TCPClient: public Client
{
public:
	TCPClient(const Client::Options& options = Client::Options(), uv::Loop& loop = uv::defaultLoop()) :
		Client(new net::TCPBase, options, loop)
	{
	}
};


// ---------------------------------------------------------------------
//
class SSLClient: public Client
{
public:
	SSLClient(const Client::Options& options = Client::Options(), uv::Loop& loop = uv::defaultLoop()) :
		Client(new net::TCPBase, options, loop)
	{
	}
};


// ---------------------------------------------------------------------
//
inline Client* createTCPClient(const Client::Options& options = Client::Options(), uv::Loop& loop = uv::defaultLoop())
{
	return new Client(new net::TCPBase, options, loop);
}


// ---------------------------------------------------------------------
//
inline Client* createSSLClient(const Client::Options& options = Client::Options(), uv::Loop& loop = uv::defaultLoop())
{
	return new Client(new net::SSLBase, options, loop);
}


// ---------------------------------------------------------------------
//
enum FilterFlags 
{
	AcceptRequests		= 0x01, 
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
			(!filter.has(AcceptRequests) || 
				(filter.has(AcceptRequests) && packet->isRequest())) &&
			(!filter.has(AcceptResponses) || 
				(filter.has(AcceptResponses) && !packet->isRequest()))) {
			return true;
		}	
		return false;
	}
};


DefinePolymorphicDelegateWithArg(messageDelegate, IPacket, MessageDelegate, const Filter&, Filter())


// ---------------------------------------------------------------------
//
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


// ---------------------------------------------------------------------
//
struct PresenceDelegate: public MessageDelegate
{
	PresenceDelegate() : MessageDelegate(AcceptRequests) {};
	PresenceDelegate(const PresenceDelegate& r) : MessageDelegate(r) {};

	virtual bool accepts(void* sender, IPacket& data, Void empty2, Void empty3, Void empty4) 
	{
		if (MessageDelegate::accepts(sender, data, empty2, empty3, empty4)) {
			Presence* p = dynamic_cast<Presence*>(&data);
			return p && p->type() == "presence";
		}
		return false;
	}
};


DefinePolymorphicDelegate(presenceDelegate, IPacket, PresenceDelegate)


// ---------------------------------------------------------------------
//
struct EventDelegate: public MessageDelegate
{
	EventDelegate() : MessageDelegate(AcceptRequests) {};
	EventDelegate(const EventDelegate& r) : MessageDelegate(r) {};

	virtual bool accepts(void* sender, IPacket& data, Void empty2, Void empty3, Void empty4) 
	{
		if (MessageDelegate::accepts(sender, data, empty2, empty3, empty4)) {
			Event* e = dynamic_cast<Event*>(&data);
			return e && e->type() == "event" && 
				(filter.path.empty() || e->name() == filter.path);
		}
		return false;
	}
};


DefinePolymorphicDelegate(eventDelegate, IPacket, EventDelegate)


} } // namespace scy::smpl


#endif //  SOURCEY_Symple_Client_H





/*


// ---------------------------------------------------------------------
//
template <class WebSocketBaseT>
class ClientBase: public Client
{
public:
	ClientBase(const Client::Options& options = Client::Options(), uv::Loop& loop = uv::defaultLoop(),) :
		_socket(runner),
		Client(_socket, runner, options)
	{
	}

protected:
	WebSocketBaseT _socket;
};


// ---------------------------------------------------------------------
//
typedef smpl::ClientBase< 
	Net::WebSocketBase< 
		Net::StatefulSocketBase< 
			Net::SocketBase< Poco::Net::StreamSocket, Net::TCP, http::WebSocket >
		> 
	> 
> TCPClient;


// ---------------------------------------------------------------------
//
typedef smpl::ClientBase< 
	Net::WebSocketBase< 
		Net::StatefulSocketBase< 
			Net::SocketBase< Poco::Net::SecureStreamSocket, Net::SSLTCP, http::WebSocket >
		> 
	> 
> SSLClient;
*/
	


/*
// ---------------------------------------------------------------------
typedef smpl::Client< 
	sockio::SocketBase< 
		Net::WebSocketBase< 
			Net::StatefulSocketBase< 
				Net::SocketBase< ::TCPContext, smpl::IClient > 
			> 
		> 
	> 
> Client;


typedef smpl::Client< 
	sockio::SocketBase< 
		Net::WebSocketBase< 
			Net::StatefulSocketBase< 
				Net::SocketBase< ::SSLContext, smpl::IClient > 
			> 
		> 
	> 
> SSLClient;
*/
	//virtual void onError();
	//virtual int announceStatus() const;
		/// Returns a reference to the options object.


	//uv::Loop& _loop;
/*
// ---------------------------------------------------------------------
class IClient: public sockio::Client
	/// Maximum simplicity. Maximum scope.
{
public:

public:
	//Client(uv::Loop& loop, const Options& options = Options()); 
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
	virtual uv::Loop& loop() = 0;
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
template <class sockio::Client>
class Client: public sockio::Client
{
public:
	Client(uv::Loop& loop, const Client::Options& options = Client::Options()) : 
		sockio::Client(reactor),
		_runner(runner),
		_options(options),
		_announceStatus(500)
	{
		traceL() << "[smpl::Client:" << this << "] Creating" << std::endl;
	}


	virtual ~Client() 
	{
		traceL() << "[smpl::Client:" << this << "] Destroying" << std::endl;
		close();
		traceL() << "[smpl::Client:" << this << "] Destroying: OK" << std::endl;
	}


	void connect()
	{
		traceL() << "[smpl::Client:" << this << "] Connecting" << std::endl;
		
		{
			//Mutex::ScopedLock lock(_mutex);
			assert(!_options.user.empty());
			//assert(!_options.token.empty());
			_srvAddr = _options.serverAddr;
		}
		reset();
		sockio::Client::connect();
	}


	void close()
	{
		traceL() << "[smpl::Client:" << this << "] Closing" << std::endl;

		sockio::Client::close();
	}


	virtual int send(const std::string data)
	{
		assert(isOnline());
		return sockio::Client::send(sockio::Packet::Message, data, false);
	}


	virtual int send(Message& message, bool ack = false)
	{	
		assert(isOnline());
		message.setFrom(ourPeer().address());
		assert(message.valid());
		assert(message.to().id() != message.from().id());
		traceL() << "[smpl::Client:" << this << "] Sending Message: " 
			<< message.id() << ":\n" 
			<< json::stringify(message, true) << std::endl;
		return sockio::Client::send(message, false);
	}


	virtual void createPresence(Presence& p)
	{
		traceL() << "[smpl::Client:" << this << "] Creating Presence" << std::endl;

		Peer& peer = ourPeer();
		UpdatePresenceData.emit(this, peer);
		p["data"] = peer;
	}


	virtual int sendPresence(bool probe = false)
	{
		traceL() << "[smpl::Client:" << this << "] Broadcasting Presence" << std::endl;

		Presence p;
		createPresence(p);
		p.setProbe(probe);
		return send(p);
	}


	virtual int sendPresence(const Address& to, bool probe = false)
	{
		traceL() << "[smpl::Client:" << this << "] Sending Presence" << std::endl;
	
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
		//Mutex::ScopedLock lock(_mutex);
		return _roster; 
	}


	virtual uv::Loop& loop() 
	{ 
		//Mutex::ScopedLock lock(_mutex);
		return _runner; 
	}


	virtual PersistenceT& persistence() 
	{ 
		//Mutex::ScopedLock lock(_mutex);
		return _persistence; 
	}


	virtual Client::Options& options() 
	{ 
		//Mutex::ScopedLock lock(_mutex);
		return _options; 
	}


	virtual std::string ourID() const
	{
		//Mutex::ScopedLock lock(_mutex);
		return _ourID;
	}


	virtual int announceStatus() const
	{
		//Mutex::ScopedLock lock(_mutex);
		return _announceStatus;
	}


	virtual Peer& ourPeer()
	{	
		//Mutex::ScopedLock lock(_mutex);
		traceL() << "[smpl::Client:" << this << "] Getting Our Peer: " << _ourID << std::endl;
		if (_ourID.empty())
			throw Exception("No active peer session is available.");
		return *_roster.get(_ourID, true);
	}


	virtual IClient& operator >> (Message& message)
	{
		send(message);
		return *this;
	}
	

	virtual const char* className() const { return "smpl::Client"; }

	
protected:	
	virtual int announce()
	{
		json::Value data;
		{
			//Mutex::ScopedLock lock(_mutex);
			data["token"]	= _options.token;
			data["group"]	= _options.group;
			data["user"]	= _options.user;
			data["name"]	= _options.name;
			data["type"]	= _options.type;
		}	
		sockio::Packet p("announce", data, true);
		sockio::Transaction* txn = new sockio::Transaction(_runner, *this, p, 1, 5000);
		txn->StateChange += delegate(this, &Client::onAnnounce);
		return txn->send();
	}


	virtual void onAnnounce(void* sender, TransactionState& state, const TransactionState&) 
	{
		traceL() << "[smpl::Client:" << this << "] Announce Response: " << state.toString() << std::endl;
	
		sockio::Transaction* transaction = reinterpret_cast<sockio::Transaction*>(sender);
		switch (state.id()) {	
		case TransactionState::Success:
			try 
			{
				json::Value data = transaction->response().json()[(size_t)0];
				_announceStatus = data["status"].asInt();

				// Notify the outside application of the response 
				// status before we transition the client state.
				Announce.emit(this, _announceStatus);

				if (_announceStatus != 200)
					throw Exception(data["message"].asString()); //"Announce Error: " + 

				_ourID = data["data"]["id"].asString(); //Address();
				if (_ourID.empty())
					throw Exception("Invalid server response.");

				// Set our local peer data from the response or fail.
				_roster.update(data["data"], true);

				// Transition our state on Online.
				sockio::Client::onOnline();

				// Broadcast a presence probe to our network.
				sendPresence(true);
			}
			catch (Exception& exc)
			{
				// Set the error message and close the connection.
				setError(exc.message());
			}
			break;		

		case TransactionState::Failed:
			Announce.emit(this, _announceStatus);
			setError(state.message());
			break;
		}
	}


	virtual void onOnline()
	{
		traceL() << "[smpl::Client:" << this << "] On Online" << std::endl;

		// Override this method because we are not quite
		// ready to transition to Online yet - we still
		// need to announce our presence on the server.
		announce();
	}


	virtual bool onPacketCreated(IPacket* packet) 
	{
		traceL() << "[smpl::Client:" << this << "] Packet Created: " << packet->className() << std::endl;

		// Catch incoming messages here so we can parse
		// messages and handle presence updates.

		sockio::Packet* p = dynamic_cast<sockio::Packet*>(packet);
		if (p && (
			p->type() == sockio::Packet::Message || 
			p->type() == sockio::Packet::JSON)) {

			json::Value data = p->json();
			if (!data.isObject() || data.isNull()) {
				Log("warning") << "[smpl::Client:" << this << "] Packet is not a JSON object" << std::endl;
				return true; // continue propagation
			}
		
			string type(data["type"].asString());
			traceL() << "[smpl::Client:" << this << "] Packet Created: Symple Type: " << type << std::endl;
			if (type == "message") {
				Message m(data);
				emit(this, m);
				return false; // stop propagation
			}
			else if (type == "command") {
				Command c(data);
				emit(this, c);
				return false; // stop propagation
			}
			else if (type == "presence") {
				Presence p(data);
				if (p.isMember("data"))
					_roster.update(p["data"], false);
				emit(this, p);
				if (p.isProbe())
					sendPresence(p.from());
				return false; // stop propagation
			}
		}
		return true;
	}


	virtual void onClose()
	{
		traceL() << "[[smpl::Client:" << this << "] Closing" << std::endl;
		sockio::Client::onClose();
		reset();
	}


	virtual void onError()
	{
		sockio::Client::onClose();
		reset();
	}


	void reset()
	{
		_roster.clear();
		_announceStatus = 500;
		_ourID = "";
	}
	

protected:	
	//mutable Mutex	_mutex;

	Roster _roster;
	uv::Loop& _loop;
	std::string _ourID;
	PersistenceT _persistence;
	Client::Options _options;
	int _announceStatus;
};


// ---------------------------------------------------------------------
typedef smpl::Client< 
	sockio::SocketBase< 
		Net::WebSocketBase< 
			Net::StatefulSocketBase< 
				Net::SocketBase< ::TCPContext, smpl::IClient > 
			> 
		> 
	> 
> Client;


typedef smpl::Client< 
	sockio::SocketBase< 
		Net::WebSocketBase< 
			Net::StatefulSocketBase< 
				Net::SocketBase< ::SSLContext, smpl::IClient > 
			> 
		> 
	> 
> SSLClient;
*/




//typedef smpl::Client<  > Client; //, smpl::IClient //smpl::SocketBase< ::TCPContext>
//typedef smpl::Client< sockio::SocketBase< Net::StatefulSocketBase< Net::SocketBase< ::SSLContext, smpl::IClient> > > > SSLClient; //, smpl::IClient //smpl::SocketBase< ::SSLContext>
//sockio::SSLSocket
// Net::SocketBase< ::SSLContext, sockio::Client>

 //, public smpl::IClient
//class Client: public sockio::Client
	/// Maximum simplicity. Maximum scope.//BaseT

/*

	Client(uv::Loop& loop, const Options& options = Options()); 
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
	uv::Loop& loop();
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
		net::Address serverAddr;
		bool secure;

		Options() {
			token		= "";
			user		= "";
			group		= "global";
			name		= "";
			type		= "peer";
			serverAddr	= net::Address("127.0.0.1", 4000);
			secure		= false;
		}
	};
	*/