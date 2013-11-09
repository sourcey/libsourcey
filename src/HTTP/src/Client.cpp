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


#include "Sourcey/HTTP/Client.h"
#include "Sourcey/Net/SSLSocket.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Util.h"


using namespace std;


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
	traceL("ClientConnection", this) << "Create: " << url << endl;

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
	traceL("ClientConnection", this) << "Create: " << url << endl;

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
	traceL("ClientConnection", this) << "Destroy" << endl;

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
	traceL("ClientConnection", this) << "Sending" << endl;	

	if (_socket.Connect.refCount() == 0) {
		_socket.Connect += delegate(this, &ClientConnection::onSocketConnect);
		_socket.connect(_url.host(), _url.port());
	}
}


void ClientConnection::send(http::Request& req)
{
	_request = req;
	send();
}


void ClientConnection::sendData(const char* buf, std::size_t len) //, int flags
{
	Connection::sendData(buf, len);
	send();
}


void ClientConnection::sendData(const std::string& buf) //, int flags
{
	Connection::sendData(buf);
	send();
}


void ClientConnection::setReadStream(std::ostream* os)
{
	// TODO: assert not running
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
	traceL("ClientConnection", this) << "On connect" << endl;

	// Don't desconnect the delegate so send() won't be called again
	//_socket.Connect -= delegate(this, &ClientConnection::onSocketConnect);
	
	// Emit the connect signal so raw connections like
	// websockets can kick off the data flow
	Connect.emit(this);

	// Start the outgoing send stream if there are
	// any queued packets or adapters attached
	if (Outgoing.base().size() ||
		Outgoing.base().numAdapters())
		Outgoing.start();
	
	// Send the outgoing HTTP header if there 
	// is no output stream.
	else
		sendHeader();
}
	

//
// Connection callbacks
//

void ClientConnection::onHeaders() 
{
	traceL("ClientConnection", this) << "On headers" << endl;	
	_incomingProgress.total = _response.getContentLength();

	Headers.emit(this, _response);
}


void ClientConnection::onPayload(const MutableBuffer& buffer)
{
	//traceL("ClientConnection", this) << "On payload: " << std::string(bufferCast<const char*>(buffer), buffer.size()) << endl;	
	
	// Update download progress
	_incomingProgress.update(buffer.size());

	if (_readStream) {		
		traceL("ClientConnection", this) << "writing to stream: " << buffer.size() << endl;	
		_readStream->write(bufferCast<const char*>(buffer), buffer.size());
		_readStream->flush();
	}

	//Payload.emit(this, buffer);
}


void ClientConnection::onMessage() 
{
	traceL("ClientConnection", this) << "On complete" << endl;
	
	// Fire Complete and clear delegates so it
	// won't fire again on close or error
	_complete = true;
	Complete.emit(this, _response);
}


void ClientConnection::onClose() 
{
	traceL("ClientConnection", this) << "On close" << endl;	

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
	traceL("http::Client", this) << "Create" << endl;

	timer.Timeout += delegate(this, &Client::onTimer);
	timer.start(5000);
}


Client::~Client()
{
	traceL("http::Client", this) << "Destroy" << endl;
	shutdown();
}


void Client::shutdown() 
{
	traceL("http::Client", this) << "Shutdown" << endl;

	timer.stop();
	if (!connections.empty())
		Shutdown.emit(this);

	// Connections must remove themselves
	assert(connections.empty());
}


void Client::addConnection(ClientConnection* conn) 
{		
	traceL("http::Client", this) << "Adding connection: " << conn << endl;
	connections.push_back(conn);
}


void Client::removeConnection(ClientConnection* conn) 
{		
	traceL("http::Client", this) << "Removing connection: " << conn << endl;
	for (auto it = connections.begin(); it != connections.end(); ++it) {
		if (conn == *it) {
			connections.erase(it);
			traceL("http::Client", this) << "Removed connection: " << conn << endl;
			return;
		}
	}
	assert(0 && "unknown connection");
}


void Client::onTimer(void*)
{
	//traceL("http::Client", this) << "On timer" << endl;

	/// Close connections that have timed out while receiving
	/// the server response, maybe due to a faulty server.
	ClientConnectionList conns = ClientConnectionList(connections);
	for (auto it = conns.begin(); it != conns.end(); ++it) {
		if ((*it)->expired()) {
			traceL("http::Client", this) << "Closing expired connection: " << *it << endl;
			(*it)->close();
		}			
	}
}


} } // namespace scy::http
