#include "UDPInitiator.h"
#include "UDPResponder.h"

#include "TCPInitiator.h"
#include "TCPResponder.h"

#include "Sourcey/TURN/client/Client.h"
#include "Sourcey/TURN/server/Server.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Util.h"

#include <iostream>


using namespace std;
using namespace Sourcey;
//using namespace Sourcey::Net;


// Detect Memory Leaks
#ifdef _DEBUG
//#include "MemLeakDetect/MemLeakDetect.h"
//#include "MemLeakDetect/MemLeakDetect.cpp"
//CMemLeakDetect memLeakDetect;
#endif


#define USE_TCP


namespace Sourcey {
namespace TURN {
	

struct EchoServer
{	
	//Runner runner;
	//TURN::Server* server;	
	Net::TCPPacketServer server;
	
	//EchoServer() : server(NULL) {}	
	//~EchoServer() { if (server) delete server; }	
		
	void run(int port) 
	{
		//Net::Reactor reactor;
		//Net::TCPPacketServer server; //(reactor);
		server.bind(Net::Address("127.0.0.1", port));
		server.SocketCreated += delegate(this, &EchoServer::onServerSocketCreated);
	}
	
	void onServerSocketCreated(void* sender, Net::TCPSocket* socket)
	{
		Log("debug") << "[Echos:" << this << "] TCP Socket Created: " << socket->peerAddress() << endl;
		//socket->StateChanged += delegate(this, &Echos::onServerSocketStateChanged);
		socket->attach(packetDelegate<EchoServer, DataPacket>(this, &EchoServer::onServerSocketRequestReceived));	
	}
	
	/*
	void onServerSocketStateChanged(void* sender, Net::ClientState& state, const Net::ClientState&)
	{
		Log("debug") << "[Echos:" << this << "] Server Connection State Changed: " << state.toString() << endl;	

		Net::TCPSocket* socket = reinterpret_cast<Net::TCPSocket*>(sender);	
	
		switch (state.id()) {
		case Net::ClientState::Connected:
			//socket->send("hey", 3);
			break;
		
		case Net::ClientState::Disconnected: 
		case Net::ClientState::Error:
			assert(false);
			break;
		}
	}
	*/

	void onServerSocketRequestReceived(void* sender, DataPacket& packet) 
	{
		Net::TCPSocket* socket = reinterpret_cast<Net::TCPSocket*>(sender);	

		//Log("debug") << "[Echos:" << this << "] Packet Received: " << packet.size << ": " << socket->remoteAddr() << endl;	
		//socket->detach(packetDelegate<EchoServer, DataPacket>(this, &EchoServer::onServerSocketRequestReceived));

		// echo the data back
		socket->send(packet);
	}
};


struct TestServer: public TURN::IServerObserver
{	
	//Runner runner;, runner
	TURN::Server* server;	
	
	TestServer() : server(NULL) {}	
	~TestServer() { if (server) delete server; }	
		
	void run(const TURN::Server::Options& so) 
	{
		server = new TURN::Server(*this, so);
		server->start();
	}
	
	AuthenticationState authenticateRequest(Server* server, const Request& request)
	{
		Log("debug") << "[TestServer:" << this << "] Authenticate Request" << endl;
		return Authorized;
	}

	void onServerAllocationCreated(Server* server, IAllocation* alloc) 
	{
	}

	void onServerAllocationRemoving(Server* server, IAllocation* alloc)
	{
	}
};


struct ClientTest
{
	enum Result {
		None,
		Success,
		Failed
	};

	//Signal<Result&>	TestDone;

	int id;
	Result result;
#ifdef USE_TCP
	TCPInitiator* initiator;
	TCPResponder* responder;
	ClientTest(int id = 0) : 
		id(id),
		result(None),
		initiator(new TCPInitiator(id)), 
		responder(new TCPResponder(id)) {}	
#else
	UDPInitiator* initiator;
	UDPResponder* responder;
	ClientTest(int id = 0) : 
		id(id),
		result(None),
		initiator(new UDPInitiator(id)), 
		responder(new UDPResponder(id)) {}	
#endif

	~ClientTest() {
		if (initiator)
			delete initiator;
		if (responder)
			delete responder;
	}

	void run(const Client::Options& o) 
	{		
		//
		// The initiating client...
		//
		Net::IP peerIP("127.0.0.1");
		initiator->AllocationCreated += delegate(this, &ClientTest::onInitiatorAllocationCreated);
		initiator->initiate(o, peerIP);
	}
	
	virtual void onInitiatorAllocationCreated(void* sender, TURN::Client& client) //
	{
		Log("debug") << "TURN: TCPInitiator Allocation Created" << endl;
		responder->start(client.relayedAddress());
	}
	
