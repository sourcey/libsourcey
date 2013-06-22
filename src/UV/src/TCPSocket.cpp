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


#include "Sourcey/UV/TCPSocket.h"
#include "Sourcey/Logger.h"

#include "Poco/Exception.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/SSLManager.h"


using namespace std;


namespace scy {
namespace UV {


TCPSocket::TCPSocket(uv_loop_t* loop) : 
	Stream(loop)
{
	LogTrace("TCPSocket", this) << "Creating" << endl;
	connect_req.data = this;	

	uv_tcp_t* tcp = new uv_tcp_t;
	setStream((uv_stream_t*)tcp, this);
	uv_tcp_init(_loop, tcp);

	//uv_tcp_t	 socket;	
	//socket.data = this;	
}

	
TCPSocket::~TCPSocket() 
{	
	LogTrace("TCPSocket", this) << "Destroying" << endl;
    //uv_close((uv_handle_t*)&_stream, UV::defaultClose);
	//uv_unref((uv_handle_t*)&_stream);	
	//if (_connection.handle)
	//	uv_close((uv_handle_t*)_connection.handle, afterClose);
	
	// TODO: Ensure socket closure
	//if (initialized())
	//	close();

	LogTrace("TCPSocket", this) << "Destroying: OK" << endl;
}


void TCPSocket::connect(const Net::Address& peerAddress) 
{
	LogTrace("TCPSocket", this) << "Connecting to " << peerAddress.toString() << endl;
	const sockaddr_in* addr = reinterpret_cast<const sockaddr_in*>(peerAddress.addr());
	int r = uv_tcp_connect(&connect_req, (uv_tcp_t*)stream(), *addr, UV::onConnected);
	if (r) {
		uv_err_t err = uv_last_error(_loop);
		setErrno(err);
		throw Poco::Exception(uv_strerror(err)); // TODO: make exception setError option
	}
}


void TCPSocket::close()
{
	LogTrace("TCPSocket", this) << "Closing" << endl;

	// TODO: Do we need to uv_close the connect_req?	
	// Sending shutdown is not disconnecting socket?
	//Stream::shutdown();

	Stream::close();
}


int TCPSocket::send(const char* data, int len) 
{	
	assert(len <= Net::MAX_TCP_PACKET_SIZE);	
	assert(initialized());

	LogTrace("TCPSocket", this) << "Send: " << len << endl;	
	uv_buf_t uvbuf;
	uvbuf.base = (char*)data;
	uvbuf.len = len;
	int r = uv_write(&write_req, stream(), &uvbuf, 1, NULL);
	if (r < 0)
		setErrno(uv_last_error(_loop));
	return  r;
}


void TCPSocket::setNoDelay(bool enable) 
{
	int r = uv_tcp_nodelay((uv_tcp_t*)stream(), enable ? 1 : 0);
	if (r) setErrno(uv_last_error(_loop));
}


void TCPSocket::setKeepAlive(int enable, unsigned int delay) 
{
	int r = uv_tcp_keepalive((uv_tcp_t*)stream(), enable, delay);
	if (r) setErrno(uv_last_error(_loop));
}


#ifdef _WIN32
void TCPSocket::setSimultaneousAccepts(bool enable) 
{
	int r = uv_tcp_simultaneous_accepts((uv_tcp_t*)stream(), enable ? 1 : 0);
	if (r) setErrno(uv_last_error(_loop));
}
#endif


bool TCPSocket::bind(const Net::Address& address) 
{
	LogTrace("TCPSocket", this) << "Binding on " << address.toString() << endl;
	const sockaddr_in* addr = reinterpret_cast<const sockaddr_in*>(address.addr());
	int r = uv_tcp_bind((uv_tcp_t*)stream(), *addr);
	if (r) setErrno(uv_last_error(_loop));
	return r == 0;
}


bool TCPSocket::bind6(const Net::Address& address) 
{
	LogTrace("TCPSocket", this) << "Binding on " << address.toString() << endl;
	const sockaddr_in6* addr = reinterpret_cast<const sockaddr_in6*>(address.addr());
	int r = uv_tcp_bind6((uv_tcp_t*)stream(), *addr);
	if (r) setErrno(uv_last_error(_loop));  
	return r == 0;
}


bool TCPSocket::listen(int backlog) 
{
	LogTrace("TCPSocket", this) << "Listening" << endl;
	int r = uv_listen(stream(), backlog, UV::onAcceptConnection);
	if (r) setErrno(uv_last_error(_loop));  
	return r == 0;
}


void TCPSocket::acceptConnection()
{
	LogTrace("TCPSocket", this) << "Accept Connection" << endl;
	TCPSocket* conn = new TCPSocket(_loop);
	int r = uv_accept(stream(), conn->stream());
	assert(r == 0); // uv_accept should always work
	conn->readStart();
	TCPSocket::Ptr ptr(conn);
	OnAcceptConnection.emit(this, ptr); //, handled, conn
	//bool handled = false;
	//if (!handled)
	//	delete conn;
}


Net::Address TCPSocket::address()
{
	struct sockaddr_storage address;
	int addrlen = sizeof(address);
	int r = uv_tcp_getsockname((uv_tcp_t*)stream(),
								reinterpret_cast<sockaddr*>(&address),
								&addrlen);
	if (r) {
		setErrno(uv_last_error(_loop));
		//throw Exception Null(node_isolate);
		throw Exception("Invalid socket");
	}

	const sockaddr* addr = reinterpret_cast<const sockaddr*>(&address);
	return Net::Address(new sockaddr(*addr), addrlen); // TODO: free ptr?
}


Net::Address TCPSocket::peerAddress()
{
	struct sockaddr_storage address;
	int addrlen = sizeof(address);
	int r = uv_tcp_getpeername((uv_tcp_t*)stream(),
								reinterpret_cast<sockaddr*>(&address),
								&addrlen);

	if (r) {
		setErrno(uv_last_error(_loop));
		throw Exception("Invalid socket");
	}

	const sockaddr* addr = reinterpret_cast<const sockaddr*>(&address);
	return Net::Address(new sockaddr(*addr), addrlen); // TODO: free ptr?
}


//
// Callbacks
//

void TCPSocket::onRead(const char* data, int len) //uv_stream_t* tcp, ssize_t nread, uv_buf_t buf) 
{
	LogTrace("TCPSocket", this) << "On Stream Recv " << len << endl;
	onRecv(data, len);
}


void TCPSocket::onRecv(const char* data, int len)
{
	Log("TCPSocket", this) << "On Recv: " << len << endl;
	OnRecv.emit(this, data, len);
}	


void TCPSocket::onClose()
{
	Log("TCPSocket", this) << "On Close" << endl;
	OnClose.emit(this);
}	


void TCPSocket::onConnected(int status)
{
	// TODO: Call stream startRecv
	/*
	int r = uv_read_start((uv_stream_t*)stream(), UV::defaultAlloc, UV::onRead);
	if (r == -1) {
		setErrno(uv_last_error(_loop));
		assert(0);
	}
	*/
	
	LogTrace("TCPSocket", this) << "On Connected" << endl;
	if (status) {
		setErrno(uv_last_error(_loop));
		LogError("TCPSocket", this) << "Connect Failed: " << errorMessage() << endl;
		return;
	}
	else
		readStart();

	LogTrace("TCPSocket", this) << "On Connected: Emit: " << OnConnected.refCount() << endl;
	OnConnected.emit(this, status);
}


void TCPSocket::onAcceptConnection(uv_stream_t* handle, int status) 
{		
	if (status == 0) {
		LogTrace("TCPSocket", this) << "On Accept Connection" << endl;
		acceptConnection();
	}
	else
		LogError("TCPSocket", this) << "Accept Connection Failed" << endl;
}


} } // namespace scy::UV



