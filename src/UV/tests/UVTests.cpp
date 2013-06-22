

#include "Sourcey/UV/UVPP.h"
#include "Sourcey/UV/UDPSocket.h"
#include "Sourcey/UV/TCPSocket.h"
#include "Sourcey/UV/SSLSocket.h"


#include "Sourcey/UV/Timer.h"
#include "Sourcey/Base.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Net/Address.h"
#include "Sourcey/Net/SSLSocket.h"

#include "Poco/Stopwatch.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/DNS.h"
#include "Poco/Mutex.h"
#include "assert.h"

#include "Poco/Net/Session.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/KeyConsoleHandler.h"
#include "Poco/Net/ConsoleCertificateHandler.h"

#include "UVEchoServer.h"

#include <iterator>
/*
#include "Sourcey/UV/TCPClientSocket.h"
//#include "Sourcey/Net/TCPSocket.h"
//#include "Sourcey/Timer.h"
//#include "Sourcey/Net/Reactor.h"
//#include "SourceyReactorEchoServer.h"
//#include "SourceyAsyncReactorEchoServer.h"
//#include "PocoReactorEchoServer.h"
//#include "PocoEchoServer.h"
//#include "UVEchoServer.h"
//#include "bak/AsyncLoop.h"


#include "Timer.h"
#include "IP.h"
#include "Poco/RefCountedObject.h"
#include "Poco/Exception.h"
#include "Net/NetException.h"
#include "Net/SocketAddress.h"
#include "Timer.h"
#include "UDPSocket.h"
#include "NetException.h"

#include "Sourcey/Net/Address.h"


#include "Sourcey/Timer.h"
#include "IOStream.h"
#include "AsyncLoop.h"
#include "uv.h"
#include "Sourcey/Timer.h"

#include "Sourcey/Net/UDPPacketSocket.h"
#include "Sourcey/Net/TCPPacketSocket.h"
#include "Sourcey/Net/TCPServer.h"
*/


#include <Windows.h>
#include <Iphlpapi.h>

using namespace std;
using namespace scy;
using namespace scy::Net;

using Poco::InvalidArgumentException;


/*
// Detect Memory Leaks
#if defined(_DEBUG) && defined(_WIN32)
#include "MemLeakDetect/MemLeakDetect.h"
#include "MemLeakDetect/MemLeakDetect.cpp"
CMemLeakDetect memLeakDetect;
#endif
*/


namespace scy {
	
	
class Tests
{
public:
	struct Result {
		int numSuccess;
		string name;
		Poco::Stopwatch sw;

		void reset() {
			numSuccess = 0;
			sw.reset();
		}
	};
	
	static Result Benchmark;

	Tests()
	{	
		
		//loop = uv_default_loop();

		//UV::UVEchoServer<UV::TCPSocket> tcpServer(1337);
		//UVEchoServer<UV::SSLSocket> sslServer(1338);
		//tcpServer.run();
		//sslServer.run();
		//testThread();
		//testAddress();
		//runSocketConenctionTest();
		//runPocoSSLSocketTest();
		runTCPSocketTest();
		/*
		runSSLSocketTest();
		runUDPSocketTest();
		runTimerTest();
		runDNSResolverTest();
		*/
	}
	
	// ============================================================================
	//
	// SSL Socket Test
	//
	// ============================================================================
	void runSSLSocketTest() 
	{
		Log("trace") << "[Tests:" << this << "] runSSLSocketTest" << endl;
			
		Net::Address addr("127.0.0.1", 1338);
		
		/*
		UV::TCPSocket serverSock;		
		serverSock.OnAcceptConnection += delegate(this, &Tests::onTCPSocketServerAccept);
		serverSock.OnWrite += delegate(this, &Tests::onTCPSocketServerSend);
		serverSock.OnRead += delegate(this, &Tests::onTCPSocketServerRecv);
		serverSock.bind(addr);
		serverSock.listen();
		*/
		
		/*
		//UV::TCPSocket clientSock;
		UV::SSLContext clientSock(pContext);
		clientSock.OnWrite += delegate(this, &Tests::onSSLSocketClientSend);
		clientSock.OnRecv += delegate(this, &Tests::onSSLSocketClientRecv);
		clientSock.OnConnected += delegate(this, &Tests::onSSLSocketClientConnected);
		clientSock.connect(addr);	// , fals
		*/
		Poco::Net::Context::Ptr pContext = Poco::Net::SSLManager::instance().defaultClientContext();	
		
		UV::SSLSocket clientSock(pContext);
		clientSock.OnWrite += delegate(this, &Tests::onSSLSocketClientSend);
		clientSock.OnRecv += delegate(this, &Tests::onSSLSocketClientRecv);
		clientSock.OnConnected += delegate(this, &Tests::onSSLSocketClientConnected);
		clientSock.connect(addr);	// , false, true
		
		RunDefaultLoop;
		Log("trace") << "[Tests:" << this << "] runTCPSocketTest: END" << endl;
	}
	
