#include "scy/application.h"
#include "scy/symple/client.h"
#include "scy/net/sslmanager.h"
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
// TODO: 
//  - Obtain authentication token
//  - Transaction test
//  - Presence test
//  - Ack test
//  - Benchmarks
class Tests
{
	Application app;
	
public:
	Tests()
	{	
		testAddress();
	}

	~Tests()
	{
		app.finalize();
	}
	
	void testAddress() 
	{
		smpl::Address a("user@group/567257247245275");
		assert(a.user == "user");
		assert(a.group == "group");
		assert(a.id == "567257247245275");
		assert(a.valid());

		smpl::Address a1("user@group");
		assert(a1.user == "user");
		assert(a1.group == "group");
		assert(a1.id == "");
		assert(a1.valid());

		smpl::Address a2("group");
		assert(a2.user == "");
		assert(a2.group == "group");
		assert(a2.id == "");
		assert(a2.valid());

		smpl::Address a3("");
		assert(a3.user == "");
		assert(a3.group == "");
		assert(a3.id == "");
		assert(!a3.valid());

		smpl::Address a4("/567257247245275");
		assert(a4.user == "");
		assert(a4.group == "");
		assert(a4.id == "567257247245275");
		assert(a4.valid());

		smpl::Address a5("group/567257247245275");
		assert(a5.user == "");
		assert(a5.group == "group");
		assert(a5.id == "567257247245275");
		assert(a5.valid());
	}

	void testClient() 
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
		smpl::SSLClient client(options); //reactor, runner, 
#else
		smpl::TCPClient client(options); //reactor, runner, 
#endif
		
		client.StateChange += delegate(this, &Tests::onClientStateChange);
		client.CreatePresence += delegate(this, &Tests::onCreatePresence);
		client.connect();

		//app.run();
		app.waitForShutdown();
		DebugL << "Event loop ended" << endl;
	}

	void onClientStateChange(void* sender, sockio::ClientState& state, const sockio::ClientState& oldState) 
	{
		smpl::Client* client = reinterpret_cast<smpl::Client*>(sender);	
		DebugL << "Client state changed: " << state.toString() << ": " << client->socket().address() << endl;
		
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
	
	void onCreatePresence(void*, smpl::Peer& peer)
	{
		DebugL << "Updating Client Data" << endl;
		
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
	
#if USE_SSL
		// Init SSL Context
		SSLContext::Ptr ptrContext = new SSLContext(SSLContext::CLIENT_USE, "", "", "",
			SSLContext::VERIFY_NONE, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");	
		SSLManager::instance().initializeClient(ptrContext);
#endif	

	{
		scy::smpl::Tests run;
	}		
		
#if USE_SSL
		SSLManager::instance().shutdown();
#endif	
	Logger::shutdown();
	return 0;
}