///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup net
/// @{


#include "scy/net/tcpsocket.h"
#include "scy/logger.h"


using std::endl;


namespace scy {
namespace net {


TCPSocket::TCPSocket(uv::Loop* loop)
    : Stream2(loop)
{
    TraceA("Create")
    init();
}


TCPSocket::~TCPSocket()
{
    TraceA("Destroy")
    close();
}


void TCPSocket::init()
{
    if (initialized())
        return;

    TraceA("Init")

    if (!get()) {
        Stream2::reset();
        get()->data = this;
    }
    Stream2::init(&uv_tcp_init_ex, _af);
}


void TCPSocket::reset()
{
    Stream2::reset();
    get()->data = this;
    init();
}


namespace internal {

UVStatusCallbackWithType(TCPSocket, onAcceptConnection, uv_stream_t);

}


void TCPSocket::connect(const net::Address& peerAddress)
{
    TraceA("Connecting to", peerAddress)
    init();

    // auto req = new uv_connect_t;
    // req->data = this;
    // invoke(&uv_tcp_connect, req, get(), peerAddress.addr(), internal::onConnect); //"TCP connect failed",

    typedef uv::Request<uv_tcp_t, uv_connect_t> Request;
    auto wrap = new Request(this);
    wrap->invoke(&uv_tcp_connect, &wrap->req, get(), peerAddress.addr(),
        [](uv_connect_t* req, int status) {
            auto wrap = reinterpret_cast<Request*>(req->data);
            if (!wrap->ctx->deleted) {
                reinterpret_cast<TCPSocket*>(wrap->parent)->onConnect(req, status);
            }
            else {
                DebugA("Dropping request for closed TCP socket")
            }
            delete wrap;
        });
}


void TCPSocket::connect(const std::string& host, uint16_t port)
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
        typedef uv::Request<uv_tcp_t, uv_getaddrinfo_t> Request;
        auto wrap = new Request(this);
        auto func = [](uv_getaddrinfo_t* req, int status, struct addrinfo* res) {
            auto wrap = reinterpret_cast<Request*>(req->data);
            if (!wrap->ctx->deleted) {
                if (status == 0) {
                    net::Address resolved(res->ai_addr, 16);
                    TraceA("DNS resolved:", resolved)
                    reinterpret_cast<TCPSocket*>(wrap->parent)->connect(resolved);
                }
                else {
                    DebugA("DNS failed to resolve")
                }
            }
            else {
                DebugA("Dropping DNS request for closed TCP socket")
            }
            uv_freeaddrinfo(res);
            delete wrap;
        };
        wrap->invoke(&uv_getaddrinfo, loop(), &wrap->req, func,
            host.c_str(), util::itostr<uint16_t>(port).c_str(), nullptr);
    }
}


void TCPSocket::bind(const net::Address& address, unsigned flags)
{
    TraceA("Binding on", address)

    // Reset the handle if the address family has changed
    if (_af != address.af()) {
        _af = address.af();
        reset();
    }

    if (_af == AF_INET6)
        flags |= UV_TCP_IPV6ONLY;

    assert(get());
    assert(initialized());
    invoke(&uv_tcp_bind, get(), address.addr(), flags); //"TCP bind failed",
}


void TCPSocket::listen(int backlog)
{
    TraceA("Listening")
    init();
    assert(get());
    assert(initialized());

    invoke(&uv_listen, get<uv_stream_t>(), backlog, internal::onAcceptConnection); //"TCP listen failed",
}


bool TCPSocket::shutdown()
{
    TraceA("Shutdown")
    return Stream2::shutdown();
}


void TCPSocket::close()
{
    TraceA("Close")
    Stream2::close();
}


bool TCPSocket::setNoDelay(bool enable)
{
    assert(get());
    return uv_tcp_nodelay(get(), enable ? 1 : 0) == 0;
}


bool TCPSocket::setKeepAlive(bool enable, int delay)
{
    assert(get());
    return uv_tcp_keepalive(get(), enable ? 1 : 0, delay) == 0;
}


bool TCPSocket::setSimultaneousAccepts(bool enable)
{
    assert(get());
#ifdef SCY_WIN
    return uv_tcp_simultaneous_accepts(get(), enable ? 1 : 0) == 0;
#else
    return false;
#endif
}


