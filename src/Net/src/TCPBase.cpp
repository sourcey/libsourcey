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


#include "Sourcey/Net/TCPBase.h"
#include "Sourcey/Net/TCPSocket.h"
#include "Sourcey/Logger.h"


using namespace std;


namespace scy {
namespace net {


TCPBase::TCPBase(uv::Loop& loop) :
	Stream(loop) //, new uv_tcp_t
{
	traceL("TCPBase", this) << "Creating" << endl;
	init();	
	//_handle->data = this;
	//uv_tcp_init(&loop, handle<uv_tcp_t>());	
	//traceL("TCPBase", this) << "Creating: OK" << endl;
}

	
TCPBase::~TCPBase() 
{	
	traceL("TCPBase", this) << "Destroying" << endl;	
}


void TCPBase::init()
{
	if (closed()) {		
		traceL("TCPBase", this) << "Init" << endl;
		uv_tcp_t* tcp = new uv_tcp_t;
		tcp->data = this;
		_handle = reinterpret_cast<uv_handle_t*>(tcp);
		uv_tcp_init(loop(), tcp);
		//assert(tcp->data == this);		
		//_connectReq.data = instance();	
		traceL("TCPBase", this) << "Init: OK" << endl;
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
	assert(_connectReq.data != this);
	_connectReq.data = this;	
	const sockaddr_in* addr = reinterpret_cast<const sockaddr_in*>(peerAddress.addr());
	int r = uv_tcp_connect(&_connectReq, handle<uv_tcp_t>(), *addr, internal::onConnect);
	if (r) setAndThrowLastError("TCP connect failed");
}


void TCPBase::bind(const net::Address& address, unsigned flags) 
{
	traceL("TCPBase", this) << "Binding on " << address << endl;
	init();
	int r;
	const sockaddr_in& addr = reinterpret_cast<const sockaddr_in&>(*address.addr());
	switch (address.af()) {
	case AF_INET:
		r = uv_tcp_bind(handle<uv_tcp_t>(), *reinterpret_cast<const sockaddr_in*>(address.addr()));
		break;
	case AF_INET6:
		r = uv_tcp_bind6(handle<uv_tcp_t>(), *reinterpret_cast<const sockaddr_in6*>(address.addr()));
		break;
	default:
		throw Exception("Unexpected address family");
	}
	if (r) setAndThrowLastError("TCP bind failed");
}


void TCPBase::listen(int backlog) 
{
	traceL("TCPBase", this) << "Listening" << endl;
	init();
	int r = uv_listen(handle<uv_stream_t>(), backlog, internal::onAcceptConnection);
	if (r) setAndThrowLastError("TCP listen failed");
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
	if (r) setLastError();
}


void TCPBase::setKeepAlive(int enable, unsigned int delay) 
{
	init();
	int r = uv_tcp_keepalive(handle<uv_tcp_t>(), enable, delay);
	if (r) setLastError();
}


#ifdef _WIN32
void TCPBase::setSimultaneousAccepts(bool enable) 
{
	init();
	int r = uv_tcp_simultaneous_accepts(handle<uv_tcp_t>(), enable ? 1 : 0);
	if (r) setLastError();
}
#endif


int TCPBase::send(const char* data, int len, int flags) 
{	
	return send(data, len, peerAddress(), flags);
}


int TCPBase::send(const char* data, int len, const net::Address& peerAddress, int flags) 
{
	//if (len < 200)
	//	traceL("TCPBase", this) << "Send: " << string(data, len) << endl;

	//assert(peerAddress == this->peerAddress());
	//assert(initialized());
	assert(len <= net::MAX_TCP_PACKET_SIZE);
	
	//_debugOutput.append(data, len);
	//if (_debugOutput.length() > 200)
	//	traceL("TCPBase", this) << "Sending: " << string(_debugOutput.c_str(), 200) << std::endl;
	
	if (!Stream::write(data, len)) {
		warnL("TCPBase", this) << "Send error" << endl;	
		return -1;
	}

	// R is -1 on error, otherwise return len
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

	if (closed())
		return net::Address();
		//throw Exception("Invalid TCP socket: No address");
	
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
	if (closed())
		return net::Address();
		//throw Exception("Invalid TCP socket: No peer address");

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
	

bool TCPBase::closed() const
{
	return uv::Base::closed();
}


SOCKET TCPBase::sockfd() const
{
	return closed() ? INVALID_SOCKET : handle<uv_tcp_t>()->socket;
}


bool TCPBase::initialized() const
{
	return sockfd() != INVALID_SOCKET;
}


//
// Callbacks
//

void TCPBase::onRead(const char* data, int len)
{
	traceL("TCPBase", this) << "On read: " << len << endl;
	_buffer.position(0);
	_buffer.limit(len);
	onRecv(_buffer);
}


void TCPBase::onRecv(Buffer& buf)
{
	traceL("TCPBase", this) << "On recv: " << buf.available() << endl;
	emitRecv(buf, peerAddress());
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


void TCPBase::onAcceptConnection(uv_stream_t* handle, int status) 
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
	errorL("TCPBase", this) << "On error: " << error.message << endl;	
	emitError(error);
	close(); // close on error
}


void TCPBase::onClose() 
{		
	errorL("TCPBase", this) << "On close" << endl;	
	emitClose();
}


} } // namespace scy::net



