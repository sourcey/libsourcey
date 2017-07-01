#include "scy/application.h"
#include "scy/net/sslmanager.h"
#include "scy/socketio/client.h"
#include "scy/socketio/transaction.h"
#include "scy/util.h"


using namespace std;
using namespace scy;
using namespace scy::net;
using namespace scy::util;


namespace scy {
namespace sockio {


#define SERVER_HOST "localhost"
#define SERVER_PORT 4444 // 443
#define USE_SSL 0        // 1


// ----------------------------------------------------------------------------
// SocketIO Client Test
//
class Tests
{
    Application app;

public:
    Tests()
    {
        // #if USE_SSL
        //         // Init SSL Context
        //         SSLContext::Ptr ptrContext = new
        //         SSLContext(SSLContext::CLIENT_USE, "", "", "",
        //             SSLContext::VERIFY_NONE, 9, true,
        //             "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
        //         SSLManager::instance().initializeClient(ptrContext);
        //
        //         sockio::SSLClient client(app.loop);
        // #else
        //         sockio::TCPClient client(app.loop);
        // #endif
        //
        //         client.StateChange += sdelegate(this,
        //         &Tests::onClientStateChange);
        //         client.connect(SERVER_HOST, SERVER_PORT);
        //
        //         app.run();
        //
        //         // TODO: Benchmarks
        //         // TODO: Transaction tests
        //
        // #if USE_SSL
        //     SSLManager::instance().shutdown();
        // #endif
        //         app.finalize();

        testClient();
    }


    ~Tests() { app.finalize(); }


    void testClient()
    {
        sockio::Client::Options options;
        options.host = SERVER_HOST;
        options.port = SERVER_PORT;

#if USE_SSL
        sockio::SSLClient client(options, app.loop);
#else
        sockio::TCPClient client(options, app.loop);
#endif

        client.StateChange += slot(this, &Tests::onClientStateChange);
        client.connect();

        // app.run();
        app.waitForShutdown(
            [](void* opaque) {
                reinterpret_cast<sockio::Client*>(opaque)->close();
            },
            &client);

        // TODO: Benchmarks
        // TODO: Transaction tests
    }

    void onClientStateChange(void* sender, sockio::ClientState& state,
                             const sockio::ClientState& oldState)
    {
        auto client = reinterpret_cast<sockio::Client*>(sender);
        LDebug("Connection state changed: ", state.toString())

        switch (state.id()) {
            case sockio::ClientState::Connecting:
                break;
            case sockio::ClientState::Connected:
                LDebug("Connected")
                break;
            case sockio::ClientState::Online:
                // TODO: Send message
                client->send("ping", "hello", true);
                break;
            case sockio::ClientState::Error:
                break;
        }
    }
};


} // namespace sockio
} // namespace scy


int main(int argc, char** argv)
{
    Logger::instance().add(new ConsoleChannel("debug", Level::Trace));
#if USE_SSL
    SSLManager::initNoVerifyClient();
#endif
    {
        scy::sockio::Tests run;
    }
#if USE_SSL
    SSLManager::instance().shutdown();
#endif
    Logger::destroy();
    return 0;
}
