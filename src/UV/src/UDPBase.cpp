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


#include "Sourcey/UV/UDPBase.h"
#include "Sourcey/Net/Types.h"
#include "Sourcey/Logger.h"


using namespace std;

using scy::net::Address;
using scy::Exception;


namespace scy {
namespace uv {
	

UDPBase::UDPBase()
{
	traceL("UDPBase", this) << "Creating" << endl;
	//closeOnError(true);
	init();
}


UDPBase::~UDPBase()
{
	traceL("UDPBase", this) << "Destroying" << endl;
}


void UDPBase::init() 
{
	if (!handle()) {
		uv_udp_t* udp = new uv_udp_t;
		udp->data = instance();
		_handle = (uv_handle_t*)udp;
		int r = uv_udp_init(loop(), udp);
		if (r)
			setLastError(true);
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
	uv::Base<CountedObject>::close();
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
		throw Exception("Unexpected address family");
	}
	if (r)
		setLastError();
	else
		recvStart();
}


int UDPBase::send(const char* data, int len, int flags) 
{	
	return send(data, len, _peer, flags);
}


int UDPBase::send(const char* data, int len, const Address& peerAddress, int flags) 
{	
	assert(len <= net::MAX_UDP_PACKET_SIZE);
	int r;	
	SendRequest* sr = new SendRequest;
	sr->buf = uv_buf_init((char*)data, len); // TODO: memcpy data?

	if (_peer.valid() && _peer != peerAddress)
		throw Exception("Unauthorized peer: " + peerAddress.toString());
	
	traceL("UDPBase", this) << "Sending: " << len << ": " << peerAddress << endl;
	switch (peerAddress.af()) {
	case AF_INET:
		r = uv_udp_send(&sr->req, handle<uv_udp_t>(), &sr->buf, 1,
			*reinterpret_cast<const sockaddr_in*>(peerAddress.addr()), UDPBase::onSend); //
		break;
	case AF_INET6:
		r = uv_udp_send6(&sr->req, handle<uv_udp_t>(), &sr->buf, 1,
			*reinterpret_cast<const sockaddr_in6*>(peerAddress.addr()), UDPBase::onSend); //
		break;
	default:
		throw Exception("Unexpected address family");
	}

	// R is -1 on error, otherwise return len
	return r ? r : len;
}


int UDPBase::send(const IPacket& packet, int flags)
{
	return net::SocketBase::send(packet, flags);
}


int UDPBase::send(const IPacket& packet, const net::Address& peerAddress, int flags)
{
	return net::SocketBase::send(packet, peerAddress, flags);
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


void UDPBase::onRecv(Buffer& buf, const net::Address& address)
{
	traceL("UDPBase", this) << "On Recv: " << buf.size() << endl;
	

	// Set the size of the buffer to send
	//net::SocketPacket packet(*this, buf, address);
	//Recv.emit(instance(), packet);
	emitRecv(buf, address);
}


net::Address UDPBase::address() const
{
	if (_peer.valid())
		return _peer;
	struct sockaddr address;
	int addrlen = sizeof(address);
	int r = uv_udp_getsockname(handle<uv_udp_t>(), &address, &addrlen);
	if (r)
		throw Exception("No local address: UDP Socket not bound");
		//throwLastError(loop(), "No local address: UDP Socket not bound: ");
	return Address(&address, addrlen);
}


net::Address UDPBase::peerAddress() const
{
	if (!_peer.valid())
		throw Exception("No peer address: UDP Socket not connected");
	return _peer;
}


net::TransportType UDPBase::transport() const 
{ 
	return net::UDP; 
}


void UDPBase::duplicate() 
{ 
	Base<CountedObject>::duplicate(); 
}


void UDPBase::release() 
{ 
	Base<CountedObject>::release(); 
}


int UDPBase::refCount() const 
{ 
	return Base<CountedObject>::refCount();
}


//
// Callbacks
//

void UDPBase::onRecv(uv_udp_t* handle, ssize_t nread, uv_buf_t buf, struct sockaddr* addr, unsigned flags) 
{	
	UDPBase* socket = static_cast<UDPBase*>(handle->data);
	traceL("UDPBase", socket) << "Handle Recv: " << nread << endl;
	
	/*	
	socket->_buffer.size(nread);
	net::PacketInfo info(*socket, net::Address(addr, sizeof *addr));
	traceL("TCPBase", this) << "On Recv: " << buf.size() << endl;	
	*/

	// EOF or Error
	if (nread == -1)
		socket->setLastError();
	//send net::Packet type, cast as RawPacket
	
	socket->_buffer.size(nread);
	socket->onRecv(socket->_buffer, net::Address(addr, sizeof *addr));
}


void UDPBase::onSend(uv_udp_send_t* req, int status) 
{
	SendRequest* sr = reinterpret_cast<SendRequest*>(req);
	UDPBase* socket = reinterpret_cast<UDPBase*>(sr->req.handle->data);	
	traceL("UDPBase", socket) << "Send Callback: " << status << endl;
	if (status) 
		socket->setLastError();
	delete sr;
}


uv_buf_t UDPBase::allocRecvBuffer(uv_handle_t *handle, size_t suggested_size)
{
	UDPBase* self = static_cast<UDPBase*>(handle->data);	
	traceL("UDPBase", self) << "Allocating Buffer: " << suggested_size << endl;	

	// XXX: libuv wants us to allocate 65536 bytes for UDP .. wha?
	if (suggested_size > self->_buffer.size())
		self->_buffer.reserve(suggested_size); 
	traceL("UDPBase", self) << "Allocating Buffer: (self->_buffer.capacity() " << self->_buffer.capacity()  << endl;	
	assert(self->_buffer.capacity() == suggested_size);
	return uv_buf_init(self->_buffer.begin(), suggested_size);
}


void UDPBase::onError(int syserr) 
{		
	errorL("UDPBase", this) << "On Error: " << errorMessage() << endl;	
	emitError(syserr, errorMessage());
	close(); // close on error
}


void UDPBase::onClose() 
{		
	errorL("UDPBase", this) << "On Close" << endl;	
	emitClose();
}


} } // namespace scy::uv