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


#ifndef SOURCEY_Router_H
#define SOURCEY_Router_H


#include "Sourcey/Runner.h"
#include "Sourcey/IPacket.h"
#include "Sourcey/PacketEmitter.h"
#include "Sourcey/PacketQueue.h"


namespace scy {


class Router: public PacketEmitter
	/// The Router acts as a asynchronous notification queue
	/// for the IPacket type.
	///
	/// This model is useful in when dispatching data in a
	/// no-delay scenario, sock as reading from a UDP socket.
	///
	/// Delegate filtering is achieved by sub classing the 
	/// PolymorphicDelegate.
{
public:
	Router(Runner& runner);	
	virtual ~Router();

	void send(const IPacket& packet);
		// Enquires the packet for sending.
		// The packet will be cloned.

	Router& operator >> (IPacket& packet);
	
	Runner& runner();

protected:
	PacketQueue*	_task;
	Runner&				_runner;
	mutable Poco::Mutex		_mutex;
};


} // namespace scy


#endif // SOURCEY_Router_H