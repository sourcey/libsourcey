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


#ifndef SCY_Net_PacketSocket_H
#define SCY_Net_PacketSocket_H


#include "scy/base.h"
#include "scy/logger.h"
#include "scy/packetsignal.h"
#include "scy/packetfactory.h"
#include "scy/net/socket.h"
#include "scy/net/socketadapter.h"


namespace scy {
namespace net {


struct PacketInfo;
class PacketSocket;


//
// Packet Socket Adapter
//


class PacketSocketAdapter: public SocketAdapter, public PacketSignal
{    
public:    
    net::Socket::Ptr socket;
        // Pointer to the underlying socket.
        // Sent data will be proxied to this socket.

    PacketFactory factory;

    PacketSocketAdapter(const net::Socket::Ptr& socket = nullptr); //const net::Socket::Ptr& socket = nullptr //SocketAdapter* sender = nullptr, SocketAdapter* receiver = nullptr
        // Creates the PacketSocketAdapter
        // This class should have a higher priority than standard
        // sockets so we can parse data packets first.
        
    virtual void onSocketRecv(const MutableBuffer& buffer, const Address& peerAddress);
        // Creates and dispatches a packet utilizing the available 
        // creation strategies. For best performance the most used 
        // strategies should have the highest priority.

    virtual void onPacket(IPacket& pkt);
};


#if 0
//
// Packet Socket
//


class PacketSocket: public PacketSocketAdapter
{
public:    
    PacketSocket(Socket* socket);
    //PacketSocket(Socket* base, bool shared = false);
    virtual ~PacketSocket();

    //PacketSocketAdapter& adapter() const;
        // Returns the PacketSocketAdapter for this socket.        
    
    //virtual void send(IPacket& packet);
        // Compatibility method for PacketSignal delegates.
};


//
// Packet Stream Socket Adapter
//


class PacketStreamSocketAdapter: public PacketProcessor, public PacketSignal
    /// Proxies arbitrary PacketStream packets to an output Socket,
    /// ensuring the Socket MTU is not exceeded.
    /// Oversize packets will be split before sending.
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


} } // namespace scy::Net


#endif // SCY_Net_PacketSocket_H