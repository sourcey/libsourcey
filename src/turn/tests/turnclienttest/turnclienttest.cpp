#include "udpinitiator.h"
#include "udpresponder.h"
#include "tcpinitiator.h"
#include "tcpresponder.h"
#include "turnserver.h"
#include "scy/logger.h"
#include "scy/util.h"

#include <iostream>


using namespace std;
using namespace scy;
using namespace scy::test;


struct TCPObserver : public turn::TCPClientObserver
{
    TCPObserver()
    {
    }

    virtual ~TCPObserver()
    {
    }

    void onClientStateChange(turn::Client&, turn::ClientState& state, const turn::ClientState&)
    {
    }

    void onRelayConnectionCreated(turn::TCPClient&, const net::TCPSocket::Ptr& socket, const net::Address& peerAddr)
    {
    }

    void onRelayConnectionClosed(turn::TCPClient&, const net::TCPSocket::Ptr& socket, const net::Address& peerAddr)
    {
    }

    void onRelayDataReceived(turn::Client&, const char* data, size_t size, const net::Address& peerAddr)
    {
    }

    void onAllocationPermissionsCreated(turn::Client&, const turn::PermissionList& permissions)
    {
    }
};


int main(int argc, char** argv)
{
#ifdef _MSC_VER
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    // Logger::instance().add(new ConsoleChannel("debug", LTrace));
    // Logger::instance().setWriter(new AsyncLogWriter);
    test::init();

    turn::Client::Options co;
    co.serverAddr = net::Address(TURN_SERVER_IP, TURN_SERVER_PORT);
    co.username = TURN_SERVER_USERNAME;
    co.password = TURN_SERVER_PASSWORD;
    co.lifetime = 120 * 1000; // 1 minute
    co.timeout = 10 * 1000;
    co.timerInterval = 3 * 1000;

    // {
    //     TCPInitiator initiator(0, co);
    //     TCPResponder responder(0);
    //     // UDPInitiator initiator(0, co);
    //     // UDPResponder responder(0);
    //
    //     initiator.initiate(TURN_AUTHORIZE_PEER_IP);
    // }
    {
        // turn::Client client(*new TCPObserver, co, net::makeSocket<net::TCPSocket>());
        // client.addPermission("127.0.0.1");
        // client.initiate();

        // turn::UDPClient client(*new TCPObserver);
        // client.addPermission("127.0.0.1");
        // client.initiate();


        // Timer timer;
        // net::SocketEmitter socket(net::makeSocket<net::TCPSocket>());
        // socket->connect("127.0.0.1", 80);

        // Timer timer(1,1);
        // timer.setInterval(1);
        // timer.start([] {});

        net::TCPSocket socket;
        socket.connect("127.0.0.1", 80);

        // TCPInitiator initiator(0, co);
        // TCPResponder responder(0);
        // UDPInitiator initiator(0, co);
        // UDPResponder responder(0);

        // initiator.initiate(TURN_AUTHORIZE_PEER_IP);

        // turn::TCPClient client(*new TCPObserver);
        // client.addPermission("127.0.0.1");
        // client.initiate();
    }

    {
        // turn::Client client(*new TCPObserver, co, net::makeSocket<net::UDPSocket>());
        // client.addPermission("127.0.0.1");
        // client.initiate();

        // Timer timer;
        // net::UDPSocket socket;
        // socket.bind(net::Address("0.0.0.0", 0));
        // socket.connect("127.0.0.1", 80);

        // net::SocketEmitter socket(net::makeSocket<net::UDPSocket>());
        // socket->bind(net::Address("0.0.0.0", 0));
        // socket->connect("127.0.0.1", 80);

        // Timer timer(1,1);
        // timer.start([] {});

        // turn::UDPClient client(*new TCPObserver);
        // client.addPermission("127.0.0.1");
        // client.initiate();

        // TCPInitiator initiator(0, co);
        // TCPResponder responder(0);
        // UDPInitiator initiator(0, co);
        // UDPResponder responder(0);

        // initiator.initiate(TURN_AUTHORIZE_PEER_IP);
    }

    // {
    //     // TCPInitiator initiator(0, co);
    //     // TCPResponder responder(0);
    //     UDPInitiator initiator(0, co);
    //     UDPResponder responder(0);
    //
    //     initiator.initiate(TURN_AUTHORIZE_PEER_IP);
    // }
    //
    // {
    //     TCPInitiator initiator(0, co);
    //     TCPResponder responder(0);
    //     // UDPInitiator initiator(0, co);
    //     // UDPResponder responder(0);
    //
    //     initiator.initiate(TURN_AUTHORIZE_PEER_IP);
    // }

//     // =========================================================================
//     // TURN TCP Client
//     //
//     describe("TURN TCP client", [&]() {
// #if RAISE_LOCAL_SERVER
//         RunTestServer()
// #endif
//         TCPInitiator initiator(0, co);
//         TCPResponder responder(0);
//
//         initiator.AllocationCreated += [&]()
//         {
//             DebugL << "Initiator allocation created" << endl;
//
//             // Start the responder when the allocation is created
//             responder.connect(initiator.client.relayedAddress());
//         };
//
//         initiator.TestComplete += [&](bool success)
//         {
//             DebugL << "Test complete: " << success << endl;
//             expect(success);
//             initiator.shutdown();
//             responder.shutdown();
// #if RAISE_LOCAL_SERVER
//             srv.stop();
// #endif
//         };
//
//         // TODO: Use STUN binding request to get IP
//         initiator.initiate(TURN_AUTHORIZE_PEER_IP);
//
//         uv::runDefaultLoop();
//     });
//
//
//     // =========================================================================
//     // TURN UDP Client
//     //
//     describe("TURN UDP client", [&]() {
// #if RAISE_LOCAL_SERVER
//         RunTestServer()
// #endif
//         UDPInitiator initiator(0, co);
//         UDPResponder responder(0);
//
//         initiator.AllocationCreated += [&]()
//         {
//             DebugL << "Initiator allocation created" << endl;
//
//             // Start the responder when the allocation is created
//             responder.connect(initiator.client.relayedAddress());
//
//             // Set the local responder address for UDP send indications
//             initiator.responderAddress = net::Address(
//                 TURN_AUTHORIZE_PEER_IP, responder.socket.address().port());
//         };
//
//         initiator.TestComplete += [&](bool success)
//         {
//             DebugL << "Test complete: " << success << endl;
//             expect(success);
//             initiator.shutdown();
//             responder.shutdown();
// #if RAISE_LOCAL_SERVER
//             srv.stop();
// #endif
//         };
//
//         // TODO: Use STUN binding request to get IP
//         initiator.initiate(TURN_AUTHORIZE_PEER_IP);
//
//         uv::runDefaultLoop();
//     });


    test::runAll();
    return test::finalize();
}
