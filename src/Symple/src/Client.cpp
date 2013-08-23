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


#include "Sourcey/Symple/Client.h"
#include "Sourcey/Net/TCPSocket.h"
#include "Sourcey/Net/SSLSocket.h"


using namespace std;


namespace scy {
namespace smpl {


//
// TCP Client
//


Client* createTCPClient(const Client::Options& options, uv::Loop& loop)
{
	return new Client(new net::TCPBase, options, loop);
}


TCPClient::TCPClient(const Client::Options& options, uv::Loop& loop) :
	Client(new net::TCPBase, options, loop)
{
}


//
// SSL Client
//
	

Client* createSSLClient(const Client::Options& options, uv::Loop& loop)
{
	return new Client(new net::SSLBase, options, loop);
}


SSLClient::SSLClient(const Client::Options& options, uv::Loop& loop) :
	Client(new net::SSLBase, options, loop)
{
}


//
// Client Implementation
//


Client::Client(net::SocketBase* socket, const Client::Options& options, uv::Loop& loop) :
	sockio::Client(socket, loop),
	_options(options),
	_announceStatus(500)
{
	log("trace") << "creating" << endl;
}


Client::~Client() 
{
	log("trace") << "Destroy" << endl;
}


void Client::connect()
{
	log("trace") << "connecting" << endl;		
	{
		//ScopedLock lock(_mutex);
		assert(!_options.user.empty());
		//assert(!_options.token.empty());
		//_serverAddr = _options.serverAddr;
		_host = _options.host;
		_port = _options.port;
	}
	reset();
	sockio::Client::connect();
}


void Client::close()
{
	log("trace") << "closing" << endl;
	sockio::Client::close();
}


int Client::send(Message& m, bool ack)
{	
	//assert(isOnline()); // may be announcing
	m.setFrom(ourPeer().address());

	if (m.to().id() == m.from().id())
		throw std::runtime_error("Cannot send message with matching sender and recipient.");

	if (!m.valid())
		throw std::runtime_error("Cannot send invalid message.");	
	
	//Message m(message);
	//if (m.from() != ourPeer().address())
	//	throw std::runtime_error("Cannot send message from another peer.");
	log("trace") << "sending message: " 
		<< m.id() << ":\n" 
		<< json::stringify(m, true) << endl;
	return sockio::Client::send(m, ack);
}


int Client::send(const std::string& data, bool ack)
{
	Message m;
	if (!m.read(data))
		throw std::runtime_error("Cannot send malformed message.");
	return send(data, ack);
}


int Client::respond(Message& m, bool ack)
{
	m.setTo(m.from());
	//m.setFrom(ourPeer().address());	
	//assert(isOnline());	
	//assert(m.valid());
	//assert(m.to().id() != m.from().id());
	//log("trace") << "Responding Message: " 
	//	<< m.id() << ":\n" 
	//	<< json::stringify(m, true) << endl;
	return send(m, ack);
}


void Client::createPresence(Presence& p)
{
	log("trace") << "creating presence" << endl;

	Peer& peer = ourPeer();
	UpdatePresenceData.emit(this, peer);
	p["data"] = peer;
}


int Client::sendPresence(bool probe)
{
	log("trace") << "broadcasting presence" << endl;

	Presence p;
	createPresence(p);
	p.setProbe(probe);
	return send(p);
}


int Client::sendPresence(const Address& to, bool probe)
{
	log("trace") << "sending presence" << endl;
	
	Presence p;
	createPresence(p);
	p.setProbe(probe);
	p.setTo(to);
	return send(p);
}


Roster& Client::roster() 
{ 
	//ScopedLock lock(_mutex);
	return _roster; 
}


PersistenceT& Client::persistence() 
{ 
	//ScopedLock lock(_mutex);
	return _persistence; 
}


Client::Options& Client::options() 
{ 
	//ScopedLock lock(_mutex);
	return _options; 
}


string Client::ourID() const
{
	//ScopedLock lock(_mutex);
	return _ourID;
}


Peer& Client::ourPeer()
{	
	//ScopedLock lock(_mutex);
	if (_ourID.empty())
		throw std::runtime_error("No active peer session is available.");
	return *_roster.get(_ourID, true);
}


Client& Client::operator >> (Message& message)
{
	send(message);
	return *this;
}

	
int Client::announce()
{
	log("trace") << "announcing" << endl;

	json::Value data;
	{
		//ScopedLock lock(_mutex);
		data["token"]	= _options.token;
		data["group"]	= _options.group;
		data["user"]	= _options.user;
		data["name"]	= _options.name;
		data["type"]	= _options.type;
	}	
	sockio::Packet p("announce", data, true);
	auto txn = createTransaction(p);
	txn->StateChange += delegate(this, &Client::onAnnounce);
	return txn->send();
}


void Client::onAnnounce(void* sender, TransactionState& state, const TransactionState&) 
{
	log("trace") << "Announce Response: " << state.toString() << endl;
	
	auto transaction = reinterpret_cast<sockio::Transaction*>(sender);
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
				throw std::runtime_error(data["message"].asString()); //"Announce Error: " + 

			_ourID = data["data"]["id"].asString(); //Address();
			if (_ourID.empty())
				throw std::runtime_error("Invalid server response.");

			// Set our local peer data from the response or fail.
			_roster.update(data["data"], true);

			// Transition to Online state.
			sockio::Client::onOnline();

			// Broadcast a presence probe to our network.
			sendPresence(true);
		}
		catch (std::exception&/*Exception&*/ exc)
		{
			// Set the error message and close the connection.
			setError(exc.what()/*message()*/);
		}
		break;		

