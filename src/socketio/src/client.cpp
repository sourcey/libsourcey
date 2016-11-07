///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup socketio
/// @{


#include "scy/socketio/client.h"
#include "scy/net/tcpsocket.h"
#include "scy/net/sslsocket.h"
#include "scy/http/client.h"
#include <stdexcept>


using std::endl;


namespace scy {
namespace sockio {


//
// TCP Client
//


Client* createTCPClient(const Client::Options& options, uv::Loop* loop)
{
    return new Client(std::make_shared<net::TCPSocket>(loop), options); //, loop
}


TCPClient::TCPClient(const Client::Options& options, uv::Loop* loop) :
    Client(std::make_shared<net::TCPSocket>(loop), options) //, loop
{
}


//
// SSL Client
//


Client* createSSLClient(const Client::Options& options, uv::Loop* loop)
{
    return new Client(std::make_shared<net::SSLSocket>(loop), options); //, loop);
}


SSLClient::SSLClient(const Client::Options& options, uv::Loop* loop) :
    Client(std::make_shared<net::SSLSocket>(loop), options) //, loop)
{
}


//
// Base Client
//


// Client::Client(const net::Socket::Ptr& socket) :
//     _pingTimer(socket->loop()),
//     _ws(socket),
//     _wasOnline(false),
//     // _reconnection(false)
// {
//     _ws.addReceiver(this);
// }


Client::Client(const net::Socket::Ptr& socket, const Client::Options& options) :
    _pingTimer(socket->loop()),
    _pingTimeoutTimer(socket->loop()),
    _reconnectTimer(socket->loop()),
    // _options.host(host),
    // _options.port(port),
    _options(options),
    _ws(socket),
    _wasOnline(false)
    // _reconnection(false)
{
    _ws.addReceiver(this);
}


Client::~Client()
{
    _ws.removeReceiver(this);
    //_ws.remove(this);
    //_ws.adapter = nullptr;
    close();
    //reset();
}


// void Client::connect(const std::string& host, std::uint16_t port)
// {
//     {
//         //Mutex::ScopedLock lock(_mutex);
//         _options.host = host;
//         _options.port = port;
//     }
//     connect();
// }


void Client::connect()
{
    TraceN(this) << "Socket.IO Connecting" << endl;

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
    url << time(NULL);

    _ws.request().setURI(url.str());
    _ws.request().setHost(_options.host, _options.port);
    _ws.socket->connect(_options.host, _options.port);
}


void Client::close()
{
    TraceN(this) << "Closing" << endl;
    if (_sessionID.empty())
        return;

    reset();
    onClose();
    TraceN(this) << "Closing: OK" << endl;
}


int Client::send(const std::string& message, bool ack)
{
    Packet packet(message, ack);
    return send(packet);
}


int Client::send(const json::Value& message, bool ack)
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


int Client::send(const std::string& event, const json::Value& message, bool ack)
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
    _pingTimeoutTimer.start(_pingTimeout);

