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


using namespace std;


namespace scy {
namespace smple {


Client::Client(net::SocketBase* socket, Runner& runner, const Client::Options& options) :
	sockio::Client(socket, runner),
	_options(options),
	_announceStatus(500)
{
	log("trace") << "Creating" << endl;
}


Client::~Client() 
{
	log("trace") << "Destroying" << endl;
	//close();
	//log("trace") << "Destroying: OK" << endl;
}


void Client::connect()
{
	log("trace") << "Connecting" << endl;		
	{
		//Poco::FastMutex::ScopedLock lock(_mutex);
		assert(!_options.user.empty());
		//assert(!_options.token.empty());
		_serverAddr = _options.serverAddr;
	}
	reset();
	sockio::Client::connect();
}


void Client::close()
{
	log("trace") << "Closing" << endl;
	sockio::Client::close();
}


int Client::send(Message& m, bool ack)
{	
	assert(isOnline());
	m.setFrom(ourPeer().address());

	if (m.to().id() == m.from().id())
		throw Exception("Cannot send message with matching sender and recipient.");

	if (!m.valid())
		throw Exception("Cannot send invalid message.");	
	
	//Message m(message);
	//if (m.from() != ourPeer().address())
	//	throw Exception("Cannot send message from another peer.");
	//log("trace") << "Sending Message: " 
	//	<< message.id() << ":\n" 
	//	<< json::stringify(message, true) << endl;
	return sockio::Client::send(m, false);
}


int Client::send(const string data, bool ack)
{
	Message m;
	m.read(data);
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
	log("trace") << "Creating Presence" << endl;

	Peer& peer = ourPeer();
	UpdatePresenceData.emit(this, peer);
	p["data"] = peer;
}


int Client::sendPresence(bool probe)
{
	log("trace") << "Broadcasting Presence" << endl;

	Presence p;
	createPresence(p);
	p.setProbe(probe);
	return send(p);
}


int Client::sendPresence(const Address& to, bool probe)
{
	log("trace") << "Sending Presence" << endl;
	
	Presence p;
	createPresence(p);
	p.setProbe(probe);
	p.setTo(to);
	return send(p);
}


Roster& Client::roster() 
{ 
	//Poco::FastMutex::ScopedLock lock(_mutex);
	return _roster; 
}


/*
Runner& runner() 
{ 
	//Poco::FastMutex::ScopedLock lock(_mutex);
	return _runner; 
}


int Client::announceStatus() const
{
	//Poco::FastMutex::ScopedLock lock(_mutex);
	return _announceStatus;
}
*/


PersistenceT& Client::persistence() 
{ 
	//Poco::FastMutex::ScopedLock lock(_mutex);
	return _persistence; 
}


Client::Options& Client::options() 
{ 
	//Poco::FastMutex::ScopedLock lock(_mutex);
	return _options; 
}


string Client::ourID() const
{
	//Poco::FastMutex::ScopedLock lock(_mutex);
	return _ourID;
}


Peer& Client::ourPeer()
{	
	//Poco::FastMutex::ScopedLock lock(_mutex);
	log("trace") << "Getting Our Peer: " << _ourID << endl;
	if (_ourID.empty())
		throw Exception("No active peer session is available.");
	return *_roster.get(_ourID, true);
}


Client& Client::operator >> (Message& message)
{
	send(message);
	return *this;
}

	
int Client::announce()
{
	json::Value data;
	{
		//Poco::FastMutex::ScopedLock lock(_mutex);
		data["token"]	= _options.token;
		data["group"]	= _options.group;
		data["user"]	= _options.user;
		data["name"]	= _options.name;
		data["type"]	= _options.type;
	}	
	sockio::Packet p("announce", data, true);
	sockio::Transaction* txn = createTransaction(p);
	txn->StateChange += delegate(this, &Client::onAnnounce);
	return txn->send();
}


void Client::onAnnounce(void* sender, net::TransactionState& state, const net::TransactionState&) 
{
	log("trace") << "Announce Response: " << state.toString() << endl;
	
	sockio::Transaction* transaction = reinterpret_cast<sockio::Transaction*>(sender);
	switch (state.id()) {	
	case net::TransactionState::Success:
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

			// Transition our state to Online.
			sockio::Client::onOnline();

			// Broadcast a presence probe to our network.
			sendPresence(true);
		}
		catch (Exception& exc)
		{
			// Set the error message and close the connection.
			setError(exc.displayText());
		}
		break;		

	case net::TransactionState::Failed:
		Announce.emit(this, _announceStatus);
		setError(state.message());
		break;
	}
}


void Client::onOnline()
{
	log("trace") << "On Online" << endl;

	// Override this method because we are not quite
	// ready to transition to Online yet - we still
	// need to announce our presence on the server.
	announce();
}


void Client::onPacket(sockio::Packet& packet) 
{
	log("trace") << "On Packet: " << packet.className() << endl;

	if (packet.type() == sockio::Packet::Message || 
		packet.type() == sockio::Packet::JSON) {
			
		//json::Value data = packet.json();
		json::Value data;
		json::Reader reader;
		if (reader.parse(packet.message(), data)) {
			string type(data["type"].asString());
			log("trace") << "Symple Packet Created: " << type << endl;
			if (type == "message") {
				Message m(data);
				PacketEmitter::emit(this, m);
				if (m.isRequest()) {
					m.setStatus(404);
					log("info") << "Message not handled: " << m.id() << endl;
					respond(m);
				}
			}
			else if (type == "command") {
				Command c(data);
				PacketEmitter::emit(this, c);
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
				PacketEmitter::emit(this, p);
				if (p.isProbe())
					sendPresence(p.from());
			}
		}
	}
	else
		PacketEmitter::emit(this, packet);
}


void Client::onClose()
{
	log("trace") << "Symple Closing" << endl;
	sockio::Client::onClose();
	reset();
}


void Client::reset()
{
	_roster.clear();
	_announceStatus = 500;
	_ourID = "";
}


} } // namespace scy::smple





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
//Client::Client(/* Net::Reactor& reactor, */Runner& runner, const Options& options) : 
//	sockio::Socket(reactor),
//	_runner(runner),
//	_options(options),
//	_announceStatus(500)
//{
//	traceL() << "[smple::Client] Creating" << endl;
//}
//
//
//Client::~Client() 
//{
//	traceL() << "[smple::Client] Destroying" << endl;
//	close();
//	traceL() << "[smple::Client] Destroying: OK" << endl;
//}
//
//
//void Client::connect()
//{
//	traceL() << "[smple::Client] Connecting" << endl;
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
//	traceL() << "[smple::Client] Closing" << endl;
//
//	sockio::Socket::close();
//}
//
//
//int Client::send(const string data)
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
//	traceL() << "[smple::Client] Sending Message: " 
//		<< message.id() << ":\n" 
//		<< json::stringify(message, true) << endl;
//	return sockio::Socket::send(message, false);
//}
//
//
//void Client::createPresence(Presence& p)
//{
//	traceL() << "[smple::Client] Creating Presence" << endl;
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
//	traceL() << "[smple::Client] Broadcasting Presence" << endl;
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
//	traceL() << "[smple::Client] Sending Presence" << endl;
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
//		FastMutex::ScopedLock lock(_mutex);
//		data["token"]	= _options.token;
//		data["user"]	= _options.user;
//		data["name"]	= _options.name;
//		data["type"]	= _options.type;
//	}	
//	sockio::Packet p("announce", data, true);
//	sockio::Transaction* txn = new sockio::Transaction(_runner, *this, p, 1, 5000);
//	txn->StateChange += delegate(this, &Client::onAnnounce);
//	return txn->send();
//}
//
//
//void Client::onAnnounce(void* sender, net::TransactionState& state, const net::TransactionState&) 
//{
//	traceL() << "[smple::Client] Announce Response: " << state.toString() << endl;
//	
//	sockio::Transaction* transaction = reinterpret_cast<sockio::Transaction*>(sender);
//	switch (state.id()) {	
//	case net::TransactionState::Success:
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
//				throw Exception(data["message"].asString()); //"Announce Error: " + 
//
//			_ourID = data["data"]["id"].asString(); //Address();
//			if (_ourID.empty())
//				throw Exception("Invalid server response.");
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
//		catch (Exception& exc)
//		{
//			// Set the error message and close the connection.
//			setError(exc.displayText());
//		}
//		break;		
//
//	case net::TransactionState::Failed:
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
//	traceL() << "[smple::Client] Packet Created: " << packet->className() << endl;
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
//			log("warning") << "[smple::Client] Packet is not a JSON object" << endl;
//			return true; // continue propagation
//		}
//		
//		string type(data["type"].asString());
//		traceL() << "[smple::Client] Packet Created: Symple Type: " << type << endl;
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
//	FastMutex::ScopedLock lock(_mutex);
//	return _roster; 
//}
//
//
//PersistenceT& Client::persistence() 
//{ 
//	FastMutex::ScopedLock lock(_mutex);
//	return _persistence; 
//}
//
//
//Client::Options& Client::options() 
//{ 
//	FastMutex::ScopedLock lock(_mutex);
//	return _options; 
//}
//
//
//string Client::ourID() const
//{
//	FastMutex::ScopedLock lock(_mutex);
//	return _ourID;
//}
//
//
//int Client::announceStatus() const
//{
//	FastMutex::ScopedLock lock(_mutex);
//	return _announceStatus;
//}
//
//
//Peer& Client::ourPeer() //bool whiny
//{	
//	FastMutex::ScopedLock lock(_mutex);
//	traceL() << "[Client:" << this << "] Getting Our Peer: " << _ourID << endl;
//	if (_ourID.empty())
//		throw Exception("No active peer session is available.");
//	return *_roster.get(_ourID, true);
//}
//
//
//Client& Client::operator >> (Message& message)
//{
//	send(message);
//	return *this;
//}