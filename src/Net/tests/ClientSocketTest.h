#include "scy/uv/uvpp.h"
#include "scy/net/tcpsocket.h"
#include "scy/net/sslsocket.h"
#include "scy/net/address.h"
#include "scy/logger.h"
#include <memory>


namespace scy {
namespace net {


template <typename SocketT>
class ClientSocketTest
{
public:
	typename SocketT socket;
	Address address;

	ClientSocketTest(short port, bool ghost = false) :
		address("127.0.0.1", port)
	{		
		traceL("ClientSocketTest") << "Creating: " << port << std::endl;
	}

	~ClientSocketTest()
	{
		traceL("ClientSocketTest") << "Destroying" << std::endl;
		assert(socket.base().refCount() == 1);
	}

	void run() 
	{
		// Create the socket instance on the stack.
		// When the socket is closed it will unref the main loop
		// causing the test to complete successfully.
		socket.Recv += delegate(this, &ClientSocketTest::onRecv);
		socket.Connect += delegate(this, &ClientSocketTest::onConnect);
		socket.Error += delegate(this, &ClientSocketTest::onError);
		socket.Close += delegate(this, &ClientSocketTest::onClose);
		socket.connect(address);
		assert(socket.base().refCount() == 1);
	}

	void stop() 
	{
		//socket.close();
		socket.shutdown();
	}
	
	void onConnect(void* sender)
	{
		traceL("ClientSocketTest") << "Connected" << endl;
		assert(sender == &socket);
		socket.send("client > server", 15);
		socket.send("client > server", 15);
		socket.send("client > server", 15);
	}
	
	void onRecv(void* sender, SocketPacket& packet)
	{
		assert(sender == &socket);
		string data(packet.data(), 15);
		traceL("ClientSocketTest") << "Recv: " << data << endl;	

		// Check for return packet echoing sent data
		if (data == "client > server") {
			traceL("ClientSocketTest") << "Recv: Got Return Packet!" << endl;
			
			// Send the shutdown command to close the connection gracefully.
			// The peer disconnection will trigger an eof error callback
			// which notifies us that the connection is dead.
			//socket.shutdown();
			//socket.close();
			stop();
		}
		else 
			assert(false); // fail...
	}

	void onError(void* sender, const Error& error)
	{
		errorL("ClientSocketTest") << "On error: " << error.message << endl;
		assert(sender == &socket);
	}
	
	void onClose(void* sender)
	{
		traceL("ClientSocketTest") << "On closed" << endl;
		// The last callback to fire is the Closed signal  
		// which notifies us the underlying libuv socket 
		// handle is closed. Das is gut!
	}
};


} } // namespace scy::net


		
		//assert(sender == &socket);
		//assert(sender == &socket); //packet.data(), packet.size());
		//traceL("UVEchoServer", this) << "On Recv: " << packet.buffer << std::endl;
		//socket.send(packet.data(), packet.size());

		/*
		SocketT socket(sock); //.get()
		traceL("UVEchoServer", this) << "On Accept: " << &socket.base() << std::endl;
		socket.Recv += delegate(this, &UVEchoServer::onSocketRecv);
		socket.Error += delegate(this, &UVEchoServer::onSocketError);
		socket.Close += delegate(this, &UVEchoServer::onSocketClose);
		sockets.push_back(socket);
		*/