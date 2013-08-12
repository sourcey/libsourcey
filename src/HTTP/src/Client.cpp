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
	_readStream(nil)
{	
	traceL("ClientConnection", this) << "Creating: " << url << endl;

	IncomingProgress.sender = this;
	OutgoingProgress.sender = this;
		
	_request.setURI(url.str());

	_socket.replaceAdapter(new ClientAdapter(*this));

	if (_client) {
		_client->Shutdown += delegate(this, &ClientConnection::onClientShutdown);
		_client->addConnection(this);
	}
}


ClientConnection::ClientConnection(const URL& url, const net::Socket& socket) :
	Connection(socket),
	_client(nil), 
	_url(url), 
	_readStream(nil)
{	
	traceL("ClientConnection", this) << "Creating: " << url << endl;

	IncomingProgress.sender = this;
	OutgoingProgress.sender = this;
	
	_request.setURI(url.str());

	_socket.replaceAdapter(new ClientAdapter(*this));
}


ClientConnection::~ClientConnection() 
{	
	traceL("ClientConnection", this) << "Destroying" << endl;

	//if (!_readStream)
	//	_readStream->destroy();
	//if (!_readStreamStream)
	//	_readStreamStream->destroy();
}

	
void ClientConnection::close()
{
	if (!closed()) {
		if (_client) {
			_client->Shutdown -= delegate(this, &ClientConnection::onClientShutdown);
			_client->removeConnection(this);
		}
		if (_readStream) {
			delete _readStream;
			_readStream = nil;
		}
		traceL("ClientConnection", this) << "Closing: Complete refc: " << Complete.refCount() << endl;	
		if (Complete.refCount())
			Complete.emit(this, _response);

		Connection::close();
	}
}


void ClientConnection::send()
{
	traceL("ClientConnection", this) << "Sending request" << endl;	
	
	/// Set HTTP_BAD_GATEWAY as default 
	_response.setStatus(http::Response::HTTP_BAD_GATEWAY);

	_socket.Connect += delegate(this, &ClientConnection::onSocketConnect);
	_socket.connect(_url.host(), _url.port());
}


void ClientConnection::send(http::Request& req)
{
	_request = req;
	send();
}


void ClientConnection::setReadStream(std::ostream* os)
{
	/// TODO: assert not running
	if (_readStream)
		delete _readStream;

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
	traceL("ClientConnection", this) << "Connected" << endl;
	_socket.Connect -= delegate(this, &ClientConnection::onSocketConnect);
	
	/// Emit the connect signal so raw connections ie. 
	/// websockets can kick off the data flow
	Connect.emit(this);

	/// Start the outgoing send stream if there  
	/// are any adapters attached.
	if (Outgoing.numAdapters())
		Outgoing.start();
	
	/// Send the outgoing HTTP header if there 
	/// is no output stream.
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


void ClientConnection::onPayload(Buffer& buffer)
{
	traceL("ClientConnection", this) << "On payload" << endl;	
	
	/// Update download progress
	_incomingProgress.update(buffer.available());

	if (_readStream)
		_readStream->write(buffer.data(), buffer.available());

	//Payload.emit(this, buffer);
}


void ClientConnection::onMessage() 
{
	traceL("ClientConnection", this) << "On complete" << endl;
	
	/// Fire Complete and clear delegates so it
	/// won't fire again on close or error
	Complete.emit(this, _response);
	Complete.clear(); 
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
		throw Exception("Unknown connection type for URL: " + url.str());

	return conn;
}


// ---------------------------------------------------------------------
//
Client::Client()
{
	traceL("http::Client", this) << "Creating" << endl;

	timer.Timeout += delegate(this, &Client::onTimer);
	timer.start(5000, 5000);
}


Client::~Client()
{
	traceL("http::Client", this) << "Destroying" << endl;
	shutdown();
}


void Client::shutdown() 
{
	traceL("http::Client", this) << "Shutdown" << endl;

	timer.stop();
	if (!connections.empty())
		Shutdown.emit(this);

	/// Connections must remove themselves
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
	for (ClientConnectionList::iterator it = connections.begin(); it != connections.end(); ++it) {
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
	for (ClientConnectionList::iterator it = conns.begin(); it != conns.end(); ++it) {
		if ((*it)->expired()) {
			traceL("http::Client", this) << "Closing expired connection: " << *it << endl;
			(*it)->close();
		}			
	}
}


} } // namespace scy::http