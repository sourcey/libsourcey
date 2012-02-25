//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2005 Sourcey
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


#include "Sourcey/Net/TCPPacketStreamConnection.h"
#include "Sourcey/Net/Types.h"
#include "Sourcey/Logger.h"


using namespace std;
using namespace Poco;


namespace Sourcey {
namespace Net {


TCPPacketStreamConnection::TCPPacketStreamConnection(PacketStream* stream, 
													 const Poco::Net::StreamSocket& socket, 
													 bool closeDisconnect, 
													 bool resetOnConnect) :
	Poco::Net::TCPServerConnection(socket),
	_stream(stream), 
	_closeDisconnect(closeDisconnect), 
	_resetOnConnect(resetOnConnect) 
{
	Log("debug") << "[TCPPacketStreamConnection:" << this << "] Creating" << endl;
}
	

TCPPacketStreamConnection::~TCPPacketStreamConnection() 
{ 
	Log("debug") << "[TCPPacketStreamConnection:" << this << "] Destroying" << endl;
	stop();
}


void TCPPacketStreamConnection::run() 
{
	Log("debug") << "[TCPPacketStreamConnection:" << this << "] Starting" << endl;
	
	try 
	{
		_stream->StateChange += delegate(this, &TCPPacketStreamConnection::onStreamStateChange);	
		_stream->attach(packetDelegate(this, &TCPPacketStreamConnection::onStreamPacket));
		if (_resetOnConnect)
			_stream->reset();

		_stop.wait();

		assert(_stream == NULL);
	}
	catch (Exception& exc) {
		Log("error", this) << "[TCPPacketStreamConnection:" << this << "] Error: " << exc.displayText() << endl;
	}

	Log("debug") << "[TCPPacketStreamConnection:" << this << "] TCP Packet Connection Exiting: " << this << endl;
}


void TCPPacketStreamConnection::stop()
{
	FastMutex::ScopedLock lock(_mutex);
	
	if (_stream) {
		_stream->StateChange -= delegate(this, &TCPPacketStreamConnection::onStreamStateChange);	
		_stream->detach(packetDelegate(this, &TCPPacketStreamConnection::onStreamPacket));
		_stream = NULL;
		_stop.set();
	}
}


int TCPPacketStreamConnection::send(const char* data, size_t size)
{						
	// Drop empty packets.
	if (!size) {			
		Log("warn") << "[TCPPacketStreamConnection:" << this << "] Dropping Empty Packet" << endl;
		return 0;
	}

	// Drop oversize packets.
	else if (size > MAX_TCP_PACKET_SIZE) {
		Log("warn") << "[TCPPacketStreamConnection:" << this << "] Dropping Oversize Data Packet: " 
			<< "\n\tSize: " << size
			<< "\n\tMax Size: " << MAX_TCP_PACKET_SIZE
			<< endl;
		return 0;
	}

	//Log("trace") << "[TCPPacketStreamConnection:" << this << "] Sending Packet: " 
	//	<< packet.className() << ": " << size << endl;	
	return socket().sendBytes(data, size);
}


void TCPPacketStreamConnection::onStreamPacket(void*, IPacket& packet) 
{
	try {
		// If the packet is a DataPacket we can access
		// the data pointer directly.
		DataPacket* p = packet.as<DataPacket>();
		if (p) {
			send((const char*)p->data(), p->size());
		}
		
		// Otherwise copy the packet onto an output buffer
		// before sending it out over the network.
		else {
			size_t size = packet.size();
			Buffer buf(size ? size : 1500);
			packet.write(buf);
			send((const char*)buf.data(), buf.size());
		}
	}
	catch (Exception& exc) {
		Log("error", this) << "[TCPPacketStreamConnection:" << this << "] Error: " << exc.displayText() << endl;		
		if (_stream && _closeDisconnect)
			_stream->close();
		else
			stop();
	}
}
	

void TCPPacketStreamConnection::onStreamStateChange(void*, PacketStreamState& state, const PacketStreamState&)
{		
	Log("debug") << "[TCPPacketStreamConnection:" << this << "] Session State Changed: " << state << endl;

	if (state.id() == PacketStreamState::Closing) {
		stop();
	}
}


} } // namespace Sourcey::Net