	/*
	traceL("TCPBase", this) << "Send: " << len << endl;	

	assert(initialized());
	assert(len <= net::MAX_TCP_PACKET_SIZE);
	
	if (!Stream::write(data, len)) {
		warnL("TCPBase", this) << "Send error" << endl;	
		return -1;
	}
		//setLastError("Invalid TCP socket");

	// R is -1 on error, otherwise return len
	//return r ? r : len;
	return len;
	*/
	//net::SocketPacket packet(*this, buf, peerAddress());
	//assert(instance() == this);
	//Recv.emit(instance(), packet);

/*
void TCPBase::init()
{
	if (!stream()) {		
		traceL("TCPBase", this) << "Init" << endl;
		uv_tcp_t* tcp = new uv_tcp_t;
		setStream((uv_stream_t*)tcp);
		tcp->data = this;
		uv_tcp_init(loop(), tcp);
		assert(tcp->data == this);		
		_connectReq.data = instance();	
		traceL("TCPBase", this) << "Init: OK" << endl;
	}
}
*/

	/*
	uv_buf_t uvbuf;
	uvbuf.base = (char*)data;
	uvbuf.len = len;
	int r = uv_write(&_writeReq, stream(), &uvbuf, 1, NULL);
	if (r < 0) setLastError();
	return  r;
	*/


/*
void TCPBase::bind6(const net::Address& address, unsigned flags) 
{
	traceL("TCPBase", this) << "Binding on " << address << endl;
	init();
	const sockaddr_in6* addr = reinterpret_cast<const sockaddr_in6*>(address.addr());
	int r = uv_tcp_bind6(handle<uv_tcp_t>(), *addr);
	if (r) {
		uv_err_t err = uv_last_error(loop());
		setLastError();
		throw Exception(uv_strerror(err)); // TODO: make exception setError option
	}
	//return r == 0;
}
*/

/*
*/


	// TODO: Call stream startRecv
	/*

void TCPBase::onClose()
{
	traceL("TCPBase", this) << "On close" << endl;
	Close.emit(instance());
	traceL("TCPBase", this) << "On close: END" << endl;
}	

	int r = uv_read_start((uv_stream_t*)stream(), uv::defaultAlloc, uv::onRead);
	if (r == -1) {
		setLastError();
		assert(0);
	}
	*/
	

