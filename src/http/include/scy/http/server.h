///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup http
/// @{


#ifndef SCY_HTTP_Server_H
#define SCY_HTTP_Server_H


#include "scy/base.h"
#include "scy/datetime.h"
#include "scy/http/connection.h"
#include "scy/http/parser.h"
#include "scy/http/request.h"
#include "scy/http/response.h"
#include "scy/logger.h"
#include "scy/net/socket.h"
#include "scy/timer.h"
#include <ctime>


namespace scy {
namespace http {


class SCY_EXTERN Server;

/// HTTP server connection.
class SCY_EXTERN ServerConnection : public Connection
{
public:
    typedef std::shared_ptr<ServerConnection> Ptr;

    ServerConnection(Server& server, net::TCPSocket::Ptr socket);
    virtual ~ServerConnection();

    Server& server();

    Signal<void(ServerConnection&, const MutableBuffer&)> Payload; ///< Signals when raw data is received
    Signal<void(ServerConnection&)> Close; ///< Signals when the connection is closed

protected:
    virtual void onHeaders();
    virtual void onPayload(const MutableBuffer& buffer);
    virtual void onComplete();
    virtual void onClose();

    http::Message* incomingHeader();
    http::Message* outgoingHeader();

protected:
    Server& _server;
    bool _upgrade;
};


// -------------------------------------------------------------------
//

/// This implementation of a ServerConnectionFactory
/// is used by HTTP Server to create ServerConnection objects.
class SCY_EXTERN ServerConnectionFactory
{
public:
    ServerConnectionFactory() {};
    virtual ~ServerConnectionFactory() {};

    /// Factory method for instantiating the ServerConnection
    /// instance using the given Socket.
    ServerConnection::Ptr createConnection(Server& server, const net::TCPSocket::Ptr& socket)
    {
        return std::make_shared<ServerConnection>(server, socket);
    };
};


// -------------------------------------------------------------------
//

/// HTTP server implementation.
///
/// This HTTP server is not strictly standards compliant.
/// It was created to be a fast (nocopy where possible)
/// solution for streaming media to web browsers.
class SCY_EXTERN Server : public net::SocketAdapter
{
public:
    Server(const net::Address& address, net::TCPSocket::Ptr socket = net::makeSocket<net::TCPSocket>());
    virtual ~Server();

    /// Start the HTTP server.
    void start();

    /// Shutdown the HTTP server.
    void shutdown();

    /// Return the server bind address.
    net::Address& address();

    /// Signals when a new connection has been created.
    /// A reference to the new connection object is provided.
    Signal<void(ServerConnection::Ptr)> Connection;

    /// Signals when the server is shutting down.
    NullSignal Shutdown;

protected:
    void onClientSocketAccept(const net::TCPSocket::Ptr& socket);
    void onConnectionReady(ServerConnection& conn);
    void onConnectionClose(ServerConnection& conn);
    void onSocketClose(net::Socket& socket);
    void onTimer();

protected:
    net::Address _address;
    net::TCPSocket::Ptr _socket;
    Timer _timer;
    ServerConnectionFactory* _factory;
    std::vector<ServerConnection::Ptr> _connections;

    friend class ServerConnection;
};


} // namespace http
} // namespace scy


#endif


/// @\}
