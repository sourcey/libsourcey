#ifndef SOURCEY_NET_EchoServer_H
#define SOURCEY_NET_EchoServer_H


#include "Sourcey/Net/Reactor.h"
#include "Sourcey/Net/TCPServer.h"
#include "Sourcey/Net/TCPSocket.h"

#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"


namespace Sourcey {
namespace Net {


class EchoServer: public ILoggable, public Poco::Util::ServerApplication
{
public:
	EchoServer();
	~EchoServer();
		
	void defineOptions(Poco::Util::OptionSet& options);
	void handleCommand(const std::string& name, const std::string& value);
	void displayHelp();

	int main(const std::vector<std::string>& args);

	void onSocketCreated(void* sender, Net::TCPSocket& socket);	
	void onSocketClosed(void* sender);
	void onRequestReceived(void* sender, DataPacket& packet);
	
	virtual const char* className() const { return "EchoServer"; }
	
private:	
	mutable Poco::FastMutex _mutex;
		
	Net::Reactor* _reactor;
	Net::TCPPacketServer* _server;
	short _port;
	bool _helpRequested;
};


} } // namespace Sourcey::Net


#endif // SOURCEY_NET_EchoServer_H
