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

#include "scy/net/tcpsocket.h"
#include "scy/logger.h"
//#if POSIX
//#include <sys/socket.h>
//#endif


using std::endl;


namespace scy {
namespace net {


TCPSocket::TCPSocket(uv::Loop* loop) :
    Stream(loop)
{
    TraceLS(this) << "Create" << endl;
    init();    
}

    
TCPSocket::~TCPSocket() 
{    
    TraceLS(this) << "Destroy" << endl;    
    close();
}


void TCPSocket::init()
{
    if (ptr()) return;

    TraceLS(this) << "Init" << endl;
    auto tcp = new uv_tcp_t;
    tcp->data = this;
    _ptr = reinterpret_cast<uv_handle_t*>(tcp);
    _closed = false;
    _error.reset();
    int r = uv_tcp_init(loop(), tcp);
    if (r)
        setUVError("Cannot initialize TCP socket", r);
}


namespace internal {

    UVStatusCallbackWithType(TCPSocket, onConnect, uv_connect_t);
    UVStatusCallbackWithType(TCPSocket, onAcceptConnection, uv_stream_t);

}


void TCPSocket::connect(const net::Address& peerAddress) 
{
    TraceLS(this) << "Connecting to " << peerAddress << endl;
    init();
    auto req = new uv_connect_t;
    req->data = this;
    int r = uv_tcp_connect(req, ptr<uv_tcp_t>(), peerAddress.addr(), internal::onConnect);
    if (r) setAndThrowError("TCP connect failed", r);
}


void TCPSocket::bind(const net::Address& address, unsigned flags) 
{
    TraceLS(this) << "Binding on " << address << endl;
    init();
    int r;
    switch (address.af()) {
    case AF_INET:
        r = uv_tcp_bind(ptr<uv_tcp_t>(), address.addr(), flags);
        break;
    //case AF_INET6:
    //    r = uv_tcp_bind6(ptr<uv_tcp_t>(), *reinterpret_cast<const sockaddr_in6*>(address.addr()));
    //    break;
    default:
        throw std::runtime_error("Unexpected address family");
    }
    if (r) setAndThrowError("TCP bind failed", r);
}


void TCPSocket::listen(int backlog) 
{
    TraceLS(this) << "Listening" << endl;
    init();
    int r = uv_listen(ptr<uv_stream_t>(), backlog, internal::onAcceptConnection);
    if (r) setAndThrowError("TCP listen failed", r);
}


bool TCPSocket::shutdown()
{
    TraceLS(this) << "Shutdown" << endl;
    return Stream::shutdown();
}


void TCPSocket::close()
{
    TraceLS(this) << "Close" << endl;
    Stream::close();
}


void TCPSocket::setNoDelay(bool enable) 
{
    init();
    int r = uv_tcp_nodelay(ptr<uv_tcp_t>(), enable ? 1 : 0);
    if (r) setUVError("TCP socket error", r);
}


void TCPSocket::setKeepAlive(int enable, unsigned int delay) 
{
    init();
    int r = uv_tcp_keepalive(ptr<uv_tcp_t>(), enable, delay);
    if (r) setUVError("TCP socket error", r);
}


#ifdef _WIN32
void TCPSocket::setSimultaneousAccepts(bool enable) 
{
    init();
    int r = uv_tcp_simultaneous_accepts(ptr<uv_tcp_t>(), enable ? 1 : 0);
    if (r) setUVError("TCP socket error", r);
}
#endif


int TCPSocket::send(const char* data, std::size_t len, int flags) 
{    
    return send(data, len, peerAddress(), flags);
}


int TCPSocket::send(const char* data, std::size_t len, const net::Address& /* peerAddress */, int /* flags */) 
{
    //assert(len <= net::MAX_TCP_PACKET_SIZE); // libuv handles this for us
    
    TraceLS(this) << "Send: " << len << endl;    
    assert(Thread::currentID() == tid());
    
#if 0
    if (len < 300)
        TraceLS(this) << "Send: " << len << ": " << std::string(data, len) << endl;
    else {
        std::string str(data, len);
        TraceLS(this) << "Send: START: " << len << ": " << str.substr(0, 100) << endl;
        TraceLS(this) << "Send: END: " << len << ": " << str.substr(str.length() - 100, str.length()) << endl;
    }
#endif

    if (!Stream::write(data, len)) {
        WarnL << "Send error" << endl;    
        return -1;
    }

    // R is -1 on error, otherwise return len
    // TODO: Return native error code?
    return len;
}


void TCPSocket::acceptConnection()
{
    // Create the shared socket pointer;
    // if it is not handled it will be destroyed.
    // TODO: Allow accepted sockets to use different event loops.
    auto socket = net::makeSocket<net::TCPSocket>(loop()); //std::make_shared<net::TCPSocket>(this->loop());
    TraceLS(this) << "Accept connection: " << socket->ptr() << endl;
    uv_accept(ptr<uv_stream_t>(), socket->ptr<uv_stream_t>()); // uv_accept should always work
    socket->readStart();        
    AcceptConnection.emit(Socket::self(), socket);
}


net::Address TCPSocket::address() const
{
    if (!active())
        return net::Address();
        //throw std::runtime_error("Invalid TCP socket: No address");
    
    struct sockaddr_storage address;
    int addrlen = sizeof(address);
    int r = uv_tcp_getsockname(ptr<uv_tcp_t>(),
                                reinterpret_cast<sockaddr*>(&address),
                                &addrlen);
    if (r)
        return net::Address();
        //throwLastError("Invalid TCP socket: No address");

    return net::Address(reinterpret_cast<const sockaddr*>(&address), addrlen);
}


net::Address TCPSocket::peerAddress() const
{
    //TraceLS(this) << "Get peer address: " << closed() << endl;
    if (!active())
        return net::Address();
        //throw std::runtime_error("Invalid TCP socket: No peer address");

    struct sockaddr_storage address;
    int addrlen = sizeof(address);
    int r = uv_tcp_getpeername(ptr<uv_tcp_t>(),
                                reinterpret_cast<sockaddr*>(&address),
                                &addrlen);

    if (r)
        return net::Address();
        //throwLastError("Invalid TCP socket: No peer address");

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


//
// Callbacks

void TCPSocket::onRead(const char* data, std::size_t len)
{
    TraceLS(this) << "On read: " << len << endl;

    // Note: The const_cast here is relatively safe since the given 
    // data pointer is the underlying _buffer.data() pointer, but
    // a better way should be devised.
    onRecv(mutableBuffer(const_cast<char*>(data), len));
}


void TCPSocket::onRecv(const MutableBuffer& buf)
{
    TraceLS(this) << "Recv: " << buf.size() << endl;
    onSocketRecv(buf, peerAddress());
}


void TCPSocket::onConnect(uv_connect_t* handle, int status)
{
    TraceLS(this) << "On connect" << endl;
    
    // Error handled by static callback proxy
    if (status == 0) {
        if (readStart())
            onSocketConnect();
    }
    else {
        setUVError("Connection failed", status);    
        //ErrorLS(this) << "Connection failed: " << error().message << endl;
    }
    delete handle;
}


void TCPSocket::onAcceptConnection(uv_stream_t*, int status) 
{        
    if (status == 0) {
        TraceLS(this) << "On accept connection" << endl;
        acceptConnection();
    }
    else
        ErrorLS(this) << "Accept connection failed" << endl;
}


void TCPSocket::onError(const scy::Error& error) 
{        
    DebugLS(this) << "Error: " << error.message << endl;
    onSocketError(error);
    close(); // close on error
}


void TCPSocket::onClose() 
{        
    TraceLS(this) << "On close" << endl;    
    onSocketClose();
}


} } // namespace scy::net