//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2002 Sourcey
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


#include "Sourcey/UV/TCPClientSocket.h"


using namespace std;
//using namespace Sourcey;

//using Sourcey::Net::ClientState;
using Sourcey::Net::Address;


namespace Sourcey {
namespace UV {

	
TCPClientSocket::TCPClientSocket(LoopHandle* loop) :
	TCPContext(loop)
{
	Log("debug") << "[TCPClientSocket:" << this << "] Creating" << endl;
}

	
TCPClientSocket::~TCPClientSocket() 
{	
	Log("debug") << "[TCPClientSocket:" << this << "] Destroying" << endl;
}
	

void TCPClientSocket::connect(const Address& peerAddress) 
{	
	setState(this, ClientState::Connecting);
	TCPContext::connect(peerAddress);
}


void TCPClientSocket::close() 
{
	setState(this, ClientState::Disconnected);
	TCPContext::close();
}


void TCPClientSocket::onConnected(int status) 
{
	setState(this, ClientState::Connected);
	TCPContext::onConnected(status);
}


void TCPClientSocket::setErrno(const uv_err_t& err) 
{
	TCPContext::setErrno(err);
	setState(this, ClientState::Disconnected, errorMessage());	
}



/*


	virtual void connect(const Address& peerAddress);	
	virtual void close();

	virtual void onConnected(int status);

	virtual void setErrno(const uv_err_t& err);

TCPClientSocket::TCPClientSocket(Reactor& reactor, bool destroyafterClose) :
	_reactor(reactor),
	_destroyafterClose(destroyafterClose)
{
	Log("debug") << "[TCPClientSocket:" << this << "] Creating" << endl;
	//bindEvents();
}


TCPClientSocket::TCPClientSocket(const Sourcey::Net::StreamSocket& socket, Reactor& reactor, bool destroyafterClose) :
	_impl(socket),
	_reactor(reactor),
	_destroyafterClose(destroyafterClose)
{
	Log("debug") << "[TCPClientSocket:" << this << "] Creating" << endl;
	//bindEvents();
}


void TCPClientSocket::transferTo(TCPClientSocket* r)
{
	Sourcey::Net::StreamSocket& rImpl = r->impl();
	 rImpl = _impl;
	delete this;
	//r = NULL;
	r->bindEvents();
}
*/


/*
static void write_cb(uv_write_t* req, int status) {
  ASSERT(req != NULL);

  if (status) {
    uv_err_t err = uv_last_error(DefaultLoop);
    fprintf(stderr, "uv_write error: %s\n", uv_strerror(err));
    ASSERT(0);
  }

  bytes_sent_done += CHUNKS_PER_WRITE * CHUNK_SIZE;
  write_cb_called++;

  free(req);
}
static void after_write(uv_write_t* req, int status) {
	
	Log("debug") << "after_writeafter_writeafter_write: " << status << endl;	

  if (status) {
    uv_err_t err = uv_last_error(DefaultLoop);
    fprintf(stderr, "uv_write error: %s\n", uv_strerror(err));
    assert(0);
  }
  */
  
	/*
  write_req_t* wr;
  wr = (write_req_t*) req;

  // Free the read/write buffer and the request
  free(wr->buf.base);
  free(wr);
}


void OnRead(uv_stream_t* handle, ssize_t nread, uv_buf_t buf) {
  //OnReadCommon(handle, nread, buf, UV_UNKNOWN_HANDLE);
	
    //uv_err_t err = uv_last_error(DefaultLoop);
    //fprintf(stderr, "uv_write error: %s\n", uv_strerror(err));
	
	Log("debug") << "OnReadOnReadOnRead: " << nread << endl;	
}


uv_buf_t defaultAlloc(Handle* handle, size_t size) {
  uv_buf_t buf;
  buf.base = (char*)malloc(size);
  buf.len = size;
  return buf;
}
  */


/*
uv_buf_t defaultAlloc(Handle* handle, size_t suggested_size) {
	Log("debug") << "defaultAlloc: " << suggested_size << endl;	
	return uv_buf_init((char *)"data", suggested_size);
}


static void AfterConnect(uv_connect_t* req, int status)
{
	Log("debug") << "AfterConnectAfterConnect: " << status << endl;	
	
	const int WRITES = 4;
	const int CHUNK_SIZE = 4;
	const int CHUNKS_PER_WRITE = 4;

	//uv_buf_t send_bufs[CHUNKS_PER_WRITE];
  uv_tcp_t* tcp;
  uv_write_t* write_req;
  uv_shutdown_t* shutdown_req;
  int i, j, r;

  assert(req != NULL);
  assert(status == 0);

  tcp = (uv_tcp_t*)req->handle;

  //connect_cb_called++;
  //free(req);
  
*/
  /*
  // Write a lot of data
  for (i = 0; i < WRITES; i++) {
    for (j = 0; j < CHUNKS_PER_WRITE; j++) {
      send_bufs[j].len = CHUNK_SIZE;
      send_bufs[j].base = send_buffer + bytes_sent;
      bytes_sent += CHUNK_SIZE;
    }


    assert(r == 0);
  }

    write_req = (uv_write_t*)malloc(sizeof(uv_write_t));
    assert(write_req != NULL);

	//_buf = uv_buf_init((char *)"data", 4);
	uv_buf_t buf = uv_buf_init((char *)"data", 4);
	r = uv_write(write_req, (uv_stream_t*) tcp, (uv_buf_t*)&buf,
		1, after_write);

	
		
	r = uv_read_start((uv_stream_t*)tcp, defaultAlloc, OnRead);
	assert(r == 0);
	//}

	// Error starting the tcp.
	//if (r) setErrno(uv_last_error(DefaultLoop));

  // Shutdown on drain. FIXME: dealloc req? 
 // shutdown_req = malloc(sizeof(uv_shutdown_t));
 // assert(shutdown_req != NULL);
 // r = uv_shutdown(shutdown_req, (uv_stream_t*)tcp, shutdown_cb);
  //assert(r == 0);

  // Start reading 
 // r = uv_read_start((uv_stream_t*)tcp, alloc_cb, read_cb);
 // assert(r == 0);
}

  */