	/*
	// TODO: Use unique_ptr
	if (_handle) {		
		
		printf("~TCPBase");
		traceL("TCPBase", this) << "Destroying: Handle" << endl;
		//delete reinterpret_cast<uv_tcp_t*>(_handle);
		
		traceL("TCPBase", this) << "Destroying: Handle: UV__HANDLE_CLOSING: " << (_tcp->flags & 0x00000001) << endl;
		traceL("TCPBase", this) << "Destroying: Handle: handle->reqs_pending: " << _tcp->reqs_pending << endl;

		// if (handle->flags & UV__HANDLE_CLOSING &&
		//	 handle->reqs_pending == 0) {
		
		traceL("TCPBase", this) << "Destroying: uv_close" << endl;
		uv_close((uv_handle_t*)_tcp, aafterClose);
		//delete static_cast<uv_tcp_t*>((uv_stream_t*)_handle);
		delete _tcp;
		_handle = NULL;
		traceL("TCPBase", this) << "Destroying: Handle: OK" << endl;
	}

    //uv_close((uv_handle_t*)&_stream, uv::defaultClose);
	//uv_unref((uv_handle_t*)&_stream);	
	//if (_connection.handle)
	//	uv_close((uv_handle_t*)_connection.handle, afterClose);
	
	// TODO: Ensure socket closure
	//if (initialized())
	//close();


static void aafterClose(uv_handle_t* peer)
{
	traceL("TCPBase") << "aafterClose: " << peer << endl;	
}
	*/



//, _sslAdapter(nil)

	//_stream = (uv_stream_t*)new uv_tcp_t;
	//_stream->data = this;	
	//_connection.handle = NULL;
	//_connection.data = this;
	//int r = uv_tcp_init(loop, handle<uv_tcp_t>());
	//assert(r == 0); // TODO: throw exc
		
		//throw Exception Null(node_isolate);


	//_sslAdapter->addIncomingData(data, len);
	//_sslAdapter->update();
	//send("data", 4);
	//assert(0);



	/*
	// Send unencrypted data to the SSL context
	_sslAdapter.addOutgoingData(data, len);
	_sslAdapter.flush();
	return len;
	*/

	// Send raw data directly

 
	/*
	TCPBase* c = static_cast<TCPBase*>(this); //con->data
	if(status == -1) {
	printf("ERROR: on_connect_callback %s\n", uv_err_name(uv_last_error(loop())));
	//::exit(0);
	}TCPBase::
	*/
 
	/*
	string http_request(
		"GET / HTTP/1.1\r\n"
	"Host: 127.0.0.1\r\n"
	"User-Agent: uv_www_client/0.1\r\n" 
	"Accept: /*\r\n"
	"Connection: keep-alive\r\n"
	"\r\n");
 
	_sslAdapter.addOutgoingData(http_request);
	ssl = SSL_new(_sslCtx);
 
	SSL_set_connect_state(ssl);
	SSL_do_handshake(ssl);
 
	_sslAdapter.init(this, ssl); //write_tosocket_callback, , read_decrypted_callback, this
	_sslAdapter.flush();
	*/








	//struct sockaddr_in address = uv_ip4_addr("127.0.0.1", 1337);
	//int r = uv_tcp_connect(&_connectReq, handle<uv_tcp_t>(), *addr, uv::onConnect); //&_connection
	//__connectReq.handle = NULL;
	//_writeReq.handle = NULL;
	//_writeReq.data = this;
	//socket.handle = NULL;
  
	/* WES WORKING		
	//TCPBase* c = static_cast<TCPBase*>(this);
		printf("ERROR: write_tosocket error: %s\n", uv_err_name(uv_last_error(loop())));
	_sslAdapter.addOutgoingData(data, len);
	//_sslAdapter.addOutgoingData("bbbbbbbbbbbbbbbbbbbbbbbb");
	_sslAdapter.flush();
	return len;
	traceL("trace") << "write_tosocket_callback: " << len << endl;
	*/
	
	/*
	uv_write_t* req = new uv_write_t;
	uv_buf_t buf = uv_buf_init((char *)data, size); // TODO: copy data?
	if (uv_write(req, stream(), &buf, 1, afterWrite)) //_connection.handle
		return -1;
	return size;
	*/

 
 /*
	assert(_sslAdapter);
void on_read_callback(uv_stream_t* tcp, ssize_t nread, uv_buf_t buf) {
  TCPBase* c = static_cast<TCPBase*>(tcp->data);
  c->_sslAdapter.addIncomingData(buf.base, nread);
  c->_sslAdapter.flush();
}
*/


/*
void write_tosocket_callback(const char* data, size_t len, void* client) {
	traceL("trace") << "write_tosocket_callback: " << len << endl;
	
    TCPBase* c = static_cast<TCPBase*>(client);
	uv_buf_t buf = uv_buf_init((char *)data, len); // TODO: copy data?
	int r = uv_write(&c->_writeReq, (uv_stream_t*)&c->socket, &buf, 1, NULL);
	if(r < 0) {
		printf("ERROR: write_tosocket error: %s\n", uv_err_name(uv_last_error(c->socket.loop)));
	}
	//return r;

  //TCPBase* c = static_cast<TCPBase*>(client);
 // c->send(data, len);
  /*
  if(len <= 0) {
    return;
  }
  uv_buf_t uvbuf;
  uvbuf.base = (char*)data;
  uvbuf.len = len;
  int r = uv_write(&c->_writeReq, (uv_stream_t*)&c->socket, &uvbuf, 1, NULL);
  
	traceL("trace") << "write_tosocket_callback: " << len << endl;
  if(r < 0) {
    printf("ERROR: write_tosocket error: %s\n", uv_err_name(uv_last_error(c->socket.loop)));
  }
}
  */
 





