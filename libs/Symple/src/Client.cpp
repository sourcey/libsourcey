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


#include "Sourcey/Symple/Client.h"
#include "Sourcey/Symple/Message.h"
#include "Sourcey/Logger.h"


using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Sourcey::Net;


namespace Sourcey {
namespace Symple {


Client::Client(Net::Reactor& reactor, Runner& runner, const Options& options) : 
	SocketIO::Socket(reactor),
	_runner(runner),
	_options(options),
	_announceStatus(500)
{
	Log("trace") << "[Symple::Client] Creating" << endl;
}


Client::~Client() 
{
	Log("trace") << "[Symple::Client] Destroying" << endl;
	close();
	Log("trace") << "[Symple::Client] Destroying: OK" << endl;
}


void Client::connect()
{
	Log("debug") << "[Symple::Client] Connecting" << endl;

	assert(!_options.user.empty());
	assert(!_options.token.empty());
	cleanup();
	SocketIO::Socket::connect(_options.serverAddr);
}


void Client::close()
{
	Log("debug") << "[Symple::Client] Closing" << endl;

	SocketIO::Socket::close();
}


int Client::send(const std::string data)
{
	assert(isOnline());
	return SocketIO::Socket::send(SocketIO::Packet::Message, data, false);
}


int Client::send(Message& message, bool ack)
{	
	assert(isOnline());
	message.setFrom(ourPeer().address());
	assert(message.valid());
	assert(message.to().id() != message.from().id());
	Log("debug") << "[Symple::Client] Sending Message: " 
		<< message.id() << ":\n" 
		<< JSON::stringify(message, true) << endl;
	return SocketIO::Socket::send(message, false);
}


void Client::createPresence(Presence& p)
{
	Log("debug") << "[Symple::Client] Creating Presence" << endl;

	Peer& peer = /*_roster.*/ourPeer();
	UpdatePresenceData.dispatch(this, peer);
	//p.setFrom(peer.address()); //ourPeer());
	p["data"] = peer;
}


int Client::sendPresence(bool probe)
{
	Log("debug") << "[Symple::Client] Broadcasting Presence" << endl;

	Presence p;
	createPresence(p);
	p.setProbe(probe);
	return send(p);
}


int Client::sendPresence(const Address& to, bool probe)
{
	Log("debug") << "[Symple::Client] Sending Presence" << endl;
	
	Presence p;
	createPresence(p);
	p.setProbe(probe);
	p.setTo(to); //*roster().get(to.id(), true));
	return send(p);
}


int Client::respond(Message& message)
{
	message.setTo(message.from());
	return send(message);
}


int Client::announce()
{
	JSON::Value data;
	{
		FastMutex::ScopedLock lock(_mutex);
		data["token"]	= _options.token;
		data["user"]	= _options.user;
		data["name"]	= _options.name;
		data["type"]	= _options.type;
	}	
	SocketIO::Packet p("announce", data, true);
	SocketIO::Transaction* txn = new SocketIO::Transaction(_runner, *this, p, 1, 5000);
	txn->StateChange += delegate(this, &Client::onAnnounce);
	return txn->send();
}


void Client::onAnnounce(void* sender, TransactionState& state, const TransactionState&) 
{
	Log("debug") << "[Symple::Client] Announce Response: " << state.toString() << endl;
	
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
			SocketIO::Socket::onOnline();

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
		Announce.dispatch(this, _announceStatus);
		setError(state.message());
		break;
	}
}


void Client::cleanup()
{
	_roster.clear();
	_announceStatus = 500;
	_ourID = "";
}


void Client::onClose()
{
	SocketIO::Socket::onClose();
	cleanup();
}


void Client::onOnline()
{
	// Override this method because we are not quite
	// ready to transition to Online yet - we still
	// need to announce our presence on the server.
	announce();
}


bool Client::onPacketCreated(IPacket* packet) 
{
	Log("trace") << "[Symple::Client] Packet Created: " << packet->className() << endl;
	//packet->print(cout);

	// Catch incoming messages here so we can parse
	// messages and handle presence updates.

	SocketIO::Packet* p = dynamic_cast<SocketIO::Packet*>(packet);
	if (p && (
		p->type() == SocketIO::Packet::Message || 
		p->type() == SocketIO::Packet::JSON)) {

		JSON::Value data = p->json();
		if (!data.isObject() || data.isNull()) {
			Log("warning") << "[Symple::Client] Packet is not a JSON object" << endl;
			return true; // continue propagation
		}
		
		string type(data["type"].asString());
		Log("trace") << "[Symple::Client] Packet Created: Symple Type: " << type << endl;
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


Roster& Client::roster() 
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _roster; 
}


PersistenceT& Client::persistence() 
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _persistence; 
}


Client::Options& Client::options() 
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _options; 
}


string Client::ourID() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _ourID;
}


int Client::announceStatus() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _announceStatus;
}


Peer& Client::ourPeer() //bool whiny
{	
	FastMutex::ScopedLock lock(_mutex);
	Log("debug") << "[Client::" << this << "] Getting Our Peer: " << _ourID << endl;
	if (_ourID.empty())
		throw Exception("No active peer session is available.");
	return *_roster.get(_ourID, true);
}


Client& Client::operator >> (Message& message)
{
	send(message);
	return *this;
}


} } // namespace Sourcey::Symple