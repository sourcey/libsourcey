#include "Poco/Thread.h"
#include "Sourcey/Net/Address.h"
#include "Sourcey/Base/UVPP.h"
#include "TCPContext.h"
#include "Sourcey/Base/Logger.h"


class UVEchoServer: public Poco::Runnable
{
public:	
	UVEchoServer(short port, UV::Loop* loop = DefaultLoop):
		_socket(loop),
		_address(Poco::Net::SocketAddress("127.0.0.1", port)),
		_thread("UVEchoServer"),
		_stop(false)
	{
		_thread.start(*this);
		_ready.wait();
	}

	~UVEchoServer()
	{
		//_reactor.stop();	
		assert(_socket.loop() != DefaultLoop);
		uv_unref(_socket.loop());
		_stop = true;
		_thread.join();
	}

	Poco::UInt16 port() const
	{
		return 0; //_socket.address().port();
	}
	
	void run()
	{	
		//DefaultLoop
		_socket.bind(_address);
		_socket.listen();
		_socket.OnAcceptConnection += Sourcey::delegate(this, &UVEchoServer::onAccept);
		_ready.set();
		//while (!_stop)
		//{
		//	Sleep(20);
		//}
		//uv_unref(_socket.loop());
		uv_run(_socket.loop());
	}
	
	void onAccept(void* sender, Sourcey::Net::TCPContext* conn)
	{	
		Sourcey::Log("debug") << "[UVEchoServer:" << this << "] onAccept: " << sender << std::endl;

		conn->OnRead += Sourcey::delegate(this, &UVEchoServer::onRead);
	}

	void onRead(void* sender, char* data, int size) 
	{
		Sourcey::Log("debug") << "[UVEchoServer:" << this << "] onRead: " << sender << std::endl;

		Sourcey::Net::TCPContext* socket = reinterpret_cast<Sourcey::Net::TCPContext*>(sender);
		socket->send(data, size);
	}

protected:
	Sourcey::Net::TCPContext _socket;
	Poco::Net::SocketAddress _address;
	Poco::Thread _thread;
	Poco::Event  _ready;
	bool         _stop;
};


/*

		//_socket.bind(Sourcey::Net::Address("127.0.0.1", port));
		//_ready.set();
		//SocketAcceptor<EchoServiceHandler> acceptor(_socket, _reactor);
		//_reactor.run();
	//Poco::Net::ServerSocket _socket;
	//Poco::Net::SocketReactor _reactor;

class UVEchoServer: public TCPContext, public Poco::Runnable
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
	Sourcey::Thread _thread;
};

*/

