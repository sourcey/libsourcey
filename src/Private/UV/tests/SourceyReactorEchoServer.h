#include "Poco/Thread.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/ServerSocket.h"
#include "Reactor.h"
#include "Poco/Net/SocketNotification.h"
#include "Poco/Net/SocketAcceptor.h"
#include "Sourcey/Logger.h"
#include "assert.h"


namespace Sourcey {
namespace Net {


int numEchoServiceHandler;
int numReadableNotification;


		/*	
class EchoServiceHandler
{
public:
	EchoServiceHandler(Poco::Net::StreamSocket& socket, Poco::Net::SocketReactor& reactor):
		_socket(socket),
		_reactor(reactor)
	{		
		numEchoServiceHandler++;
		_id = numEchoServiceHandler;

		Sourcey::Log("trace") << "[EchoServiceHandler:" << this << "] " << _id << ": EchoServiceHandler: " << socket.available() << std::endl;

		_reactor.addEventHandler(_socket, Poco::Observer<EchoServiceHandler, Poco::Net::ReadableNotification>(*this, &EchoServiceHandler::onReadable));
			
	
		// In high traffic situations the socket may become
		// writable with data in the read buffers, and the
		// readable notification get's missed. Let's flush 
		// the beffers now...
					
		if (socket.available() > 0) {
			numReadableNotification++;
			std::stringstream ss;
			ss << _id;
			_socket.sendBytes(ss.str().data(), ss.str().length());
			Sourcey::Log("debug") << "[EchoServiceHandler:" << this << "] " << _id << ": Sent" << std::endl;

			return;
			//std::cout << ">>>>>>>>>>>>>>>>>>>> " << std::endl; 
			//_socket.sendBytes("hitme", 5);
			//onReadable(rNf);
			Poco::Net::ReadableNotification* rNf = new Poco::Net::ReadableNotification(&reactor);
			onReadable(rNf);
		}

	}
		
	~EchoServiceHandler()
	{
		_reactor.removeEventHandler(_socket, Poco::Observer<EchoServiceHandler, Poco::Net::ReadableNotification>(*this, &EchoServiceHandler::onReadable));
	}
		
		
	void doit()
	{	
		printf("EchoServiceHandler:doit\n");
	}


	void onReadable(Poco::Net::ReadableNotification* pNf)
	{		
		Sourcey::Log("debug") << "[EchoServiceHandler:" << this << "] " << _id << ": onReadable" << std::endl;

		numReadableNotification++;
		
		//pNf->duplicate();
		//pNf->release();
		char buffer[8];
		int n = _socket.receiveBytes(buffer, sizeof(buffer));
		if (n > 0)
		{
			std::stringstream ss;
			ss << _id;
			_socket.sendBytes(ss.str().data(), ss.str().length());
			Sourcey::Log("debug") << "[EchoServiceHandler:" << this << "] " << _id << ": Sent" << std::endl;

			//std::cout << ">>>>>>>>>>>>>>>>>>>> " << std::endl; 	
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




class ReactorSocket: public IReactorHandler
{	
	virtual void onReadable() {
		Sourcey::Log("debug") << "[ReactorSocket:" << this << "] onReadable" << std::endl;
	};

	virtual void onWritable() {
		Sourcey::Log("debug") << "[ReactorSocket:" << this << "] onWritable" << std::endl;
	};

	virtual void onError() {
		Sourcey::Log("debug") << "[ReactorSocket:" << this << "] onError" << std::endl;
	};

	virtual void onIdle() {
		Sourcey::Log("debug") << "[ReactorSocket:" << this << "] onIdle" << std::endl;
	};
};
*/


struct ReactorSocket//: public IReactorHandler
{
	Poco::Net::StreamSocket socket;
	Sourcey::Net::Reactor& reactor;
	//std::vector<Poco::Net::StreamSocket> connections;

	ReactorSocket(Sourcey::Net::Reactor& reactor, const Poco::Net::StreamSocket& socket) : reactor(reactor),socket(socket)
	{
		reactor.attach(socket, reactorCallback(this, &ReactorSocket::onRead, SocketReadable));
	};
	
	/*
	virtual void onReactorEvent(ReactorEvent& event)  {		
		//assert(0);
		switch (event.type) {
		case SocketReadable:
			onRead(event);
			break;		
		case SocketWritable:
			assert(false);
			break;
		case SocketError:
			assert(false);
			break;
		default:
			assert(false);
			break;
		}
	}
	*/
		
	
	virtual void onRead(Reactor&, const Poco::Net::Socket& socket, const SocketEvent&) 
	{
		Sourcey::Log("debug") << "[PocoEchoServer:" << this << "] onRead" << std::endl;
		
		try
		{
			char buffer[256];
			int size = socket.impl()->receiveBytes(buffer, sizeof(buffer));
			//
			if (size > 0)
			{
				socket.impl()->sendBytes(buffer, size);
				reactor.detach(socket, reactorCallback(this, &ReactorSocket::onRead, SocketReadable));
			}
		}
		catch (Poco::Exception& exc)
		{
			std::cerr << "EchoServer: " << exc.displayText() << std::endl;
		}
		
		//assert(0);
		//Sourcey::Log("debug") << "[PocoEchoServer:" << this << "] onRead: 1" << std::endl;
		//Sleep(100);
		Sourcey::Log("debug") << "[PocoEchoServer:" << this << "] onRead: OK" << std::endl;
		//_reactor.detach(_socket, reactorCallback(this, &SourceyReactorEchoServer::onAccept, SocketReadable));	
	/*
		*/
	}

