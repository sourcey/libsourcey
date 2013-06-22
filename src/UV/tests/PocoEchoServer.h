#include "Poco/Thread.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/ServerSocket.h"
#include "Sourcey/Logger.h"


class PocoEchoServer: public Poco::Runnable
{
public:	
	PocoEchoServer(short port):
		_socket(Poco::Net::SocketAddress("127.0.0.1", port)),
		_thread("PocoEchoServer"),
		_stopped(false)
	{
		_thread.start(*this);
		_ready.wait();
	}


	~PocoEchoServer()
	{
		_stopped = true;
		_thread.join();
	}

	Poco::UInt16 port() const
	{
		return _socket.address().port();
	}
	
	void run()
	{
		_ready.set();
		Poco::Timespan span(250000);
		std::vector<Poco::Net::StreamSocket> sockets;
		while (!_stopped)
		{			
			Poco::Net::Socket::SocketList readable;
			Poco::Net::Socket::SocketList writable;
			Poco::Net::Socket::SocketList except;
			
			readable.push_back(_socket);

			if (Poco::Net::Socket::select(readable, writable, except, span))
			{
				sockets.push_back(_socket.acceptConnection());
				Poco::Net::StreamSocket ss = sockets.back(); //_socket.acceptConnection();
				scy::Log("debug") << "[PocoEchoServer:" << this << "] acceptConnection" << std::endl;
				try
				{
					char buffer[256];
					int n = ss.receiveBytes(buffer, sizeof(buffer));
					while (n > 0 && !_stopped)
					{
						ss.sendBytes(buffer, n);

						// read once
						break;
						//n = ss.receiveBytes(buffer, sizeof(buffer));

					}
				}
				catch (Poco::Exception& exc)
				{
					std::cerr << "EchoServer: " << exc.displayText() << std::endl;
				}
				/*
				onBusy();

				for (Socket::SocketList::iterator it = readable.begin(); it != readable.end(); ++it)
					emit(*it, _pReadableNotification);
				for (Socket::SocketList::iterator it = writable.begin(); it != writable.end(); ++it)
					emit(*it, _pWritableNotification);
				for (Socket::SocketList::iterator it = except.begin(); it != except.end(); ++it)
					emit(*it, _pErrorNotification);
					*/
			} 
			/*
			if (_socket.poll(span, Poco::Net::Socket::SELECT_READ))
			{
				sockets.push_back(_socket.acceptConnection());
				Poco::Net::StreamSocket ss = sockets.back(); //_socket.acceptConnection();
				scy::Log("debug") << "[PocoEchoServer:" << this << "] acceptConnection" << std::endl;
				try
				{
					char buffer[256];
					int n = ss.receiveBytes(buffer, sizeof(buffer));
					while (n > 0 && !_stopped)
					{
						ss.sendBytes(buffer, n);

						// read once
						break;
						//n = ss.receiveBytes(buffer, sizeof(buffer));

					}
				}
				catch (Poco::Exception& exc)
				{
					std::cerr << "EchoServer: " << exc.displayText() << std::endl;
				}
			}
			*/
		}
	}

protected:
	Poco::Net::ServerSocket _socket;
	Poco::Thread _thread;
	Poco::Event  _ready;
	bool         _stopped;
};


