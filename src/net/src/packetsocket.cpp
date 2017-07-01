///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup net
/// @{


#include "scy/net/packetsocket.h"


using std::endl;


namespace scy {
namespace net {


//
// Packet Socket Adapter
//


PacketSocketEmitter::PacketSocketEmitter(const Socket::Ptr& socket)
    : SocketEmitter(socket)
{
    // LTrace("Create: ", socket)
}


PacketSocketEmitter::~PacketSocketEmitter()
{
}


void PacketSocketEmitter::onSocketRecv(Socket& socket, const MutableBuffer& buffer, const Address& peerAddress)
{
    // LTrace("Recv: ", buffer.size())

    IPacket* pkt = nullptr;
    const char* buf = bufferCast<const char*>(buffer);
    size_t len = buffer.size();
    size_t nread = 0;
    while (len > 0 && (pkt = factory.createPacket(constBuffer(buf, len), nread))) {
        assert(nread > 0);
        pkt->info = new PacketInfo(this->impl, peerAddress);
        onPacket(*pkt);
        delete pkt;
        buf += nread;
        len -= nread;
    }
}


void PacketSocketEmitter::onPacket(IPacket& pkt)
{
    // LTrace("On packet: ", pkt.size())
    PacketSignal::emit(pkt);
}


#if 0
//
// Packet Socket
//


PacketSocket::PacketSocket(const Socket& socket) :
    Socket(socket)
{
    addReceiver(new PacketSocketEmitter);
    //assert(Socket::base().refCount() >= 2);
}


PacketSocket::PacketSocket(Socket* base, bool shared) :
    Socket(base, shared)
{
    addReceiver(new PacketSocketEmitter);
    //assert(!shared || Socket::base().refCount() >= 2);
}


PacketSocket::~PacketSocket()
{
}


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
    // LTrace("Process: ", packet.className())

    //Mutex::ScopedLock lock(_mutex);

    // TODO: Split packet if needed
    _socket.send(packet);
}


bool PacketStreamSocketAdapter::accepts(IPacket* packet)
{
    return dynamic_cast<RawPacket*>(&packet) != 0;
}


void PacketStreamSocketAdapter::onStreamStateChange(const PacketStreamState& state)
{
    // LTrace("Stream state change: ", state)

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


} // namespace net
} // namespace scy


/// @\}
