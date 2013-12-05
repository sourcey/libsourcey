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
#include "scy/net/sslsocket.h"
#include "scy/logger.h"
#include "scy/util.h"


using std::endl;


namespace scy { 
namespace http {


// -------------------------------------------------------------------
// Client Connection
//
ClientConnection::ClientConnection(http::Client* client, const URL& url, const net::Socket& socket) : 
	Connection(socket),
	_client(client), 
	_url(url), 
	_readStream(nullptr),
	_complete(false)
{	
	TraceLS(this) << "Create: " << url << endl;

	IncomingProgress.sender = this;
	OutgoingProgress.sender = this;
		
	_request.setURI(url.pathEtc());
	_request.setHost(url.host(), url.port());

	_socket.replaceAdapter(new ClientAdapter(*this));
		
	// Set default error status
	_response.setStatus(http::StatusCode::BadGateway);

	if (_client) {
		_client->Shutdown += delegate(this, &ClientConnection::onClientShutdown);
		_client->addConnection(this);
	}
}


ClientConnection::ClientConnection(const URL& url, const net::Socket& socket) :
	Connection(socket),
	_client(nullptr), 
	_url(url),
	_readStream(nullptr), 
	_complete(false)
{	
	TraceLS(this) << "Create: " << url << endl;

	IncomingProgress.sender = this;
	OutgoingProgress.sender = this;
			
	_request.setURI(url.pathEtc());
	_request.setHost(url.host(), url.port());
	
	// Set default error status
	_response.setStatus(http::StatusCode::BadGateway);

	_socket.replaceAdapter(new ClientAdapter(*this));
}


ClientConnection::~ClientConnection() 
{	
	TraceLS(this) << "Destroy" << endl;

	if (_readStream) {
		delete _readStream;
		_readStream = nullptr;
	}
}

	
void ClientConnection::close()
{
	if (!_complete) {
		_closed = true; // in case close() is called inside callback 
		Complete.emit(this, _response);
		_closed = false;
	}

	if (!closed()) {
		if (_client) {
			_client->Shutdown -= delegate(this, &ClientConnection::onClientShutdown);
			_client->removeConnection(this);
		}

		Connection::close();
	}
}


void ClientConnection::send()
{
	// TODO: assert not connected
	connect();
}


void ClientConnection::send(http::Request& req)
{
	// TODO: assert not connected
	_request = req;
	connect();
}


void ClientConnection::sendData(const char* buf, std::size_t len) //, int flags
{
	connect();
	if (Outgoing.active())
		Connection::sendData(buf, len);
	else
		_outgoingBuffer.push_back(std::string(buf, len));	
}


void ClientConnection::sendData(const std::string& buf) //, int flags
{
	connect();
	if (Outgoing.active())
		Connection::sendData(buf);
	else
		_outgoingBuffer.push_back(buf);	
}


void ClientConnection::connect()
{
	if (_socket.Connect.ndelegates() == 0) {

		TraceLS(this) << "Connecting" << endl;	
		_socket.Connect += delegate(this, &ClientConnection::onSocketConnect);
		_socket.connect(_url.host(), _url.port());
	}
}


void ClientConnection::setReadStream(std::ostream* os)
{
	// TODO: assert connection not active
	if (_readStream) {
		delete _readStream;
	}

	_readStream = os;
}

			
http::Client* ClientConnection::client()
{
	return _client;
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
// Socket callbacks
//

void ClientConnection::onSocketConnect(void*) 
{
	TraceLS(this) << "On connect" << endl;

	// Don't disconnect the delegate so send() won't be called again
	//_socket.Connect -= delegate(this, &ClientConnection::onSocketConnect);
	
	// Emit the connect signal so raw connections like
	// websockets can kick off the data flow
	Connect.emit(this);

	// Start the outgoing send stream if there are
	// any queued packets or adapters attached
	//if (Outgoing.base().size() ||
	//	Outgoing.base().numAdapters())
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
	else if (Outgoing.base().numAdapters() == 0) {
		TraceLS(this) << "On connect: Send header" << endl;
		sendHeader();
	}	
}
	

//
// Connection callbacks
//

void ClientConnection::onHeaders() 
{
	TraceLS(this) << "On headers" << endl;	
	_incomingProgress.total = _response.getContentLength();

	Headers.emit(this, _response);
}


void ClientConnection::onPayload(const MutableBuffer& buffer)
{
	//TraceLS(this) << "On payload: " << std::string(bufferCast<const char*>(buffer), buffer.size()) << endl;	
	
	// Update download progress
	_incomingProgress.update(buffer.size());

	if (_readStream) {		
		TraceLS(this) << "writing to stream: " << buffer.size() << endl;	
		_readStream->write(bufferCast<const char*>(buffer), buffer.size());
		_readStream->flush();
	}

	//Payload.emit(this, buffer);
}


void ClientConnection::onMessage() 
{
	TraceLS(this) << "On complete" << endl;
	
	// Fire Complete and clear delegates so it
	// won't fire again on close or error
	_complete = true;
	Complete.emit(this, _response);
}


void ClientConnection::onClose() 
{
	TraceLS(this) << "On close" << endl;	

	Connection::onClose();
}


void ClientConnection::onClientShutdown(void*)
{
	close();
}

// ---------------------------------------------------------------------
//

ClientConnection* createConnection(const URL& url)
{
	ClientConnection* conn = 0;

	if (url.scheme() == "http") {
		conn = new ClientConnection(url);
	}
	else if (url.scheme() == "https") {
		conn = new ClientConnection(url, net::SSLSocket());
	}
	else if (url.scheme() == "ws") {
		conn = new ClientConnection(url);
		conn->socket().replaceAdapter(new WebSocketConnectionAdapter(*conn, WebSocket::ClientSide));
	}
	else if (url.scheme() == "wss") {
		conn = new ClientConnection(url, net::SSLSocket());
		conn->socket().replaceAdapter(new WebSocketConnectionAdapter(*conn, WebSocket::ClientSide));
	}
	else
		throw std::runtime_error("Unknown connection type for URL: " + url.str());

	return conn;
}


// ---------------------------------------------------------------------
//
Client::Client()
{
	TraceLS(this) << "Create" << endl;

	timer.Timeout += delegate(this, &Client::onTimer);
	timer.start(5000);
}


Client::~Client()
{
	TraceLS(this) << "Destroy" << endl;
	shutdown();
}


void Client::shutdown() 
{
	TraceLS(this) << "Shutdown" << endl;

	timer.stop();
	if (!connections.empty())
		Shutdown.emit(this);

	// Connections must remove themselves
	assert(connections.empty());
}


void Client::addConnection(ClientConnection* conn) 
{		
	TraceLS(this) << "Adding connection: " << conn << endl;
	connections.push_back(conn);
}


void Client::removeConnection(ClientConnection* conn) 
{		
	TraceLS(this) << "Removing connection: " << conn << endl;
	for (auto it = connections.begin(); it != connections.end(); ++it) {
		if (conn == *it) {
			connections.erase(it);
			TraceLS(this) << "Removed connection: " << conn << endl;
			return;
		}
	}
	assert(0 && "unknown connection");
}


void Client::onTimer(void*)
{
	//TraceLS(this) << "On timer" << endl;

	/// Close connections that have timed out while receiving
	/// the server response, maybe due to a faulty server.
	ClientConnectionList conns = ClientConnectionList(connections);
	for (auto it = conns.begin(); it != conns.end(); ++it) {
		if ((*it)->expired()) {
			TraceLS(this) << "Closing expired connection: " << *it << endl;
			(*it)->close();
		}			
	}
}


} } // namespace scy::http
