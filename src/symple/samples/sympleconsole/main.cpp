#include "scy/application.h"
#include "scy/filesystem.h"
#include "scy/symple/client.h"
#include "scy/net/sslmanager.h"
#include "scy/util.h"
#include "scy/ipc.h"

#include <iostream>


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


#define USE_SSL 0


class SympleApplication: public scy::Application
{
public:
#if USE_SSL
    smpl::SSLClient client;
#else
    smpl::TCPClient client;
#endif
    ipc::SyncQueue<> ipc;
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
            "\nSymple Console Client v0.1.0"
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
            else if (key == "logfile") {
                auto log = dynamic_cast<FileChannel*>(scy::Logger::instance().get("Symple"));
                log->setPath(value);
            }
            else {
                cerr << "Unrecognized command: " << key << "=" << value << endl;
            }
        }
    }

    void shutdown()
    {
        ipc.close();
        client.close();
        Application::stop();
        Application::finalize();
    }

    void start()
    {
        try {
            // Print help
            if (showHelp) {
                printHelp();
                return;
            }

            // Setup the client
            client += slot(this, &SympleApplication::onRecvPacket);
            // client += slot(this, &SympleApplication::onRecvMessage);
            client.Announce += slot(this, &SympleApplication::onClientAnnounce);
            client.StateChange += slot(this, &SympleApplication::onClientStateChange);
            client.CreatePresence += slot(this, &SympleApplication::onCreatePresence);
            client.connect();

            // Start the console thread
          	Thread console([](void* arg)
           	{
                auto app = reinterpret_cast<SympleApplication*>(arg);

             		char o = 0;
             		while (o != 'Q')
             		{
               			cout <<
                 				"COMMANDS:\n"
                 				"  M	Send a message.\n"
                 				"  J	Join a room.\n"
                 				"  L	Leave a room.\n"
                 				"  C	Print contacts list.\n"
                 				"  Q	Quit.\n";

               			o = toupper(std::getchar());
                    std::cin.ignore();

               			// Send a message
               			if (o == 'M') {
                 				cout << "Compose your message: " << endl;
                 				std::string data;
                 		    std::getline(std::cin, data);

                        auto message = new smpl::Message();
                        message->setData(data);

                     		cout << "Sending message: " << data << endl;
                        // app->client.send(message, true);

                        // Synchronize the message with the main thread
                        app->ipc.push(new ipc::Action(
                            std::bind(&SympleApplication::onSyncMessage, app, std::placeholders::_1), message));
                    }

           			// Join a room
           			else if (o == 'J') {
                        cout << "Join a room: " << endl;
                        auto data = new std::string();
                        std::getline(std::cin, *data);

                        app->ipc.push(new ipc::Action(
                            std::bind(&SympleApplication::onSyncCommand, app, std::placeholders::_1), data, "join"));
           			}

           			// Leave a room
           			else if (o == 'L') {
                    cout << "Leave a room: " << endl;
                    auto data = new std::string();
                    std::getline(std::cin, *data);

                    app->ipc.push(new ipc::Action(
                        std::bind(&SympleApplication::onSyncCommand, app, std::placeholders::_1), data, "leave"));
           			}

           			// List contacts
           			else if (o == 'C') {
             				cout << "Listing contacts:" << endl;
             				app->client.roster().print(cout);
                 		cout << endl;
           			}
               	}

                cout << "Quiting" << endl;
                app->shutdown();
            }, this);

            // Run the event loop
            waitForShutdown([](void* opaque) {
                reinterpret_cast<SympleApplication*>(opaque)->shutdown();
            }, this);
        }
        catch (std::exception& exc) {
            cerr << "Symple runtime error: " << exc.what() << endl;
        }
    }

    void onSyncMessage(const ipc::Action& action)
    {
        // Send the message on the main thread
        auto message = reinterpret_cast<smpl::Message*>(action.arg);

        // Send without transaction
        // client.send(*message);

        // Send with transaction
        auto transaction = client.createTransaction(*message);
        transaction->StateChange += slot(this, &SympleApplication::onAckState);
        transaction->send();

        delete message;
    }

    void onSyncCommand(const ipc::Action& action)
    {
        auto arg = reinterpret_cast<std::string*>(action.arg);

        if (action.data == "join") {
            client.joinRoom(*arg);
        }
        else if (action.data == "leave") {
            client.leaveRoom(*arg);
        }
    }

    void onAckState(void* sender, TransactionState& state, const TransactionState&)
    {
        DebugL << "####### On announce response: " << state << endl;

        // auto transaction = reinterpret_cast<sockio::Transaction*>(sender);
        switch (state.id()) {
        case TransactionState::Success:
            // Handle transaction success
            break;

        case TransactionState::Failed:
            // Handle transaction failure
            break;
        }
    }

    void onRecvPacket(IPacket& raw)
    {
        auto message = dynamic_cast<smpl::Message*>(&raw);
        if (message) {
            return onRecvMessage(*message);
        }

        auto packet = dynamic_cast<sockio::Packet*>(&raw);
        if (packet) {
            return onRecvPacket(*packet);
        }

        DebugL << "####### On raw packet: " << raw.className() << endl;

        // Handle incoming raw packets here
    }

    void onRecvMessage(smpl::Message& message)
    {
        DebugL << "####### On message: " << message.className() << endl;

        // Handle incoming Symple messages here
    }

    void onClientAnnounce(const int& status)
    {
        DebugL << "####### On announce: " << status << endl;
        assert(status == 200);
    }

    void onClientStateChange(void*, sockio::ClientState& state, const sockio::ClientState& oldState)
    {
        DebugL << "Client state changed: " << state << ": " << client.ws().socket->address() << endl;

        switch (state.id()) {
        case sockio::ClientState::Connecting:
            break;
        case sockio::ClientState::Connected:
            break;
        case sockio::ClientState::Online:
            cout << "Client online" << endl;

            // Join the public room
            client.joinRoom("public");
            break;
        case sockio::ClientState::Error:
            cout << "Client disconnected" << endl;
            break;
        }
    }

    void onCreatePresence(smpl::Peer& peer)
    {
        DebugL << "####### Updating presence data" << endl;

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

        // Setup the logger
        // std::string logPath(getCwd());
        // fs::addnode(logPath, util::format("Symple_%Ld.log", static_cast<long>(Timestamp().epochTime())));
        // cout << "Log path: " << logPath << endl;
        // Logger::instance().add(new FileChannel("Symple", logPath, LDebug));

        Logger::instance().add(new ConsoleChannel("debug", LTrace)); //LDebug

        // Init SSL client context
#if USE_SSL
        net::SSLManager::initNoVerifyClient();
#endif

        // Run the application
        {
            SympleApplication app;
            app.parseOptions(argc, argv);
            app.start();
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
