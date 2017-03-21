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

    ~ClientSocketTest()
    {
        // assert(socket.base().refCount() == 1);
        TraceA("Destroying")
    }

    void run()
    {
        // Create the socket instance on the stack.
        // When the socket is closed it will unref the main loop
        // causing the test to complete successfully.
        // socket.Recv += slot(this, &ClientSocketTest::onRecv);
        // socket.Connect += slot(this, &ClientSocketTest::onConnect);
        // socket.Error += slot(this, &ClientSocketTest::onError);
        // socket.Close += slot(this, &ClientSocketTest::onClose);
        // assert(socket.base().refCount() == 1);
        socket.addReceiver(this);
        socket.connect(address);
    }

    void stop()
    {
        socket.removeReceiver(this);
        socket.close();
        //socket.shutdown();
    }

    void onSocketConnect(Socket& sock)
    {
        DebugL << "Connected" << std::endl;

        sock.send("client > server", 15);
    }

    void onSocketRecv(Socket&, const MutableBuffer& buffer, const Address& peerAddress)
    {
        std::string data(bufferCast<const char*>(buffer), buffer.size());
        DebugA("On recv: ", data)
        // Check for return packet echoing sent data
        if (data == "client > server") {
            TraceA("Recv: Got Return Packet!")
            passed = true;

            // Send the shutdown command to close the connection gracefully.
            // The peer disconnection will trigger an eof error callback
            // which notifies us that the connection is dead.
            // socket.shutdown();
            // socket.close();
            stop();
        } else
            assert(false); // fail...
    }

    void onSocketError(Socket&, const Error& error)
    {
        // assert(sender == &socket);
        InfoL << "On error: " << error.message << std::endl;
    }

    void onSocketClose(Socket&)
    {
        // The last callback to fire is the Closed signal
        // which notifies us the underlying libuv socket
        // handle is closed. Das is gut!
        TraceA("On closed")
    }
};


} // namespace net
} // namespace scy
