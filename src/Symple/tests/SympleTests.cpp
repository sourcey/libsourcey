#include "Sourcey/Application.h"
#include "Sourcey/Symple/Client.h"
#include "Sourcey/Net/SSLManager.h"
#include "Sourcey/Util.h"


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
namespace smpl {

	
#define SERVER_HOST "anionu.com" //"localhost" 
#define USE_SSL     1
#if USE_SSL
#define SERVER_PORT 443
#else
#define SERVER_PORT 4000 
#endif

	
// ----------------------------------------------------------------------------
// SocketIO Client Test
//
class Tests
{
	Application app;
	
public:
	Tests()
	{	
		smpl::Client::Options options;
		options.host = SERVER_HOST;
		options.port = SERVER_PORT;
		options.user = "test";
		options.group = "global";
		options.name = "crash";
		options.type = "testapp";

		// NOTE: The server should use anonymous 
		// authentication for this test.
		//options.token = ""; used for authentication
	
#if USE_SSL
		// Init SSL Context
		SSLContext::Ptr ptrContext = new SSLContext(SSLContext::CLIENT_USE, "", "", "",
			SSLContext::VERIFY_NONE, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");	
		SSLManager::instance().initializeClient(ptrContext);

		smpl::SSLClient client(options); //reactor, runner, 
#else
		smpl::TCPClient client(options); //reactor, runner, 
#endif
		
		client.StateChange += delegate(this, &Tests::onClientStateChange);
		client.UpdatePresenceData += delegate(this, &Tests::onUpdatePresenceData);
		client.connect();
		
		//app.run();
		app.waitForShutdown();
		debugL() << "Event loop ended" << endl;
		
		// TODO: Obtain authentication token
		// TODO: Transaction test
		// TODO: Presence test
		// TODO: Ack test
		// TODO: Benchmarks
		
#if USE_SSL
		SSLManager::instance().shutdown();
#endif
		app.finalize();
	}


	void onClientStateChange(void* sender, sockio::ClientState& state, const sockio::ClientState& oldState) 
	{
		smpl::Client* client = reinterpret_cast<smpl::Client*>(sender);	
		debugL() << "Client state changed: " << state.toString() << ": " << client->socket().address() << endl;
		
		switch (state.id()) {
		case sockio::ClientState::Connecting:
			break;
		case sockio::ClientState::Connected: 
			break;
		case sockio::ClientState::Online: 
			break;
		case sockio::ClientState::Error:
			assert(0);
			break;
		}
	}
	
	void onUpdatePresenceData(void*, smpl::Peer& peer)
	{
		debugL() << "Updating Client Data" << endl;
		
		// Update the peer object to be broadcast with presence.
		// Any arbitrary data can be broadcast with presence.
		peer["agent"] = "Spot";
		peer["version"] = "1.0.1";
	}
};


} } // namespace scy::smpl


int main(int argc, char** argv) 
{	
	Logger::instance().add(new ConsoleChannel("debug", LTrace));

	// Init SSL Context
	//SSLManager::instance().initializeClient(
	//	new SSLContext(SSLContext::CLIENT_USE, "", "", "",
	//		SSLContext::VERIFY_NONE, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"));
	{
		scy::smpl::Tests run;
	}		
	//SSLManager::instance().shutdown();
	Logger::shutdown();
	return 0;
}