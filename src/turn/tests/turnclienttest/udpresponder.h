#ifndef TURN_UDPresponder_TEST_H
#define TURN_UDPresponder_TEST_H


#include "scy/net/udpsocket.h"
#include "scy/time.h"
#include "scy/timer.h"
#include "scy/logger.h"


using namespace std;


namespace scy {
namespace turn {


class UDPResponder: public net::SocketAdapter
{
	
public:
	int id;
	net::Address relayedAddr;
	net::UDPSocket socket;	
	Timer timer;

	UDPResponder(int id) : 
		id(id)
	{
		debugL("UDPResponder", this) << id << ": Creating" << endl;
		net::SocketAdapter::socket = &socket;
		socket.setAdapter(this);
	}

	virtual ~UDPResponder() 
	{ 
		debugL("UDPResponder", this) << id << ": Destroying" << endl;
		socket.setAdapter(nullptr);
		stop(); 
	}

	void start(const net::Address& relayedAddr) 
	{		
		debugL("UDPResponder", this) << id << ": Starting on: " << relayedAddr << endl;
		
		try	{
			this->relayedAddr = relayedAddr;
			
			socket.bind(net::Address("0.0.0.0", 0));
			socket.connect(relayedAddr);
		}
		catch (std::exception& exc) {
			errorL("UDPResponder", this) << id << ": ERROR: " << exc.what() << endl;
			assert(false);
		}
	}
	
	void stop() 
	{	
		timer.stop();
		socket.close();
	}
	
	void onSocketConnect() 
	{
		// Send some early media to client
		sendLatencyCheck();

		// Start the send timer
		timer.Timeout += delegate(this, &UDPResponder::onSendTimer);
		timer.start(1000, 100);
	}

	
	void sendLatencyCheck()
	{		
		std::string payload;
		
		// Send the unix ticks milisecond for checking latency
		//payload.append(":");
		payload.append(util::itostr(time::ticks()));
		//payload.append(":");

		// Send a large packets to test throttling
		//payload.append(65536, 'x');
		payload.append(30000, 'x');
		//payload.append(1024, 'x');

		// Send it
		socket.send(payload.c_str(), payload.length());

		/*
		// Send a large packets to test throttling
		//std::string payload(65536, 'x');
		std::string payload(10000, 'x');
		assert(payload.length() == 10000);
		socket.send(payload.c_str(), payload.length());

		// Send the unix ticks milisecond for checking RTT
		payload.assign(util::itostr(time::ticks()));
		socket.send(payload.c_str(), payload.length());
		*/
	}

	void onSendTimer(void*)
	{
		sendLatencyCheck();
	}
	
	void onSocketRecv(const MutableBuffer& buf, const net::Address& peerAddr) //net::SocketPacket& packet) 
	{
		std::string payload(bufferCast<const char*>(buf), buf.size());
		traceL("UDPResponder", this) << id << ": On recv: " << peerAddr << ": " << payload << std::endl;

		// Echo back to client
		socket.send(payload.c_str(), payload.size());
	}

	void onSocketError(const Error& error) 
	{
		traceL("UDPResponder", this) << id << ": On error: " << error.message << std::endl;
	}

	void onSocketClose() 
	{
		traceL("UDPResponder", this) << id << ": On close" << std::endl;
		stop();
	}
};


} } //  namespace scy::turn


