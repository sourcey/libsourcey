#ifndef TURN_UDPresponder_TEST_H
#define TURN_UDPresponder_TEST_H


#include "Sourcey/Net/UDPSocket.h"
#include "Sourcey/Logger.h"


using namespace std;


namespace Sourcey {
namespace TURN {


class UDPResponder
{
	int id;
	Net::Address relayedAddr;
	Net::UDPSocket* socket;

public:
	UDPResponder(int id = 0): 
		id(id), socket(NULL) {}
	virtual ~UDPResponder() {}

	void start(const Net::Address& relayedAddr) 
	{		
		Log("debug") << "UDPResponder: " << id << ": Starting on: " << relayedAddr.toString() << endl;
		
		try	{
			assert(!socket);
			this->relayedAddr = relayedAddr;
			socket = new Net::UDPSocket();
			socket->attach(packetDelegate<UDPResponder, STUN::Message>(this, &UDPResponder::onSTUNPacketReceived, 101));
			socket->attach(packetDelegate<UDPResponder, DataPacket>(this, &UDPResponder::onDataPacketReceived, 102));
			socket->connect(relayedAddr);

			Timer::getDefault().start(TimerCallback<UDPResponder>(this, &UDPResponder::onTimer, 2000, 2000));
		}
		catch (Poco::Exception& exc) {
			Log("error") << "UDPResponder: " << id << ": ERROR: " << exc.displayText() << endl;
		}
	}
	
	void stop() 
	{		
		Timer::getDefault().stop(TimerCallback<UDPResponder>(this, &UDPResponder::onTimer));
		if (socket) {	
			socket->detach(packetDelegate<UDPResponder, STUN::Message>(this, &UDPResponder::onSTUNPacketReceived, 101));
			socket->detach(packetDelegate<UDPResponder, DataPacket>(this, &UDPResponder::onDataPacketReceived, 102));
			delete socket;
			socket = NULL;
		}
	}
	
protected:	
	void onTimer(TimerCallback<UDPResponder>& timer)
	{
		Log("debug") << "UDPResponder: " << id << ": On Timer: Sending data to: " << relayedAddr.toString() << endl;
		assert(socket);
		socket->send("hey baby!", 9, relayedAddr);
	}
	
	void onDataPacketReceived(void* sender, DataPacket& packet)
	{
		// echoed from client...
		Log("debug") << "WOOOOOOOOOOOOOOOHOOOOOOOOOOOOOOOOOOOO" << endl;
		Log("debug") << "UDPResponder: " << id << ": Data Packet Received: " << packet.size() << endl;
	}

	void onSTUNPacketReceived(void* sender, STUN::Message& message)
	{
		assert(false); // responder is unaware of STUN
		Log("debug") << "UDPResponder: " << id << ": STUN Packet Received: " << message.toString() << endl;
	}
};


} } //  namespace Sourcey::TURN


#endif // TURN_UDPresponder_TEST_H


	/* //,  // << remoteAddr.toString() << endl;
		//const Net::Address& localAddr, const Net::Address& remoteAddr) 
	 //,  // << remoteAddr.toString() << endl;//: public Sourcey::TURN::IClientObserver
		//const Net::Address& localAddr, const Net::Address& remoteAddr) 
protected:	
	  
		
	void setRelayedAddr()
	{
	}
	virtual void onAllocationCreated(TURN::Client* client, TURN::Client* allocation)
	{
		Log("debug") << "UDPResponder: " << id << ": Allocation Created: " << transaction << endl;
		client->sendCreatePermissionRequest(_peerIP);
	}


	virtual void onAllocationRemoving(TURN::Client* client, TURN::Client* allocation)
	{
		Log("debug") << "UDPResponder: " << id << ": Allocation Deleted: " << transaction << endl;
	}


	virtual void onPermissionsCreated(TURN::Client* client, TURN::Client* allocation)
	{
		Log("debug") << "UDPResponder: " << id << ": Permissions Created: " << transaction << endl;
	}


	virtual void onDataReceived(TURN::Client* client, const char* data, int size)
	{
		Log("debug") << "UDPResponder: " << id << ": Received Data: " << string(data, size) << endl;
	}
	
	virtual void onAllocationFailed(Client* client, int errorCode)
	{		
		Log("debug") << "UDPResponder: " << id << ": Allocation Failed: " << errorCode << endl;
	}
	
	void onTransactionResponse(Client* client, STUN::Transaction* transaction)
	{		
		Log("debug") << "TURN Initiator: Transaction Response: " << transaction << endl;
	}	
	
	void onAllocationPermissionsCreated(Client* client, Client* allocation)
	{
		Log("debug") << "TURN Initiator: Permissions Created: " << allocation << endl;
	}	
	*/

		/*
		try	{
			Client::Options opts;
			opts.serverAddr = Sourcey::Net::Address("127.0.0.1", 3478);
			opts.timeout = 10000;
			opts.username = "user";
			opts.password = "E94rKjRkC3nqyj8UvoYW";

			client = new Client(*this, opts);
			client->sendAllocateRequest();

			system("pause");
		}
		catch (Poco::Exception& exc) {
			Log("error") << "UDPResponder: " << id << ": Error: " << exc.displayText() << endl;
		}
		*/