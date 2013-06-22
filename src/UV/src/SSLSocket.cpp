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


#include "Sourcey/UV/SSLSocket.h"
#include "Sourcey/Logger.h"

#include "Poco/Exception.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/SSLManager.h"


using namespace std;


namespace scy {
namespace UV {


SSLSocket::SSLSocket(uv_loop_t* loop) : 
	TCPSocket(loop), 
	_context(NULL), 
	_session(NULL), 
	_sslBuffer(this)
{
	LogTrace("SSLSocket", this) << "Creating" << endl;
	connect_req.data = this;
	//socket.data = this;
}


SSLSocket::SSLSocket(Poco::Net::Context::Ptr context, uv_loop_t* loop) : 
	TCPSocket(loop), 
	_context(context), 
	_session(NULL), 
	_sslBuffer(this)
{
	Poco::Net::Session::Ptr _session;
	LogTrace("SSLSocket", this) << "Creating" << endl;
	connect_req.data = this;
	//socket.data = this;
}
	

SSLSocket::SSLSocket(Poco::Net::Context::Ptr context, Poco::Net::Session::Ptr session, uv_loop_t* loop) : 
	TCPSocket(loop), 
	_context(context), 
	_session(session), 
	_sslBuffer(this)
{
	LogTrace("SSLSocket", this) << "Creating" << endl;
	connect_req.data = this;
	//socket.data = this;
}

	
SSLSocket::~SSLSocket() 
{	
	LogTrace("SSLSocket", this) << "Destroying" << endl;
    //uv_close((uv_handle_t*)&_stream, UV::defaultClose);
	//uv_unref((uv_handle_t*)&_stream);	
	//if (_connection.handle)
	//	uv_close((uv_handle_t*)_connection.handle, afterClose);
		
	// TODO: cleanup via TCPSocket
	LogTrace("SSLSocket", this) << "Destroying: OK" << endl;
}


void SSLSocket::connect(const Net::Address& peerAddress) 
{
	if (!_context) 
		throw Exception("Cannot connect without SSL context");

	LogTrace("SSLSocket", this) << "Connecting to " << peerAddress << endl;
	const sockaddr_in* addr = reinterpret_cast<const sockaddr_in*>(peerAddress.addr());
	int r = uv_tcp_connect(&connect_req, (uv_tcp_t*)stream(), *addr, UV::onConnected);
	if (r) {
		uv_err_t err = uv_last_error(_loop);
		setErrno(err);
		throw Poco::Exception(uv_strerror(err)); // TODO: make exception setError option
	}
}


int SSLSocket::available() const
{
	assert(initialized());
	return _sslBuffer.available();
}


void SSLSocket::close()
{
	LogTrace("SSLSocket", this) << "Closing" << endl;
	try
	{
		// Try to gracefully shutdown the SSL connection
		_sslBuffer.shutdown();
	}
	catch (...)
	{
	}
	TCPSocket::close();
}


int SSLSocket::send(const char* data, int len) 
{	
	assert(len <= Net::MAX_TCP_PACKET_SIZE);	
	assert(initialized());
	LogTrace("SSLSocket", this) << "Send: " << len << endl;
	
	// Send unencrypted data to the SSL context
	_sslBuffer.addApplicationData(data, len);
	_sslBuffer.update();
	return len;
}


Poco::Net::Session::Ptr SSLSocket::currentSession()
{
	if (_sslBuffer._ssl)
	{
		SSL_SESSION* session = SSL_get1_session(_sslBuffer._ssl);
		if (session)
		{
			if (_session && session == _session->sslSession())
			{
				SSL_SESSION_free(session);
				return _session;
			}
			else return new Poco::Net::Session(session);
		}
	}
	return 0;
}

	
void SSLSocket::useSession(Poco::Net::Session::Ptr pSession)
{
	_session = pSession;
}


bool SSLSocket::sessionWasReused()
{
	if (_sslBuffer._ssl)
		return SSL_session_reused(_sslBuffer._ssl) != 0;
	else
		return false;
}


//
// Callbacks
// 

void SSLSocket::onRead(const char* data, int len) //uv_stream_t* tcp, ssize_t nread, uv_buf_t buf) 
{
	LogTrace("SSLSocket", this) << "On SSL Stream Recv " << len << endl;

	// SSL encrypted data is sent to the SSL conetext
	_sslBuffer.addEncryptedData(data, len);
	_sslBuffer.update();
}


void SSLSocket::onRecv(const char* data, int len) //uv_stream_t* tcp, ssize_t nread, uv_buf_t buf) 
{
	Log("TCPSocket", this) << "On Recv: " << len << endl;
	OnRecv.emit(this, data, len);
}


void SSLSocket::onConnected(int status)
{
	/*
	// TODO: Call stream startRecv
	int r = uv_read_start(stream(), UV::defaultAlloc, UV::onRead);
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
 
	SSL* ssl = SSL_new(_context->sslContext());

	// TODO: Automatic SSL session handling
	if (_session)
		SSL_set_session(ssl, _session->sslSession());
 
	SSL_set_connect_state(ssl);
	SSL_do_handshake(ssl);
	
	//Poco::Net::Session::Ptr _session;
 
	_sslBuffer.init(ssl); //write_tosocket_callback, , read_decrypted_callback, this
	_sslBuffer.update();

	LogTrace("SSLSocket", this) << "On Connected: Emit: " << OnConnected.refCount() << endl;
	OnConnected.emit(this, status);
}


} } // namespace scy::UV



	// TODO: ...

	//if (_stream->data)
	//	uv_close((uv_tcp_t*)&socket, afterClose);
	//if (_connection.handle)
	//	uv_close((uv_handle_t*)_connection.handle, afterClose);
	//TCPSocket::shutdown();

	//_sslBuffer->addEncryptedData(data, len);
	//_sslBuffer->update();
	//send("data", 4);
	//assert(0);
	// Send raw data directly
	/*


void SSLSocket::onRecv(const char* data, int len)
{
	Log("SSLSocket", this) << "On Recv: " << len << endl;
	OnRecv.emit(this, data, len);
}


	uv_buf_t uvbuf;
	uvbuf.base = (char*)data;
	uvbuf.len = len;
	int r = uv_write(&write_req, (uv_stream_t*)&socket, &uvbuf, 1, NULL);
	if (r < 0)
		setErrno(uv_last_error(_loop));
	return  r;
	*/
	/*
	TCPSocket* c = static_cast<TCPSocket*>(this); //con->data
	if(status == -1) {
	printf("ERROR: on_connect_callback %s\n", uv_err_name(uv_last_error(_loop)));
	//::exit(0);
	}SSLSocket::
	*/
 

	//_stream = (uv_stream_t*)new uv_tcp_t;
	//_stream->data = this;	
	//_connection.handle = NULL;
	//_connection.data = this;
	//int r = uv_tcp_init(loop, (uv_tcp_t*)&socket);
	//assert(r == 0); // TODO: throw exc	
	//uv_tcp_init(_loop, &socket);
	/*


void SSLSocket::onAcceptConnection(uv_stream_t* handle, int status) 
{		
	if (status == 0) {
		LogTrace("SSLSocket", this) << "On Connection" << endl;
		acceptConnection();
	}
	else
		LogError("SSLSocket", this) << "Connection Failed" << endl;
}

	string http_request(
		"GET / HTTP/1.1\r\n"
	"Host: 127.0.0.1\r\n"
	"User-Agent: uv_www_client/0.1\r\n" 
	"Accept: /*\r\n"
	"Connection: keep-alive\r\n"
	"\r\n");
 
	_sslBuffer.addApplicationData(http_request);
	*/
		

	//struct sockaddr_in address = uv_ip4_addr("127.0.0.1", 1337);
	//int r = uv_tcp_connect(&connect_req, (uv_tcp_t*)&socket, *addr, UV::onConnected); //&_connection
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
	if (uv_write(req, (uv_stream_t*)_stream, &buf, 1, afterWrite)) //_connection.handle
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
	LogTrace("SSLSocket", this) << "On Connected" << endl;
	if (status) {
		setErrno(uv_last_error(_loop));
		LogError("SSLSocket", this) << "Connect Failed: " << errorMessage() << endl;
		return;
	}
	//else
	//	readStart();

  int r = uv_read_start((uv_stream_t*)_stream, on_alloc_callback, on_read_callback);
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