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


#include "Sourcey/HTTP/Server.h"
#include "Sourcey/HTTP/WebSocket.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Util.h"


using namespace std;


namespace scy { 
namespace http {

	
Server::Server(short port, ServerResponderFactory* factory) :
	address("0.0.0.0", port),
	factory(factory)
{
	traceL("http::Server", this) << "Creating" << endl;
}


Server::~Server()
{
	traceL("http::Server", this) << "Destroying" << endl;
	shutdown();
	if (factory)
		delete factory;
}

	
void Server::start()
{	
	/// TODO: Register self as an observer
	socket.base().AcceptConnection += delegate(this, &Server::onAccept);	
	socket.Close += delegate(this, &Server::onClose);
	socket.bind(address);
	socket.listen();

	traceL("http::Server", this) << "Server listening on " << port() << endl;		

	//timer.Timeout += delegate(this, &Server::onTimer);
	//timer.start(5000, 5000);
}


void Server::shutdown() 
{		
	traceL("http::Server", this) << "Shutdown" << endl;

	socket.base().AcceptConnection -= delegate(this, &Server::onAccept);	
	socket.Close -= delegate(this, &Server::onClose);
	socket.close();

	//timer.stop();
	if (!connections.empty())
		Shutdown.emit(this);

	/// Connections are self removing
	assert(connections.empty());
	assert(socket.base().refCount() == 1);
}


UInt16 Server::port()
{
	//return socket.address().port();
	return address.port();
}	


ServerConnection* Server::createConnection(const net::Socket& sock)
{
	return new ServerConnection(*this, sock);
}


ServerResponder* Server::createResponder(ServerConnection& conn)
{
	/// The initial HTTP request headers have already
	/// been parsed by now, but the request body may 
	/// be incomplete (especially if chunked).
	return factory->createResponder(conn);
}


void Server::addConnection(ServerConnection* conn) 
{		
	traceL("http::Server", this) << "Adding connection: " << conn << endl;
	connections.push_back(conn);
}


void Server::removeConnection(ServerConnection* conn) 
{		
	traceL("http::Server", this) << "Removing connection: " << conn << endl;
	for (ServerConnectionList::iterator it = connections.begin(); it != connections.end(); ++it) {
		if (conn == *it) {
			connections.erase(it);
			return;
		}
	}
	assert(0 && "unknown connection");
}


void Server::onAccept(void* sender, const net::TCPSocket& sock)
{	
	traceL("http::Server", this) << "On server accept" << endl;
	ServerConnection* conn = createConnection(sock);
	assert(conn);
}


void Server::onClose(void* sender) 
{
	traceL("http::Server", this) << "On server socket close" << endl;
}


//
// Server Connection
//


ServerConnection::ServerConnection(Server& server, const net::Socket& socket) : 
	Connection(socket), 
	_server(server), 
	_responder(nil),
	_upgrade(false),
	_requestComplete(false)
{	
	traceL("ServerConnection", this) << "Creating" << endl;

	_socket.replaceAdapter(new ServerAdapter(*this));

	server.Shutdown += delegate(this, &ServerConnection::onServerShutdown);
	server.addConnection(this);
}

	
ServerConnection::~ServerConnection() 
{	
	traceL("ServerConnection", this) << "Destroying" << endl;

	if (_responder) {
		traceL("ServerConnection", this) << "Destroying: Responder: " << _responder << endl;
		delete _responder;
	}
}

	
void ServerConnection::close()
{
	if (!closed()) {
		_server.Shutdown -= delegate(this, &ServerConnection::onServerShutdown);
		_server.removeConnection(this);

		Connection::close(); // close and destroy
	}
}


/*
bool ServerConnection::send()
{
	traceL("ServerConnection", this) << "Respond" << endl;

	/// TODO: Detect end of message and close()

	/// KLUDGE: Temp solution for quick sending small requests only.
	/// Use Connection::write() for nocopy binary stream.
	//string body(_response.body.str());
	//_response.setContentLength(body.length());
	//return write(body.data(), body.length()) > 0;

	/// NOTE: Buffer may be empty.
	/// Zero length call will push the response headers  
	/// through on the initial call so Socket::send()
	//_response.setContentLength(outgoingBuffer().available());
	//return write(outgoingBuffer().data(), outgoingBuffer().available()) > 0;
}
*/

			
Server& ServerConnection::server()
{
	return _server;
}
	

//
// Connection callbacks
//

void ServerConnection::onHeaders() 
{
	traceL("ServerConnection", this) << "On headers" << endl;	
	
	/*
	/// NOTE: To upgrade the connection we need to upgrade the 
	/// ConnectionAdapter, but we can't do it yet since we are
	/// still inside the default adapter's parser callback scope.
	/// Just set the _upgrade flag for now, and we will do the actual 
	/// upgrade when the parser is complete (on the on next iteration).
	_upgrade = _request.hasToken("Connection", "upgrade");
	*/	

	/// Upgrade the connection if required
	if (util::icompare(_request.get("Connection", ""), "upgrade") == 0 && 
		util::icompare(_request.get("Upgrade", ""), "websocket") == 0)
	{			
		traceL("ServerConnection", this) << "Upgrading to WebSocket: " << _request << endl;
		_upgrade = true;

		WebSocketConnectionAdapter* wsAdapter = new WebSocketConnectionAdapter(*this, WebSocket::ServerSide);
				
		/// NOTE: To upgrade the connection we need to replace the 
		/// underlying SocketAdapter instance. Since we are currently 
		/// inside the default ConnectionAdapter's HTTP sarser callback 
		/// scope we just swap the SocketAdapter instance pointers and do
		/// a deferred delete on the old adapter. No more callbacks will be 
		/// received from the old adapter after replaceAdapter is called.
		socket().replaceAdapter(wsAdapter);

		ostringstream oss;
		_request.write(oss);
		Buffer buffer(oss.str().c_str(), oss.str().length());		

		/// Send the handshake request to the WS adapter for handling.
		/// If the request fails the underlying socket will be closed
		/// resulting in the destruction of the current connection.
		wsAdapter->onSocketRecv(buffer, socket().peerAddress());
	}
	
	/// Instantiate the responder when request headers have been parsed
	_responder = _server.createResponder(*this);
	assert(_responder);

	/// Upgraded connections don't receive the onHeaders callback
	if (!_upgrade)
		_responder->onHeaders(_request);
}


void ServerConnection::onPayload(Buffer& buffer)
{
	traceL("ServerConnection", this) << "On payload: " << buffer.available() << endl;	
	
	//assert(_upgrade); // no payload for upgrade requests
	assert(_responder);
	_responder->onPayload(buffer);
}


void ServerConnection::onMessage() 
{
	traceL("ServerConnection", this) << "On complete" << endl;	

	/// The HTTP request is complete.
	/// The request handler can give a response.
	assert(_responder);
	assert(!_requestComplete);
	_requestComplete = true;
	_responder->onRequest(_request, _response);
}


void ServerConnection::onClose() 
{
	traceL("ServerConnection", this) << "On close" << endl;	

	if (_responder)
		_responder->onClose();

	Connection::onClose();
}


void ServerConnection::onServerShutdown(void*)
{
	traceL("ServerConnection", this) << "On server shutdown" << endl;	

	close();
}


http::Message* ServerConnection::incomingHeader() 
{ 
	return static_cast<http::Message*>(&_request);
}


http::Message* ServerConnection::outgoingHeader() 
{ 
	return static_cast<http::Message*>(&_response);
}


} } // namespace scy::http