	virtual void onTestDone(void* sender, bool success)
	{
		Log("debug") << "ClientTestRunner: TestDone" << endl;
		result = success ? Success : Failed;
		//TestDone.dispatch(this, result);
	}
};


struct ClientTestRunner
{
	std::vector<TURN::ClientTest*> tests;
	Poco::Event done;

	int nTimes;	
	int nComplete;
	int nSucceeded;
	
	ClientTestRunner() : nTimes(0), nComplete(0), nSucceeded(0) {}
	~ClientTestRunner() { 		
		Timer::getDefault().stop(TimerCallback<ClientTestRunner>(this, &ClientTestRunner::onTimeout));
		Util::ClearVector(tests);  
	} //

	void run(const Client::Options& o, int times, int timeout) 
	{	
		if (timeout)
			Timer::getDefault().start(TimerCallback<ClientTestRunner>(this, &ClientTestRunner::onTimeout, timeout));

		nTimes = times;
		for (unsigned i = 0; i < nTimes; i++) {
			TURN::ClientTest* client = new TURN::ClientTest(i);
			client->initiator->TestDone += delegate(this, &ClientTestRunner::onTestDone);
			client->run(o);
			tests.push_back(client);
		}

		done.wait();
	}	

	void onTimeout(TimerCallback<ClientTestRunner>& timer)
	{
		// took too long...
		done.set();
	}	
	
	virtual void onTestDone(void* sender, bool result)
	{
		Log("debug") << "ClientTestRunner: TestDone" << endl;

		nComplete++;
		if (result)
			nSucceeded++;
		if (nComplete == nTimes)	
			done.set();
		print(cout);
	}

	void print(std::ostream& ost) 
	{	
		ost << "ClientTestRunner Results:" 
			<< "\n\tTimes: " << nTimes
			<< "\n\tComplete: " << nComplete
			<< "\n\tSucceeded: " << nSucceeded
			<< endl;
	}

};



} } //  namespace Sourcey::TURN


