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


#ifndef SOURCEY_NET_TCPClientSocket_H
#define SOURCEY_NET_TCPClientSocket_H


#include "Sourcey/Base.h"
#include "Sourcey/Stateful.h"
#include "Sourcey/Net/TCPSocket.h"


namespace Sourcey {
namespace Net {


struct ClientState: public StateT 
{
	enum Type 
	{
		None				= 0x00,
		Connecting			= 0x01,
		Connected			= 0x04,
		Handshaking			= 0x08,
		Online				= 0x10,
		Disconnected		= 0x20
	};

	std::string str(unsigned int id) const 
	{ 
		switch(id) {
		case None:			return "None";
		case Connecting:	return "Connecting";
		case Connected:		return "Connected";
		case Handshaking:	return "Handshaking";
		case Online:		return "Online";
		case Disconnected:	return "Disconnected";
		}
		return "undefined"; 
	};
};


class TCPClientSocket: public TCPSocket, public StatefulSignal<ClientState>
{
public:
	TCPClientSocket(Reactor& reactor/* = Reactor::getDefault()*/);
	TCPClientSocket(const TCPClientSocket& r);
	virtual ~TCPClientSocket();
	
	virtual void connect(const Address& peerAddress, int timeout = 0);

	//virtual void send(IPacket& packet);

	bool isActive();
	bool isOnline();

protected:
	virtual void onConnect();
	virtual void onHandshake();		// never called in default impl
	virtual void onOnline();		// never called in default impl
	virtual void onClose();
};


} } // namespace Sourcey::Net


#endif // SOURCEY_NET_TCPClientSocket_H