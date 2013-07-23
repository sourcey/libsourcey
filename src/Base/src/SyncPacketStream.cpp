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


#include "Sourcey/SyncPacketStream.h"
#include <iostream>


using namespace std;


namespace scy {


SyncPacketStream::SyncPacketStream(const string& name) : 
	PacketStream(name), _queue(new SyncPacketQueue(*this))
{
	traceL("SyncPacketStream", this) << "Creating" << endl;
}


SyncPacketStream::~SyncPacketStream()
{
	traceL("SyncPacketStream", this) << "Destroying" << endl;
	{
		Mutex::ScopedLock lock(_mutex);
		if (_queue) {
			_queue->destroy();
			_queue = nullptr;
		}
	}
	traceL("SyncPacketStream", this) << "Destroying: OK" << endl;
}


void SyncPacketStream::close()
{
	traceL("SyncPacketStream", this) << "Closing" << endl;
	{
		Mutex::ScopedLock lock(_mutex);
		if (_queue) {
			_queue->destroy();
			_queue = nullptr;
		}
	}
	PacketStream::close();
	traceL("SyncPacketStream", this) << "Closing: OK" << endl;
}


void SyncPacketStream::onFinalPacket(IPacket& packet)
{
	//traceL("SyncPacketStream", this) << "On final packet: " << &packet << ": " << packet.className() << endl;
	Mutex::ScopedLock lock(_mutex);

	if (_queue) {
		// Ensure the stream is running and enabled 
		// before we dispatch the goods.
		if (!running() || !enabled()) {
			debugL("SyncPacketStream", this) << "Dropping final packet: " 
				<< enabled() << ": " << state() << endl;	
			return;
		}
		_queue->push(packet.clone());
	}
	else {
		//assert(0);
		debugL("SyncPacketStream", this) << "Dropping final packet: Invalid sync queue" << endl;	
	}
}


// ----------------------------------------------------------------------------
//
SyncPacketQueue::SyncPacketQueue(uv::Loop& loop, PacketSignal& signal, int maxSize, int dispatchTimeout) :
	 SyncQueue<IPacket>(loop, maxSize, dispatchTimeout),
	_signal(signal)
{	
	traceL("SyncPacketQueue", this) << "Creating" << endl;
}


SyncPacketQueue::SyncPacketQueue(PacketSignal& signal, int maxSize, int dispatchTimeout) :
	 SyncQueue<IPacket>(uv::defaultLoop(), maxSize, dispatchTimeout),
	_signal(signal)
{	
	traceL("SyncPacketQueue", this) << "Creating" << endl;
}
	

SyncPacketQueue::~SyncPacketQueue()
{
	traceL("SyncPacketQueue", this) << "Destroying" << endl;
}


void SyncPacketQueue::close()
{
	traceL("SyncPacketQueue", this) << "Closing" << endl;
	uv::Base::close();
}


void SyncPacketQueue::emit(IPacket& packet)
{
	traceL("SyncPacketQueue", this) << "Emitting: " << &packet << endl;

	// Emit should never be called after closure.
	// Any late packets should by dealt with and 
	// dropped by the run() function.
	assert(!closed());

	_signal.emit(this, packet);
	traceL("SyncPacketQueue", this) << "Emitting: OK: " << &packet << endl;
}


} // namespace scy