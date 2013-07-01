#include "Poco/Thread.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketReactor.h"
#include "Poco/Net/SocketNotification.h"
#include "Poco/Net/SocketAcceptor.h"
#include "Sourcey/Logger.h"
#include "assert.h"


int numEchoServiceHandler;
int numReadableNotification;


class EchoServiceHandler
{
public:
	EchoServiceHandler(Poco::Net::StreamSocket& socket, Poco::Net::SocketReactor& reactor):
		_socket(socket),
		_reactor(reactor)
	{		
		numEchoServiceHandler++;
		_id = numEchoServiceHandler;

		scy::Log("trace") << "[EchoServiceHandler:" << this << "] " << _id << ": EchoServiceHandler: " << socket.available() << std::endl;

		_reactor.addEventHandler(_socket, Poco::Observer<EchoServiceHandler, Poco::Net::ReadableNotification>(*this, &EchoServiceHandler::onReadable));
			
		/*		
		// In high traffic situations the socket may become
		// writable with data in the read buffers, and the
		// readable notification get's missed. Let's flush 
		// the beffers now...					
		if (socket.available() > 0) {
			Poco::Net::ReadableNotification* rNf = new Poco::Net::ReadableNotification(&reactor);
			onReadable(rNf);
		}
		*/
	}
		
	~EchoServiceHandler()
	{
		_reactor.removeEventHandler(_socket, Poco::Observer<EchoServiceHandler, Poco::Net::ReadableNotification>(*this, &EchoServiceHandler::onReadable));
	}


	void onReadable(Poco::Net::ReadableNotification* pNf)
	{		
		scy::Log("debug") << "[EchoServiceHandler:" << this << "] " << _id << ": onReadable" << std::endl;

		numReadableNotification++;
		
		pNf->release();
		char buffer[8];
		int n = _socket.receiveBytes(buffer, sizeof(buffer));
		if (n > 0)
		{
			std::stringstream ss;
			ss << _id;
			_socket.sendBytes(ss.str().data(), ss.str().length());
			scy::Log("debug") << "[EchoServiceHandler:" << this << "] " << _id << ": Sent" << std::endl;
		}
		else
		{
			_socket.shutdownSend();
			delete this;
		}
	}
		
private:
	int   _id;
	Poco::Net::StreamSocket   _socket;
	Poco::Net::SocketReactor& _reactor;
};


class PocoReactorEchoServer: public Poco::Runnable
{
public:	
	PocoReactorEchoServer(short port):
		_socket(Poco::Net::SocketAddress("127.0.0.1", port)),
		_thread("PocoReactorEchoServer")
	{
		numEchoServiceHandler = 0;
		numReadableNotification = 0;
		_thread.start(*this);
		_ready.wait();
	}

	virtual ~PocoReactorEchoServer()
	{
		_reactor.stop();
		_thread.join();
	}

	Poco::UInt16 port() const
	{
		return _socket.address().port();
	}
	
	void run()
	{		
		scy::Log("debug") << "[UVEchoServer:" << this << "] Liistening: " << _socket.address() << std::endl;
		_socket.listen(1000);
		Poco::Net::SocketAcceptor<EchoServiceHandler> acceptor(_socket, _reactor);
		scy::Log("debug") << "[UVEchoServer:" << this << "] Liistening 1: " << _socket.address() << std::endl;
		_ready.set();
		scy::Log("debug") << "[UVEchoServer:" << this << "] Liistening 2: " << _socket.address() << std::endl;
		_reactor.run();

	}

protected:
	Poco::Net::ServerSocket _socket;
	Poco::Net::SocketReactor _reactor;
	Poco::Thread _thread;
	Poco::Event  _ready;
};