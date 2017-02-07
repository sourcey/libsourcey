#include "scy/logger.h"
#include "scy/net/address.h"

#include <memory>


namespace scy {
namespace net {


template <typename SocketT> class ClientSocketTest
{
public:
    SocketT socket;
    Address address;
    bool passed;

    ClientSocketTest(short port)
        : address("127.0.0.1", port)
        , passed(false)
    {
        TraceL << "Creating: " << port << std::endl;
    }

    ~ClientSocketTest()
    {
        // assert(socket.base().refCount() == 1);
        TraceL << "Destroying" << std::endl;
    }

    void run()
    {
        // Create the socket instance on the stack.
        // When the socket is closed it will unref the main loop
        // causing the test to complete successfully.
        socket.Recv += slot(this, &ClientSocketTest::onRecv);
        socket.Connect += slot(this, &ClientSocketTest::onConnect);
        socket.Error += slot(this, &ClientSocketTest::onError);
        socket.Close += slot(this, &ClientSocketTest::onClose);
        // assert(socket.base().refCount() == 1);
        socket.connect(address);
    }

    void stop()
    {
        socket.close();
        //socket.shutdown();
    }

    void onConnect(Socket& sock)
    {
        // assert(sender == &socket);
        // socket.send("client > server", 15);
        // socket.send("client > server", 15);
        TraceL << "Connected" << std::endl;

        sock.send("client > server", 15);
    }

    void onRecv(Socket&, const MutableBuffer& buffer, const Address& peerAddress)
    {
        std::string data(bufferCast<const char*>(buffer), buffer.size());
        DebugL << "On recv: " << data << std::endl;
        // Check for return packet echoing sent data
        if (data == "client > server") {
            TraceL << "Recv: Got Return Packet!" << std::endl;
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

    void onError(Socket&, const Error& error)
    {
        // assert(sender == &socket);
        InfoL << "On error: " << error.message << std::endl;
    }

    void onClose(Socket&)
    {
        // The last callback to fire is the Closed signal
        // which notifies us the underlying libuv socket
        // handle is closed. Das is gut!
        TraceL << "On closed" << std::endl;
    }
};


} // namespace net
} // namespace scy
