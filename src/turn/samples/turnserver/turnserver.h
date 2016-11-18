#ifndef SCY_TURNServer_H
#define SCY_TURNServer_H


#include "scy/turn/server/server.h"

#include <assert.h>


/*
#include "scy/application.h"
#include "scy/logger.h"
#include "scy/polymorphic.h"
#include "scy/util.h"
////#include "scy/net/reactor.h"

#include "Poco/Path.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/ServerApplication.h"


#define SERVER_SOFTWARE                    "Sourcey STUN/TURN Server [rfc5766]"
#define SERVER_REALM                    "sourcey.com"
#define SERVER_HOST                        "127.0.0.1"
#define SERVER_PORT                        3478
#define ALLOCATION_DEFAULT_LIFETIME        1 * 60 * 1000    // 1 min
#define ALLOCATION_MAX_LIFETIME            15 * 60 * 1000    // 15 min


class TURNServer: public turn::ServerObserver, public Polymorphic, public
Poco::util::ServerApplication
{

namespace scy {
namespace turn {


#define SERVER_SOFTWARE                    "Sourcey STUN/TURN Server [rfc5766]"
#define SERVER_REALM                    "sourcey.com"
#define SERVER_HOST                        "127.0.0.1"
#define SERVER_PORT                        3478
#define ALLOCATION_DEFAULT_LIFETIME        1 * 60 * 1000    // 1 min
#define ALLOCATION_MAX_LIFETIME            15 * 60 * 1000    // 15 min


class TURNServer: public turn::ServerObserver, public Polymorphic, public
Poco::util::ServerApplication
{
public:
    TURNServer();
    ~TURNServer();

    void defineOptions(Poco::util::OptionSet& options);
    void handleCommand(const std::string& name, const std::string& value);
    void displayHelp();

    int main(const std::vector<std::string>& args);

    void onServerAllocationCreated(Server* server, IAllocation* alloc);
    void onServerAllocationRemoving(Server* server, IAllocation* alloc);

    AuthenticationState authenticateRequest(Server* server, Request& request);

    virtual const char* className() const { return "TURNServer"; }

private:
    mutable Poco::Mutex _mutex;

    //Runner*            _runner;
    //Net::Reactor*   _reactor;
    turn::Server*   _server;
    turn::ServerOptions _options;
    bool            _helpRequested;
};



} // namespace turn
} // namespace scy
*/


#endif // SCY_TURNServer_H


/// @\}
