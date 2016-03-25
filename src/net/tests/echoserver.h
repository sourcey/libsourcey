#include "scy/net/tcpsocket.h"
#include "scy/net/sslsocket.h"


namespace scy {
namespace net {


template <typename SocketT>
class EchoServer
{
public:
    SocketT socket;
    typename SocketT::List sockets;
    Address address;

    EchoServer(short port, bool ghost = false) :
        address("127.0.0.1", port)
    {
        // Unref the socket handle if running in ghost mode.
        // This way the server won't prevent the main uv_loop
        // from exiting when other processed complete.
        if (ghost)
            reinterpret_cast<typename SocketT::Base&>(socket.base()).handle().unref();
    }

    ~EchoServer()
    {
        traceL("EchoServer", this) << "Destroying" << std::endl;
        // assert(socket.base().refCount() == 1);

        // This will close the server socket, and should cause the
        // main loop to terminate if there are no active processes.
        sockets.clear();
    }

    void run()
    {
        // traceL("EchoServer", this) << "Run " << socket.base().refCount() << std::endl;
        // assert(socket.base().refCount() == 1);
        socket.bind(address);
        socket.listen();
        socket.AcceptConnection += sdelegate(this, &EchoServer::onAccept);
        traceL("EchoServer", this) << "Server listening on " << port() << std::endl;
    }

    void stop()
    {
        socket.close();
    }

    void onAccept(void* sender, const TCPSocket& sock)
    {
        sockets.push_back(sock);
        SocketT& socket = sockets.back();
        traceL("EchoServer", this) << "On Accept: " << &socket.base() << std::endl;
        socket.Recv += sdelegate(this, &EchoServer::onSocketRecv);
        socket.Error += sdelegate(this, &EchoServer::onSocketError);
        socket.Close += sdelegate(this, &EchoServer::onSocketClose);
    }

    void onSocketRecv(void* sender, SocketPacket& packet)
    {
        SocketT* socket = reinterpret_cast<SocketT*>(sender);
        traceL("EchoServer", this) << "On Recv: "
            << socket //<< ": " << packet.length()
            << std::endl;

        // Echo it back
        socket->send(packet);
    }

    void onSocketError(void* sender, int syserr, const std::string& message)
    {
        traceL("EchoServer", this) << "On Error: " << syserr << ": " << message << std::endl;
    }

    void onSocketClose(void* sender)
    {
        traceL("EchoServer", this) << "On Close" << std::endl;
        releaseSocket(reinterpret_cast<SocketT*>(sender));
    }

    void releaseSocket(SocketT* socket)
    {
        for (typename SocketT::List::iterator it = sockets.begin(); it != sockets.end(); ++it) {
            if (socket == &(*it)) {
                traceL("EchoServer", this) << "Removing: " << socket << std::endl;

                // All we need to do is erase the socket in order to
                // deincrement the ref counter and destroy the socket.
                // assert(socket->base().refCount() == 1);
                sockets.erase(it);
                return;
            }
        }
    }

    std::uint16_t port()
    {
        return socket.address().port();
    }
};


// Some generic server types
typedef EchoServer<TCPSocket> TCPEchoServer;
typedef EchoServer<SSLSocket> SSLEchoServer;


} } // namespace scy::net
