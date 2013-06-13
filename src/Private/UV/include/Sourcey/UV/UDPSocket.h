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


#ifndef SOURCEY_UV_UDPSocket_H
#define SOURCEY_UV_UDPSocket_H


#include "Sourcey/UV/UVPP.h"
#include "Sourcey/UV/IOStream.h"
	
#include "Sourcey/Net/Types.h"
#include "Sourcey/Net/Address.h"



namespace Sourcey {
namespace UV {


struct UDPSendRequest 
{
	uv_udp_send_t req;
	uv_buf_t buf;
	Net::Address addr;
};


class UDPSocket: public UV::Interface
{
public:
	UDPSocket(LoopHandle* loop = DefaultLoop);
	virtual ~UDPSocket();
	
	virtual void connect(const Net::Address& peerAddress);
	virtual void close();	
	virtual bool bind(const Net::Address& address, unsigned flags = 0);
	virtual int send(const char* data, int size, const Net::Address& peerAddress);

	virtual bool recvStart();
	virtual bool recvStop();
	virtual Net::Address address();	
	
	Signal3<const char*, int, const Net::Address&> OnRecv;
	Signal3<const char*, int, const Net::Address&> OnSend;

protected:	
	static void onRecv(uv_udp_t* handle, ssize_t nread, uv_buf_t buf, struct sockaddr* addr, unsigned flags);
	static void onSend(uv_udp_send_t* req, int status); 
	static void afterClose(Handle* handle);
		
	uv_udp_t	_handle;
};


} } // namespace Sourcey::UV


#endif // SOURCEY_UV_UDPSocket_H
