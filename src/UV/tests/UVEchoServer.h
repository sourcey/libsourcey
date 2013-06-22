#include "Sourcey/UV/UVPP.h"
#include "Sourcey/UV/TCPSocket.h"
#include "Sourcey/UV/SSLSocket.h"
#include "Sourcey/Net/Address.h"
#include "Sourcey/Logger.h"
#include <memory>


namespace scy {
namespace UV {


template <class SocketT>
class UVEchoServer//: public SocketT
{
public:		

	UVEchoServer(short port):
		_address(Poco::Net::SocketAddress("127.0.0.1", port)),
		//_thread("UVEchoServer"),
		_stopped(false)
	{
		//_thread.start(*this);
		//_ready.wait();
		//uv_unref(_socket.loop());
	}

	~UVEchoServer()
	{
		//uv_unref(_socket.loop());
		_stopped = true;
		//_thread.join();
	}

	Poco::UInt16 port() const
	{
		return _socket.address().port();
	}
	
	void run()
	{	
		//DefaultLoop
		_socket.bind(_address);
		_socket.listen();
		_socket.OnAcceptConnection += scy::delegate(this, &UVEchoServer::onAccept);
		//_ready.set();
		//while (!_stopped)
		//{
		//	Sleep(20);
		//}
		//uv_unref(_socket.loop());
		//uv_run(_socket.loop());
	}
	//typename SocketPtr TCPSocket::Ptr;

	std::vector<scy::UV::TCPSocket::Ptr> sockets;
	//typedef
	
	void onAccept(void* sender, scy::UV::TCPSocket::Ptr& conn) //, bool& reg
	{	
		scy::Log("debug") << "[UVEchoServer:" << this << "] onAccept: " << sender << std::endl;
		conn.OnRecv += scy::delegate(this, &UVEchoServer::onRead);
		conn.OnClose += scy::delegate(this, &UVEchoServer::onClose);
		sockets.push_back(conn);
		//SocketPtr ptr(conn);
		//reg = true;
		// NOTE: Not handling pointers
	}

	void onClose(void* sender, int error) 
	{
		SocketT* socket = reinterpret_cast<SocketT*>(sender);	
		for (unsigned i = 0; i < sockets.size(); i++) {
			if (socket == reinterpret_cast<SocketT*>(&sockets[i])) {
				socket->release();
			}
		}
	}


	void onRead(void* sender, const char* data, int len) 
	{
		scy::Log("debug") << "[UVEchoServer:" << this << "] onRead: " << sender << std::endl;
		scy::UV::TCPSocket* socket = reinterpret_cast<scy::UV::TCPSocket*>(sender);
		socket->send(data, len);
	}

protected:
	SocketT _socket;
	scy::Net::Address _address;
	bool         _stopped;
};


} } // namespace scy::UV


/*
	//Poco::Thread _thread;
	//Poco::Event  _ready;

		//_socket.bind(scy::Net::Address("127.0.0.1", port));
		//_ready.set();
		//SocketAcceptor<EchoServiceHandler> acceptor(_socket, _reactor);
		//_reactor.run();
	//Poco::Net::ServerSocket _socket;
	//Poco::Net::SocketReactor _reactor;

class UVEchoServer: public TCPSocket, public Poco::Runnable
{
public:
	
	UVEchoServer()
	{	
		_thread.start(*this);
	}
	

	UV~UVEchoServer()
	{
	}

	
	virtual void run()
	{
	}


protected:
	scy::Thread _thread;
};

*/

