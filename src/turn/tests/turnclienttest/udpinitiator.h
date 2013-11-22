#ifndef TURN_UDPinitiator_TEST_H
#define TURN_UDPinitiator_TEST_H


#include "scy/turn/client/udpclient.h"
#include "scy/logger.h"

#include <iostream>


using namespace std;


namespace scy {
namespace turn {


class UDPInitiator: public scy::turn::ClientObserver
{
	int id;
	UDPClient* client;
	Client::Options	opts;
	//Net::Reactor& reactor;, Net::Reactor& reactor, Runner& runner
	//Runner& runner;reactor(reactor), runner(runner), 

public:
	UDPInitiator(int id) : 
		id(id), client(NULL) {}
	virtual ~UDPInitiator() { if (client) delete client; }
	
	void initiate(const Client::Options opts, const std::string& peerIP) 
	{
		try	{
			client = new UDPClient(*this, opts); //, reactor, runner
			//client->StateChange += delegate(this, &UDPInitiator::onStateChange);
			client->addPermission(peerIP);	
			client->initiate();
		} 
		catch (std::exception& exc) {
			errorL() << "UDPInitiator: " << id << ": Error: " << exc.what() << std::endl;
		}
	}

	NullSignal AllocationCreated;

protected:
	void onClientStateChange(turn::Client& client, turn::ClientState& state, const turn::ClientState&) 
	{
		debugL() << "UDPInitiator: " << id << ": State change: " << state.toString() << endl;

		switch(state.id()) {
		case ClientState::None:				
			break;
		case ClientState::Allocating:				
			break;
		case ClientState::Authorizing:		
			break;
		case ClientState::Success:
			AllocationCreated.emit(this);
			break;
		case ClientState::Failed:				
			break;
		//case ClientState::Terminated:				
		//	break;
		}
	}

	void onRelayConnectionDataReceivedReceived(turn::Client& client, const char* data, int size, const net::Address& peerAddr)
	{
		debugL() << "UDPInitiator: " << id << ": Received Data: " << string(data, size) << endl;

		// echo it back...
		client.sendData(data, size, peerAddr);
	}
	
	void onPermissionsCreated(turn::Client& client, const turn::PermissionList& permissions)
	{
		debugL() << "UDPInitiator: " << id << ": Permissions Created" << endl;
	}
};


} } //  namespace scy::turn


#endif // TURN_UDPinitiator_TEST_H