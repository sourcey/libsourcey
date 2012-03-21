//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is is distributed under a dual license that allows free, 
// open source use and closed source use under a standard commercial
// license.
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


#ifndef SOURCEY_PacketQueue_H
#define SOURCEY_PacketQueue_H


#include "Sourcey/PacketDispatcher.h"
#include "Sourcey/DispatchQueue.h"


namespace Sourcey {


class PacketQueue: public DispatchQueue<IPacket>
{
public:
	PacketQueue(PacketDispatcher& dispatcher, Runner& runner, int queueSize = 1024, int dispatchTimeout = DEFAULT_TIMEOUT);
	
	virtual void dispatch(IPacket& packet);

protected:
	virtual ~PacketQueue();
	PacketDispatcher&	_dispatcher;
};


} // namespace Sourcey


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