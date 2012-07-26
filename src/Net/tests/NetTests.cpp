#include "Sourcey/Base.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Net/Address.h"
#include "Sourcey/Net/Reactor.h"
#include "Sourcey/Net/TCPSocket.h"
#include "Sourcey/Net/SSLSocket.h"
#include "Sourcey/Net/StatefulSocket.h"
#include "Sourcey/Net/WebSocket.h"

#include "Poco/Net/SSLManager.h"
#include "Poco/Net/KeyConsoleHandler.h"
#include "Poco/Net/ConsoleCertificateHandler.h"


using namespace std;
using namespace Sourcey;
using namespace Sourcey::Net;
using namespace Poco;
using namespace Poco::Net;


// Detect Memory Leaks
#ifdef _DEBUG
#include "MemLeakDetect/MemLeakDetect.h"
#include "MemLeakDetect/MemLeakDetect.cpp"
CMemLeakDetect memLeakDetect;
#endif


namespace Sourcey {
namespace Net {

	
#define SERVER_HOST "127.0.0.1"
#define SERVER_PORT 1337
#define USE_SSL     0

	
class Tests
{
	enum Result 
	{
		Passed,
		Failed
	};

public:
	Poco::Event ready;

	Reactor reactor;
	Address srvAddr;

	Tests() :	
		srvAddr(SERVER_HOST, SERVER_PORT)
	{	
		//runStatefulSocketTest();
		//runTCPStatefulClientSocketTest();
		runWebSocketTest();
		
#if USE_SSL
		//runSSLClientSocketTest();
		//runSSLStatefulClientSocketTest();
		//runSecureWebSocketTest();
#endif

		Util::pause();
	}
	 
	void onConnectionStateChange(void* sender, Net::SocketState& state, const Net::SocketState& oldState) 
	{
		ISocket* socket = reinterpret_cast<ISocket*>(sender);	
		Log("debug") << "Connection State Changed: " << state.toString() << ": " << socket->address() << endl;
		
		switch (state.id()) {
		case Net::SocketState::Connecting:
			break;
		case Net::SocketState::Connected: 
			ready.set(); // pass
			break;
		//case Net::SocketState::Handshaking: 
		//	break;
		//case Net::SocketState::Online: 
		//	break;
		case Net::SocketState::Disconnected: 
			ready.set(); // fail
			break;
		}
	}

	
	void onSocketConnected(void* sender)
	{
		ISocket* socket = reinterpret_cast<ISocket*>(sender);	
		Log("debug") << "Socket Connected to " << socket->peerAddress() << endl;	
		ready.set();
	}

	/*
	void onWebSocketConnected(void* sender)
	{
		IWebSocket* socket = reinterpret_cast<IWebSocket*>(sender);	
		Log("debug") << "Web Socket Connected to " << socket->peerAddress() << endl;	
		ready.set();
	}
	*/
	
	
	void onWebSocketStateChange(void* sender, Net::SocketState& state, const Net::SocketState& oldState) 
	{
		IWebSocket* socket = reinterpret_cast<IWebSocket*>(sender);	
		Log("debug") << "Web Socket Connection State Changed: " << state.toString() << ": " << socket->address() << endl;
		
		switch (state.id()) {
		case Net::SocketState::Connecting:
			break;
		case Net::SocketState::Connected: 
			break;
		//case Net::SocketState::Handshaking: 
		//	break;
		//case Net::SocketState::Online: 
		//	socket->send("echo", 4);
		//	break;
		case Net::SocketState::Disconnected: 
			ready.set(); // fail
			break;
		}
	}
	
	void onWebSocketResponseReceived(void* sender, DataPacket& packet) 
	{
		IWebSocket* socket = reinterpret_cast<IWebSocket*>(sender);	
		string data((const char*)packet.data(), packet.size());
		Log("debug") << "Web Socket Response Received: " << data << ": " << socket->peerAddress() << endl;	
		if (data == "echo")	
			ready.set(); // pass
		else			
			ready.set(); // fail
	}
	