	case TransactionState::Failed:
		Announce.emit(this, _announceStatus);
		setError(state.message());
		break;
	}
}


void Client::onSocketConnect(void*)
{
	// Start the socket.io timers etc
	sockio::Client::onConnect();

	// Authorize the symple connection	
	announce();
}


/*
void Client::onOnline()
{
	log("trace") << "On connect" << endl;

	// Override this method because we are not quite
	// ready to transition to online yet state - we 
	// still need to announce our presence on the server.
	announce();
}
*/


void Client::onPacket(sockio::Packet& packet) 
{
	// Parse Symple messages from SocketIO JSON packets
	if (packet.type() == sockio::Packet::Message || 
		packet.type() == sockio::Packet::JSON) {	

		json::Value data;
		json::Reader reader;
		if (reader.parse(packet.message(), data)) {
			std::string type(data["type"].asString());
			log("trace") << "Symple packet created: " << type << endl;
			if (type == "message") {
				Message m(data);
				PacketSignal::emit(this, m);
				if (m.isRequest()) {
					m.setStatus(404);
					log("info") << "Message not handled: " << m.id() << endl;
					respond(m);
				}
			}
			else if (type == "command") {
				Command c(data);
				PacketSignal::emit(this, c);
				if (c.isRequest()) {
					c.setStatus(404);
					log("info") << "Command not handled: " << c.id() << ": " << c.node() << endl;
					respond(c);
				}
			}
			else if (type == "presence") {
				Presence p(data);
				if (p.isMember("data"))
					_roster.update(p["data"], false);
				PacketSignal::emit(this, p);
				if (p.isProbe())
					sendPresence(p.from());
			}
		}
		else assert(0 && "invalid packet");
	}

	// Other packet types are proxied directly
	else {		
		PacketSignal::emit(this, packet);
	}
}


void Client::onClose()
{
	log("trace") << "Symple closing" << endl;
	sockio::Client::onClose();
	reset();
}


void Client::reset()
{
	_roster.clear();
	_announceStatus = 500;
	_ourID = "";
}


} } // namespace scy::smpl





/*
uv::Loop& loop() 
{ 
	//ScopedLock lock(_mutex);
	return _runner; 
}


int Client::announceStatus() const
{
	//ScopedLock lock(_mutex);
	return _announceStatus;
}
*/