//, _sslBuffer(NULL)

	//_stream = (uv_stream_t*)new uv_tcp_t;
	//_stream->data = this;	
	//_connection.handle = NULL;
	//_connection.data = this;
	//int r = uv_tcp_init(loop, (uv_tcp_t*)stream());
	//assert(r == 0); // TODO: throw exc
		
		//throw Exception Null(node_isolate);


	//_sslBuffer->addEncryptedData(data, len);
	//_sslBuffer->update();
	//send("data", 4);
	//assert(0);



	/*
	// Send unencrypted data to the SSL context
	_sslBuffer.addApplicationData(data, len);
	_sslBuffer.update();
	return len;
	*/

	// Send raw data directly

 
	/*
	TCPSocket* c = static_cast<TCPSocket*>(this); //con->data
	if(status == -1) {
	printf("ERROR: on_connect_callback %s\n", uv_err_name(uv_last_error(_loop)));
	//::exit(0);
	}TCPSocket::
	*/
 
	/*
	string http_request(
		"GET / HTTP/1.1\r\n"
	"Host: 127.0.0.1\r\n"
	"User-Agent: uv_www_client/0.1\r\n" 
	"Accept: /*\r\n"
	"Connection: keep-alive\r\n"
	"\r\n");
 
	_sslBuffer.addApplicationData(http_request);
	ssl = SSL_new(_sslCtx);
 
	SSL_set_connect_state(ssl);
	SSL_do_handshake(ssl);
 
	_sslBuffer.init(this, ssl); //write_tosocket_callback, , read_decrypted_callback, this
	_sslBuffer.update();
	*/








	//struct sockaddr_in address = uv_ip4_addr("127.0.0.1", 1337);
	//int r = uv_tcp_connect(&connect_req, (uv_tcp_t*)stream(), *addr, UV::onConnected); //&_connection
	//_connect_req.handle = NULL;
	//write_req.handle = NULL;
	//write_req.data = this;
	//socket.handle = NULL;
  
	/* WES WORKING		
	//TCPSocket* c = static_cast<TCPSocket*>(this);
		printf("ERROR: write_tosocket error: %s\n", uv_err_name(uv_last_error(_loop)));
	_sslBuffer.addApplicationData(data, len);
	//_sslBuffer.addApplicationData("bbbbbbbbbbbbbbbbbbbbbbbb");
	_sslBuffer.update();
	return len;
	Log("trace") << "write_tosocket_callback: " << len << endl;
	*/
	
	/*
	uv_write_t* req = new uv_write_t;
	uv_buf_t buf = uv_buf_init((char *)data, size); // TODO: copy data?
	if (uv_write(req, stream(), &buf, 1, afterWrite)) //_connection.handle
		return -1;
	return size;
	*/

 
 /*
	assert(_sslBuffer);
void on_read_callback(uv_stream_t* tcp, ssize_t nread, uv_buf_t buf) {
  TCPSocket* c = static_cast<TCPSocket*>(tcp->data);
  c->_sslBuffer.addEncryptedData(buf.base, nread);
  c->_sslBuffer.update();
}
*/