	/*
	// ----------------------------------------------------------------------------
	//		
	void onSSLSocketServerAccept(void* sender, UV::SSLContext* conn)
	{	
		Log("debug") << "[Tests:" << this << "] onSSLSocketServerAccept: " << sender << std::endl;
		conn->OnRead += delegate(this, &Tests::onSSLSocketServerRecv);
	}

	void onSSLSocketServerSend(void* sender, const char* data, int len)
	{
		Log("trace") << "[Tests:" << this << "] SSLSocket Server Send: " << string(data, len) << endl;
	}
	
	void onSSLSocketServerRecv(void* sender, const char* data, int len)
	{
		Log("trace") << "[Tests:" << this << "] SSLSocket Server Recv: " << string(data, len) << endl;
	}
	*/
	
	// ----------------------------------------------------------------------------
	//	
	void onSSLSocketClientSend(void* sender, const char* data, int len)
	{
		Log("trace") << "[Tests:" << this << "] SSLSocket Client Send: " << len << ": " << string(data, len) << endl;
	}
	
	void onSSLSocketClientRecv(void* sender, const char* data, int len)
	{
		Log("trace") << "[Tests:" << this << "] SSLSocket Client Recv: " << len << endl;	
		std::copy(data, data+len, std::ostream_iterator<char>(std::cout, ""));
		//UV::SSLSocket* socket = reinterpret_cast<UV::SSLSocket*>(sender);
		StopDefaultLoop;
	}
	
	void onSSLSocketClientConnected(void* sender, int val)
	{
		Log("trace") << "[Tests:" << this << "] SSLSocket Client Connected: " << val << endl;

		/*
		// Send  HTTP requesat
		UV::SSLSocket* socket = reinterpret_cast<UV::SSLSocket*>(sender);	
		string request(
			"GET / HTTP/1.1\r\n"
			"Host: 127.0.0.1\r\n"
			"User-Agent: uv_www_client/0.1\r\n" 
			"Connection: keep-alive\r\n"
			"\r\n");
		socket->send(request.data(), request.length());
		*/
	}

	
	
	// ============================================================================
	//
	// Poco SSL Socket Test
	//
	// ============================================================================
	void runPocoSSLSocketTest() 
	{
		Log("trace") << "[Tests:" << this << "] runPocoSSLSocketTest" << endl;
			
		/*
		// Init SSL Context
		Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> ptrCert;
		Poco::Net::Context::Ptr ptrContext = new Poco::Net::Context(
			Poco::Net::Context::CLIENT_USE, "", "", "", Poco::Net::Context::VERIFY_NONE, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");		
		Poco::Net::SSLManager::instance().initializeClient(0, ptrCert, ptrContext);
		*/

		Net::Address addr("127.0.0.1", 1338);
		
		Net::SSLSocket sock(Net::Reactor::getDefault());
		sock.connect(addr);

		system("pause");
	}
	
	// ============================================================================
	//
	// TCP Socket Test
	//
	// ============================================================================
	void runTCPSocketTest() 
	{
		Log("trace") << "[Tests:" << this << "] runTCPSocketTest" << endl;
			
		Net::Address addr("127.0.0.1", 1337);

		UV::TCPSocket::Ptr clientSock;
		clientSock.OnWrite += delegate(this, &Tests::onTCPSocketClientSend);
		clientSock.OnRead += delegate(this, &Tests::onTCPSocketClientRecv);
		clientSock.OnConnected += delegate(this, &Tests::onTCPSocketClientConnected);
		clientSock.OnClose += delegate(this, &Tests::onTCPSocketClientClosed);
		clientSock.connect(addr);	

		RunDefaultLoop;
		Log("trace") << "[Tests:" << this << "] runTCPSocketTest: END" << endl;
	}
	
	// ----------------------------------------------------------------------------
	//	
	void onTCPSocketClientSend(void* sender, const char* data, int len)
	{
		Log("trace") << "[Tests:" << this << "] TCPSocket Client Send: " << string(data, len) << endl;
	}
	
	void onTCPSocketClientRecv(void* sender, const char* data, int len)
	{
		Log("trace") << "[Tests:" << this << "] TCPSocket Client Recv: " << string(data, len) << endl;
	}
	
