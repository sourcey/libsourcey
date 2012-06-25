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


#include "Sourcey/Net/TCPSocketAcceptor.h"


using namespace std;
using namespace Poco;
using namespace Poco::Net;


namespace Sourcey {
namespace Net {


TCPSocketAcceptor::TCPSocketAcceptor(Reactor& reactor) :
	_reactor(reactor)
{
	Log("trace") << "[TCPSocketAcceptor: " << this << "] Creating" << endl;
}


TCPSocketAcceptor::~TCPSocketAcceptor() 
{	
	Log("trace") << "[TCPSocketAcceptor: " << this << "] Destroying" << endl;	

	close();
}


void TCPSocketAcceptor::bind(UInt16 localPort) 
{
	bind(Address("127.0.0.1", localPort));
}


void TCPSocketAcceptor::bind(const Address& address) 
{
	Log("debug") << "[TCPSocketAcceptor:" << this << "] Binding on " << address << endl;
	
	_reactor.attach(*this, reactorDelegate(this, &TCPSocketAcceptor::onAccept, SocketReadable));

	ServerSocket::bind(address);
	ServerSocket::listen();
}


void TCPSocketAcceptor::close()
{
	Log("trace") << "[TCPSocketAcceptor:" << this << "] Closing" << endl;
	
	_reactor.detach(*this, reactorDelegate(this, &TCPSocketAcceptor::onAccept, SocketReadable));
	try	{
		// If the socket is already closed an exception
		// will be thrown. Just swallow it.
		ServerSocket::close();
	}
	catch (Poco::IOException& exc) {
		Log("error") << "[TCPSocketAcceptor:" << this << "] " << exc.displayText() << endl;
	}

	Log("trace") << "[TCPSocketAcceptor:" << this << "] Closing: OK" << endl;
}


void TCPSocketAcceptor::onAccept() 
{
	Log("trace") << "[TCPSocketAcceptor:" << this << "] On Accept" << endl;
	try	{
		StreamSocket sock = acceptConnection();
		SocketAccepted.dispatch(this, sock, _reactor);
	}
	catch (Poco::IOException& exc) {
		Log("error") << "[TCPSocketAcceptor:" << this << "] " << exc.displayText() << endl;
	}
}


} } // namespace Sourcey::Net