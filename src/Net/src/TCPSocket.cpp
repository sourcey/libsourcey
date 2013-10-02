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

#include "Sourcey/Net/TCPSocket.h"
#include "Sourcey/Net/TCPSocket.h"
#include "Sourcey/Logger.h"


using namespace std;


namespace scy {
namespace net {


TCPSocket::TCPSocket() : 
	net::Socket(new TCPBase, false)
{	
	traceL("TCPSocket", this) << "Create" << endl;	
}


TCPSocket::TCPSocket(TCPBase* base, bool shared) : 
	net::Socket(base, shared) 
{
	traceL("TCPSocket", this) << "Destroy" << endl;	
}


TCPSocket::TCPSocket(const Socket& socket) : 
	net::Socket(socket)
{
	if (!dynamic_cast<TCPBase*>(_base))
		throw std::runtime_error("Cannot assign incompatible socket");
}
	

TCPBase& TCPSocket::base() const
{
	return static_cast<TCPBase&>(*_base);
}


//
// TCP Base
//


TCPBase::TCPBase(uv::Loop* loop) :
	Stream(loop)
{
	traceL("TCPBase", this) << "Create" << endl;
	init();	
}

	
TCPBase::~TCPBase() 
{	
	traceL("TCPBase", this) << "Destroy" << endl;	
}


void TCPBase::init()
{
	if (closed()) {		
		traceL("TCPBase", this) << "Init" << endl;
		auto tcp = new uv_tcp_t;
		tcp->data = this;
		_ptr = reinterpret_cast<uv_handle_t*>(tcp);
		uv_tcp_init(loop(), tcp);
		//assert(tcp->data == this);		
		//_connectReq.data = instance();	
	}
}


namespace internal {

