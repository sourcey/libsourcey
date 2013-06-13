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


#include "Sourcey/UV/UDPSocket.h"
#include "Sourcey/Logger.h"


using namespace std;

using Sourcey::Net::Address;
using Sourcey::Exception;


namespace Sourcey {
namespace UV {


UDPSocket::UDPSocket(LoopHandle* loop) :
	UV::Interface(loop)
{
	Log("debug") << "[UDPSocket:" << this << "] Creating" << endl;

	int r = uv_udp_init(loop, &_handle);
	assert(r == 0); // TODO: throw exc

	_handle.data = this;
}


UDPSocket::~UDPSocket()
{
	Log("debug") << "[UDPSocket:" << this << "] Destroying" << endl;
}


void UDPSocket::connect(const Address& peerAddress) 
{
	// TODO: Restrict received data to given address...
}


void UDPSocket::close()
{
	Log("debug") << "[UDPSocket:" << this << "] Closing" << endl;	

	uv_close((Handle*)&_handle, afterClose);
	//uv_close(_connection.handle, afterClose);
}


bool UDPSocket::bind(const Address& address, unsigned flags) 
{	
	Log("debug") << "[UDPSocket:" << this << "] Binding on " << address << endl;

	int r;

	switch (address.af()) {
	case AF_INET:
		r = uv_udp_bind(&_handle, 
			*reinterpret_cast<const sockaddr_in*>(address.addr()), flags);
		break;
	case AF_INET6:
		r = uv_udp_bind6(&_handle, 
			*reinterpret_cast<const sockaddr_in6*>(address.addr()), flags);
		break;
	default:
		throw Exception("Unexpected address family");
	}

	if (r) 
		setErrno(uv_last_error(_loop));
	else
		recvStart();

	return r == 0;
}


int UDPSocket::send(const char* data, int size, const Address& peerAddress) 
{	
	assert(size <= Net::MAX_UDP_PACKET_SIZE);
	int r;
	UDPSendRequest* sr = new UDPSendRequest;
	sr->buf = uv_buf_init((char *)data, size); // TODO: memcpy data?
	sr->addr = peerAddress;

	switch (peerAddress.af()) {
	case AF_INET:
		r = uv_udp_send(&sr->req, &_handle, &sr->buf, 1,
			*reinterpret_cast<const sockaddr_in*>(sr->addr.addr()), UDPSocket::onSend);
		break;
	case AF_INET6:
		r = uv_udp_send6(&sr->req, &_handle, &sr->buf, 1,
			*reinterpret_cast<const sockaddr_in6*>(sr->addr.addr()), UDPSocket::onSend);
		break;
	default:
		throw Exception("Unexpected address family");
	}

	return r;
}


bool UDPSocket::recvStart() 
{
	// UV_EALREADY means that the socket is already bound but that's okay
	int r = uv_udp_recv_start(&_handle, UV::defaultAlloc, onRecv);
	if (r && uv_last_error(_loop).code != UV_EALREADY) {
		setErrno(uv_last_error(_loop));
		return false;
	}
  
	return true;
}


bool UDPSocket::recvStop() 
{
	return uv_udp_recv_stop(&_handle) == 0;
}


Net::Address UDPSocket::address() 
{
	struct sockaddr address;

	int addrlen = sizeof(address);
	int r = uv_udp_getsockname(&_handle, &address, &addrlen);

	if (r) {
		setErrno(uv_last_error(_loop));
		return Address();
	}
  
	return Address(&address, addrlen); //reinterpret_cast<sockaddr>(address)
}


/*
void UDPSocket::setErrno(const uv_err_t& err)
{
	_error = err;
	Log("error") << "[UDPSocket:" << this << "] Error: " << uv_strerror(err) << endl;	
	//setState(ClientState::Disconnected, uv_strerror(err));
	//shutdown();
}


LoopHandle* UDPSocket::loop()
{
	return _loop;
}


uv_err_t UDPSocket::error()
{
	return _error;
}
*/


//
// Callbacks
//

void UDPSocket::onRecv(uv_udp_t* handle, ssize_t nread, uv_buf_t buf, struct sockaddr* addr, unsigned flags) 
{	
	UDPSocket* socket = static_cast<UDPSocket*>(handle->data);

	Log("debug") << "[UDPSocket: " << socket << "] Handle Recv: " << nread << endl;
	
	// EOF or Error
	if (nread == -1)  {
		socket->setErrno(uv_last_error(socket->loop()));
	}

	Net::Address peerAddress(addr, sizeof *addr);
	socket->OnRecv.emit(socket, buf.base, nread, peerAddress);
}


void UDPSocket::onSend(uv_udp_send_t* req, int status) 
{
	UDPSendRequest* sr = reinterpret_cast<UDPSendRequest*>(req);
	UDPSocket* socket = reinterpret_cast<UDPSocket*>(sr->req.handle->data);
	
	Log("debug") << "[UDPSocket: " << socket << "] Send Callback: " << status << endl;

	if (status) 
		socket->setErrno(uv_last_error(socket->_loop));
		
	// TODO: Move to after callbak
	socket->OnSend.emit(socket, sr->buf.base, sr->buf.len, sr->addr);

	delete sr;
}


void UDPSocket::afterClose(Handle* handle) 
{
	UDPSocket* socket = reinterpret_cast<UDPSocket*>(handle->data);
	Log("debug") << "[UDPSocket: " << socket << "] On Close" << endl;
}


/*




void UDPSocket::afterRead(uv&_handle_t* handle, ssize_t nread, uv_buf_t buf) 
{
	UDPSocket* socket = reinterpret_cast<UDPSocket*>(handle->data);

	Log("debug") << "[UDPSocket: " << socket << "] Read Callback: " << nread << endl;
}

void UDPSocket::onConnected(uv_connect_t* req, int status)
{
	UDPSocket* socket = reinterpret_cast<UDPSocket*>(req->handle->data);
	socket->readStart();
}
*/



/*
UDPSocket* UDPSocket::acceptConnection()
{
	UDPSocket* conn = new UDPSocket(_loop);
	int r = uv_accept(&_handle, conn->&_handle);
	assert(r == 0); // uv_accept should always work.
	conn->readStart();
	return NULL;
}


void UDPSocket::onAcceptConnection(uv&_handle_t* handle, int status) {

	UDPSocket* socket = reinterpret_cast<UDPSocket*>(handle->data);

	Log("debug") << "[UDPSocket: " << socket << "] On Connection: " << status << endl;

	if (status) {
		socket->setErrno(uv_last_error(socket->_loop));
		return;
	}

	UDPSocket* conn = socket->acceptConnection();
	
	// TODO: Check ref count or delete
}
*/




 
/*
void UDPSocket::setErrno(const uv_err_t& err)
{
	_error = err;
	Log("error") << "[UDPSocket:" << this << "] Socket Error: " << uv_strerror(err) << endl;	
	//setState(ClientState::Disconnected, uv_strerror(err));
	close();
}

bool UDPSocket::setState(unsigned int id, const std::string& message)
{
	return Stateful<ClientState>::setState(this, id, message);
}
*/
	
