///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup net
/// @{

#include "scy/net/socketadapter.h"
#include "scy/net/socket.h"

using std::endl;

namespace scy {
namespace net {

SocketAdapter::SocketAdapter(SocketAdapter* sender, SocketAdapter* receiver)
    : _sender(sender)
{
    // TraceS(this) << "Create" << endl;
    assert(sender != this);
    // assert(receiver != this);

    if (receiver)
        addReceiver(receiver);
}

SocketAdapter::~SocketAdapter()
{
// TraceS(this) << "Destroy" << endl;

#if 0
    // Delete child adapters
    // In order to prevent deletion, the outside
    // application must nullify the adapter pointers
    if (_recvAdapter)
        delete _recvAdapter;
    if (_sender)
        delete _sender;
#endif
}

int SocketAdapter::send(const char* data, std::size_t len, int flags)
{
    assert(_sender); // should have output adapter if default impl is used
    if (!_sender)
        return -1;
    return _sender->send(data, len, flags);
}

int SocketAdapter::send(const char* data, std::size_t len,
                        const Address& peerAddress, int flags)
{
    assert(_sender); // should have output adapter if default impl is used
    if (!_sender)
        return -1;
    return _sender->send(data, len, peerAddress, flags);
}

int SocketAdapter::sendPacket(const IPacket& packet, int flags)
{
    // Try to cast as RawPacket so we can send without copying any data.
    auto raw = dynamic_cast<const RawPacket*>(&packet);
    if (raw)
        return send((const char*)raw->data(), raw->size(), flags);

    // Dynamically generated packets need to be written to a
    // temp buffer for sending.
    else {
        Buffer buf;
        packet.write(buf);
        return send(buf.data(), buf.size(), flags);
    }
}

int SocketAdapter::sendPacket(const IPacket& packet, const Address& peerAddress,
                              int flags)
{
    // Try to cast as RawPacket so we can send without copying any data.
    auto raw = dynamic_cast<const RawPacket*>(&packet);
    if (raw)
        return send((const char*)raw->data(), raw->size(), peerAddress, flags);

    // Dynamically generated packets need to be written to a
    // temp buffer for sending.
    else {
        Buffer buf;
        buf.reserve(2048);
        packet.write(buf);
        return send(buf.data(), buf.size(), peerAddress, flags);
    }
}

void SocketAdapter::sendPacket(IPacket& packet)
{
    int res = sendPacket(packet, 0);
    if (res < 0)
        throw std::runtime_error("Invalid socket operation");
}

void SocketAdapter::onSocketConnect(Socket& socket)
{
    emitSocketConnect(socket);
}

void SocketAdapter::onSocketRecv(Socket& socket, const MutableBuffer& buffer,
                                 const Address& peerAddress)
{
    emitSocketRecv(socket, buffer, peerAddress);
}

void SocketAdapter::onSocketError(Socket& socket,
                                  const scy::Error& error) // const Error& error
{
    emitSocketError(socket, error);
}

void SocketAdapter::onSocketClose(Socket& socket)
{
    emitSocketClose(socket);
}

void SocketAdapter::emitSocketConnect(Socket& socket)
{
    Connect.emit(socket /*self()*/);
}

void SocketAdapter::emitSocketRecv(Socket& socket, const MutableBuffer& buffer,
                                   const Address& peerAddress)
{
    Recv.emit(/*self(), */ socket, buffer, peerAddress);
}

void SocketAdapter::emitSocketError(Socket& socket, const scy::Error& error)
{
    Error.emit(/*self(), */ socket, error);
}

void SocketAdapter::emitSocketClose(Socket& socket)
{
    Close.emit(socket /*self()*/);
}

void SocketAdapter::addReceiver(SocketAdapter* adapter, int priority)
{
    Connect += slot(adapter, &net::SocketAdapter::onSocketConnect, priority);
    Recv += slot(adapter, &net::SocketAdapter::onSocketRecv, priority);
    Error += slot(adapter, &net::SocketAdapter::onSocketError, priority);
    Close += slot(adapter, &net::SocketAdapter::onSocketClose, priority);
}

void SocketAdapter::removeReceiver(SocketAdapter* adapter)
{
    Connect -= slot(adapter, &net::SocketAdapter::onSocketConnect);
    Recv -= slot(adapter, &net::SocketAdapter::onSocketRecv);
    Error -= slot(adapter, &net::SocketAdapter::onSocketError);
    Close -= slot(adapter, &net::SocketAdapter::onSocketClose);
}

void SocketAdapter::setSender(SocketAdapter* adapter, bool freeExisting)
{
    if (_sender == adapter)
        return;
    if (_sender && freeExisting)
        delete _sender;
    _sender = adapter;
}

} // namespace net
} // namespace scy

/// @\}
