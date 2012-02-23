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
	Net::TCPClientSocket* socket;

	TCPResponder(int id = 0): id(id), socket(NULL) {}
	virtual ~TCPResponder() {}

	void start(const Net::Address& relayedAddr) 
	{		
		Log("debug") << "TCPResponder: " << id << ": Starting on: " << relayedAddr.toString() << endl;
		
		try	{
			assert(!socket);
			this->relayedAddr = relayedAddr;
			socket = new Net::TCPClientSocket();
			socket->StateChanged += delegate(this, &TCPResponder::onSocketStateChanged);
			socket->registerPacketType<DataPacket>(0);
			socket->attach(packetDelegate<TCPResponder, DataPacket>(this, &TCPResponder::onDataPacketReceived, 102));
			socket->connect(relayedAddr);

			//Timer::getDefault().start(TimerCallback<TCPResponder>(this, &TCPResponder::onTimer, 1000, 1000));

		}
		catch (Poco::Exception& exc) {
			Log("error") << "TCPResponder: " << id << ": ERROR: " << exc.displayText() << endl;
			assert(false);
		}
	}
	
	void stop() 
	{		
		//Timer::getDefault().stop(TimerCallback<TCPResponder>(this, &TCPResponder::onTimer));
		if (socket) {	
			socket->StateChanged -= delegate(this, &TCPResponder::onSocketStateChanged);
			socket->detach(packetDelegate(this, &TCPResponder::onDataPacketReceived, 102));
			delete socket;
			socket = NULL;
		}
	}
	
protected:	

	void onTimer(TimerCallback<TCPResponder>& timer)
	{
		if (socket) {
			Log("debug") << "TCPResponder: " << id << ": On Timer: Sending data to: " << relayedAddr.toString() << endl;
			//assert(socket);
			socket->send("hey baby!", 9); //, relayedAddr
		}
		else 
			Log("debug") << "TCPResponder: " << id << ": Socket is dead!" << endl;
	}
	
	void onDataPacketReceived(void* sender, DataPacket& packet)
	{
		Log("debug") << "#################################### TCPResponder: " << id << ": Received Data: " << string((const char*)packet.data(), packet.size()) << endl;
		
		// Echo back to client
		//socket->send(packet);
	}

	void onSocketStateChanged(void* sender, Net::ClientState& state, const Net::ClientState&)
	{
		Log("debug") << "Connection State Changed: " << state.toString() << endl;	
	
		switch (state.id()) {
		case Net::ClientState::Disconnected: 
		//case Net::ClientState::Error: 
			socket = NULL;
			break;
		}
	}

};


} } //  namespace Sourcey::TURN


#endif // TURN_TCPresponder_TEST_H
