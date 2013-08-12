#include "Sourcey/UV/UVPP.h"
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


using namespace scy;// {
	

using uv::TCPEchoServer;
//using uv::TCPServerPtr;
//using uv::SSLEchoServer;
//using uv::SSLServerPtr;


template <typename SocketT>
class UVClientSocketTest
{
public:
	typename SocketT socket;
	net::Address address;

	UVClientSocketTest(short port, bool ghost = false) :
		address("127.0.0.1", port)
	{		
		traceL("UVClientSocketTest") << "Creating: " << port << std::endl;
	}

	~UVClientSocketTest()
	{
		traceL("UVClientSocketTest") << "Destroying" << std::endl;
		assert(socket.base().refCount() == 1);
	}

	void run() 
	{
		// Create the socket instance on the stack.
		// When the socket is closed it will unref the main loop
		// causing the test to complete successfully.
		socket.Recv += delegate(this, &UVClientSocketTest::onRecv);
		socket.Connect += delegate(this, &UVClientSocketTest::onConnected);
		socket.Error += delegate(this, &UVClientSocketTest::onError);
		socket.Close += delegate(this, &UVClientSocketTest::onClosed);
		socket.connect(address);
		assert(socket.base().refCount() == 1);
	}

	void stop() 
	{
		//socket.close();
		socket.shutdown();
	}
	
	void onConnected(void* sender)
	{
		traceL("UVClientSocketTest") << "Connected" << endl;
		assert(sender == &socket);
		socket.send("client > server", 15);
		socket.send("client > server", 15);
		socket.send("client > server", 15);
	}
	
	void onRecv(void* sender, net::SocketPacket& packet)
	{
		assert(sender == &socket);
		string data(packet.data(), 15);
		traceL("UVClientSocketTest") << "Recv: " << data << endl;	

		// Check for return packet echoing sent data
		if (data == "client > server") {
			traceL("UVClientSocketTest") << "Recv: Got Return Packet!" << endl;
			
			// Send the shutdown command to close the connection gracefully.
			// The peer disconnection will trigger an eof error callback
			// which notifies us that the connection is dead.
			//socket.shutdown();
			//socket.close();
		}
		else 
			assert(false); // fail...
	}

	void onError(void* sender, int err, const std::string& message)
	{
		//uv::SocketT::Base* socket = reinterpret_cast<uv::SocketT::Base*>(sender);	
		errorL("UVClientSocketTest") << "On Error: " << err << ": " << message << endl;
		assert(sender == &socket);

		// Close the socket handle to dereference the main loop.
		//socket.close();
	}
	
	void onClosed(void* sender)
	{
		traceL("UVClientSocketTest") << "On Closed" << endl;
		// The last callback to fire is the Closed signal  
		// which notifies us the underlying libuv socket 
		// handle is closed. Das is gut!
	}
};


