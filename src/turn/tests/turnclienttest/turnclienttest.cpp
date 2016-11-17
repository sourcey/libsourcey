#include "udpinitiator.h"
#include "udpresponder.h"

#include "tcpinitiator.h"
#include "tcpresponder.h"

#include "scy/application.h"
#include "scy/logger.h"
#include "scy/turn/client/client.h"
#include "scy/turn/server/server.h"
#include "scy/util.h"

#include <iostream>


using namespace std;
using namespace scy;
using namespace scy::net;
using namespace scy::turn;


namespace scy {
namespace turn {


struct TestServer : public turn::ServerObserver
{
    turn::Server* server;

    TestServer()
        : server(new turn::Server(*this))
    {
    }
    virtual ~TestServer() { delete server; }

    void run(const turn::ServerOptions& opts)
    {
        server->options()= opts;
        server->start();
    }

    void stop() { server->stop(); }

    AuthenticationState authenticateRequest(Server* server,
                                            const Request& request)
    {
        return Authorized;
    }

    void onServerAllocationCreated(Server* server, IAllocation* alloc)
    {
        DebugS(this) << "Allocation created" << endl;
    }

    void onServerAllocationRemoved(Server* server, IAllocation* alloc)
    {
        DebugS(this) << "Allocation removed" << endl;
    }
};


/// The initiating client...
struct ClientTest
{
    enum Result
    {
        None,
        Success,
        Failed
    };

    // Signal<Result&> TestComplete;

    int id;
    Result result;
#if TEST_TCP
    TCPInitiator* initiator;
    TCPResponder* responder;
    ClientTest(int id, const Client::Options& opts)
        : id(id)
        , result(None)
        , initiator(new TCPInitiator(id, opts))
        , responder(new TCPResponder(id))
    {
    }
#else
    UDPInitiator* initiator;
    UDPResponder* responder;
    ClientTest(int id, const Client::Options& opts)
        : id(id)
        , result(None)
        , initiator(new UDPInitiator(id, opts))
        , responder(new UDPResponder(id))
    {
    }
#endif

    ~ClientTest()
    {
        if (initiator)
            delete initiator;
        if (responder)
            delete responder;
    }

    void run()
    {
        initiator->AllocationCreated+=
            slot(this, &ClientTest::onInitiatorAllocationCreated);

        // TODO: Use STUN binding request to get IP
        initiator->initiate(TURN_AUTHORIZE_PEER_IP);
    }

    void onInitiatorAllocationCreated() //, turn::Client& client
    {
        DebugL << "Initiator allocation created" << endl;

        // Start the responder when the allocation is created
        responder->start(initiator->client.relayedAddress());

#if !TEST_TCP
        // Set the local responder address for UDP send indications
        initiator->responderAddress= net::Address(
            TURN_AUTHORIZE_PEER_IP, responder->socket.address().port());
#endif
    }

    void onTestComplete(void* sender, bool success)
    {
        DebugL << "Test complete: " << success << endl;
        result= success ? Success : Failed;
        // TestComplete.emit(/*this, */result);
    }
};


struct ClientTestRunner
{
    int nTimes;
    int nComplete;
    int nSucceeded;

    std::vector<turn::ClientTest*> tests;

    ClientTestRunner()
        : nTimes(0)
        , nComplete(0)
        , nSucceeded(0)
    {
    }
    ~ClientTestRunner()
    {
        // Timer::getDefault().stop(TimerCallback<ClientTestRunner>(this,
        // &ClientTestRunner::onTimeout));
        util::clearVector(tests);
    }

    void run(const Client::Options& o, int times, int timeout)
    {
        // if (timeout)
        //    Timer::getDefault().start(TimerCallback<ClientTestRunner>(this,
        //    &ClientTestRunner::onTimeout, timeout));

        nTimes= times;
        for (int i= 0; i < nTimes; i++) {
            auto client= new turn::ClientTest(i, o); //, reactor, runner
            client->initiator->TestComplete+=
                slot(this, &ClientTestRunner::onTestComplete);
            client->run();
            tests.push_back(client);
        }

        // done.wait();
    }

