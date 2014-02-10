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

#include "scy/net/packetsocket.h"

using std::endl;


namespace scy {
namespace net {

	
//
// Packet Socket Adapter
//


PacketSocketAdapter::PacketSocketAdapter(const net::Socket::Ptr& socket) : 
	SocketAdapter(socket.get()), socket(socket)
{
	TraceLS(this) << "Create: " << socket << endl;
}

		
void PacketSocketAdapter::onSocketRecv(const MutableBuffer& buffer, const Address& peerAddress)
{	
	TraceLS(this) << "Recv: " << buffer.size() << endl;
	
	IPacket* pkt = nullptr;
	const char* buf = bufferCast<const char*>(buffer);
	std::size_t len = buffer.size();
	std::size_t nread = 0;
	while (len > 0 && (pkt = factory.createPacket(constBuffer(buf, len), nread))) {
		assert(nread > 0);
		pkt->info = new PacketInfo(socket, peerAddress);
		onPacket(*pkt);
		delete pkt;
		buf += nread;
		len -= nread;
	}
}


void PacketSocketAdapter::onPacket(IPacket& pkt) 
{		
	//TraceLS(this) << "onPacket: emitting: " << pkt.size() << endl;
	PacketSignal::emit(socket.get(), pkt);
}

	
#if 0
//
// Packet Socket
//
	

PacketSocket::PacketSocket(const Socket& socket) : 
	Socket(socket)
{
	addReceiver(new PacketSocketAdapter);
	//assert(Socket::base().refCount() >= 2);
}


PacketSocket::PacketSocket(Socket* base, bool shared) : 
	Socket(base, shared)
{		
	addReceiver(new PacketSocketAdapter);
	//assert(!shared || Socket::base().refCount() >= 2);
}


PacketSocket::~PacketSocket() 
{
}


/*
PacketSocketAdapter& PacketSocket::adapter() const
{
	return *reinterpret_cast<PacketSocketAdapter*>(_adapter);
}	

	
void PacketSocket::sendPacket(IPacket& packet)
{
	//TraceLS(this) << "send" << endl;	
	Socket::send(packet);
}
*/



//
// Packet Stream Socket Adapter
//


PacketStreamSocketAdapter::PacketStreamSocketAdapter(Socket& socket) :
	PacketProcessor(PacketStreamSocketAdapter::emitter), 
	_socket(socket)
{
}


PacketStreamSocketAdapter::~PacketStreamSocketAdapter()
{
}


void PacketStreamSocketAdapter::process(IPacket& packet)
{	
	TraceLS(this) << "Process: " << packet.className() << endl;

	//Mutex::ScopedLock lock(_mutex);

	// TODO: Split packet if needed
	_socket.send(packet);
}


bool PacketStreamSocketAdapter::accepts(IPacket& packet) 
{ 
	return dynamic_cast<RawPacket*>(&packet) != 0; 
}

					
void PacketStreamSocketAdapter::onStreamStateChange(const PacketStreamState& state) 
{ 
	TraceLS(this) << "Stream state change: " << state << endl;
	
	// TODO: Sync socket with stream?

	//Mutex::ScopedLock lock(_mutex);

	switch (state.id()) {
	case PacketStreamState::Running:
		break;
		
	case PacketStreamState::Stopped:
	case PacketStreamState::Error:
	case PacketStreamState::Resetting:
		break;
	//case PacketStreamState::None:
	//case PacketStreamState::Stopping:
	//case PacketStreamState::Closed:
	}
}
#endif


} } // namespace scy::net