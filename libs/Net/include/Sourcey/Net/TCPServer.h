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


#ifndef SOURCEY_NET_TCPServer_H
#define SOURCEY_NET_TCPServer_H


#include "Sourcey/Base.h"
#include "Sourcey/Stateful.h"
#include "Sourcey/Net/TCPSocketAcceptor.h"
#include "Sourcey/Net/ISocket.h"
#include "Sourcey/Net/Reactor.h"

#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/StreamSocket.h"


namespace Sourcey {
namespace Net {


template <class SocketT>
class TCPServer: public TCPSocketAcceptor
{
public:
	TCPServer(Reactor& reactor = Reactor::getDefault()) : TCPSocketAcceptor(reactor) {};
	virtual ~TCPServer() {};

	Signal<SocketT*> SocketCreated;
	
protected:
	virtual void onAccept()
	{
		Poco::Net::StreamSocket ss = acceptConnection();
		
		Log("debug") << "[TCPServer: " << this << "] Accept: " << ss.peerAddress().toString() << std::endl;

		SocketT* socket = createSocket(ss);		
		//socket->deleteOnClose(true); // destroy on close by default
		SocketCreated.dispatch(this, socket);
	}

	virtual SocketT* createSocket(Poco::Net::StreamSocket& socket)
	{
		return new SocketT(socket, _reactor);
	}
};


typedef TCPServer<TCPSocket> TCPPacketServer;


} } // namespace Sourcey::Net


#endif // SOURCEY_NET_TCPServer_H