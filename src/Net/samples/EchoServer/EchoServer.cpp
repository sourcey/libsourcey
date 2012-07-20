#include "EchoServer.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Util.h"
#include "Sourcey/Net/TCPSocket.h"

#include "Poco/String.h"
#include "Poco/Format.h"
#include "Poco/Path.h"

#include <iostream>
#include <assert.h>


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


namespace Sourcey {
namespace Net {


#define SERVER_PORT 1337


EchoServer::EchoServer() :
	_server(NULL),
	_reactor(NULL),
	_port(SERVER_PORT),
	_helpRequested(false)
{		
	// TODO: Get logger path from config.	
	Path path(Poco::Path::current());
	path.setFileName("EchoServer.log");
	Logger::instance().add(new FileChannel("EchoServer", path.toString(), 
#ifdef _DEBUG
		TraceLevel
#else
		InfoLevel
#endif
		));
}


EchoServer::~EchoServer() 
{
	if (_server)
		delete _server;
	if (_reactor)
		delete _reactor;

	// Free singletons
	//Poco::Net::uninitializeNetwork();
	Logger::uninitialize();
}
	

void EchoServer::defineOptions(OptionSet& options)
{
	Application::defineOptions(options);

	options.addOption(
		Option("help", "h", "display help information on command line arguments")
			.required(false)
			.repeatable(false)
			.callback(OptionCallback<EchoServer>(this, &EchoServer::handleCommand)));
		
	options.addOption(
		Option("port", "p", "the server port")
			.required(false)
			.repeatable(false)
			.argument("value")
			.callback(OptionCallback<EchoServer>(this, &EchoServer::handleCommand)));	

	options.addOption(
		Option("logfile", "l", "define the log file path")
			.required(false)
			.repeatable(false)
			.argument("value")
			.callback(OptionCallback<EchoServer>(this, &EchoServer::handleCommand)));
}
	

void EchoServer::handleCommand(const string& name, const string& value)
{
	if (name == "help") {
		_helpRequested = true;
		displayHelp();
		stopOptionsProcessing();
	}
	else if(name == "port") {
		_port = Util::atoi(value);
	}
	else if(name == "logfile") {
		FileChannel* log = dynamic_cast<FileChannel*>(&Sourcey::Logger::instance().get("VideoAnalyzer"));
		log->setPath(value);
	}
}

	
void EchoServer::displayHelp()
{
	HelpFormatter helpFormatter(options());
	helpFormatter.setCommand(commandName());
	helpFormatter.setUsage("OPTIONS");
	helpFormatter.setHeader("A simple echo server.");
	helpFormatter.format(cout);
}


int EchoServer::main(const vector<string>& args)
{			
	try {		
		if (!_helpRequested)
		{
			Log("debug", this) << "Creating: " << _port << endl;
			_reactor = new Reactor();

			_server = new TCPSocketAcceptor(*_reactor);
			_server->bind(Address("127.0.0.1", _port));
			_server->SocketAccepted += delegate(this, &EchoServer::onSocketCreated);

			// Wait for kill			
			//Util::pause();
			waitForTerminationRequest();
		}
	}
	catch (Poco::Exception& exc) {
		Log("error", this) << "Error: " << exc.displayText() << endl;
	}

	return Application::EXIT_OK;
}

	
void EchoServer::onSocketCreated(void* sender, Poco::Net::StreamSocket& socket, Net::Reactor& reactor)
{
	Log("debug", this) << "TCP Socket Created: " << socket.peerAddress().toString() << endl;
	
	TCPPacketSocket* sock = new TCPPacketSocket(socket, reactor);
	sock->setDeleteOnClose(true);
	sock->registerPacketType<DataPacket>(1);
	sock->attach(packetDelegate(this, &EchoServer::onRequestReceived));
	//sock->Closed += delegate(this, &EchoServer::onSocketClosed, -1);	
}


void EchoServer::onRequestReceived(void* sender, DataPacket& packet) 
{
	TCPPacketSocket* socket = reinterpret_cast<TCPPacketSocket*>(sender);	

	Log("debug", this) << "Packet Received: " << packet.size() << ": " << socket->peerAddress() << endl;	

	// Echo the data back
	socket->send(packet);
}


/*
void EchoServer::onSocketClosed(void* sender) 
{
	TCPPacketSocket* socket = reinterpret_cast<TCPPacketSocket*>(sender);	
	Log("trace", this) << "[TCPServer:" << this << "] Freeing Socket: " << socket->peerAddress().toString() << std::endl;
	delete socket;
}
*/


} } // namespace Sourcey::Net


POCO_SERVER_MAIN(EchoServer)