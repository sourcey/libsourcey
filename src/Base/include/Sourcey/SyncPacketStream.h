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


#ifndef SOURCEY_SyncPacketStream_H
#define SOURCEY_SyncPacketStream_H


#include "Sourcey/PacketStream.h"
#include "Sourcey/SyncQueue.h"


namespace scy {


class SyncPacketQueue: public SyncQueue<IPacket>
	/// SyncPacketQueue entends SyncQueue to enable 
	/// synchronization of IPackets generated in a seperate
	/// thread to be handled by the given uv::Loop thread.
{
public:
	SyncPacketQueue(uv::Loop& loop, PacketSignal& signal, int maxSize = 1024, int dispatchTimeout = DEFAULT_TIMEOUT);
	SyncPacketQueue(PacketSignal& signal, int maxSize = 1024, int dispatchTimeout = DEFAULT_TIMEOUT);
	virtual ~SyncPacketQueue();
	
	virtual void close();

protected:	
	virtual void emit(IPacket& packet);
		/// Emits the synchronized IPacket via the PacketSignal 
		/// in the uv::Loop thread.

	PacketSignal& _signal;
};


// ----------------------------------------------------------------------------
//
class SyncPacketStream: public PacketStream
{
public:
	SyncPacketStream(uv::Loop& loop, const std::string& name = "");
	SyncPacketStream(const std::string& name = "");

	virtual ~SyncPacketStream(); // make ref count and protected?

	virtual void close();

protected:		
	virtual void onFinalPacket(IPacket& packet);

	SyncPacketQueue* _queue;
};



} // namespace scy


#endif // SOURCEY_SyncPacketStream_H