	//assert(&_handle == (uv_udp_t*)handle);

	// We should not be getting this callback if someone as already called
	// uv_close() on the handle.
	//assert(wrap->object_.IsEmpty() == false);	
	
  /*
  uv&_handle_t* stream;
  int r;

  if (status != 0) {
    //fprintf(stderr, "Connect error %d\n",
    //    uv_last_error(loop).code);
  }
  assert(status == 0);
  
    stream = (uv&_handle_t*)malloc(sizeof(uv_udp_t));
    assert(stream != NULL);
    r = uv_udp_init(socket->_loop, (uv_udp_t*)stream);
    assert(r == 0);


  // associate server with stream 
  stream->data = socket;

  r = uv_accept(handle, stream);
  assert(r == 0);

  r = uv_read_start(stream, UV::defaultAlloc, afterRead); //, echo_alloc, after_read);
  assert(r == 0);
  */



	
	/*
	//socket->

	
	//uv&_handle_t* stream;
    //stream = malloc(sizeof(uv_udp_t));
	UDPSocket* conn = new UDPSocket(socket->_loop);

	int r = uv_accept((uv&_handle_t*)socket->&_handle, (uv&_handle_t*)conn->&_handle);
	assert(r == 0); // uv_accept should always work.

	uv_udp_t* stream = new uv_udp_t;
	int r = uv_accept(handle, (uv&_handle_t*)stream);	
	assert(r == 0); // uv_accept should always work.
	//return new UDPSocket(stream, _loop);
	//uv&_handle_t* stream;
    //stream = malloc(sizeof(uv_udp_t));
	
	Log("debug") << "[UDPSocket: " << socket << "] On Connection 1: " << status << endl;

	
	Log("debug") << "[UDPSocket: " << socket << "] On Connection 2: " << status << endl;


	//conn->send("aaa", 3);
	*/


