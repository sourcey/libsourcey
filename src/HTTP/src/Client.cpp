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



using namespace std;


#include "Sourcey/HTTP/Client.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Util.h"


namespace scy { 
namespace http {

	
// -------------------------------------------------------------------
// Client Connection
//
ClientConnection::ClientConnection(Client& client, const net::Address& address) : 
	Connection(net::TCPSocket(new net::TCPBase), HTTP_RESPONSE), _client(client), _address(address)
{	
	traceL("ClientConnection", this) << "Creating" << endl;
	client.Shutdown += delegate(this, &ClientConnection::onShutdown);
	client.addConnection(this);
}

	
ClientConnection::~ClientConnection() 
{	
	traceL("ClientConnection", this) << "Destroying" << endl;
}

	
void ClientConnection::close()
{
	_client.Shutdown -= delegate(this, &ClientConnection::onShutdown);
	_client.removeConnection(this);
	Connection::close();
}


void ClientConnection::send(bool whiny)
{
	traceL("ClientConnection", this) << "Send Request" << endl;

	// Connect to server
	_socket.connect(_address);
}

			
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

void ClientConnection::onSocketConnect() 
{
	traceL("ClientConnection", this) << "Connected" << endl;
	
	// KLUDGE: Temp solution for quick sending small requests only.
	// Use Connection::sendBytes() for nocopy binary stream.
	sendBytes(
		_request->body.str().data(),
		_request->body.str().length());
}


//
// Client callbacks
//

void ClientConnection::onShutdown(void*)
{
	close();
}
	

//
// Parser callbacks
//

void ClientConnection::onParserHeadersDone() 
{
	ResponseHeaders.emit(this, *_response);
}


void ClientConnection::onParserChunk(const char* buf, size_t len)
{
	traceL("ClientConnection", this) << "On Parser Chunk" << endl;	

	// Just keep appending body chunks to the buffer.
	// The handler can manage the buffer as required,
	// including clearing the buffer on each body chunk.
	//_buffer.write(buf, len);
	
	//ResponseBody.emit(this, _buffer);
	ResponseBody.emit(this, buffer());
}


void ClientConnection::onParserDone() 
{
	traceL("ClientConnection", this) << "On Parser Done" << endl;	

	// The entire HTTP message is complete.
	// Emit the Complete signal.
	ResponseComplete.emit(this, *_response);
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