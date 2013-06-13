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


#ifndef SOURCEY_NET_TCPPacketStreamConnection_H
#define SOURCEY_NET_TCPPacketStreamConnection_H


#include "Sourcey/Base.h"
#include "Sourcey/PacketStream.h"
#include "Sourcey/Logger.h"
#include "Poco/Net/TCPServerConnection.h"


namespace Scy {
namespace Net {


class TCPPacketStreamConnection: public Poco::Net::TCPServerConnection, public IPolymorphic
{
public:
	TCPPacketStreamConnection(PacketStream* stream, 
							  const Poco::Net::StreamSocket& socket, 
							  //bool closeDisconnect = true, 
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
	//bool			_closeOnDisconnect;
	mutable Poco::FastMutex _mutex;
};


} } // namespace Scy::Net


#endif // SOURCEY_NET_TCPPacketStreamConnection_H