  //MakeCallback(wrap->object_, "onconnection", 1, argv);
	/*
    // Instantiate the client javascript object and handle.
    Local<Object> client_obj = Instantiate();

    // Unwrap the client javascript object.
    assert(client_obj->InternalFieldCount() > 0);
    UDPSocket* client_wrap =
        static_cast<UDPSocket*>(client_obj->GetPointerFromInternalField(0));
	*/
	/*
    // Successful accept. Call the onconnection callback in JavaScript land.
    argv[0] = client_obj;

uv_buf_t UDPSocket::defaultAlloc(Handle* handle, size_t size) 
{
	uv_buf_t buf;
	buf.base = (char*)malloc(size);
	buf.len = size;
	return buf;
}
	*/
	


	//uv_write_t req;
	//uv_buf_t buf = uv_buf_init((char *)data, size);
	//if (uv_write(&req, _connection.handle, &buf, 1, onSend)) {
	/*
	Log("debug") << "onConnectedonConnected: " << status << endl;	
	
	const int WRITES = 4;
	const int CHUNK_SIZE = 4;
	const int CHUNKS_PER_WRITE = 4;

	//uv_buf_t send_bufs[CHUNKS_PER_WRITE];
  uv_udp_t* tcp;
  uv_write_t* write_req;
  uv_shutdown_t* shutdown_req;
  int i, j, r;

  assert(req != NULL);
  assert(status == 0);

  tcp = (uv_udp_t*)req->handle;

  //connect_cb_called++;
  //free(req);

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
	r = uv_write(write_req, (uv&_handle_t*) tcp, (uv_buf_t*)&buf,
		1, onSend);

	
		
	r = uv_read_start((uv&_handle_t*)tcp, defaultAlloc, afterRead);
	assert(r == 0);
  */
	//}

	// Error starting the tcp.
	//if (r) setErrno(uv_last_error(loop));

  // shutdown on drain. FIXME: dealloc req? 
 // shutdown_req = malloc(sizeof(uv_shutdown_t));
 // assert(shutdown_req != NULL);
 // r = uv_shutdown(shutdown_req, (uv&_handle_t*)tcp, shutdown_cb);
  //assert(r == 0);

  // Start reading 
 // r = uv_read_start((uv&_handle_t*)tcp, alloc_cb, read_cb);
 // assert(r == 0);

	/*
	if (isActive())
		throw Exception("The UDP Client is already active.");
	
	FastMutex::ScopedLock lock(_mutex); 
	
	try {
		setState(this, ClientState::Connecting);
	*/

	
	/*
	// Buffer belongs to user
	free(req);

	write_req_t* wr;
	wr = (write_req_t*) req;

	 // Free the read/write buffer and the request
	free(wr->buf.base);
	free(wr);
	free(req);
	free(req.base);
	*/
		//uv_err_t err = uv_last_error(_loop);
		
