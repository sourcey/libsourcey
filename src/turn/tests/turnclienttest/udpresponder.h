#ifndef TURN_UDPresponder_TEST_H
#define TURN_UDPresponder_TEST_H


#include "scy/logger.h"
#include "scy/net/udpsocket.h"
#include "scy/time.h"
#include "scy/timer.h"
#include "turnclienttest.h"


using namespace std;


namespace scy {



class UDPResponder : public net::SocketAdapter
{

public:
    int id;
    net::Address relayedAddr;
    net::UDPSocket socket;
    Timer timer;

    UDPResponder(int id)
        : id(id)
        , timer(1000, 1000)
    {
        DebugS(this) << id << ": Creating" << endl;

        socket.addReceiver(this);

        // socket.bind(net::Address(TURN_AUTHORIZE_PEER_IP, 4020));
        socket.bind(net::Address("0.0.0.0", 0));

        DebugS(this) << id << ": Listening on: " << socket.address() << endl;
    }

    virtual ~UDPResponder()
    {
        DebugS(this) << id << ": Destroying" << endl;
        socket.removeReceiver(this);
    }

    void connect(const net::Address& relayAddr)
    {
        DebugS(this) << id << ": Starting on: " << relayAddr << endl;

        try {
            this->relayedAddr = relayAddr;

            // socket.bind(net::Address("0.0.0.0", 0));
            // socket.bind(net::Address(TURN_AUTHORIZE_PEER_IP, 4020));
            // socket.bind(net::Address("0.0.0.0", 4020));
            socket.connect(relayAddr);
        } catch (std::exception& exc) {
            ErrorS(this) << id << ": ERROR: " << exc.what() << endl;
            assert(false);
        }
    }

    void shutdown()
    {
        timer.stop();
        socket.close();
    }

    void onSendTimer()
    {
        std::string payload(createLatencyCheck(30000));
        socket.send(payload.c_str(), payload.length());
    }

    void onSocketConnect(net::Socket&)
    {
//#if TEST_RESPONDER_TO_INITIATOR_LATENCY
//        timer.Timeout += slot(this, &UDPResponder::onSendTimer);
//        timer.start();
//#endif
    }

    void onSocketRecv(net::Socket&, const MutableBuffer& buffer, const net::Address& peerAddr)
    {
        std::string payload(bufferCast<const char*>(buffer), buffer.size());
        DebugS(this) << id << ": On recv: " << peerAddr << ": " << buffer.size() << std::endl;

        // Echo back to client
        socket.send(payload.c_str(), payload.size(), relayedAddr); // peerAddr
    }

    void onSocketError(net::Socket&, const scy::Error& error)
    {
        DebugS(this) << id << ": On error: " << error.message << std::endl;
    }

    void onSocketClose(net::Socket&)
    {
        DebugS(this) << id << ": On close" << std::endl;
        shutdown();
    }
};


} //  namespace scy


#endif // TURN_UDPresponder_TEST_H
