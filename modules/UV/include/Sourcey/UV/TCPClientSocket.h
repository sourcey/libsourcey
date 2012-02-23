//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2002 Sourcey
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


#ifndef SOURCEY_UV_TCPClientSocket_H
#define SOURCEY_UV_TCPClientSocket_H


#include "Sourcey/UV/UVPP.h"
#include "Sourcey/UV/TCPContext.h"

#include "Sourcey/Logger.h"
#include "Sourcey/Stateful.h"
#include "Sourcey/Net/Address.h"


namespace Sourcey {
namespace UV {
	
	
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


class TCPClientSocket: public TCPContext, public StatefulSignal<ClientState>
{
public:
	TCPClientSocket(LoopHandle* loop = DefaultLoop);
	virtual ~TCPClientSocket();	
	
	virtual void connect(const Net::Address& peerAddress);	
	virtual void close();

	virtual void onConnected(int status);

	virtual void setErrno(const ErrorHandle& err);
};


} } // namespace Sourcey::UV


#endif // SOURCEY_UV_TCPClientSocket_H