/*
void write_tosocket_callback(const char* data, size_t len, void* client) {
	Log("trace") << "write_tosocket_callback: " << len << endl;
	
    TCPSocket* c = static_cast<TCPSocket*>(client);
	uv_buf_t buf = uv_buf_init((char *)data, len); // TODO: copy data?
	int r = uv_write(&c->write_req, (uv_stream_t*)&c->socket, &buf, 1, NULL);
	if(r < 0) {
		printf("ERROR: write_tosocket error: %s\n", uv_err_name(uv_last_error(c->socket.loop)));
	}
	//return r;

  //TCPSocket* c = static_cast<TCPSocket*>(client);
 // c->send(data, len);
  /*
  if(len <= 0) {
    return;
  }
  uv_buf_t uvbuf;
  uvbuf.base = (char*)data;
  uvbuf.len = len;
  int r = uv_write(&c->write_req, (uv_stream_t*)&c->socket, &uvbuf, 1, NULL);
  
	Log("trace") << "write_tosocket_callback: " << len << endl;
  if(r < 0) {
    printf("ERROR: write_tosocket error: %s\n", uv_err_name(uv_last_error(c->socket.loop)));
  }
}
  */
 





	/*
  char client_key_file[1024];
  //sprintf(client_key_file, "%s/%s", dirname(__FILE__), "client-key.pem");
  sprintf(client_key_file, "./%s", "private-key.pem");
 
	Log("trace") << "BEFORE RUN" << endl;

  // Initialize SSL
  SSL_library_init();
  SSL_load_error_strings();
 
	Log("trace") << "BEFORE RUN 2" << endl;

  BIO* bio_err = BIO_new_fp(stderr, BIO_NOCLOSE);
	Log("trace") << "BEFORE RUN 21" << endl;
  this->_sslCtx = SSL_CTX_new(SSLv23_client_method());
	Log("trace") << "BEFORE RUN 22" << endl;
  int rc = SSL_CTX_use_PrivateKey_file(_sslCtx, client_key_file, SSL_FILETYPE_PEM);
  if(!rc) {
	  assert(0);
    EXIT("Could not load client key file.\n");
  }
	Log("trace") << "BEFORE RUN 3" << endl;
 
  SSL_CTX_set_options(_sslCtx, SSL_OP_NO_SSLv2); 
  SSL_CTX_set_verify(_sslCtx, SSL_VERIFY_PEER, dummy_ssl_verify_callback); // our callback always returns true, so no validation
  SSL_CTX_set_info_callback(_sslCtx, dummy_ssl_info_callback);  // for dibugging
  SSL_CTX_set_msg_callback(_sslCtx, dummy_ssl_msg_callback);
 
	Log("trace") << "BEFORE RUN 4" << endl;
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
  TCPSocket* c = static_cast<TCPSocket*>(tcp->data);
  c->_sslBuffer->addEncryptedData(buf.base, nread);
  c->_sslBuffer->update();
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
	LogTrace("TCPSocket", this) << "On Connected" << endl;
	if (status) {
		setErrno(uv_last_error(_loop));
		LogError("TCPSocket", this) << "Connect Failed: " << errorMessage() << endl;
		return;
	}
	//else
	//	readStart();

  int r = uv_read_start(stream(), on_alloc_callback, on_read_callback);
  if(r == -1) {
	  assert(0);
    printf("ERROR: uv_read_start error: %s\n", uv_err_name(uv_last_error(_loop)));
    //::exit(0);
  }
 
	  char client_key_file[1024];
  //sprintf(client_key_file, "%s/%s", dirname(__FILE__), "client-key.pem");
  sprintf(client_key_file, "./%s", "private-key.pem");
 
	Log("trace") << "BEFORE RUN" << endl;

  // Initialize SSL
  SSL_library_init();
  SSL_load_error_strings();
 
	Log("trace") << "BEFORE RUN 2" << endl;

  BIO* bio_err = BIO_new_fp(stderr, BIO_NOCLOSE);
	Log("trace") << "BEFORE RUN 21" << endl;
  SSL_CTX* _sslCtx = SSL_CTX_new(SSLv23_client_method());
	Log("trace") << "BEFORE RUN 22" << endl;
  int rc = SSL_CTX_use_PrivateKey_file(_sslCtx, client_key_file, SSL_FILETYPE_PEM);
  if(!rc) {
	  assert(0);
    //EXIT("Could not load client key file.\n");
  }

	

	//if (ssl) {
		 assert(_sslBuffer == NULL);
		 
		//SecureStreamSocketImpl* pImpl = new SecureStreamSocketImpl(static_cast<StreamSocketImpl*>(streamSocket.impl()), SSLManager::instance().defaultClientContext());
		//pImpl->connectSSL();	
		//Poco::Net::Context::Ptr pContext = Poco::Net::SSLManager::instance().defaultClientContext();	 
		SSL_CTX* _sslCtx = _sslCtx; //pContext->sslContext();
 
		  SSL_CTX_set_options(_sslCtx, SSL_OP_NO_SSLv2); 
		  SSL_CTX_set_verify(_sslCtx, SSL_VERIFY_PEER, dummy_ssl_verify_callback); // our callback always returns true, so no validation
		  SSL_CTX_set_info_callback(_sslCtx, dummy_ssl_info_callback);  // for dibugging
		  SSL_CTX_set_msg_callback(_sslCtx, dummy_ssl_msg_callback);

		SSL* ssl = SSL_new(_sslCtx);
		_sslBuffer = new SSLBuffer();
		
		  const char* http_request_tpl = "" \
			"GET %s HTTP/1.1\r\n"
			"Host: %s\r\n"
			"User-Agent: uv_www_client/0.1\r\n" 
			"Accept: /*\r\n"
			"Connection: close\r\n"
			"\r\n";
 
		  char http_request[1024];
		  sprintf(http_request, http_request_tpl, "/", "1338");
		  _sslBuffer->addApplicationData(http_request);

		_sslBuffer->init(ssl, write_tosocket_callback, this, read_decrypted_callback, this);
		_sslBuffer->update();

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
		send(request.data(), request.length());
			*/


	/*
	TCPSocket* this = reinterpret_cast<TCPSocket*>(handle->data);

	LogTrace("debug") << "[TCPSocket: " << this << "] On Connection: " << status << endl;

	if (status) {
		this->setErrno(uv_last_error(this->_loop));
		return;
	}

	*/
	
	// TODO: Check ref count or delete