	// ---------------------------------------------------------------------
	//
	// TCP Socket Test
	//
	// ---------------------------------------------------------------------	
	Result runStatefulSocketTest()
	{	
		TCPPacketSocket socket(reactor);
		socket.Connected += delegate(this, &Tests::onSocketConnected);
		socket.connect(srvAddr);
		ready.wait();	
		socket.Connected -= delegate(this, &Tests::onSocketConnected);
		return socket.isConnected() ? Passed : Failed;
	}

	
	// ---------------------------------------------------------------------
	//
	// SSL Socket Test
	//
	// ---------------------------------------------------------------------	
	Result runSSLClientSocketTest()
	{	
		SSLSocket socket(reactor);
		socket.Connected += delegate(this, &Tests::onSocketConnected);
		socket.connect(srvAddr);
		ready.wait();	
		socket.Connected -= delegate(this, &Tests::onSocketConnected);
		return socket.isConnected() ? Passed : Failed;
	}

				
	// ---------------------------------------------------------------------
	//
	// TCP Stateful Socket Test
	//
	// ---------------------------------------------------------------------	
	Result runTCPStatefulClientSocketTest()
	{	
		TCPStatefulSocket socket(reactor);
		socket.StateChange += delegate(this, &Tests::onConnectionStateChange);
		socket.connect(srvAddr);
		ready.wait();	
		socket.StateChange -= delegate(this, &Tests::onConnectionStateChange);
		return socket.isConnected() ? Passed : Failed;
	}
	

	// ---------------------------------------------------------------------
	//
	// SSL Stateful Socket Test
	//
	// ---------------------------------------------------------------------	
	Result runSSLStatefulClientSocketTest()
	{	
		SSLStatefulSocket socket(reactor);
		socket.StateChange += delegate(this, &Tests::onConnectionStateChange);
		socket.connect(srvAddr);
		ready.wait();	
		socket.StateChange -= delegate(this, &Tests::onConnectionStateChange);
		return socket.isConnected() ? Passed : Failed;
	}

	
	// ---------------------------------------------------------------------
	//
	// TCP WebSocket Test
	//
	// ---------------------------------------------------------------------		
	Result runWebSocketTest()
	{	
		PacketWebSocket socket(reactor);
		//socket.StateChange += delegate(this, &Tests::onWebSocketStateChange);
		socket += packetDelegate(this, &Tests::onWebSocketResponseReceived);
		//socket.Connected += delegate(this, &Tests::onWebSocketConnected);
		socket.connect("ws://echo.websocket.org");
		ready.wait();	
		//socket.StateChange -= delegate(this, &Tests::onWebSocketStateChange);
		socket -= packetDelegate(this, &Tests::onWebSocketResponseReceived);
		//socket.Connected -= delegate(this, &Tests::onWebSocketConnected);
		return socket.isConnected() ? Passed : Failed;
	}

	
	// ---------------------------------------------------------------------
	//
	// Secure WebSocket Test
	//
	// ---------------------------------------------------------------------		
	Result runSecureWebSocketTest()
	{	
		SSLPacketWebSocket socket(reactor);
		//socket.StateChange += delegate(this, &Tests::onWebSocketStateChange);
		socket += packetDelegate(this, &Tests::onWebSocketResponseReceived);
		//socket.Connected += delegate(this, &Tests::onWebSocketConnected);
		socket.connect("wss://echo.websocket.org");
		ready.wait();	
		//socket.StateChange -= delegate(this, &Tests::onWebSocketStateChange);
		socket -= packetDelegate(this, &Tests::onWebSocketResponseReceived);
		//socket.Connected -= delegate(this, &Tests::onWebSocketConnected);
		return socket.isConnected() ? Passed : Failed;
	}
};


} } // namespace Sourcey::Net


