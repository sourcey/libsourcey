#include "scy/net/tcpserver.h"
#include "scy/net/tcpsocket.h"
#include "scy/logger.h"
#include "scy/util.h"

#include "Poco/Path.h"

#include <iostream>


using namespace std;
using namespace Sourcey;
using namespace Sourcey::Net;
//using namespace Poco;


// Detect Memory Leaks
#ifdef _DEBUG
#include "MemLeakDetect/MemLeakDetect.h"
#include "MemLeakDetect/MemLeakDetect.cpp"
CMemLeakDetect memLeakDetect;
#endif


namespace Sourcey {
namespace Net {
	

struct EchoServer
{
	Net::TCPPacketServer server;
	
	EchoServer(Net::Reactor& reactor, int port) : 
		server(reactor) 
	{
		Log("debug") << "[EchoServer:" << this << "] Creating: " << port << endl;	
		server.bind(Net::Address("127.0.0.1", port));
		server.SocketCreated += delegate(this, &EchoServer::onServerSocketCreated);
	}
	virtual ~EchoServer() 
	{
		Log("debug") << "[EchoServer:" << this << "] Destroying" << endl;
	}
	
	void onServerSocketCreated(void* sender, Net::TCPSocket& socket)
	{
		Log("debug") << "[EchoServer:" << this << "] TCP Socket Created: " << socket->peerAddress() << endl;
		//socket->StateChange += delegate(this, &Echos::onServerSocketStateChange);
		
		socket->registerPacketType<DataPacket>(1);
		socket->attach(packetDelegate(this, &EchoServer::onServerSocketRequestReceived));	
	}
	
	/*
	void onServerSocketStateChange(void* sender, Net::ClientState& state, const Net::ClientState&)
	{
		Log("debug") << "[EchoServer:" << this << "] Server Connection State Changed: " << state.toString() << endl;	

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

		Log("debug") << "[EchoServer:" << this << "] Packet Received: " << packet.size() << ": " << socket->peerAddress() << endl;	

		// echo the data back
		socket->send(packet);
		
		socket->close();
		socket->detach(packetDelegate(this, &EchoServer::onServerSocketRequestReceived));
		delete socket;
	}
};


struct EchoClient
{
	enum Result {
		None,
		Success,
		Failed
	};

	int id;
	Net::Reactor& reactor;
	Result		  result;
	TCPSocket	  socket;

	EchoClient(Net::Reactor& reactor, int id, int port) : 
		id(id),
		reactor(reactor), 
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
		Net::TCPSocket* socket = reinterpret_cast<Net::TCPSocket*>(sender);	

		Log("debug") << "########################## [EchoClient:" << this << "] Packet Received: " << packet.size() << ": " << socket->peerAddress() << endl;	
		//socket->detach(packetDelegate<EchoServer, DataPacket>(this, &EchoServer::onServerSocketRequestReceived));

		// echo the data back
		//socket->send(packet);
	}
};


} } //  namespace Sourcey::Net


int main(int argc, char** argv)
{
	//Poco::Path path(Poco::Path::current());
	//path.setFileName("TCPServerTest.log");
	//Logger::instance().add(new FileChannel("debug", path.toString(), TraceLevel));
	Sourcey::Logger::instance().add(new Sourcey::ConsoleChannel("debug", Sourcey::TraceLevel));
	
	try	{
		Net::Reactor reactor;
		Log("debug") << "Raising server..." << endl;	
		EchoServer srv(reactor, 1337);	
		Log("debug") << "Connecting to server..." << endl;	
		EchoClient client(reactor, 0, 1337);
		Log("debug") << "Waiting for completion..." << endl;	
		system("pause");
	} 
	catch (Poco::Exception& exc) {
		Log("error") << "TEST: Error: " << exc.displayText() << endl;
	}
	
	Logger::shutdown();

	system("pause");

	return 0;
}









	/*
	//Sourcey::Net::EchoClient app;
	//app.run();
	*/
	
