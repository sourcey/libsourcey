///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup http
/// @{


#include "scy/http/client.h"
#include "scy/logger.h"
#include "scy/util.h"


using std::endl;


namespace scy {
namespace http {


//
// Client Connection
//


ClientConnection::ClientConnection(const URL& url, const net::TCPSocket::Ptr& socket)
    : Connection(socket)
    , _url(url)
    , _connect(false)
    , _active(false)
    , _complete(false)
{
    // LTrace("Create: ", url)

    auto uri = url.pathEtc();
    if (!uri.empty())
        _request.setURI(uri);
    _request.setHost(url.host(), url.port());

    // Set default error status
    _response.setStatus(http::StatusCode::BadGateway);

    replaceAdapter(new ConnectionAdapter(this, HTTP_RESPONSE));
}


ClientConnection::~ClientConnection()
{
    // LTrace("Destroy")
}


void ClientConnection::send()
{
    assert(!_connect);
    connect();
}


void ClientConnection::send(http::Request& req)
{
    assert(!_connect);
    _request = req;
    connect();
}


ssize_t ClientConnection::send(const char* data, size_t len, int flags)
{
    connect();

    if (_active)
        // Raw data will be pushed onto the Outgoing packet stream
        return Connection::send(data, len);
    else
        _outgoingBuffer.push_back(std::string(data, len));
    return (int)len;
}


void ClientConnection::connect()
{
    if (!_connect) {
        _connect = true;
        // LTrace("Connecting")
        _socket->connect(_url.host(), _url.port());
    }
}


void ClientConnection::setReadStream(std::ostream* os)
{
    assert(!_connect);

    //Incoming.attach(new StreamWriter(os), -1, true);
    _readStream.reset(os);
}


http::Message* ClientConnection::incomingHeader()
{
    return static_cast<http::Message*>(&_response);
}


http::Message* ClientConnection::outgoingHeader()
{
    return static_cast<http::Message*>(&_request);
}


//
// Socket Callbacks

void ClientConnection::onSocketConnect(net::Socket& socket)
{
    // LTrace("On connect")

    // Set the connection to active
    _active = true;

    // Emit the connect signal so raw connections like
    // websockets can kick off the data flow
    Connect.emit();

    // Flush queued packets
    if (!_outgoingBuffer.empty()) {
        // LTrace("Sending buffered: ", _outgoingBuffer.size())
        for (const auto& packet : _outgoingBuffer) {
            send(packet.c_str(), packet.length());
        }
        _outgoingBuffer.clear();
    }
    else {

        // Send the header
        sendHeader();
    }

    // Send the outgoing HTTP header if it hasn't already been sent.
    // Note the first call to socket().send() will flush headers.
    // Note if there are stream adapters we wait for the stream to push
    // through any custom headers. See ChunkedAdapter::emitHeader
    //if (Outgoing.numAdapters() == 0) {
    //    // LTrace("On connect: Send header")
    //    sendHeader();
    //}
}


//
// Connection Callbacks

void ClientConnection::onHeaders()
{
    // LTrace("On headers")
    //IncomingProgress.total = _response.getContentLength();

    Headers.emit(_response);
}


void ClientConnection::onPayload(const MutableBuffer& buffer)
{
    // LTrace("On payload: ", buffer.size())

    //// Update download progress
    //IncomingProgress.update(buffer.size());

    //// Write to the incoming packet stream if adapters are attached
    //if (Incoming.numAdapters() > 0 || Incoming.emitter.nslots() > 0) {
    //    // if (!Incoming.active());
    //    //     throw std::runtime_error("startInputStream() must be called");
    //    Incoming.write(bufferCast<const char*>(buffer), buffer.size());
    //}

    // Write to the STL read stream if available
    if (_readStream) {
        // LTrace("Writing to stream: ", buffer.size())
        _readStream->write(bufferCast<const char*>(buffer), buffer.size());
        _readStream->flush();
    }

    Payload.emit(buffer);
}


void ClientConnection::onComplete()
{
    // LTrace("On complete")

    assert(!_complete);
    _complete = true; // in case close() is called inside callback

    // Release any file handles
    if (_readStream) {
        auto fstream = dynamic_cast<std::ofstream*>(_readStream.get());
        if (fstream) {
            // LTrace("Closing file stream")
            fstream->close();
        }
    }

    Complete.emit(_response);
}


void ClientConnection::onClose()
{
    // LTrace("On close")

    if (!_complete)
        onComplete();
    Close.emit(*this);
}


//
// HTTP Client
//


Singleton<Client>& singleton()
{
    static Singleton<Client> singleton;
    return singleton;
}


Client& Client::instance()
{
    return *singleton().get();
}


void Client::destroy()
{
    singleton().destroy();
}


Client::Client()
{
    // LTrace("Create")

    //_timer.Timeout += sdelegate(this, &Client::onConnectionTimer);
    //_timer.start(5000);
}


Client::~Client()
{
    // LTrace("Destroy")
    shutdown();
}


void Client::shutdown()
{
    // LTrace("Shutdown")

    //_timer.stop();
    Shutdown.emit(/*this*/);

    //_connections.clear();
    auto conns = _connections;
    for (auto conn : conns) {
        // LTrace("Shutdown: ", conn)
        conn->close(); // close and remove via callback
    }
    assert(_connections.empty());
}


void Client::addConnection(ClientConnection::Ptr conn)
{
    // LTrace("Adding connection: ", conn)

    // conn->Close += [&](net::Socket&) {
    //     removeConnection(conn.get());
    // };

    conn->Close += slot(this, &Client::onConnectionClose, -1, -1); // lowest priority
    _connections.push_back(conn);
}


void Client::removeConnection(ClientConnection* conn)
{
    // LTrace("Removing connection: ", conn)
    for (auto it = _connections.begin(); it != _connections.end(); ++it) {
        if (conn == it->get()) {
            // LTrace("Removed connection: ", conn)
            _connections.erase(it);
            return;
        }
    }
    assert(0 && "unknown connection");
}


void Client::onConnectionClose(Connection& conn)
{
    removeConnection(reinterpret_cast<ClientConnection*>(&conn));
}


#if 0
void Client::onConnectionTimer(void*)
{
    // Close connections that have timed out while receiving
    // the server response, maybe due to a faulty server.
    auto conns = _connections;
    for (auto conn : conns) {
        if (conn->closed()) { // conn->expired()
            // LTrace("Closing expired connection: ", conn)
            conn->close();
        }
    }
}
#endif


} // namespace http
} // namespace scy


/// @\}
