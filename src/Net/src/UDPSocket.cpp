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


#include "Sourcey/Net/UDPSocket.h"
#include "Sourcey/Net/Types.h"
#include "Sourcey/Logger.h"


using namespace std;


namespace scy {
namespace net {


UDPSocket::UDPSocket() : 
	net::Socket(new UDPBase, false)
{
}


UDPSocket::UDPSocket(UDPBase* base, bool shared) : 
	net::Socket(base, shared) 
{
}


UDPSocket::UDPSocket(const Socket& socket) : 
	net::Socket(socket)
{
	if (!dynamic_cast<UDPBase*>(_base))
		throw std::runtime_error("Cannot assign incompatible socket");
}
	

UDPBase& UDPSocket::base() const
{
	return static_cast<UDPBase&>(*_base);
}


//
// UDP Base
//


struct SendRequest 
{
	uv_udp_send_t req;
	uv_buf_t buf;
};



UDPBase::UDPBase()
{
	traceL("UDPBase", this) << "Create" << endl;
	//closeOnError(true);
	init();
}


UDPBase::~UDPBase()
{
	traceL("UDPBase", this) << "Destroy" << endl;
}


void UDPBase::init() 
{
	if (!handle()) {
		uv_udp_t* udp = new uv_udp_t;
		udp->data = this; //instance();
		_handle = (uv_handle_t*)udp;
		int r = uv_udp_init(loop(), udp);
		if (r)
			setLastError("Invalid UDP socket");
	}
}


void UDPBase::connect(const Address& peerAddress) 
{
	_peer = peerAddress;

	// Send the Connected signal to mimic TCP behaviour  
	// since socket implementations are interchangable.
	//Connect.emit(instance());
	emitConnect();
}


void UDPBase::close()
{
	traceL("UDPBase", this) << "Closing" << endl;	
	recvStop();
	uv::Handle::close();
}


void UDPBase::bind(const Address& address, unsigned flags) 
{	
	traceL("UDPBase", this) << "Binding on " << address << endl;

	int r;
	switch (address.af()) {
	case AF_INET:
		r = uv_udp_bind(handle<uv_udp_t>(), 
			*reinterpret_cast<const sockaddr_in*>(address.addr()), flags);
		break;
	case AF_INET6:
		r = uv_udp_bind6(handle<uv_udp_t>(), 
			*reinterpret_cast<const sockaddr_in6*>(address.addr()), flags);
		break;
	default:
		throw std::runtime_error("Unexpected address family");
	}
	if (r)
		setLastError("Invalid UDP socket");
	else
		recvStart();
}


int UDPBase::send(const char* data, int len, int flags) 
{	
	return send(data, len, _peer, flags);
}


int UDPBase::send(const char* data, int len, const Address& peerAddress, int /* flags */) 
{	
	assert(len <= net::MAX_UDP_PACKET_SIZE);
	int r;	
	SendRequest* sr = new SendRequest;
	sr->buf = uv_buf_init((char*)data, len); // TODO: memcpy data?

	if (_peer.valid() && _peer != peerAddress)
		throw std::runtime_error("NotAuthorized peer: " + peerAddress.toString());
	
	traceL("UDPBase", this) << "send: " << len << ": " << peerAddress << endl;
	switch (peerAddress.af()) {
	case AF_INET:
		r = uv_udp_send(&sr->req, handle<uv_udp_t>(), &sr->buf, 1,
			*reinterpret_cast<const sockaddr_in*>(peerAddress.addr()), UDPBase::afterSend); //
		break;
	case AF_INET6:
		r = uv_udp_send6(&sr->req, handle<uv_udp_t>(), &sr->buf, 1,
			*reinterpret_cast<const sockaddr_in6*>(peerAddress.addr()), UDPBase::afterSend); //
		break;
	default:
		throw std::runtime_error("Unexpected address family");
	}
	if (r)
		setLastError("Invalid UDP socket");

	// R is -1 on error, otherwise return len
	//return r ? r : len;
	return len;
}


bool UDPBase::recvStart() 
{
	// UV_EALREADY means that the socket is already bound but that's okay
	int r = uv_udp_recv_start(handle<uv_udp_t>(), UDPBase::allocRecvBuffer, onRecv);
	if (r && uv_last_error(loop()).code != UV_EALREADY) {
		setLastError();
		return false;
	}  
	return true;
}


bool UDPBase::recvStop() 
{
	if (!handle())
		return false;
	return uv_udp_recv_stop(handle<uv_udp_t>()) == 0;
}


void UDPBase::onRecv(const MutableBuffer& buf, const net::Address& address)
{
	traceL("UDPBase", this) << "recv: " << buf.size() << endl;	

	emitRecv(buf, address);
}


void UDPBase::setError(const Error& err)
{
	uv::Handle::setError(err);
}

		
const Error& UDPBase::error() const
{
	return uv::Handle::error();
}


net::Address UDPBase::address() const
{	
	if (closed())
		return net::Address();
		//throw std::runtime_error("Invalid UDP socket: No address");

	if (_peer.valid())
		return _peer;
	
	struct sockaddr address;
	int addrlen = sizeof(address);
	int r = uv_udp_getsockname(handle<uv_udp_t>(), &address, &addrlen);
	if (r)
		return net::Address();
		//throwLastError("Invalid UDP socket: No address");

	return Address(&address, addrlen);
}


net::Address UDPBase::peerAddress() const
{
	if (!_peer.valid())
		return net::Address();
		//throw std::runtime_error("Invalid UDP socket: No peer address");
	return _peer;
}


net::TransportType UDPBase::transport() const 
{ 
	return net::UDP; 
}
	

bool UDPBase::closed() const
{
	return uv::Handle::closed();
}


//
// Callbacks
//

void UDPBase::onRecv(uv_udp_t* handle, ssize_t nread, uv_buf_t /* buf */, struct sockaddr* addr, unsigned /* flags */) 
{	
	UDPBase* socket = static_cast<UDPBase*>(handle->data);
	traceL("UDPBase", socket) << "Handle Recv: " << nread << endl;

	// EOF or Error
	if (nread == -1)
		socket->setLastError();
	
	//socket->_buffer.limit(nread);
	socket->onRecv(mutableBuffer(socket->_buffer.data(), nread), net::Address(addr, sizeof *addr));
}


void UDPBase::afterSend(uv_udp_send_t* req, int status) 
{
	SendRequest* sr = reinterpret_cast<SendRequest*>(req);
	UDPBase* socket = reinterpret_cast<UDPBase*>(sr->req.handle->data);	
	if (status) 
		socket->setLastError();
	delete sr;
}


uv_buf_t UDPBase::allocRecvBuffer(uv_handle_t *handle, size_t suggested_size)
{
	UDPBase* self = static_cast<UDPBase*>(handle->data);	
	//traceL("UDPBase", self) << "Allocating Buffer: " << suggested_size << endl;	
	
	// Reserve the recommended buffer size
	// XXX: libuv wants us to allocate 65536 bytes for UDP .. hmmm
	//if (suggested_size > self->_buffer.available())
	//	self->_buffer.reserve(suggested_size); 
	assert(self->_buffer.capacity() >= suggested_size);

	// Reset the buffer position on each read
	//self->_buffer.position(0);
	return uv_buf_init(self->_buffer.data(), suggested_size);
}


void UDPBase::onError(const Error& error) 
{		
	errorL("UDPBase", this) << "Error: " << error.message << endl;	
	emitError(error);
	close(); // close on error
}


void UDPBase::onClose() 
{		
	errorL("UDPBase", this) << "On close" << endl;	
	emitClose();
}



} } // namespace scy::net