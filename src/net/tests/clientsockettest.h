#include "scy/logger.h"
#include "scy/net/address.h"

#include <memory>


namespace scy {
namespace net {


template <typename SocketT>
class ClientSocketTest : public SocketAdapter
{
public:
    SocketT socket;
    Address address;
    bool passed;

    ClientSocketTest(short port)
        : address("127.0.0.1", port)
        , passed(false)
    {
        TraceA("Creating: ", port)
    }

    virtual ~ClientSocketTest()
    {
        // assert(socket.base().refCount() == 1);
        TraceA("Destroying")
    }

    void run()
    {
        socket.addReceiver(this);
        socket.connect(address);
    }

    void stop()
    {
        socket.removeReceiver(this);
        socket.close();
        //socket.shutdown();
    }

    virtual void onSocketConnect(Socket& sock) override
    {
        DebugA("Connected")

        sock.send("client > server", 15);
    }

    virtual void onSocketRecv(Socket&, const MutableBuffer& buffer, const Address& peerAddress) override
    {
        std::string data(bufferCast<const char*>(buffer), buffer.size());
        DebugA("On recv:", data)
        // Check for return packet echoing sent data
        if (data == "client > server") {
            TraceA("Recv: Got Return Packet!")
            passed = true;
            stop();
        } else
            assert(false); // fail...
    }

    virtual void onSocketError(Socket&, const Error& error) override
    {
        // assert(sender == &socket);
        DebugA("On error:", error.message)
    }

    virtual void onSocketClose(Socket&) override
    {
        // The last callback to fire is the Closed signal
        // which notifies us the underlying libuv socket
        // handle is closed. Das is gut!
        DebugA("On closed")
    }
};


} // namespace net
} // namespace scy