	void onTCPSocketClientConnected(void* sender, int val)
	{
		Log("trace") << "[Tests:" << this << "] TCPSocket Client Connected: " << val << endl;
		UV::TCPSocket* socket = reinterpret_cast<UV::TCPSocket*>(sender);	

		socket->send("somedata", 8);
		socket->send("somedata", 8);
		socket->send("somedata", 8);

		// Close the connection
		socket->close();
	}
	
	void onTCPSocketClientClosed(void* sender, int err)
	{
		Log("trace") << "[Tests:" << this << "] TCPSocket Client Close: " << err << endl;
		UV::TCPSocket* socket = reinterpret_cast<UV::TCPSocket*>(sender);	

		// Release the connection, this should destroy 
		// the socket and unref the main loop.
		//socket->release();

		StopDefaultLoop;
	}

	
	// ============================================================================
	//
	// UDP Socket Test
	//
	// ============================================================================
	const static int numUDPPackets = 3;

	void runUDPSocketTest() 
	{
		Log("trace") << "[Tests:" << this << "] runUDPSocketTest" << endl;
			
		Net::Address addr("127.0.0.1", 1337);

		/*
		UV::UDPSocket serverSock;
		serverSock.OnSend += delegate(this, &Tests::onUDPSocketServerSend);
		serverSock.OnRecv += delegate(this, &Tests::onUDPSocketServerRecv);
		serverSock.bind(addr);
		*/

		UV::UDPSocket clientSock;
		clientSock.OnSend += delegate(this, &Tests::onUDPSocketClientSend);
		clientSock.OnRecv += delegate(this, &Tests::onUDPSocketClientRecv);
		
		clientSock.send("somedata", 8, addr);
		clientSock.send("somedata", 8, addr);
		clientSock.send("somedata", 8, addr);
		
		Benchmark.reset();
		Log("trace") << "[Tests:" << this << "] runUDPSocketTest 0" << endl;
		RunDefaultLoop;
		Log("trace") << "[Tests:" << this << "] runUDPSocketTest 1" << endl;
	}
	
	
	// ----------------------------------------------------------------------------
	//	
	void onUDPSocketClientSend(void* sender, const char* data, int len, const Net::Address& peerAddr)
	{
		Log("trace") << "[Tests:" << this << "] UDPSocket Client Send: " << string(data, len) << ": " << peerAddr << endl;
	}
	
	void onUDPSocketClientRecv(void* sender, const char* data, int len, const Net::Address& peerAddr)
	{
		Log("trace") << "[Tests:" << this << "] UDPSocket Client Recv: " << string(data, len) << ": " << peerAddr << endl;

		Benchmark.numSuccess++;
		if (Benchmark.numSuccess == numUDPPackets)
			return;
		StopDefaultLoop;
	}
	

	// ----------------------------------------------------------------------------
	//	
	void onUDPSocketServerSend(void* sender, const char* data, int len, const Net::Address& peerAddr)
	{
		Log("trace") << "[Tests:" << this << "] UDPSocket Server Send: " << string(data, len) << ": " << peerAddr << endl;
	}
	
	void onUDPSocketServerRecv(void* sender, const char* data, int len, const Net::Address& peerAddr)
	{
		Log("trace") << "[Tests:" << this << "] UDPSocket Server Recv: " << string(data, len) << ": " << peerAddr << endl;		
		UV::UDPSocket* socket = reinterpret_cast<UV::UDPSocket*>(sender);	
		socket->send(data, len, peerAddr);
	}

	// ============================================================================
	//
	// Timer Test
	//
	// ============================================================================
	const static int numTimerTicks = 5;

	void runTimerTest() 
	{
		UV::Timer timer;
		timer.OnTimeout += delegate(this, &Tests::onOnTimerTimeout);
		timer.start(100, 100);
		
		Benchmark.reset();
		Log("trace") << "[Tests:" << this << "] runTimerTest 0" << endl;
		RunDefaultLoop;
		Log("trace") << "[Tests:" << this << "] runTimerTest 1" << endl;
	}

	void onOnTimerTimeout(void* sender, Int64 count)
	{
		UV::Timer* timer = static_cast<UV::Timer*>(sender);
		Log("trace") << "[Tests:" << this << "] Timer: " << count << endl;

		if (count == numTimerTicks) {
			timer->stop(); // event loop will be released
			StopDefaultLoop;
		}
	}
};


} // namespace scy

/*
*/