/*
//
// Parser callbacks
//

void ServerConnection::onParserHeadersEnd() 
{
	/// When headers have been parsed we instantiate the request handler
	_responder = _server.createResponder(*this);
	assert(_responder);
	_responder->onHeaders(*_request);
}


void ServerConnection::onParserChunk(const char* buf, size_t len)
{
	traceL("ServerConnection", this) << "On Parser Chunk" << endl;	
}


void ServerConnection::onParserEnd() 
{
	traceL("ServerConnection", this) << "On Request Complete" << endl;	

	//traceL("ServerConnection", this) << "On Message Complete 1" << endl;	
	//traceL("ServerConnection", this) << "On Message Complete 2" << endl;
	//traceL("ServerConnection", this) << "On Message Complete 2: " << _request.getKeepAlive() << endl;	
	//traceL("ServerConnection", this) << "On Message Complete 2: " << _response.getKeepAlive() << endl;
}
*/



	

	/*
	

	bool res = write(body.c_str(), body.length());

	/// Set Connection: Close unless otherwise stated
	if (!isExplicitKeepAlive(_request) || 
		_response.hasContentLength()) {
		traceL("ServerConnection", this) << "Respond: No keepalive" << endl;	
		_response.setKeepAlive(false);
	}
	*/
	/* 	
	/// KLUDGE: Temp solution for quick sending small requests only.
	/// Use Connection::write() for nocopy binary stream.
	bool res = write(
		_response.body.str().c_str(),
		_response.body.str().length());
	
	/// Close unless keepalive is set
	if (!_response.getKeepAlive()) {
		traceL("Client", this) << "Closing: No keepalive" << endl; 
		close();
	}	
	*/