		//UV_UDP_PRIVATE_FIELDS;
		//UV_HANDLE_FIELDS;
		///UV_HANDLE_FIELDS;
		//uv_udp_t handle_;
		//UV_HANDLE_FIELDS b;
		// UV_STREAM_FIELDS a;
	

	//write_req_t wr;
	//wr = (write_req_t*) malloc(sizeof *wr);

	//wr.buf = uv_buf_init(data, size);

	//int r = uv_write(&req, (uv&_handle_t*)&_handle, &buf, 1, onSend);
		
	//int uv_udp_write(LoopHandle* loop, uv_write_t* req, uv_udp_t* handle,
	//	uv_buf_t bufs[], int bufcnt, uv_write_cb cb);loop, 
	
	//UV_EXTERN int uv_write(uv_write_t* req, uv&_handle_t* handle,
    //	uv_buf_t bufs[], int bufcnt, uv_write_cb cb);

	//uv_write_t* write_req;
    //write_req = (uv_write_t*)malloc(sizeof(uv_write_t));
    //assert(write_req != NULL);
		
	
	/*

	// NOTE: Must be connected to peer
	int sent = 0;
	try	{
		FastMutex::ScopedLock lock(_mutex); 
		Log("trace") << "[UDPSocket:" << this << "] SEND: " << size << ": " 
			<< _impl.address().toString() << "-->" << _impl.peerAddress().toString() << endl;
		sent = _impl.sendBytes(data, size);
	}
	catch (Sourcey::Exception& exc) {
		Log("error") << "[UDPSocket:" << this << "] Error: " << exc.displayText() << endl;
	}
	assert(sent == size);
	return sent;
	*/
		/*
		// Break out of the current loop so we can 
		// initialize the outgoing connection.
		//uv_unref(_loop);
		//uv_ref(_loop);
		//uv_run(_loop);
		

		Log("info") << "[UDPSocket:" << this << "] EVENT LOOP RUNNING" << endl;
		
		//uv_unref(loop);
		uv_run(loop);

		//else {
		//	return scope.Close(req_wrap->object_);
		//}


		//LPFN_CONNECTEX func_connectaaa; 
	
	uv_connect_t	_connection;
	uv_udp_t		&_handle;

		//uv_udp_t handle_;
		//uv_connect_t*			_impl1;//"127.0.0.1", TEST_PORT
		uv_udp_t* client = (uv_udp_t*)malloc(sizeof *client);

		int r = uv_udp_connect(_impl1, client, address,
			  onConnected);

		
		  struct sockaddr_in addr = uv_ip4_addr("127.0.0.1", TEST_PORT);
		  uv_udp_t* client = (uv_udp_t*)malloc(sizeof *client);
		  uv_connect_t* connect_req = malloc(sizeof *connect_req);
		  int r;

		  assert(client != NULL);
		  assert(connect_req != NULL);

		  r = uv_udp_init(loop, client);
		  assert(r == 0);

		  r = uv_udp_connect(connect_req, client, addr, connect_cb);
		  assert(r == 0);


		//if (timeout)
		//	_impl.connect(peerAddress, Timespan(timeout, 0));
		//else
		//	_impl.connect(peerAddress);
		//bindEvents();
		resetBuffer();
		//setState(ClientState::Connected); // async via onWritable
	} 
		  */

