#ifndef TURN_TCPresponder_TEST_H
#define TURN_TCPresponder_TEST_H


#include "Sourcey/Net/TCPSocket.h"
#include "Sourcey/Logger.h"
#include <string>


using namespace std;


namespace Sourcey {
namespace TURN {


class TCPResponder
{
public:
	int id;
	Net::Address relayedAddr;
	Net::Reactor&   reactor;
	Runner&         runner;
	Net::TCPStatefulPacketSocket* socket;	

	TCPResponder(int id, Net::Reactor& reactor, Runner& runner) : 
		id(id), reactor(reactor), runner(runner), socket(NULL) {}
	virtual ~TCPResponder() { stop(); }

	void start(const Net::Address& relayedAddr) 
	{		
		Log("debug") << "[TCPResponder: " << id << "] Starting on: " << relayedAddr.toString() << endl;
		
		try	{
			assert(!socket);
			this->relayedAddr = relayedAddr;
			socket = new Net::TCPStatefulPacketSocket(reactor);
			socket->StateChange += delegate(this, &TCPResponder::onSocketStateChange);
			socket->registerPacketType<DataPacket>(0);
			socket->attach(packetDelegate(this, &TCPResponder::onDataPacketReceived, 102));
			socket->connect(relayedAddr);

			//Timer::getDefault().start(TimerCallback<TCPResponder>(this, &TCPResponder::onTimer, 1000)); //, 1000));
			Timer::getDefault().start(TimerCallback<TCPResponder>(this, &TCPResponder::onRecreateTimer, 1000, 1000));
		}
		catch (Poco::Exception& exc) {
			Log("error") << "[TCPResponder: " << id << "] ERROR: " << exc.displayText() << endl;
			assert(false);
		}
	}
	
	void stop() 
	{		
		//Timer::getDefault().stop(TimerCallback<TCPResponder>(this, &TCPResponder::onTimer));
		Timer::getDefault().stop(TimerCallback<TCPResponder>(this, &TCPResponder::onRecreateTimer));
		if (socket) {	
			socket->StateChange -= delegate(this, &TCPResponder::onSocketStateChange);
			socket->detach(packetDelegate(this, &TCPResponder::onDataPacketReceived, 102));
			delete socket;
			socket = NULL;
		}
	}
	
protected:	

	/*
	void onTimer(TimerCallback<TCPResponder>& timer)
	{
		if (socket) {
			Log("debug") << "[TCPResponder: " << id << "] On Timer: Sending data to: " << relayedAddr.toString() << endl;
			assert(socket);
			socket->send("echo", 5); //, relayedAddr
		}
		else 
			Log("debug") << "[TCPResponder: " << id << "] Socket is dead!" << endl;
	}
	*/

	void onRecreateTimer(TimerCallback<TCPResponder>& timer)
	{
		Log("debug") << "###################################### TURN: Recreating Responder" << endl;
		
		if (socket) {
			socket->close();
			socket->connect(relayedAddr);
		}
		//delete responder;
		//responder = new TCPResponder(id, reactor, runner);
		//responder->start(initiator->client->relayedAddress());
	}
	
	void onDataPacketReceived(void* sender, DataPacket& packet)
	{
		Log("debug") << "########################## [TCPResponder: " << id << "] Received Data: " << string((const char*)packet.data(), packet.size()) << endl;
		
		// Echo back to client
		if (socket)
			socket->send(packet);
	}

	void onSocketStateChange(void* sender, Net::SocketState& state, const Net::SocketState&)
	{
		Log("debug") << "Connection State Changed: " << state.toString() << endl;	
	
		/*
		switch (state.id()) {
		case Net::SocketState::Disconnected: 
			//case Net::ClientState::Error: 
			socket = NULL;
			break;
		}
		*/
	}

};


} } //  namespace Sourcey::TURN


#endif // TURN_TCPresponder_TEST_H