	UVStatusCallback(TCPBase, onConnect, uv_connect_t);
	UVStatusCallbackWithType(TCPBase, onAcceptConnection, uv_stream_t);

}


void TCPBase::connect(const net::Address& peerAddress) 
{
	traceL("TCPBase", this) << "Connecting to " << peerAddress << endl;
	init();
	_connectReq.reset(new uv_connect_t);
	_connectReq.get()->data = this;
	//assert(_connectReq.data != this);
	//_connectReq.data = this;	
	//auto addr = reinterpret_cast<const sockaddr_in*>(peerAddress.addr());
	int r = uv_tcp_connect(_connectReq.get(), handle<uv_tcp_t>(), peerAddress.addr(), internal::onConnect);
	if (r) setAndThrowError("TCP connect failed", r);
}


void TCPBase::bind(const net::Address& address, unsigned /* flags */) 
{
	traceL("TCPBase", this) << "Binding on " << address << endl;
	init();
	int r;
	switch (address.af()) {
	case AF_INET:
		r = uv_tcp_bind(handle<uv_tcp_t>(), address.addr());
		break;
	//case AF_INET6:
	//	r = uv_tcp_bind6(handle<uv_tcp_t>(), *reinterpret_cast<const sockaddr_in6*>(address.addr()));
	//	break;
	default:
		throw std::runtime_error("Unexpected address family");
	}
	if (r) setAndThrowError("TCP bind failed", r);
}


void TCPBase::listen(int backlog) 
{
	traceL("TCPBase", this) << "Listening" << endl;
	init();
	int r = uv_listen(handle<uv_stream_t>(), backlog, internal::onAcceptConnection);
	if (r) setAndThrowError("TCP listen failed", r);
}


bool TCPBase::shutdown()
{
	traceL("TCPBase", this) << "Shutdown" << endl;
	return Stream::shutdown();
}


void TCPBase::close()
{
	traceL("TCPBase", this) << "Close" << endl;
	Stream::close();
}


void TCPBase::setNoDelay(bool enable) 
{
	init();
	int r = uv_tcp_nodelay(handle<uv_tcp_t>(), enable ? 1 : 0);
	if (r) setUVError("TCP socket error", r);
}


void TCPBase::setKeepAlive(int enable, unsigned int delay) 
{
	init();
	int r = uv_tcp_keepalive(handle<uv_tcp_t>(), enable, delay);
	if (r) setUVError("TCP socket error", r);
}


#ifdef _WIN32
void TCPBase::setSimultaneousAccepts(bool enable) 
{
	init();
	int r = uv_tcp_simultaneous_accepts(handle<uv_tcp_t>(), enable ? 1 : 0);
	if (r) setUVError("TCP socket error", r);
}
#endif


int TCPBase::send(const char* data, int len, int flags) 
{	
	return send(data, len, peerAddress(), flags);
}


int TCPBase::send(const char* data, int len, const net::Address& /* peerAddress */, int /* flags */) 
{
	//assert(len <= net::MAX_TCP_PACKET_SIZE); // libuv handles this for us
	
	//if (len < 300)
	//	traceL("TCPBase", this) << "Send: " << string(data, len) << endl;

	if (!Stream::write(data, len)) {
		warnL("TCPBase", this) << "Send error" << endl;	
		return -1;
	}

	// R is -1 on error, otherwise return len
	// TODO: Return native error code?
	return len;
}


void TCPBase::acceptConnection()
{
	// Create the socket on the stack.
	// If the socket is not handled it will be destroyed.
	net::TCPSocket socket;
	traceL("TCPBase", this) << "Accept connection: " << socket.base().handle() << endl;
	int r = uv_accept(handle<uv_stream_t>(), socket.base().handle<uv_stream_t>());
	assert(r == 0); // uv_accept should always work
	socket.base().readStart();		
	AcceptConnection.emit(instance(), socket);
	if (socket.base().refCount() == 1)
		traceL("TCPBase", this) << "Accept connection: Not handled" << endl;
}


net::Address TCPBase::address() const
{
	//traceL("TCPBase", this) << "Get address: " << closed() << endl;

	if (!active())
		return net::Address();
		//throw std::runtime_error("Invalid TCP socket: No address");
	
	struct sockaddr_storage address;
	int addrlen = sizeof(address);
	int r = uv_tcp_getsockname(handle<uv_tcp_t>(),
								reinterpret_cast<sockaddr*>(&address),
								&addrlen);
	if (r)
		return net::Address();
		//throwLastError("Invalid TCP socket: No address");

	return net::Address(reinterpret_cast<const sockaddr*>(&address), addrlen);
}


net::Address TCPBase::peerAddress() const
{
	//traceL("TCPBase", this) << "Get peer address: " << closed() << endl;
	if (!active())
		return net::Address();
		//throw std::runtime_error("Invalid TCP socket: No peer address");

	struct sockaddr_storage address;
	int addrlen = sizeof(address);
	int r = uv_tcp_getpeername(handle<uv_tcp_t>(),
								reinterpret_cast<sockaddr*>(&address),
								&addrlen);

	if (r)
		return net::Address();
		//throwLastError("Invalid TCP socket: No peer address");

	return net::Address(reinterpret_cast<const sockaddr*>(&address), addrlen);
}


void TCPBase::setError(const Error& err)
{
	Stream::setError(err);
}

		
const Error& TCPBase::error() const
{
	return Stream::error();
}


net::TransportType TCPBase::transport() const 
{ 
	return net::TCP; 
}
	

//bool TCPBase::initialized() const
//{
//	return Stream::initialized(); //uv::Handle::closed();
//}
	

bool TCPBase::closed() const
{
	return Stream::closed(); //uv::Handle::closed();
}


uv::Loop* TCPBase::loop() const
{
	return uv::Handle::loop();
}


/*
SOCKET TCPBase::sockfd() const
{
	return closed() ? INVALID_SOCKET : handle<uv_tcp_t>()->socket;
}


bool TCPBase::initialized() const
{
	return !closed();
	//return sockfd() != INVALID_SOCKET;
}
*/


//
// Callbacks
//

void TCPBase::onRead(const char* data, int len)
{
	traceL("TCPBase", this) << "On read: " << len << endl;
	
	//if (len < 300)
	//	traceL("TCPBase", this) << "Received: " << string(data, len) << endl;
	//_buffer.position(0);
	//_buffer.limit(len);
	//onRecv(mutableBuffer(_buffer.data(), len));

	// Note: The const_cast here is relatively safe since the given 
	// data pointer is the underlying _buffer.data() pointer, but
	// a better way should be devised.
	onRecv(mutableBuffer(const_cast<char*>(data), len));
}


void TCPBase::onRecv(const MutableBuffer& buf)
{
	traceL("TCPBase", this) << "Recv: " << buf.size() << endl;
	emitRecv(buf, peerAddress()); //MutableBuffer(buf)
}


void TCPBase::onConnect(int status)
{
	traceL("TCPBase", this) << "On connect" << endl;
	
	// Error handled by static callback proxy
	if (status == 0) {
		if (readStart())
			emitConnect();
	}
	else
		errorL("TCPBase", this) << "Connection failed: " << error().message << endl;
}


void TCPBase::onAcceptConnection(uv_stream_t*, int status) 
{		
	if (status == 0) {
		traceL("TCPBase", this) << "On accept connection" << endl;
		acceptConnection();
	}
	else
		errorL("TCPBase", this) << "Accept connection failed" << endl;
}


void TCPBase::onError(const Error& error) 
{		
	errorL("TCPBase", this) << "Error: " << error.message << endl;	
	emitError(error);
	close(); // close on error
}


void TCPBase::onClose() 
{		
	errorL("TCPBase", this) << "On close" << endl;	
	emitClose();
}


} } // namespace scy::net