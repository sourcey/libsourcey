#include "Sourcey/UV/UVPP.h"
#include "Sourcey/HTTP/Server.h"
#include "Sourcey/HTTP/Connection.h"
#include "Sourcey/HTTP/Client.h"
#include "Sourcey/HTTP/WebSocket.h"
#include "Sourcey/Runner.h"
#include "Sourcey/Timer.h"

#include "Sourcey/Base.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Net/Address.h"

/*
// TODO: depreciate Poco dependency
#include "Poco/Stopwatch.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/DNS.h"
#include "Poco/Net/Session.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/KeyConsoleHandler.h"
#include "Poco/Net/ConsoleCertificateHandler.h"
*/

#include "assert.h"
#include <iterator>


using namespace std;
using namespace scy;
using namespace scy::net;


/*
// Detect memory leaks on winders
#if defined(_DEBUG) && defined(_WIN32)
#include "MemLeakDetect/MemLeakDetect.h"
#include "MemLeakDetect/MemLeakDetect.cpp"
CMemLeakDetect memLeakDetect;
#endif
*/


namespace scy {
namespace http {

	
#define TEST_SSL 0
#define TEST_HTTP_PORT 1337


// -------------------------------------------------------------------
//
class SimpleResponser: public ServerResponser
{
public:
	SimpleResponser(ServerConnection& conn) : 
		ServerResponser(conn)
	{
	}

	void onRequestComplete(Request& request, Response& response) 
	{
		response.body << "Hello universe";
		connection().respond();
	}
};


// -------------------------------------------------------------------
//
class ChunkedResponser: public ServerResponser
{
public:
	uv::Timer timer;
	bool gotHeaders;
	bool gotRequest;
	bool gotClose;

	ChunkedResponser(ServerConnection& conn) : 
		ServerResponser(conn), 
		gotHeaders(false), 
		gotRequest(false), 
		gotClose(false)
	{
	}

	~ChunkedResponser()
	{
		assert(gotHeaders);
		assert(gotRequest);
		assert(gotClose);
	}

	void onRequestHeaders(Request& request) 
	{
		gotHeaders = true;
	}

	void onRequestBody(const Buffer& body)
	{
		// may be empty
	}

	void onRequestComplete(Request& request, Response& response) 
	{
		gotRequest = true;
		timer.Timeout += delegate(this, &ChunkedResponser::onTimer);
		timer.start(100, 100);
	}

	void onClose()
	{
		traceL("ChunkedResponser") << "On connection close" << endl;
		gotClose = true;
		timer.stop();
	}
	
	void onTimer(void*)
	{
		connection().sendBytes("bigfatchunk", 11);

		if (timer.count() == 10)
			connection().close();
	}
};


// -------------------------------------------------------------------
//
class WebSocketResponser: public ServerResponser
{
public:
	uv::Timer timer;
	bool gotHeaders;
	bool gotRequest;
	bool gotClose;

	WebSocketResponser(ServerConnection& conn) : 
		ServerResponser(conn), 
		gotHeaders(false), 
		gotRequest(false), 
		gotClose(false)
	{
	}

	~WebSocketResponser()
	{
		assert(gotHeaders);
		assert(gotRequest);
		assert(gotClose);
	}

	void onRequestHeaders(Request& request) 
	{
		gotHeaders = true;
	}

	void onRequestBody(const Buffer& body)
	{
		// may be empty
	}

	void onRequestComplete(Request& request, Response& response) 
	{
		gotRequest = true;

		timer.Timeout += delegate(this, &WebSocketResponser::onTimer);
		timer.start(100, 100);
	}

	void onClose()
	{
		traceL("WebSocketResponser") << "On connection close" << endl;
		gotClose = true;
		timer.stop();
	}
	
	void onTimer(void*)
	{
		connection().sendBytes("bigfatchunk", 11);

		if (timer.count() == 10)
			connection().close();
	}
};


// -------------------------------------------------------------------
//
class OurResponserFactory: public ServerResponserFactory
{
public:
	ServerResponser* createResponser(ServerConnection& conn)
	{		
		ostringstream os;
		conn.request().write(os);
		string headers(os.str().data(), os.str().length());
		traceL("OurResponserFactory") << "@@@@@@@@@@@@@@@@@@@ Request: " << headers << endl; // remove me

		// TODO: Chunked responser
		// TODO: WebSocket responser
		if (conn.request().getURI() == "/chunked")
			return new ChunkedResponser(conn);
		else
			return new SimpleResponser(conn);
	}
};


// -------------------------------------------------------------------
//
template <typename SocketT>
class SocketClientEchoTest
{
public:
	typename SocketT socket;
	net::Address address;

