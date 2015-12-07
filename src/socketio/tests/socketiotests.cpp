#include "scy/socketio/client.h"
#include "scy/socketio/transaction.h"
#include "scy/net/sslmanager.h"
#include "scy/application.h"
#include "scy/util.h"


using namespace std;
using namespace scy;
using namespace scy::net;
using namespace scy::util;


/*
// Detect Memory Leaks
#ifdef _DEBUG
#include "MemLeakDetect/MemLeakDetect.h"
#include "MemLeakDetect/MemLeakDetect.cpp"
CMemLeakDetect memLeakDetect;
#endif
*/


namespace scy {
namespace sockio {


#define SERVER_HOST "localhost"
#define SERVER_PORT 88 //443
#define USE_SSL     0 //1


// ----------------------------------------------------------------------------
// SocketIO Client Test
//
class Tests
{
    Application app;

public:
    Tests()
    {
#if USE_SSL
        // Init SSL Context
        SSLContext::Ptr ptrContext = new SSLContext(SSLContext::CLIENT_USE, "", "", "",
            SSLContext::VERIFY_NONE, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
        SSLManager::instance().initializeClient(ptrContext);

        sockio::SSLClient client(app.loop);
#else
        sockio::TCPClient client(app.loop);
#endif

        client.StateChange += sdelegate(this, &Tests::onClientStateChange);
        client.connect(SERVER_HOST, SERVER_PORT);

        app.run();

        // TODO: Benchmarks
        // TODO: Transaction tests

#if USE_SSL
    SSLManager::instance().shutdown();
#endif
        app.finalize();
    }

    void onClientStateChange(void* sender, sockio::ClientState& state, const sockio::ClientState& oldState)
    {
        //audo client = reinterpret_cast<sockio::Client*>(sender);    
        DebugL << "Connection state changed: " << state.toString() << endl;

        switch (state.id()) {
        case sockio::ClientState::Connecting:
            break;
        case sockio::ClientState::Connected:
            DebugL << "Connected" << endl;
            break;
        case sockio::ClientState::Online:
            // TODO: Send message
            break;
        case sockio::ClientState::Error:
            break;
        }
    }
};


} } // namespace scy::sockio


int main(int argc, char** argv)
{
    Logger::instance().add(new ConsoleChannel("debug", LTrace));
    {
        scy::sockio::Tests run;
    }
    Logger::destroy();
    return 0;
}
