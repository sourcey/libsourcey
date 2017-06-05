///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup http
/// @{


#ifndef SCY_HTTP_Client_H
#define SCY_HTTP_Client_H


#include "scy/http/connection.h"
#include "scy/http/websocket.h"
#include "scy/net/dns.h"
#include "scy/net/socket.h"
#include "scy/net/sslsocket.h"
#include "scy/net/tcpsocket.h"
#include "scy/packetio.h"
#include "scy/timer.h"


namespace scy {
namespace http {


class HTTP_API Client;
class HTTP_API ClientConnection : public Connection
{
public:
    typedef std::shared_ptr<ClientConnection> Ptr;

    /// Create a standalone connection with the given host.
    ClientConnection(const URL& url, const net::TCPSocket::Ptr& socket = std::make_shared<net::TCPSocket>());

    virtual ~ClientConnection();

    /// Send the HTTP request.
    ///
    /// Calls connect() internally if the socket is not
    /// already connecting or connected. The actual request
    /// will be sent when the socket is connected.
    virtual void send();

    /// Send the given HTTP request.
    /// The given request will overwrite the internal HTTP
    /// request object.
    ///
    /// Calls connect() internally if the socket is not
    /// already connecting or connected. The actual request
    /// will be sent when the socket is connected.
    virtual void send(http::Request& req);

    /// Send raw data to the peer.
    /// Calls send() internally.
    ///
    /// Throws an exception if the socket is not already or connected.
    virtual ssize_t send(const char* data, size_t len, int flags = 0) override;
    // virtual ssize_t send(const std::string& buf, int flags = 0);
    // virtual void sendData(const char* buf, size_t len); //, int flags = 0
    // virtual void sendData(const std::string& buf); //, int flags = 0

    /// Forcefully closes the HTTP connection.
    //virtual void close();

    /// Set the output stream for writing response data to.
    /// The stream pointer is managed internally,
    /// and will be freed along with the connection.
    virtual void setReadStream(std::ostream* os);

    /// Return the cast read stream pointer or nullptr.
    template <class StreamT> StreamT& readStream()
    {
        if (!_readStream)
            throw std::runtime_error("No stream reader associated with HTTP client.");

        return *dynamic_cast<StreamT*>(_readStream.get());
    }

    /// Optional unmanaged client data pointer.
    void* opaque;

    //
    /// Connection interface

    virtual void onHeaders();
    virtual void onPayload(const MutableBuffer& buffer);
    virtual void onComplete();
    virtual void onClose();

    //
    /// Status signals

    NullSignal Connect;                         ///< Signals when the client socket is connected and data can flow
    Signal<void(Response&)> Headers;            ///< Signals when the response HTTP header has been received
    Signal<void(const MutableBuffer&)> Payload; ///< Signals when raw data is received
    Signal<void(const Response&)> Complete;     ///< Signals when the HTTP transaction is complete
    Signal<void(Connection&)> Close;            ///< Signals when the connection is closed

protected:
    /// Connects to the server endpoint.
    /// All sent data is buffered until the connection is made.
    virtual void connect();

    http::Message* incomingHeader();
    http::Message* outgoingHeader();

