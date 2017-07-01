#include "scy/net/socketadapter.h"
#include "scy/net/udpsocket.h"
#include "scy/time.h"


namespace scy {
namespace net {


class UDPEchoServer : public SocketAdapter
{
public:
    UDPSocket::Ptr server;

    UDPEchoServer()
        : server(std::make_shared<UDPSocket>())
    {
        server->addReceiver(this);
    }

    virtual ~UDPEchoServer()
    {
        server->removeReceiver(this);
        shutdown();
    }

    void start(const std::string& host, uint16_t port)
    {
        server->bind(Address(host, port));
    }

    void shutdown()
    {
        server->close();
    }

    void onSocketRecv(Socket&, const MutableBuffer& buffer, const net::Address& peerAddress) override
    {
        LDebug("On recv: ", peerAddress, ": ", buffer.size())

#if 0
        std::string payload(bufferCast<const char*>(buffer), buffer.size());
        payload.erase(std::remove(payload.begin(), payload.end(), 'x'), payload.end());
        if (payload.length() < 12) {
            uint64_t sentAt = util::strtoi<uint64_t>(payload);
            uint64_t latency = time::ticks() - sentAt;

            SDebug << "Recv latency packet from " << peerAddress << ": "
                << "payload=" << payload.length() << ", "
                << "latency=" << latency
                << std::endl;
        }
#endif
        // Echo back to client
        server->send(bufferCast<const char*>(buffer), buffer.size(), peerAddress);
    }

    void onSocketError(Socket&, const scy::Error& error) override
    {
        LError("On error: ", error.message)
    }

    void onSocketClose(Socket&) override
    {
        LDebug("On close")
    }
};


} // namespace net
} // namespace scy
