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
	_recvStream(nullptr)
{	
	traceL("ClientConnection", this) << "Creating: " << url << endl;
		
	_request.setURI(url.str());

	_socket.replaceAdapter(new ClientAdapter(*this));

	if (_client) {
		_client->Shutdown += delegate(this, &ClientConnection::onClientShutdown);
		_client->addConnection(this);
	}
}


ClientConnection::ClientConnection(const URL& url, const net::Socket& socket) :
	Connection(socket),
	_client(nullptr), 
	_url(url), 
	_recvStream(nullptr)
{	
	traceL("ClientConnection", this) << "Creating: " << url << endl;
	
	_request.setURI(url.str());

	_socket.replaceAdapter(new ClientAdapter(*this));
}


ClientConnection::~ClientConnection() 
{	
	traceL("ClientConnection", this) << "Destroying" << endl;

	//if (!_recvStream)
	//	_recvStream->destroy();
	//if (!_recvStreamStream)
	//	_recvStreamStream->destroy();
}

	
void ClientConnection::close()
{
	if (!closed()) {
		if (_client) {
			_client->Shutdown -= delegate(this, &ClientConnection::onClientShutdown);
			_client->removeConnection(this);
		}
		if (_recvStream) {
			delete _recvStream;
			_recvStream = nullptr;
		}

		Connection::close();
	}
}


void ClientConnection::send()
{
	traceL("ClientConnection", this) << "Sending request" << endl;	
	_socket.Connect += delegate(this, &ClientConnection::onSocketConnect);
	_socket.connect(_url.host(), _url.port());
}


void ClientConnection::send(http::Request& req)
{
	_request = req;
	send();
}


void ClientConnection::setRecvStream(std::ostream* os)
{
	// TODO: assert not running
	if (_recvStream)
		delete _recvStream;

	_recvStream = os;
}

			
http::Client* ClientConnection::client()
{
	return _client;
}


http::Message* ClientConnection::incomingHeaders() 
{ 
	return static_cast<http::Message*>(&_response);
}


http::Message* ClientConnection::outgoingHeaders() 
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

	// Start the outgoing send stream if there  
	// are any adapters attached.
	if (Outgoing.numAdapters())
		Outgoing.start();
	
	// Send the outgoing HTTP request if they 
	// weren't pushed through by the stream.
	sendHeaders();
	
	// Emit the connect signal so raw connections ie. 
	// websockets can kick off the data flow
	Connect.emit(this);
}
	

//
// Connection callbacks
//

void ClientConnection::onHeaders() 
{
	traceL("ClientConnection", this) << "On headers" << endl;	

	Headers.emit(this, _response);
}


void ClientConnection::onPayload(Buffer& buffer)
{
	traceL("ClientConnection", this) << "On payload" << endl;	

	_incomingProgress.total = response().getContentLength();
	_incomingProgress.current += buffer.available();
	assert(_incomingProgress.current <= _incomingProgress.total);
	IncomingProgress.emit(this, _incomingProgress);

	if (_recvStream)
		_recvStream->write(buffer.data(), buffer.available());

	//Payload.emit(this, buffer);
}


void ClientConnection::onMessage() 
{
	traceL("ClientConnection", this) << "On complete" << endl;

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
Client::Client()
{
	traceL("Client", this) << "Creating" << endl;

	//timer.Timeout += delegate(this, &Client::onTimer);
	//timer.start(5000, 5000);
}


Client::~Client()
{
	traceL("Client", this) << "Destroying" << endl;
	shutdown();
}


void Client::shutdown() 
{
	traceL("Client", this) << "Shutdown" << endl;

	timer.stop();
	if (!connections.empty())
		Shutdown.emit(this);

	// Connections must remove themselves
	assert(connections.empty());
}


void Client::addConnection(ClientConnection* conn) 
{		
	traceL("Client", this) << "Adding Connection: " << conn << endl;
	connections.push_back(conn);
}


void Client::removeConnection(ClientConnection* conn) 
{		
	traceL("Client", this) << "Removing Connection: " << conn << endl;
	for (ClientConnectionList::iterator it = connections.begin(); it != connections.end(); ++it) {
		if (conn == *it) {
			connections.erase(it);
			traceL("Client", this) << "Removed Connection: " << conn << endl;
			return;
		}
	}
	assert(0 && "unknown connection");
}


void Client::onTimer(void*)
{
	// TODO: Print server stats
	// TODO: Proper handling for timed out requests.
	traceL("Client", this) << "On timer" << endl;

	ClientConnectionList conns = ClientConnectionList(connections);
	for (ClientConnectionList::iterator it = conns.begin(); it != conns.end();) {
		if ((*it)->expired()) {
			traceL("Server", this) << "Closing Expired Connection: " << *it << endl;
			
			// TODO: Send a HTTP Error 408 or some such
			(*it)->close();
			++it;
		}
		else
			++it;
	}
}


} } // namespace scy::http




/*
	//if (_address.valid()) {
		// Connect to server, the request will be sent on connect
		//_sendOnResolved = false;
		//return;
	//}
	//else if (_dns.resolving()) {
	//	_sendOnResolved = true;
	//}
	//else {
		// Must be a DNS failure
		// The connection should be in error state.
		//assert(_dns.failed());
		//assert(_error.any());
		//assert(0);
	//}


int ClientConnection::write(const char* buf, size_t len, int flags)
{
	if (!_recvStreamStream)
		_recvStreamStream = new PacketStream;
}
*/
	/*
ClientConnection::ClientConnection(Client& client, const net::Address& address) : 
	Connection(net::TCPSocket(), new ClientAdapter(this)), 
	_client(client), 
	_address(address)
{	
	traceL("ClientConnection", this) << "Creating" << endl;

	client->Shutdown += delegate(this, &ClientConnection::onClientShutdown);
	client->addConnection(this);
}
*/



	/*
	traceL("Client", this) << "Stopping: " << connections.size() << endl;
	for (int i = 0; i < connections.size(); i++) {
		traceL("Client", this) << "Stopping: Closing: " << connections[i] << endl;
		connections[i]->close();
		traceL("Client", this) << "Stopping: Deleting: " << connections[i] << endl;
		//delete connections[i];
	}
	connections.clear();
	*/


		/*
		if ((*it)->closed()) {
			traceL("Server", this) << "Deleting Closed Connection: " << (*it) << endl;
			delete *it;
			it = connections.erase(it);
		}
		*/

/*
*/



/*
void Client::onConnectionMessage(void* sender, const Response&) 
{
	traceL("Client", this) << "#### On connection close: " << sender << endl;
	//removeConnection(static_cast<ClientConnection*>(sender));
}
*/