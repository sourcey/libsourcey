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

#include "Sourcey/PacketQueue.h"


using namespace std;


namespace scy {


//
// Synchronization Packet Queue
//


SyncPacketQueue::SyncPacketQueue(uv::Loop& loop, int maxSize, int dispatchTimeout) : 
	SyncQueue<IPacket>(loop, maxSize, dispatchTimeout), 
	PacketProcessor(Emitter)
{	
	traceL("SyncPacketQueue", this) << "Creating" << endl;
}


SyncPacketQueue::SyncPacketQueue(int maxSize, int dispatchTimeout) : 
	SyncQueue<IPacket>(uv::defaultLoop(), maxSize, dispatchTimeout), 
	PacketProcessor(Emitter)
{	
	traceL("SyncPacketQueue", this) << "Creating" << endl;
}
	

SyncPacketQueue::~SyncPacketQueue()
{
	traceL("SyncPacketQueue", this) << "Destroying" << endl;
}


void SyncPacketQueue::process(IPacket& packet)
{
	if (!closed())
		push(packet.clone());
	else
		warnL("SyncPacketQueue", this) << "Dropping late packet" << endl;
}


void SyncPacketQueue::emit(IPacket& packet)
{
	traceL("SyncPacketQueue", this) << "Emitting: " << &packet << endl;
	
	// Emit should never be called after closure.
	// Any late packets should have been dealt with  
	// and dropped by the run() function.
	assert(!closed());

	PacketStreamAdapter::emit(packet);
}


void SyncPacketQueue::onStreamStateChange(const PacketStreamState& state)
{
	traceL("SyncPacketQueue", this) << "Stream state change: " << state << endl;
	
	switch (state.id()) {
	//case PacketStreamState::Running:
	//case PacketStreamState::Stopped:
	//case PacketStreamState::Error:
	//case PacketStreamState::Resetting:
	//case PacketStreamState::None:
	//case PacketStreamState::Closed:
	case PacketStreamState::Stopping:
		close();
		break;
	}
}


//
// Asynchronous Packet Queue
//


AsyncPacketQueue::AsyncPacketQueue(int maxSize, int dispatchTimeout) : 
	AsyncQueue<IPacket>(maxSize, dispatchTimeout), 
	PacketProcessor(Emitter)
{	
	traceL("AsyncPacketQueue", this) << "Creating" << endl;
}
	

AsyncPacketQueue::~AsyncPacketQueue()
{
	traceL("AsyncPacketQueue", this) << "Destroying" << endl;
}


void AsyncPacketQueue::process(IPacket& packet)
{
	push(packet.clone());
}


void AsyncPacketQueue::emit(IPacket& packet)
{
	traceL("AsyncPacketQueue", this) << "Emitting: " << &packet << endl;
	PacketStreamAdapter::emit(packet);
}


void AsyncPacketQueue::onStreamStateChange(const PacketStreamState& state)
{
	traceL("AsyncPacketQueue", this) << "Stream state change: " << state << endl;
	
	switch (state.id()) {
	case PacketStreamState::Running:
		_cancelled = false;
		break;
		
	case PacketStreamState::Stopped:

		// Flush all queued items on stop()
		flush();	
		assert(queue().empty());
		_cancelled = true;
		break;

	//case PacketStreamState::Error:
	//case PacketStreamState::Resetting:
	//case PacketStreamState::None:
	//case PacketStreamState::Stopping:
	//case PacketStreamState::Closed:
	}
}


} // namespace scy




/*
PacketStream::PacketStream(const string& name) : 
	PacketStream(name)
{
	_queue = new SyncPacketQueue(*this);
	traceL("PacketStream", this) << "Creating" << endl;
}


PacketStream::~PacketStream()
{
	traceL("PacketStream", this) << "Destroying" << endl;
	{
		ScopedLock lock(_mutex);
		if (_queue) {
			_queue->destroy();
			_queue = NULL;
		}
	}
	traceL("PacketStream", this) << "Destroying: OK" << endl;
}


void PacketStream::close()
{
	traceL("PacketStream", this) << "Closing" << endl;
	{
		ScopedLock lock(_mutex);
		if (_queue) {
			_queue->destroy();
			_queue = NULL;
		}
	}
	PacketStream::close();
	traceL("PacketStream", this) << "Closing: OK" << endl;
}


void PacketStream::onFinalPacket(IPacket& packet)
{
	//traceL("PacketStream", this) << "On final packet: " << &packet << ": " << packet.className() << endl;
	ScopedLock lock(_mutex);

	if (_queue) {
		// Ensure the stream is running and enabled 
		// before we dispatch the goods.
		if (!running() || !enabled()) {
			debugL("PacketStream", this) << "Dropping final packet: " 
				<< enabled() << ": " << state() << endl;	
			return;
		}
		_queue->push(packet.clone());
	}
	else {
		//assert(0);
		debugL("PacketStream", this) << "Dropping final packet: Invalid sync queue" << endl;	
	}
}
*/
