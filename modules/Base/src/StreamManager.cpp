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


#include "Sourcey/StreamManager.h"
#include "Sourcey/Logger.h"


using namespace std; 
using namespace Poco;


namespace Sourcey {


StreamManager::StreamManager(bool freeClosedStreams) :
	_freeClosedStreams(freeClosedStreams)
{	
}


StreamManager::~StreamManager() 
{
	//Log("debug", this) << "Destroying" << endl;
	closeAll();
}


void StreamManager::closeAll()
{
	FastMutex::ScopedLock lock(_mutex);

	Log("debug", this) << "Closing All Streams: " << _items.size() << endl;
	
	StreamManager::Map::iterator it = _items.begin();
	StreamManager::Map::iterator it2;
	while (it != _items.end()) {
		it2 = it++;
		(*it2).second->StateChange -= delegate(this, &StreamManager::onStreamStateChange);
		(*it2).second->close();
		if (_freeClosedStreams)
			delete (*it2).second;
		_items.erase(it2);
	}
}


bool StreamManager::addStream(PacketStream* stream, bool whiny) 
{
	assert(stream);
	assert(!stream->name().empty());
	
	Log("debug", this) << "Adding Stream: " << stream->name() << endl;

	if (StreamManager::Manager::add(stream->name(), stream, false)) {

		// Receive callbacks after all other listeners so
		// we can delete the stream when it closes.
		stream->StateChange += delegate(this, &StreamManager::onStreamStateChange, -1);
		return true;
	}
	
	if (whiny)
		throw Exception("A stream already exists with name: " + stream->name());
	return false;
}


/*
bool StreamManager::removeStream(const string& name) 
{
	assert(!name.empty());

	Log("debug", this) << "Removing Stream: " << name << endl;		
	return StreamManager::Manager::remove(name) != 0;
}
*/


bool StreamManager::closeStream(const string& name, bool whiny) 
{
	assert(!name.empty());

	Log("debug", this) << "Closing Stream: " << name << endl;
	PacketStream* stream = get(name, whiny);
	if (stream) {
		stream->close();
		return true;
	}
	return false;
}


PacketStream* StreamManager::getStream(const string& name, bool whiny) 
{
	return StreamManager::Manager::get(name, whiny);
}


PacketStream* StreamManager::getDafaultStream()
{
	FastMutex::ScopedLock lock(_mutex);

	// Returns the first stream or NULL.
	if (!_items.empty()) {
		StreamManager::Map::const_iterator it = _items.begin();
		return it->second;
	}

	return NULL;
}


void StreamManager::onStreamStateChange(void* sender, PacketStreamState& state, const PacketStreamState&)
{
	Log("debug", this) << "Stream State Changed: " << state << endl;
	
	if (state.id() == PacketStreamState::Closed) {
		PacketStream* stream = reinterpret_cast<PacketStream*>(sender);
		stream->StateChange -= delegate(this, &StreamManager::onStreamStateChange);
		if (_freeClosedStreams) {
			Log("debug", this) << "Stream State Changed: Freeing: " << stream->name() << endl;
			StreamManager::Manager::free(stream->name());
		} else {
			Log("debug", this) << "Stream State Changed: Removing: " << stream->name() << endl;
			StreamManager::Manager::remove(stream->name());
		}
	}
}


StreamManager::Map StreamManager::streams() const 
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _items; 
}


void StreamManager::print(std::ostream& os) const
{
	FastMutex::ScopedLock lock(_mutex);

	os << "StreamManager[";
	for (StreamManager::Map::const_iterator it = _items.begin(); it != _items.end(); ++it) {	
		os << "\n\t" << it->second << ": " << it->first;
	}
	os << "\n]";
}


} // namespace Sourcey