    void onSocketConnect(net::Socket& socket);

protected:
    URL _url;
    bool _connect;
    bool _active;
    bool _complete;
    std::vector<std::string> _outgoingBuffer;
    std::unique_ptr<std::ostream> _readStream;
};


typedef std::vector<ClientConnection::Ptr> ClientConnectionPtrVec;


//
// HTTP Connection Helpers
//


template <class ConnectionT>
inline ClientConnection::Ptr createConnectionT(const URL& url, uv::Loop* loop = uv::defaultLoop())
{
    ClientConnection::Ptr conn;

    if (url.scheme() == "http") {
        conn = std::make_shared<ConnectionT>(url, std::make_shared<net::TCPSocket>(loop));
        // conn->replaceAdapter(new ConnectionAdapter(conn, HTTP_RESPONSE));
        // conn = std::shared_ptr<ConnectionT>(
        //     new ConnectionT(url, std::make_shared<net::TCPSocket>(loop)),
        //     deleter::Deferred<ConnectionT>());
    } else if (url.scheme() == "https") {
        conn = std::make_shared<ConnectionT>(url, std::make_shared<net::SSLSocket>(loop));
        // conn->replaceAdapter(new ConnectionAdapter(conn, HTTP_RESPONSE));
        // conn = std::shared_ptr<ConnectionT>(
        //    new ConnectionT(url, std::make_shared<net::SSLSocket>(loop)),
        //     deleter::Deferred<ConnectionT>());
    } else if (url.scheme() == "ws") {
        conn = std::make_shared<ConnectionT>(url, std::make_shared<net::TCPSocket>(loop));
        conn->replaceAdapter(new ws::ConnectionAdapter(conn.get(), ws::ClientSide));
        // conn = std::shared_ptr<ConnectionT>(
        //     new ConnectionT(url, std::make_shared<net::TCPSocket>(loop)),
        //    deleter::Deferred<ConnectionT>());
    } else if (url.scheme() == "wss") {
        conn = std::make_shared<ConnectionT>(url, std::make_shared<net::SSLSocket>(loop));
        conn->replaceAdapter(new ws::ConnectionAdapter(conn.get(), ws::ClientSide));
        // conn = std::shared_ptr<ConnectionT>(
        //     new ConnectionT(url, std::make_shared<net::SSLSocket>(loop)),
        //     deleter::Deferred<ConnectionT>());
    } else
        throw std::runtime_error("Unknown connection type for URL: " + url.str());

    return conn;
}


//
// HTTP Client
//


class HTTP_API Client
{
public:
    Client();
    virtual ~Client();

    /// Return the default HTTP Client singleton.
    static Client& instance();

    /// Destroys the default HTTP Client singleton.
    static void destroy();

    /// Shutdown the Client and close all connections.
    void shutdown();

    template <class ConnectionT>
    ClientConnection::Ptr createConnectionT(const URL& url, uv::Loop* loop = uv::defaultLoop())
    {
        auto connection = http::createConnectionT<ConnectionT>(url, loop);
        if (connection) {
            addConnection(connection);
        }
        return connection;
    }

    ClientConnection::Ptr createConnection(const URL& url, uv::Loop* loop = uv::defaultLoop())
    {
        auto connection = http::createConnectionT<ClientConnection>(url, loop);
        if (connection) {
            addConnection(connection);
        }
        return connection;
    }

    virtual void addConnection(ClientConnection::Ptr conn);
    virtual void removeConnection(ClientConnection* conn);

    NullSignal Shutdown;

protected:
    void onConnectionClose(Connection& conn);

    friend class ClientConnection;

    ClientConnectionPtrVec _connections;
};


inline ClientConnection::Ptr createConnection(const URL& url, http::Client* client = nullptr,
                                              uv::Loop* loop = uv::defaultLoop())
{
    auto connection = createConnectionT<ClientConnection>(url, loop);
    if (client && connection)
        client->addConnection(connection);

    return connection;
}


#if 0
class HTTP_API SecureClientConnection: public ClientConnection
{
public:
    SecureClientConnection(Client* client, const URL& url) : //, const net::Address& address
        ClientConnection(client, url, net::SSLSocket()) //, address
    {
    }

    virtual ~SecureClientConnection()
    {
    }
};


class HTTP_API WebSocketClientConnection: public ClientConnection
{
public:
    WebSocketClientConnection(Client* client, const URL& url) : //, const net::Address& address
        ClientConnection(client, url) //, address
    {
        socket().replaceAdapter(new ws::ConnectionAdapter(*this, ws::ClientSide));    //&socket(), &request(), request(), request()
    }

    virtual ~WebSocketClientConnection()
    {
    }
};


class HTTP_API WebSocketSecureClientConnection: public ClientConnection
{
public:
    WebSocketSecureClientConnection(Client* client, const URL& url) : //, const net::Address& address
        ClientConnection(client, url, net::SSLSocket()) //, address
    {
        socket().replaceAdapter(new ws::ConnectionAdapter(*this, ws::ClientSide)); //(&socket(), &request()
    }

    virtual ~WebSocketSecureClientConnection()
    {
    }
};
#endif


} // namespace http
} // namespace scy


#endif


/// @\}