	/*
		connections.push_back(server.acceptConnection()); //impl()->
		Poco::Net::StreamSocket ss = connections.back(); //server.acceptConnection();
		Sourcey::Log("debug") << "[PocoEchoServer:" << this << "] acceptConnection" << std::endl;
		try
		{
			char buffer[256];
			int size = ss.receiveBytes(buffer, sizeof(buffer));
			while (size > 0)
			{
				ss.sendBytes(buffer, size);

				// read once
				break;
				//n = ss.receiveBytes(buffer, sizeof(buffer));

			}
		}
		catch (Poco::Exception& exc)
		{
			std::cerr << "EchoServer: " << exc.displayText() << std::endl;
		}
	virtual void onWritable(ReactorEvent& n) {
		Sourcey::Log("debug") << "[ReactorSocket:" << this << "] onWritable" << std::endl;
	};

	virtual void onError(ReactorEvent& n) {
		Sourcey::Log("debug") << "[ReactorSocket:" << this << "] onError" << std::endl;
	};

	virtual void onIdle() {
		Sourcey::Log("debug") << "[ReactorSocket:" << this << "] onIdle" << std::endl;
	};
*/
};


class SourceyReactorEchoServer: public Poco::Runnable//, public IReactorHandler
{
public:	
	SourceyReactorEchoServer(short port, Sourcey::Net::Reactor& reactor):
		_socket(Poco::Net::SocketAddress("127.0.0.1", port)),
		_thread("SourceyReactorEchoServer"),
		_reactor(reactor)
	{
		numEchoServiceHandler = 0;
		numReadableNotification = 0;
		_thread.start(*this);
		_ready.wait();
	}

	virtual ~SourceyReactorEchoServer()
	{
		_reactor.detach(_socket, reactorCallback(this, &SourceyReactorEchoServer::onAccept, SocketReadable));
		_reactor.stop();
		_ready.set();
		_thread.join();
	}

	Poco::UInt16 port() const
	{
		return _socket.address().port();
	}
	
	void run()
	{		
		Sourcey::Log("debug") << "[SourceyReactorEchoServer:" << this << "] Listening: " << _socket.address().toString() << std::endl;
		_ready.set();
		_reactor.attach(_socket, reactorCallback(this, &SourceyReactorEchoServer::onAccept, SocketReadable));	
		_ready.wait();	
		//_reactor.run();		
	}

	/*
	virtual void onReactorEvent(ReactorEvent& event)  {		
		//assert(0);
		switch (event.type) {
		case SocketReadable:
			onAccept(event);
			break;		
		case SocketWritable:
			assert(false);
			break;
		case SocketError:
			assert(false);
			break;
		default:
			assert(false);
			break;
		}
	}
	*/
	
	virtual void onAccept(Reactor&, const Poco::Net::Socket& socket, const SocketEvent& event) 
	{	
		Sourcey::Log("debug") << "[PocoEchoServer:" << this << "] acceptConnection: " << _socket.available() << std::endl;
		
		assert(_socket == socket);

		//Poco::Net::StreamSocket ss = _socket.acceptConnection();
		
		// Just in case we receive delayed notification
		//if (_socket.available() == 0)
		//	return;
		//if (_connections.size() == 1)
		//	return;

		//assert(_socket.available() > 0);

		//ss.close();

		//_reactor.detach(_socket, reactorCallback(this, &SourceyReactorEchoServer::onAccept, SocketReadable));	
		_connections.push_back(_socket.acceptConnection());
		Poco::Net::StreamSocket ss = _connections.back();	
		new ReactorSocket(_reactor, ss);
		//Sourcey::Log("debug") << "[PocoEchoServer:" << this << "] acceptConnection: 1" << std::endl;
		Sourcey::Log("debug") << "[PocoEchoServer:" << this << "] acceptConnection: OK" << std::endl;

	};


protected:
	Poco::Net::ServerSocket _socket;
	Sourcey::Net::Reactor& _reactor;
	std::vector<Poco::Net::StreamSocket> _connections;
	Poco::Thread _thread;
	Poco::Event  _ready;
};


} } // namespace Sourcey::Net