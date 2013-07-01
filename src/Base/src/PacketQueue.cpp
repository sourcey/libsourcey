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
#include <iostream>


using namespace std;
using namespace Poco;
using namespace Poco;


namespace scy {


PacketQueue::PacketQueue(PacketEmitter& dispatcher, Runner& runner, int queueSize, int dispatchTimeout) :
	 DispatchQueue<IPacket>(runner, queueSize, dispatchTimeout),
	_dispatcher(dispatcher)
{	
	traceL() << "[PacketQueue:" << this << "] Creating" << endl;
}
	

PacketQueue::~PacketQueue()
{
	traceL() << "[PacketQueue:" << this << "] Destroying" << endl;
}


void PacketQueue::emit(IPacket& packet)
{
	//traceL() << "[PacketQueue:" << this << "] Dispatching: " << &packet << endl;
	_dispatcher.emit(this, packet);
}


} // namespace scy