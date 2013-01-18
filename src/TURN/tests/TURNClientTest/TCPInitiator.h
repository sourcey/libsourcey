#ifndef TURN_TCPinitiator_TEST_H
#define TURN_TCPinitiator_TEST_H


#include "Sourcey/TURN/client/TCPClient.h"
#include "Sourcey/Util/Timer.h"
#include "Sourcey/Signal.h"
#include "Sourcey/Logger.h"

#include <iostream>


using namespace std;


namespace Sourcey {
namespace TURN {


class TCPInitiator: public Sourcey::TURN::ITCPClientObserver
{
public:
	bool			success;
	int				id;
	Client*			client;
	Net::Reactor&   reactor;
	Runner&         runner;
	Net::Address	lastPeerAddr;
	
	Signal<TURN::Client&>	AllocationCreated;
	Signal<TURN::Client&>	ConnectionCreated;
	Signal<bool>			TestDone;

	TCPInitiator(int id, Net::Reactor& reactor, Runner& runner) : 
		id(id), reactor(reactor), runner(runner), client(NULL), success(false) {}
	virtual ~TCPInitiator() { if (client) { client->terminate(); delete client; } }

	void initiate(const Client::Options opts, const Net::IP& peerIP) 
	{
		Log("debug") << "TCPInitiator: " << id << ": Initializing" << endl;

		try	{
			client = new TCPClient(*this, reactor, runner, opts);
			client->addPermission(peerIP);	
			client->initiate();			
		
			//STUN::SocketTransaction* transaction = client->createTransaction();
			//delete transaction;
		} 
		catch (Poco::Exception& exc) {
			Log("error") << "TCPInitiator: " << id << ": Error: " << exc.displayText() << std::endl;
		}
	}

protected:
	void onRelayStateChange(TURN::Client& client, TURN::ClientState& state, const TURN::ClientState&) 
	{
		Log("debug") << "####################### TCPInitiator: " << id << ": State Changed: " << state.toString() << endl;

		switch(state.id()) {
		case ClientState::Waiting:				
			break;
		case ClientState::Allocating:				
			break;
		case ClientState::Authorizing:
			//success = true;
			//TestDone.dispatch(this, success);
			//client.terminate();
			break;
		case ClientState::Success:
			AllocationCreated.dispatch(this, *this->client);
			success = true;
			TestDone.dispatch(this, success);
			//client.terminate();
			break;
		case ClientState::Failed:
			//assert(false);
			//throw Exception(state.toString());
			success = false;
			TestDone.dispatch(this, success);
			break;
		case ClientState::Terminated:				
			break;
		}
	}
	
	void onTimer(TimerCallback<TCPInitiator>& timer)
	{
		if (client) {
			Log("debug") << "TCPInitiator: " << id << ": On Timer: Sending data to: " << lastPeerAddr.toString() << endl;
			client->sendData("rand@m", 6, lastPeerAddr);
		}
		else 
			Log("debug") << "TCPInitiator: " << id << ": Socket is dead!" << endl;
	}

	void onClientConnectionCreated(TCPClient& client, Net::IPacketSocket* sock, const Net::Address& peerAddr) //UInt32 connectionID, 
	{
		Log("debug") << "######################### TCPInitiator: " << id << ": Connection Created: " << peerAddr << endl;
				
		// Send the intial data packet to peer
		client.sendData("rand@m", 6, peerAddr);

		//Timer::getDefault().start(TimerCallback<TCPInitiator>(this, &TCPInitiator::onTimer, 1000, 1000));
		lastPeerAddr = peerAddr; // last peer
		success = true;
		
		ConnectionCreated.dispatch(this, *this->client);
		TestDone.dispatch(this, success);
	}
	
	void onClientConnectionState(TCPClient& client, Net::IPacketSocket*, 
		Net::SocketState& state, const Net::SocketState& oldState) 
	{
		Log("debug") << "######################### TCPInitiator: " << id << ": Connection State: " << state.toString() << endl;
	}

	void onRelayedData(TURN::Client& client, const char* data, int size, const Net::Address& peerAddr)
	{
		Log("debug") << "######################### TCPInitiator: " << id << ": Received Data: " << string(data, size) <<  ": " << peerAddr << endl;
		
		// Echo back to peer
		client.sendData(data, size, peerAddr);
	}
	
	void onAllocationPermissionsCreated(TURN::Client& client, const TURN::PermissionList& permissions)
	{
		Log("debug") << "######################### TCPInitiator: " << id << ": Permissions Created" << endl;
	}
};


} } //  namespace Sourcey::TURN


#endif // TURN_TCPinitiator_TEST_H



	

	/*
	bool onConnectionAttempt(TCPClient& client, UInt32 connectionID, const Net::Address& peerAddr) 
	{ 
		Log("debug") << "TCPInitiator: " << id << ": Connection Attempt: " << peerAddr.toString() << endl;
		return true; 
	};
	*/

	/*
	void onConnectionBindError(TCPClient& client, UInt32 connectionID)
	{
		Log("debug") << "TCPInitiator: " << id << ": Connection Error: " << connectionID << endl;
		if (_dataSocket) {
			_dataSocket->StateChange -= delegate(this, &TCPInitiator::onDataSocketStateChange);
			_dataSocket->detach(packetDelegate<TCPInitiator, DataPacket>(this, &TCPInitiator::onDataPacketReceived, 102));
			_dataSocket = NULL;
		}
	}
		
	void onDataPacketReceived(void* sender, DataPacket& packet)
	{
		//Log("debug") << "TCPInitiator: " << id << ": Data Packet Received: " << string(packet.data, packet.size) << endl;
		Log("debug") << "TCPInitiator: " << id << ": Data Packet Received: " << packet.size << endl;
	}

	void onDataSocketStateChange(void* sender, Net::ClientState& state, const Net::ClientState&)
	{
		Log("debug") << "Connection State Changed: " << state.toString() << endl;	
	
		switch (state.id()) {
		case Net::ClientState::Disconnected: 
		//case Net::ClientState::Error: 
			_dataSocket = NULL;
			break;
		}
	}
	*/