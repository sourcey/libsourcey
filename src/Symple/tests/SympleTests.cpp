#include "Sourcey/Symple/Client.h"
#include "Sourcey/Util.h"

#include "Poco/Net/SSLManager.h"
#include "Poco/Net/KeyConsoleHandler.h"
#include "Poco/Net/ConsoleCertificateHandler.h"


using namespace std;
using namespace Sourcey;
using namespace Sourcey::Util;
using namespace Poco;
using namespace Poco::Net;


/*
// Detect Memory Leaks
#ifdef _DEBUG
#include "MemLeakDetect/MemLeakDetect.h"
#include "MemLeakDetect/MemLeakDetect.cpp"
CMemLeakDetect memLeakDetect;
#endif
*/


namespace Sourcey {
namespace Symple {

	
#define SERVER_HOST "127.0.0.1"
#define SERVER_PORT 443
#define USE_SSL     1

	
// ----------------------------------------------------------------------------
//
// SocketIO Client Test
//
// ----------------------------------------------------------------------------	
class Tests
{
	Poco::Event ready;
	
	Runner runner;
	Net::Reactor reactor;
	
public:
	Tests()
	{	
		// NOTE: The server should use anonymous 
		// authentication for this test.
		Symple::Client::Options options;
		//options.token = ""; used for authentication
		options.user = "test";
		options.group = "global";
		options.name = "crash";
		options.type = "testapp";
		options.serverAddr = Net::Address(SERVER_HOST, SERVER_PORT);
	
#if USE_SSL
		Symple::SSLClient client(reactor, runner, options);
#else
		Symple::TCPClient client(reactor, runner, options);
#endif

		client.StateChange += delegate(this, &Tests::onConnectionStateChange);
		client.UpdatePresenceData += delegate(this, &Tests::onUpdatePresenceData);
		client.connect();

		ready.wait();	
		
		// TODO: Transaction Test
		// TODO: Presence Test
		// TODO: Ack Test

		Util::pause();
	}


	void onConnectionStateChange(void* sender, SocketIO::ClientState& state, const SocketIO::ClientState& oldState) 
	{
		Symple::Client* client = reinterpret_cast<Symple::Client*>(sender);	
		Log("debug") << "Connection State Changed: " << state.toString() << ": " << client->socket().address() << endl;
		
		switch (state.id()) {
		case SocketIO::ClientState::Connecting:
			break;
		case SocketIO::ClientState::Connected: 
			break;
		//case SocketIO::ClientState::Handshaking: 
		//	break;
		case SocketIO::ClientState::Online: 
			ready.set(); // pass
			break;
		case SocketIO::ClientState::Disconnected: 
			ready.set(); // fail
			break;
		}
	}


	void onUpdatePresenceData(void*, Symple::Peer& peer)
	{
		Log("debug") << "Updating Client Data" << endl;
		
		// Update the peer object to be broadcast with presence.
		// Any arbitrary data can be broadcast with presence.
		peer["agent"] = "Spot";
		peer["version"] = "1.0.1";
	}
};


} } // namespace Sourcey::Symple


int main(int argc, char** argv) 
{	
	Logger::instance().add(new ConsoleChannel("debug", TraceLevel));

	// Init SSL Context
	SharedPtr<InvalidCertificateHandler> ptrCert;
	Context::Ptr ptrContext = new Context(Context::CLIENT_USE, "", "", "",
		Context::VERIFY_NONE, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");	
	SSLManager::instance().initializeClient(0, ptrCert, ptrContext);
	{
		Sourcey::Symple::Tests run;
	}		
	Poco::Net::SSLManager::instance().shutdown();
	Logger::uninitialize();
	return 0;
}