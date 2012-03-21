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


#ifndef SOURCEY_NET_TCPSocketAcceptor_H
#define SOURCEY_NET_TCPSocketAcceptor_H


#include "Sourcey/Base.h"
#include "Sourcey/Stateful.h"
#include "Sourcey/Net/ISocket.h"
#include "Sourcey/Net/Reactor.h"

#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/StreamSocket.h"


namespace Sourcey {
namespace Net {


class TCPSocketAcceptor: public Poco::Net::ServerSocket
{
public:
	TCPSocketAcceptor(Reactor& reactor = Reactor::getDefault());
	virtual ~TCPSocketAcceptor();
	
	virtual void bind(UInt16 localPort);		
	virtual void bind(const Address& address);		
	virtual void close();

	Signal2<Poco::Net::StreamSocket&, Reactor&> SocketAccepted;
	
protected:
	virtual void onAccept();

	Reactor& _reactor;
};


} } // namespace Sourcey::Net


#endif // SOURCEY_NET_TCPSocketAcceptor_H