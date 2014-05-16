#include "UDPInitiator.h"
#include "UDPResponder.h"

#include "TCPInitiator.h"
#include "TCPResponder.h"

#include "scy/application.h"
#include "scy/turn/client/client.h"
#include "scy/turn/server/server.h"
#include "scy/logger.h"
#include "scy/util.h"

#include <iostream>


using namespace std;
using namespace scy;
using namespace scy::net;
using namespace scy::turn;

/*
// Detect Memory Leaks
#ifdef _DEBUG
#include "MemLeakDetect/MemLeakDetect.h"
#include "MemLeakDetect/MemLeakDetect.cpp"
CMemLeakDetect memLeakDetect;
#endif
*/


namespace scy {
namespace turn {
	

struct TestServer: public turn::ServerObserver
{	
	turn::Server* server;

	TestServer() : server(new turn::Server(*this)) {}	
	virtual ~TestServer() { delete server; }	
		
	void run(const turn::ServerOptions& opts) 
	{
		server->options() = opts;
		server->start();
	}
	
	void stop() 
	{
		server->stop();
	}

	AuthenticationState authenticateRequest(Server* server, const Request& request)
	{
		return Authorized;
	}	

	void onServerAllocationCreated(Server* server, IAllocation* alloc) 
	{
		DebugLS(this) << "Allocation created" << endl;
	}

	void onServerAllocationRemoved(Server* server, IAllocation* alloc)
	{		
		DebugLS(this) << "Allocation removed" << endl;
	}
};


struct ClientTest
	//
	// The initiating client...
{
	enum Result {
		None,
		Success,
		Failed
	};

	//Signal<Result&>	TestComplete;

	int id;
	Result result;
#if TEST_TCP
	TCPInitiator* initiator;
	TCPResponder* responder;
	ClientTest(int id, const Client::Options& opts) : 
		id(id),
		result(None),
		initiator(new TCPInitiator(id, opts)), 
		responder(new TCPResponder(id))
		{}	
#else
	UDPInitiator* initiator;
	UDPResponder* responder;
	ClientTest(int id, const Client::Options& opts) : 
		id(id),
		result(None),
		initiator(new UDPInitiator(id, opts)), 
		responder(new UDPResponder(id)) {}	
#endif

	~ClientTest() {
		if (initiator)
			delete initiator;
		if (responder)
			delete responder;
	}

	void run() 
	{				
		initiator->AllocationCreated += sdelegate(this, &ClientTest::onInitiatorAllocationCreated);

		// TODO: Use STUN binding request to get IP
		initiator->initiate(TURN_AUTHORIZE_PEER_IP);
	}
	
	void onInitiatorAllocationCreated(void* sender) //, turn::Client& client
	{
		DebugL << "Initiator allocation created" << endl;

		// Start the responder when the allocation is created
		responder->start(initiator->client.relayedAddress());
		
#if !TEST_TCP
		// Set the local responder address for UDP send indications
		initiator->responderAddress = net::Address(TURN_AUTHORIZE_PEER_IP, responder->socket.address().port());
#endif
	}
	
	void onTestComplete(void* sender, bool success)
	{
		DebugL << "Test complete: " << success << endl;
		result = success ? Success : Failed;
		//TestComplete.emit(this, result);
	}
};


struct ClientTestRunner
{
	int nTimes;	
	int nComplete;
	int nSucceeded;
	
	std::vector<turn::ClientTest*> tests;
	
	ClientTestRunner() : nTimes(0), nComplete(0), nSucceeded(0) {}
	~ClientTestRunner() { 		
		//Timer::getDefault().stop(TimerCallback<ClientTestRunner>(this, &ClientTestRunner::onTimeout));
		util::clearVector(tests);  
	}

