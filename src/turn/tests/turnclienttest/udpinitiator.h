#ifndef TURN_UDPinitiator_TEST_H
#define TURN_UDPinitiator_TEST_H


#include "scy/turn/client/udpclient.h"
#include "scy/time.h"
#include "scy/logger.h"

#include <iostream>


using namespace std;


namespace scy {
namespace turn {


class UDPInitiator: public scy::turn::ClientObserver
{
public:
	int id;
	UDPClient client;
	Client::Options	opts;

	NullSignal AllocationCreated;
	Signal<const net::Address&>	ConnectionCreated;
	Signal<bool> TestComplete;

	UDPInitiator(int id, const Client::Options& opts) : 
		id(id), client(*this, opts) {}
	virtual ~UDPInitiator() { }
	
	void initiate(const std::string& peerIP) 
	{
		debugL("UDPInitiator", this) << id << ": Initializing" << endl;
		try	{
			client.addPermission(peerIP);
			client.addPermission("127.0.0.1");		
			client.addPermission("192.168.1.1");			
			client.initiate();
		} 
		catch (std::exception& exc) {
			errorL("TCPInitiator") << id << ": Error: " << exc.what() << std::endl;
		}
	}

protected:
	void onClientStateChange(turn::Client& client, turn::ClientState& state, const turn::ClientState&) 
	{
		debugL("UDPInitiator", this) << id << ": State change: " << state.toString() << endl;

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
	
	void onRelayDataReceived(turn::Client& client, const char* data, int size, const net::Address& peerAddr)
	{	
		std::string payload(data, size);
		payload.erase(std::remove(payload.begin(), payload.end(), 'x'), payload.end());
		if (payload.length() == 8) {
			UInt64 sentAt = util::strtoi<UInt64>(payload);
			UInt64 latency = time::ticks() - sentAt;

			debugL("UDPInitiator") << id << ": Received data from " << peerAddr << ": payload=" << payload << ", latency=" << latency << endl;
		}

		/*
		if (size < 150) {
			std::string payload(data, size);
			UInt64 sentAt = util::strtoi<UInt64>(payload);
			UInt64 latency = time::ticks() - sentAt;

			debugL("UDPInitiator") << id << ": Received data from " << peerAddr << ": payload=" << payload << ", latency=" << latency << endl;
		}
		else
			debugL("UDPInitiator") << id << ": Received dummy data from " << peerAddr << ": size=" << size << endl;
		*/
		// Echo back to peer
		//client.sendData(data, size, peerAddr);
	}
	
	void onAllocationPermissionsCreated(turn::Client& client, const turn::PermissionList& permissions)
	{
		debugL("UDPInitiator") << id << ": Permissions Created" << endl;
	}
};


} } //  namespace scy::turn


#endif // TURN_UDPinitiator_TEST_H




	/*
	void initiate(const Client::Options opts, const std::string& peerIP) 
	{
		try	{
			client = new UDPClient(*this, opts); //, reactor, runner
			//client.StateChange += delegate(this, &UDPInitiator::onStateChange);
			client.addPermission(peerIP);	
			client.initiate();
		} 
		catch (std::exception& exc) {
			errorL() << "UDPInitiator: " << id << ": Error: " << exc.what() << std::endl;
		}
	}
	*/