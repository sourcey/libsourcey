#include "scy/net/udpsocket.h"
#include "scy/time.h"


namespace scy {
namespace net {


#define PRINT_LATENCY_PACKET 1


class UDPEchoServer: public SocketAdapter
{
public:
	UDPSocket socket;

	UDPEchoServer()
	{
		SocketAdapter::socket = &socket;
		socket.setAdapter(this);
	}

	virtual ~UDPEchoServer()
	{
		socket.setAdapter(nullptr);
		shutdown();
	}
	
	void start(const std::string& host, UInt16 port)
	{	
		socket.bind(Address(host, port));
	}

	void shutdown() 
	{
		socket.close();
	}
	
	void onSocketRecv(const MutableBuffer& buf, const net::Address& peerAddr)
	{
		debugL("UDPEchoServer", this) << "On recv: " << peerAddr << ": " << buf.size() << std::endl;

#if PRINT_LATENCY_PACKET		
		std::string payload(bufferCast<const char*>(buf), buf.size());
		payload.erase(std::remove(payload.begin(), payload.end(), 'x'), payload.end());
		if (payload.length() < 12) {
			UInt64 sentAt = util::strtoi<UInt64>(payload);
			UInt64 latency = time::ticks() - sentAt;

			debugL("UDPEchoServer") << "Recv latency packet from " << peerAddr << ": " 
				<< "payload=" << payload.length() << ", " 
				<< "latency=" << latency 
				<< std::endl;
		}
#endif

		// Echo back to client
		socket.send(bufferCast<const char*>(buf), buf.size(), peerAddr);
	}

	void onSocketError(const Error& error) 
	{
		errorL("UDPResponder", this) << "On error: " << error.message << std::endl;
	}

	void onSocketClose() 
	{
		debugL("UDPResponder", this) << "On close" << std::endl;
	}
};


} } // namespace scy::net