	SocketClientEchoTest(const net::Address& addr, bool ghost = false) :
		address(addr)
	{		
		traceL("SocketClientEchoTest") << "Creating: " << addr << std::endl;

		socket.Recv += delegate(this, &SocketClientEchoTest::onRecv);
		socket.Connect += delegate(this, &SocketClientEchoTest::onConnect);
		socket.Error += delegate(this, &SocketClientEchoTest::onError);
		socket.Close += delegate(this, &SocketClientEchoTest::onClose);
	}

	~SocketClientEchoTest()
	{
		traceL("SocketClientEchoTest") << "Destroying" << std::endl;
		assert(socket.base().refCount() == 1);
	}

	void start() 
	{
		// Create the socket instance on the stack.
		// When the socket is closed it will unref the main loop
		// causing the test to complete successfully.
		socket.connect(address);
		assert(socket.base().refCount() == 1);
	}

	void stop() 
	{
		//socket.close();
		socket.shutdown();
	}
	
	void onConnect(void* sender)
	{
		traceL("SocketClientEchoTest") << "Connected" << endl;
		assert(sender == &socket);
		socket.send("client > server", 15, WebSocket::FRAME_TEXT);
	}
	
	void onRecv(void* sender, net::SocketPacket& packet)
	{
		assert(sender == &socket);
		string data(packet.data(), 15);
		traceL("SocketClientEchoTest") << "Recv: " << data << endl;	

		// Check for return packet echoing sent data
		if (data == "client > server") {
			traceL("SocketClientEchoTest") << "Recv: Got Return Packet!" << endl;
			
			// Send the shutdown command to close the connection gracefully.
			// The peer disconnection will trigger an error callback which
			// will result is socket closure.
			socket.shutdown();
		}
		else
			assert(0 && "not echo response"); // fail...
	}

	void onError(void* sender, int err, const std::string& message)
	{
		errorL("SocketClientEchoTest") << "On Error: " << err << ": " << message << endl;
		assert(sender == &socket);
	}
	
	void onClose(void* sender)
	{
		traceL("SocketClientEchoTest") << "On Closed" << endl;
		assert(sender == &socket);
	}
};


// -------------------------------------------------------------------
//
class Tests
{
public:
	static Runner Loop; 	

	Tests()
	{	
		debugL("Tests") << "#################### Starting" << endl;
#ifdef _MSC_VER
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
		{		
			/*
			//runWebSocketSocketTest();
			runHTTPClientTest();	
			runHTTPClientTest();	
			runHTTPClientTest();	
			runHTTPClientTest();	
			runHTTPClientTest();	
			runHTTPClientTest();	
			runHTTPClientTest();	
			*/
			runChunkedHTTPClientTest();	

			//runHTTPServerTest();	
		}
		
		debugL("Tests") << "#################### Finalizing" << endl;
		Loop.cleanup();
		debugL("Tests") << "#################### Exiting" << endl;
	}


	void runLoop() {
		debugL("Tests") << "#################### Running" << endl;
		Loop.run();
		debugL("Tests") << "#################### Ended" << endl;
	}

	
	// ============================================================================
	// HTTP Client Tests
	//
	struct HTTPClientTest 
	{
		http::Server server;
		http::Client client;

		HTTPClientTest() :
			server(TEST_HTTP_PORT, new OurResponserFactory) 
		{
		}

		ClientConnection* create() {
			server.start();

			ClientConnection* conn = client.createConnection(net::Address("127.0.0.1", TEST_HTTP_PORT));		
			conn->ResponseHeaders += delegate(this, &HTTPClientTest::onResponseHeaders);
			conn->ResponseBody += delegate(this, &HTTPClientTest::onResponseBody);
			conn->ResponseComplete += delegate(this, &HTTPClientTest::onResponseComplete);
			conn->Close += delegate(this, &HTTPClientTest::onClose);
			return conn;
		}		

		void onResponseHeaders(void*, Response& res)
		{
			debugL("ClientConnectionTest") << "On headers: " <<  endl;
		}

		void onResponseBody(void*, Buffer& buf)
		{
			debugL("ClientConnectionTest") << "On body: " << buf << endl;
			buf.clear();
		}

		void onResponseComplete(void*, const Response& res)
		{		
			ostringstream os;
			res.write(os);
			debugL("ClientConnectionTest") << "Response complete: " << os.str() << endl;
		}

		void onClose(void*)
		{	
			debugL("ClientConnectionTest") << "Connection closed" << endl;
			//assert(0);

			// Stop the client and server to release the loop
			server.shutdown();
			client.shutdown();
		}
	};

	void runBasicHTTPClientTest() 
	{	
		HTTPClientTest test;
		test.create()->send(); // default GET request
		runLoop();
	}

