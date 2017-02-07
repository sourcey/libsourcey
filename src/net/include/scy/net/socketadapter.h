///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup net
/// @{


#ifndef SCY_Net_SocketAdapter_H
#define SCY_Net_SocketAdapter_H


#include "scy/base.h"
#include "scy/memory.h"
#include "scy/net/address.h"
#include "scy/net/dns.h"
#include "scy/net/types.h"
#include "scy/packetstream.h"
#include "scy/signal.h"


namespace scy {
namespace net {


class SCY_EXTERN Socket;


/// SocketAdapter is the abstract interface for all socket classes.
/// A SocketAdapter can also be attached to a Socket in order to
/// override default Socket callbacks and behaviour, while still
/// maintaining the default Socket interface (see Socket::setAdapter).
///
/// This class also be extended to implement custom processing
/// for received socket data before it is dispatched to the application
/// (see PacketSocketAdapter and Transaction classes).
class SCY_EXTERN SocketAdapter
{
public:
    /// Creates the SocketAdapter.
    SocketAdapter(SocketAdapter* sender = nullptr,
                  SocketAdapter* receiver = nullptr);

    /// Destroys the SocketAdapter.
    virtual ~SocketAdapter();

    /// Sends the given data buffer to the connected peer.
    /// Returns the number of bytes sent or -1 on error.
    /// No exception will be thrown.
    /// For TCP sockets the given peer address must match the
    /// connected peer address.
    virtual int send(const char* data, std::size_t len, int flags = 0);
    virtual int send(const char* data, std::size_t len,
                     const Address& peerAddress, int flags = 0);

    /// Sends the given packet to the connected peer.
    /// Returns the number of bytes sent or -1 on error.
    /// No exception will be thrown.
    /// For TCP sockets the given peer address must match the
    /// connected peer address.
    virtual int sendPacket(const IPacket& packet, int flags = 0);
    virtual int sendPacket(const IPacket& packet, const Address& peerAddress,
                           int flags = 0);

    /// Sends the given packet to the connected peer.
    /// This method provides delegate compatability, and unlike
    /// other send methods throws an exception if the underlying
    /// socket is closed.
    virtual void sendPacket(IPacket& packet);

    /// These virtual methods can be overridden as necessary
    /// to intercept socket events before they hit the application.
    virtual void onSocketConnect(Socket& socket);
    virtual void onSocketRecv(Socket& socket, const MutableBuffer& buffer, const Address& peerAddress);
    virtual void onSocketError(Socket& socket, const Error& error);
    virtual void onSocketClose(Socket& socket);

    /// A pointer to the adapter for handling outgoing data.
    /// Send methods proxy data to this adapter by default.
    /// Note that we only keep a simple pointer so
    /// as to avoid circular references preventing destruction.
    virtual void setSender(SocketAdapter* adapter, bool freeExisting = false);

    /// Returns the output SocketAdapter pointer
    SocketAdapter* sender();

    /// Adds an input SocketAdapter for receiving socket callbacks.
    virtual void addReceiver(SocketAdapter* adapter, int priority = 0);

    /// Removes an input SocketAdapter.
    virtual void removeReceiver(SocketAdapter* adapter);

    /// Optional client data pointer.
    ///
    /// The pointer is not initialized or managed
    /// by the socket base.
    void* opaque;

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
    /// Returns the polymorphic instance pointer
    /// for signal delegate callbacks.
    virtual void* self() { return this; };

    virtual void emitSocketConnect(Socket& socket);
    virtual void emitSocketRecv(Socket& socket, const MutableBuffer& buffer, const Address& peerAddress);
    virtual void emitSocketError(Socket& socket, const scy::Error& error);
    virtual void emitSocketClose(Socket& socket);

    SocketAdapter* _sender;
};


} // namespace net
} // namespace scy


#endif // SCY_Net_SocketAdapter_H


/// @\}
