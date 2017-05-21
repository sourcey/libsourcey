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
    : Stream(loop)
{
    // TraceS(this) << "Create" << endl;
    initialize();
}


TCPSocket::~TCPSocket()
{
    // TraceS(this) << "Destroy" << endl;
}


void TCPSocket::initialize()
{
    if (!_ptr) {
        auto tcp = new uv_tcp_t;
        tcp->data = this;
        _ptr = reinterpret_cast<uv_handle_t*>(tcp);
        invoke(&uv_tcp_init_ex, loop(), tcp, _af);
    }

    Handle::initialize();
}


void TCPSocket::reset()
{
    Handle::reset();
}


namespace internal {

UVStatusCallbackWithType(TCPSocket, onConnect, uv_connect_t);
UVStatusCallbackWithType(TCPSocket, onAcceptConnection, uv_stream_t);

}


void TCPSocket::connect(const net::Address& peerAddress)
{
    // TraceS(this) << "Connecting to " << peerAddress << endl;
    initialize();
    auto req = new uv_connect_t;
    req->data = this;
    invoke(&uv_tcp_connect, req, ptr<uv_tcp_t>(), peerAddress.addr(), internal::onConnect); //"TCP connect failed",
}


void TCPSocket::connect(const std::string& host, uint16_t port)
{
    Socket::connect(host, port);
}


void TCPSocket::bind(const net::Address& address, unsigned flags)
{
    // TraceS(this) << "Binding on " << address << endl;

    // Reset the handle if the address family has changed
    if (_af != address.af()) {
        _af = address.af();
        reset();
    }

    if (_af == AF_INET6)
        flags |= UV_TCP_IPV6ONLY;

    invoke(&uv_tcp_bind, ptr<uv_tcp_t>(), address.addr(), flags); //"TCP bind failed",
}


void TCPSocket::listen(int backlog)
{
    // TraceS(this) << "Listening" << endl;
    invoke(&uv_listen, ptr<uv_stream_t>(), backlog, internal::onAcceptConnection); //"TCP listen failed",
}


bool TCPSocket::shutdown()
{
    // TraceS(this) << "Shutdown" << endl;
    return Stream::shutdown();
}


void TCPSocket::close()
{
    // TraceS(this) << "Close" << endl;
    Stream::close();
}


bool TCPSocket::setNoDelay(bool enable)
{
    assert(_ptr);
    return uv_tcp_nodelay(ptr<uv_tcp_t>(), enable ? 1 : 0) == 0;
}


bool TCPSocket::setKeepAlive(bool enable, int delay)
{
    assert(_ptr);
    return uv_tcp_keepalive(ptr<uv_tcp_t>(), enable ? 1 : 0, delay) == 0;
}


bool TCPSocket::setSimultaneousAccepts(bool enable)
{
    assert(_ptr);
#ifdef SCY_WIN
    return uv_tcp_simultaneous_accepts(ptr<uv_tcp_t>(), enable ? 1 : 0) == 0;
#else
    return false;
#endif
}


bool TCPSocket::setReusePort()
{
    assert(_ptr);
#if SCY_HAS_KERNEL_SOCKET_LOAD_BALANCING
    if (_af == AF_UNSPEC) {
        assert (0 && "bind() must be called first");
        return false;
    }

    uv_os_fd_t fd;
    uv_fileno(ptr(), &fd);
    int on = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(int)) < 0) {
        ErrorS(this) << "setsockopt(SO_REUSEPORT) failed" << endl;
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
    assert(_ptr);

    // NOTE: libuv handles this for us
    // assert(len <= net::MAX_TCP_PACKET_SIZE);

    if (!Stream::write(data, len)) {
        WarnL << "Send error" << endl;
        return -1;
    }

    // TODO: Return native error code
    return len;
}


net::Address TCPSocket::address() const
{
    assert(_ptr);
    struct sockaddr_storage address;
    int addrlen = sizeof(address);
    int r = uv_tcp_getsockname(ptr<uv_tcp_t>(), reinterpret_cast<sockaddr*>(&address), &addrlen);
    if (r)
        return net::Address();
    return net::Address(reinterpret_cast<const sockaddr*>(&address), addrlen);
}


net::Address TCPSocket::peerAddress() const
{
    assert(_ptr);
    struct sockaddr_storage address;
    int addrlen = sizeof(address);
    int r = uv_tcp_getpeername(ptr<uv_tcp_t>(), reinterpret_cast<sockaddr*>(&address), &addrlen);
    if (r)
        return net::Address();
    return net::Address(reinterpret_cast<const sockaddr*>(&address), addrlen);
}


void TCPSocket::setError(const scy::Error& err)
{
    assert(!error().any());
    Stream::setError(err);
}


const scy::Error& TCPSocket::error() const
{
    return Stream::error();
}


net::TransportType TCPSocket::transport() const
{
    return net::TCP;
}


bool TCPSocket::closed() const
{
    return Stream::closed();
}


uv::Loop* TCPSocket::loop() const
{
    return uv::Handle::loop();
}


void TCPSocket::setMode(SocketMode mode)
{
    _mode = mode;
}


const SocketMode TCPSocket::mode() const
{
    return _mode;
}


//
// Callbacks

void TCPSocket::onRead(const char* data, size_t len)
{
    // TraceS(this) << "On read: " << len << endl;

    // Note: The const_cast here is relatively safe since the given
    // data pointer is the underlying _buffer.data() pointer, but
    // a better way should be devised.
    onRecv(mutableBuffer(const_cast<char*>(data), len));
}


void TCPSocket::onRecv(const MutableBuffer& buf)
{
    // TraceS(this) << "On recv: " << buf.size() << endl;
    onSocketRecv(*this, buf, peerAddress());
}


void TCPSocket::onConnect(uv_connect_t* handle, int status)
{
    // TraceS(this) << "On connect" << endl;

    if (status == 0) {
        if (readStart())
            onSocketConnect(*this);
    }
    else {
        // Error handled by callback proxy
        setUVError("Connection failed", status);
    }
    delete handle;
}


void TCPSocket::acceptConnection()
{
    // Create the shared socket pointer so the if the socket handle is not
    // incremented the accepted socket will be destroyed.
    auto socket = net::makeSocket<net::TCPSocket>(loop());
    // TraceS(this) << "Accept connection: " << socket->ptr() << endl;

    invoke(&uv_tcp_init, loop(), socket->ptr<uv_tcp_t>()); // "Cannot initialize TCP socket",

    if (uv_accept(ptr<uv_stream_t>(), socket->ptr<uv_stream_t>()) == 0) {
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
        // TraceS(this) << "On accept connection" << endl;
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
    // TraceS(this) << "On close" << endl;
    onSocketClose(*this);
}


} // namespace net
} // namespace scy


/// @\}