int main(int argc, char** argv) 
{	
	Logger::instance().add(new ConsoleChannel("debug", TraceLevel));
	
	// Init SSL Context
	SharedPtr<InvalidCertificateHandler> ptrCert;
	Context::Ptr ptrContext = new Context(Context::CLIENT_USE, "", "", "",
		Context::VERIFY_NONE, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");	
	SSLManager::instance().initializeClient(0, ptrCert, ptrContext);
	{
		Tests run;
	}	
	Poco::Net::SSLManager::instance().shutdown();
	Sourcey::Logger::uninitialize();
	return 0;
}



	
	


/*
		//testThread();
		//testAddress();
		//runTimerTest();
		//runUDPSocketTest();

		//app.run(argc, argv);
		//Sourcey::TURN::RelayServer app;
		//app.run(); //argc, argv


	Result runTCPStatefulClientSocketTest()
	{
		EchoClient client(reactor, SERVER_PORT, 0);


	}


	struct EchoClient
	{

		int id;
		Result		  result;
		TCPPacketSocket	  socket;

		EchoClient(Net::Reactor& reactor, int port, int id) : 
			id(id),
			result(None),
			socket(reactor) 
		{
			Log("debug") << "[EchoClient:" << this << "] Creating: " << id << ": " << port << endl;	

			socket += packetDelegate(this, &EchoClient::onServerResponseReceived);
			socket.registerPacketType<DataPacket>(1);
			socket.connect(Net::Address("127.0.0.1", port));
			socket.send("echo", 4);
		}

		virtual ~EchoClient() {
			Log("debug") << "[EchoClient:" << this << "] Destroying: " << id << endl;	
			socket -= packetDelegate(this, &EchoClient::onServerResponseReceived);	
		}
	
		void onServerResponseReceived(void* sender, DataPacket& packet) 
		{
			Net::ISocket* socket = reinterpret_cast<Net::ISocket*>(sender);	

			Log("debug") << "########################## [EchoClient:" << this << "] Packet Received: " << packet.size() << ": " << socket->peerAddress() << endl;	
			//socket->detach(packetDelegate<EchoServer, DataPacket>(this, &EchoServer::onServerSocketRequestReceived));
			// echo the data back
			//socket->send(packet);
		}
	};








#include "Sourcey/Util/Timer.h"

#include "Poco/Stopwatch.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/DNS.h"
#include "Poco/Mutex.h"

#include "SourceyReactorEchoServer.h"
//#include "Sourcey/UVPP.h"
//#include "SourceyAsyncReactorEchoServer.h"
#include "PocoReactorEchoServer.h"
#include "PocoEchoServer.h"
//#include "UVEchoServer.h"
//#include "bak/AsyncLoop.h"

#include "assert.h"

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


#include "Sourcey/Util/Timer.h"
#include "IOStream.h"
#include "AsyncLoop.h"
#include "uv.h"
#include "Sourcey/Util/Timer.h"

#include "Sourcey/Net/UDPPacketSocket.h"
#include "Sourcey/Net/TCPPacketSocket.h"
#include "Sourcey/Net/TCPServer.h"


#include <Windows.h>
#include <Iphlpapi.h>

using Poco::InvalidArgumentException;
*/

	/*
struct BenchmarkResult {
	int numSuccess;
	std::string name;
	Poco::Stopwatch sw;
};
	
static int numRequests;
static int numIterations;
static int numInstances;
static Poco::Event ready;
static BenchmarkResult* benchmark;
*/
	
	
	// ============================================================================
	//
	// Thread Test
	//
	// ============================================================================	
	
	/*
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
		Log("trace") << "[Tests:" << this << "] onClientSocketConnected" << std::endl;
	}

	void onBasicReactorSignal() //void*
	{
		Log("trace") << "[Tests:" << this << "] onBasicReactorSignal" << std::endl;
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
*/
	

	/*
	// ============================================================================
	//
	// Thread Test
	//
	// ============================================================================	
	// A simple thread with 1s sleep
	class BTestThread : public Thread{
	public:
		void run(){
			std::cout<<"\t\t [T]I am going to Sleep"<<std::endl;
			Thread::sleep(2000);
			std::cout<<"\t\t [T]Okay I quit now"<<std::endl;
		}
	};

	// A simple thread which waits a mutex
	class MutexThread : public Thread{
	public:
		Mutex *tex;
	
		MutexThread(Mutex* in_tex){
			tex = in_tex;
		}
	
		void run(){
			std::cout<<"\t\t [T]Lock Mutex"<<std::endl;
			tex->lock();
			std::cout<<"\t\t [T]Mutex Unlocked -> exit"<<std::endl;
		}
	};

	void testThread()
	{	
		BTestThread th;	
		std::cout<<"Execute sleep-thread"<<std::endl;
		th.start();
		//std::cout<<"Wait thread to finish"<<std::endl;
		//Thread::sleep(50); // To be sure that the thread has started before I wait!
		th.wait();
		std::cout<<"Thread has Finished"<<std::endl;
	
		std::cout<<"-----------------------------------------"<<std::endl;
	
		std::cout<<"Create and lock mutex"<<std::endl;
		Mutex tex;
		tex.lock();
		MutexThread mth(&tex);
		std::cout<<"Start the second thread"<<std::endl;
		mth.start();
		std::cout<<"Sleep (the thread is wainting the mutex :)"<<std::endl;
		Thread::sleep(1000);
		std::cout<<"Release the mutex"<<std::endl;
		tex.unlock();
	}
	*/

	/*
	// ============================================================================
	//
	// Timer Test
	//
	// ============================================================================
	int numTimerTicks;
	void runTimerTest() 
	{
		numTimerTicks = 5;

		Timer timer;
		timer.OnTimeout += delegate(this, &Tests::onOnTimerTimeout);
		timer.start(100, 100);

		RunDefaultLoop;
	}

	void onOnTimerTimeout(void* sender, Int64 count)
	{
		Timer* timer = static_cast<Timer*>(sender);

		Log("trace") << "[Tests:" << this << "] Timer: " << count << std::endl;

		if (count == numTimerTicks)
			timer->stop(); // event loop will be released
	}
	*/
	
	/*
	// ============================================================================
	//
	// UDP Socket Test
	//
	// ============================================================================
	void runUDPSocketTest() 
	{
		Log("trace") << "[Tests:" << this << "] runSocketConenctionTest" << std::endl;
			
		Net::Address addr("127.0.0.1", 1337);

		Net::UDPSocket serverSock;
		serverSock.OnSend += delegate(this, &Tests::onUDPSocketServerSend);
		serverSock.OnRecv += delegate(this, &Tests::onUDPSocketServerRecv);
		serverSock.bind(addr);

		Net::UDPSocket clientSock;
		clientSock.OnSend += delegate(this, &Tests::onUDPSocketClientSend);
		clientSock.OnRecv += delegate(this, &Tests::onUDPSocketClientRecv);
		
		clientSock.send("somedata", 8, addr);
		clientSock.send("somedata", 8, addr);
		clientSock.send("somedata", 8, addr);
		
		RunDefaultLoop;
	}
	

	// ----------------------------------------------------------------------------
	//	
	void onUDPSocketClientSend(void* sender, const char* data, int size, const Net::Address& peerAddr)
	{
		Log("trace") << "[Tests:" << this << "] UDPSocket Client Send: " << string(data, size) << ": " << peerAddr << std::endl;
	}
	
	void onUDPSocketClientRecv(void* sender, const char* data, int size, const Net::Address& peerAddr)
	{
		Log("trace") << "[Tests:" << this << "] UDPSocket Client Recv: " << string(data, size) << ": " << peerAddr << std::endl;
	}
	

	// ----------------------------------------------------------------------------
	//	
	void onUDPSocketServerSend(void* sender, const char* data, int size, const Net::Address& peerAddr)
	{
		Log("trace") << "[Tests:" << this << "] UDPSocket Server Send: " << string(data, size) << ": " << peerAddr << std::endl;
	}
	
	void onUDPSocketServerRecv(void* sender, const char* data, int size, const Net::Address& peerAddr)
	{
		Log("trace") << "[Tests:" << this << "] UDPSocket Server Recv: " << string(data, size) << ": " << peerAddr << std::endl;
	}
	*/
	
			
		/*		
	// ============================================================================
	//
	// Socket Conenction Test
	//
	// ============================================================================
	
	//static int numRequestsSent;
	//static int numRequestsReceived;

	//ReactorSignal ReactorEventSignal;	
	//ReactorSignal BasicReactorSignal;
	

	void onClientSocketConnected()
	{
		//reactor.detach(sock, reactorCallback(this, &Tests::onClientSocketConnected, SocketWritable));
	}


	void runSocketConenctionTest() 
	{
		Log("trace") << "[Tests:" << this << "] runPacketSignalReceivers" << std::endl;
		
	//AsyncReactor areactor;
	Reactor reactor;
	Poco::Net::StreamSocket sock;

		reactor.attach(sock, reactorCallback(this, &Tests::onClientSocketConnected, SocketWritable));
		//reactor.attach(sock, reactorCallback(this, &Tests::onClientSocketConnected, SocketWritable));
		//reactor.attach(sock, reactorCallback(this, &Tests::onClientSocketConnected, SocketWritable));
		reactor.detach(sock, reactorCallback(this, &Tests::onClientSocketConnected, SocketWritable));
		reactor.detach(sock, reactorCallback(this, &Tests::onClientSocketConnected, SocketWritable));
		reactor.detach(sock, reactorCallback(this, &Tests::onClientSocketConnected, SocketWritable));
	
		Util::pause();
		return;

		numInstances = 60;
		numIterations = 1;
		
		short port = 1333;
		Net::TCPPacketSocket* socket = NULL;
		Poco::Runnable* server = NULL;
		vector<BenchmarkResult*> results;
			


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
				socket = new Net::TCPPacketSocket(reactor);
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
	}


	void onTimeout(TimerCallback<Tests>& timer)
	{
		// took too long...
		ready.set();
	}		
	
	
	void onClientSocketStateChanged(void* sender, Net::ConnectionState& state, const Net::ConnectionState&)
	{
		Log("debug") << "[Tests:" << this << "] Server Connection State Changed: " << state.toString() << endl;	
		Net::TCPPacketSocket* socket = reinterpret_cast<Net::TCPPacketSocket*>(sender);	
	
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
		Net::TCPPacketSocket* socket = reinterpret_cast<Net::TCPPacketSocket*>(sender);	
		
		benchmark->numSuccess++;
		Log("debug") << "[Tests:" << socket << "] Response Received: " << benchmark->numSuccess << "/" << numInstances << endl;	

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

		Net::TCPPacketSocket* socket = reinterpret_cast<Net::TCPPacketSocket*>(sender);	
		for (unsigned i = 0; i < numIterations; i++) {
			socket->send("hitme", 5);
		}	

	void onClientSocketConnected(ReactorEvent& event)
	{
		Log("trace") << "[Tests:" << this << "] onClientSocketConnected" << std::endl;
		//throw;
		//reactor.detach(event.socket, reactorCallback(this, &Tests::onClientSocketConnected, SocketWritable));
		//reactor.attach(event.socket, reactorCallback(this, &Tests::onClientSocketResponseReceived, SocketReadable));
		//event.socket.impl()->sendBytes("hitme", 5);
	}


	void onClientSocketResponseReceived(ReactorEvent& event)
	{
		//Log("trace") << "[Tests:" << this << "] onClientSocketResponseReceived" << std::endl;
		//assert(0);
		reactor.detach(event.socket, reactorCallback(this, &Tests::onClientSocketResponseReceived, SocketReadable));
		
		benchmark->numSuccess++;
		Log("debug") << "[Tests:" << socket << "] Response Received: " << benchmark->numSuccess << "/" << numInstances << endl;	//: " << " << string(data, size)
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

		Net::TCPPacketSocket* socket = reinterpret_cast<Net::TCPPacketSocket*>(sender);	
		for (unsigned i = 0; i < numIterations; i++) {
			socket->send("hitme", 5);
		}	
	}
		*/

	
		/*
	void onClientSocketResponseReceived(void* sender, char* data, int size) 
	{
		Net::TCPPacketSocket* socket = reinterpret_cast<Net::TCPPacketSocket*>(sender);	

		benchmark->numSuccess++;
		Log("debug") << "[Tests:" << socket << "] Response Received: " << benchmark->numSuccess << "/" << numInstances << ": " << string(data, size) << endl;	
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
		//Util::pause();		

		Timer::getDefault().start(TimerCallback<Tests>(this, &Tests::onTimeout, 5000));
		
		numRequestsSent = 1;
		numRequestsReceived = 0;
		numResponsesReceived = 0;
		for (unsigned i = 0; i < numRequestsSent; i++) {	
			Net::StatefulSocket* socket = new Net::StatefulSocket();
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
		Net::StatefulSocket* socket = new Net::StatefulSocket();
		socket->connect(Net::Address("127.0.0.1", 1337));
		for (unsigned i = 0; i < 100; i++) {	
			socket->send("grr", 3);
		}
		
		Util::pause();		

		//Net::AsyncLoop reactor;

		//for (unsigned i = 0; i < 100; i++) {	
		//	socket->send("grr", 3);
		//}
		
		Net::TCPPacketSocket server;
		server.bind(Net::Address("127.0.0.1", 1337));
		server.listen();
		*/