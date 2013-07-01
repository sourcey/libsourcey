#include "Sourcey/UV/UVPP.h"
#include "Sourcey/Net/TCPSocket.h"
#include "Sourcey/Net/SSLSocket.h"
#include "Sourcey/Net/Address.h"
#include "Sourcey/Logger.h"
#include <memory>


namespace scy {
namespace uv {


template <typename SocketT>
class UVEchoServer: public CountedObject
{
public:
	typename SocketT socket;
	typename SocketT::List sockets;
	net::Address address;

	UVEchoServer(short port, bool ghost = false) :
		address("127.0.0.1", port)
	{
		// Unref the socket handle if running in ghost mode.
		// This way the server won't prevent the main uv_loop 
		// from exiting when other processed complete.
		if (ghost)
			uv_unref(reinterpret_cast<SocketT::Base&>(socket.base()).handle());
	}

	~UVEchoServer()
	{
		traceL("UVEchoServer", this) << "Destroying" << std::endl;
		assert(socket.base().refCount() == 1);

		// This will close the server socket, and should cause the
		// main loop to terminate if there are no active processes.
		sockets.clear();

		// Shutdown gracefully
		//traceL("UVEchoServer", this) << "Destroying: Shutdown:" << &socket.base() << std::endl;
		//socket.shutdown();

		traceL("UVEchoServer", this) << "Destroying: OK" << std::endl;
	}
	
	void run()
	{	
		traceL("UVEchoServer", this) << "Serrun " << socket.base().refCount() << endl;
		assert(socket.base().refCount() == 1);
		socket.bind(address);
		socket.listen();
		socket.base().AcceptConnection += delegate(this, &UVEchoServer::onAccept);		
		traceL("UVEchoServer", this) << "Server listening on " << port() << endl;
	}

	void stop() 
	{
		socket.close();
	}
	
	void onAccept(void* sender, const net::TCPSocket& sock)
	{	
		sockets.push_back(sock);
		SocketT& socket = sockets.back();
		traceL("UVEchoServer", this) << "On Accept: " << &socket.base() << std::endl;
		socket.Recv += delegate(this, &UVEchoServer::onSocketRecv);
		socket.Error += delegate(this, &UVEchoServer::onSocketError);
		socket.Close += delegate(this, &UVEchoServer::onSocketClose);
	}
	
	void onSocketRecv(void* sender, net::SocketPacket& packet) 
	{
		SocketT* socket = reinterpret_cast<SocketT*>(sender);
		traceL("UVEchoServer", this) << "On Recv: " 
			<< socket << ": " << packet.buffer << std::endl;

		// Echo it back
		socket->send(packet);
	}

	void onSocketError(void* sender, int syserr, const std::string& message) 
	{
		traceL("UVEchoServer", this) << "On Error: " << syserr << ": " << message << std::endl;
	}

	void onSocketClose(void* sender) 
	{
		traceL("UVEchoServer", this) << "On Close" << std::endl;
		releaseSocket(reinterpret_cast<SocketT*>(sender));
	}

	void releaseSocket(typename SocketT* socket) 
	{		
		for (SocketT::List::iterator it = sockets.begin(); it != sockets.end(); ++it) { //::Ptr
			if (socket == &(*it)) { //.base()
				traceL("UVEchoServer", this) << "Removing: " << socket << std::endl;

				// All we need to do is erase the socket in order to 
				// deincrement the ref counter and destroy the socket.
				assert(socket->base().refCount() == 1);
				sockets.erase(it);
				return;
			}
		}
	}

	UInt16 port()
	{
		return socket.address().port();
	}
};
	

// Some generic server types
typedef uv::UVEchoServer<net::TCPSocket> TCPEchoServer;
//typedef uv::UVEchoServer<net::SSLSocket> SSLEchoServer;


} } // namespace scy::uv


		
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