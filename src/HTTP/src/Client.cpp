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
	Connection(net::TCPSocket(new net::TCPBase), HTTP_REQUEST), _client(client), _address(address)
{	
	traceL("ClientConnection", this) << "Creating" << endl;
}

	
ClientConnection::~ClientConnection() 
{	
	traceL("ClientConnection", this) << "Destroying" << endl;
}


Poco::Net::HTTPMessage* ClientConnection::headers() 
{ 
	return static_cast<Poco::Net::HTTPMessage*>(_request); 
}


void ClientConnection::send(bool whiny)
{
	traceL("ClientConnection", this) << "Respond" << endl;
	
	_socket.connect(_address);
}

			
Client& ClientConnection::client()
{
	return _client;
}


//
// Socket callbacks
//

void ClientConnection::onConnect() 
{
	traceL("ClientConnection", this) << "Connected" << endl;

	// KLUDGE: Temp solution for quick sending small requests only.
	// Use Connection::sendBytes() for nocopy binary stream.
	sendBytes(
		_request->body.str().data(),
		_request->body.str().length());
	
	//close();
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
	// Just keep response body / chunks to the handler.
	// The handler can manage the buffer as required.
	_buffer.write(buf, len);

	ResponseChunk.emit(this, _buffer);
}


void ClientConnection::onParserDone() 
{
	traceL("ClientConnection", this) << "On Message Complete" << endl;	

	// The entireHTTP message is complete.
	// Send the complete signal.
	Complete.emit(this, *_response);
}


// ---------------------------------------------------------------------
//

Client::Client()
{
	traceL("Client", this) << "Creating" << endl;
}


Client::~Client()
{
	traceL("Client", this) << "Destroying" << endl;
	for (int i = 0; i < connections.size(); i++)
		delete connections[i];
}


void Client::onTimer(void*)
{
	// TODO: Print server stats
	// TODO: Proper handling for timed out requests.
	for (ClientConnectionList::iterator it = connections.begin(); it != connections.end();) {
		if ((*it)->expired()) {
			traceL("Server", this) << "Closing Expired Connection: " << (*it) << endl;
			// TODO: Send a HTTP Error 408 or some such
			(*it)->close();
			++it;
		}
		if ((*it)->closed()) {
			traceL("Server", this) << "Deleting Closed Connection: " << (*it) << endl;
			delete *it;
			it = connections.erase(it);
		}
		else
			++it;
	}
}


} } // namespace scy::http





/*
void Client::addConnection(ClientConnection* conn) 
{		
	traceL("Client", this) << "#### Adding: " << conn << endl;
	connections.push_back(conn);
}


void Client::removeConnection(ClientConnection* conn) 
{		
	traceL("Client", this) << "#### Removing: " << conn << endl;
	for (ClientConnectionList::iterator it = connections.begin(); it != connections.end(); ++it) {
		if (conn == *it) {

			// All we need to do is erase the socket in order to 
			// deincrement the ref counter and destroy the socket.
			//assert(socket->base().refCount() == 1);
			//delete *it;
			connections.erase(it);
			traceL("Client", this) << "#### Removedg: " << conn << endl;
			return;
		}
	}
	assert(0 && "unknown connection");
}
*/



/*
void Client::onConnectionComplete(void* sender, const Response&) 
{
	traceL("Client", this) << "#### On connection close: " << sender << endl;
	//removeConnection(static_cast<ClientConnection*>(sender));
}
*/