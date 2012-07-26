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


#ifndef SOURCEY_SocketIO_Transaction_H
#define SOURCEY_SocketIO_Transaction_H


#include "Sourcey/PacketTransaction.h"
#include "Sourcey/SocketIO/Packet.h"


namespace Sourcey {
namespace SocketIO {


class Client;


struct Transaction: public PacketTransaction<SocketIO::Packet>
{
	Transaction(SocketIO::Client& client, long timeout = 10000);
	Transaction(SocketIO::Client& client, const SocketIO::Packet& request, long timeout = 10000);
	virtual ~Transaction();
	
	virtual bool send();
	virtual bool match(const Packet& packet);

	virtual void onPotentialResponse(void*, Packet& packet);
	virtual void onComplete();

	SocketIO::Client& client;
};


} } // namespace Sourcey::SocketIO


#endif // SOURCEY_SocketIO_Transaction_H