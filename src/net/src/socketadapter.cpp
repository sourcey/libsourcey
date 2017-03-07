///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup net
/// @{


#include "scy/net/socketadapter.h"
#include "scy/net/socket.h"


using std::endl;


namespace scy {
namespace net {


SocketAdapter::SocketAdapter(SocketAdapter* sender, SocketAdapter* receiver)
    : opaque(nullptr)
    , _sender(sender)
    , _receiver(receiver)
{
    // TraceS(this) << "Create" << endl;
    assert(sender != this);
    assert(receiver != this);
}


SocketAdapter::~SocketAdapter()
{
    // TraceS(this) << "Destroy" << endl;

#if 0
    // Delete child adapters
    // In order to prevent deletion, the outside
    // application must nullify the adapter pointers
    if (_sender)
        delete _sender;
    if (_receiver)
        delete _receiver;
#endif
}


std::size_t SocketAdapter::send(const char* data, std::size_t len, int flags)
{
    assert(_sender); // should have output adapter if default impl is used
    if (!_sender)
        return -1;
    return _sender->send(data, len, flags);
}


std::size_t SocketAdapter::send(const char* data, std::size_t len, const Address& peerAddress, int flags)
{
    assert(_sender); // should have output adapter if default impl is used
    if (!_sender)
        return -1;
    return _sender->send(data, len, peerAddress, flags);
}


std::size_t SocketAdapter::sendPacket(const IPacket& packet, int flags)
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


std::size_t SocketAdapter::sendPacket(const IPacket& packet, const Address& peerAddress, int flags)
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
    if (_receiver)
        _receiver->onSocketConnect(socket);
}


void SocketAdapter::onSocketRecv(Socket& socket, const MutableBuffer& buffer, const Address& peerAddress)
{
    if (_receiver)
        _receiver->onSocketRecv(socket, buffer, peerAddress);
}


void SocketAdapter::onSocketError(Socket& socket, const scy::Error& error) // const Error& error
{
    if (_receiver)
        _receiver->onSocketError(socket, error);
}


void SocketAdapter::onSocketClose(Socket& socket)
{
    if (_receiver)
        _receiver->onSocketClose(socket);
}


void SocketAdapter::setSender(SocketAdapter* adapter) //, bool freeExisting
{
    assert(adapter != this);
    if (_sender == adapter)
        return;
    //if (_sender && freeExisting)
    //    delete _sender;
    _sender = adapter;
}


void SocketAdapter::setReceiver(SocketAdapter* adapter) //, bool freeExisting
{
    assert(adapter != this);
    if (_receiver == adapter)
        return;
    //if (_receiver && freeExisting)
    //    delete _receiver;
    _receiver = adapter;
}


void SocketAdapter::removeReceiver(SocketAdapter* adapter)
{
    assert(adapter != this);
    if (_receiver == adapter)
        _receiver = nullptr;
}


SocketAdapter* SocketAdapter::sender()
{
    return _sender;
}


SocketAdapter* SocketAdapter::receiver()
{
    return _receiver;
}


#if 0
//
// SocketSignalAdapter
//


SocketSignalAdapter::SocketSignalAdapter(SocketAdapter* sender, SocketAdapter* receiver)
    : SocketAdapter(sender, receiver)
{
}


SocketSignalAdapter::~SocketSignalAdapter()
{
}


void SocketSignalAdapter::addReceiver(SocketAdapter* adapter, int priority)
{
    Connect.attach(slot(adapter, &net::SocketAdapter::onSocketConnect, -1, priority));
    Recv.attach(slot(adapter, &net::SocketAdapter::onSocketRecv, -1, priority));
    Error.attach(slot(adapter, &net::SocketAdapter::onSocketError, -1, priority));
    Close.attach(slot(adapter, &net::SocketAdapter::onSocketClose, -1, priority));
}


void SocketSignalAdapter::removeReceiver(SocketAdapter* adapter)
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


void SocketSignalAdapter::onSocketConnect(Socket& socket)
{
    if (_receiver)
        _receiver->onSocketConnect(socket);
    else
        Connect.emit(socket);
}


void SocketSignalAdapter::onSocketRecv(Socket& socket, const MutableBuffer& buffer, const Address& peerAddress)
{
    if (_receiver)
        _receiver->onSocketRecv(socket, buffer, peerAddress);
    else
        Recv.emit(socket, buffer, peerAddress);
}


void SocketSignalAdapter::onSocketError(Socket& socket, const scy::Error& error) // const Error& error
{
    if (_receiver)
        _receiver->onSocketError(socket, error);
    else
        Error.emit(socket, error);
}


void SocketSignalAdapter::onSocketClose(Socket& socket)
{
    if (_receiver)
        _receiver->onSocketClose(socket);
    else
        Close.emit(socket);
}
#endif


} // namespace net
} // namespace scy


/// @\}
