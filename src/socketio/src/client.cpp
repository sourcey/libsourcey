//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// LibSourcey is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//


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


Client* createTCPClient(uv::Loop* loop)
{
    return new Client(std::make_shared<net::TCPSocket>(loop)); //, loop
}


TCPClient::TCPClient(uv::Loop* loop) :
    Client(std::make_shared<net::TCPSocket>(loop)) //, loop
{
}


//
// SSL Client
//


Client* createSSLClient(uv::Loop* loop)
{
    return new Client(std::make_shared<net::SSLSocket>(loop)); //, loop);
}


SSLClient::SSLClient(uv::Loop* loop) :
    Client(std::make_shared<net::SSLSocket>(loop)) //, loop)
{
}


//
// Base Client
//


Client::Client(const net::Socket::Ptr& socket) :
    _pingTimer(socket->loop()),
    _ws(socket),
    //_loop(loop),
    _wasOnline(false)
{
    _ws.addReceiver(this);
}


Client::Client(const net::Socket::Ptr& socket, const std::string& host, std::uint16_t port) :
    _pingTimer(socket->loop()),
    _host(host),
    _port(port),
    _ws(socket),
    //_loop(loop),
    _wasOnline(false)
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


void Client::connect(const std::string& host, std::uint16_t port)
{
    {
        //Mutex::ScopedLock lock(_mutex);
        _host = host;
        _port = port;
    }
    connect();
}


void Client::connect()
{
    TraceL << "SocketIO Connecting" << endl;

    if (_host.empty() || !_port)
        throw std::runtime_error("The SocketIO server address is not set.");

    reset();

    setState(this, ClientState::Connecting);

    //_ws.Connect += sdelegate(this, &Client::onSocketConnect);
    //_ws.Recv += sdelegate(this, &Client::onSocketRecv);
    //_ws.Error += sdelegate(this, &Client::onSocketError);
    //_ws.Close += sdelegate(this, &Client::onSocketClose);

    // sendHandshakeRequest();


    std::ostringstream url;
    url << "/socket.io/?EIO=4&transport=websocket";
    if (!_sessionID.empty()) {
      url << "&sid=";
      url << _sessionID; // TODO: Get sid from handshake response
    }
    url << "&t=";
    url << time(NULL);

    _ws.request().setURI(url.str());
    _ws.request().setHost(_host, _port);
    _ws.socket->connect(_host, _port);
}


void Client::close()
{
    TraceL << "Closing" << endl;
    if (_sessionID.empty())
        return;

    reset();
    onClose();
    TraceL << "Closing: OK" << endl;
}


