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
ClientConnection::ClientConnection(Client& client, const net::Address& address, const net::Socket& socket) : 
	Connection(socket),
	_client(client), 
	_address(address)
{	
	traceL("ClientConnection", this) << "Creating" << endl;

	replaceAdapter(new ClientAdapter(*this));
	client.Shutdown += delegate(this, &ClientConnection::onClientShutdown);
	client.addConnection(this);
}
	
ClientConnection::~ClientConnection() 
{	
	traceL("ClientConnection", this) << "Destroying" << endl;
}

	
void ClientConnection::close()
{
	if (!closed()) {
		_client.Shutdown -= delegate(this, &ClientConnection::onClientShutdown);
		_client.removeConnection(this);

		Connection::close();
	}
}


void ClientConnection::send()
{
	traceL("ClientConnection", this) << "Send Request" << endl;

	// Connect to server, the request will be sent on connect
	_socket.Connect += delegate(this, &ClientConnection::onSocketConnect);
	_socket.connect(_address);
}


bool ClientConnection::flush()
{
	traceL("ClientConnection", this) << "Flushing" << endl;

	// TODO: Optimize ... use sendRaw for large packers
	string body(_request->body.str());
	if (body.length() == 0)
		return false;

	return sendRaw(body.data(), body.length()) > 0;
}


/*
//return sendMessage();
bool Connection::sendMessage()
{	
}
*/

			
Client& ClientConnection::client()
{
	return _client;
}


Poco::Net::HTTPMessage* ClientConnection::incomingHeaders() 
{ 
	return static_cast<Poco::Net::HTTPMessage*>(_response);
}


Poco::Net::HTTPMessage* ClientConnection::outgoingHeaders() 
{ 
	return static_cast<Poco::Net::HTTPMessage*>(_request);
}


//
// Socket callbacks
//

void ClientConnection::onSocketConnect(void*) 
{
	traceL("ClientConnection", this) << "Connected" << endl;
	assert(_adapter);
	_socket.Connect -= delegate(this, &ClientConnection::onSocketConnect);



	// Send the request
	// TODO: Can't do this for WS
	//sendMessage();
	flush();

	//
	// KLUDGE: Temp solution for quick sending small requests only.
	// Use Connection::sendRaw() for nocopy binary stream.
	//sendRaw(
	//	_request->body.str().data(),
	//	_request->body.str().length());
}
	

//
// Connection callbacks
//

void ClientConnection::onHeaders() 
{
	traceL("ClientConnection", this) << "On headers" << endl;	

	Headers.emit(this, *_response);
}


void ClientConnection::onPayload(Buffer& buffer)
{
	traceL("ClientConnection", this) << "On payload" << endl;	

	Payload.emit(this, buffer);
}


void ClientConnection::onComplete() 
{
	traceL("ClientConnection", this) << "On complete" << endl;	

	Complete.emit(this, *_response);
}


void ClientConnection::onClose() 
{
	traceL("ClientConnection", this) << "On close" << endl;	

	Connection::onClose();
}


//
// Client callbacks
//

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
ClientConnection::ClientConnection(Client& client, const net::Address& address) : 
	Connection(net::TCPSocket(), new ClientAdapter(this)), 
	_client(client), 
	_address(address)
{	
	traceL("ClientConnection", this) << "Creating" << endl;

	client.Shutdown += delegate(this, &ClientConnection::onClientShutdown);
	client.addConnection(this);
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
void Client::onConnectionComplete(void* sender, const Response&) 
{
	traceL("Client", this) << "#### On connection close: " << sender << endl;
	//removeConnection(static_cast<ClientConnection*>(sender));
}
*/