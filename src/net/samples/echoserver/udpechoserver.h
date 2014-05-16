#include "scy/net/udpsocket.h"
#include "scy/net/socketadapter.h"
#include "scy/time.h"


namespace scy {
namespace net {


class UDPEchoServer: public SocketAdapter
{
public:
	UDPSocket::Ptr socket;

	UDPEchoServer() : 
		socket(std::make_shared<UDPSocket>())
	{
		socket->addReceiver(this);
	}

	virtual ~UDPEchoServer()
	{
		socket->removeReceiver(this); 
		shutdown();
	}
	
	void start(const std::string& host, UInt16 port)
	{	
		socket->bind(Address(host, port));
	}

	void shutdown() 
	{
		socket->close();
	}
	
	void onSocketRecv(const MutableBuffer& buffer, const net::Address& peerAddress)
	{
		DebugL << "On recv: " << peerAddress << ": " << buffer.size() << std::endl;

#if 0		
		std::string payload(bufferCast<const char*>(buffer), buffer.size());
		payload.erase(std::remove(payload.begin(), payload.end(), 'x'), payload.end());
		if (payload.length() < 12) {
			UInt64 sentAt = util::strtoi<UInt64>(payload);
			UInt64 latency = time::ticks() - sentAt;

			DebugL << "Recv latency packet from " << peerAddress << ": " 
				<< "payload=" << payload.length() << ", " 
				<< "latency=" << latency 
				<< std::endl;
		}
#endif

		// Echo back to client
		socket->send(bufferCast<const char*>(buffer), buffer.size(), peerAddress);
	}

	void onSocketError(const scy::Error& error) 
	{
		ErrorL << "On error: " << error.message << std::endl;
	}

	void onSocketClose() 
	{
		DebugL << "On close" << std::endl;
	}
};


} } // namespace scy::net