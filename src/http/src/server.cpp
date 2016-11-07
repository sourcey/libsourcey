///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup http
/// @{


#include "scy/http/server.h"
#include "scy/http/websocket.h"
#include "scy/logger.h"
#include "scy/util.h"


using std::endl;


namespace scy {
namespace http {


Server::Server(short port, ServerResponderFactory* factory) :
    socket(net::makeSocket<net::TCPSocket>()),
    factory(factory),
    address("0.0.0.0", port)
{
    TraceS(this) << "Create" << endl;
}


Server::~Server()
{
    TraceS(this) << "Destroy" << endl;
    shutdown();
    if (factory)
        delete factory;
}


void Server::start()
{
    // TODO: Register self as an observer
    //socket.reset(new net::TCPSocket);
    socket->AcceptConnection += delegate(this, &Server::onAccept);
    socket->Close += delegate(this, &Server::onClose);
    socket->bind(address);
    socket->listen();

    TraceS(this) << "Server listening on " << port() << endl;

    //timer.Timeout += delegate(this, &Server::onTimer);
    //timer.start(5000, 5000);
}


void Server::shutdown()
{
    TraceS(this) << "Shutdown" << endl;

    if (socket) {
        socket->AcceptConnection -= delegate(this, &Server::onAccept);
        socket->Close -= delegate(this, &Server::onClose);
        socket->close();
    }

    Shutdown.emit(this);

    for (auto conn : this->connections) {
        conn->close(); // close and remove via callback
    }
    assert(this->connections.empty());
}


std::uint16_t Server::port()
{
    return address.port();
}


ServerConnection::Ptr Server::createConnection(const net::Socket::Ptr& sock)
{
    auto conn = std::shared_ptr<ServerConnection>(
        new ServerConnection(*this, sock),
            deleter::Deferred<ServerConnection>());
    addConnection(conn);
    return conn; //return new ServerConnection(*this, sock);
}


ServerResponder* Server::createResponder(ServerConnection& conn)
{
    // The initial HTTP request headers have already
    // been parsed by now, but the request body may
    // be incomplete (especially if chunked).
    return factory->createResponder(conn);
}


void Server::addConnection(ServerConnection::Ptr conn)
{
    TraceS(this) << "Adding connection: " << conn << endl;
    conn->Close += sdelegate(this, &Server::onConnectionClose, -1); // lowest priority
    connections.push_back(conn);
}


void Server::removeConnection(ServerConnection* conn)
{
    TraceS(this) << "Removing connection: " << conn << endl;
    for (auto it = connections.begin(); it != connections.end(); ++it) {
        if (conn == it->get()) {
            connections.erase(it);
            return;
        }
    }
    assert(0 && "unknown connection");
}


void Server::onAccept(const net::TCPSocket::Ptr& sock)
{
    TraceS(this) << "On server accept" << endl;
    ServerConnection::Ptr conn = createConnection(sock);
    if (!conn) {
        WarnL << "Cannot create connection" << endl;
        assert(0);
    }
}


void Server::onClose()
{
    TraceS(this) << "On server socket close" << endl;
}


void Server::onConnectionClose(void* sender)
{
    TraceS(this) << "On connection close" << endl;
    removeConnection(reinterpret_cast<ServerConnection*>(sender));
}


//
// Server Connection
//


ServerConnection::ServerConnection(Server& server, net::Socket::Ptr socket) :
    Connection(socket),
    _server(server),
    _responder(nullptr),
    _upgrade(false),
    _requestComplete(false)
{
    TraceS(this) << "Create" << endl;

    replaceAdapter(new ServerAdapter(*this));
}


ServerConnection::~ServerConnection()
{
    TraceS(this) << "Destroy" << endl;

    if (_responder) {
        TraceS(this) << "Destroy: Responder: " << _responder << endl;
        delete _responder;
    }
}


void ServerConnection::close()
{
    if (!closed()) {
        Connection::close(); // close and destroy
    }
}


Server& ServerConnection::server()
{
    return _server;
}


//
// Connection Callbacks

void ServerConnection::onHeaders()
{
    TraceS(this) << "On headers" << endl;

    // Upgrade the connection if required
    if (util::icompare(_request.get("Connection", ""), "upgrade") == 0 &&
        util::icompare(_request.get("Upgrade", ""), "websocket") == 0) {
        TraceS(this) << "Upgrading to WebSocket: " << _request << endl;
        _upgrade = true;


        auto wsAdapter = new ws::ConnectionAdapter(*this, ws::ServerSide);

        // Note: To upgrade the connection we need to replace the
        // underlying SocketAdapter instance. Since we are currently
        // inside the default ConnectionAdapter's HTTP parser callback
        // scope we just swap the SocketAdapter instance pointers and do
        // a deferred delete on the old adapter. No more callbacks will be
        // received from the old adapter after replaceAdapter is called.
        replaceAdapter(wsAdapter);

        std::ostringstream oss;
        _request.write(oss);
        _request.clear();
        std::string buffer(oss.str());

        // Send the handshake request to the WS adapter for handling.
        // If the request fails the underlying socket will be closed
        // resulting in the destruction of the current connection.
        wsAdapter->onSocketRecv(mutableBuffer(buffer), socket()->peerAddress());
    }

    // Instantiate the responder when request headers have been parsed
    _responder = _server.createResponder(*this);

    // If no responder was created we close the connection.
    // TODO: Should we return a 404 instead?
    if (!_responder) {
        WarnL << "Ignoring unhandled request: " << _request << endl;
        close();
        return;
    }

    // Upgraded connections don't receive the onHeaders callback
    if (!_upgrade)
        _responder->onHeaders(_request);

    // NOTE: Outgoing.start() must be manually called by the ServerResponder,
    // since adapters cannot be added once started.
    // Start the Outgoing packet stream
    // Outgoing.start();
}


void ServerConnection::onPayload(const MutableBuffer& buffer)
{
    TraceS(this) << "On payload: " << buffer.size() << endl;

    // The connection may have been closed inside a previous callback.
    if (closed()) {
        TraceS(this) << "On payload: Closed" << endl;
        return;
    }

    //assert(_upgrade); // no payload for upgrade requests
    assert(_responder);
    _responder->onPayload(buffer);
}


void ServerConnection::onMessage()
{
    TraceS(this) << "On complete" << endl;

    // The connection may have been closed inside a previous callback.
    if (closed()) {
        TraceS(this) << "On complete: Closed" << endl;
        return;
    }

    // The HTTP request is complete.
    // The request handler can give a response.
    assert(_responder);
    assert(!_requestComplete);
    _requestComplete = true;
    _responder->onRequest(_request, _response);
}


void ServerConnection::onClose()
{
    TraceS(this) << "On close" << endl;

    if (_responder)
        _responder->onClose();

    Connection::onClose();
}


/*
void ServerConnection::onServerShutdown(void*)
{
    TraceS(this) << "On server shutdown" << endl;

    close();
}
*/


http::Message* ServerConnection::incomingHeader()
{
    return reinterpret_cast<http::Message*>(&_request);
}


http::Message* ServerConnection::outgoingHeader()
{
    return reinterpret_cast<http::Message*>(&_response);
}


} } // namespace scy::http

/// @\}
