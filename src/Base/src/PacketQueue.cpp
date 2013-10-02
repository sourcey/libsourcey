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


SyncPacketQueue::SyncPacketQueue(uv::Loop* loop, int maxSize) : 
	SyncQueue<IPacket>(loop, maxSize), 
	PacketProcessor(this->emitter)
{	
}


SyncPacketQueue::SyncPacketQueue(int maxSize) : 
	SyncQueue<IPacket>(uv::defaultLoop(), maxSize), 
	PacketProcessor(this->emitter)
{	
}
	

SyncPacketQueue::~SyncPacketQueue()
{
}


void SyncPacketQueue::process(IPacket& packet)
{
	if (cancelled()) {
		warnL("SyncPacketQueue", this) << "Process late packet" << endl;
		assert(0);
		return;
	}
	
	push(packet.clone());
}


void SyncPacketQueue::dispatch(IPacket& packet)
{	
	// Emit should never be called after closure.
	// Any late packets should have been dealt with  
	// and dropped by the run() function.
	if (cancelled()) {
		warnL("SyncPacketQueue", this) << "Dispatch late packet" << endl;
		assert(0);
		return;
	}
	
	PacketStreamAdapter::emit(packet);
}


void SyncPacketQueue::onStreamStateChange(const PacketStreamState& state)
{
	traceL("SyncPacketQueue", this) << "Stream state: " << state << endl;
	
	switch (state.id()) {
	//case PacketStreamState::None:
	//case PacketStreamState::Active:
	//case PacketStreamState::Resetting:
	//case PacketStreamState::Stopping:
	//case PacketStreamState::Stopped:
	case PacketStreamState::Closed:
	case PacketStreamState::Error:
		// Can't call close here since we can't 
		// guarantee the calling thread.
		//SyncContext::close();
		SyncQueue<IPacket>::cancel();
		break;
	}
}


//
// Asynchronous Packet Queue
//


AsyncPacketQueue::AsyncPacketQueue(int maxSize) : 
	AsyncQueue<IPacket>(maxSize), 
	PacketProcessor(this->emitter)
{	
}
	

AsyncPacketQueue::~AsyncPacketQueue()
{
}


void AsyncPacketQueue::process(IPacket& packet)
{
	if (cancelled()) {
		warnL("AsyncPacketQueue", this) << "Process late packet" << endl;
		assert(0);
		return;
	}
	
	push(packet.clone());
}


void AsyncPacketQueue::dispatch(IPacket& packet)
{
	if (cancelled()) {
		warnL("AsyncPacketQueue", this) << "Dispatch late packet" << endl;
		assert(0);
		return;
	}

	PacketStreamAdapter::emit(packet);
}


void AsyncPacketQueue::onStreamStateChange(const PacketStreamState& state)
{
	traceL("AsyncPacketQueue", this) << "Stream state: " << state << endl;
	
	switch (state.id()) {
	case PacketStreamState::Active:
		break;
		
	case PacketStreamState::Stopped:

		// Flush all queued items on stop()
		flush();	
		cancel();
		assert(empty());
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
PacketStream::PacketStream(const std::string& name) : 
	PacketStream(name)
{
	_queue = new SyncPacketQueue(*this);
	traceL("PacketStream", this) << "Create" << endl;
}


PacketStream::~PacketStream()
{
	traceL("PacketStream", this) << "Destroy" << endl;
	{
		Mutex::ScopedLock lock(_mutex);
		if (_queue) {
			_queue->destroy();
			_queue = nullptr;
		}
	}
	traceL("PacketStream", this) << "Destroy: OK" << endl;
}


void PacketStream::close()
{
	traceL("PacketStream", this) << "Closing" << endl;
	{
		Mutex::ScopedLock lock(_mutex);
		if (_queue) {
			_queue->destroy();
			_queue = nullptr;
		}
	}
	PacketStream::close();
	traceL("PacketStream", this) << "Closing: OK" << endl;
}


void PacketStream::onFinalPacket(IPacket& packet)
{
	//traceL("PacketStream", this) << "On final packet: " << &packet << ": " << packet.className() << endl;
	Mutex::ScopedLock lock(_mutex);

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