	/*
  char client_key_file[1024];
  //sprintf(client_key_file, "%s/%s", dirname(__FILE__), "client-key.pem");
  sprintf(client_key_file, "./%s", "private-key.pem");
 
	traceL("trace") << "BEFORE RUN" << endl;

  // Initialize SSL
  SSL_library_init();
  SSL_load_error_strings();
 
	traceL("trace") << "BEFORE RUN 2" << endl;

  BIO* bio_err = BIO_new_fp(stderr, BIO_NOCLOSE);
	traceL("trace") << "BEFORE RUN 21" << endl;
  this->_sslCtx = SSL_CTX_new(SSLv23_client_method());
	traceL("trace") << "BEFORE RUN 22" << endl;
  int rc = SSL_CTX_use_PrivateKey_file(_sslCtx, client_key_file, SSL_FILETYPE_PEM);
  if(!rc) {
	  assert(0);
    EXIT("Could not load client key file.\n");
  }
	traceL("trace") << "BEFORE RUN 3" << endl;
 
  SSL_CTX_set_options(_sslCtx, SSL_OP_NO_SSLv2); 
  SSL_CTX_set_verify(_sslCtx, SSL_VERIFY_PEER, dummy_ssl_verify_callback); // our callback always returns true, so no validation
  SSL_CTX_set_info_callback(_sslCtx, dummy_ssl_info_callback);  // for dibugging
  SSL_CTX_set_msg_callback(_sslCtx, dummy_ssl_msg_callback);
 
	traceL("trace") << "BEFORE RUN 4" << endl;
	*/

/*
#define WHERE_INFO(ssl, w, flag, msg) { \
    if(w & flag) { \
      printf("\t"); \
      printf(msg); \
      printf(" - %s ", SSL_state_string(ssl)); \
      printf(" - %s ", SSL_state_string_long(ssl)); \
      printf("\n"); \
    }\
 } 
 
// INFO CALLBACK
void dummy_ssl_info_callback(const SSL* ssl, int where, int ret) {
  if(ret == 0) {
    printf("dummy_ssl_info_callback, error occured.\n");
    return;
  }
  WHERE_INFO(ssl, where, SSL_CB_LOOP, "LOOP");
  WHERE_INFO(ssl, where, SSL_CB_EXIT, "EXIT");
  WHERE_INFO(ssl, where, SSL_CB_READ, "READ");
  WHERE_INFO(ssl, where, SSL_CB_WRITE, "WRITE");
  WHERE_INFO(ssl, where, SSL_CB_ALERT, "ALERT");
  WHERE_INFO(ssl, where, SSL_CB_HANDSHAKE_DONE, "HANDSHAKE DONE");
}
 
// MSG CALLBACK
void dummy_ssl_msg_callback(
                            int writep
                            ,int version
                            ,int contentType
                            ,const void* buf
                            ,size_t len
                            ,SSL* ssl
                            ,void *arg
                            ) 
{
  printf("\tMessage callback with length: %zu\n", len);
}
 
// VERIFY
int dummy_ssl_verify_callback(int ok, X509_STORE_CTX* store) {
  return 1;  // We always return 1, so no verification actually
}


void on_read_callback(uv_stream_t* tcp, ssize_t nread, uv_buf_t buf) {
  TCPBase* c = static_cast<TCPBase*>(tcp->data);
  c->_sslAdapter->addIncomingData(buf.base, nread);
  c->_sslAdapter->update();
}

// HANDLE BUFFERS HERE!
uv_buf_t on_alloc_callback(uv_handle_t* con, size_t size) {
  uv_buf_t buf;
  buf.base = (char*)malloc(size);
  buf.len = size;
  return buf;
}
*/