    // TraceN(this) << "Sending ping" << endl;
    return _ws.send("2", 1);
}


void Client::reset()
{
    // Mutex::ScopedLock lock(_mutex);

    // Note: Only reset session related variables here.
    // Do not reset host and port variables.

    _pingTimer.Timeout -= delegate(this, &Client::onPingTimer);
    _pingTimer.stop();

    _pingTimeoutTimer.Timeout -= delegate(this, &Client::onPingTimeoutTimer);
    _pingTimeoutTimer.stop();

    _reconnectTimer.Timeout -= delegate(this, &Client::onReconnectTimer);
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
    ErrorN(this) << "Set error: " << error.message << std::endl;

    // Set the wasOnline flag if previously online before error
    if (stateEquals(ClientState::Online))
        _wasOnline = true;

    _error = error;
    setState(this, ClientState::Error, error.message);

    // Start the reconnection timer if required
    if (_options.reconnection) {
        startReconnectTimer();

        // Note: Do not call close() here, since we will be trying to reconnect...
    }
    else {
        close();
    }
}


void Client::onConnect()
{
    TraceN(this) << "On connect" << endl;

    setState(this, ClientState::Connected);

    // Stop the reconnection timer if required
    stopReconnectTimer();
}


void Client::startReconnectTimer()
{
    assert(_options.reconnection);
    _reconnectTimer.Timeout += delegate(this, &Client::onReconnectTimer);
    _reconnectTimer.start(_options.reconnectDelay);
    _reconnectTimer.handle().ref();
    _reconnecting = true;
}


void Client::stopReconnectTimer()
{
    if (_reconnecting) {
        _reconnectTimer.Timeout -= delegate(this, &Client::onReconnectTimer);
        _reconnectTimer.handle().unref();
        _reconnectTimer.stop();
        _reconnecting = false;
    }
}


void Client::onOnline()
{
    TraceN(this) << "On online" << endl;

    assert(stateEquals(ClientState::Connected));
    setState(this, ClientState::Online);

    // Setup and start the ping timer
    assert(_pingInterval);
    _pingTimer.Timeout += delegate(this, &Client::onPingTimer);
    _pingTimer.start(_pingInterval); //, _pingInterval

    // Setup the ping timeout timer
    assert(_pingTimeout);
    _pingTimeoutTimer.Timeout += delegate(this, &Client::onPingTimeoutTimer);
}


void Client::onClose()
{
    TraceN(this) << "On close" << endl;

    stopReconnectTimer();

    // Back to initial Closed state
    setState(this, ClientState::Closed);
    _wasOnline = false;
}


//
// Socket Callbacks

void Client::onSocketConnect()
{
    onConnect();
}


void Client::onSocketError(const scy::Error& error)
{
    TraceN(this) << "On socket error: " << error.message << endl;

    setError(error);
}


void Client::onSocketClose()
{
    TraceN(this) << "On socket close" << endl;

    // Nothing to do since the error is set via onSocketError

    // If no socket error was set we have an EOF
    //if (!error().any())
    //    setError("Disconnected from the server");
}


void Client::onSocketRecv(const MutableBuffer& buffer, const net::Address& peerAddress)
{
    TraceN(this) << "On socket recv: " << buffer.size() << endl;

    sockio::Packet pkt;
    char* buf = bufferCast<char*>(buffer);
    std::size_t len = buffer.size();
    std::size_t nread = 0;
    while (len > 0 && (nread = pkt.read(constBuffer(buf, len))) > 0) {
        onPacket(pkt);
        buf += nread;
        len -= nread;
    }
    if (len == buffer.size())
        WarnN(this) << "Failed to parse incoming Socket.IO packet." << endl;

#if 0
    sockio::Packet pkt;
    if (pkt.read(constBuffer(packet.data(), packet.size())))
        onPacket(pkt);
    else
        WarnN(this) << "Failed to parse incoming Socket.IO packet." << endl;
#endif
}


void Client::onHandshake(sockio::Packet& packet)
{
    assert(stateEquals(ClientState::Connected));
    assert(packet.frame() == sockio::Packet::Frame::Open);

    json::Value json = packet.json();
    if (json.isMember("sid")) _sessionID = json["sid"].asString();
    if (json.isMember("pingInterval")) _pingInterval = json["pingInterval"].asInt();
    if (json.isMember("pingTimeout")) _pingTimeout = json["pingTimeout"].asInt();

    DebugN(this) << "On handshake: "
        << "sid=" << _sessionID << ", "
        << "pingInterval=" << _pingInterval << ", "
        << "pingTimeout=" << _pingTimeout
        <<  endl;
}


void Client::onMessage(sockio::Packet& packet)
{
    switch (packet.type()) {
        case Packet::Packet::Type::Connect:
            // Transition to online state
            onOnline();
            break;
        case Packet::Packet::Type::Disconnect:
            // Do nothing, attempt to reconnect after ping timeout
            break;
        case Packet::Packet::Type::Event:
            assert(stateEquals(ClientState::Online));
            // PacketSignal::
            emit(this, packet);
            break;
        case Packet::Packet::Type::Ack:
            // assert(stateEquals(ClientState::Online));
            // PacketSignal::
            emit(this, packet);
            break;
        case Packet::Packet::Type::Error:
            // assert(stateEquals(ClientState::Online));
            // PacketSignal::
            emit(this, packet);
            break;
        case Packet::Packet::Type::BinaryEvent:
            assert(0 && "not implemented");
            break;
        case Packet::Packet::Type::BinaryAck:
            assert(0 && "not implemented");
            break;
        default: assert(0 && "unknown type");
    }
}


void Client::onPacket(sockio::Packet& packet)
{
    TraceN(this) << "On packet: " << packet.toString() << endl;

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
        default: assert(0 && "unknown type");
    }
}


void Client::onPong()
{
    // Pong received, stop the ping timeout
    _pingTimeoutTimer.stop();
}


void Client::onPingTimer()
{
    TraceN(this) << "On heartbeat" << endl;

    // Do nothing unless online
    if (!isOnline())
        return;

    sendPing();
    _pingTimer.again();
}


void Client::onPingTimeoutTimer()
{
    assert(0 && "implement me");
}


void Client::onReconnectTimer()
{
    TraceN(this) << "On reconnect timer" << endl;
    try {
        connect();
    }
    catch (std::exception& exc) {
        ErrorN(this) << "Reconnection attempt failed: " << exc.what() << endl;
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
    //return _ws.socket->error();
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
    //Mutex::ScopedLock lock(_mutex);
    return _wasOnline;
}


} } // namespace scy::sockio

/// @\}
