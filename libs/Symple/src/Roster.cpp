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


#include "Sourcey/Symple/Roster.h"
#include "Sourcey/Logger.h"


using namespace std;
using namespace Poco;


namespace Sourcey {
namespace Symple {


Roster::Roster()
{	
	//Log("debug") << "[Roster::" << this << "] Creating" << endl;
}
	

Roster::~Roster() 
{
	//Log("debug") << "[Roster::" << this << "] Destroying" << endl;
}


void Roster::setOurID(const ID& id)
{
	FastMutex::ScopedLock lock(_mutex);
	assert(id.valid());
	_ourID = id;
}


ID Roster::ourID() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _ourID;
}


Peer* Roster::ourPeer(bool whiny)
{	
	string id;
	{
		FastMutex::ScopedLock lock(_mutex);
		Log("debug") << "[Roster::" << this << "] Getting Our Peer: " << _ourID.toString() << endl;
		if (!_ourID.valid()) {
			if (whiny)
				throw Exception("No active peer is available");
			else return NULL;
		}
		id = _ourID.toString();
		Log("debug") << "[Roster::" << this << "] Getting Our Peer: 1: " << _ourID.toString() << endl;
	}
	return Roster::Manager::get(id, whiny);
}


void Roster::update(const JSON::Value& data, bool whiny)
{
	if (data.isObject() &&
		data.isMember("id") && 
		data.isMember("user") && 
		data.isMember("name") && 
		data.isMember("type")) {
		Log("debug") << "[Roster::" << this << "] Updating: " << JSON::stringify(data, true) << endl;
		string id = data["id"].asString();
		Peer* peer = get(id, false);
		if (!peer) {
			peer = new Peer(data);
			add(id, peer);
		} else
			static_cast<JSON::Value&>(*peer) = data;
	}
	else if (data.isArray()) {
		for (JSON::ValueIterator it = data.begin(); it != data.end(); it++) {
			update(*it, whiny);	
		}
	}
	else {
		string error("Bad roster item data: " + JSON::stringify(data));
		Log("error") << "[Roster::" << this << "] " << error << endl;	
		if (whiny)
			throw Exception(error);
	}
}


/*
Roster::~Roster() 
{
	//Log("debug") << "[Roster::" << this << "] Destroying" << endl;
	closeAll();
	Log("debug") << "[Roster::" << this << "] Destroying: OK" << endl;
}


void Roster::closeAll()
{
	FastMutex::ScopedLock lock(_mutex);
	Log("debug") << "[Roster::" << this << "] Closing All Peers: " << _items.size() << endl;
	
	PeerMap::iterator it = _items.begin();
	PeerMap::iterator it2;
	while (it != _items.end()) {
		it2 = it++;
		Log("debug") << "[Roster::" << this << "] Closing: " << it->second->name() << endl;
		(*it2).second->StateChange -= delegate(this, &Roster::onPeerStateChange);
		(*it2).second->close();
		if (_freeClosedPeers)
			delete (*it2).second;
		_items.erase(it2);
	}

}


bool Roster::addPeer(Peer* stream, bool whiny) 
{
	assert(stream);
	assert(!stream->name().empty());
	
	Log("debug") << "[Roster::" << this << "] Adding Peer: " << stream->name() << endl;

	if (Roster::Manager::add(stream->name(), stream, false)) {

		// Receive callbacks after all other listeners so
		// we can delete the stream when it closes.
		stream->StateChange += delegate(this, &Roster::onPeerStateChange, -1);
		return true;
	}
	
	if (whiny)
		throw Exception("A stream already exists with name: " + stream->name());
	return false;
}

bool Roster::removePeer(const string& name) 
{
	assert(!name.empty());

	Log("debug") << "[Roster::" << this << "] Removing Peer: " << name << endl;		
	return Roster::Manager::remove(name) != 0;
}


bool Roster::closePeer(const string& name, bool whiny) 
{
	assert(!name.empty());

	Log("debug") << "[Roster::" << this << "] Closing Peer: " << name << endl;
	Peer* stream = get(name, whiny);
	if (stream) {
		stream->close();
		return true;
	}
	return false;
}

Peer* Roster::getPeer(const string& name, bool whiny) 
{
	return Roster::Manager::get(name, whiny);
}
*/


/*
void Roster::onPeerStateChange(void* sender, PeerState& state, const PeerState&)
{
	Log("debug") << "[Roster::" << this << "] Peer State Changed: " << state.toString() << endl;
	
	if (state.id() == PeerState::Closed) {
		Peer* stream = reinterpret_cast<Peer*>(sender);
		stream->StateChange -= delegate(this, &Roster::onPeerStateChange);
		if (_freeClosedPeers) {
			Log("debug") << "[Roster::" << this << "] Peer State Changed: Freeing: " << stream->name() << endl;
			Roster::Manager::free(stream->name());
		} else {
			Log("debug") << "[Roster::" << this << "] Peer State Changed: Removing: " << stream->name() << endl;
			Roster::Manager::remove(stream->name());
		}
	}
}
*/


Roster::PeerMap Roster::peers() const 
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _items; 
}


void Roster::print(ostream& os) const
{
	FastMutex::ScopedLock lock(_mutex);

	os << "Roster[";
	for (PeerMap::const_iterator it = _items.begin(); it != _items.end(); ++it) {	
		os << "\n\t" << it->second << ": " << it->first;
	}
	os << "\n]";
}


} } // namespace Sourcey::Symple
