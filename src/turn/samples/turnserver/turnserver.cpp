#include "scy/application.h"
#include "scy/turn/server/server.h"
#include "scy/crypto/hash.h"


using namespace std;
using namespace scy;
using namespace scy::uv;
using namespace scy::net;
using namespace scy::turn;


const std::string SERVER_USERNAME("username");
const std::string SERVER_PASSWORD("password");
const std::string SERVER_REALM   ("sourcey.com");


struct TURNServer: public ServerObserver
{
	Server server;

	TURNServer(const ServerOptions& so) : server(*this, so) 
	{
	}

	virtual ~TURNServer() 
	{
	}

	void run() 
	{
		server.start();
	}
	
	virtual AuthenticationState authenticateRequest(Server* server, const Request& request)
	{
		debugL("TURNServer", this) << "Authenticating: " << request.transactionID() << endl;

		// The authentication information (e.g., username, password, realm, and
		// nonce) is used to both verify subsequent requests and to compute the
		// message integrity of responses.  The username, realm, and nonce
		// values are initially those used in the authenticated Allocate request
		// that creates the allocation, though the server can change the nonce
		// value during the lifetime of the allocation using a 438 (Stale Nonce)
		// reply.  Note that, rather than storing the password explicitly, for
		// security reasons, it may be desirable for the server to store the key
		// value, which is an MD5 hash over the username, realm, and password
		// (see [RFC5389]).

		// Note that the long-term credential mechanism cannot be used to
		// protect indications, since indications cannot be challenged. Usages
		// utilizing indications must either use a short-term credential or omit
		// authentication and message integrity for them.
		if (request.type() == stun::Message::SendIndication ||
			request.type() == stun::Message::Binding)
			return Authorized;

		// The initial packet from the client does not include the USERNAME, REALM, NONCE, 
		// or MESSAGE-INTEGRITY attributes. If these attributes are not provided we return
		// a 401 (Unauthorized) response.
		auto usernameAttr = request.get<stun::Username>();
		const stun::Realm* realmAttr = request.get<stun::Realm>();
		const stun::Nonce* nonceAttr = request.get<stun::Nonce>();
		const stun::MessageIntegrity* integrityAttr = request.get<stun::MessageIntegrity>();
		if (!usernameAttr || !realmAttr || !nonceAttr || !integrityAttr) {
			debugL("TURNServer", this) << "Authenticating: Unauthorized STUN Request" << endl;
			return turn::NotAuthorized;
		}
			
		// Determine authentication status and return either Authorized, 
		// Unauthorized or Authenticating.
		// Since no authentication is required we just return Authorized.
		//return turn::Authorized;


		std::string credentials(SERVER_USERNAME + ":" + SERVER_REALM + ":" + SERVER_PASSWORD);
		crypto::Hash engine("md5");
		engine.update(credentials);
		debugL("TURNServer", this) << "Generating HMAC: data=" << credentials << ", key=" << engine.digestStr() << endl;

		if (integrityAttr->verifyHmac(engine.digestStr()))
			return turn::Authorized;
		return turn::NotAuthorized;	
	}

	virtual void onServerAllocationCreated(Server* server, IAllocation* alloc) 
	{
		debugL("TURNServer", this) << "Allocation Created" << endl;
	}

	virtual void onServerAllocationRemoved(Server* server, IAllocation* alloc)
	{		
		debugL("TURNServer", this) << "Allocation Removed" << endl;
	}
};


int main(void)
{	
#ifdef _MSC_VER
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	Logger::instance().add(new ConsoleChannel("debug", LTrace));	
	Application app;
	{
		ServerOptions opts;		
		opts.software						= "Sourcey STUN/TURN Server [rfc5766]";
		opts.realm							= "sourcey.com";
		opts.allocationDefaultLifetime		= 1 * 60 * 1000;
		opts.allocationMaxLifetime			= 15 * 60 * 1000;
		opts.timerInterval					= 5 * 1000;
		opts.listenAddr						= Address("0.0.0.0", 3478);
		opts.enableUDP						= false;
	
		TURNServer srv(opts);
		srv.run();
		app.waitForShutdown([](void* opaque) {
			reinterpret_cast<TURNServer*>(opaque)->server.stop();
		}, &srv);
	}
	Logger::shutdown();
	return 0;
}


/*
// Detect Memory Leaks
#ifdef _DEBUG
#include "MemLeakDetect/MemLeakDetect.h"
#include "MemLeakDetect/MemLeakDetect.cpp"
CMemLeakDetect memLeakDetect;
#endif
*/


