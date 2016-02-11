#include "Poco/Thread.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketReactor.h"
#include "Poco/Net/SocketNotification.h"
#include "Poco/Net/SocketAcceptor.h"
#include "scy/logger.h"
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

        Sourcey::Log("trace") << "[EchoServiceHandler:" << this << "] " << _id << ": EchoServiceHandler: " << socket.available() << std::endl;

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
        Sourcey::Log("debug") << "[EchoServiceHandler:" << this << "] " << _id << ": onReadable" << std::endl;

        numReadableNotification++;
        
        pNf->release();
        char buffer[8];
        int n = _socket.receiveBytes(buffer, sizeof(buffer));
        if (n > 0)
        {
            std::stringstream ss;
            ss << _id;
            _socket.sendBytes(ss.str().data(), ss.str().length());
            Sourcey::Log("debug") << "[EchoServiceHandler:" << this << "] " << _id << ": Sent" << std::endl;
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

    Poco::std::uint16_t port() const
    {
        return _socket.address().port();
    }
    
    void run()
    {        
        Sourcey::Log("debug") << "[UVEchoServer:" << this << "] Liistening: " << _socket.address().toString() << std::endl;
        _socket.listen(1000);
        Poco::Net::SocketAcceptor<EchoServiceHandler> acceptor(_socket, _reactor);
        Sourcey::Log("debug") << "[UVEchoServer:" << this << "] Liistening 1: " << _socket.address().toString() << std::endl;
        _ready.set();
        Sourcey::Log("debug") << "[UVEchoServer:" << this << "] Liistening 2: " << _socket.address().toString() << std::endl;
        _reactor.run();

    }

protected:
    Poco::Net::ServerSocket _socket;
    Poco::Net::SocketReactor _reactor;
    Poco::Thread _thread;
    Poco::Event  _ready;
};