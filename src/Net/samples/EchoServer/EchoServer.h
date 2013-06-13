#ifndef SOURCEY_NET_EchoServer_H
#define SOURCEY_NET_EchoServer_H


#include "Sourcey/Net/Reactor.h"
#include "Sourcey/Net/SocketAcceptor.h"

#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"


namespace Scy {
namespace Net {


class EchoServer: public IPolymorphic, public Poco::Util::ServerApplication
{
public:
	EchoServer();
	~EchoServer();
		
	void defineOptions(Poco::Util::OptionSet& options);
	void handleCommand(const std::string& name, const std::string& value);
	void displayHelp();

	int main(const std::vector<std::string>& args);

	void onSocketCreated(void* sender, Poco::Net::StreamSocket& socket, Net::Reactor& reactor);	
	//void onSocketClosed(void* sender);
	void onRequestReceived(void* sender, DataPacket& packet);
	
	virtual const char* className() const { return "EchoServer"; }
	
private:	
	mutable Poco::FastMutex _mutex;
		
	Net::Reactor* _reactor;
	Net::TCPSocketAcceptor* _server;
	short _port;
	bool _helpRequested;
};


} } // namespace Scy::Net


#endif // SOURCEY_NET_EchoServer_H
