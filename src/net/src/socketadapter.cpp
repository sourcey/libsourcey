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


#include "scy/net/socketadapter.h"
#include "scy/net/socket.h"


using std::endl;


namespace scy {
namespace net {


SocketAdapter::SocketAdapter(SocketAdapter* sender, SocketAdapter* receiver) :
     _sender(sender)
{
    //TraceS(this) << "Create" << endl;
    assert(sender != this);
    //assert(receiver != this);

    if (receiver)
        addReceiver(receiver);
}


SocketAdapter::~SocketAdapter()
{
    //TraceS(this) << "Destroy" << endl;

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
    if (!_sender) return -1;
    return _sender->send(data, len, flags);
}


int SocketAdapter::send(const char* data, std::size_t len, const Address& peerAddress, int flags)
{
    assert(_sender); // should have output adapter if default impl is used
    if (!_sender) return -1;
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


int SocketAdapter::sendPacket(const IPacket& packet, const Address& peerAddress, int flags)
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


void SocketAdapter::onSocketConnect()
{
    emitSocketConnect();
}


void SocketAdapter::onSocketRecv(const MutableBuffer& buffer, const Address& peerAddress)
{
    emitSocketRecv(buffer, peerAddress);
}


void SocketAdapter::onSocketError(const scy::Error& error) //const Error& error
{
    emitSocketError(error);
}


void SocketAdapter::onSocketClose()
{
    emitSocketClose();
}


void SocketAdapter::emitSocketConnect()
{
    Connect.emit(self());
}


void SocketAdapter::emitSocketRecv(const MutableBuffer& buffer, const Address& peerAddress)
{
    Recv.emit(self(), buffer, peerAddress);
}


void SocketAdapter::emitSocketError(const scy::Error& error)
{
    Error.emit(self(), error);
}


void SocketAdapter::emitSocketClose()
{
    Close.emit(self());
}



void SocketAdapter::addReceiver(SocketAdapter* adapter, int priority)
{
    Connect += delegate(adapter, &net::SocketAdapter::onSocketConnect, priority);
    Recv += delegate(adapter, &net::SocketAdapter::onSocketRecv, priority);
    Error += delegate(adapter, &net::SocketAdapter::onSocketError, priority);
    Close += delegate(adapter, &net::SocketAdapter::onSocketClose, priority);
}


void SocketAdapter::removeReceiver(SocketAdapter* adapter)
{
    Connect -= delegate(adapter, &net::SocketAdapter::onSocketConnect);
    Recv -= delegate(adapter, &net::SocketAdapter::onSocketRecv);
    Error -= delegate(adapter, &net::SocketAdapter::onSocketError);
    Close -= delegate(adapter, &net::SocketAdapter::onSocketClose);
}


void SocketAdapter::setSender(SocketAdapter* adapter, bool freeExisting)
{
    if (_sender == adapter) return;
    if (_sender && freeExisting)
        delete _sender;
    _sender = adapter;
}


} } // namespace scy::net