bool TCPSocket::setReusePort()
{
    assert(get());
#if SCY_HAS_KERNEL_SOCKET_LOAD_BALANCING
    if (_af == AF_UNSPEC) {
        assert (0 && "bind() must be called first");
        return false;
    }

    uv_os_fd_t fd;
    uv_fileno(get<uv_handle_t>(), &fd);
    int on = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(int)) < 0) {
        ErrorA("setsockopt(SO_REUSEPORT) failed")
        return false;
    }

    return true;
#else
    return false;
#endif
}


ssize_t TCPSocket::send(const char* data, size_t len, int flags)
{
    return send(data, len, peerAddress(), flags);
}


ssize_t TCPSocket::send(const char* data, size_t len, const net::Address& /* peerAddress */, int /* flags */)
{
    // TraceS(this) << "Send: " << len << ": " << std::string(data, len) << endl;
    assert(Thread::currentID() == tid());
    assert(get());

    // NOTE: libuv handles this for us
    // assert(len <= net::MAX_TCP_PACKET_SIZE);

    if (!Stream2::write(data, len)) {
        WarnA("TCP send error")
        return -1;
    }

    // TODO: Return native error code
    return len;
}


net::Address TCPSocket::address() const
{
    if (initialized()) {
        struct sockaddr address;
        int addrlen = sizeof(address);
        if (uv_tcp_getsockname(get(), &address, &addrlen) == 0)
            return net::Address(&address, addrlen);
    }
    return net::Address();
}


net::Address TCPSocket::peerAddress() const
{
    if (initialized()) {
        struct sockaddr address;
        int addrlen = sizeof(address);
        if (uv_tcp_getpeername(get(), &address, &addrlen) == 0)
            return net::Address(&address, addrlen);
    }
    return net::Address();
}


void TCPSocket::setError(const scy::Error& err)
{
    assert(!error().any());
    Stream2::setError(err);
}


const scy::Error& TCPSocket::error() const
{
    return Stream2::error();
}


net::TransportType TCPSocket::transport() const
{
    return net::TCP;
}


bool TCPSocket::closed() const
{
    return Stream2::closed();
}


uv::Loop* TCPSocket::loop() const
{
    return Stream2::loop();
}


void TCPSocket::setMode(SocketMode mode)
{
    _mode = mode;
}


void* TCPSocket::self()
{
    return this;
}


const SocketMode TCPSocket::mode() const
{
    return _mode;
}


//
// Callbacks

void TCPSocket::onRead(const char* data, size_t len)
{
    TraceS(this) << "On read: " << len << endl;

    // Note: The const_cast here is relatively safe since the given
    // data pointer is the underlying _buffer.data() pointer, but
    // a better way should be devised.
    onRecv(mutableBuffer(const_cast<char*>(data), len));
}


void TCPSocket::onRecv(const MutableBuffer& buf)
{
    TraceS(this) << "On recv: " << buf.size() << endl;
    onSocketRecv(*this, buf, peerAddress());
}


void TCPSocket::onConnect(uv_connect_t* handle, int status)
{
    TraceA("On connect")
    assert(status == 0);
    // std::cout << "TCPSocket::onConnect: " << status << std::endl;

    if (status == 0) {
        if (readStart())
            onSocketConnect(*this);
    }
    else {
        // Error handled by callback proxy
        setUVError("Connection failed", status);
    }

    // FIXME
    // delete handle;
}


void TCPSocket::acceptConnection()
{
    // Create the shared socket pointer so the if the socket handle is not
    // incremented the accepted socket will be destroyed.
    auto socket = net::makeSocket<net::TCPSocket>(loop());
    TraceS(this) << "Accept connection: " << socket->get() << endl;

    invoke(&uv_tcp_init, loop(), socket->get()); // "Cannot initialize TCP socket",

    if (uv_accept(get<uv_stream_t>(), socket->get<uv_stream_t>()) == 0) {
        socket->readStart();
        AcceptConnection.emit(socket);
    }
    else {
        assert(0 && "uv_accept should not fail");
    }
}


void TCPSocket::onAcceptConnection(uv_stream_t*, int status)
{
    if (status == 0) {
        TraceA("On accept connection")
        acceptConnection();
    }
    else {
        ErrorS(this) << "Accept connection failed: " << uv_strerror(status) << endl;
    }
}


void TCPSocket::onError(const scy::Error& error)
{
    DebugS(this) << "Error: " << error.message << endl;
    onSocketError(*this, error);
    close(); // close on error
}


void TCPSocket::onClose()
{
    TraceA("On close")
    onSocketClose(*this);
}


} // namespace net
} // namespace scy


/// @\}
