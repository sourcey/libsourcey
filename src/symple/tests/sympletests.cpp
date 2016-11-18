#include "sympletests.h"


using namespace std;
using namespace scy;
using namespace scy::test;


#define SERVER_HOST "localhost"
#define USE_SSL 0
#if USE_SSL
#define SERVER_PORT 443
#else
#define SERVER_PORT 4500
#endif


int main(int argc, char** argv)
{
    // Logger::instance().add(new ConsoleChannel("debug", LTrace));
    test::initialize();

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
        loptions.user = "l";
        loptions.name = "Left";
        // loptions.token = "2NuMmyXw2YDuQfyPCKDO2Qtta";

        // NOTE: The server should allow anonymous
        // authentication for this test.
        // options.token = ""; used for authentication

        smpl::Client::Options roptions;
        roptions.host = SERVER_HOST;
        roptions.port = SERVER_PORT;
        roptions.user = "r";
        roptions.name = "Right";
        // roptions.token = "2NuMmyXw2YDuQfyPCKDO2Qtta";

        TestClient lclient(loptions);
        TestClient rclient(roptions);

        lclient.connect();
        rclient.connect();

        while (!lclient.completed() || !rclient.completed()) {
            // DebugL << "waiting for test completion" << std::endl;
            uv::runDefaultLoop(UV_RUN_ONCE);

            // // Connect the rclient when lclient is online
            // if (lclient.client.isOnline() &&
            //     rclient.client.stateEquals(sockio::ClientState::Closed))
            //     rclient.connect();
        }

        // TODO: check client failed status
        lclient.check();
        rclient.check();

        // lclient.close();
        // rclient.close();
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
