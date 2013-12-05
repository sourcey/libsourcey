#ifndef TURN_UDPinitiator_TEST_H
#define TURN_UDPinitiator_TEST_H


#include "turnclienttest.h"
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
	Timer timer;

	// The responder local socket address for Send indications
	net::Address responderAddress;
	
	//Client::Options	opts;
	//Signal<const net::Address&>	ConnectionCreated;

	NullSignal AllocationCreated;
	Signal<bool> TestComplete; // unused

	UDPInitiator(int id, const Client::Options& opts) : 
		id(id), client(*this, opts) {}
	virtual ~UDPInitiator() { }
	
	void initiate(const std::string& peerIP) 
	{
		DebugLS(this) << id << ": Initializing" << endl;
		try	{
			client.addPermission(peerIP);
			client.addPermission("127.0.0.1");		
			client.addPermission("192.168.1.1");			
			client.initiate();
		} 
		catch (std::exception& exc) {
			ErrorLS(this) << id << ": " << exc.what() << std::endl;
		}
	}

protected:
	void onClientStateChange(turn::Client& client, turn::ClientState& state, const turn::ClientState&) 
	{
		DebugLS(this) << id << ": State change: " << state.toString() << endl;

		switch(state.id()) {
		case ClientState::None:				
			break;
		case ClientState::Allocating:				
			break;
		case ClientState::Authorizing:		
			break;
		case ClientState::Success:
			AllocationCreated.emit(this);
#ifdef TEST_INITIATOR_TO_RESPONDER
			timer.Timeout += delegate(this, &UDPInitiator::onSendTimer);
			timer.start(0, 1000);
#endif
			break;
		case ClientState::Failed:				
			break;
		//case ClientState::Terminated:				
		//	break;
		}
	}

	void onSendTimer(void*)
	{
		assert(responderAddress.valid());
		std::string payload(createLatencyCheck(1024));
		client.sendData(payload.c_str(), payload.length(), responderAddress);

		//socket.send(payload.c_str(), payload.length());	

		// Echo back to peer
		//client.sendData(data, size, peerAddr);
	}
	
	void onRelayDataReceived(turn::Client& client, const char* data, int size, const net::Address& peerAddr)
	{	
#ifdef TEST_RESPONDER_TO_INITIATOR
		std::string payload(data, size);
		payload.erase(std::remove(payload.begin(), payload.end(), 'x'), payload.end());
		if (payload.length() == 9) {
			UInt64 sentAt = util::strtoi<UInt64>(payload);
			UInt64 latency = time::ticks() - sentAt;

			DebugLS(this) << id << ": Received data from " << peerAddr << ": payload=" << payload << ", latency=" << latency << endl;
		}
		else		
			DebugLS(this) << id << ": Received dummy data from " << peerAddr << ": payloadLength=" << payload.length() << endl;
		
		/*
		if (size < 150) {
			std::string payload(data, size);
			UInt64 sentAt = util::strtoi<UInt64>(payload);
			UInt64 latency = time::ticks() - sentAt;

			DebugLS(this) << id << ": Received data from " << peerAddr << ": payload=" << payload << ", latency=" << latency << endl;
		}
		else
			DebugLS(this) << id << ": Received dummy data from " << peerAddr << ": size=" << size << endl;
		*/

		// Echo back to peer
		//client.sendData(data, size, peerAddr);
#else //if TEST_INITIATOR_TO_RESPONDER
		DebugLS(this) << id << ": Received response data from " << peerAddr << ": size=" << size << endl;
#endif
	}
	
	void onAllocationPermissionsCreated(turn::Client& client, const turn::PermissionList& permissions)
	{
		DebugLS(this) << id << ": Permissions Created" << endl;
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