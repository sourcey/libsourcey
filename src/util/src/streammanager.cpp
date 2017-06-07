///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup util
/// @{


#include "scy/util/streammanager.h"
#include "scy/logger.h"


using namespace std;


namespace scy {


StreamManager::StreamManager(bool freeClosedStreams)
    : _freeClosedStreams(freeClosedStreams)
{
}


StreamManager::~StreamManager()
{
    closeAll();
}


void StreamManager::closeAll()
{
    std::lock_guard<std::mutex> guard(_mutex);

    LDebug("Close all streams: ", _map.size())
    StreamManager::Map::iterator it = _map.begin();
    StreamManager::Map::iterator it2;
    while (it != _map.end()) {
        it2 = it++;
        (*it2).second->StateChange -= slot(this, &StreamManager::onStreamStateChange);
        (*it2).second->close();
        if (_freeClosedStreams) {
            StreamManager::Deleter func;
            func((*it2).second);
        }
        _map.erase(it2);
    }
}


bool StreamManager::addStream(PacketStream* stream, bool whiny)
{
    assert(stream);
    assert(!stream->name().empty());
    return Manager::add(stream->name(), stream, whiny);
}


PacketStream* StreamManager::getStream(const std::string& name, bool whiny)
{
    return Manager::get(name, whiny);
}


bool StreamManager::closeStream(const std::string& name, bool whiny)
{
    assert(!name.empty());

    LDebug("Close stream: ", name)
    PacketStream* stream = get(name, whiny);
    if (stream) {
        stream->close();
        return true;
    }
    return false;
}


PacketStream* StreamManager::getDafaultStream()
{
    std::lock_guard<std::mutex> guard(_mutex);

    // Returns the first stream or NULL.
    if (!_map.empty()) {
        StreamManager::Map::const_iterator it = _map.begin();
        return it->second;
    }

    return NULL;
}


void StreamManager::onAdd(const std::string&, PacketStream* stream)
{
    // Stream name can't be empty
    assert(!stream->name().empty());

    // Receive callbacks after all other listeners
    // so we can delete the stream when it closes.
    LDebug("stream added: ", stream->name())
    stream->StateChange += slot(this, &StreamManager::onStreamStateChange, -1);
}


void StreamManager::onRemove(const std::string&, PacketStream* stream)
{
    LDebug("stream removed: ", stream->name())
    stream->StateChange -= slot(this, &StreamManager::onStreamStateChange);
}


void StreamManager::onStreamStateChange(void* sender, PacketStreamState& state,
                                        const PacketStreamState&)
{
    LDebug("Stream state change: ", state)

    // Cantch stream closed state and free it if necessary
    if (state.equals(PacketStreamState::Closed)) {
        auto stream = reinterpret_cast<PacketStream*>(sender);
        stream->StateChange -= slot(this, &StreamManager::onStreamStateChange);
        bool success = false;
        if (_freeClosedStreams) {
            LDebug("On stream close: freeing: ", stream->name())
            success = Manager::free(stream->name());
        } else {
            LDebug("On stream close: removing: ", stream->name())
            success = !!Manager::remove(stream->name());
        }
        if (!success) {
            LWarn("Cannot remove stream: ", stream->name())
            assert(0);
        }
    }
}


StreamManager::Map StreamManager::streams() const
{
    std::lock_guard<std::mutex> guard(_mutex);
    return _map;
}


void StreamManager::print(std::ostream& os) const
{
    std::lock_guard<std::mutex> guard(_mutex);
    os << "StreamManager[";
    for (auto it = _map.begin(); it != _map.end(); ++it) {
        os << "\n\t" << it->second << ": " << it->first;
    }
    os << "\n]";
}


} // namespace scy
