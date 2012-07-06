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


/*
#include "Sourcey/Net/StatefulSocket.h"


using namespace std;
using namespace Poco;
using namespace Poco::Net;
*/


namespace Sourcey {
namespace Net {
	

	/*
TCPStatefulSocket::TCPStatefulSocket(Reactor& reactor) : 
	TCPSocket(reactor)
{
}


TCPStatefulSocket::TCPStatefulSocket(const TCPStatefulSocket& r) : 
	TCPSocket(r)
{
}
	
	
TCPStatefulSocket::~TCPStatefulSocket()
{
}


void TCPStatefulSocket::connect(const Address& peerAddress, int timeout) 
{
	setState(this, ClientState::Connecting);
	TCPSocket::connect(peerAddress, timeout);
}


void TCPStatefulSocket::onConnect()
{
	assert(stateEquals(ClientState::Connecting));
	setState(this, ClientState::Connected);
	TCPSocket::onConnect();
}


void TCPStatefulSocket::onHandshake()
{
	assert(stateEquals(ClientState::Connected));
	setState(this, ClientState::Handshaking);
}


void TCPStatefulSocket::onOnline()
{
	assert(stateEquals(ClientState::Handshaking)
		|| stateEquals(ClientState::Connected));
	setState(this, ClientState::Online);
}


void TCPStatefulSocket::onClose()
{
	Log("trace") << "[TCPStatefulSocket:" << this << "] On Close" << endl;
	setState(this, ClientState::Disconnected, _error);
	TCPSocket::onClose();
}


bool TCPStatefulSocket::isActive()
{
	return stateEquals(ClientState::Connecting)
		|| stateEquals(ClientState::Handshaking)
		|| stateEquals(ClientState::Connected)
		|| stateEquals(ClientState::Online);
}


bool TCPStatefulSocket::isOnline()
{
	return stateEquals(ClientState::Online);
}
*/


} } // namespace Sourcey::Net