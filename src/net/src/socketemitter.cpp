///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup net
/// @{


#include "scy/net/socketemitter.h"


using std::endl;


namespace scy {
namespace net {


SocketEmitter::SocketEmitter(const Socket::Ptr& socket)
    : SocketAdapter(socket.get())
    , impl(socket)
{
    if (impl)
        impl->setReceiver(this);
}


SocketEmitter::~SocketEmitter()
{
    if (impl)
        impl->removeReceiver(this);
}


void SocketEmitter::swap(const Socket::Ptr& socket)
{
    assert(!impl && "must not be initialized");
    impl = socket;
    impl->setReceiver(this);
}


void SocketEmitter::addReceiver(SocketAdapter* adapter, int priority)
{
    Connect.attach(slot(adapter, &net::SocketAdapter::onSocketConnect, -1, priority));
    Recv.attach(slot(adapter, &net::SocketAdapter::onSocketRecv, -1, priority));
    Error.attach(slot(adapter, &net::SocketAdapter::onSocketError, -1, priority));
    Close.attach(slot(adapter, &net::SocketAdapter::onSocketClose, -1, priority));
}


void SocketEmitter::removeReceiver(SocketAdapter* adapter)
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


void SocketEmitter::onSocketConnect(Socket& socket)
{
    assert(&socket == impl.get());
    if (_receiver)
        _receiver->onSocketConnect(socket);
    else
        Connect.emit(socket);
}


void SocketEmitter::onSocketRecv(Socket& socket, const MutableBuffer& buffer, const Address& peerAddress)
{
    assert(&socket == impl.get());
    if (_receiver)
        _receiver->onSocketRecv(socket, buffer, peerAddress);
    else
        Recv.emit(socket, buffer, peerAddress);
}


void SocketEmitter::onSocketError(Socket& socket, const scy::Error& error) // const Error& error
{
    assert(&socket == impl.get());
    if (_receiver)
        _receiver->onSocketError(socket, error);
    else
        Error.emit(socket, error);
}


void SocketEmitter::onSocketClose(Socket& socket)
{
    assert(&socket == impl.get());
    if (_receiver)
        _receiver->onSocketClose(socket);
    else
        Close.emit(socket);
}


} // namespace net
} // namespace scy


/// @\}