/* 

	
//static void onShutdown(void* opaque)
//{
//	reinterpret_cast<TURNServer*>(opaque)->server.stop();
//}


Old Poco App

#include "TURNServer.h"
using namespace std;
//using namespace Poco;
using namespace Poco::Util;
using namespace scy::net;
using namespace scy::turn;


namespace scy {
namespace turn {


TURNServer::turnServer() :
	//_runner(NULL), 
	//_reactor(NULL), 
	_server(NULL),
	_helpRequested(false)
{		
	Poco::Path path(Poco::Path::current());
	path.setFileName("TURNServer.log");
	Logger::instance().add(new FileChannel("TURNServer", path.toString(), 
#ifdef _DEBUG
		TraceLevel
#else
		InfoLevel
#endif
		));
	
	//_runner = new Runner();
	//_reactor = new Net::Reactor();

	// Set default options
	_options.software                         = SERVER_SOFTWARE;
	_options.realm                            = SERVER_REALM;
	_options.allocationDefaultLifetime        = ALLOCATION_DEFAULT_LIFETIME;
	_options.allocationMaxLifetime            = ALLOCATION_MAX_LIFETIME;
	_options.listenAddr                       = Address(SERVER_HOST, SERVER_PORT);
	
	_server = new turn::Server(*this, _options); //, *_reactor, *_runner
}


TURNServer::~TURNServer() 
{
	if (_server)
		delete _server;
	//if (_reactor)
	//	delete _reactor;
	//if (_runner)
	//	delete _runner;
	
	Logger::shutdown();
}
	
void TURNServer::defineOptions(OptionSet& options)
{
	Application::defineOptions(options);

	options.addOption(
		Option("help", "h", "display help information on command line arguments")
			.required(false)
			.repeatable(false)
			.callback(OptionCallback<TURNServer>(this, &TURNServer::handleCommand)));
		
	options.addOption(
		Option("port", "p", "the server port")
			.required(false)
			.repeatable(false)
			.argument("value")
			.callback(OptionCallback<TURNServer>(this, &TURNServer::handleCommand)));	

	options.addOption(
		Option("logfile", "l", "define the log file path")
			.required(false)
			.repeatable(false)
			.argument("value")
			.callback(OptionCallback<TURNServer>(this, &TURNServer::handleCommand)));
}

	
void TURNServer::handleCommand(const std::string& name, const std::string& value)
{
	if (name == "help") {
		_helpRequested = true;
		displayHelp();
		stopOptionsProcessing();
	}
	else if(name == "port") {
		_options.listenAddr = net::Address(SERVER_HOST, util::fromString<int>(value));
	}
	else if(name == "logfile") {
		FileChannel* log = dynamic_cast<FileChannel*>(Logger::instance().get("VideoAnalyzer"));
		log->setPath(value);
	}
}
	
void TURNServer::displayHelp()
{
	HelpFormatter helpFormatter(options());
	helpFormatter.setCommand(commandName());
	helpFormatter.setUsage("OPTIONS");
	helpFormatter.setHeader("A simple video analyzer which outputs spectral data in CSV format.");
	helpFormatter.format(cout);
}


int TURNServer::main(const std::vector<std::string>& args)
{	
	Log("trace", this) << "Initializing" << endl;
		
	try {		
		if (!_helpRequested)
		{
			_server->options() = _options;
			_server->start();

			// Wait for kill
			//waitForTerminationRequest();
			uv_run(uv_default_loop(), UV_RUN_DEFAULT);
		}
	}
	catch (Poco::Exception& exc) {
		Log("error", this) << "Error: " << std::string(exc.what()) << endl;
	}

	return Application::EXIT_OK;
}


AuthenticationState TURNServer::authenticateRequest(Server* server, const Request& request)
{
	// The authentication information (e.g., username, password, realm, and
	// nonce) is used to both verify subsequent requests and to compute the
	// message integrity of responses.  The username, realm, and nonce
	// values are initially those used in the authenticated Allocate request
	// that creates the allocation, though the server can change the nonce
	// value during the lifetime of the allocation using a 438 (Stale Nonce)
	// reply.  Note that, rather than storing the password explicitly, for
	// security reasons, it may be desirable for the server to store the key
	// value, which is an MD5 hash over the username, realm, and password
	// (see [RFC5389]).

	Mutex::ScopedLock lock(_mutex); 	

	Log("trace", this) << "Authenticating: " << request.transactionID() << endl;

	// Note that the long-term credential mechanism cannot be used to
	// protect indications, since indications cannot be challenged. Usages
	// utilizing indications must either use a short-term credential or omit
	// authentication and message integrity for them.
	if (request.type() == stun::Message::SendIndication ||
		request.type() == stun::Message::Binding)
		return Authorized;

	// The initial packet from the client does not include the USERNAME, REALM, NONCE, 
	// or MESSAGE-INTEGRITY attributes. If these attributes are not provided we return
	// a 401 (Unauthorized) response.
	auto usernameAttr = request.get<stun::Username>();
	const stun::Realm* realmAttr = request.get<stun::Realm>();
	const stun::Nonce* nonceAttr = request.get<stun::Nonce>();
	const stun::MessageIntegrity* integrityAttr = request.get<stun::MessageIntegrity>();
	if (!usernameAttr || !realmAttr || !nonceAttr || !integrityAttr) {
		Log("trace", this) << "Authenticating: Unauthorized STUN Request" << endl;
		return Unauthorized;
	}
	
	// Determine authentication status and return either Authorized, 
	// Unauthorized or Authenticating.
	// Since no authentication is required we just return Authorized.
	return Authorized;
}


void TURNServer::onServerAllocationCreated(Server* server, IAllocation* alloc)
{
	Log("trace", this) << "Server Allocation Created: " << alloc << endl;
}


void TURNServer::onServerAllocationRemoving(Server* server, IAllocation* alloc)
{
	Log("trace", this) << "Server Allocation Removing: " << alloc << endl;
}


} } // namespace scy::turn


POCO_SERVER_MAIN(TURNServer)
*/