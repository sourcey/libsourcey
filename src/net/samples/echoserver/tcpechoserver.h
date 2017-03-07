#include "scy/net/sslsocket.h"
#include "scy/net/tcpsocket.h"


namespace scy {
namespace net {


/// The TCP echo server accepts a template argument
/// of either a TCPSocket or a SSLSocket.
template <class SocketT>
class EchoServer : public SocketAdapter
{
public:
    typename SocketT::Ptr server;
    typename Socket::Vec sockets;

    EchoServer()
        : server(std::make_shared<SocketT>())
    {
    }

    ~EchoServer()
    {
        shutdown();
    }

    void start(const std::string& host, std::uint16_t port)
    {
        auto ssl = dynamic_cast<SSLSocket*>(server.get());
        if (ssl)
            ssl->useContext(SSLManager::instance().defaultServerContext());

        server->bind(Address(host, port));
        server->listen();
        server->AcceptConnection += slot(this, &EchoServer::onAcceptConnection);
    }

    void shutdown()
    {
        server->close();
        sockets.clear();
    }

    void onAcceptConnection(const TCPSocket::Ptr& socket)
    {
        socket->setReceiver(this);
        // socket->Recv += slot(this, &EchoServer::onClientSocketRecv);
        // socket->Error += slot(this, &EchoServer::onClientSocketError);
        // socket->Close += slot(this, &EchoServer::onClientSocketClose);
        sockets.push_back(socket);
        // DebugL << "On accept: " << socket << std::endl;
    }

    void onSocketRecv(Socket& socket, const MutableBuffer& buffer, const Address& peerAddress)
    {
        // DebugL << "On recv: " << &socket << ": " << buffer.str() << std::endl;
        //std::cout << "On recv: " << &socket << ": " << buffer.str() << std::endl;

        // Echo it back
        // socket.send(bufferCast<const char*>(buffer), buffer.size());

        // Send a HTTP packet
        std::ostringstream res;
        res << "HTTP/1.1 200 OK\r\n"
            << "Connection: close\r\n"
            << "Content-Length: 0" << "\r\n"
            << "\r\n";
        std::string response(res.str());
        socket.send(response.c_str(), response.size());
    }

    void onSocketError(Socket& socket, const Error& error)
    {
        InfoL << "On error: " << error.errorno << ": " << error.message << std::endl;
    }

    void onSocketClose(Socket& socket)
    {
        DebugL << "On close" << std::endl;
        releaseSocket(&socket);
    }

    void releaseSocket(net::Socket* sock)
    {
        //std::cout << "release socket: " << sockets.size() << std::endl;
        for (typename Socket::Vec::iterator it = sockets.begin();
             it != sockets.end(); ++it) {
            if (sock == it->get()) {
                DebugL << "Removing: " << sock << std::endl;

                // All we need to do is erase the socket in order to
                // deincrement the ref counter and destroy the socket->
                // assert(sock->base().refCount() == 1);
                sockets.erase(it);
                return;
            }
        }
        assert(0 && "unknown socket");
    }
};


#if 0
/// The TCP echo server accepts a template argument
/// of either a TCPSocket or a SSLSocket.
template <class SocketT>
class EchoServer
{
public:
    typename SocketT::Ptr server;
    typename Socket::Vec sockets;

    EchoServer()
        : server(std::make_shared<SocketT>())
    {
    }

    ~EchoServer() { shutdown(); }

    void start(const std::string& host, std::uint16_t port)
    {
        auto ssl = dynamic_cast<SSLSocket*>(server.get());
        if (ssl)
            ssl->useContext(SSLManager::instance().defaultServerContext());

        server->bind(Address(host, port));
        server->listen();
        server->AcceptConnection += slot(this, &EchoServer::onAcceptConnection);
    }

    void shutdown()
    {
        server->close();
        sockets.clear();
    }

    void onAcceptConnection(const TCPSocket::Ptr& socket)
    {
        sockets.push_back(socket);
        DebugL << "On accept: " << socket << std::endl;
        socket->Recv += slot(this, &EchoServer::onClientSocketRecv);
        socket->Error += slot(this, &EchoServer::onClientSocketError);
        socket->Close += slot(this, &EchoServer::onClientSocketClose);
    }

    void onClientSocketRecv(Socket& socket, const MutableBuffer& buffer, const Address& peerAddress)
    {
        DebugL << "On recv: " << &socket << ": " << buffer.str() << std::endl; // Echo it back
        socket.send(bufferCast<const char*>(buffer), buffer.size());
    }

    void onClientSocketError(Socket& socket, const Error& error)
    {
        InfoL << "On error: " << error.errorno << ": " << error.message << std::endl;
    }

    void onClientSocketClose(Socket& socket)
    {
        DebugL << "On close" << std::endl;
        releaseSocket(&socket);
    }

    void releaseSocket(net::Socket* sock)
    {
        for (typename Socket::Vec::iterator it = sockets.begin();
             it != sockets.end(); ++it) {
            if (sock == it->get()) {
                DebugL << "Removing: " << sock << std::endl;

                // All we need to do is erase the socket in order to
                // deincrement the ref counter and destroy the socket->
                // assert(sock->base().refCount() == 1);
                sockets.erase(it);
                return;
            }
        }
        assert(0 && "unknown socket");
    }
};
#endif


// Some generic server types
typedef EchoServer<TCPSocket> TCPEchoServer;
typedef EchoServer<SSLSocket> SSLEchoServer;


} // namespace net
} // namespace scy
