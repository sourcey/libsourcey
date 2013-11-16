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


#ifndef SCY_PacketQueue_H
#define SCY_PacketQueue_H


#include "scy/packetstream.h"
#include "scy/synccontext.h"


namespace scy {
	

//
// Synchronization Packet Queue
//


class SyncPacketQueue: public SyncQueue<IPacket>, public PacketProcessor
{
public:
	SyncPacketQueue(uv::Loop* loop, int maxSize = 1024);
	SyncPacketQueue(int maxSize = 1024);
	virtual ~SyncPacketQueue();

	virtual void process(IPacket& packet);

	PacketSignal emitter;

protected:	
	virtual void dispatch(IPacket& packet);

	virtual void onStreamStateChange(const PacketStreamState&);
};


//
// Asynchronous Packet Queue
//


class AsyncPacketQueue: public AsyncQueue<IPacket>, public PacketProcessor
{
public:
	AsyncPacketQueue(int maxSize = 1024);
	virtual ~AsyncPacketQueue();

	virtual void process(IPacket& packet);
	
	PacketSignal emitter;

protected:	
	virtual void dispatch(IPacket& packet);

	virtual void onStreamStateChange(const PacketStreamState&);
};


} // namespace scy


#endif // SCY_PacketQueue_H





/*
//
// Synchronized Packet Stream
//


class PacketStream: public PacketStream
{
public:
	PacketStream(uv::Loop* loop, const std::string& name = "");
	PacketStream(const std::string& name = "");

	virtual ~PacketStream(); // make ref count and protected?

	virtual void close();

protected:		
	virtual void onFinalPacket(IPacket& packet);

	SyncPacketQueue* _queue;
};
*/
