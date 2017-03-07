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


/// SocketAdapter class that adds signal callbacks for sockets.
class SCY_EXTERN SignalSocket : public SocketAdapter
{
public:
    /// Pointer to the underlying socket.
    /// Sent data will be proxied to this socket.
    Socket::Ptr socket;

    /// Creates the SignalSocket
    SignalSocket(const Socket::Ptr& socket);

    /// Destroys the SocketAdapter.
    virtual ~SignalSocket();

    /// Adds an input SocketAdapter for receiving socket signals.
    virtual void addReceiver(SocketAdapter* adapter, int priority = 0);

    /// Removes an input SocketAdapter.
    virtual void removeReceiver(SocketAdapter* adapter);

    /// Signals that the socket is connected.
    Signal<void(Socket&)> Connect;

    /// Signals when data is received by the socket.
    Signal<void(Socket&, const MutableBuffer&, const Address&)> Recv;

    /// Signals that the socket is closed in error.
    /// This signal will be sent just before the
    /// Closed signal.
    Signal<void(Socket&, const scy::Error&)> Error;

    /// Signals that the underlying socket is closed.
    Signal<void(Socket&)> Close;

protected:

    /// Internal callback events.
    virtual void onSocketConnect(Socket& socket);
    virtual void onSocketRecv(Socket& socket, const MutableBuffer& buffer, const Address& peerAddress);
    virtual void onSocketError(Socket& socket, const scy::Error& error);
    virtual void onSocketClose(Socket& socket);
};


} // namespace net
} // namespace scy


#endif // SCY_Net_PacketSocket_H


/// @\}
