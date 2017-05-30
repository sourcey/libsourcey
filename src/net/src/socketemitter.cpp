///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
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
        impl->addReceiver(this);
}


SocketEmitter::SocketEmitter(const SocketEmitter& that)
    : SocketAdapter(that)
    , Connect(that.Connect)
    , Recv(that.Recv)
    , Error(that.Error)
    , Close(that.Close)
    , impl(that.impl)
{
    if (impl)
        impl->addReceiver(this);
    assert(that._receivers.empty() || !_receivers.empty());
}


SocketEmitter::~SocketEmitter()
{
    if (impl)
        impl->removeReceiver(this);
}


void SocketEmitter::swap(const Socket::Ptr& socket)
{
    assert(!impl && "must not be initialized");
    if (impl)
        impl->removeReceiver(this);
    if (socket)
        socket->addReceiver(this);
    impl = socket;
}


void SocketEmitter::addReceiver(SocketAdapter* adapter)
{
    // LTrace("Remove receiver: ", adapter)
    assert(adapter->priority <= 100);
    Connect.attach(slot(adapter, &net::SocketAdapter::onSocketConnect, -1, adapter->priority));
    Recv.attach(slot(adapter, &net::SocketAdapter::onSocketRecv, -1, adapter->priority));
    Error.attach(slot(adapter, &net::SocketAdapter::onSocketError, -1, adapter->priority));
    Close.attach(slot(adapter, &net::SocketAdapter::onSocketClose, -1, adapter->priority));
}


void SocketEmitter::removeReceiver(SocketAdapter* adapter)
{
    // LTrace("Remove receiver: ", adapter)
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
    SocketAdapter::onSocketConnect(socket);
    Connect.emit(socket);
}


void SocketEmitter::onSocketRecv(Socket& socket, const MutableBuffer& buffer, const Address& peerAddress)
{
    assert(&socket == impl.get());
    SocketAdapter::onSocketRecv(socket, buffer, peerAddress);
    Recv.emit(socket, buffer, peerAddress);
}


void SocketEmitter::onSocketError(Socket& socket, const scy::Error& error)
{
    assert(&socket == impl.get());
    SocketAdapter::onSocketError(socket, error);
    Error.emit(socket, error);
}


void SocketEmitter::onSocketClose(Socket& socket)
{
    assert(&socket == impl.get());
    SocketAdapter::onSocketClose(socket);
    Close.emit(socket);
}


} // namespace net
} // namespace scy


/// @\}
