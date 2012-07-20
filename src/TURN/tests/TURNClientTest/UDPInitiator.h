#ifndef TURN_UDPinitiator_TEST_H
#define TURN_UDPinitiator_TEST_H


#include "Sourcey/TURN/client/Client.h"
#include "Sourcey/Logger.h"

#include <iostream>


using namespace std;


namespace Sourcey {
namespace TURN {


class UDPInitiator: public Sourcey::TURN::IClientObserver
{
	int id;
	Client* client;
	Client::Options	opts;
	Net::Reactor& reactor;
	Runner& runner;

public:
	UDPInitiator(int id, Net::Reactor& reactor, Runner& runner) : 
		id(id), reactor(reactor), runner(runner), client(NULL) {}
	virtual ~UDPInitiator() { if (client) delete client; }
	
	void initiate(const Client::Options opts, const Net::IP& peerIP) 
	{
		try	{
			client = new Client(*this, reactor, runner, opts);
			//client->StateChange += delegate(this, &UDPInitiator::onStateChange);
			client->addPermission(peerIP);	
			client->initiate();
		} 
		catch (Poco::Exception& exc) {
			Log("error") << "UDPInitiator: " << id << ": Error: " << exc.displayText() << std::endl;
		}
	}

	NullSignal AllocationCreated;

protected:
	void onRelayStateChange(TURN::Client& client, TURN::ClientState& state, const TURN::ClientState&) 
	{
		Log("debug") << "UDPInitiator: " << id << ": State Changed: " << state.toString() << endl;

		switch(state.id()) {
		case ClientState::Waiting:				
			break;
		case ClientState::Allocating:				
			break;
		case ClientState::Authorizing:		
			break;
		case ClientState::Success:
			AllocationCreated.dispatch(this);
			break;
		case ClientState::Failed:				
			break;
		case ClientState::Terminated:				
			break;
		}
	}

	void onRelayedDataReceived(TURN::Client& client, const char* data, int size, const Net::Address& peerAddr)
	{
		Log("debug") << "UDPInitiator: " << id << ": Received Data: " << string(data, size) << endl;

		// echo it back...
		client.sendData(data, size, peerAddr);
	}
	
	void onPermissionsCreated(TURN::Client& client, const TURN::PermissionList& permissions)
	{
		Log("debug") << "UDPInitiator: " << id << ": Permissions Created" << endl;
	}
};


} } //  namespace Sourcey::TURN


#endif // TURN_UDPinitiator_TEST_H