	void run(const Client::Options& o, int times, int timeout) 
	{	
		//if (timeout)
		//	Timer::getDefault().start(TimerCallback<ClientTestRunner>(this, &ClientTestRunner::onTimeout, timeout));

		nTimes = times;
		for (int i = 0; i < nTimes; i++) {
			auto client = new turn::ClientTest(i, o); //, reactor, runner
			client->initiator->TestComplete += sdelegate(this, &ClientTestRunner::onTestComplete);
			client->run();
			tests.push_back(client);
		}

		//done.wait();
	}	

	//void onTimeout(TimerCallback<ClientTestRunner>& timer)
	//{
		// took too long...
		//done.set();
	//}
	
	void onTestComplete(void* sender, bool result)
	{
		DebugL << "ClientTestRunner: TestComplete" << endl;

		nComplete++;
		if (result)
			nSucceeded++;
		//if (nComplete == nTimes)	
		//	done.set();
		print(cout);
	}

	void print(ostream& ost) 
	{	
		ost << "#################Test results:" 
			<< "\n\tTimes: " << nTimes
			<< "\n\tComplete: " << nComplete
			<< "\n\tSucceeded: " << nSucceeded
			<< endl;
	}
};


struct TestTCPClientObserver: public TCPClientObserver
{
	void onClientStateChange(turn::Client& client, turn::ClientState& state, const turn::ClientState&) 
	{
		DebugLS(this) << "State change: " << state.toString() << endl;
	}
	
	void onTimer(TCPClient& client)
	{
		DebugLS(this) << "onTimer" << endl;
	}

	void onRelayConnectionCreated(TCPClient& client, const net::TCPSocket::Ptr& socket, const net::Address& peerAddr) //UInt32 connectionID, 
	{
		DebugLS(this) << "Relay Connection Created: " << peerAddr << endl;
	}

	void onRelayConnectionClosed(TCPClient& client, const net::TCPSocket::Ptr& socket, const net::Address& peerAddr) 
	{
		DebugLS(this) << "Connection Closed" << endl;
	}

	void onRelayConnectionDataReceived(turn::Client& client, const char* data, std::size_t size, const net::Address& peerAddr)
	{
		DebugLS(this) << "Received Data: " << std::string(data, size) <<  ": " << peerAddr << endl;
	}
	
	void onAllocationPermissionsCreated(turn::Client& client, const turn::PermissionList& permissions)
	{
		DebugLS(this) << "Permissions Created" << endl;
	}
};


} } //  namespace scy::turn

	
/*
static void onKillSignal(uv_signal_t *req, int signum)
{
	((turn::TestServer*)req->data)->stop();
	uv_signal_stop(req);
}
*/


int main(int argc, char** argv)
{
#ifdef _MSC_VER
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	Logger::instance().add(new ConsoleChannel("debug", LTrace));	
	//Logger::instance().setWriter(new AsyncLogWriter);	
	{
		Application app;
		try	{
#if RAISE_LOCAL_SERVER
			DebugL << "Running Test Server" << endl;
			turn::ServerOptions so;	
			so.software                         = "Sourcey STUN/TURN Server [rfc5766]";
			so.realm                            = "sourcey.com";
			so.allocationDefaultLifetime        = 1 * 60 * 1000;
			so.allocationMaxLifetime            = 15 * 60 * 1000;
			//so.allocationDefaultLifetime        = 1 * 60 * 1000; // 1 min
			//so.allocationMaxLifetime            = 5 * 1000; // 5 seccs
			so.timerInterval					= 5 * 1000; // 5 seccs
			so.listenAddr                       = net::Address("127.0.0.1", 3478);
			so.enableUDP						= false;

			DebugL << "Binding Test Server" << endl;
			turn::TestServer srv;
			DebugL << "Binding Test Server: 1" << endl;
			srv.run(so);
			DebugL << "Binding Test Server: OK" << endl;
#endif

			//
			// Initialize clients
			{
				DebugL << "Running Client Tests" << endl;	
				turn::Client::Options co;
				co.serverAddr = net::Address(TURN_SERVER_IP, TURN_SERVER_PORT);
				co.username = TURN_SERVER_USERNAME;
				co.password = TURN_SERVER_PASSWORD;
				co.lifetime  = 120 * 1000; // 1 minute
				co.timeout = 10 * 1000;
				co.timerInterval = 3 * 1000;	

				turn::ClientTestRunner test;
				test.run(co, 1, 10000);
				DebugL << "Running Client Tests: OK" << endl;
				app.waitForShutdown();
				test.print(cout);
			}
		} 
		catch (std::exception& exc) {
			ErrorL << exc.what() << endl;
		}
	
		DebugL << "Finalizing" << endl;
		app.finalize();
		DebugL << "Finalizing: OK" << endl;
	}
	scy::Logger::destroy();
	return 0;
}


		
		/*
		uv_signal_t sig;
		sig.data = &srv;
		uv_signal_init(uv_default_loop(), &sig);
		uv_signal_start(&sig, ::onKillSignal, SIGINT);
	
		loop.run();		
		*/