#endif // TURN_UDPresponder_TEST_H


	
	/*
	int id;
	net::Address relayedAddr;
	Net::UDPPacketSocket* socket;
	Net::Reactor& reactor;
	Runner& runner;

public:
	UDPResponder(int id): //, Net::Reactor& reactor, Runner& runner
		id(id), socket(NULL) {} //, reactor(reactor), runner(runner)
	virtual ~UDPResponder() {}

	void start(const net::Address& relayedAddr) 
	{		
		debugL() << "UDPResponder: " << id << ": Starting on: " << relayedAddr << endl;
		
		try	{
			assert(!socket);
			this->relayedAddr = relayedAddr;
			socket = new Net::UDPPacketSocket(reactor, runner);
			socket->attach(packetDelegate<UDPResponder, stun::Message>(this, &UDPResponder::onSTUNPacketReceived, 101));
			socket->attach(packetDelegate<UDPResponder, RawPacket>(this, &UDPResponder::onRawPacketReceived, 102));
			socket->connect(relayedAddr);

			Timer::getDefault().start(TimerCallback<UDPResponder>(this, &UDPResponder::onTimer, 2000, 2000));
		}
		catch (std::exception& exc) {
			Log("error") << "UDPResponder: " << id << ": ERROR: " << exc.what() << endl;
		}
	}
	
	void stop() 
	{		
		Timer::getDefault().stop(TimerCallback<UDPResponder>(this, &UDPResponder::onTimer));
		if (socket) {	
			socket->detach(packetDelegate<UDPResponder, stun::Message>(this, &UDPResponder::onSTUNPacketReceived, 101));
			socket->detach(packetDelegate<UDPResponder, RawPacket>(this, &UDPResponder::onRawPacketReceived, 102));
			delete socket;
			socket = NULL;
		}
	}
	
protected:	

	void onRelayConnectionDataReceived(turn::Client& client, const char* data, int size, const net::Address& peerAddr)
	{
		debugL() << "UDPInitiator: " << id << ": Received Data: " << std::string(data, size) << endl;

		// echo it back...
		client.sendData(data, size, peerAddr);
	}
	
	void onPermissionsCreated(turn::Client& client, const turn::PermissionList& permissions)
	{
		debugL() << "UDPInitiator: " << id << ": Permissions Created" << endl;
	}

	void onTimer(TimerCallback<UDPResponder>& timer)
	{
		debugL() << "UDPResponder: " << id << ": On Timer: Sending data to: " << relayedAddr << endl;
		assert(socket);
		socket->send("hey baby!", 9, relayedAddr);
	}
	
	void onRawPacketReceived(void* sender, RawPacket& packet)
	{
		// echoed from client...
		debugL() << "UDPResponder: " << id << ": Data Packet Received: " << packet.size() << endl;
	}

	void onSTUNPacketReceived(void* sender, stun::Message& message)
	{
		assert(false); // responder is unaware of STUN
		debugL() << "UDPResponder: " << id << ": STUN Packet Received: " << message.toString() << endl;
	}
	*/

	/* //,  // << remoteAddr << endl;
		//const net::Address& localAddr, const net::Address& remoteAddr) 
	 //,  // << remoteAddr << endl;//: public scy::turn::ClientObserver
		//const net::Address& localAddr, const net::Address& remoteAddr) 
protected:	
	  
		
	void setRelayedAddr()
	{
	}
	virtual void onAllocationCreated(turn::Client* client, turn::Client* allocation)
	{
		debugL() << "UDPResponder: " << id << ": Allocation Created: " << transaction << endl;
		client->sendCreatePermissionRequest(_peerIP);
	}


	virtual void onAllocationRemoving(turn::Client* client, turn::Client* allocation)
	{
		debugL() << "UDPResponder: " << id << ": Allocation Deleted: " << transaction << endl;
	}


	virtual void onPermissionsCreated(turn::Client* client, turn::Client* allocation)
	{
		debugL() << "UDPResponder: " << id << ": Permissions Created: " << transaction << endl;
	}


	virtual void onDataReceived(turn::Client* client, const char* data, int size)
	{
		debugL() << "UDPResponder: " << id << ": Received Data: " << std::string(data, size) << endl;
	}
	
	virtual void onAllocationFailed(Client* client, int errorCode)
	{		
		debugL() << "UDPResponder: " << id << ": Allocation Failed: " << errorCode << endl;
	}
	
	void onTransactionResponse(Client* client, stun::Transaction* transaction)
	{		
		debugL() << "TURN Initiator: Transaction Response: " << transaction << endl;
	}	
	
	void onAllocationPermissionsCreated(Client* client, Client* allocation)
	{
		debugL() << "TURN Initiator: Permissions Created: " << allocation << endl;
	}	
	*/

		/*
		try	{
			Client::Options opts;
			opts.serverAddr = scy::net::Address("127.0.0.1", 3478);
			opts.timeout = 10000;
			opts.username = "user";
			opts.password = "E94rKjRkC3nqyj8UvoYW";

			client = new Client(*this, opts);
			client->sendAllocateRequest();

			//util::pause();
		}
		catch (std::exception& exc) {
			Log("error") << "UDPResponder: " << id << ": Error: " << exc.what() << endl;
		}
		*/