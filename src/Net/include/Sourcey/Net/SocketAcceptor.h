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


#ifndef SOURCEY_NET_SocketAcceptor_H
#define SOURCEY_NET_SocketAcceptor_H


#include "Sourcey/Base.h"
#include "Sourcey/Stateful.h"
#include "Sourcey/Net/ISocket.h"
#include "Sourcey/Net/Reactor.h"

#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SecureServerSocket.h"
#include "Poco/Net/SecureStreamSocket.h"


namespace Sourcey {
namespace Net {


template <class ServerSocketT, class StreamSocketT>
class SocketAcceptorBase: public ServerSocketT
	/// This class implements a generic server socket   
	/// acceptor for TCP and SSL socket types.
{
public:
	SocketAcceptorBase(Reactor& reactor/* = Reactor::getDefault()*/) :
		_reactor(reactor)
	{
		LogTrace() << "[TCPSocketAcceptor: " << this << "] Creating" << endl;
	}


	virtual ~SocketAcceptorBase()
	{	
		LogTrace() << "[TCPSocketAcceptor: " << this << "] Destroying" << endl;
		close();
	}
	

	virtual void bind(UInt16 localPort)
	{
		bind(Address("127.0.0.1", localPort));
	}


	virtual void bind(const Address& address)
	{
		LogDebug() << "[TCPSocketAcceptor:" << this << "] Binding on " << address << endl;
	
		_reactor.attach(*this, reactorDelegate(this, &SocketAcceptorBase<ServerSocketT, StreamSocketT>::onAccept, SocketReadable));

		ServerSocketT::bind(address);
		ServerSocketT::listen();
	}


	virtual void close()
	{
		LogTrace() << "[TCPSocketAcceptor:" << this << "] Closing" << endl;
	
		_reactor.detach(*this, reactorDelegate(this, &SocketAcceptorBase<ServerSocketT, StreamSocketT>::onAccept, SocketReadable));
		try	{
			// If the socket is already closed an exception
			// will be thrown. Just swallow it.
			ServerSocketT::close();
		}
		catch (Poco::IOException& exc) {
			LogError() << "[TCPSocketAcceptor:" << this << "] Closing Error: " << exc.displayText() << endl;
		}
	}


	Signal2<StreamSocketT&, Reactor&> SocketAccepted;
	

protected:
	virtual void onAccept()
	{
		LogTrace() << "[TCPSocketAcceptor:" << this << "] On Accept" << endl;
		StreamSocketT sock = acceptConnection();
		SocketAccepted.dispatch(this, sock, _reactor);
	}

	
protected:
	Reactor& _reactor;
};


typedef SocketAcceptorBase<Poco::Net::ServerSocket, Poco::Net::StreamSocket>             TCPSocketAcceptor;
typedef SocketAcceptorBase<Poco::Net::SecureServerSocket, Poco::Net::SecureStreamSocket> SSLSocketAcceptor;


} } // namespace Sourcey::Net


#endif // SOURCEY_NET_SocketAcceptor_H