	/*
void TCPClientSocket::connect(const Address1& remoteAddr) 
{
	Log("debug") << "[TCPClientSocket:" << this << "] Connecting to " << remoteAddr.toString() << endl;	

	if (isActive())
		throw Exception("The TCP Client is already active.");
	
	FastMutex::ScopedLock lock(_mutex); 
	
	try {
		setState(this, ClientState::Connecting);
		//UV_TCP_PRIVATE_FIELDS;
		//UV_HANDLE_FIELDS;
		///UV_HANDLE_FIELDS;
		//uv_tcp_t handle_;
		//UV_HANDLE_FIELDS b;
		// UV_STREAM_FIELDS a;

		struct sockaddr_in address = uv_ip4_addr(remoteAddr.host().toString().data(), remoteAddr.port()); 
		int r = uv_tcp_connect(&_connection, &_client, uv_ip4_addr(remoteAddr.host().toString().data(), remoteAddr.port()), 
			AfterConnect);

		if (r) {
			const char* err = uv_err_name(uv_last_error(DefaultLoop));
			Log("error") << "[TCPClientSocket:" << this << "] Connection failed: " << err << endl;
			setState(this, ClientState::Disconnected, err);
			throw Exception(err);
		} 
		
		Log("info") << "[TCPClientSocket:" << this << "] EVENT LOOP RUNNING" << endl;
		
		//uv_unref(DefaultLoop);
		uv_run(DefaultLoop);
	*/
		

		/*
		
		//else {
		//	return scope.Close(req_wrap->object_);
		//}


		//LPFN_CONNECTEX func_connectaaa; 
	
	uv_connect_t	_connection;
	uv_tcp_t		_client;

		//uv_tcp_t handle_;
		//uv_connect_t*			_impl1;//"127.0.0.1", TEST_PORT
		uv_tcp_t* client = (uv_tcp_t*)malloc(sizeof *client);

		int r = uv_tcp_connect(_impl1, client, address,
			  AfterConnect);

		
		  struct sockaddr_in addr = uv_ip4_addr("127.0.0.1", TEST_PORT);
		  uv_tcp_t* client = (uv_tcp_t*)malloc(sizeof *client);
		  uv_connect_t* connect_req = malloc(sizeof *connect_req);
		  int r;

		  assert(client != NULL);
		  assert(connect_req != NULL);

		  r = uv_tcp_init(DefaultLoop, client);
		  assert(r == 0);

		  r = uv_tcp_connect(connect_req, client, addr, connect_cb);
		  assert(r == 0);


		//if (timeout)
		//	_impl.connect(remoteAddr, Timespan(timeout, 0));
		//else
		//	_impl.connect(remoteAddr);
		//bindEvents();
		resetBuffer();
		//setState(ClientState::Connected); // async via onWritable
	} 
		  */