	/*
	catch (Sourcey::Exception& e) {
		setState(this, ClientState::Disconnected, e.displayText());
		Log("error") << "[UDPSocket:" << this << "] Connection failed: " << e.displayText() << endl;
		e.rethrow();
	}
	*/




/*

UDPSocket::UDPSocket(Reactor& reactor, bool destroyafterClose) :
	_reactor(reactor),
	_destroyafterClose(destroyafterClose)
{
	Log("debug") << "[UDPSocket:" << this << "] Creating" << endl;
	//bindEvents();
}


UDPSocket::UDPSocket(const Sourcey::Net::StreamSocket& socket, Reactor& reactor, bool destroyafterClose) :
	_impl(socket),
	_reactor(reactor),
	_destroyafterClose(destroyafterClose)
{
	Log("debug") << "[UDPSocket:" << this << "] Creating" << endl;
	//bindEvents();
}


void UDPSocket::transferTo(UDPSocket* r)
{
	Sourcey::Net::StreamSocket& rImpl = r->impl();
	 rImpl = _impl;
	delete this;
	//r = NULL;
	r->bindEvents();
}
*/


/*
void UDPSocket::write_cb(uv_write_t* req, int status) {
  assert(req != NULL);

  if (status) {
    uv_err_t err = uv_last_error(loop);
    fprintf(stderr, "uv_write error: %s\n", uv_strerror(err));
    assert(0);
  }

  bytes_sent_done += CHUNKS_PER_WRITE * CHUNK_SIZE;
  write_cb_called++;

  free(req);
}
  */



/*
Handle<Value> UDPSocket::Connect(const Arguments& args) {
  HandleScope scope;

  UNWRAP

  String::AsciiValue ip_address(args[0]->ToString());
  int port = args[1]->Int32Value();

  struct sockaddr_in address = uv_ip4_addr(*ip_address, port);

  // I hate when people program C++ like it was C, and yet I do it too.
  // I'm too lazy to come up with the perfect class hierarchy here. Let's
  // just do some type munging.
  ConnectWrap* req_wrap = new ConnectWrap();

  int r = uv_udp_connect(&req_wrap->req_, &_handle, address,
      onConnected);

  req_wrap->Dispatched();

  if (r) {
    setErrno(uv_last_error(loop));
    delete req_wrap;
    return scope.Close(v8::Null());
  } else {
    return scope.Close(req_wrap->object_);
  }
}


void UDPSocket::close()
{
	Log("debug") << "[UDPSocket:" << this << "] Closing" << endl;	
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


void UDPSocket::afterReadable(ReadableNotification* pNf) 
{
	Log("debug") << "[UDPSocket:" << this << "] On Readable" << endl;
	pNf->release();

	try	{
		int size = 0;
		Buffer* buffer = NULL;;
		{
			FastMutex::ScopedLock lock(_mutex); 

			// Resize our buffer as required.
			// This is preferable to allocating a huge buffer on startup.
			size = min(_impl.available(), MAX_UDP_PACKET_SIZE);
			if (_buffer.capacity() < size) {
				Log("debug") << "[UDPSocket:" << this << "] Resizing buffer: " << size << endl;
				_buffer.resize(size);
			}
			size = _impl.receiveBytes(_buffer.bytes(), size);
			Log("trace") << "[UDPSocket:" << this << "] RECV: " << size << ": "
				<< _impl.address().toString() << "<--" << _impl.peerAddress().toString() << endl;
			buffer = &_buffer;
			_buffer.position(0);
			_buffer.size(size);
		}
		
		if (size) {	
			receive(*buffer);
		}
		else {
			Log("debug") << "[UDPSocket:" << this << "] Received EOF" << endl;
			close();
		}
	}
	catch (Sourcey::Net::ConnectionResetException& exc) {
		Log("error") << "[UDPSocket:" << this << "] Peer closed connection: " << exc.displayText() << endl;
		setState(this, ClientState::Disconnected, "Peer closed connection");
		close();
	}
}


void UDPSocket::receive(Buffer& buffer) 
{
	// No processing done by default..
	//packetize(buffer);
	OnData.emit(this, buffer);
}


void UDPSocket::onWritable(WritableNotification* pNf) 
{
	Log("debug") << "[UDPSocket:" << this << "] On Writable: " << _impl.available() << endl;
	pNf->release();

	// This event tells us that we are connected. ie. 
	// the connection is writable. There is no more 
	// need for this event so we can remove the handler.
	_reactor.removeEventHandler(_impl, Sourcey::Observer<UDPSocket, WritableNotification>(*this, &UDPSocket::onWritable));

	setState(this, ClientState::Connected);

	// In high traffic situations the socket may become
	// writable with data in the read buffers, and the
	// readable notification get's missed. Let's flush 
	// the beffers now...
	if (bytesAvailable() > 0) {
		ReadableNotification* rNf = new ReadableNotification(&_reactor);
		afterReadable(rNf);
	}
}


void UDPSocket::onError(ErrorNotification* pNf) 
{
	Log("warn") << "[UDPSocket:" << this << "] On Error" << endl;
	pNf->release();
	if (!stateEquals(ClientState::Error)) {
		setState(this, ClientState::Disconnected, "Connection error");
		close();
	}
}

*/


/*
int UDPSocket::send(const IPacket& packet) 
{
	Buffer buf;
	packet.write(buf);
	return send(buf.data(), buf.size());
}


void UDPSocket::resetBuffer() 
{	
	FastMutex::ScopedLock lock(_mutex); 

	// Set the initial buffer size to the getReceiveBufferSize value.
	// If read sizes surpass this value the buffer size will be adjusted
	// accordingly.
	int recvSize = _impl.getReceiveBufferSize();
	if (recvSize != _buffer.capacity()) {
		Log("debug") << "[UDPSocket:" << this << "] Buffer Size: " << recvSize << endl;
		_buffer.resize(recvSize);
	}	
	_buffer.clear();
}


void UDPSocket::bindEvents()
{
	//unbindEvents(); // must unbind or we will have problems
	_reactor.addEventHandler(_impl, Sourcey::Observer<UDPSocket, ReadableNotification>(*this, &UDPSocket::afterReadable));
	_reactor.addEventHandler(_impl, Sourcey::Observer<UDPSocket, WritableNotification>(*this, &UDPSocket::onWritable));
	_reactor.addEventHandler(_impl, Sourcey::Observer<UDPSocket, ErrorNotification>(*this, &UDPSocket::onError));	
}


void UDPSocket::unbindEvents()
{
	_reactor.removeEventHandler(_impl, Sourcey::Observer<UDPSocket, ReadableNotification>(*this, &UDPSocket::afterReadable));
	_reactor.removeEventHandler(_impl, Sourcey::Observer<UDPSocket, WritableNotification>(*this, &UDPSocket::onWritable));
	_reactor.removeEventHandler(_impl, Sourcey::Observer<UDPSocket, ErrorNotification>(*this, &UDPSocket::onError));
}
	

void UDPSocket::setDestroyafterClose(bool flag)
{
	FastMutex::ScopedLock lock(_mutex);
	_destroyafterClose = flag;
}


Reactor& UDPSocket::reactor()
{
	FastMutex::ScopedLock lock(_mutex); 
	return _reactor;
}


Sourcey::Net::StreamSocket& UDPSocket::impl()
{
	FastMutex::ScopedLock lock(_mutex); 
	return _impl;
}


bool UDPSocket::isActive()
{
	return stateEquals(ClientState::Connecting)
		|| stateEquals(ClientState::Connected)
		|| stateEquals(ClientState::Online);
}


bool UDPSocket::isConnected()
{
	return stateEquals(ClientState::Connected)
		|| stateEquals(ClientState::Online);
}


bool UDPSocket::isError()
{
	return stateEquals(ClientState::Error);
}


int UDPSocket::bytesAvailable()
{
	FastMutex::ScopedLock lock(_mutex); 
	return _impl.available();
}


Address UDPSocket::localAddr() const 
{
	FastMutex::ScopedLock lock(_mutex); 
	return _impl.address();
}


Address UDPSocket::peerAddress() const 
{
	FastMutex::ScopedLock lock(_mutex); 
	return _impl.peerAddress();
}


TransportType UDPSocket::transport() const 
{ 
	return Net::UDP; 
}
*/


} } // namespace Sourcey::UV