///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup net
/// @{


#include "scy/net/signalsocket.h"


using std::endl;


namespace scy {
namespace net {


SignalSocket::SignalSocket(const Socket::Ptr& socket)
    : SocketAdapter(socket.get())
    , socket(socket)
{
    socket->setReceiver(this);
}


SignalSocket::~SignalSocket()
{
    socket->removeReceiver(this);
}


void SignalSocket::addReceiver(SocketAdapter* adapter, int priority)
{
    Connect.attach(slot(adapter, &net::SocketAdapter::onSocketConnect, -1, priority));
    Recv.attach(slot(adapter, &net::SocketAdapter::onSocketRecv, -1, priority));
    Error.attach(slot(adapter, &net::SocketAdapter::onSocketError, -1, priority));
    Close.attach(slot(adapter, &net::SocketAdapter::onSocketClose, -1, priority));
}


void SignalSocket::removeReceiver(SocketAdapter* adapter)
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


void SignalSocket::onSocketConnect(Socket& sock)
{
    assert(&sock == socket.get());
    if (_receiver)
        _receiver->onSocketConnect(sock);
    else
        Connect.emit(sock);
}


void SignalSocket::onSocketRecv(Socket& sock, const MutableBuffer& buffer, const Address& peerAddress)
{
    assert(&sock == socket.get());
    if (_receiver)
        _receiver->onSocketRecv(sock, buffer, peerAddress);
    else
        Recv.emit(sock, buffer, peerAddress);
}


void SignalSocket::onSocketError(Socket& sock, const scy::Error& error) // const Error& error
{
    assert(&sock == socket.get());
    if (_receiver)
        _receiver->onSocketError(sock, error);
    else
        Error.emit(sock, error);
}


void SignalSocket::onSocketClose(Socket& sock)
{
    assert(&sock == socket.get());
    if (_receiver)
        _receiver->onSocketClose(sock);
    else
        Close.emit(sock);
}


} // namespace net
} // namespace scy


/// @\}