	/*
	catch (Sourcey::Exception& e) {
		setState(this, ClientState::Disconnected, e.displayText());
		Log("error") << "[TCPClientSocket:" << this << "] Connection failed: " << e.displayText() << endl;
		e.rethrow();
	}
}




int TCPClientSocket::send(const char* data, int size) 
{	
	assert(size <= MAX_TCP_PACKET_SIZE);
	

	_buf = uv_buf_init((char *)data, size);

	//write_req_t wr;
	//wr = (write_req_t*) malloc(sizeof *wr);

	//wr.buf = uv_buf_init(data, size);

	//int r = uv_write(&req, (uv_stream_t*)&_client, &buf, 1, after_write);
		
	//int uv_tcp_write(LoopHandle* loop, uv_write_t* req, uv_tcp_t* handle,
	//	uv_buf_t bufs[], int bufcnt, uv_write_cb cb);DefaultLoop, 
	
	//UV_EXTERN int uv_write(uv_write_t* req, uv_stream_t* handle,
    //	uv_buf_t bufs[], int bufcnt, uv_write_cb cb);
	//uv_write_t req;
	uv_write_t* write_req;
    write_req = (uv_write_t*)malloc(sizeof(uv_write_t));
    assert(write_req != NULL);

	//(uv_stream_t*)
	if (uv_write(write_req, _connection.handle, &_buf, 1, after_write)) {
		uv_err_t err = uv_last_error(DefaultLoop);
		Log("error") << "[TCPClientSocket:" << this << "] Send Error: " << uv_strerror(err) << endl;
	}
	else
		assert(0);
	
	*/
	/*

	// NOTE: Must be connected to peer
	int sent = 0;
	try	{
		FastMutex::ScopedLock lock(_mutex); 
		Log("trace") << "[TCPClientSocket:" << this << "] SEND: " << size << ": " 
			<< _impl.address().toString() << "-->" << _impl.peerAddress1().toString() << endl;
		sent = _impl.sendBytes(data, size);
	}
	catch (Sourcey::Exception& exc) {
		Log("error") << "[TCPClientSocket:" << this << "] Error: " << exc.displayText() << endl;
	}
	assert(sent == size);
	return sent;
	return size;
}

	*/



/*
Handle<Value> TCPWrap::Connect(const Arguments& args) {
  HandleScope scope;

  UNWRAP

  String::AsciiValue ip_address(args[0]->ToString());
  int port = args[1]->Int32Value();

  struct sockaddr_in address = uv_ip4_addr(*ip_address, port);

  // I hate when people program C++ like it was C, and yet I do it too.
  // I'm too lazy to come up with the perfect class hierarchy here. Let's
  // just do some type munging.
  ConnectWrap* req_wrap = new ConnectWrap();

  int r = uv_tcp_connect(&req_wrap->req_, &wrap->handle_, address,
      AfterConnect);

  req_wrap->Dispatched();

  if (r) {
    setErrno(uv_last_error(DefaultLoop));
    delete req_wrap;
    return scope.Close(v8::Null());
  } else {
    return scope.Close(req_wrap->object_);
  }
}


void TCPClientSocket::close()
{
	Log("debug") << "[TCPClientSocket:" << this << "] Closing" << endl;	
	bool destroy = false;
	{
		FastMutex::ScopedLock lock(_mutex);

		//unbindEvents();

		if (isActive())
			_impl.close();
		
		destroy = _destroyafterClose;
	}

	if (!stateEquals(ClientState::Error))
		setState(this, ClientState::Disconnected);

	if (destroy)
		delete this;
}


void TCPClientSocket::onReadable(ReadableNotification* pNf) 
{
	Log("debug") << "[TCPClientSocket:" << this << "] On Readable" << endl;
	pNf->release();

	try	{
		int size = 0;
		Buffer* buffer = NULL;;
		{
			FastMutex::ScopedLock lock(_mutex); 

			// Resize our buffer as required.
			// This is preferable to allocating a huge buffer on startup.
			size = min(_impl.available(), MAX_TCP_PACKET_SIZE);
			if (_buffer.capacity() < size) {
				Log("debug") << "[TCPClientSocket:" << this << "] Resizing buffer: " << size << endl;
				_buffer.resize(size);
			}
			size = _impl.receiveBytes(_buffer.bytes(), size);
			Log("trace") << "[TCPClientSocket:" << this << "] RECV: " << size << ": "
				<< _impl.address().toString() << "<--" << _impl.peerAddress1().toString() << endl;
			buffer = &_buffer;
			_buffer.position(0);
			_buffer.size(size);
		}
		
		if (size) {	
			receive(*buffer);
		}
		else {
			Log("debug") << "[TCPClientSocket:" << this << "] Received EOF" << endl;
			close();
		}
	}
	catch (Sourcey::Net::ConnectionResetException& exc) {
		Log("error") << "[TCPClientSocket:" << this << "] Peer closed connection: " << exc.displayText() << endl;
		setState(this, ClientState::Disconnected, "Peer closed connection");
		close();
	}
}


void TCPClientSocket::receive(Buffer& buffer) 
{
	// No processing done by default..
	//packetize(buffer);
	OnData.dispatch(this, buffer);
}


void TCPClientSocket::onWritable(WritableNotification* pNf) 
{
	Log("debug") << "[TCPClientSocket:" << this << "] On Writable: " << _impl.available() << endl;
	pNf->release();

	// This event tells us that we are connected. ie. 
	// the connection is writable. There is no more 
	// need for this event so we can remove the handler.
	_reactor.removeEventHandler(_impl, Sourcey::Observer<TCPClientSocket, WritableNotification>(*this, &TCPClientSocket::onWritable));

	setState(this, ClientState::Connected);

	// In high traffic situations the socket may become
	// writable with data in the read buffers, and the
	// readable notification get's missed. Let's flush 
	// the beffers now...
	if (bytesAvailable() > 0) {
		ReadableNotification* rNf = new ReadableNotification(&_reactor);
		onReadable(rNf);
	}
}


void TCPClientSocket::onError(ErrorNotification* pNf) 
{
	Log("warn") << "[TCPClientSocket:" << this << "] On Error" << endl;
	pNf->release();
	if (!stateEquals(ClientState::Error)) {
		setState(this, ClientState::Disconnected, "Connection error");
		close();
	}
}

*/


/*
int TCPClientSocket::send(const IPacket& packet) 
{
	Buffer buf;
	packet.write(buf);
	return send(buf.data(), buf.size());
}


void TCPClientSocket::resetBuffer() 
{	
	FastMutex::ScopedLock lock(_mutex); 

	// Set the initial buffer size to the getReceiveBufferSize value.
	// If read sizes surpass this value the buffer size will be adjusted
	// accordingly.
	int recvSize = _impl.getReceiveBufferSize();
	if (recvSize != _buffer.capacity()) {
		Log("debug") << "[TCPClientSocket:" << this << "] Buffer Size: " << recvSize << endl;
		_buffer.resize(recvSize);
	}	
	_buffer.clear();
}


void TCPClientSocket::bindEvents()
{
	//unbindEvents(); // must unbind or we will have problems
	_reactor.addEventHandler(_impl, Sourcey::Observer<TCPClientSocket, ReadableNotification>(*this, &TCPClientSocket::onReadable));
	_reactor.addEventHandler(_impl, Sourcey::Observer<TCPClientSocket, WritableNotification>(*this, &TCPClientSocket::onWritable));
	_reactor.addEventHandler(_impl, Sourcey::Observer<TCPClientSocket, ErrorNotification>(*this, &TCPClientSocket::onError));	
}


void TCPClientSocket::unbindEvents()
{
	_reactor.removeEventHandler(_impl, Sourcey::Observer<TCPClientSocket, ReadableNotification>(*this, &TCPClientSocket::onReadable));
	_reactor.removeEventHandler(_impl, Sourcey::Observer<TCPClientSocket, WritableNotification>(*this, &TCPClientSocket::onWritable));
	_reactor.removeEventHandler(_impl, Sourcey::Observer<TCPClientSocket, ErrorNotification>(*this, &TCPClientSocket::onError));
}
	

void TCPClientSocket::setDestroyafterClose(bool flag)
{
	FastMutex::ScopedLock lock(_mutex);
	_destroyafterClose = flag;
}


Reactor& TCPClientSocket::reactor()
{
	FastMutex::ScopedLock lock(_mutex); 
	return _reactor;
}


Sourcey::Net::StreamSocket& TCPClientSocket::impl()
{
	FastMutex::ScopedLock lock(_mutex); 
	return _impl;
}


bool TCPClientSocket::isActive()
{
	return stateEquals(ClientState::Connecting)
		|| stateEquals(ClientState::Connected)
		|| stateEquals(ClientState::Online);
}


bool TCPClientSocket::isConnected()
{
	return stateEquals(ClientState::Connected)
		|| stateEquals(ClientState::Online);
}


bool TCPClientSocket::isError()
{
	return stateEquals(ClientState::Error);
}


int TCPClientSocket::bytesAvailable()
{
	FastMutex::ScopedLock lock(_mutex); 
	return _impl.available();
}


Address1 TCPClientSocket::localAddr() const 
{
	FastMutex::ScopedLock lock(_mutex); 
	return _impl.address();
}


Address1 TCPClientSocket::remoteAddr() const 
{
	FastMutex::ScopedLock lock(_mutex); 
	return _impl.peerAddress1();
}


TransportType TCPClientSocket::transport() const 
{ 
	return Net::TCP; 
}
*/


} } // namespace Sourcey::UV