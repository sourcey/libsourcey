#ifndef SOURCEY_TURNServer_H
#define SOURCEY_TURNServer_H


#include "Sourcey/Runner.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Util.h"
#include "Sourcey/IPolymorphic.h"
#include "Sourcey/Net/Reactor.h"
#include "Sourcey/TURN/server/Server.h"

#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Path.h"

#include <assert.h>


namespace Sourcey {
namespace TURN {


#define SERVER_SOFTWARE					"Sourcey STUN/TURN Server [rfc5766]"
#define SERVER_REALM					"sourcey.com"
#define SERVER_HOST						"127.0.0.1"
#define SERVER_PORT						3478
#define ALLOCATION_DEFAULT_LIFETIME		1 * 60 * 1000	// 1 min
#define ALLOCATION_MAX_LIFETIME			15 * 60 * 1000	// 15 min


class TURNServer: public TURN::IServerObserver, public IPolymorphic, public Poco::Util::ServerApplication
{
public:
	TURNServer();
	~TURNServer();
		
	void defineOptions(Poco::Util::OptionSet& options);
	void handleCommand(const std::string& name, const std::string& value);
	void displayHelp();

	int main(const std::vector<std::string>& args);
	
	void onServerAllocationCreated(Server* server, IAllocation* alloc);
	void onServerAllocationRemoving(Server* server, IAllocation* alloc);
	
	AuthenticationState authenticateRequest(Server* server, const Request& request);
	
	virtual const char* className() const { return "TURNServer"; }
	
private:	
	mutable Poco::FastMutex _mutex;

	Runner*			_runner;
	Net::Reactor*   _reactor;		
    TURN::Server*   _server;
	TURN::Server::Options _options;
	bool            _helpRequested;
};


} } // namespace Sourcey::TURN


#endif // SOURCEY_TURNServer_H
