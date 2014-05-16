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
		TraceL << "Starting" << endl;		
		
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
		TraceL << "TCP Socket Test: Starting" << endl;			
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
	int UDPPacketSize;
	int UDPNumPacketsWanted;
	int UDPNumPacketsReceived;
	net::Address udpServerAddr;
	net::UDPSocket* udpClientSock;
	/*
	net::UDPSocket* serverSock;
	net::Address serverBindAddr;	
	net::Address clientBindAddr;	
	net::Address clientSendAddr;
	*/
	
	void runUDPSocketTest() 
	{
		// Notes: Sending over home wireless network via
		// ADSL to US server round trip stays around 200ms
		// when sending 1450kb packets at 50ms intervals.
		// At 40ms send intervals latency increated to around 400ms.

		TraceL << "UDP Socket Test: Starting" << endl;
		
		//UDPPacketSize = 10000;
		UDPPacketSize = 1450;
		UDPNumPacketsWanted = 100;
		UDPNumPacketsReceived = 0;
		
		//serverBindAddr.swap(net::Address("0.0.0.0", 1337));	 //
		udpServerAddr.swap(net::Address("74.207.248.97", 1337));	 //
		//udpServerAddr.swap(net::Address("127.0.0.1", 1337));	 //

		//clientBindAddr.swap(net::Address("0.0.0.0", 1338));	
		//clientSendAddr.swap(net::Address("58.7.41.244", 1337));	 //
		//clientSendAddr.swap(net::Address("127.0.0.1", 1337));	 //

		//net::UDPSocket serverSock;
		//serverSock.Recv += sdelegate(this, &Tests::onUDPSocketServerRecv);
		//serverSock.bind(serverBindAddr);
		//this->serverSock = &serverSock;
		
		net::UDPSocket clientSock;
		//clientSock.Recv += sdelegate(this, &Tests::onUDPClientSocketRecv);		
		assert(0 && "fixme");
		clientSock.bind(net::Address("0.0.0.0", 0));	
		clientSock.connect(udpServerAddr);	
		this->udpClientSock = &clientSock;

		//for (unsigned i = 0; i < UDPNumPacketsWanted; i++)
		//	clientSock.send("bounce", 6, serverBindAddr);		

		// Start the send timer
		Timer timer;
		timer.Timeout += sdelegate(this, &Tests::onUDPClientSendTimer);
		timer.start(50, 50);
		timer.handle().ref();
			
		runLoop();
		
		//this->serverSock = nullptr;
		this->udpClientSock = nullptr;
	}
	
	/*
	void onUDPSocketServerRecv(void* sender, net::SocketPacket& packet)
	{
		std::string payload(packet.data(), packet.size());		
		DebugL << "UDPSocket server recv from " 
			<< packet.info->peerAddress << ": payloadLength=" << payload.length() << endl;
		
		// Send the unix ticks milisecond for checking RTT
		//payload.assign(util::itostr(time::ticks()));
		
		// Relay back to the client to check RTT
		//packet.info->socket->send(packet, packet.info->peerAddress);
		//packet.info->socket->send(payload.c_str(), payload.length(), packet.info->peerAddress);		

		packet.info->socket->send(payload.c_str(), payload.length(), clientSendAddr);	
		
	}
	*/

	void onUDPClientSendTimer(void*)
	{
		std::string payload(util::itostr(time::ticks()));
		payload.append(UDPPacketSize - payload.length(), 'x');
		udpClientSock->send(payload.c_str(), payload.length(), udpServerAddr);
	}

	void onUDPClientSocketRecv(void* sender, net::SocketPacket& packet)
	{				
		std::string payload(packet.data(), packet.size());
		payload.erase(std::remove(payload.begin(), payload.end(), 'x'), payload.end());
		UInt64 sentAt = util::strtoi<UInt64>(payload);
		UInt64 latency = time::ticks() - sentAt;

		DebugL << "UDPSocket recv from " << packet.info->peerAddress << ": " 
			<< "payload=" << payload.length() << ", " 
			<< "latency=" << latency 
			<< endl;
		

		/*
		UDPNumPacketsReceived++;
		if (UDPNumPacketsReceived == UDPNumPacketsWanted) {

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
		TraceL << "Timer Test: Starting" << endl;
		Timer timer;
		timer.Timeout += sdelegate(this, &Tests::onOnTimerTimeout);
		timer.start(10, 10);
		
		runLoop();
	}

	void onOnTimerTimeout(void* sender)
	{
		Timer* timer = static_cast<Timer*>(sender);
		TraceL << "On Timer: " << timer->count() << endl;

		if (timer->count() == numTimerTicks)
			timer->stop(); // event loop will be released
	}

	void runLoop() {
		DebugL << "#################### Running" << endl;
		app.run();
		DebugL << "#################### Ended" << endl;
	}

	void runCleanup() {
		DebugL << "#################### Finalizing" << endl;
		app.finalize();
		DebugL << "#################### Exiting" << endl;
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
	Logger::destroy();
	return 0;
}


	/*
//Tests::Result Tests::Benchmark;
//Application Tests::app;


		//TraceL << "UDPSocket Recv: " << packet << ": " << packet.buffer
		//	<< "\n\tPacket " << Benchmark.numSuccess << " of " << UDPNumPacketsWanted << endl;
		//uv::UDPSocket* socket = reinterpret_cast<uv::UDPSocket*>(sender);	

		//TraceL << "UDPSocket Server Recv: " << packet << ": " << packet.buffer << endl;		
		//uv::UDPSocket* socket = reinterpret_cast<uv::UDPSocket*>(sender);	
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

			TraceL << "#################### Running" << endl;
			//app.waitForShutdown(onShutdown, this);
			app.run();
			TraceL << "#################### Ended" << endl;
			*/
			
/*
	

//using uv::TCPEchoServer;
//using uv::TCPServerPtr;
//using uv::SSLEchoServer;
//using uv::SSLServerPtr;
	static void onKillSignal2(uv_signal_t *req, int signum)
	{
		DebugL << "Kill Signal: " << req << endl;
	
		((Tests*)req->data)->tcpServer->stop();
		((Tests*)req->data)->tcpConnector.stop();
		//(*((Handle<TCPEchoServer>*)req->data))->stop(); //->server.stop();delete
		uv_signal_stop(req);
	
		// print active handles
		uv_walk(req->loop, onPrintHandle1, NULL);
	}
	*/