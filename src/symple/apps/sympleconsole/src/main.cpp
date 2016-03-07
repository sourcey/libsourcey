#include "scy/application.h"
#include "scy/filesystem.h"
#include "scy/symple/client.h"
#include "scy/net/sslmanager.h"
#include "scy/util.h"


using std::cout;
using std::cerr;
using std::endl;
using namespace scy;
using namespace scy::net;
using namespace scy::util;


//
// Symple Console Application
//
// Examples:
// symple -help
// symple -host localhost -port 4500 -token nLIgQ2R8DUiVsxm3kLG0xQtt -user 42 -name Somedude
// symple -host mydomain.com -port 80 -token nLIgQ2R8DUiVsxm3kLG0xQtt -user 42 -name Somedude
//
// TODO:
// - implement console thread for sending messages
//


#define USE_SSL 0


class SympleApplication: public scy::Application
{
public:
#if USE_SSL
    smpl::SSLClient client;
#else
    smpl::TCPClient client;
#endif

    bool showHelp;

    SympleApplication() :
        showHelp(false)
    {
    }

    virtual ~SympleApplication()
    {
    }

    void printHelp()
    {
        cout <<
            "\nSymple Coneole Client v0.1.0"
            "\n(c) Sourcey"
            "\nhttp://sourcey.com/symple"
            "\n"
            "\nGeneral options:"
            "\n  -help           Print help"
            "\n  -logfile        Log file path"
            "\n"
            "\nClient options:"
            "\n  -host           Symple server hostname or IP address"
            "\n  -port           Symple server port"
            "\n  -token          Session token to authenticate with"
            "\n  -user           User ID to register on the server with"
            "\n  -name           Display name to register on the server with"
            "\n  -type           User type to register on the server with (optional)"
            << endl;
    }

    void parseOptions(int argc, char* argv[])
    {
        OptionParser optparse(argc, argv, "-");
        for (auto& kv : optparse.args) {
            const std::string& key = kv.first;
            const std::string& value = kv.second;
            DebugL << "Setting option: " << key << ": " << value << std::endl;

            if (key == "help") {
                showHelp = true;
            }
            else if (key == "host") {
                client.options().host = value;
            }
            else if (key == "port") {
                client.options().port = util::strtoi<std::uint16_t>(value);
            }
            else if (key == "token") {
                client.options().token = value;
            }
            else if (key == "user") {
                client.options().user = value;
            }
            else if (key == "name") {
                client.options().name = value;
            }
            else if (key == "type") {
                client.options().type = value;
            }
            // else if (key == "logfile") {
            //     auto log = dynamic_cast<FileChannel*>(scy::Logger::instance().get("Symple"));
            //     log->setPath(value);
            // }
            else {
                cerr << "Unrecognized command: " << key << "=" << value << endl;
            }
        }
    }

    void work()
    {
        try {
            // Print help
            if (showHelp) {
                printHelp();
                return;
            }

            client.Announce += delegate(this, &SympleApplication::onClientAnnounce);
            client.StateChange += delegate(this, &SympleApplication::onClientStateChange);
            client.CreatePresence += delegate(this, &SympleApplication::onCreatePresence);
            client.connect();

            scy::Application::run();
        }
        catch (std::exception& exc) {
            cerr << "Symple runtime error: " << exc.what() << endl;
        }
    }

    void onClientAnnounce(const int& status)
    {
        assert(status == 200);
    }

    void onClientStateChange(sockio::ClientState& state, const sockio::ClientState& oldState)
    {
        // smpl::Client* client = reinterpret_cast<smpl::Client*>(sender);
        DebugL << "Client state changed: " << state << ": " << client.ws().socket->address() << endl;

        switch (state.id()) {
        case sockio::ClientState::Connecting:
            break;
        case sockio::ClientState::Connected:
            break;
        case sockio::ClientState::Online:
            {
                // Send a message when online
                smpl::Message m;
                m.setData("olay");
                client.send(m, true);
            }
            break;
        case sockio::ClientState::Error:
            assert(0);
            break;
        }
    }

    void onCreatePresence(smpl::Peer& peer)
    {
        DebugL << "Updating Client Data" << endl;

        // Update the peer object to be broadcast with presence.
        // Any arbitrary data can be broadcast with presence.
        peer["agent"] = "SympleConsole";
        peer["version"] = "0.1.0";
    }
};


int main(int argc, char** argv)
{
    {
#ifdef _MSC_VER
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

        // // Setup the file logger
        // std::string logPath(getCwd());
        // fs::addnode(logPath, "logs");
        // fs::addnode(logPath, util::format("Symple_%Ld.log", static_cast<long>(Timestamp().epochTime())));
        // Logger::instance().add(new FileChannel("Symple", logPath, LDebug));

        Logger::instance().add(new ConsoleChannel("debug", LTrace));

        // Init SSL client context
#if USE_SSL
        net::SSLManager::initNoVerifyClient();
#endif

        // Run the application
        {
            SympleApplication app;
            app.parseOptions(argc, argv);
            app.work();
        }

        // Cleanup all singletons
        // http::Client::destroy();
#if USE_SSL
        // SSLManager::instance().shutdown();
        net::SSLManager::destroy();
#endif
        GarbageCollector::destroy();
        Logger::destroy();
    }

    return 0;
}
