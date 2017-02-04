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
#include "scy/http/connection.h"
#include "scy/http/parser.h"
#include "scy/http/request.h"
#include "scy/http/response.h"
#include "scy/logger.h"
#include "scy/net/socket.h"
#include "scy/timer.h"


namespace scy {
namespace http {


class SCY_EXTERN Server;
class SCY_EXTERN ServerResponder;
class SCY_EXTERN ServerConnection : public Connection
{
public:
    typedef std::shared_ptr<ServerConnection> Ptr;

    ServerConnection(Server& server, net::Socket::Ptr socket);
    virtual ~ServerConnection();

    /// Sends the HTTP response
    // virtual bool send();

    /// Closes the HTTP connection
    virtual void close();

protected:
    virtual void onHeaders();
    virtual void onPayload(const MutableBuffer& buffer);
    virtual void onMessage();
    virtual void onClose();

    Server& server();

    http::Message* incomingHeader();
    http::Message* outgoingHeader();

    /// Server callbacks
    // void onServerShutdown(void*);

protected:
    Server& _server;
    ServerResponder* _responder;
    bool _upgrade;
    bool _requestComplete;
};


typedef std::vector<ServerConnection::Ptr> ServerConnectionList;


// -------------------------------------------------------------------
//
class SCY_EXTERN ServerAdapter : public ConnectionAdapter
{
public:
    ServerAdapter(ServerConnection& connection)
        : ConnectionAdapter(connection, HTTP_REQUEST)
    {
    }
};


// -------------------------------------------------------------------
//

/// The abstract base class for HTTP ServerResponders
/// created by HTTP Server.
///
/// Derived classes must override the handleRequest() method.
///
/// A new HTTPServerResponder object will be created for
/// each new HTTP request that is received by the HTTP Server.
///
class SCY_EXTERN ServerResponder
{
public:
    ServerResponder(ServerConnection& connection)
        : _connection(connection)
    {
    }

    virtual ~ServerResponder() {}

    virtual void onHeaders(Request& /* request */) {}
    virtual void onPayload(const MutableBuffer& /* body */) {}
    virtual void onRequest(Request& /* request */, Response& /* response */) {}
    virtual void onClose(){};

    ServerConnection& connection() { return _connection; }

    Request& request() { return _connection.request(); }

    Response& response() { return _connection.response(); }

protected:
    ServerConnection& _connection;

private:
    /// NonCopyable and NonMovable
    ServerResponder(const ServerResponder&) = delete;
    ServerResponder& operator=(const ServerResponder&) = delete;
};


// -------------------------------------------------------------------
//

/// This implementation of a ServerResponderFactory
/// is used by HTTPServer to create ServerResponder objects.
class SCY_EXTERN ServerResponderFactory
{
public:
    ServerResponderFactory(){};
    virtual ~ServerResponderFactory(){};

    /// Factory method for instantiating the ServerResponder
    /// instance using the given ServerConnection.
    virtual ServerResponder* createResponder(ServerConnection& connection) = 0;
};


// -------------------------------------------------------------------
//

/// DISCLAIMER: This HTTP server is not standards compliant.
/// It was created to be a fast (nocopy where possible)
/// solution for streaming video to web browsers.
///
/// TODO:
/// - SSL Server
/// - Enable responders (controllers?) to be instantiated via registered routes.
class SCY_EXTERN Server
{
public:
    ServerResponderFactory* factory;
    net::TCPSocket::Ptr socket;
    net::Address address;
    Timer timer;
    ServerConnectionList connections;

    Server(short port, ServerResponderFactory* factory,
           net::TCPSocket::Ptr socket = net::makeSocket<net::TCPSocket>());
    virtual ~Server();

    void start();
    void shutdown();

    std::uint16_t port();

    NullSignal Shutdown;

protected:
    ServerConnection::Ptr createConnection(const net::Socket::Ptr& sock);
    ServerResponder* createResponder(ServerConnection& conn);

    virtual void addConnection(ServerConnection::Ptr conn);
    virtual void removeConnection(ServerConnection* conn);

    void onSocketAccept(const net::TCPSocket::Ptr& socket);
    void onSocketClose(net::Socket& socket);    // main socket close
    void onConnectionClose(Connection& socket); // connection socket close
    void onTimer();

    friend class ServerConnection;
};


// ---------------------------------------------------------------------
//
class SCY_EXTERN BadRequestHandler : public ServerResponder
{
public:
    BadRequestHandler(ServerConnection& connection)
        : ServerResponder(connection)
    {
    }

    void onRequest(Request&, Response& response)
    {
        response.setStatus(http::StatusCode::BadRequest);
        connection().sendHeader();
        connection().close();
    }
};


} // namespace http
} // namespace scy


#endif


/// @\}
