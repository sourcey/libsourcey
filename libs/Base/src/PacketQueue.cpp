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


#include "Sourcey/PacketQueue.h"
#include <iostream>


using namespace std;
using namespace Poco;
using namespace Poco;


namespace Sourcey {


PacketQueue::PacketQueue(PacketDispatcher& dispatcher, Runner& runner, int queueSize, int dispatchTimeout) :
	 DispatchQueue<IPacket>(runner, queueSize, dispatchTimeout),
	_dispatcher(dispatcher)
{	
	Log("trace") << "[PacketQueue:" << this << "] Creating" << endl;
}
	

PacketQueue::~PacketQueue()
{
	Log("trace") << "[PacketQueue:" << this << "] Destroying" << endl;
}


void PacketQueue::dispatch(IPacket& packet)
{
	//Log("trace") << "[PacketQueue:" << this << "] Dispatching: " << &packet << endl;
	_dispatcher.dispatch(this, packet);
}


} // namespace Sourcey