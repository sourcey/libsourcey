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


#ifndef SOURCEY_SocketIO_Transaction_H
#define SOURCEY_SocketIO_Transaction_H


#include "Sourcey/ITransaction.h"
#include "Sourcey/SocketIO/Packet.h"


namespace Sourcey {
namespace SocketIO {


class Socket;


struct Transaction: public ITransaction<SocketIO::Packet>
{
	Transaction(SocketIO::Socket& socket, int maxAttempts = 1, long timeout = 10000);
	Transaction(SocketIO::Socket& socket, const SocketIO::Packet& request, int maxAttempts = 1, long timeout = 10000);
	virtual ~Transaction();
	
	virtual bool send();
	virtual bool match(const Packet& packet);

	virtual void onPotentialResponse(void*, Packet& packet);
	virtual void onComplete();

	SocketIO::Socket& socket;
};


} } // namespace Sourcey::SocketIO


#endif // SOURCEY_SocketIO_Transaction_H