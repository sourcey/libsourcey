///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup net
/// @{


#ifndef SCY_Net_SocketEmitter_H
#define SCY_Net_SocketEmitter_H


#include "scy/base.h"
#include "scy/logger.h"
#include "scy/net/socket.h"
#include "scy/net/socketadapter.h"
#include "scy/packetfactory.h"
#include "scy/packetsignal.h"


namespace scy {
namespace net {


/// SocketAdapter class that adds signal callbacks for sockets.
/// Asde from adding a signal interface the class works as a wrapper for
/// the socket instance, and is designed to be used the same way
/// as a `std::unique_ptr` by overriding the ->() operator.
class Net_API SocketEmitter : public SocketAdapter
{
public:
    /// Creates the SocketEmitter
    SocketEmitter(const Socket::Ptr& socket = nullptr);

    /// Copy constructor
    SocketEmitter(const SocketEmitter& that);

    /// Destroys the SocketAdapter.
    virtual ~SocketEmitter();

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

    /// Adds an input SocketAdapter for receiving socket signals.
    virtual void addReceiver(SocketAdapter* adapter) override;

    /// Removes an input SocketAdapter.
    virtual void removeReceiver(SocketAdapter* adapter) override;

    /// Swap the underlying socket pointer.
    virtual void swap(const Socket::Ptr& socket);

    /// Cast getter for the underlying socket.
    template <class T>
    T* as()
    {
        return dynamic_cast<T*>(impl.get());
    }

    /// Accessor to the underlying socket.
    Socket* operator->() const
    {
        return impl.get();
    }

    /// Pointer to the underlying socket.
    /// Sent data will be proxied to this socket.
    Socket::Ptr impl;

protected:

    /// Internal callback events.
    virtual void onSocketConnect(Socket& socket);
    virtual void onSocketRecv(Socket& socket, const MutableBuffer& buffer, const Address& peerAddress);
    virtual void onSocketError(Socket& socket, const scy::Error& error);
    virtual void onSocketClose(Socket& socket);
};


#if 0

/// This class provides request/response functionality for IPacket
/// types emitted from a Socket.
template <class SocketT>
class Net_API SocketEmitter : public SocketT
{
public:
    /// Pointer to the underlying socket.
    /// Sent data will be proxied to this socket.
    // Socket::Ptr socket;

    /// Creates the SocketEmitter
    SocketEmitter();

    /// Destroys the SocketAdapter.
    virtual ~SocketEmitter();

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


template <class T>
inline void SocketEmitter<T>::addReceiver(SocketAdapter* adapter, int priority)
{
    Connect.attach(slot(adapter, &net::SocketAdapter::onSocketConnect, -1, priority));
    Recv.attach(slot(adapter, &net::SocketAdapter::onSocketRecv, -1, priority));
    Error.attach(slot(adapter, &net::SocketAdapter::onSocketError, -1, priority));
    Close.attach(slot(adapter, &net::SocketAdapter::onSocketClose, -1, priority));
}


template <class T>
inline void SocketEmitter<T>::removeReceiver(SocketAdapter* adapter)
{
    Connect.detach(adapter);
    Recv.detach(adapter);
    Error.detach(adapter);
    Close.detach(adapter);

    // Connect -= slot(adapter, &net::SocketAdapter::onSocketConnect);
    // Recv -= slot(adapter, &net::SocketAdapter::onSocketRecv);
    // Error -= slot(adapter, &net::SocketAdapter::onSocketError);
    // Close -= slot(adapter, &net::SocketAdapter::onSocketClose);
}


template <class T>
inline void SocketEmitter<T>::onSocketConnect(Socket& sock)
{
    assert(&sock == socket.get());
    if (_receiver)
        _receiver->onSocketConnect(sock);
    else
        Connect.emit(sock);
}


template <class T>
inline void SocketEmitter<T>::onSocketRecv(Socket& sock, const MutableBuffer& buffer, const Address& peerAddress)
{
    assert(&sock == socket.get());
    if (_receiver)
        _receiver->onSocketRecv(sock, buffer, peerAddress);
    else
        Recv.emit(sock, buffer, peerAddress);
}


template <class T>
inline void SocketEmitter<T>::onSocketError(Socket& sock, const scy::Error& error) // const Error& error
{
    assert(&sock == socket.get());
    if (_receiver)
        _receiver->onSocketError(sock, error);
    else
        Error.emit(sock, error);
}



template <class T>
inline void SocketEmitter<T>::onSocketClose(Socket& sock)
{
    assert(&sock == socket.get());
    if (_receiver)
        _receiver->onSocketClose(sock);
    else
        Close.emit(sock);
}

#endif



} // namespace net
} // namespace scy


#endif // SCY_Net_SocketEmitter_H


/// @\}
