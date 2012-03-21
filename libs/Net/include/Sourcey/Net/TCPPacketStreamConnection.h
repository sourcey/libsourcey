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


#ifndef SOURCEY_NET_TCPPacketStreamConnection_H
#define SOURCEY_NET_TCPPacketStreamConnection_H


#include "Sourcey/Base.h"
#include "Sourcey/PacketStream.h"
#include "Sourcey/Logger.h"
#include "Poco/Net/TCPServerConnection.h"


namespace Sourcey {
namespace Net {


class TCPPacketStreamConnection: public Poco::Net::TCPServerConnection, public ILoggable
{
public:
	TCPPacketStreamConnection(PacketStream* stream, 
							  const Poco::Net::StreamSocket& socket, 
							  bool closeDisconnect = true, 
							  bool resetOnConnect = true);
	virtual ~TCPPacketStreamConnection();

	virtual void run();
	virtual void stop();

	virtual int send(const char* data, size_t size);

	virtual PacketStream* stream() const { return _stream; };

	virtual void onStreamPacket(void*, IPacket& packet);
	virtual void onStreamStateChange(void*, PacketStreamState& state, const PacketStreamState&);	
	
	virtual const char* className() const { return "TCPPacketStreamConnection"; }

protected:	
	PacketStream*	_stream;
	Poco::Event		_stop;
	bool			_resetOnConnect;
	bool			_closeDisconnect;
	mutable Poco::FastMutex _mutex;
};


} } // namespace Sourcey::Net


#endif // SOURCEY_NET_TCPPacketStreamConnection_H
