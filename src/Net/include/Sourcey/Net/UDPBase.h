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


#ifndef SOURCEY_UV_UDPBase_H
#define SOURCEY_UV_UDPBase_H


#include "Sourcey/UV/UVPP.h"
#include "Sourcey/UV/Base.h"
#include "Sourcey/Net/Socket.h"
	
#include "Sourcey/Net/Types.h"
#include "Sourcey/Net/Address.h"


namespace scy {
namespace uv {


class UDPBase: public uv::Base<CountedObject>, public net::SocketBase
{
public:
	UDPBase();
	virtual ~UDPBase();
	
	virtual void connect(const net::Address& peerAddress);
	virtual void close();	

	virtual void bind(const net::Address& address, unsigned flags = 0);

	virtual int send(const char* data, int len, int flags = 0);
	virtual int send(const char* data, int len, const net::Address& peerAddress, int flags = 0);

	/*
	virtual int send(const IPacket& packet, int flags = 0);
	virtual int send(const IPacket& packet, const net::Address& peerAddress, int flags = 0);
	*/
	
	virtual net::Address address() const;
	virtual net::Address peerAddress() const;

	net::TransportType transport() const;
		/// Returns the UDP transport protocol.

	virtual void duplicate();	
	virtual void release();
	virtual int refCount() const;
	
	virtual void onRecv(Buffer& buf, const net::Address& address);

	struct SendRequest 
	{
		uv_udp_send_t req;
		uv_buf_t buf;
	};

protected:	
	virtual void init();	
	virtual bool recvStart();
	virtual bool recvStop();

	static void onRecv(uv_udp_t* handle, ssize_t nread, uv_buf_t buf, struct sockaddr* addr, unsigned flags);
	static void onSend(uv_udp_send_t* req, int status); 
	static uv_buf_t allocRecvBuffer(uv_handle_t* handle, size_t suggested_size);

	virtual void onError(const Error& error);
	virtual void onClose();
	
	net::Address _peer;
	scy::Buffer _buffer;
};


} } // namespace scy::uv


#endif // SOURCEY_UV_UDPBase_H


	
	//Signal3<const char*, int, const net::Address&> Recv;
	//Signal3<const char*, int, const net::Address&> Send;