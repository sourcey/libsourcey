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


#include "scy/http/client.h"
#include "scy/logger.h"
#include "scy/util.h"


using std::endl;


namespace scy { 
namespace http {


//
// Client Connection
//


ClientConnection::ClientConnection(const URL& url, const net::Socket::Ptr& socket) :
    Connection(socket),
    _url(url),
    _readStream(nullptr), 
    _complete(false),
    _connect(false)
{    
    TraceS(this) << "Create: " << url << endl;

    IncomingProgress.sender = this;
    OutgoingProgress.sender = this;
            
    _request.setURI(url.pathEtc());
    _request.setHost(url.host(), url.port());
    
    // Set default error status
    _response.setStatus(http::StatusCode::BadGateway);

    replaceAdapter(new ClientAdapter(*this));
}


ClientConnection::~ClientConnection() 
{    
    TraceS(this) << "Destroy" << endl;

    if (_readStream) {
        delete _readStream;
        _readStream = nullptr;
    }
}


void ClientConnection::close()
{
    if (!closed()) {
        onComplete();
        Connection::close();
    }
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


int ClientConnection::send(const char* data, std::size_t len, int flags)
{
    connect();
    if (Outgoing.active())
        return Connection::send(data, len);
    else
        _outgoingBuffer.push_back(std::string(data, len));    
    return len;
}


#if 0
int ClientConnection::send(const std::string& buf, int flags) //, int flags
{
    connect();
    if (Outgoing.active())
        return Connection::send(buf);
    else
        _outgoingBuffer.push_back(buf);    
    return buf.length();
}


void ClientConnection::sendData(const char* buf, std::size_t len) //, int flags
{
    connect();
    if (Outgoing.active())
        Connection::sendData(buf, len);
    else
        _outgoingBuffer.push_back(std::string(buf, len));    
}

    
http::Client* ClientConnection::client()
{
    return _client;
}
#endif


void ClientConnection::connect()
{
    if (!_connect) {
        _connect = true;
        TraceS(this) << "Connecting" << endl;    
        _socket->connect(_url.host(), _url.port());
    }
}


void ClientConnection::setReadStream(std::ostream* os)
{
    assert(!_connect);
    if (_readStream) {
        delete _readStream;
    }

    _readStream = os;
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

void ClientConnection::onSocketConnect() 
{
    TraceS(this) << "On connect" << endl;
    
    // Emit the connect signal so raw connections like
    // websockets can kick off the data flow
    Connect.emit(this);

    // Start the outgoing send stream if there are
    // any queued packets or adapters attached
    Outgoing.start();

    // Flush queued packets
    if (!_outgoingBuffer.empty()) {
        for (const auto & packet : _outgoingBuffer) {
            Outgoing.write(packet.c_str(), packet.length());
        }
        _outgoingBuffer.clear();
    }

    // Send the outgoing HTTP header if it hasn't already been sent. 
    // Note the first call to socket().send() will flush headers.
    // Note if there are stream adapters we wait for the stream to push
    // through any custom headers. See ChunkedAdapter::emitHeader
    else if (Outgoing.numAdapters() == 0) {
        TraceS(this) << "On connect: Send header" << endl;
        sendHeader();
    }    
}
    

//
// Connection Callbacks

void ClientConnection::onHeaders() 
{
    TraceS(this) << "On headers" << endl;    
    IncomingProgress.total = _response.getContentLength();

    Headers.emit(this, _response);
}


void ClientConnection::onPayload(const MutableBuffer& buffer)
{
    //TraceS(this) << "On payload: " << 
    // std::string(bufferCast<const char*>(buffer), buffer.size()) << endl;    
    
    // Update download progress
    IncomingProgress.update(buffer.size());

    if (_readStream) {        
        TraceS(this) << "Writing to stream: " << buffer.size() << endl;    
        _readStream->write(bufferCast<const char*>(buffer), buffer.size());
        _readStream->flush();
    }

    //Payload.emit(this, buffer);
}


void ClientConnection::onMessage() 
{
    TraceS(this) << "On complete" << endl;

    onComplete();
}

    
void ClientConnection::onComplete()
{
    if (!_complete) {
        _complete = true; // in case close() is called inside callback 
        Complete.emit(this, _response);
    }
}


void ClientConnection::onClose() 
{
    TraceS(this) << "On close" << endl;    

    Connection::onClose();
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
    TraceS(this) << "Create" << endl;

    //_timer.Timeout += sdelegate(this, &Client::onConnectionTimer);
    //_timer.start(5000);
}


Client::~Client()
{
    TraceS(this) << "Destroy" << endl;
    shutdown();
}


void Client::shutdown() 
{
    TraceS(this) << "Shutdown" << endl;

    //_timer.stop();
    Shutdown.emit(this);

    //_connections.clear();
    auto conns = _connections;
    for (auto conn : conns) {
        conn->close(); // close and remove via callback
    }
    assert(_connections.empty());
}


void Client::addConnection(ClientConnection::Ptr conn) 
{        
    TraceS(this) << "Adding connection: " << conn << endl;    

    conn->Close += sdelegate(this, &Client::onConnectionClose, -1); // lowest priority
    _connections.push_back(conn);
}


void Client::removeConnection(ClientConnection* conn) 
{        
    TraceS(this) << "Removing connection: " << conn << endl;
    for (auto it = _connections.begin(); it != _connections.end(); ++it) {
        if (conn == it->get()) {
            TraceS(this) << "Removed connection: " << conn << endl;
            _connections.erase(it);
            return;
        }
    }
    assert(0 && "unknown connection");
}


void Client::onConnectionClose(void* sender)
{
    removeConnection(reinterpret_cast<ClientConnection*>(sender));
}


#if 0
void Client::onConnectionTimer(void*)
{
    // Close connections that have timed out while receiving
    // the server response, maybe due to a faulty server.
    auto conns = _connections;
    for (auto conn : conns) {
        if (conn->closed()) { // conn->expired()
            TraceS(this) << "Closing expired connection: " << conn << endl;
            conn->close();
        }            
    }
}
#endif


} } // namespace scy::http
