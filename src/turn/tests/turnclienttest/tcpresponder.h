#ifndef TURN_TCPresponder_TEST_H
#define TURN_TCPresponder_TEST_H


#include "scy/logger.h"
#include "scy/time.h"
#include "scy/util.h"
#include "scy/net/tcpsocket.h"
#include <string>


using namespace std;


namespace scy {


class TCPResponder : public net::SocketAdapter
{
public:
    int id;
    net::Address relayedAddr;
    net::TCPSocket socket;
    Timer timer;

    TCPResponder(int id)
        : id(id)
        , timer(1000, 1000)
    {
        DebugS(this) << id << ": Creating" << endl;

        socket.setReceiver(this);
    }

    virtual ~TCPResponder()
    {
        DebugS(this) << id << ": Destroying" << endl;
        socket.removeReceiver(this);
    }

    void connect(const net::Address& relayAddr)
    {
        DebugS(this) << id << ": Starting on: " << relayAddr << endl;

        try {
            this->relayedAddr = relayAddr;

            // Since we extend SocketAdapter socket callbacks
            // will be received below.
            socket.connect(relayAddr);
        } catch (std::exception& exc) {
            ErrorS(this) << id << ": ERROR: " << exc.what() << endl;
            assert(false);
        }
    }

    void shutdown()
    {
        socket.close();
        timer.stop();
    }
    
    void onSocketConnect(net::Socket&)
    {
        // Send some early media to client
        // sendPacketToInitiator();

        // Start the send timer
        // timer.Timeout += slot(this, &TCPResponder::onSendTimer);
        // timer.start();
    }

    void onSocketRecv(net::Socket&, const MutableBuffer& buffer, const net::Address& peerAddress)
    {
        DebugS(this) << id << ": On recv: " << peerAddress << ": " << buffer.size() << endl;

        // Echo data back to client
        socket.send(bufferCast<const char*>(buffer), buffer.size());
    }

    void onSocketError(net::Socket&, const scy::Error& error)
    {
        DebugS(this) << id << ": On error: " << error.message << std::endl;
    }

    void onSocketClose(net::Socket&)
    {
        DebugS(this) << id << ": On close" << std::endl;
    }

#if 0
    void sendPacketToInitiator()
    {
        std::string payload;

        // Send the unix ticks milisecond for checking latency
        // payload.append(":");
        // payload.append(util::itostr(time::ticks()));

        // Send large packets to test throttling
        // payload.append(65536, 'x');

        payload.append(10000, 'x');
        socket.send(payload.c_str(), payload.length());
    }

    void onSendTimer() 
    { 
        sendPacketToInitiator();
        timer.stop();
    }
#endif
};


} //  namespace scy


#endif // TURN_TCPresponder_TEST_H