/*
void Client::onError()
{
	sockio::Client::onError();
	reset();
}
*/

			
		//sockio::Packet* p = dynamic_cast<sockio::Packet*>(packet);
		//json::Value data = packet.json();
		//if (!data.isObject() || data.isNull()) {
		//	log("warning") << "Packet is not a JSON object" << endl;
		//	return true; // continue propagation
		//}
			//return false; // stop propagation
			//return false; // stop propagation
			//return false; // stop propagation
//Client::Client(uv::Loop& loop, const Options& options) : 
//	sockio::Socket(reactor),
//	_runner(runner),
//	_options(options),
//	_announceStatus(500)
//{
//	traceL() << "[smpl::Client] Creating" << endl;
//}
//
//
//Client::~Client() 
//{
//	traceL() << "[smpl::Client] Destroying" << endl;
//	close();
//	traceL() << "[smpl::Client] Destroying: OK" << endl;
//}
//
//
//void Client::connect()
//{
//	traceL() << "[smpl::Client] Connecting" << endl;
//
//	assert(!_options.user.empty());
//	assert(!_options.token.empty());
//	cleanup();
//	if (_options.secure)
//		sockio::Socket::setSecure(true);
//	sockio::Socket::connect(_options.serverAddr);
//}
//
//
//void Client::close()
//{
//	traceL() << "[smpl::Client] Closing" << endl;
//
//	sockio::Socket::close();
//}
//
//
//int Client::send(const std::string& data)
//{
//	assert(isOnline());
//	return sockio::Socket::send(sockio::Packet::Message, data, false);
//}
//
//
//int Client::send(Message& message, bool ack)
//{	
//	assert(isOnline());
//	message.setFrom(ourPeer().address());
//	assert(message.valid());
//	assert(message.to().id() != message.from().id());
//	traceL() << "[smpl::Client] Sending Message: " 
//		<< message.id() << ":\n" 
//		<< json::stringify(message, true) << endl;
//	return sockio::Socket::send(message, false);
//}
//
//
//void Client::createPresence(Presence& p)
//{
//	traceL() << "[smpl::Client] Creating Presence" << endl;
//
//	Peer& peer = /*_roster.*/ourPeer();
//	UpdatePresenceData.emit(this, peer);
//	//p.setFrom(peer.address()); //ourPeer());
//	p["data"] = peer;
//}
//
//
//int Client::sendPresence(bool probe)
//{
//	traceL() << "[smpl::Client] Broadcasting Presence" << endl;
//
//	Presence p;
//	createPresence(p);
//	p.setProbe(probe);
//	return send(p);
//}
//
//
//int Client::sendPresence(const Address& to, bool probe)
//{
//	traceL() << "[smpl::Client] Sending Presence" << endl;
//	
//	Presence p;
//	createPresence(p);
//	p.setProbe(probe);
//	p.setTo(to); //*roster().get(to.id(), true));
//	return send(p);
//}
//
//
//int Client::respond(Message& message)
//{
//	message.setTo(message.from());
//	return send(message);
//}
//
//
//int Client::announce()
//{
//	json::Value data;
//	{
//		Mutex::ScopedLock lock(_mutex);
//		data["token"]	= _options.token;
//		data["user"]	= _options.user;
//		data["name"]	= _options.name;
//		data["type"]	= _options.type;
//	}	
//	sockio::Packet p("announce", data, true);
//	auto txn = new sockio::Transaction(_runner, *this, p, 1, 5000);
//	txn->StateChange += delegate(this, &Client::onAnnounce);
//	return txn->send();
//}
//
//
//void Client::onAnnounce(void* sender, TransactionState& state, const TransactionState&) 
//{
//	traceL() << "[smpl::Client] Announce Response: " << state.toString() << endl;
//	
//	auto transaction = reinterpret_cast<sockio::Transaction*>(sender);
//	switch (state.id()) {	
//	case TransactionState::Success:
//		try 
//		{
//			json::Value data = transaction->response().json()[(size_t)0];
//			_announceStatus = data["status"].asInt();
//
//			// Notify the outside application of the response 
//			// status before we transition the client state.
//			Announce.emit(this, _announceStatus);
//
//			if (_announceStatus != 200)
//				throw std::runtime_error(data["message"].asString()); //"Announce Error: " + 
//
//			_ourID = data["data"]["id"].asString(); //Address();
//			if (_ourID.empty())
//				throw std::runtime_error("Invalid server response.");
//
//			// Set our local peer data from the response or fail.
//			_roster.update(data["data"], true);
//
//			// Transition our state on Online.
//			sockio::Socket::onOnline();
//
//			// Broadcast a presence probe to our network.
//			sendPresence(true);
//		}
//		catch (std::exception&/*Exception&*/ exc)
//		{
//			// Set the error message and close the connection.
//			setError(exc.what()/*message()*/);
//		}
//		break;		
//
//	case TransactionState::Failed:
//		Announce.emit(this, _announceStatus);
//		setError(state.message());
//		break;
//	}
//}
//
//
//void Client::cleanup()
//{
//	_roster.clear();
//	_announceStatus = 500;
//	_ourID = "";
//}
//
//
//void Client::onClose()
//{
//	sockio::Socket::onClose();
//	cleanup();
//}
//
//
//void Client::onOnline()
//{
//	// Override this method because we are not quite
//	// ready to transition to Online yet - we still
//	// need to announce our presence on the server.
//	announce();
//}
//
//
//bool Client::onPacketCreated(IPacket* packet) 
//{
//	traceL() << "[smpl::Client] Packet Created: " << packet->className() << endl;
//
//	// Catch incoming messages here so we can parse
//	// messages and handle presence updates.
//
//	sockio::Packet* p = dynamic_cast<sockio::Packet*>(packet);
//	if (p && (
//		p->type() == sockio::Packet::Message || 
//		p->type() == sockio::Packet::JSON)) {
//
//		json::Value data = p->json();
//		if (!data.isObject() || data.isNull()) {
//			log("warning") << "[smpl::Client] Packet is not a JSON object" << endl;
//			return true; // continue propagation
//		}
//		
//		std::string type(data["type"].asString());
//		traceL() << "[smpl::Client] Packet Created: Symple Type: " << type << endl;
//		if (type == "message") {
//			Message m(data);
//			emit(this, m);
//			return false; // stop propagation
//		}
//		else if (type == "command") {
//			Command c(data);
//			emit(this, c);
//			return false; // stop propagation
//		}
//		else if (type == "presence") {
//			Presence p(data);
//			if (p.isMember("data"))
//				_roster.update(p["data"], false);
//			emit(this, p);
//			if (p.isProbe())
//				sendPresence(p.from());
//			return false; // stop propagation
//		}
//	}
//	return true;
//}
//
//
//Roster& Client::roster() 
//{ 
//	Mutex::ScopedLock lock(_mutex);
//	return _roster; 
//}
//
//
//PersistenceT& Client::persistence() 
//{ 
//	Mutex::ScopedLock lock(_mutex);
//	return _persistence; 
//}
//
//
//Client::Options& Client::options() 
//{ 
//	Mutex::ScopedLock lock(_mutex);
//	return _options; 
//}
//
//
//string Client::ourID() const
//{
//	Mutex::ScopedLock lock(_mutex);
//	return _ourID;
//}
//
//
//int Client::announceStatus() const
//{
//	Mutex::ScopedLock lock(_mutex);
//	return _announceStatus;
//}
//
//
//Peer& Client::ourPeer() //bool whiny
//{	
//	Mutex::ScopedLock lock(_mutex);
//	traceL() << "[Client: " << this << "] Getting Our Peer: " << _ourID << endl;
//	if (_ourID.empty())
//		throw std::runtime_error("No active peer session is available.");
//	return *_roster.get(_ourID, true);
//}
//
//
//Client& Client::operator >> (Message& message)
//{
//	send(message);
//	return *this;
//}