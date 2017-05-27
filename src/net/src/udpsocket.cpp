///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup net
/// @{


#include "scy/net/udpsocket.h"
#include "scy/logger.h"
#include "scy/net/net.h"


using namespace std;


namespace scy {
namespace net {


UDPSocket::UDPSocket(uv::Loop* loop)
    : uv::Handle2<uv_udp_t>(loop)
    , _buffer(65536)
{
    TraceA("Create")
    init();
}


UDPSocket::~UDPSocket()
{
    TraceA("Destroy")
    close();
}


void UDPSocket::init()
{
    if (initialized())
        return;

    TraceA("Init")

    if (!get()) {
        uv::Handle2<uv_udp_t>::reset();
        get()->data = this;
    }
    uv::Handle2<uv_udp_t>::init(&uv_udp_init);
}


void UDPSocket::reset()
{
    TraceA("Reset")
    uv::Handle2<uv_udp_t>::reset();
    get()->data = this;
    init();
}


void UDPSocket::connect(const Address& peerAddress)
{
    TraceA("Connect:", peerAddress)
    init();
    _peer = peerAddress;

    // Emit the Connected signal to mimic TCPSocket behaviour
    // since socket implementations are interchangable.
    onSocketConnect(*this);
}


void UDPSocket::connect(const std::string& host, uint16_t port)
{
    if (Address::validateIP(host)) {
        connect(Address(host, port));
    }
    else if (host == "localhost") {
        // NOTE: Forcefully translate localhost to 127.0.0.1 since
        // the DNS service returns 0.0.0.0 on some systems resulting
        // in connection failure.
        connect(Address("127.0.0.1", port));
    }
    else {
        init();
        typedef uv::Request<uv_udp_t, uv_getaddrinfo_t> Request;
        auto wrap = new Request(this);
        auto func = [](uv_getaddrinfo_t* req, int status, struct addrinfo* res) {
            auto wrap = reinterpret_cast<Request*>(req->data);
            if (!wrap->ctx->deleted) {
                if (status == 0) {
                    net::Address resolved(res->ai_addr, 16);
                    reinterpret_cast<UDPSocket*>(wrap->parent)->connect(resolved);
                }
                else {
                    DebugA("DNS failed to resolve")
                }
            }
            else {
                DebugA("Dropping DNS request for closed UDP socket")
            }
            uv_freeaddrinfo(res);
            delete wrap;
        };
        wrap->invoke(&uv_getaddrinfo, loop(), &wrap->req, func,
            host.c_str(), util::itostr<uint16_t>(port).c_str(), nullptr);
    }
}


void UDPSocket::close()
{
    TraceA("Closing")
    if (initialized() && !closed())
        recvStop();
    uv::Handle2<uv_udp_t>::close();
}


void UDPSocket::bind(const Address& address, unsigned flags)
{
    TraceA("Binding on", address)
    init();

    if (address.af() == AF_INET6)
        flags |= UV_UDP_IPV6ONLY;

    if (invoke(&uv_udp_bind, get(), address.addr(), flags))
        recvStart();
}


ssize_t UDPSocket::send(const char* data, size_t len, int flags)
{
    assert(_peer.valid());
    return send(data, len, _peer, flags);
}


ssize_t UDPSocket::send(const char* data, size_t len, const Address& peerAddress, int /* flags */)
{
    return len;

    TraceA("Send:", len, ":", peerAddress)
    assert(Thread::currentID() == tid());
    assert(get());
    assert(initialized());
    assert(active());
    assert(!closed());
    // assert(len <= net::MAX_UDP_PACKET_SIZE);

    if (_peer.valid() && _peer != peerAddress) {
        ErrorA("Peer not authorized:", peerAddress)
        return -1;
    }

    if (!peerAddress.valid()) {
        ErrorA("Peer not valid:", peerAddress)
        return -1;
    }

    typedef uv::Request<uv_udp_t, uv_udp_send_t> Request;

    auto wrap = new Request(this);
    wrap->buf = uv_buf_init((char*)data, (unsigned int)len); // TODO: memcpy data?
    if (invoke(&uv_udp_send, &wrap->req, get(), &wrap->buf, 1, peerAddress.addr(),
        [](uv_udp_send_t* req, int status) {
            auto wrap = reinterpret_cast<Request*>(req->data);
            if (!wrap->ctx->deleted) {
                if (status) {
                    DebugA("Send error:", uv_err_name(status))
                    wrap->ctx->handle->setUVError("UDP send error", status);
                }
            }
            else {
                DebugA("Dropping send request for closed UDP socket")
            }
            delete wrap;
        })) {
        return len;
    }

    return error().errorno;
}


bool UDPSocket::setBroadcast(bool enable)
{
    assert(initialized());
    return uv_udp_set_broadcast(get(), enable ? 1 : 0) == 0;
}


bool UDPSocket::setMulticastLoop(bool enable)
{
    assert(initialized());
    return uv_udp_set_multicast_loop(get(), enable ? 1 : 0) == 0;
}


bool UDPSocket::setMulticastTTL(int ttl)
{
    assert(initialized());
    assert(ttl > 0 && ttl <= 255);
    return uv_udp_set_multicast_ttl(get(), ttl) == 0;
}


bool UDPSocket::recvStart()
{
    assert(initialized());
    // assert(active());
    assert(!closed());
    assert(get()->data == this);

    // UV_EALREADY means that the socket is already bound but that's okay
    // TODO: No need for boolean value as this method can throw exceptions
    // since it is called internally by bind().
    int r = uv_udp_recv_start(get(), UDPSocket::allocRecvBuffer, UDPSocket::onRecv);
    if (r && r != UV_EALREADY) {
        setUVError("Cannot start recv on invalid UDP socket", r);
        return false;
    }
    return true;

    // return invoke(&uv_udp_recv_start, get(), UDPSocket::allocRecvBuffer, UDPSocket::onRecv);
}


bool UDPSocket::recvStop()
{
    assert(initialized());
    // assert(active());
    assert(!closed());

    // This method must not throw since it is called
    // internally via libuv callbacks.
    return get() && uv_udp_recv_stop(get()) == 0;
}


void UDPSocket::onRecv(const MutableBuffer& buf, const net::Address& address)
{
    TraceA("Recv:", buf.size())
    // emitRecv(buf, address);
    onSocketRecv(*this, buf, address);
}


void UDPSocket::setError(const scy::Error& err)
{
    uv::Handle2<uv_udp_t>::setError(err);
}


const scy::Error& UDPSocket::error() const
{
    return uv::Handle2<uv_udp_t>::error();
}


net::Address UDPSocket::address() const
{
    if (initialized()) {
        struct sockaddr address;
        int addrlen = sizeof(address);
        if (uv_udp_getsockname(get(), &address, &addrlen) == 0)
            return net::Address(&address, addrlen);
    }
    return net::Address();
}


net::Address UDPSocket::peerAddress() const
{
    return _peer;
}


net::TransportType UDPSocket::transport() const
{
    return net::UDP;
}


bool UDPSocket::closed() const
{
    return uv::Handle2<uv_udp_t>::closed();
}


//
// Callbacks

void UDPSocket::onRecv(uv_udp_t* handle, ssize_t nread, const uv_buf_t* buf,
                       const struct sockaddr* addr, unsigned /* flags */)
{
    auto socket = static_cast<UDPSocket*>(handle->data);
    assert(socket->initialized());
    assert(!socket->closed());
    TraceA("On recv:", nread)

    if (nread < 0) {
        // assert(0 && "unexpected error");
        DebugA("Recv error:", uv_err_name((int)nread))
        socket->setUVError("UDP recv error", (int)nread);
        return;
    }
    else if (nread == 0) {
        assert(addr == nullptr);
        // Returning unused buffer, this is not an error
        // 11/12/13: This happens on linux but not windows
        // socket->setUVError("End of file", UV_EOF);
        return;
    }

    socket->onRecv(mutableBuffer(buf->base, nread),
                   net::Address(addr, sizeof(*addr)));
}


// void UDPSocket::afterSend(uv_udp_send_t* req, int status)
// {
//     auto sr = reinterpret_cast<internal::SendRequest*>(req);
//     auto socket = reinterpret_cast<UDPSocket*>(sr->req.handle->data);
//     if (status) {
//         DebugS(socket) << "Send error: " << uv_err_name(status))
//         socket->setUVError("UDP send error", status);
//     }
//     delete sr;
// }


void UDPSocket::allocRecvBuffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{
    auto& buffer = static_cast<UDPSocket*>(handle->data)->_buffer;
    // TraceA("Allocating buffer:", suggested_size)

    // Reserve the recommended buffer size
    // XXX: libuv wants us to allocate 65536 bytes for UDP .. hmmm
    // if (suggested_size > self->_buffer.available())
    //    self->_buffer.reserve(suggested_size);
    // assert(self->_buffer.capacity() >= suggested_size);
    assert(buffer.size() >= suggested_size);
    assert(buffer.size() < suggested_size * 3);

    // Reset the buffer position on each read
    // self->_buffer.position(0);
    buf->base = buffer.data();
    buf->len = buffer.size();

    // return uv_buf_init(buffer.data(), suggested_size);
}


void UDPSocket::onError(const scy::Error& error)
{
    DebugA("Error", error.message)
    onSocketError(*this, error);
    close(); // close on error
}


void UDPSocket::onClose()
{
    DebugA("On close")
    onSocketClose(*this);
}


uv::Loop* UDPSocket::loop() const
{
    return uv::Handle2<uv_udp_t>::loop();
}


void* UDPSocket::self()
{
    return this;
}


} // namespace net
} // namespace scy


/// @\}
