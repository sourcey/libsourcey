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


#include "Sourcey/Util/StreamManager.h"
#include "Sourcey/Logger.h"


using namespace std; 


namespace scy {


StreamManager::StreamManager(bool freeClosedStreams) :
	_freeClosedStreams(freeClosedStreams)
{	
}


StreamManager::~StreamManager() 
{
	log("debug") << "Destroying" << endl;
	closeAll();
}


void StreamManager::closeAll()
{
	Mutex::ScopedLock lock(_mutex);
	
	log("debug") << "Closing all streams: " << _map.size() << endl;	
	StreamManager::Map::iterator it = _map.begin();
	StreamManager::Map::iterator it2;
	while (it != _map.end()) {
		it2 = it++;
		(*it2).second->StateChange -= delegate(this, &StreamManager::onStreamStateChange);
		(*it2).second->close();
		if (_freeClosedStreams)
			StreamManager::Deleter::func((*it2).second);
			//delete (*it2).second;
		_map.erase(it2);
	}
}


bool StreamManager::addStream(PacketStream* stream, bool whiny) 
{
	assert(stream);
	assert(!stream->name().empty());
	return Manager::add(stream->name(), stream, whiny);
}


PacketStream* StreamManager::getStream(const string& name, bool whiny) 
{
	return Manager::get(name, whiny);
}


bool StreamManager::closeStream(const string& name, bool whiny) 
{
	assert(!name.empty());

	log("debug") << "Closing Stream: " << name << endl;
	PacketStream* stream = get(name, whiny);
	if (stream) {
		stream->close();
		return true;
	}
	return false;
}


PacketStream* StreamManager::getDafaultStream()
{
	Mutex::ScopedLock lock(_mutex);

	// Returns the first stream or NULL.
	if (!_map.empty()) {
		StreamManager::Map::const_iterator it = _map.begin();
		return it->second;
	}

	return NULL;
}


void StreamManager::onAdd(PacketStream* stream)
{
	// Stream name can't be empty
	assert(!stream->name().empty());

	// Receive callbacks after all other listeners 
	// so we can delete the stream when it closes.
	log("debug") << "Stream Added: " << stream->name() << endl;
	stream->StateChange += delegate(this, &StreamManager::onStreamStateChange, -1);
}


void StreamManager::onRemove(PacketStream* stream)
{
	log("debug") << "Stream Removed: " << stream->name() << endl;
	stream->StateChange -= delegate(this, &StreamManager::onStreamStateChange);
}


void StreamManager::onStreamStateChange(void* sender, PacketStreamState& state, const PacketStreamState&)
{
	log("debug") << "Stream State Changed: " << state << endl;
	
	// Cantch stream closed state and free it if necessary
	if (state.equals(PacketStreamState::Closed)) {
		PacketStream* stream = reinterpret_cast<PacketStream*>(sender);
		stream->StateChange -= delegate(this, &StreamManager::onStreamStateChange);
		bool success = false;
		if (_freeClosedStreams) {
			log("debug") << "On Stream Closed: Freeing: " << stream->name() << endl;
			success = Manager::free(stream->name());
		} else {
			log("debug") << "On Stream Closed: Removing: " << stream->name() << endl;
			success = !!Manager::remove(stream->name());
		}
		assert(success);
	}
}


StreamManager::Map StreamManager::streams() const 
{ 
	Mutex::ScopedLock lock(_mutex);
	return _map; 
}


void StreamManager::print(std::ostream& os) const
{
	Mutex::ScopedLock lock(_mutex);

	os << "StreamManager[";
	for (StreamManager::Map::const_iterator it = _map.begin(); it != _map.end(); ++it) {	
		os << "\n\t" << it->second << ": " << it->first;
	}
	os << "\n]";
}


} // namespace scy





/*
bool StreamManager::removeStream(const string& name) 
{
	assert(!name.empty());

	log("debug") << "Removing Stream: " << name << endl;		
	return Manager::remove(name) != 0;
}
*/