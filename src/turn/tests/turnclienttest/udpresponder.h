#ifndef TURN_UDPresponder_TEST_H
#define TURN_UDPresponder_TEST_H


#include "scy/net/udpsocket.h"
#include "scy/timer.h"
#include "scy/logger.h"


using namespace std;


namespace scy {
namespace turn {


/*
class UDPResponder
{
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
};
*/


} } //  namespace scy::turn


#endif // TURN_UDPresponder_TEST_H


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
		debugL() << "UDPResponder: " << id << ": Received Data: " << string(data, size) << endl;
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