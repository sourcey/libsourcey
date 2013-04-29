//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is is distributed under a dual license that allows free, 
// open source use and closed source use under a standard commercial
// license.
//
// Non-Commercial Use:
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// Commercial Use:
// Please contact mail@sourcey.com
//


#include "Sourcey/UV/TCPContext.h"
#include "Sourcey/Logger.h"

#include "Poco/Exception.h"


using namespace std;


namespace Sourcey {
namespace UV {


TCPContext::TCPContext(LoopHandle* loop) : 
	IOStream(loop)
{
	Log("debug") << "[TCPContext:" << this << "] Creating" << endl;

	_stream = (uv_stream_t*)new uv_tcp_t;
	_stream->data = this;

	_connection.data = this;

	int r = uv_tcp_init(loop, (uv_tcp_t*)_stream);
	assert(r == 0); // TODO: throw exc
}


/*
TCPContext::TCPContext(uv_tcp_t* stream, LoopHandle* loop) : 
	IOStream((uv_stream_t*)stream, loop)
{
	int r = uv_tcp_init(loop, (uv_tcp_t*)_stream);
	assert(r == 0); // TODO: throw exc
}
*/

	
TCPContext::~TCPContext() 
{	
	Log("debug") << "[TCPContext:" << this << "] Destroying" << endl;
}


	/*
void handleRead(uv_stream_t* handle, ssize_t nread, uv_buf_t buf) //, uv_handle_type pending
{	
	TCPContext* socket = reinterpret_cast<TCPContext*>(handle->data);
	//IOStream* io = static_cast<IOStream*>(handle->data);

	Log("debug") << "[IOStream: " << socket << "] Handle Read: " << nread << endl;

	// EOF or Error
	if (nread == -1)  {
		socket->setErrno(uv_last_error(socket->loop()));
		socket->OnRead.emit(socket, NULL, 0);
		return;
	}

	else {
		if (pending == UV_TCP) {
			assert(0);
		} else {
			// We only support sending UV_TCP right now.
			assert(pending == UV_UNKNOWN_HANDLE);
		}
		
		socket->OnRead.emit(socket, buf.base, buf.len);
	//}
}
		*/