	void runChunkedHTTPClientTest() 
	{	
		HTTPClientTest test;		
		ClientConnection* conn = test.create();
		conn->request().setURI("/chunked");
		conn->send();
		runLoop();
	}

	SSL WebSocket
	HTTP WebSocket Upgrade

	
	
	// ============================================================================
	// WebSocket Test
	//
	void runWebSocketSocketTest() 
	{
		traceL("Tests") << "TCP Socket Test: Starting" << endl;			
		// ws://echo.websocket.org		
		//SocketClientEchoTest<http::WebSocket> test(net::Address("174.129.224.73", 1339));
		//SocketClientEchoTest<http::WebSocket> test(net::Address("174.129.224.73", 80));
		SocketClientEchoTest<http::WebSocket> test(net::Address("127.0.0.1", 1340));
		test.start();

		runLoop();
	}	
	
	// ============================================================================
	// HTTP Server Test
	//
	void runHTTPServerTest() 
	{
		http::Server srv(TEST_HTTP_PORT, new OurResponserFactory);
		srv.start();
		
		/*
		uv_signal_t sig;
		sig.data = &srv;
		uv_signal_init(Loop.loop, &sig);
		uv_signal_start(&sig, Tests::onKillHTTPServer, SIGINT);
		*/
		uv_signal_t* sig = new uv_signal_t;
		sig->data = &srv;
		uv_signal_init(Loop.loop, sig);
		uv_signal_start(sig, Tests::onKillHTTPServer, SIGINT);

		runLoop();
	}
	
	static void onPrintHTTPServerHandle(uv_handle_t* handle, void* arg) 
	{
		debugL("HTTPServerTest") << "#### Active HTTPServer Handle: " << handle << std::endl;
	}

	static void onKillHTTPServer(uv_signal_t *req, int signum)
	{
		debugL("HTTPServerTest") << "Kill Signal: " << req << endl;
	
		// print active handles
		uv_walk(req->loop, Tests::onPrintHTTPServerHandle, NULL);
	
		((http::Server*)req->data)->shutdown();

		uv_signal_stop(req);
	}

};


} } // namespace scy::http


int main(int argc, char** argv) 
{	
	Logger::instance().add(new ConsoleChannel("debug", TraceLevel));
	{
		http::Tests app;
	}
	Logger::uninitialize();
	return 0;
}


//Tests::Result Tests::Benchmark;
Runner http::Tests::Loop;



			
			/*
			// ============================================================================
			// HTTP Server
			//
			http::Server srv(81, new OurResponserFactory);
			srv.start();

			(sock);
			net::TCPSocket sock;	
			runHTTPClientTest();

			uv_signal_t sig;
			sig.data = this;
			uv_signal_init(Loop.loop, &sig);
			uv_signal_start(&sig, Tests::onKillSignal2, SIGINT);

			runUDPSocketTest();
			runTimerTest();
			//runDNSResolverTest();
			
			debugL("Tests") << "#################### Running" << endl;
			Loop.run();
			debugL("Tests") << "#################### Ended" << endl;
			*/

			/*		
			http::Server srv(81, new OurResponserFactory);
			srv.start();

			//runHTTPClientTest();	
			(sock);
			net::TCPSocket sock;	
			runHTTPClientTest();

			uv_signal_t sig;
			sig.data = this;
			uv_signal_init(Loop.loop, &sig);
			uv_signal_start(&sig, Tests::onKillSignal2, SIGINT);

			runUDPSocketTest();
			runTimerTest();
			//runDNSResolverTest();
			
			debugL("Tests") << "#################### Running" << endl;
			Loop.run();
			debugL("Tests") << "#################### Ended" << endl;
			*/
	
		/*
	// ============================================================================
	// HTTP ClientConnection Test
	//
	void runHTTPClientTest() 
	{
		debugL("ClientConnectionTest") << "Starting" << endl;	

		// Setup the transaction
		http::Request req("GET", "http://google.com");
		http::Response res;
		http::ClientConnection txn(&req);
		txn.Complete += delegate(this, &Tests::onResponseComplete);
		txn.DownloadProgress += delegate(this, &Tests::onResponseProgress);	
		txn.send();

		// Run the looop
		Loop.run();
		util::pause();

		debugL("ClientConnectionTest") << "Ending" << endl;
	}		

	void onResponseComplete(void* sender, http::Response& response)
	{
		debugL("ClientConnectionTest") << "On Complete: " << &response << endl;
	}

	void onResponseProgress(void* sender, http::TransferProgress& progress)
	{
		debugL("ClientConnectionTest") << "On Progress: " << progress.progress() << endl;
	}
		*/
	
	
//using http::Request;
//using http::Response;
//using http::ClientConnection;

/*
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
*/