	/*
	void onTCPSocketCreated(void* sender, Net::TCPSocket& socket)
	{
		Log("debug") << "[TEchoClient:" << this << "] onTCPSocketCreated" << endl;		
		//socket->bindEvents();
		socket->attach(packetDelegate<EchoClient, DataPacket>(this, &EchoClient::onPacketReceived));
		//socket->unbindEvents();
		//socket->unbindEvents();

		Net::TCPSocket* socket1 = new Net::TCPSocket(socket->impl());
		//socket1->bindEvents();
		socket->attach(packetDelegate<EchoClient, DataPacket>(this, &EchoClient::onPacketReceived));
		socket1->attach(packetDelegate<EchoClient, DataPacket>(this, &EchoClient::onPacketReceived1));
		
		socket->transferTo(socket1);

		//Poco::Net::SocketImpl* impl = socket->impl().impl();
		//Poco::Net::SocketImpl* impl1 = socket1->impl().impl();
		//assert(impl == impl1);
		//impl->duplicate();

		//delete socket;

		//socket1->bindEvents();
		//socket->unbindEvents();

		//Log("debug") << "[TEchoClient:" << this << "] onTCPSocketCreated: " << impl->referenceCount() << endl;		
	}

	void onPacketReceived(void* sender, DataPacket& packet) 
	{
		Log("debug") << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAA" << endl;	
		Log("debug") << "[EchoClient:" << this << "] onPacketReceived: " << packet.size << endl;	

	}

	void onPacketReceived1(void* sender, DataPacket& packet) 
	{
		Log("debug") << "WOOOOHOOOOOOOOOOOOOOOOOOOOOO" << endl;	
		Log("debug") << "[EchoClient:" << this << "] onPacketReceived1: " << packet.size << endl;	

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
		//c.StateChange += delegate(this, &TCPInitiator::onStateChange);
		//c.addPermission(_peerIP);	
		c.initiate();

		//socket = new Net::TCPSocket();
		socket.registerPacketType<STUN::Message>(1);
		socket.attach(packetDelegate<EchoClient, STUN::Message>(this, &EchoClient::onSTUNMessageReceived, 0));
		//static_cast<Net::TCPSocket&>(socket).bind(Net::Address("127.0.0.1", 4000));

		socket1.registerPacketType<STUN::Message>(1);
		socket1.attach(packetDelegate<EchoClient, STUN::Message>(this, &EchoClient::onSTUNMessageReceived, 0));
		static_cast<Net::TCPSocket&>(socket1).bind(Net::Address("127.0.0.1", 4001));			
		
		try {
			STUN::Message m;
			//socket.send(m, Net::Address("127.0.0.1", 4001));
			//socket.send(m, c.socket().localAddr());
			socket.send(m, socket1.localAddr());
		}
		catch (Exception& e) {
			Log("debug") << "[EchoClient:" << this << "] ERR: " << e.displayText() << endl;	
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

		
		Log("debug") << "Net: TCPInitiator: " << peerAttr1->address().toString() << endl;
		
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
		Log("debug") << "[EchoClient:" << this << "] onPacketReceived: " << message.toString() << endl;	

	}



protected:
	
	void startResponder(const Net::Address& relayedAddr) 
	{
	}

	void onAllocationCreated(const void* sender, STUN::MessagePacket& packet) 
	{
		Log("debug") << "Net: Allocation Created" << endl;
		Log("debug") << "Net: Peer connecting to relayed address: " << client->relayedAddr().toString() << endl;

		//vector<Net::IP> peerIPs;
		//peerIPs.push_back(Net::IP("127.0.0.1"));
		//client->sendCreatePermissionRequest(peerIPs);
		
		_peer = new Net::TCPSocket();
		_peer->bind(Net::Address("127.0.0.1", 0));
		_peer->DataReceived += delegate(this, &EchoClient::onPeerDataReceived);

		_timer.setStartInterval(0);
		_timer.setPeriodicInterval(2000);
		_timer.start(Poco::TimerCallback<EchoClient>(*this, &EchoClient::onDataTimer));

		client->sendDataIndication("client2peer", 11, _peer->localAddr());
		client->sendDataIndication("client2peer", 11, _peer->localAddr());
	}


	void onPeerDataReceived(const void* sender, Sourcey::STUN::BufferPacket& packet) {
		Log("debug") << "################ Net: Peer Received Data: " << string(packet.buffer.bytes(), packet.buffer.length()) << endl;
		//_peer->send("peer2client", 11, client->relayedAddr());
		//_peer->send("peer2client", 11, packet.remoteAddr);
	}


	void onDataReceived(const void* sender, Sourcey::STUN::DataPacket& packet) {
		Log("debug") << "Net: Client Received Data: " << string(packet.data, packet.size) << endl;
		//client->sendDataIndication("client2peer", 11, _peer->localAddr());
		//client->sendDataIndication("client2peer", 11, packet.remoteAddr);
	}


	void onDataTimer(Poco::Timer& timer) {
		client->sendDataIndication("client2peer", 11, _peer->localAddr());
	}

	virtual void onAllocationCreated(Net::Client* client, Net::Client* allocation)
	{
		Log("debug") << "####################### Net: Client Allocation Created: " << allocation << endl;

		//vector<Net::IP> peerIPs;
		//peerIPs.push_back(Net::IP("127.0.0.1"));
		client->addPermission(Net::IP("127.0.0.1"));
		client->sendCreatePermission();
	}


	virtual void onAllocationRemoving(Net::Client* client, Net::Client* allocation)
	{
		Log("debug") << "####################### Net: Client Allocation Deleted: " << allocation << endl;
	}


	virtual void onPermissionsCreated(Net::Client* client, Net::Client* allocation)
	{
		Log("debug") << "####################### Net: Client Permissions Created: " << allocation << endl;
	}


	virtual void onDataReceived(Net::Client* client, const char* data, int size)
	{
		Log("debug") << "####################### Net: Client Received Data: " << string(data, size) << endl;
	}
	
	virtual void onAllocationFailed(Client* client, int errorCode)
	{		
		Log("debug") << "####################### Net: Allocation Failed: " << errorCode << endl;
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
	//socket.attach(packetDelegate<EchoClient, STUN::Message>(this, &EchoClient::onSTUNMessageReceived, 1));
	//socket.bind(o.listenAddr);
		
		/*
		static_cast<Net::TCPSocket&>(socket1).connect(Net::Address("127.0.0.1", 4001));		
		for (unsigned i = 0; i < 100; i++) {
			Log("debug") << "Net: TCPInitiator sendddddddddddd" << endl;
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
				sock->attach(packetDelegate<EchoClient, STUN::Message>(this, &EchoClient::onPacketReceived, 1));
				sock->detach(packetDelegate<EchoClient, STUN::Message>(this, &EchoClient::onPacketReceived, 1));

				delete sock;				
			}
			client = new Client(*this, opts);
			//client->attach(this);
			client->sendAllocateRequest();
			//client->AllocationCreated += delegate(this, &EchoClient::onAllocationCreated);
			//client->DataReceived += delegate(this, &EchoClient::onDataReceived);
			//client->sendAllocateRe quest();
			system("pause");
		try	{
		}
		catch (Poco::Exception& exc) {
			Log("error") << "Net Client: " << exc.displayText() << endl;
		}

		return Application::EXIT_OK;
		*/