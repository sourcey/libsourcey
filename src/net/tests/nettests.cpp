#include "scy/base.h"
#include "scy/application.h"
#include "scy/time.h"
#include "scy/timer.h"
#include "scy/logger.h"
#include "scy/net/tcpsocket.h"
#include "scy/net/sslsocket.h"
#include "scy/net/sslmanager.h"
#include "scy/net/sslcontext.h"
#include "scy/net/udpsocket.h"
#include "scy/net/address.h"

#include "EchoServer.h"
#include "ClientSocketTest.h"

#include "assert.h"


using namespace std;
using namespace scy;


/*
// Detect memory leaks on winders
#if defined(_DEBUG) && defined(_WIN32)
#include "MemLeakDetect/MemLeakDetect.h"
#include "MemLeakDetect/MemLeakDetect.cpp"
CMemLeakDetect memLeakDetect;
#endif
*/


namespace scy {
namespace net {


#define TEST_SSL 1


class Tests
{
public:
	Application app; 

	Tests()
	{	
#ifdef _MSC_VER
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
		{			

#if TEST_SSL
			// Init SSL Context 
			SSLContext::Ptr ptrContext = new SSLContext(
				SSLContext::CLIENT_USE, "", "", "", 
				SSLContext::VERIFY_NONE, 9, false, 
				"ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");		
			SSLManager::instance().initializeClient(ptrContext);

			// Raise a SSL echo server
			//Handle<SSLEchoServer> sslServer(new SSLEchoServer(1338, true), false);
			//sslServer->run();
#endif

			// Raise a TCP echo server
			//Handle<TCPEchoServer> tcpServer(new TCPEchoServer(1337, true), false); //true
			//tcpServer->run();

			//runAddressTest();			
			//runTCPSocketTest();	
			runUDPSocketTest();

#if TEST_SSL
			//runSSLSocketTest();
#endif	

#if TEST_SSL
			// Shutdown SSL
			SSLManager::instance().shutdown();
#endif

			// Shutdown the garbage collector so we can free memory.
			GarbageCollector::instance().shutdown();
		
			// Run the final cleanup
			runCleanup();
		}
	}
	
	// ============================================================================
	// Address Test
	//
	void runAddressTest() 
	{
		traceL("AddressTest") << "Starting" << endl;		
		
		Address sa1("192.168.1.100", 100);
		assert(sa1.host() == "192.168.1.100");
		assert(sa1.port() == 100);

		Address sa2("192.168.1.100", "100");
		assert(sa2.host() == "192.168.1.100");
		assert(sa2.port() == 100);

		Address sa3("192.168.1.100", "ftp");
		assert(sa3.host() == "192.168.1.100");
		assert(sa3.port() == 21);
		
		Address sa7("192.168.2.120:88");
		assert(sa7.host() == "192.168.2.120");
		assert(sa7.port() == 88);

		Address sa8("[192.168.2.120]:88");
		assert(sa8.host() == "192.168.2.120");
		assert(sa8.port() == 88);

		try {
			Address sa3("192.168.1.100", "f00bar");
			assert(0 && "bad service name - must throw");
		}
		catch (std::exception&) {}

		try {
			Address sa6("192.168.2.120", "80000");
			assert(0 && "invalid port - must throw");
		}
		catch (std::exception&) {}

		try {
			Address sa5("192.168.2.260", 80);
			assert(0 && "invalid address - must throw");
		}
		catch (std::exception&) {}

		try {
			Address sa9("[192.168.2.260:", 88);
			assert(0 && "invalid address - must throw");
		}
		catch (std::exception&) {}

		try {
			Address sa9("[192.168.2.260]");
			assert(0 && "invalid address - must throw");
		}
		catch (std::exception&) {}
	}	
	
	// ============================================================================
	// TCP Socket Test
	//
	void runTCPSocketTest() 
	{
		traceL("Tests") << "TCP Socket Test: Starting" << endl;			
		ClientSocketTest<net::TCPSocket> test(1337);
		test.run();
		runLoop();
	}		

	// ============================================================================
	// SSL Socket Test
	//
	void runSSLSocketTest() 
	{		
		ClientSocketTest<net::SSLSocket> test(1338);
		test.run();
		runLoop();
	}
	
	// ============================================================================
	// UDP Socket Test
	//
	int numUDPPacketsWanted;
	int numUDPPacketsReceived;
	net::UDPSocket serverSock;
	net::UDPSocket clientSock;
	net::Address serverAddr;	
	net::Address clientAddr;	
	
	void runUDPSocketTest() 
	{
		traceL("Tests") << "UDP Socket Test: Starting" << endl;
		
		numUDPPacketsWanted = 100;
		numUDPPacketsReceived = 0;
		
		serverAddr.swap(net::Address("127.0.0.1", 1337));	
		clientAddr.swap(net::Address("127.0.0.1", 1338));	

		serverSock.Recv += delegate(this, &Tests::onUDPSocketServerRecv);
		serverSock.bind(serverAddr);
		
		clientSock.Recv += delegate(this, &Tests::onUDPClientSocketRecv);		
		clientSock.bind(clientAddr);	
		clientSock.connect(serverAddr);	

		//for (unsigned i = 0; i < numUDPPacketsWanted; i++)
		//	clientSock.send("bounce", 6, serverAddr);		

		// Start the send timer
		Timer timer;
		timer.Timeout += delegate(this, &Tests::onUDPClientSendTimer);
		timer.start(100, 100);
			
		runLoop();
	}
	
