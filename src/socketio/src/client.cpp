///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup socketio
/// @{


#include "scy/socketio/client.h"
#include "scy/http/client.h"
#include "scy/net/sslsocket.h"
#include "scy/net/tcpsocket.h"
#include <stdexcept>


using std::endl;


namespace scy {
namespace sockio {


//
// TCP Client
//


Client* createTCPClient(const Client::Options& options, uv::Loop* loop)
{
    return new Client(std::make_shared<net::TCPSocket>(loop), options);
}


TCPClient::TCPClient(const Client::Options& options, uv::Loop* loop)
    : Client(std::make_shared<net::TCPSocket>(loop), options)
{
}


//
// SSL Client
//


Client* createSSLClient(const Client::Options& options, uv::Loop* loop)
{
    return new Client(std::make_shared<net::SSLSocket>(loop), options);
}


SSLClient::SSLClient(const Client::Options& options, uv::Loop* loop)
    : Client(std::make_shared<net::SSLSocket>(loop), options)
{
}


//
// Base Client
//


Client::Client(const net::Socket::Ptr& socket, const Client::Options& options)
    : _pingTimer(socket->loop())
    , _pingTimeoutTimer(socket->loop())
    , _reconnectTimer(socket->loop())
    , _options(options)
    , _ws(socket)
    , _wasOnline(false)
{
    _ws.addReceiver(this);
}


Client::~Client()
{
    _ws.removeReceiver(this);

    close();
}


void Client::connect()
{
    LTrace("Socket.IO Connecting")

    if (_options.host.empty() || !_options.port)
        throw std::runtime_error("The Socket.IO server address is not set.");

    reset();

    setState(this, ClientState::Connecting);

    // Build the request
    // TODO: Allow custom URI params
    std::ostringstream url;
    url << "/socket.io/?EIO=4&transport=websocket";
    if (!_sessionID.empty()) {
        url << "&sid=";
        url << _sessionID;
    }
    url << "&t=";
    url << std::time(NULL);

    _ws.request().setURI(url.str());
    _ws.request().setHost(_options.host, _options.port);
    _ws.socket->connect(_options.host, _options.port);
}


void Client::close()
{
    LTrace("Closing")
    if (_sessionID.empty())
        return;

    reset();
    onClose();
    LTrace("Closing: OK")
}


int Client::send(const std::string& message, bool ack)
{
    Packet packet(message, ack);
    return send(packet);
}


int Client::send(const json::value& message, bool ack)
{
    Packet packet(message, ack);
    return send(packet);
}


int Client::send(const sockio::Packet& packet)
{
    return _ws.sendPacket(packet);
}


int Client::send(const std::string& event, const char* message, bool ack)
{
    Packet packet(event, std::string(message), ack);
    return send(packet);
}


int Client::send(const std::string& event, const std::string& message, bool ack)
{
    Packet packet(event, message, ack);
    return send(packet);
}


int Client::send(const std::string& event, const json::value& message, bool ack)
{
    Packet packet(event, message, ack);
    return send(packet);
}


Transaction* Client::createTransaction(const sockio::Packet& request, long timeout)
{
    return new Transaction(*this, request, timeout);
}


int Client::sendPing()
{
    // Start the ping timeout
    _pingTimeoutTimer.stop();
    _pingTimeoutTimer.setTimeout(_pingTimeout);
    _pingTimeoutTimer.start();

    LTrace("Sending ping")
    return _ws.send("2", 1);
}


void Client::reset()
{
    // Note: Only reset session related variables here.
    // Do not reset host and port variables.

    _pingTimer.Timeout -= slot(this, &Client::onPingTimer);
    _pingTimer.stop();

    _pingTimeoutTimer.Timeout -= slot(this, &Client::onPingTimeoutTimer);
    _pingTimeoutTimer.stop();

    _reconnectTimer.Timeout -= slot(this, &Client::onReconnectTimer);
    _reconnectTimer.stop();

    _ws.socket->close();

    _error.reset();
    _sessionID = "";
    _pingInterval = 0;
    _pingTimeout = 0;
    // _wasOnline = false; // Reset via onClose()
}


void Client::setError(const scy::Error& error)
{
    LError("Set error: ", error.message)

    // Set the wasOnline flag if previously online before error
    if (stateEquals(ClientState::Online))
        _wasOnline = true;

    _error = error;
    setState(this, ClientState::Error);

    // Start the reconnection timer if required
    if (_options.reconnection) {
        startReconnectTimer();

        // Note: Do not call close() here, since we will be trying to reconnect...
    } else {
        close();
    }
}


void Client::onConnect()
{
    LTrace("On connect")

    setState(this, ClientState::Connected);

    // Stop the reconnection timer if required
    stopReconnectTimer();
}


void Client::startReconnectTimer()
{
    assert(_options.reconnection);
    _reconnectTimer.Timeout += slot(this, &Client::onReconnectTimer);
    _reconnectTimer.setTimeout(_options.reconnectDelay);
    _reconnectTimer.start();
    _reconnectTimer.handle().ref();
    _reconnecting = true;
}


void Client::stopReconnectTimer()
{
    if (_reconnecting) {
        _reconnectTimer.Timeout -= slot(this, &Client::onReconnectTimer);
        _reconnectTimer.handle().unref();
        _reconnectTimer.stop();
        _reconnecting = false;
    }
}


void Client::onOnline()
{
    LTrace("On online")

    assert(stateEquals(ClientState::Connected));
    setState(this, ClientState::Online);

    // Setup and start the ping timer
    assert(_pingInterval);
    _pingTimer.Timeout += slot(this, &Client::onPingTimer);
    _pingTimer.setTimeout(_pingInterval);
    _pingTimer.setInterval(_pingInterval);
    _pingTimer.start();

    // Setup the ping timeout timer
    assert(_pingTimeout);
    _pingTimeoutTimer.Timeout += slot(this, &Client::onPingTimeoutTimer);
}


void Client::onClose()
{
    LTrace("On close")

    stopReconnectTimer();

    // Back to initial Closed state
    setState(this, ClientState::Closed);
    _wasOnline = false;
}


//
// Socket Callbacks

void Client::onSocketConnect(net::Socket& socket)
{
    onConnect();
}


void Client::onSocketError(net::Socket& socket, const scy::Error& error)
{
    LTrace("On socket error: ", error.message)

    setError(error);
}
//

void Client::onSocketClose(net::Socket& socket)
{
    LTrace("On socket close")

    // Nothing to do since the error is set via onSocketError

    // If no socket error was set we have an EOF
    // if (!error().any())
    //    setError("Disconnected from the server");
}


void Client::onSocketRecv(net::Socket& socket, const MutableBuffer& buffer, const net::Address& peerAddress)
{
    LTrace("On socket recv: ", buffer.size())

    sockio::Packet pkt;
    char* buf = bufferCast<char*>(buffer);
    size_t len = buffer.size();
    size_t nread = 0;
    while (len > 0 && (nread = pkt.read(constBuffer(buf, len))) > 0) {
        onPacket(pkt);
        buf += nread;
        len -= nread;
    }
    if (len == buffer.size())
        LWarn("Failed to parse incoming Socket.IO packet.")

#if 0
    sockio::Packet pkt;
    if (pkt.read(constBuffer(packet.data(), packet.size())))
        onPacket(pkt);
    else
        LWarn("Failed to parse incoming Socket.IO packet.")
#endif
}


void Client::onHandshake(sockio::Packet& packet)
{
    LTrace("On handshake: ", state())
    // assert(stateEquals(ClientState::Connected));
    assert(packet.frame() == sockio::Packet::Frame::Open);

    json::value json = packet.json();
    if (json.find("sid") != json.end())
        _sessionID = json["sid"].get<std::string>();
    if (json.find("pingInterval") != json.end())
        _pingInterval = json["pingInterval"].get<int>();
    if (json.find("pingTimeout") != json.end())
        _pingTimeout = json["pingTimeout"].get<int>();

    SDebug << "On handshake: "
                 << "sid=" << _sessionID << ", "
                 << "pingInterval=" << _pingInterval << ", "
                 << "pingTimeout=" << _pingTimeout << endl;

}


void Client::onMessage(sockio::Packet& packet)
{
    LTrace("On message: ", packet.toString())

    switch (packet.type()) {
        case Packet::Packet::Type::Connect:
            // Transition to online state
            onOnline();
            break;
        case Packet::Packet::Type::Disconnect:
            // Do nothing, attempt to reconnect after ping timeout
            break;
        case Packet::Packet::Type::Event:
            // assert(stateEquals(ClientState::Online));
            emit(packet);
            break;
        case Packet::Packet::Type::Ack:
            // assert(stateEquals(ClientState::Online));
            emit(packet);
            break;
        case Packet::Packet::Type::Error:
            // assert(stateEquals(ClientState::Online));
            emit(packet);
            break;
        case Packet::Packet::Type::BinaryEvent:
            assert(0 && "not implemented");
            break;
        case Packet::Packet::Type::BinaryAck:
            assert(0 && "not implemented");
            break;
        default:
            assert(0 && "unknown type");
    }
}


void Client::onPacket(sockio::Packet& packet)
{
    LTrace("On packet: ", packet.toString())

    // Handle packets by frame type
    switch (packet.frame()) {
        case Packet::Frame::Open:
            onHandshake(packet);
            break;
        case Packet::Frame::Close:
            reset(); // close everything down
            break;
        case Packet::Frame::Pong:
            break;
        case Packet::Frame::Message:
            onMessage(packet);
            break;
        case Packet::Frame::Ping:
        case Packet::Frame::Upgrade:
        case Packet::Frame::Noop:
            break;
        default:
            assert(0 && "unknown type");
    }
}


void Client::onPong()
{
    LTrace("On pong")

    // Pong received, stop the ping timeout
    _pingTimeoutTimer.stop();
}


void Client::onPingTimer()
{
    LTrace("On heartbeat")

    // Do nothing unless online
    if (!isOnline())
        return;

    sendPing();
    //_pingTimer.again();
}


void Client::onPingTimeoutTimer()
{
    LTrace("On ping timeout")

    // assert(0 && "implement me");
}


void Client::onReconnectTimer()
{
    LTrace("On reconnect timer")
    try {
        connect();
    } catch (std::exception& exc) {
        LError("Reconnection attempt failed: ", exc.what())
    }
    _reconnectTimer.again();
}


http::ws::WebSocket& Client::ws()
{
    return _ws;
}


std::string Client::sessionID() const
{
    return _sessionID;
}


Error Client::error() const
{
    // return _ws.socket->error();
    return _error;
}


bool Client::isOnline() const
{
    return stateEquals(ClientState::Online);
}


Client::Options& Client::options()
{
    return _options;
}


bool Client::reconnecting() const
{
    return _reconnecting;
}


bool Client::wasOnline() const
{

    return _wasOnline;
}


} // namespace sockio
} // namespace scy


/// @\}
