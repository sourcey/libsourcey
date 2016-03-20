#include "sympletests.h"


using namespace std;
using namespace scy;
using namespace scy::test;


#define SERVER_HOST "localhost"
#define USE_SSL     0
#if USE_SSL
#define SERVER_PORT 443
#else
#define SERVER_PORT 4500
#endif


int main(int argc, char** argv)
{
    Logger::instance().add(new ConsoleChannel("debug", LTrace));

#if USE_SSL
    SSLManager::initNoVerifyClient();
#endif

    // =========================================================================
    // Address
    //
    describe("address", []() {
        smpl::Address a1("user|");
        assert(a1.user == "user");
        assert(a1.id == "");
        assert(a1.valid());

        smpl::Address a2("user");
        assert(a2.user == "user");
        assert(a2.id == "");
        assert(a2.valid());

        smpl::Address a3("");
        assert(a3.user == "");
        assert(a3.id == "");
        assert(!a3.valid());

        smpl::Address a4("|567257247245275");
        assert(a4.user == "");
        assert(a4.id == "567257247245275");
        assert(a4.valid());
    });

    // =========================================================================
    // Client
    //
    describe("client", []() {
        smpl::Client::Options loptions;
        loptions.host = SERVER_HOST;
        loptions.port = SERVER_PORT;
        loptions.user = "george";
        loptions.name = "George";

        // NOTE: The server should allow anonymous
        // authentication for this test.
        // options.token = ""; used for authentication

        smpl::Client::Options roptions;
        roptions.host = SERVER_HOST;
        roptions.port = SERVER_PORT;
        roptions.user = "ringo";
        roptions.name = "Ringo";

        static TestClient lclient(roptions);
        static TestClient rclient(roptions);

        while(!lclient.completed() && !rclient.completed()) {
            // DebugL << "waiting for test completion" << std::endl;
            uv::runDefaultLoop(UV_RUN_ONCE);

            // TODO: check client failed status
        }
    });

    // TODO:
    //  - Obtain authentication token
    //  - Transaction test
    //  - Presence test
    //  - Ack test
    //  - Benchmarks

    test::runAll();

#if USE_SSL
    SSLManager::instance().shutdown();
#endif
    Logger::destroy();

    return finalize();
}