#define TEST_SSL 0// 1



	
			/*
static void onPrintHandle1(uv_handle_t* handle, void* arg) 
{
	debugL("onPrintHandle") << ">>>#### Active Handle: " << handle << std::endl;
	uv_tcp_t* tcp = (uv_tcp_t*)handle;
	if (handle->type == UV_TCP) {
		debugL("onPrintHandle") << ">>>#### Active TCP Handle: " << tcp << std::endl;
		tcp;
	}
}


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
	static uv::Loop ourLoop; 

	static void onKillSignal2(uv_signal_t *req, int signum)
	{
		debugL("Application") << "Kill Signal: " << req << endl;
	
		((Tests*)req->data)->tcpServer->stop();
		((Tests*)req->data)->tcpConnector.stop();
		//(*((Handle<TCPEchoServer>*)req->data))->stop(); //->server.stop();delete
		uv_signal_stop(req);
	
		// print active handles
		uv_walk(req->loop, onPrintHandle1, NULL);
	}

	Handle<TCPEchoServer> tcpServer;

	UVClientSocketTest<net::TCPSocket> tcpConnector;

	Tests() :
		tcpServer(new TCPEchoServer(1337, true), false),
		tcpConnector(1337)
	{	
		traceL("Tests") << "#################### Starting" << endl;
#ifdef _MSC_VER
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
		{			
#if TEST_SSL
			// Init SSL Context 
			// The singleton leaks a little openssl memory on close.
			// TODO: Refactor SSL stuff into the network manager.
			Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> ptrCert;
			Poco::Net::Base::Ptr ptrContext = new Poco::Net::Base(
				Poco::Net::Base::CLIENT_USE, "", "", "", Poco::Net::Base::VERIFY_NONE, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");		
			Poco::Net::SSLManager::instance().initializeClient(0, ptrCert, ptrContext);

			// Raise a SSL echo server
			Handle<SSLEchoServer> sslServer(new SSLEchoServer(1338, true), false);
			sslServer->run();
#endif

			// Raise a TCP echo server
			//Handle<TCPEchoServer> tcpServer(new TCPEchoServer(1337, true), false); //true
			tcpServer->run();
			
			//runTCPSocketTest();
#if TEST_SSL
			runSSLSocketTest();
#endif
			
			tcpConnector.run();
			
			uv_signal_t sig;
			sig.data = this;
			uv_signal_init(ourLoop.loop, &sig);
			uv_signal_start(&sig, Tests::onKillSignal2, SIGINT);

			runUDPSocketTest();
			runTimerTest();
			//runDNSResolverTest();
			
			traceL("Tests") << "#################### Running" << endl;
			ourLoop.waitForKill();
			traceL("Tests") << "#################### Ended" << endl;
	
#if TEST_SSL
			// Shutdown SSL
			//Poco::Net::SSLManager::instance().shutdown();
#endif
		}
		
		traceL("Tests") << "#################### Finalizing" << endl;
		//ourLoop.cleanup();
		traceL("Tests") << "#################### Exiting" << endl;
	}
	
	// ============================================================================
	// TCP Socket Test
	//
	void runTCPSocketTest() 
	{
		traceL("Tests") << "TCP Socket Test: Starting" << endl;			
		UVClientSocketTest<net::TCPSocket> test(1337);
		test.run();
		traceL("Tests") << "TCP Socket Test: Running" << endl;	
		ourLoop.run();
		traceL("Tests") << "TCP Socket Test: Ending" << endl;
	}		

	// ============================================================================
	// SSL Socket Test
	//
	void runSSLSocketTest() 
	{		
		UVClientSocketTest<net::SSLSocket> test(1338);
		test.run();
		ourLoop.run();
	}
	
	// ============================================================================
	// UDP Socket Test
	//
	const static int numUDPPacketsWanted = 10;
	
	void runUDPSocketTest() 
	{
		traceL("Tests") << "UDP Socket Test: Starting" << endl;
		net::Address serverAddr("127.0.0.1", 1337);	
		Benchmark.reset();

		net::UDPSocket serverSock;
		serverSock.Recv += delegate(this, &Tests::onUDPSocketServerRecv);
		serverSock.bind(serverAddr);
		
		net::Address clientAddr("127.0.0.1", 1338);	
		net::UDPSocket clientSock;
		clientSock.Recv += delegate(this, &Tests::onUDPSocketRecv);		
		clientSock.bind(clientAddr);	
		for (unsigned i = 0; i < numUDPPacketsWanted; i++)
			clientSock.send("somedata", 8, serverAddr);

		ourLoop.run();
		traceL("Tests") << "UDP Socket Test: Ending" << endl;	
	}
	
	// ----------------------------------------------------------------------------
	//		
	void onUDPSocketRecv(void* sender, net::SocketPacket& packet)
	{
		//traceL("Tests") << "UDPSocket Recv: " << packet << ": " << packet.buffer
		//	<< "\n\tPacket " << Benchmark.numSuccess << " of " << numUDPPacketsWanted << endl;
		uv::UDPBase* socket = reinterpret_cast<uv::UDPBase*>(sender);	
		Benchmark.numSuccess++;
		if (Benchmark.numSuccess == numUDPPacketsWanted) {

			// Close the client socket dereferencing the main loop.
			socket->close();
			
			// Unref the loop once should cause the loop to stop and
			// the destroy the socket instances.
			ourLoop.stop();
			return;
		}
	}
	
	// ----------------------------------------------------------------------------
	//		
	void onUDPSocketServerRecv(void* sender, net::SocketPacket& packet)
	{
		traceL("Tests") << "UDPSocket Server Recv: " << (IPacket&)packet << ": " << packet.buffer << endl;
		//traceL("Tests") << "UDPSocket Server Recv: " << packet << ": " << packet.buffer << endl;		
		uv::UDPBase* socket = reinterpret_cast<uv::UDPBase*>(sender);	
		socket->send(packet, packet.info->peerAddress);
	}
	
	// ============================================================================
	// Timer Test
	//
	const static int numTimerTicks = 5;

	void runTimerTest() 
	{
		traceL("Tests") << "Timer Test: Starting" << endl;
		Timer timer;
		timer.Timeout += delegate(this, &Tests::onOnTimerTimeout);
		timer.start(100, 100);
		
		Benchmark.reset();
		traceL("Tests") << "Timer Test: ourLooping" << endl;
		ourLoop.run();
		traceL("Tests") << "Timer Test: Ending" << endl;
	}

	void onOnTimerTimeout(void* sender)
	{
		Timer* timer = static_cast<Timer*>(sender);
		traceL("Tests") << "On Timer: " << timer->count() << endl;

		if (timer->count() == numTimerTicks) {
			timer->stop(); // event loop will be released
			//ourLoop.stop();;
		}
	}
};
			*/


//} // namespace scy


int main(int argc, char** argv) 
{	
	Logger::instance().add(new ConsoleChannel("debug", TraceLevel));
	{
		//Tests app;
	}
	Logger::shutdown();
	return 0;
}


//Tests::Result Tests::Benchmark;
//uv::Loop Tests::ourLoop;