/*
		
		//
		// Init initiator
		turn::TCPInitiator initiator(0);
		initiator.initiate(co, std::string("127.0.0.1"));				
		//TCPResponder responder(0);
		loop.run();		
		TCPClient client;
		{
			//net::Socket* socket = new net::Socket(NULL);
			//socket->assign(new net::TCPSocket, false);
			//net::TCPSocket* base = new net::TCPSocket;
			//net::TCPSocket* base = new net::TCPSocket;
			//turn::TCPClient* client = new turn::TCPClient(obs, co);
			//turn::Client* client = new turn::Client(obs, co);
			TestSocketHeap* test = new TestSocketHeap();
			//turn::TCPInitiator initiator(0);
			loop.run();	
			delete test;
			//delete client;
			//delete base;
			//delete socket;
		}
		
		//TCPInitiator* initiator = new TCPInitiator(0);
		//TCPResponder* responder = new TCPResponder(0);
		
		//loop.run();		

		//if (initiator)
		//	delete initiator;
		//if (responder)
		//	delete responder;
		
		//
		// Init initiator
		turn::ClientTest test(0);
		test.run(co); // std::string("127.0.0.1"))
		loop.run();
		*/





	/*
	//scy::turn::ClientTest app;
	//app.run();
	*/
	
	/*
	void onTCPSocketCreated(void* sender, Net::TCPSocket& socket)
	{
		DebugL << "[TClientTest:" << this << "] onTCPSocketCreated" << endl;		
		//socket->bindEvents();
		socket->attach(packetDelegate<ClientTest, RawPacket>(this, &ClientTest::onPacketReceived));
		//socket->unbindEvents();
		//socket->unbindEvents();

		Net::TCPSocket* socket1 = new Net::TCPSocket(socket->impl());
		//socket1->bindEvents();
		socket->attach(packetDelegate<ClientTest, RawPacket>(this, &ClientTest::onPacketReceived));
		socket1->attach(packetDelegate<ClientTest, RawPacket>(this, &ClientTest::onPacketReceived1));
		
		socket->transferTo(socket1);

		//Poco::Net::SocketImpl* impl = socket->impl().impl();
		//Poco::Net::SocketImpl* impl1 = socket1->impl().impl();
		//assert(impl == impl1);
		//impl->duplicate();

		//delete socket;

		//socket1->bindEvents();
		//socket->unbindEvents();

		//DebugL << "[TClientTest:" << this << "] onTCPSocketCreated: " << impl->refCount() << endl;		
	}

	void onPacketReceived(void* sender, RawPacket& packet) 
	{
		DebugL << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAA" << endl;	
		DebugL << "[ClientTest:" << this << "] onPacketReceived: " << packet.size << endl;	

	}

	void onPacketReceived1(void* sender, RawPacket& packet) 
	{
		DebugL << "WOOOOHOOOOOOOOOOOOOOOOOOOOOO" << endl;	
		DebugL << "[ClientTest:" << this << "] onPacketReceived1: " << packet.size << endl;	

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
		o.serverAddr = scy::net::Address("127.0.0.1", 3478);
		o.timeout = 10000;
		o.username = "user";
		o.password = "E94rKjRkC3nqyj8UvoYW";
			
		
		Client c(o);
		//c.StateChange += sdelegate(this, &TCPInitiator::onStateChange);
		//c.addPermission(_peerIP);	
		c.initiate();

		//socket = new Net::TCPSocket();
		socket.registerPacketType<stun::Message>(1);
		socket.attach(packetDelegate<ClientTest, stun::Message>(this, &ClientTest::onSTUNMessageReceived, 0));
		//static_cast<Net::TCPSocket&>(socket).bind(net::Address("127.0.0.1", 4000));

		socket1.registerPacketType<stun::Message>(1);
		socket1.attach(packetDelegate<ClientTest, stun::Message>(this, &ClientTest::onSTUNMessageReceived, 0));
		static_cast<Net::TCPSocket&>(socket1).bind(net::Address("127.0.0.1", 4001));			
		
		try {
			stun::Message m;
			//socket.send(m, net::Address("127.0.0.1", 4001));
			//socket.send(m, c.socket().localAddr());
			socket.send(m, socket1.localAddr());
		}
		catch (Exception& e) {
			DebugL << "[ClientTest:" << this << "] ERR: " << e.message() << endl;	
		}
		
		//util::pause();
		*/
		/*
		stun::Message request;
		request.setType(stun::Message::DataIndication);
	
		stun::XorPeerAddress* peerAttr = new stun::XorPeerAddress;
		peerAttr->setFamily(1);
		peerAttr->setPort(100);
		peerAttr->setIP("127.1.1.1");
		request.add(peerAttr);
		
		const stun::XorPeerAddress* peerAttr1 = request.get<stun::XorPeerAddress>();
		if (!peerAttr1 || peerAttr1 && peerAttr1->family() != 1) {
			assert(false);
		}

		DebugL << "stun::Message: " << request.toString() << endl;
		request.dump(DebugL);
		//util::pause();

		
		DebugL << "TURN: TCPInitiator: " << peerAttr1->address() << endl;
		
		Runner& runner = Runner::getDefault();
		TCPSocket socket;
		//TCPAsyncConnector* task = new TCPAsyncConnector(runner, socket, net::Address("127.0.0.1", 3478), 1);
		//task->start();
		socket.connect( net::Address("127.0.0.1", 3478));
		//return conn;		
		*/
	
	/*	
	
	
		
	
	void onSTUNMessageReceived(void* sender, stun::Message& message) 
	{
		DebugL << "[ClientTest:" << this << "] onPacketReceived: " << message.toString() << endl;	

	}



protected:
	
	void startResponder(const net::Address& relayedAddr) 
	{
	}

	void onAllocationCreated(const void* sender, stun::MessagePacket& packet) 
	{
		DebugL << "TURN: Allocation Created" << endl;
		DebugL << "TURN: Peer connecting to relayed address: " << client->relayedAddr().toString() << endl;

		//vector<Net::IP> peerIPs;
		//peerIPs.push_back(Net::IP("127.0.0.1"));
		//client->sendCreatePermissionRequest(peerIPs);
		
		_peer = new Net::TCPSocket();
		_peer->bind(net::Address("127.0.0.1", 0));
		_peer->DataReceived += sdelegate(this, &ClientTest::onRelayConnectionDataReceived);

		_timer.setStartInterval(0);
		_timer.setPeriodicInterval(2000);
		_timer.start(Poco::TimerCallback<ClientTest>(*this, &ClientTest::onDataTimer));

		client->sendDataIndication("client2peer", 11, _peer->localAddr());
		client->sendDataIndication("client2peer", 11, _peer->localAddr());
	}


	void onRelayConnectionDataReceived(const void* sender, scy::stun::BufferPacket& packet) {
		DebugL << "################ TURN: Peer Received Data: " << std::string(packet.buffer.bytes(), packet.buffer.available()) << endl;
		//_peer->send("peer2client", 11, client->relayedAddr());
		//_peer->send("peer2client", 11, packet.remoteAddr);
	}


	void onDataReceived(const void* sender, scy::stun::RawPacket& packet) {
		DebugL << "TURN: Client Received Data: " << std::string(packet.data, packet.size) << endl;
		//client->sendDataIndication("client2peer", 11, _peer->localAddr());
		//client->sendDataIndication("client2peer", 11, packet.remoteAddr);
	}


	void onDataTimer(Poco::Timer& timer) {
		client->sendDataIndication("client2peer", 11, _peer->localAddr());
	}

	virtual void onAllocationCreated(turn::Client* client, turn::Client* allocation)
	{
		DebugL << "######TURN: Client Allocation Created: " << allocation << endl;

		//vector<Net::IP> peerIPs;
		//peerIPs.push_back(Net::IP("127.0.0.1"));
		client->addPermission(Net::IP("127.0.0.1"));
		client->sendCreatePermission();
	}


	virtual void onAllocationRemoving(turn::Client* client, turn::Client* allocation)
	{
		DebugL << "######TURN: Client Allocation Deleted: " << allocation << endl;
	}


	virtual void onPermissionsCreated(turn::Client* client, turn::Client* allocation)
	{
		DebugL << "######TURN: Client Permissions Created: " << allocation << endl;
	}


	virtual void onDataReceived(turn::Client* client, const char* data, int size)
	{
		DebugL << "######TURN: Client Received Data: " << std::string(data, size) << endl;
	}
	
	virtual void onAllocationFailed(Client* client, int errorCode)
	{		
		DebugL << "######TURN: Allocation Failed: " << errorCode << endl;
	}
	

	void onPacketReceived(void* sender, const stun::Message& message, 
		const net::Address& localAddr, const net::Address& remoteAddr) 
	{
	}
	
private:
	Client* client;
	Net::TCPSocket* _peer;
	Poco::Timer _timer;
	*/

		//Server1 sev;
		//sev.start();); //

	//socket.registerPacketType<stun::Message>(1);
	//socket.attach(packetDelegate<ClientTest, stun::Message>(this, &ClientTest::onSTUNMessageReceived, 1));
	//socket.bind(o.listenAddr);
		
		/*
		static_cast<Net::TCPSocket&>(socket1).connect(net::Address("127.0.0.1", 4001));		
		for (unsigned i = 0; i < 100; i++) {
			DebugL << "TURN: TCPInitiator sendddddddddddd" << endl;
			stun::Message m;
			//socket.send(m, socket1.localAddr());
			socket1.send(m, net::Address("127.0.0.1", 4001));
		}

		
		try	{
			Client::Options opts;
			opts.serverAddr = scy::net::Address("127.0.0.1", 3478);
			opts.timeout = 10000;
			opts.username = "user";
			opts.password = "E94rKjRkC3nqyj8UvoYW";
			
			for (unsigned i = 0; i < 100; i++) {
				Client* client = new Client(*this, opts);
				client->sendAllocateRequest();

				Net::TCPSocket* sock = new Net::TCPSocket();
				sock->bind(net::Address("127.0.0.1", 0));
				sock->attach(packetDelegate<ClientTest, stun::Message>(this, &ClientTest::onPacketReceived, 1));
				sock->detach(packetDelegate<ClientTest, stun::Message>(this, &ClientTest::onPacketReceived, 1));

				delete sock;				
			}
			client = new Client(*this, opts);
			//client->attach(this);
			client->sendAllocateRequest();
			//client->AllocationCreated += sdelegate(this, &ClientTest::onAllocationCreated);
			//client->DataReceived += sdelegate(this, &ClientTest::onDataReceived);
			//client->sendAllocateRe quest();
			//util::pause();
		try	{
		}
		catch (std::exception& exc) {
			Log("error") << "TURN Client: " << exc.what() << endl;
		}

		return Application::EXIT_OK;
		*/