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


#ifndef SOURCEY_UV_TCPContext_H
#define SOURCEY_UV_TCPContext_H


#include "Sourcey/UV/UVPP.h"
#include "Sourcey/UV/IOStream.h"
#include "Sourcey/Net/Address.h"
#include "Sourcey/Net/Types.h"


namespace Sourcey {
namespace UV {


class TCPContext: public IOStream
{
public:
	TCPContext(LoopHandle* loop = DefaultLoop);
	virtual ~TCPContext();
	
	virtual TCPContext* acceptConnection();	
	
	virtual void connect(const Net::Address& peerAddress);	
	virtual void close();

	virtual int send(const char* data, int size);
	
	virtual void setNoDelay(bool enable);
	virtual void setKeepAlive(int enable, unsigned int delay);
	virtual bool bind(const Net::Address& address);
	virtual bool bind6(const Net::Address& address);
	virtual bool listen(int backlog = 64);
	
	#ifdef _WIN32
	virtual void setSimultaneousAccepts(bool enable);
	#endif
	
	virtual void onConnected(int status);
	virtual void onAcceptConnection(StreamHandle* handle, int status);

	Signal<int> OnConnected;
	Signal<TCPContext*> OnAcceptConnection;
		// TODO: Refcount TCPContext so we know it has been handled, 
		// delete it othewrwise.
	
protected:
	ConnectReq	_connection;
};


//
// UV Callbacks
//

UVStatusCallback(TCPContext, onConnected, ConnectReq);
UVStatusCallbackWithType(TCPContext, onAcceptConnection, StreamHandle);


} } // namespace Sourcey::UV


#endif // SOURCEY_UV_TCPContext_H


	//uv_tcp_t*		_tcp;
	//uv_err_t		_error;
	//LoopHandle*		_loop;
	//static void afterClose(Handle* handle);
	//static void afterRead(uv_stream_t* handle, ssize_t nread, uv_buf_t buf); 
	//static void afterWrite(uv_write_t* req, int status); 

	//virtual void setErrno(const uv_err_t& err);
	//virtual bool setState(unsigned int id, const std::string& message = "");
	//static uv_buf_t defaultAlloc(Handle* handle, size_t size); 