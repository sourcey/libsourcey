#include "TURNServer.h"


// Detect Memory Leaks
#ifdef _DEBUG
#include "MemLeakDetect/MemLeakDetect.h"
#include "MemLeakDetect/MemLeakDetect.cpp"
CMemLeakDetect memLeakDetect;
#endif


using namespace std;
using namespace Poco;
using namespace Poco::Util;
using namespace Sourcey::Net;
using namespace Sourcey::TURN;


namespace Sourcey {
namespace TURN {


TURNServer::TURNServer() :
	_runner(NULL), 
	_reactor(NULL), 
	_server(NULL),
	_helpRequested(false)
{		
	Path path(Poco::Path::current());
	path.setFileName("TURNServer.log");
	Logger::instance().add(new FileChannel("TURNServer", path.toString(), 
#ifdef _DEBUG
		TraceLevel
#else
		InfoLevel
#endif
		));
	
	_runner = new Runner();
	_reactor = new Net::Reactor();

	// Set default options
	_options.software                         = SERVER_SOFTWARE;
	_options.realm                            = SERVER_REALM;
	_options.allocationDefaultLifetime        = ALLOCATION_DEFAULT_LIFETIME;
	_options.allocationMaxLifetime            = ALLOCATION_MAX_LIFETIME;
	_options.listenAddr                       = Address(SERVER_HOST, SERVER_PORT);
	
	_server = new TURN::Server(*this, *_reactor, *_runner, _options);
}


TURNServer::~TURNServer() 
{
	if (_server)
		delete _server;
	if (_reactor)
		delete _reactor;
	if (_runner)
		delete _runner;
	
	Logger::uninitialize();
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

	
void TURNServer::handleCommand(const string& name, const string& value)
{
	if (name == "help") {
		_helpRequested = true;
		displayHelp();
		stopOptionsProcessing();
	}
	else if(name == "port") {
		_options.listenAddr = Net::Address(SERVER_HOST, Util::atoi(value));
	}
	else if(name == "logfile") {
		FileChannel* log = dynamic_cast<FileChannel*>(&Logger::instance().get("VideoAnalyzer"));
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


int TURNServer::main(const vector<string>& args)
{	
	Log("trace", this) << "Initializing" << endl;
		
	try {		
		if (!_helpRequested)
		{
			_server->options() = _options;
			_server->start();

			// Wait for kill
			waitForTerminationRequest();
		}
	}
	catch (Poco::Exception& exc) {
		Log("error", this) << "Error: " << exc.displayText() << endl;
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

	FastMutex::ScopedLock lock(_mutex); 	

	Log("trace", this) << "Authenticating: " << request.transactionID() << endl;

	// Note that the long-term credential mechanism cannot be used to
	// protect indications, since indications cannot be challenged. Usages
	// utilizing indications must either use a short-term credential or omit
	// authentication and message integrity for them.
	if (request.type() == STUN::Message::SendIndication ||
		request.type() == STUN::Message::Binding)
		return Authorized;

	// The initial packet from the client does not include the USERNAME, REALM, NONCE, 
	// or MESSAGE-INTEGRITY attributes. If these attributes are not provided we return
	// a 401 (Unauthorized) response.
	const STUN::Username* usernameAttr = request.get<STUN::Username>();
	const STUN::Realm* realmAttr = request.get<STUN::Realm>();
	const STUN::Nonce* nonceAttr = request.get<STUN::Nonce>();
	const STUN::MessageIntegrity* integrityAttr = request.get<STUN::MessageIntegrity>();
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


} } // namespace Sourcey::TURN


POCO_SERVER_MAIN(TURNServer)