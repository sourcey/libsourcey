///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup net
/// @{


#ifndef SCY_Net_PacketSocket_H
#define SCY_Net_PacketSocket_H


#include "scy/base.h"
#include "scy/logger.h"
#include "scy/net/socket.h"
#include "scy/net/socketadapter.h"
#include "scy/packetfactory.h"
#include "scy/packetsignal.h"


namespace scy {
namespace net {


struct PacketInfo;
class SCY_EXTERN PacketSocket;


//
// Packet Socket Adapter
//


class SCY_EXTERN PacketSocketAdapter : public SocketAdapter, public PacketSignal
{
public:
    /// Pointer to the underlying socket.
    /// Sent data will be proxied to this socket.
    Socket::Ptr socket;

    PacketFactory factory;

    /// Creates the PacketSocketAdapter
    /// This class should have a higher priority than standard
    /// sockets so we can parse data packets first.
    /// Creates and dispatches a packet utilizing the available
    /// creation strategies. For best performance the most used
    /// strategies should have the highest priority.
    PacketSocketAdapter(const Socket::Ptr& socket = nullptr);

    virtual void onSocketRecv(Socket& socket, const MutableBuffer& buffer,
                              const Address& peerAddress);

    virtual void onPacket(IPacket& pkt);
};


#if 0
//
// Packet Socket
//


class SCY_EXTERN PacketSocket: public PacketSocketAdapter
{
public:
    PacketSocket(Socket* socket);
    //PacketSocket(Socket* base, bool shared = false);
    virtual ~PacketSocket();    /// Returns the PacketSocketAdapter for this socket.
    //PacketSocketAdapter& adapter() const;

    /// Compatibility method for PacketSignal delegates.
    //virtual void send(IPacket& packet);

};


//
// Packet Stream Socket Adapter
//

/// Proxies arbitrary PacketStream packets to an output Socket,
/// ensuring the Socket MTU is not exceeded.
/// Oversize packets will be split before sending.
class SCY_EXTERN PacketStreamSocketAdapter: public PacketProcessor, public PacketSignal
{
public:
    PacketStreamSocketAdapter(Socket& socket);
    virtual ~PacketStreamSocketAdapter();

protected:
    virtual bool accepts(IPacket& packet);
    virtual void process(IPacket& packet);
    virtual void onStreamStateChange(const PacketStreamState& state);

    friend class PacketStream;

    Socket _socket;
};
#endif


} // namespace Net
} // namespace scy


#endif // SCY_Net_PacketSocket_H


/// @\}