    // void onTimeout(TimerCallback<ClientTestRunner>& timer)
    // {
    //     // took too long...
    //     done.set();
    // }

    void onTestComplete(bool result)
    {
        DebugL << "ClientTestRunner: TestComplete" << endl;

        nComplete++;
        if (result)
            nSucceeded++;
        // if (nComplete == nTimes)
        //    done.set();
        print(cout);
    }

    void print(ostream& ost)
    {
        ost << "Test results:"
            << "\n\tTimes: " << nTimes << "\n\tComplete: " << nComplete
            << "\n\tSucceeded: " << nSucceeded << endl;
    }
};


struct TestTCPClientObserver : public TCPClientObserver
{
    void onClientStateChange(turn::Client& client, turn::ClientState& state,
                             const turn::ClientState&)
    {
        DebugS(this) << "State change: " << state.toString() << endl;
    }

    void onTimer(TCPClient& client) { DebugS(this) << "onTimer" << endl; }

    void onRelayConnectionCreated(
        TCPClient& client, const net::TCPSocket::Ptr& socket,
        const net::Address& peerAddr) // std::uint32_t connectionID,
    {
        DebugS(this) << "Relay Connection Created: " << peerAddr << endl;
    }

    void onRelayConnectionClosed(TCPClient& client,
                                 const net::TCPSocket::Ptr& socket,
                                 const net::Address& peerAddr)
    {
        DebugS(this) << "Connection Closed" << endl;
    }

    void onRelayConnectionDataReceived(turn::Client& client, const char* data,
                                       std::size_t size,
                                       const net::Address& peerAddr)
    {
        DebugS(this) << "Received Data: " << std::string(data, size) << ": "
                     << peerAddr << endl;
    }

    void onAllocationPermissionsCreated(turn::Client& client,
                                        const turn::PermissionList& permissions)
    {
        DebugS(this) << "Permissions Created" << endl;
    }
};
}
} //  namespace scy::turn


/*
static void onKillSignal(uv_signal_t *req, int signum)
{
    ((turn::TestServer*)req->data)->stop();
    uv_signal_stop(req);
}
*/


int main(int argc, char** argv)
{
#ifdef _MSC_VER
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    Logger::instance().add(new ConsoleChannel("debug", LTrace));
    // Logger::instance().setWriter(new AsyncLogWriter);
    {
        Application app;
        try {
#if RAISE_LOCAL_SERVER
            DebugL << "Running Test Server" << endl;
            turn::ServerOptions so;
            so.software= "Sourcey STUN/TURN Server [rfc5766]";
            so.realm= "sourcey.com";
            so.allocationDefaultLifetime= 1 * 60 * 1000;
            so.allocationMaxLifetime= 15 * 60 * 1000;
            // so.allocationDefaultLifetime        = 1 * 60 * 1000; // 1 min
            // so.allocationMaxLifetime            = 5 * 1000; // 5 seccs
            so.timerInterval= 5 * 1000; // 5 seccs
            so.listenAddr= net::Address("127.0.0.1", 3478);
            so.enableUDP= false;

            DebugL << "Binding Test Server" << endl;
            turn::TestServer srv;
            DebugL << "Binding Test Server: 1" << endl;
            srv.run(so);
            DebugL << "Binding Test Server: OK" << endl;
#endif

            //
            // Initialize clients
            {
                DebugL << "Running Client Tests" << endl;
                turn::Client::Options co;
                co.serverAddr= net::Address(TURN_SERVER_IP, TURN_SERVER_PORT);
                co.username= TURN_SERVER_USERNAME;
                co.password= TURN_SERVER_PASSWORD;
                co.lifetime= 120 * 1000; // 1 minute
                co.timeout= 10 * 1000;
                co.timerInterval= 3 * 1000;

                turn::ClientTestRunner test;
                test.run(co, 1, 10000);
                DebugL << "Running Client Tests: OK" << endl;
                app.waitForShutdown();
                test.print(cout);
            }
        } catch (std::exception& exc) {
            ErrorL << exc.what() << endl;
        }

        DebugL << "Finalizing" << endl;
        app.finalize();
        DebugL << "Finalizing: OK" << endl;
    }
    scy::Logger::destroy();
    return 0;
}
