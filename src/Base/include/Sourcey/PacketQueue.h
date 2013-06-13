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


#ifndef SOURCEY_PacketQueue_H
#define SOURCEY_PacketQueue_H


#include "Sourcey/PacketEmitter.h"
#include "Sourcey/DispatchQueue.h"


namespace Scy {


class PacketQueue: public DispatchQueue<IPacket>
{
public:
	PacketQueue(PacketEmitter& dispatcher, Runner& runner, int queueSize = 1024, int dispatchTimeout = DEFAULT_TIMEOUT);
	
	virtual void emit(IPacket& packet);

protected:
	virtual ~PacketQueue();
	PacketEmitter&	_dispatcher;
};


} // namespace Scy


#endif // SOURCEY_PacketQueue_H

	

	//enum
	//{
	//	DEFAULT_TIMEOUT = 250000
	//};


	//IPacketQueue		_queue;
	//int					_queueSize;
	//int					_timeout;
	//Poco::Stopwatch		_stopwatch;	
	//mutable Poco::FastMutex	_mutex;


	//virtual bool start();
	//virtual bool stop();
	
	//virtual void push(IPacket* packet);
		// Pushes a packet onto the queue.
		// NOTE: Packets should be clone()'d.

	//virtual void clear();
		// Clears all queued packets.

	//virtual void run();
		// Called by the Runner to clear the broadcast queue.
		// Provides asynchronous socket reading for our UDP packet
		// socket to minimize packet loss and improve performance.