// void Client::sendHandshakeRequest()
// {
//     //Mutex::ScopedLock lock(_mutex);
//
//     TraceL << "Send handshake request" << endl;
//
//     std::ostringstream url;
//     url << (_ws.socket->transport() == net::SSLTCP ? "https://" : "http://")
//         << _host << ":" << _port
//         << "/socket.io/1/";
//
//     //http::URL url(
//     //    _ws.socket->transport() == net::SSLTCP ? "https" : "http",
//     //    _endpoint, "/socket.io/1/websocket/");
//     //assert(url.valid());
//
//     auto conn = http::Client::instance().createConnection(url.str());
//     conn->Complete += sdelegate(this, &Client::onHandshakeResponse);
//     conn->setReadStream(new std::stringstream);
//     conn->request().setMethod("POST");
//     conn->request().setKeepAlive(false);
//     conn->request().setContentLength(0);
//     conn->request().setURI("/socket.io/1/");
//     conn->send();
// }
//
//
// void Client::onHandshakeResponse(void* sender, const http::Response& response)
// {
//     auto conn = reinterpret_cast<http::ClientConnection*>(sender);
//
//     std::string body = conn->readStream<std::stringstream>()->str();
//     //TraceL << "SocketIO handshake response:"
//     //    << "\n\tStatus: " << response.getStatus()
//     //    << "\n\tReason: " << response.getReason()
//     //    << "\n\tResponse: " << body << endl;
//
//     // The server can respond in three different ways:
//     // 401 Unauthorized: If the server refuses to authorize the client to connect,
//     //        based on the supplied information (eg: Cookie header or custom query components).
//     // 503 Service Unavailable: If the server refuses the connection for any reason (eg: overload).
//     // 200 OK: The handshake was successful.
//     if (response.getStatus() != http::StatusCode::OK) {
//         setError(util::format("SocketIO handshake failed: HTTP error: %d %s",
//             static_cast<int>(response.getStatus()), response.getReason().c_str()));
//         return;
//     }
//
//     // Parse the response
//     std::vector<std::string> respData = util::split(body, ':', 4);
//     if (respData.size() < 4) {
//         setError(body.empty() ?
//             "Invalid SocketIO handshake response." : util::format(
//             "Invalid SocketIO handshake response: %s", body.c_str()));
//         return;
//     }
//
//     _sessionID = respData[0];
//     _pingTimeout = util::strtoi<std::uint32_t>(respData[1]);
//     _connectionClosingTimeout = util::strtoi<std::uint32_t>(respData[2]);
//     _protocols = util::split(respData[3], ',');
//
//     // Check websockets are supported
//     bool wsSupported = false;
//     for (unsigned i = 0; i < _protocols.size(); i++) {
//         TraceL << "Supports protocol: " << _protocols[i] << endl;
//         if (_protocols[i] == "websocket") {
//             wsSupported = true;
//             break;
//         }
//     }
//
//     if (!wsSupported) {
//         setError("The SocketIO server does not support WebSockets.");
//         return;
//     }
//
//     //Mutex::ScopedLock lock(_mutex);
//
//     // Initialize the WebSocket
//     TraceL << "Websocket connecting: " << _sessionID << endl;
//
//     //_ws.setRecvAdapter(this);
//     //_ws.adapter = this;
//     _ws.request().setURI("/socket.io/1/websocket/" + _sessionID);
//     _ws.request().setHost(_host, _port);
//     _ws.socket->connect(_host, _port);
// }
//
//
// int Client::sendConnect(const std::string& endpoint, const std::string& query)
// {
//     // (1) Connect
//     // Only used for multiple sockets. Signals a connection to the endpoint.
//     // Once the server receives it, it's echoed back to the client.
//     //
//     // Example, if the client is trying to connect to the endpoint /test,
//     // a message like this will be delivered:
//     //
//     // '1::' [path] [query]
//     // Example:
//     //
//     // 1::/test?my=param
//     std::string out = "1::";
//     if (!endpoint.empty())
//         out += "/" + endpoint;
//     if (!query.empty())
//         out += "?" + query;
//     return _ws.send(out.c_str(), out.size());
// }


// int Client::send(sockio::Packet::Type type, const std::string& message, bool ack)
// {
//     Packet packet(type, message, ack);
//     return send(packet);
// }


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
    //TraceL << "Sending heartbeat" << endl;
    return _ws.send("2", 1); //::
}


void Client::reset()
{
    //Mutex::ScopedLock lock(_mutex);

    // Note: Only reset session related variables here.
    // Do not reset host and port variables.

    _pingTimer.Timeout -= sdelegate(this, &Client::onPingTimer);
    _pingTimer.stop();

    _pingTimeoutTimer.Timeout -= sdelegate(this, &Client::onPingTimeoutTimer);
    _pingTimeoutTimer.stop();

    //_ws.socket->Connect -= sdelegate(this, &Client::onSocketConnect);
    //_ws.socket->Recv -= sdelegate(this, &Client::onSocketRecv);
    //_ws.socket->Error -= sdelegate(this, &Client::onSocketError);
    //_ws.socket->Close -= sdelegate(this, &Client::onSocketClose);
    _ws.socket->close();

    _sessionID = "";
    _pingInterval = 0;
    _pingTimeout = 0;
    //_wasOnline = false; // Reset via onClose()
    _error.reset();

}


void Client::setError(const scy::Error& error)
{
    ErrorL << "Set error: " << error.message << std::endl;

    // Set the wasOnline flag if previously online before error
    if (stateEquals(ClientState::Online))
        _wasOnline = true;

    _error = error;
    setState(this, ClientState::Error, error.message);

    // Note: Do not call close() here, since we will be trying to reconnect...
}


void Client::onConnect()
{
    TraceL << "On connect" << endl;

    setState(this, ClientState::Connected);

    //Mutex::ScopedLock lock(_mutex);
}