	/*
	traceL("TCPBase", this) << "On connected" << endl;
	if (status) {
		setLastError();
		errorL("TCPBase", this) << "Connect Failed: " << error().message << endl;
		return;
	}
	//else
	//	readStart();

  int r = uv_read_start(handle<uv_stream_t>(), on_alloc_callback, on_read_callback);
  if(r == -1) {
	  assert(0);
    printf("ERROR: uv_read_start error: %s\n", uv_err_name(uv_last_error(loop())));
    //::exit(0);
  }
 
	  char client_key_file[1024];
  //sprintf(client_key_file, "%s/%s", dirname(__FILE__), "client-key.pem");
  sprintf(client_key_file, "./%s", "private-key.pem");
 
	traceL("trace") << "BEFORE RUN" << endl;

  // Initialize SSL
  SSL_library_init();
  SSL_load_error_strings();
 
	traceL("trace") << "BEFORE RUN 2" << endl;

  BIO* bio_err = BIO_new_fp(stderr, BIO_NOCLOSE);
	traceL("trace") << "BEFORE RUN 21" << endl;
  SSL_CTX* _sslCtx = SSL_CTX_new(SSLv23_client_method());
	traceL("trace") << "BEFORE RUN 22" << endl;
  int rc = SSL_CTX_use_PrivateKey_file(_sslCtx, client_key_file, SSL_FILETYPE_PEM);
  if(!rc) {
	  assert(0);
    //EXIT("Could not load client key file.\n");
  }

	

	//if (ssl) {
		 assert(_sslAdapter == NULL);
		 
		//SecureStreamSocketImpl* pImpl = new SecureStreamSocketImpl(static_cast<StreamSocketImpl*>(streamSocket.impl()), SSLManager::instance().defaultClientContext());
		//pImpl->connectSSL();	
		//SSLContext::Ptr pContext = SSLManager::instance().defaultClientContext();	 
		SSL_CTX* _sslCtx = _sslCtx; //pContext->sslContext();
 
		  SSL_CTX_set_options(_sslCtx, SSL_OP_NO_SSLv2); 
		  SSL_CTX_set_verify(_sslCtx, SSL_VERIFY_PEER, dummy_ssl_verify_callback); // our callback always returns true, so no validation
		  SSL_CTX_set_info_callback(_sslCtx, dummy_ssl_info_callback);  // for dibugging
		  SSL_CTX_set_msg_callback(_sslCtx, dummy_ssl_msg_callback);

		SSL* ssl = SSL_new(_sslCtx);
		_sslAdapter = new SSLAdapter();
		
		  const char* http_request_tpl = "" \
			"GET %s HTTP/1.1\r\n"
			"Host: %s\r\n"
			"User-Agent: uv_www_client/0.1\r\n" 
			"Accept: /*\r\n"
			"Connection: close\r\n"
			"\r\n";
 
		  char http_request[1024];
		  sprintf(http_request, http_request_tpl, "/", "1338");
		  _sslAdapter->addOutgoingData(http_request);

		_sslAdapter->init(ssl, write_tosocket_callback, this, read_decrypted_callback, this);
		_sslAdapter->update();

		SSL_set_connect_state(ssl);
		SSL_do_handshake(ssl);
	/*
		
	//}
		string request(
			"GET / HTTP/1.1\r\n"
			"Host: 127.0.0.1\r\n"
			"User-Agent: uv_www_client/0.1\r\n" 
			"Accept: /*\r\n"
			"Connection: close\r\n"
			"\r\n"
			);
		send(request.c_str(), request.length());
			*/


	/*
	TCPBase* this = reinterpret_cast<TCPBase*>(handle->data);

	traceL("debug") << "[TCPBase: " << this << "] On connection: " << status << endl;

	if (status) {
		this->setError(uv_last_error(this->loop()));
		return;
	}

	*/
	
	// TODO: Check ref count or delete