#include "scy/net/tcpsocket.h"
#include "scy/net/sslsocket.h"


namespace scy {
namespace net {


template <class SocketT>
class EchoServer
	/// The TCP echo server accepts a template argument  
	/// of either a TCPSocket or a SSLSocket.
{
public:
	SocketT socket;
	typename SocketT::List sockets;

	EchoServer()
	{
	}

	~EchoServer()
	{
		shutdown();
	}
		
	void start(const std::string& host, UInt16 port)
	{	
		socket.bind(Address(host, port));
		socket.listen();
		socket.base().AcceptConnection += delegate(this, &EchoServer::onAccept);	
	}

	void shutdown() 
	{
		socket.close();
		sockets.clear();
	}
	
	void onAccept(void* sender, const TCPSocket& sock)
	{	
		// Accept client connections
		sockets.push_back(sock);
		SocketT& socket = sockets.back();
		debugL("EchoServer", this) << "On accept: " << &socket.base() << std::endl;
		socket.Recv += delegate(this, &EchoServer::onClientSocketRecv);
		socket.Error += delegate(this, &EchoServer::onClientSocketError);
		socket.Close += delegate(this, &EchoServer::onClientSocketClose);
	}
	
	void onClientSocketRecv(void* sender, SocketPacket& packet) 
	{
		SocketT* socket = reinterpret_cast<SocketT*>(sender);
		debugL("EchoServer", this) << "On recv: " << socket << ": " 
			<< std::string(packet.data(), packet.size()) << std::endl;

		// Echo it back
		socket->send(packet);
	}

	void onClientSocketError(void* sender, const Error& error)
	{
		errorL("EchoServer", this) << "On error: " 
			<< error.errorno << ": " << error.message << std::endl;
	}

	void onClientSocketClose(void* sender) 
	{
		debugL("EchoServer", this) << "On close" << std::endl;
		releaseSocket(reinterpret_cast<SocketT*>(sender));
	}

	void releaseSocket(SocketT* sock) 
	{		
		for (typename SocketT::List::iterator it = sockets.begin(); it != sockets.end(); ++it) {
			if (sock == &(*it)) { 
				debugL("EchoServer", this) << "Removing: " << sock<< std::endl;

				// All we need to do is erase the socket in order to 
				// deincrement the ref counter and destroy the socket.
				assert(sock->base().refCount() == 1);
				sockets.erase(it);
				return;
			}
		}
	}
};
	

// Some generic server types
typedef EchoServer<TCPSocket> TCPEchoServer;
typedef EchoServer<SSLSocket> SSLEchoServer;


} } // namespace scy::net