void Client::onOnline()
{
    TraceL << "On online" << endl;

    assert(stateEquals(ClientState::Connected));

    setState(this, ClientState::Online);

    // Start the ping timer
    assert(_pingInterval);
    // int interval = static_cast<int>(_pingTimeout * .75) * 1000;
    _pingTimer.Timeout += sdelegate(this, &Client::onPingTimer);
    _pingTimer.start(_pingInterval, _pingInterval);

    // Start the ping timeout timer
    assert(_pingTimeout);
    // int interval = static_cast<int>(_pingTimeout * .75) * 1000;
    _pingTimeoutTimer.Timeout += sdelegate(this, &Client::onPingTimeoutTimer);
    _pingTimeoutTimer.start(_pingTimeout);
}


void Client::onClose()
{
    TraceL << "On close" << endl;

    // Back to initial state
    setState(this, ClientState::None);
    _wasOnline = false;
}


//
// Socket Callbacks

void Client::onSocketConnect()
{
    // Start
    onConnect();
}


void Client::onSocketError(const scy::Error& error)
{
    TraceL << "On socket error: " << error.message << endl;

    setError(error);
}


void Client::onSocketClose()
{
    TraceL << "On socket close" << endl;

    // Nothing to do since the error is set via onSocketError

    // If no socket error was set we have an EOF
    //if (!error().any())
    //    setError("Disconnected from the server");
}


void Client::onSocketRecv(const MutableBuffer& buffer, const net::Address& peerAddress)
{
    TraceL << "On socket recv: " << buffer.size() << endl;

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
        WarnL << "Failed to parse incoming SocketIO packet." << endl;

#if 0
    sockio::Packet pkt;
    if (pkt.read(constBuffer(packet.data(), packet.size())))
        onPacket(pkt);
    else
        WarnL << "Failed to parse incoming SocketIO packet." << endl;
#endif
}


void Client::onHandshake(sockio::Packet& packet) {
    assert(packet.frame() == sockio::Packet::Frame::Open);

    json::Value json = packet.json();
    if (json.isMember("sid")) _sessionID = json["sid"].asString();
    if (json.isMember("pingInterval")) _pingInterval = json["pingInterval"].asInt();
    if (json.isMember("pingTimeout")) _pingTimeout = json["pingTimeout"].asInt();

    DebugL << "On handshake: "
        << "sid=" << _sessionID << ", "
        << "pingInterval=" << _pingInterval << ", "
        << "pingTimeout=" << _pingTimeout << ", "
        <<  endl;
}


void Client::onMessage(sockio::Packet& packet) {
    switch (packet.type()) {
        case Packet::Packet::Type::Connect:
            // Transition to online state
            onOnline();
            break;
        case Packet::Packet::Type::Disconnect:
            // Do nothing. Handled by close frame
            break;
        case Packet::Packet::Type::Event:
            assert(stateEquals(ClientState::Online));
            PacketSignal::emit(this, packet);
            break;
        case Packet::Packet::Type::Ack:
            assert(stateEquals(ClientState::Online));
            PacketSignal::emit(this, packet);
            break;
        case Packet::Packet::Type::Error:
            assert(stateEquals(ClientState::Online));
            PacketSignal::emit(this, packet);
            break;
        case Packet::Packet::Type::BinaryEvent:
            assert(0 && "not implemented");
            break;
        case Packet::Packet::Type::BinaryAck:
            assert(0 && "not implemented");
            break;
    }
}


void Client::onPacket(sockio::Packet& packet)
{
    TraceL << "On packet: " << packet.toString() << endl;

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
    }
}


void Client::onPong()
{
    _pingTimeoutTimer.stop();
}


void Client::onPingTimer(void*)
{
    TraceL << "On heartbeat" << endl;

    if (isOnline())
        sendPing();

    // Try to reconnect if disconnected in error
    else if (error().any()) {
        TraceL << "Attempting to reconnect" << endl;
        try {
            connect();
        }
        catch (std::exception& exc) {
            ErrorL << "Reconnection attempt failed: " << exc.what() << endl;
        }
    }
}


void Client::onPingTimeoutTimer(void*)
{
    assert(0 && "implement me");
}


http::ws::WebSocket& Client::ws()
{
    //Mutex::ScopedLock lock(_mutex);
    return _ws;
}


std::string Client::sessionID() const
{
    //Mutex::ScopedLock lock(_mutex);
    return _sessionID;
}


Error Client::error() const
{
    //Mutex::ScopedLock lock(_mutex);
    return _error;
    //return _ws.socket->error();
}


bool Client::isOnline() const
{
    return stateEquals(ClientState::Online);
}


bool Client::wasOnline() const
{
    //Mutex::ScopedLock lock(_mutex);
    return _wasOnline;
}


} } // namespace scy::sockio
