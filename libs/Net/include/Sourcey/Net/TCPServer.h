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


#ifndef SOURCEY_NET_TCPServer_H
#define SOURCEY_NET_TCPServer_H


#include "Sourcey/Base.h"
#include "Sourcey/Stateful.h"
#include "Sourcey/Net/TCPSocket.h"
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
	TCPServer(Reactor& reactor/* = Reactor::getDefault()*/) : TCPSocketAcceptor(reactor) {};
	virtual ~TCPServer() {};

	Signal<SocketT&> SocketCreated;
		/// Dispatches created sockets.
	
protected:
	virtual void onAccept()
	{
		Poco::Net::StreamSocket ss = acceptConnection();
		
		Log("trace") << "[TCPServer:" << this << "] Accept: " << ss.peerAddress().toString() << std::endl;

		SocketT socket = SocketT(ss, _reactor);
		SocketCreated.dispatch(this, socket);
	}
};


typedef TCPServer<TCPSocket> TCPPacketServer;


} } // namespace Sourcey::Net


#endif // SOURCEY_NET_TCPServer_H



	
		/// NOTE: The Socket pointer must be freed by the outside application.
		//if (SocketCreated.refCount()) {
		//}
			// TODO: Create the socket on the stack so we don't
			// need to listen for close to free socket pointer.
	//socket->Closed += delegate(this, &TCPServer::onSocketClosed, -1);	
	//void onSocketClosed(void* sender) 
	//{
	//	SocketT* socket = reinterpret_cast<SocketT*>(sender);	
	//	Log("trace") << "[TCPServer:" << this << "] Freeing Socket: " << socket->peerAddress().toString() << std::endl;
	//	delete socket;
	//}

	//virtual SocketT* createSocket(Poco::Net::StreamSocket& socket)
	//{
	//	return new SocketT(socket, _reactor);
	//}