#ifndef TURN_TCPresponder_TEST_H
#define TURN_TCPresponder_TEST_H


#include "scy/net/tcpsocket.h"
#include "scy/logger.h"
#include <string>


using namespace std;


namespace scy {
namespace turn {


class TCPResponder: public net::SocketAdapter
{
public:
	int id;
	net::Address relayedAddr;
	net::TCPSocket socket;	
	Timer timer;

	TCPResponder(int id) : 
		id(id)
	{
		DebugLS(this) << id << ": Creating" << endl;
		//net::SocketAdapter::socket = &socket;
		socket.setInputAdapter(this);
	}

	virtual ~TCPResponder() 
	{ 
		DebugLS(this) << id << ": Destroying" << endl;
		//socket.base().removeObserver(this);
		socket.setInputAdapter(nullptr, false);
		stop(); 
	}

	void start(const net::Address& relayedAddr) 
	{		
		DebugLS(this) << id << ": Starting on: " << relayedAddr << endl;
		
		try	{
			this->relayedAddr = relayedAddr;

			// Since we extend SocketAdapter socket callbacks 
			// will be received below.
			socket.connect(relayedAddr);
		}
		catch (std::exception& exc) {
			errorL("TCPResponder", this) << id << ": ERROR: " << exc.what() << endl;
			assert(false);
		}
	}
	
	void stop() 
	{	
		timer.stop();
	}

	void onSocketConnect() 
	{
		// Send some early media to client
		sendLatencyCheck();

		// Start the send timer
		timer.Timeout += sdelegate(this, &TCPResponder::onSendTimer);
		timer.start(1000, 1000);
	}
	
	void onSocketRecv(const MutableBuffer& buffer, const net::Address& peerAddress) //net::SocketPacket& packet) 
	{
		//assert(&packet.info->socket == &socket);
		std::string payload(bufferCast<const char*>(buf), buf.size());
		DebugLS(this) << id << ": On recv: " << peerAddr << ": " << payload << std::endl;

		//assert(payload == "hello peer");
		//assert(0 && "ok");

		// Echo back to client
		//socket.send(payload.c_str(), payload.size());
	}

	void onSocketError(const scy::Error& error) 
	{
		DebugLS(this) << id << ": On error: " << error.message << std::endl;
	}

	void onSocketClose() 
	{
		DebugLS(this) << id << ": On close" << std::endl;
		stop();
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
		payload.append(10000, 'x');

		// Send it
		socket.send(payload.c_str(), payload.length());
	}

	void onSendTimer(void*)
	{
		sendLatencyCheck();
	}
};


} } //  namespace scy::turn


#endif // TURN_TCPresponder_TEST_H


	
	/*
	void onSocketConnect() 
	{
		// Send some early media to client.
		// This will be buffered by the server, and send
		// to the client when the connection is accepted.
		socket.send("early media", 11);

		// Start the send timer
		//timer.Timeout += sdelegate(this, &TCPResponder::onSendTimer);
		//timer.start(5000, 5000);
	}
	*/
	/*
	void onSendTimer(void*)
	{
		DebugLS(this) << "Send timer" << endl;	
		//socket.send("bouncey", 7);
	}
	*/

			/*
			assert(!socket);
			socket = new Net::TCPStatefulPacketSocket(reactor);
			socket.StateChange += sdelegate(this, &TCPResponder::onSocketStateChange);
			socket.registerPacketType<RawPacket>(0);
			socket.attach(packetDelegate(this, &TCPResponder::onRawPacketReceived, 102));
			*/

			//Timer::getDefault().start(TimerCallback<TCPResponder>(this, &TCPResponder::onSendTimer, 1000)); //, 1000));
			//Timer::getDefault().start(TimerCallback<TCPResponder>(this, &TCPResponder::onRecreateTimer, 1000, 1000));
		/*
		//Timer::getDefault().stop(TimerCallback<TCPResponder>(this, &TCPResponder::onSendTimer));
		//Timer::getDefault().stop(TimerCallback<TCPResponder>(this, &TCPResponder::onRecreateTimer));
		if (socket) {	
			socket.StateChange -= sdelegate(this, &TCPResponder::onSocketStateChange);
			socket.detach(packetDelegate(this, &TCPResponder::onRawPacketReceived, 102));
			delete socket;
			socket = NULL;
		}
		*/

	
		/*
	
protected:	
	
	void onRawPacketReceived(void* sender, RawPacket& packet)
	{
		debugL() << "########################## [TCPResponder: " << id << "] Received Data: " << std::string((const char*)packet.data(), packet.size()) << endl;
		
		// Echo back to client
		socket.send(packet);
	}

		if (socket)
	void onSocketStateChange(void* sender, Net::SocketState& state, const Net::SocketState&)
	{
		debugL() << "Connection state change: " << state.toString() << endl;	
	
		switch (state.id()) {
		case Net::SocketState::Disconnected: 
			//case Net::ClientState::Error: 
			socket = NULL;
			break;
		}
	}
		*/

	/*
	void onSendTimer(TimerCallback<TCPResponder>& timer)
	{
		if (socket) {
			debugL() << "[TCPResponder: " << id << "] On Timer: Sending data to: " << relayedAddr << endl;
			assert(socket);
			socket.send("echo", 5); //, relayedAddr
		}
		else 
			debugL() << "[TCPResponder: " << id << "] Socket is dead!" << endl;
	}

	void onRecreateTimer(TimerCallback<TCPResponder>& timer)
	{
		debugL() << "###################################### TURN: Recreating Responder" << endl;
		
		if (socket) {
			socket.close();
			socket.connect(relayedAddr);
		}
		//delete responder;
		//responder = new TCPResponder(id, reactor, runner);
		//responder->start(initiator->client->relayedAddress());
	}
	*/
