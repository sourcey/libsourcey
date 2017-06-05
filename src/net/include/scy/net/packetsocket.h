///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup net
/// @{


#ifndef SCY_Net_PacketSocket_H
#define SCY_Net_PacketSocket_H


#include "scy/base.h"
#include "scy/packetfactory.h"
#include "scy/packetsignal.h"
#include "scy/net/socket.h"
#include "scy/net/socketemitter.h"


namespace scy {
namespace net {


//
// Packet Socket Emitter
//


class Net_API PacketSocketEmitter : public SocketEmitter, public PacketSignal
{
public:
    /// Creates the PacketSocketEmitter
    /// This class should have a higher priority than standard
    /// sockets so we can parse data packets first.
    /// Creates and dispatches a packet utilizing the available
    /// creation strategies. For best performance the most used
    /// strategies should have the highest priority.
    PacketSocketEmitter(const Socket::Ptr& socket = nullptr);

    virtual ~PacketSocketEmitter();

    virtual void onSocketRecv(Socket& socket, const MutableBuffer& buffer, const Address& peerAddress) override;

    virtual void onPacket(IPacket& pkt);

    /// The packet factory.
    PacketFactory factory;
};


#if 0
//
// Packet Socket
//


class Net_API PacketSocket: public PacketSocketEmitter
{
public:
    PacketSocket(Socket* socket);
    //PacketSocket(Socket* base, bool shared = false);
    virtual ~PacketSocket();    /// Returns the PacketSocketEmitter for this socket.
    //PacketSocketEmitter& adapter() const;

    /// Compatibility method for PacketSignal delegates.
    //virtual void send(IPacket& packet);

};


//
// Packet Stream Socket Adapter
//

/// Proxies arbitrary PacketStream packets to an output Socket,
/// ensuring the Socket MTU is not exceeded.
/// Oversize packets will be split before sending.
class Net_API PacketStreamSocketAdapter: public PacketProcessor, public PacketSignal
{
public:
    PacketStreamSocketAdapter(Socket& socket);
    virtual ~PacketStreamSocketAdapter();

protected:
    virtual bool accepts(IPacket* packet);
    virtual void process(IPacket& packet);
    virtual void onStreamStateChange(const PacketStreamState& state);

    friend class PacketStream;

    Socket _socket;
};
#endif


} // namespace net
} // namespace scy


#endif // SCY_Net_PacketSocket_H


/// @\}