	/*
void onConnected1(ConnectReq* conn, int status)
{
	//assert(status == 0);
	TCPContext* sock = reinterpret_cast<TCPContext*>(conn->data);
	
	Log("debug") << "[TCPContext:" << conn << "] FIXMEEEEE: " << conn << endl;
	Log("debug") << "[TCPContext:" << conn << "] FIXMEEEEE 1: " << sock->_connection << endl;

	assert(conn == sock->_connection);

	Log("debug") << "[TCPContext: " << sock << "] On Connection: " << status << endl;

	if (status) {
		sock->setErrno(uv_last_error(sock->loop()));
		assert(false);
	}

	Log("debug") << "[TCPContext: " << sock << "] On Connection: 1: " << status << endl;
	status = uv_read_start((uv_stream_t*)sock->_stream, UV::defaultAlloc, handleRead);	
	if (status) {
		sock->setErrno(uv_last_error(sock->loop()));
		assert(false);
	}

	Log("debug") << "[TCPContext: " << sock << "] On Connection: 2: " << status << endl;


static void connect_cb(ConnectReq* req, int status) {
	assert(0);
}
}
	*/
void onConnected1(ConnectReq* conn, int status)
{
	assert(0);
}


void TCPContext::connect(const Net::Address& peerAddress) 
{
	Log("debug") << "[TCPContext:" << this << "] Connecting to " << peerAddress.toString() << endl;
	//ConnectReq* conn = new ConnectReq;
	//conn.data = this;
	const sockaddr_in* addr = reinterpret_cast<const sockaddr_in*>(peerAddress.addr());
	int r = uv_tcp_connect(&_connection, (uv_tcp_t*)_stream, *addr, UV::onConnected); //connect_cb); //
	if (r) {
		uv_err_t err = uv_last_error(_loop);
		setErrno(err);
		throw Poco::Exception(uv_strerror(err));
	}
}


void TCPContext::close()
{
	Log("debug") << "[TCPContext:" << this << "] Closing" << endl;
	
	//uv_close((Handle*)(uv_tcp_t*)_stream, afterClose);
	//uv_close(_connection.handle, afterClose);
	IOStream::shutdown();
}


int TCPContext::send(const char* data, int size) 
{	
	assert(size <= Net::MAX_TCP_PACKET_SIZE);
	
	Log("debug") << "[TCPContext:" << this << "] Send: " << size << endl;

	uv_write_t* req = new uv_write_t;
	uv_buf_t buf = uv_buf_init((char *)data, size); // TODO: copy data?
	if (uv_write(req, (uv_stream_t*)_stream, &buf, 1, afterWrite)) //_connection.handle
		return -1;
	return size;
}


void TCPContext::setNoDelay(bool enable) 
{
	int r = uv_tcp_nodelay((uv_tcp_t*)_stream, enable ? 1 : 0);
	if (r) setErrno(uv_last_error(_loop));
}


void TCPContext::setKeepAlive(int enable, unsigned int delay) 
{
	int r = uv_tcp_keepalive((uv_tcp_t*)_stream, enable, delay);
	if (r) setErrno(uv_last_error(_loop));
}


#ifdef _WIN32
void TCPContext::setSimultaneousAccepts(bool enable) 
{
	int r = uv_tcp_simultaneous_accepts((uv_tcp_t*)_stream, enable ? 1 : 0);
	if (r) setErrno(uv_last_error(_loop));
}
#endif


bool TCPContext::bind(const Net::Address& address) 
{
	Log("debug") << "[TCPContext:" << this << "] Binding on " << address.toString() << endl;

	const sockaddr_in* addr = reinterpret_cast<const sockaddr_in*>(address.addr());
	int r = uv_tcp_bind((uv_tcp_t*)_stream, *addr);
	if (r) 
		setErrno(uv_last_error(_loop));

	return r == 0;
}


bool TCPContext::bind6(const Net::Address& address) 
{
	const sockaddr_in6* addr = reinterpret_cast<const sockaddr_in6*>(address.addr());
	int r = uv_tcp_bind6((uv_tcp_t*)_stream, *addr);
	if (r) setErrno(uv_last_error(_loop));  
	return r == 0;
}


bool TCPContext::listen(int backlog) 
{
	Log("debug") << "[TCPContext:" << this << "] Listening" << endl;

	int r = uv_listen((uv_stream_t*)(uv_tcp_t*)_stream, backlog, UV::onAcceptConnection);
	if (r) setErrno(uv_last_error(_loop));  
	return r == 0;
}


TCPContext* TCPContext::acceptConnection()
{
	TCPContext* conn = new TCPContext(_loop);
	int r = uv_accept(_stream, conn->_stream);
	assert(r == 0); // uv_accept should always work.
	conn->readStart();
	OnAcceptConnection.emit(this, conn);
	return NULL;
}


//
// Callbacks
//
void TCPContext::onConnected(int status) //ConnectReq* req, 
{
	if (status) {
		Log("error") << "[TCPContext: " << this << "] Connect Failed: " << errorMessage() << endl;
	}
	else
		readStart();

	OnConnected.emit(this, status);
}


void TCPContext::onAcceptConnection(uv_stream_t* handle, int status) 
{		
	if (status == 0) {
		Log("debug") << "[TCPContext: " << this << "] On Connection" << endl;
		acceptConnection();
	}
	else
		Log("error") << "[TCPContext: " << this << "] Connection Failed" << endl;

	/*
	TCPContext* this = reinterpret_cast<TCPContext*>(handle->data);

	Log("debug") << "[TCPContext: " << this << "] On Connection: " << status << endl;

	if (status) {
		this->setErrno(uv_last_error(this->_loop));
		return;
	}

	*/
	
	// TODO: Check ref count or delete
}


} } // namespace Sourcey::UV