int main(int argc, char** argv) 
{	
	Logger::instance().add(new ConsoleChannel("debug", TraceLevel));
	//Logger::instance().add(new FileChannel("debug", "", "log", NULL, 12 * 3600, TraceLevel)); //d:/"%H:%M:%S"
		
	
	// Init SSL Context
	Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> ptrCert;
	Poco::Net::Context::Ptr ptrContext = new Poco::Net::Context(
		Poco::Net::Context::CLIENT_USE, "", "", "", Poco::Net::Context::VERIFY_NONE, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");		
	Poco::Net::SSLManager::instance().initializeClient(0, ptrCert, ptrContext);

	{
		scy::Tests app;
	}
	
	system("pause");
	Poco::Net::SSLManager::instance().shutdown();
	Logger::uninitialize();
	return 0;
}





	/*

#include <iostream>
//#include <libgen.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <uv.h>
#include <vector>
#include <iterator>
#include <algorithm>
//#include "SSLBuffer.h"
 
#define EXIT(msg) { printf(msg); } // //::exit(0);
 
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
 
// LIBUV
struct Client {
  void addAppData(const std::string str) {
    std::copy(str.begin(), str.end(), std::back_inserter(buffer_out));
  }
  uv_loop_t* loop;
  uv_tcp_t socket;
  uv_write_t write_req;
  uv_connect_t connect_req;
  char host[1024];
  char port[1024];
  char page[1024];
  std::vector<char> buffer_in; // app data in
  std::vector<char> buffer_out; // app data out
  SSL_CTX* ssl_ctx;
  UV::SSLBuffer ssl_buffer;
  SSL* ssl;
};
 
void write_to_socket_callback(const char* data, size_t len, void* client) {
  if(len <= 0) {
    return;
  }
  Client* c = static_cast<Client*>(client);
  uv_buf_t uvbuf;
  uvbuf.base = (char*)data;
  uvbuf.len = len;
  int r = uv_write(&c->write_req, (uv_stream_t*)&c->socket, &uvbuf, 1, NULL);
  
	Log("trace") << "write_to_socket_callback: " << len << endl;
  if(r < 0) {
    printf("ERROR: write_to_socket error: %s\n", uv_err_name(uv_last_error(c->socket.loop)));
  }
}
 
void read_decrypted_callback(const char* data, size_t len, void* client) {
  std::copy(data, data+len, std::ostream_iterator<char>(std::cout, ""));
}
 
// HANDLE BUFFERS HERE!
uv_buf_t on_alloc_callback(uv_handle_t* con, size_t size) {
  uv_buf_t buf;
  buf.base = (char*)malloc(size);
  buf.len = size;
  return buf;
}
 
void on_read_callback(uv_stream_t* tcp, ssize_t nread, uv_buf_t buf) {
  Client* c = static_cast<Client*>(tcp->data);
  c->ssl_buffer.addEncryptedData(buf.base, nread);
  c->ssl_buffer.update();
}
 
void on_connect_callback(uv_connect_t* con, int status) {
  Client* c = static_cast<Client*>(con->data);
  if(status == -1) {
    printf("ERROR: on_connect_callback %s\n", uv_err_name(uv_last_error(c->loop)));
    //::exit(0);
  }
 
  int r = uv_read_start((uv_stream_t*)&c->socket, on_alloc_callback, on_read_callback);
  if(r == -1) {
	  assert(0);
    printf("ERROR: uv_read_start error: %s\n", uv_err_name(uv_last_error(c->loop)));
    //::exit(0);
  }
 
  const char* http_request_tpl = "" \
    "GET %s HTTP/1.1\r\n"
    "Host: %s\r\n"
    "User-Agent: uv_www_client/0.1\r\n" 
    "Accept: /*\r\n"
    "Connection: close\r\n"
    "\r\n";
 
  char http_request[1024];
  sprintf(http_request, http_request_tpl, c->page, c->host);
  c->ssl_buffer.addApplicationData(http_request);
  c->ssl = SSL_new(c->ssl_ctx);
 
  SSL_set_connect_state(c->ssl);
  SSL_do_handshake(c->ssl);
 
  c->ssl_buffer.init(c->ssl, write_to_socket_callback, c, read_decrypted_callback, c);
  c->ssl_buffer.update();
}
 
void on_resolved_callback(uv_getaddrinfo_t* resolver, int status, struct addrinfo * res) {
  Client* c = static_cast<Client*>(resolver->data);
  if(status == -1) {
	  assert(0);
    printf("ERROR: getaddrinfo callback error: %s\n", uv_err_name(uv_last_error(c->loop)));
    //::exit(0);
  }
  
  char addr[17] = {'\0'};
  uv_ip4_name((struct sockaddr_in*) res->ai_addr, addr, 16);
  printf("Found host:  %s\n", addr);
 
  uv_tcp_init(c->loop, &c->socket);
  uv_tcp_connect(&c->connect_req, &c->socket, *(sockaddr_in*)res->ai_addr, on_connect_callback);
  uv_freeaddrinfo(res);
}
 
int main() {
	Logger::instance().add(new ConsoleChannel("debug", TraceLevel));
	Log("trace") << "BEFORE RUN" << endl;

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
  SSL_CTX* ssl_ctx = SSL_CTX_new(SSLv23_client_method());
	Log("trace") << "BEFORE RUN 22" << endl;
  int rc = SSL_CTX_use_PrivateKey_file(ssl_ctx, client_key_file, SSL_FILETYPE_PEM);
  if(!rc) {
	  assert(0);
    EXIT("Could not load client key file.\n");
  }
	Log("trace") << "BEFORE RUN 3" << endl;
 
  SSL_CTX_set_options(ssl_ctx, SSL_OP_NO_SSLv2); 
  SSL_CTX_set_verify(ssl_ctx, SSL_VERIFY_PEER, dummy_ssl_verify_callback); // our callback always returns true, so no validation
  SSL_CTX_set_info_callback(ssl_ctx, dummy_ssl_info_callback);  // for dibugging
  SSL_CTX_set_msg_callback(ssl_ctx, dummy_ssl_msg_callback);
 
	Log("trace") << "BEFORE RUN 4" << endl;

  uv_loop_t* loop = uv_loop_new();
 
  // Client context
  Client c;
  c.loop = loop;
  c.connect_req.data = &c;
  c.socket.data = &c;
  c.ssl = NULL;
  c.ssl_ctx = ssl_ctx;
 
  sprintf(c.host, "%s", "localhost");
  sprintf(c.port, "%s", "1338");
  sprintf(c.page, "%s", "/chunked.php");
 
  // Resolve host
  struct addrinfo hints;
  hints.ai_family = PF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = 0;
  uv_getaddrinfo_t resolver;
  resolver.data = &c;
  int r = uv_getaddrinfo(loop, &resolver, on_resolved_callback, c.host, c.port, &hints);
	Log("trace") << "BEFORE RUN" << endl;
  uv_run(loop, UV_RUN_DEFAULT);
	Log("trace") << "aFTER RUN" << endl;
  return 0;
}




	

	// ============================================================================
	//
	// Thread Test
	//
	// ============================================================================	

	
	// ============================================================================
	//
	// Thread Test
	//
	// ============================================================================	
	
class BasicReactorSignal: public SignalBase<Reactor&, const Poco::Net::Socket&, void*, void*, ReactorDelegate>
{
public:	
	BasicReactorSignal() {};
	virtual ~BasicReactorSignal() {}

	unsigned events; // pending events
};void*,
	//
	//ReactorSignal ReactorEventSignal;	
	//ReactorSignal BasicReactorSignal;
	

	void onClientSocketConnected(ReactorEvent&)
	{
		Log("trace") << "[Tests:" << this << "] onClientSocketConnected" << endl;
	}

	void onBasicReactorSignal() //void*
	{
		Log("trace") << "[Tests:" << this << "] onBasicReactorSignal" << endl;
	}

	void testThread()
	{	
		//assert(reactorCallback(this, &Tests::onClientSocketConnected) == reactorCallback(this, &Tests::onClientSocketConnected));
		//Reactor reactor; //(333);
		//reactor.attach();
		//ReactorEventSignal += reactorCallback(this, &Tests::onClientSocketConnected);
		//BasicReactorSignal += reactorCallback(this, &Tests::onBasicReactorSignal);
		//BasicReactorSignal += delegate(this, &Tests::onBasicReactorSignal);
	}




		//app.run(argc, argv);
		//scy::TURN::RelayServer app;
		//app.run(); //argc, argv



	
	
	// ============================================================================
	//
	// Socket Conenction Test
	//
	// ============================================================================
	
	//static int numRequestsSent;
	//static int numRequestsReceived;

	//ReactorSignal ReactorEventSignal;	
	//ReactorSignal BasicReactorSignal;
	
		/*	
	//AsyncReactor areactor;
	Reactor reactor;
	Poco::Net::StreamSocket sock;

	void onClientSocketConnected()
	{
		reactor.detach(sock, reactorCallback(this, &Tests::onClientSocketConnected, SocketWritable));
	}


	void runSocketConenctionTest() 
	{
		Log("trace") << "[Tests:" << this << "] runPacketSignalReceivers" << endl;


		numInstances = 60;
		numIterations = 1;
		
		short port = 1333;
		Net::TCPSocket* socket = NULL;
		Poco::Runnable* server = NULL;
		vector<BenchmarkResult*> results;
		
				
		reactor.attach(sock, reactorCallback(this, &Tests::onClientSocketConnected, SocketWritable));
		//reactor.attach(sock, reactorCallback(this, &Tests::onClientSocketConnected, SocketWritable));
		//reactor.attach(sock, reactorCallback(this, &Tests::onClientSocketConnected, SocketWritable));
			
		sock.connect(Poco::Net::SocketAddress("google.com", 80));
		

		reactor.attach();
		_reactor.attach(_impl, Poco::Observer<TCPSocket, ReadableNotification>(*this, &TCPSocket::onReadable));
		_reactor.attach(_impl, Poco::Observer<TCPSocket, WritableNotification>(*this, &TCPSocket::onWritable));
		_reactor.attach(_impl, Poco::Observer<TCPSocket, ErrorNotification>(*this, &TCPSocket::onError));	

		system("pause");
		return;

		for (unsigned n = 0; n < 1; n++)
		{	

			if (socket)
				delete socket;
			if (server)
				delete server;
		
			benchmark = new BenchmarkResult;
			benchmark->numSuccess = 0;
			results.push_back(benchmark);

			Timer::getDefault().start(TimerCallback<Tests>(this, &Tests::onTimeout, 8000));
			
			//server = new SourceyAsyncReactorEchoServer(port, areactor);
			//benchmark->name = "SourceyReactorEchoServer";
			
			server = new SourceyReactorEchoServer(port, reactor);
			benchmark->name = "SourceyReactorEchoServer";
			// raise server
			//if (n == 0) {
			//server = new PocoReactorEchoServer(port);
			//benchmark->name = "PocoReactorEchoServer";
			//} else if (n == 1) {
			//server = new PocoEchoServer(port);
			//benchmark->name = "PocoEchoServer";
			//} else if (n == 2) {
			//server = new UVEchoServer(port); // 1562500 microsecs
			//benchmark->name = "UVEchoServer";
			//}			
			//
				
			//AsyncLoop run;

			Log("debug") << "[Tests:" << this << "] connecting... " << endl;	
		
			for (unsigned n = 0; n < numInstances; n++)
			{	
				//numRequests
				socket = new Net::TCPSocket(reactor);
				socket->StateChanged += delegate(this, &Tests::onClientSocketStateChanged);				
				socket->DataReceived += delegate(this, &Tests::onClientSocketResponseReceived);
				//socket->StateChanged += delegate(this, &Tests::onClientSocketStateChanged);
				//reactor.attach(socket->impl(), reactorCallback(this, &Tests::onClientSocketConnected, SocketWritable));
				//socket->OnConnected += delegate(this, &Tests::onClientSocketConnected);
				//socket->OnRead += delegate(this, &Tests::onClientSocketResponseReceived);
				try 
				{
					socket->connect(Poco::Net::SocketAddress("127.0.0.1", port));
					//socket->connect(Poco::Net::SocketAddress("google.com", 80));	
				}
				catch (Poco::Net::NetException& exc)
				{
					Log("error") << "[Tests:" << this << "] Net Error: " << exc.displayText() << endl;
					benchmark->numSuccess++;
					//delete socket;
					//throw exc;
				}
				//Sleep(100);
			}

			ready.wait();

			//assert(false);
			
			// wait for success or timeout
			//benchmark->sw.start();

			benchmark->sw.start();
			//RunDefaultLoop;
			//delete socket;
			delete server;

			Timer::getDefault().stop(TimerCallback<Tests>(this, &Tests::onTimeout, 3000));

			// print stats
		}
		
		for (unsigned i = 0; i < results.size(); i++) {
			Log("info") << "[Tests:" << this << "] Benchamer Result:" 
					<< "\n\tName: " << benchmark->name
					//<< "\n\tnumEchoServiceHandler: " << numEchoServiceHandler
					//<< "\n\tnumReadableNotification: " << numReadableNotification
					<< "\n\tSuccessful: " << benchmark->numSuccess
					<< "\n\tElapsed: " << benchmark->sw.elapsed()
					<< endl;
		}
		*/
		
		/*
			socket->send("hitme", 5);
			
		// raise server

		socket->connect(Net::Address("127.0.0.1", 1337));		
		for (unsigned i = 0; i < numIterations; i++) {
			socket->send("hitme", 5);
		}

		system("pause");
		socket->close();

		// print stats
		
		system("pause");
		
		// raise server

		socket->connect(Net::Address("127.0.0.1", 1337));		
		for (unsigned i = 0; i < numIterations; i++) {
			socket->send("hitme", 5);
		}		
		
		system("pause");
		socket->close();

		// print stats
		
		system("pause");
	}


	void onTimeout(TimerCallback<Tests>& timer)
	{
		// took too long...
		ready.set();
	}		
	
	
	void onClientSocketStateChanged(void* sender, Net::ConnectionState& state, const Net::ConnectionState&)
	{
		Log("debug") << "[Tests:" << this << "] Server Connection State Changed: " << state.toString() << endl;	
		Net::TCPSocket* socket = reinterpret_cast<Net::TCPSocket*>(sender);	
	
		switch (state.id()) {
		case Net::ConnectionState::Connected:
			socket->send("hey", 3);
			break;
		
		case Net::ConnectionState::Disconnected: 
		case Net::ConnectionState::Error:
			//socket->unbindEvents();
			//assert(false);
			delete socket;
			break;
		}
	}
	
	void onClientSocketResponseReceived(void* sender, Buffer&) 
	{		
		Net::TCPSocket* socket = reinterpret_cast<Net::TCPSocket*>(sender);	
		
		//throw;

		benchmark->numSuccess++;
		Log("debug") << "[Tests:" << socket << "] !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Response Received: " << benchmark->numSuccess << "/" << numInstances << endl;	//: " << " << string(data, len)
		if (benchmark->numSuccess == numInstances) {
			//uv_unref(DefaultLoop);
			//delete socket;			
			ready.set();
		}
	}
	
	
		*/
		/*
		if (status != 0)
			return;

		Net::TCPSocket* socket = reinterpret_cast<Net::TCPSocket*>(sender);	
		for (unsigned i = 0; i < numIterations; i++) {
			socket->send("hitme", 5);
		}	

	void onClientSocketConnected(ReactorEvent& event)
	{
		Log("trace") << "[Tests:" << this << "] onClientSocketConnected" << endl;
		//throw;
		//reactor.detach(event.socket, reactorCallback(this, &Tests::onClientSocketConnected, SocketWritable));
		//reactor.attach(event.socket, reactorCallback(this, &Tests::onClientSocketResponseReceived, SocketReadable));
		//event.socket.impl()->sendBytes("hitme", 5);
	}


	void onClientSocketResponseReceived(ReactorEvent& event)
	{
		//Log("trace") << "[Tests:" << this << "] onClientSocketResponseReceived" << endl;
		//assert(0);
		reactor.detach(event.socket, reactorCallback(this, &Tests::onClientSocketResponseReceived, SocketReadable));
		
		benchmark->numSuccess++;
		Log("debug") << "[Tests:" << socket << "] Response Received: " << benchmark->numSuccess << "/" << numInstances << endl;	//: " << " << string(data, len)
		if (benchmark->numSuccess == numInstances) {
			//uv_unref(DefaultLoop);
			//delete socket;			
			ready.set();
		}
	}

	void onClientSocketConnected(void* sender, int status) 
	{
		if (status != 0)
			return;

		Net::TCPSocket* socket = reinterpret_cast<Net::TCPSocket*>(sender);	
		for (unsigned i = 0; i < numIterations; i++) {
			socket->send("hitme", 5);
		}	
	}
		*/

	
		/*
	void onClientSocketResponseReceived(void* sender, char* data, int len) 
	{
		Net::TCPSocket* socket = reinterpret_cast<Net::TCPSocket*>(sender);	

		benchmark->numSuccess++;
		Log("debug") << "[Tests:" << socket << "] Response Received: " << benchmark->numSuccess << "/" << numInstances << ": " << string(data, len) << endl;	
		if (benchmark->numSuccess == numInstances) {
			//uv_unref(DefaultLoop);
			//delete socket;			
			ready.set();
		}
	}
		*/
	
		//uv_unref(DefaultLoop);
		//RunDefaultLoop;

		/*
		Log("info") << "[Tests:" << this << "] On Completion Timer:" 
				<< "\n\tSent: " << numRequestsSent
				<< "\n\tRequests: " << numRequestsReceived
				<< "\n\tResponses: " << numResponsesReceived
				<< endl;
		
		//assert(numRequestsSent == numRequestsReceived);
		//assert(numRequestsSent == numResponsesReceived);
		*/
	/*	
	
	void onTimer(TimerCallback<Tests>& timer)
	{
		socket->send("grr", 3);
	}

	// ----------------------------------------------------------------------------
	//

	void onClientSocketResponseReceived(void* sender, DataPacket& packet) 
	{
		Net::TCPPacketSocket* socket = reinterpret_cast<Net::TCPPacketSocket*>(sender);	

		Log("debug") << "[Tests:" << this << "] Response Received: " << packet.size << ": " << socket->peerAddr() << endl;	
		socket->removeReceiver(packetDelegate<Tests, DataPacket>(this, &Tests::onClientSocketResponseReceived));
		numSuccess++;
		if (numSuccess == numIterations) 

	}

	void onClientSocketStateChanged(void* sender, Net::ConnectionState& state, const Net::ConnectionState&)
	{
		Log("debug") << "[Tests:" << this << "] Client Connection State Changed: " << state.toString() << endl;	

		Net::TCPPacketSocket* socket = reinterpret_cast<Net::TCPPacketSocket*>(sender);	
	
		switch (state.id()) {
		case Net::ConnectionState::Connected:

			// send some data when we are writable
			socket->send("hey", 3);
			break;
		
		case Net::ConnectionState::Disconnected: 
		case Net::ConnectionState::Error:
			assert(false);
			break;
		}
	}

	void onClientSocketResponseReceived(void* sender, DataPacket& packet) 
	{
		Net::TCPPacketSocket* socket = reinterpret_cast<Net::TCPPacketSocket*>(sender);	

		Log("debug") << "[Tests:" << this << "] Response Received: " << packet.size << ": " << socket->peerAddr() << endl;	
		socket->removeReceiver(packetDelegate<Tests, DataPacket>(this, &Tests::onClientSocketResponseReceived));
		numResponsesReceived++;
	}
	
	// ----------------------------------------------------------------------------
	//
	void onServerSocketallocated(void* sender, Net::TCPPacketSocket* socket)
	{
		Log("debug") << "[Tests:" << this << "] TCP Socket allocated: " << socket->peerAddr() << endl;
		socket->StateChanged += delegate(this, &Tests::onServerSocketStateChanged);
		socket->addReceiver(packetDelegate<Tests, DataPacket>(this, &Tests::onServerSocketRequestReceived));	
	}
	
	void onServerSocketStateChanged(void* sender, Net::ConnectionState& state, const Net::ConnectionState&)
	{
		Log("debug") << "[Tests:" << this << "] Server Connection State Changed: " << state.toString() << endl;	

		Net::TCPPacketSocket* socket = reinterpret_cast<Net::TCPPacketSocket*>(sender);	
	
		switch (state.id()) {
		case Net::ConnectionState::Connected:
			//socket->send("hey", 3);
			break;
		
		case Net::ConnectionState::Disconnected: 
		case Net::ConnectionState::Error:
			assert(false);
			break;
		}
	}

	void onServerSocketRequestReceived(void* sender, DataPacket& packet) 
	{
		Net::TCPPacketSocket* socket = reinterpret_cast<Net::TCPPacketSocket*>(sender);	

		Log("debug") << "[Tests:" << this << "] Packet Received: " << packet.size << ": " << socket->peerAddr() << endl;	
		socket->removeReceiver(packetDelegate<Tests, DataPacket>(this, &Tests::onServerSocketRequestReceived));
		numRequestsReceived++;

		// echo it back
		socket->send(packet);
	}
	*/	
		/*
		Net::Reactor reactor;
		Net::TCPPacketServer server(reactor);
		server.bind(Net::Address("127.0.0.1", 1337));
		server.Socketallocated += delegate(this, &Tests::onServerSocketallocated);
		//EchoServerThread s;
		//EchoServer s(4000);		
		//system("pause");		

		Timer::getDefault().start(TimerCallback<Tests>(this, &Tests::onTimeout, 5000));
		
		numRequestsSent = 1;
		numRequestsReceived = 0;
		numResponsesReceived = 0;
		for (unsigned i = 0; i < numRequestsSent; i++) {	
			Net::TCPClientSocket* socket = new Net::TCPClientSocket();
			socket->connect(Net::Address("127.0.0.1", 1337));
			//socket->connect(Net::Address("127.0.0.1", 9977)); //
			Net::TCPPacketSocket* socket = new Net::TCPPacketSocket();
			socket->connect(Net::Address("127.0.0.1", 1337)); //99774000
			//socket->connect(Net::Address("127.0.0.1", 9977)); //
			socket->addReceiver(packetDelegate<Tests, DataPacket>(this, &Tests::onClientSocketResponseReceived));		
			socket->StateChanged += delegate(this, &Tests::onClientSocketStateChanged);
			//socket->close();
			//delete socket;
			Sleep(10);
		}	
		Net::TCPClientSocket* socket = new Net::TCPClientSocket();
		socket->connect(Net::Address("127.0.0.1", 1337));
		for (unsigned i = 0; i < 100; i++) {	
			socket->send("grr", 3);
		}
		
		system("pause");		

		//Net::AsyncLoop reactor;

		//for (unsigned i = 0; i < 100; i++) {	
		//	socket->send("grr", 3);
		//}
		
		Net::TCPSocket server;
		server.bind(Net::Address("127.0.0.1", 1337));
		server.listen();
		*/