	void onUDPSocketServerRecv(void* sender, net::SocketPacket& packet)
	{
		std::string payload(packet.data(), packet.size());		
		debugL("UDPInitiator") << "UDPSocket server recv from " 
			<< packet.info->peerAddress << ": payload=" << payload << endl;
		
		// Send the unix ticks milisecond for checking RTT
		//payload.assign(util::itostr(time::ticks()));
		
		// Relay back to the client to check RTT
		//packet.info->socket->send(packet, packet.info->peerAddress);
		packet.info->socket->send(payload.c_str(), payload.length(), packet.info->peerAddress);		
	}

	void onUDPClientSendTimer(void*)
	{
		std::string payload(util::itostr(time::ticks()));
		clientSock.send(payload.c_str(), payload.length(), serverAddr);
	}

	void onUDPClientSocketRecv(void* sender, net::SocketPacket& packet)
	{				
		std::string payload(packet.data(), packet.size());
		UInt64 sentAt = util::strtoi<UInt64>(payload);
		UInt64 latency = time::ticks() - sentAt;

		debugL("UDPInitiator") << "UDPSocket ckient recv from " << packet.info->peerAddress << ": payload=" << payload << ", latency=" << latency << endl;
		

		/*
		numUDPPacketsReceived++;
		if (numUDPPacketsReceived == numUDPPacketsWanted) {

			// Close the client socket dereferencing the main loop.
			packet.info->socket->close();			

			// The server socket is still active so unref the loop once
			// to cause the destruction of both the socket instances.
			app.stop();
		}
		*/
	}
	
	
	// ============================================================================
	// Timer Test
	// TODO: Move to Base tests
	//
	const static int numTimerTicks = 5;

	void runTimerTest() 
	{
		traceL("Tests") << "Timer Test: Starting" << endl;
		Timer timer;
		timer.Timeout += delegate(this, &Tests::onOnTimerTimeout);
		timer.start(10, 10);
		
		runLoop();
	}

	void onOnTimerTimeout(void* sender)
	{
		Timer* timer = static_cast<Timer*>(sender);
		traceL("Tests") << "On Timer: " << timer->count() << endl;

		if (timer->count() == numTimerTicks)
			timer->stop(); // event loop will be released
	}

	void runLoop() {
		debugL("Tests") << "#################### Running" << endl;
		app.run();
		debugL("Tests") << "#################### Ended" << endl;
	}

	void runCleanup() {
		debugL("Tests") << "#################### Finalizing" << endl;
		app.finalize();
		debugL("Tests") << "#################### Exiting" << endl;
	}
};


} } // namespace scy::net


int main(int argc, char** argv) 
{	
	Logger::instance().add(new ConsoleChannel("debug", LTrace));
	Logger::instance().setWriter(new AsyncLogWriter);	
	{
		net::Tests run;
	}
	Logger::shutdown();
	return 0;
}


	/*
//Tests::Result Tests::Benchmark;
//Application Tests::app;


		//traceL("Tests") << "UDPSocket Recv: " << packet << ": " << packet.buffer
		//	<< "\n\tPacket " << Benchmark.numSuccess << " of " << numUDPPacketsWanted << endl;
		//uv::UDPBase* socket = reinterpret_cast<uv::UDPBase*>(sender);	

		//traceL("Tests") << "UDPSocket Server Recv: " << packet << ": " << packet.buffer << endl;		
		//uv::UDPBase* socket = reinterpret_cast<uv::UDPBase*>(sender);	
	static void onShutdown(void* opaque)
	{
		//reinterpret_cast<MediaServer*>(opaque)->shutdown();
	}

	//Handle<TCPEchoServer> tcpServer;
	//ClientSocketTest<net::TCPSocket> tcpConnector; //:
		//tcpServer(new TCPEchoServer(1337, true), false),
		//tcpConnector(1337)
		*/


			
			//tcpConnector.run();
			
			/*
			uv_signal_t sig;
			sig.data = this;
			uv_signal_init(app.loop, &sig);
			uv_signal_start(&sig, Tests::onKillSignal2, SIGINT);

			runUDPSocketTest();
			runTimerTest();
			//runDNSResolverTest();

			traceL("Tests") << "#################### Running" << endl;
			//app.waitForShutdown(onShutdown, this);
			app.run();
			traceL("Tests") << "#################### Ended" << endl;
			*/
			
/*
	

//using uv::TCPEchoServer;
//using uv::TCPServerPtr;
//using uv::SSLEchoServer;
//using uv::SSLServerPtr;
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
	*/