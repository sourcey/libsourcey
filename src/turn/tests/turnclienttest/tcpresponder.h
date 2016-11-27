#ifndef TURN_TCPresponder_TEST_H
#define TURN_TCPresponder_TEST_H


#include "scy/logger.h"
#include "scy/net/tcpsocket.h"
#include <string>


using namespace std;


namespace scy {
namespace turn {


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
        // net::SocketAdapter::socket = &socket;
        DebugS(this) << id << ": Creating" << endl;

        socket.addReceiver(this);
    }

    virtual ~TCPResponder()
    {
        // socket.base().removeObserver(this);
        DebugS(this) << id << ": Destroying" << endl;

        socket.removeReceiver(this);
        stop();
    }

    void start(const net::Address& relayedAddr)
    {
        DebugS(this) << id << ": Starting on: " << relayedAddr << endl;

        try {
            this->relayedAddr = relayedAddr;

            // Since we extend SocketAdapter socket callbacks
            // will be received below.
            socket.connect(relayedAddr);
        } catch (std::exception& exc) {
            errorL("TCPResponder", this) << id << ": ERROR: " << exc.what()
                                         << endl;
            assert(false);
        }
    }

    void stop() { timer.stop(); }

    void onSocketConnect(net::Socket& socket)
    {
        // Send some early media to client
        sendLatencyCheck();

        // Start the send timer
        timer.Timeout += slot(this, &TCPResponder::onSendTimer);
        timer.start();
    }

    void onSocketRecv(net::Socket& socket, const MutableBuffer& buffer,
                      const net::Address& peerAddress)
    {
        // assert(&packet.info->socket == &socket);
        std::string payload(bufferCast<const char*>(buffer), buffer.size());
        DebugS(this) << id << ": On recv: " << peerAddress << ": " << payload
                     << std::endl;

        // assert(0 && "ok");
        // assert(payload == "hello peer");

        // Echo back to client
        // socket.send(payload.c_str(), payload.size());
    }

    void onSocketError(net::Socket& socket, const scy::Error& error)
    {
        DebugS(this) << id << ": On error: " << error.message << std::endl;
    }

    void onSocketClose(net::Socket& socket)
    {
        DebugS(this) << id << ": On close" << std::endl;
        stop();
    }

    void sendLatencyCheck()
    {
        std::string payload;

        // // Send the unix ticks milisecond for checking latency
        // //payload.append(":");
        // payload.append(util::itostr(time::ticks()));

        // Send a large packets to test throttling
        // payload.append(65536, 'x');
        payload.append(10000, 'x'); /// Send it
        socket.send(payload.c_str(), payload.length());
    }

    void onSendTimer() { sendLatencyCheck(); }
};
}
} //  namespace scy::turn


#endif // TURN_TCPresponder_TEST_H
