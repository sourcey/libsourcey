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


#ifndef SOURCEY_NET_TCPSocketAcceptor_H
#define SOURCEY_NET_TCPSocketAcceptor_H


#include "Sourcey/Base.h"
#include "Sourcey/Stateful.h"
/*
#include "Sourcey/Net/ISocket.h"
#include "Sourcey/Net/Reactor.h"

#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SecureServerSocket.h"
#include "Poco/Net/SecureStreamSocket.h"
*/


namespace Scy {
namespace UV {


//template <class ServerSocketT, class StreamSocketT>
class TCPSocketAcceptorBase//: public ServerSocketT
	/// This class implements a generic server socket   
	/// acceptor for TCP and SSL socket types.
{
public:
	TCPSocketAcceptorBase(/*Reactor& reactor = Reactor::getDefault()*/) :
		_reactor(reactor)
	{
		LogTrace() << "[TCPTCPSocketAcceptor: " << this << "] Creating" << endl;
	}


	virtual ~TCPSocketAcceptorBase()
	{	
		LogTrace() << "[TCPTCPSocketAcceptor: " << this << "] Destroying" << endl;
		close();
	}
	

	virtual void bind(UInt16 localPort)
	{
		bind(Address("127.0.0.1", localPort));
	}


	virtual void bind(const Address& address)
	{
		LogDebug() << "[TCPTCPSocketAcceptor:" << this << "] Binding on " << address << endl;
	
		_reactor.attach(*this, reactorDelegate(this, &TCPSocketAcceptorBase<ServerSocketT, StreamSocketT>::onAccept, SocketReadable));

		ServerSocketT::bind(address);
		ServerSocketT::listen();
	}


	virtual void close()
	{
		LogTrace() << "[TCPTCPSocketAcceptor:" << this << "] Closing" << endl;
	
		_reactor.detach(*this, reactorDelegate(this, &TCPSocketAcceptorBase<ServerSocketT, StreamSocketT>::onAccept, SocketReadable));
		try	{
			// If the socket is already closed an exception
			// will be thrown. Just swallow it.
			ServerSocketT::close();
		}
		catch (Poco::IOException& exc) {
			LogError() << "[TCPTCPSocketAcceptor:" << this << "] Closing Error: " << exc.displayText() << endl;
		}
	}


	Signal2<StreamSocketT&, Reactor&> SocketAccepted;
	

protected:
	virtual void onAccept()
	{
		LogTrace() << "[TCPTCPSocketAcceptor:" << this << "] On Accept" << endl;
		StreamSocketT sock = acceptConnection();
		SocketAccepted.emit(this, sock, _reactor);
	}

	
protected:
	Reactor& _reactor;
};


typedef TCPSocketAcceptorBase<Poco::Net::ServerSocket, Poco::Net::StreamSocket>             TCPTCPSocketAcceptor;
typedef TCPSocketAcceptorBase<Poco::Net::SecureServerSocket, Poco::Net::SecureStreamSocket> SSLTCPSocketAcceptor;


} } // namespace Scy::Net


#endif // SOURCEY_NET_TCPSocketAcceptor_H