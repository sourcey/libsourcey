#include "Sourcey/UV/UVPP.h"
#include "Sourcey/HTTP/Server.h"
#include "Sourcey/HTTP/Connection.h"
#include "Sourcey/HTTP/Client.h"
#include "Sourcey/Net/SSLSocket.h"
#include "Sourcey/UV/Timer.h"

#include "Sourcey/Base.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Net/Address.h"

/*
// TODO: depreciate Poco dependency
#include "Poco/Stopwatch.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/DNS.h"
#include "Poco/Net/Session.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/KeyConsoleHandler.h"
#include "Poco/Net/ConsoleCertificateHandler.h"
*/

#include "assert.h"
#include <iterator>


using namespace std;
using namespace scy;
using namespace scy::net;


/*
// Detect memory leaks on winders
#if defined(_DEBUG) && defined(_WIN32)
#include "MemLeakDetect/MemLeakDetect.h"
#include "MemLeakDetect/MemLeakDetect.cpp"
CMemLeakDetect memLeakDetect;
#endif
*/


namespace scy {
namespace http {


#define TEST_SSL 0


// -------------------------------------------------------------------
//
class SimpleResponser: public ServerResponser
{
public:
	SimpleResponser(ServerConnection& conn) : 
		ServerResponser(conn)
	{
	}

	void onRequestComplete(Request& request, Response& response) 
	{
		response.body << "Hello universe";
		connection().respond();
	}
};


// -------------------------------------------------------------------
//
class ChunkedResponser: public ServerResponser
{
public:
	ChunkedResponser(ServerConnection& conn) : 
		ServerResponser(conn)
	{
	}

	void onRequestHeaders(Request& request) 
	{
	}

	void onRequestBody(const Buffer& body)
	{
	}

	void onRequestComplete(Request& request, Response& response) 
	{
	}
};


// -------------------------------------------------------------------
//
class OurResponserFactory: public ServerResponserFactory
{
public:
	ServerResponser* createResponser(ServerConnection& conn)
	{
		// TODO: Chunked responser
		return new SimpleResponser(conn);
	}
};


// -------------------------------------------------------------------
//
class Tests
{
public:
	static uv::EventLoop Loop; 

	Tests()
	{	
		debugL("Tests") << "#################### Starting" << endl;
#ifdef _MSC_VER
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
		{			
			http::Server srv(81, new OurResponserFactory);
			srv.start();
			
			{
				http::Client client;	
				ClientConnection* conn = client.createConnection(net::Address("127.0.0.1", 81));		
				conn->ResponseHeaders += delegate(this, &Tests::onClientConnectionHeaders);
				conn->ResponseChunk += delegate(this, &Tests::onClientConnectionChunk);
				conn->Complete += delegate(this, &Tests::onClientConnectionComplete);
				conn->send(); // default get request
				
				debugL("Tests") << "#################### Running" << endl;
				Loop.run();
				debugL("Tests") << "#################### Ended" << endl;
			}

			/*	
			//runClientConnectionTest();	
			(sock);
			net::TCPSocket sock;	
			runClientConnectionTest();

			uv_signal_t sig;
			sig.data = this;
			uv_signal_init(Loop.loop, &sig);
			uv_signal_start(&sig, Tests::onKillSignal2, SIGINT);

			runUDPSocketTest();
			runTimerTest();
			//runDNSResolverTest();
			
			debugL("Tests") << "#################### Running" << endl;
			Loop.run();
			debugL("Tests") << "#################### Ended" << endl;
			*/
		}
		
		debugL("Tests") << "#################### Finalizing" << endl;
		Loop.cleanup();
		debugL("Tests") << "#################### Exiting" << endl;
	}


	void onClientConnectionHeaders(void* sender, Response& res) //, http::TransferComplete& progress
	{
		//debugL("ClientConnection Test") << "On Chunk: " << progress.progress() << endl;
	}

	void onClientConnectionChunk(void* sender, Buffer& buf) //, http::TransferComplete& progress
	{
		//debugL("ClientConnection Test") << "On Chunk: " << progress.progress() << endl;
	}

	void onClientConnectionComplete(void* sender, const Response& res) //, http::TransferComplete& progress
	{		
		ostringstream os;
		res.write(os);
		debugL("ClientConnection Test") << "############ Client Connection Complete: " << os.str() << endl;
	}
};


} } // namespace scy::http


int main(int argc, char** argv) 
{	
	Logger::instance().add(new ConsoleChannel("debug", TraceLevel));
	{
		http::Tests app;
	}
	Logger::uninitialize();
	return 0;
}


//Tests::Result Tests::Benchmark;
uv::EventLoop http::Tests::Loop;



	
		/*
	// ============================================================================
	// HTTP ClientConnection Test
	//
	void runClientConnectionTest() 
	{
		debugL("ClientConnection Test") << "Starting" << endl;	

		// Setup the transaction
		http::Request req("GET", "http://google.com");
		http::Response res;
		http::ClientConnection txn(&req);
		txn.Complete += delegate(this, &Tests::onClientConnectionComplete);
		txn.DownloadProgress += delegate(this, &Tests::onClientConnectionProgress);	
		txn.send();

		// Run the looop
		Loop.run();
		util::pause();

		debugL("ClientConnection Test") << "Ending" << endl;
	}		

	void onClientConnectionComplete(void* sender, http::Response& response)
	{
		debugL("ClientConnection Test") << "On Complete: " << &response << endl;
	}

	void onClientConnectionProgress(void* sender, http::TransferProgress& progress)
	{
		debugL("ClientConnection Test") << "On Progress: " << progress.progress() << endl;
	}
		*/
	
	
//using http::Request;
//using http::Response;
//using http::ClientConnection;

/*
struct Result {
	int numSuccess;
	string name;
	Poco::Stopwatch sw;

	void reset() {
		numSuccess = 0;
		sw.reset();
	}
};

static Result Benchmark;
*/