int main(int argc, char** argv)
{
	//Sourcey::Logger::instance().add(new Sourcey::ConsoleChannel("debug", Sourcey::TraceLevel));
	Logger::instance().add(new FileChannel("debug", "", "log", "%H:%M:%S", 12 * 3600, TraceLevel)); //d:/
	
	try	{
		
	//
	// Initialize the server
	//
	TURN::Server::Options so;
	//so.software						= "SERVER_SOFTWARE";
	//so.realm							= SERVER_REALM;
	//so.allocationDefaultLifetime		= ALLOCATION_DEFAULT_LIFETIME;
	//so.allocationMaxLifetime			= ALLOCATION_MAX_LIFETIME;
	so.listenAddr						= Net::Address("127.0.0.1", 3478);
	
	TURN::TestServer srv;
	srv.run(so);
	system("pause");
	
	/*
	//
	// Initialize the clients
	//
	TURN::Client::Options co;
	co.serverAddr = Net::Address("127.0.0.1", 3478);
	co.lifetime  = 120 * 1000; // 1 minute
	co.timeout = 10 * 1000;
	co.timerInterval = 3 * 1000;
	co.username = "user";
	co.password = Anionu_API_PASSWORD;
	
		{
			TURN::ClientTestRunner test;
			test.run(co, 10, 10000);
			system("pause");
			test.print(cout);
			system("pause"); 
		}
		Log("debug") << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ [TEST] ENDING" << endl;
		*/
	} 
	catch (Poco::Exception& exc) {
		Log("error") << "TEST: Error: " << exc.displayText() << std::endl;
		system("pause");
	}
	Log("debug") << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ [TEST] ENDED" << endl;


	return 0;
}



	/*
	//Sourcey::TURN::ClientTest app;
	//app.run();
	*/
	
	/*
	void onTCPSocketCreated(void* sender, Net::TCPSocket* socket)
	{
		Log("debug") << "[TClientTest:" << this << "] onTCPSocketCreated" << endl;		
		//socket->bindEvents();
		socket->attach(packetDelegate<ClientTest, DataPacket>(this, &ClientTest::onPacketReceived));
		//socket->unbindEvents();
		//socket->unbindEvents();

		Net::TCPSocket* socket1 = new Net::TCPSocket(socket->impl());
		//socket1->bindEvents();
		socket->attach(packetDelegate<ClientTest, DataPacket>(this, &ClientTest::onPacketReceived));
		socket1->attach(packetDelegate<ClientTest, DataPacket>(this, &ClientTest::onPacketReceived1));
		
		socket->transferTo(socket1);

		//Poco::Net::SocketImpl* impl = socket->impl().impl();
		//Poco::Net::SocketImpl* impl1 = socket1->impl().impl();
		//assert(impl == impl1);
		//impl->duplicate();

		//delete socket;

		//socket1->bindEvents();
		//socket->unbindEvents();

		//Log("debug") << "[TClientTest:" << this << "] onTCPSocketCreated: " << impl->referenceCount() << endl;		
	}

	void onPacketReceived(void* sender, DataPacket& packet) 
	{
		Log("debug") << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAA" << endl;	
		Log("debug") << "[ClientTest:" << this << "] onPacketReceived: " << packet.size << endl;	

	}

	void onPacketReceived1(void* sender, DataPacket& packet) 
	{
		Log("debug") << "WOOOOHOOOOOOOOOOOOOOOOOOOOOO" << endl;	
		Log("debug") << "[ClientTest:" << this << "] onPacketReceived1: " << packet.size << endl;	

	}
	*/
	
		/*
		*/


				
		/*
		

		//
		// The receiving peer...
		//
		responder = new UDPResponder();
		//initiator = new UDPInitiator(o, peerIP);

		//socket1 = new Net::TCPSocket();
		Client::Options o;
		o.serverAddr = Sourcey::Net::Address("127.0.0.1", 3478);
		o.timeout = 10000;
		o.username = "user";
		o.password = "E94rKjRkC3nqyj8UvoYW";
			
		
		Client c(o);
		//c.StateChanged += delegate(this, &TCPInitiator::onStateChanged);
		//c.addPermission(_peerIP);	
		c.initiate();

		//socket = new Net::TCPSocket();
		socket.registerPacketType<STUN::Message>(1);
		socket.attach(packetDelegate<ClientTest, STUN::Message>(this, &ClientTest::onSTUNMessageReceived, 0));
		//static_cast<Net::TCPSocket&>(socket).bind(Net::Address("127.0.0.1", 4000));

		socket1.registerPacketType<STUN::Message>(1);
		socket1.attach(packetDelegate<ClientTest, STUN::Message>(this, &ClientTest::onSTUNMessageReceived, 0));
		static_cast<Net::TCPSocket&>(socket1).bind(Net::Address("127.0.0.1", 4001));			
		
		try {
			STUN::Message m;
			//socket.send(m, Net::Address("127.0.0.1", 4001));
			//socket.send(m, c.socket().localAddr());
			socket.send(m, socket1.localAddr());
		}
		catch (Exception& e) {
			Log("debug") << "[ClientTest:" << this << "] ERR: " << e.displayText() << endl;	
		}
		
		system("pause");
		*/
		/*
		STUN::Message request;
		request.setType(STUN::Message::DataIndication);
	
		STUN::XorPeerAddress* peerAttr = new STUN::XorPeerAddress;
		peerAttr->setFamily(1);
		peerAttr->setPort(100);
		peerAttr->setIP("127.1.1.1");
		request.add(peerAttr);
		
		const STUN::XorPeerAddress* peerAttr1 = request.get<STUN::XorPeerAddress>();
		if (!peerAttr1 || peerAttr1 && peerAttr1->family() != 1) {
			assert(false);
		}

		Log("debug") << "STUN::Message: " << request.toString() << endl;
		request.dump(Log("debug"));
		system("pause");

		
		Log("debug") << "TURN: TCPInitiator: " << peerAttr1->address().toString() << endl;
		
		Runner& runner = Runner::getDefault();
		TCPSocket socket;
		//TCPAsyncConnector* task = new TCPAsyncConnector(runner, socket, Net::Address("127.0.0.1", 3478), 1);
		//task->start();
		socket.connect( Net::Address("127.0.0.1", 3478));
		//return conn;		
		*/
	
	/*	
	
	
		
	
	void onSTUNMessageReceived(void* sender, STUN::Message& message) 
	{
		Log("debug") << "[ClientTest:" << this << "] onPacketReceived: " << message.toString() << endl;	

	}



protected:
	
	void startResponder(const Net::Address& relayedAddr) 
	{
	}

	void onAllocationCreated(const void* sender, STUN::MessagePacket& packet) 
	{
		Log("debug") << "TURN: Allocation Created" << endl;
		Log("debug") << "TURN: Peer connecting to relayed address: " << client->relayedAddr().toString() << endl;

		//std::vector<Net::IP> peerIPs;
		//peerIPs.push_back(Net::IP("127.0.0.1"));
		//client->sendCreatePermissionRequest(peerIPs);
		
		_peer = new Net::TCPSocket();
		_peer->bind(Net::Address("127.0.0.1", 0));
		_peer->DataReceived += delegate(this, &ClientTest::onPeerDataReceived);

		_timer.setStartInterval(0);
		_timer.setPeriodicInterval(2000);
		_timer.start(Poco::TimerCallback<ClientTest>(*this, &ClientTest::onDataTimer));

		client->sendDataIndication("client2peer", 11, _peer->localAddr());
		client->sendDataIndication("client2peer", 11, _peer->localAddr());
	}


	void onPeerDataReceived(const void* sender, Sourcey::STUN::BufferPacket& packet) {
		Log("debug") << "################ TURN: Peer Received Data: " << string(packet.buffer.bytes(), packet.buffer.length()) << endl;
		//_peer->send("peer2client", 11, client->relayedAddr());
		//_peer->send("peer2client", 11, packet.remoteAddr);
	}


	void onDataReceived(const void* sender, Sourcey::STUN::DataPacket& packet) {
		Log("debug") << "TURN: Client Received Data: " << string(packet.data, packet.size) << endl;
		//client->sendDataIndication("client2peer", 11, _peer->localAddr());
		//client->sendDataIndication("client2peer", 11, packet.remoteAddr);
	}


	void onDataTimer(Poco::Timer& timer) {
		client->sendDataIndication("client2peer", 11, _peer->localAddr());
	}

	virtual void onAllocationCreated(TURN::Client* client, TURN::Client* allocation)
	{
		Log("debug") << "####################### TURN: Client Allocation Created: " << allocation << endl;

		//std::vector<Net::IP> peerIPs;
		//peerIPs.push_back(Net::IP("127.0.0.1"));
		client->addPermission(Net::IP("127.0.0.1"));
		client->sendCreatePermission();
	}


	virtual void onAllocationRemoving(TURN::Client* client, TURN::Client* allocation)
	{
		Log("debug") << "####################### TURN: Client Allocation Deleted: " << allocation << endl;
	}


	virtual void onPermissionsCreated(TURN::Client* client, TURN::Client* allocation)
	{
		Log("debug") << "####################### TURN: Client Permissions Created: " << allocation << endl;
	}


	virtual void onDataReceived(TURN::Client* client, const char* data, int size)
	{
		Log("debug") << "####################### TURN: Client Received Data: " << string(data, size) << endl;
	}
	
	virtual void onAllocationFailed(Client* client, int errorCode)
	{		
		Log("debug") << "####################### TURN: Allocation Failed: " << errorCode << endl;
	}
	

	void onPacketReceived(void* sender, const STUN::Message& message, 
		const Net::Address& localAddr, const Net::Address& remoteAddr) 
	{
	}
	
private:
	Client* client;
	Net::TCPSocket* _peer;
	Poco::Timer _timer;
	*/

		//Server1 sev;
		//sev.start();); //

	//socket.registerPacketType<STUN::Message>(1);
	//socket.attach(packetDelegate<ClientTest, STUN::Message>(this, &ClientTest::onSTUNMessageReceived, 1));
	//socket.bind(o.listenAddr);
		
		/*
		static_cast<Net::TCPSocket&>(socket1).connect(Net::Address("127.0.0.1", 4001));		
		for (unsigned i = 0; i < 100; i++) {
			Log("debug") << "TURN: TCPInitiator sendddddddddddd" << endl;
			STUN::Message m;
			//socket.send(m, socket1.localAddr());
			socket1.send(m, Net::Address("127.0.0.1", 4001));
		}

		
		try	{
			Client::Options opts;
			opts.serverAddr = Sourcey::Net::Address("127.0.0.1", 3478);
			opts.timeout = 10000;
			opts.username = "user";
			opts.password = "E94rKjRkC3nqyj8UvoYW";
			
			for (unsigned i = 0; i < 100; i++) {
				Client* client = new Client(*this, opts);
				client->sendAllocateRequest();

				Net::TCPSocket* sock = new Net::TCPSocket();
				sock->bind(Net::Address("127.0.0.1", 0));
				sock->attach(packetDelegate<ClientTest, STUN::Message>(this, &ClientTest::onPacketReceived, 1));
				sock->detach(packetDelegate<ClientTest, STUN::Message>(this, &ClientTest::onPacketReceived, 1));

				delete sock;				
			}
			client = new Client(*this, opts);
			//client->attach(this);
			client->sendAllocateRequest();
			//client->AllocationCreated += delegate(this, &ClientTest::onAllocationCreated);
			//client->DataReceived += delegate(this, &ClientTest::onDataReceived);
			//client->sendAllocateRe quest();
			system("pause");
		try	{
		}
		catch (Poco::Exception& exc) {
			Log("error") << "TURN Client: " << exc.displayText() << std::endl;
		}

		return Application::EXIT_OK;
		*/