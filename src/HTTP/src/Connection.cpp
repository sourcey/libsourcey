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


#include "Sourcey/HTTP/Connection.h"
#include "Sourcey/HTTP/Server.h"
#include "Sourcey/HTTP/Client.h"
#include "Sourcey/Logger.h"

#include <assert.h>


using namespace std;


namespace scy { 
namespace http {


Connection::Connection(const net::Socket& socket) : 
	_socket(socket), 
	_closed(false),
	_shouldSendHeader(true),
	_timeout(30 * 60 * 1000) // 30 secs
{	
	traceL("Connection", this) << "Creating: " << &_socket << endl;

	_socket.Recv += delegate(this, &Connection::onSocketRecv);
	_socket.Error += delegate(this, &Connection::onSocketError);
	_socket.Close += delegate(this, &Connection::onSocketClose);

	// Attach the outgoing stream to the socket
	Outgoing.emitter() += delegate(&_socket, &net::Socket::send);
}

	
Connection::~Connection() 
{	
	traceL("Connection", this) << "Destroying" << endl;

	// Connections must be close()d
	assert(_closed);
}


int Connection::send(const char* buf, size_t len, int flags)
{
	traceL("Connection", this) << "Sending: " << len << endl;

	return _socket.send(buf, len, flags);
}


int Connection::send(const std::string& buf, int flags)
{
	traceL("Connection", this) << "Sending: " << buf.length() << endl;

	return _socket.send(buf.c_str(), buf.length(), flags);
}


int Connection::sendHeader()
{
	traceL("Connection", this) << "Sending HEADSER" << endl;

	if (!_shouldSendHeader)
		return 0;
	assert(outgoingHeader());
	
	ostringstream os;
	outgoingHeader()->write(os);
	string headers(os.str().c_str(), os.str().length());

	_timeout.start();
	_shouldSendHeader = false;
	
	traceL("Connection", this) << "Sending header: " << headers << endl; // remove me
	return _socket.base().send(headers.c_str(), headers.length());
}


void Connection::close()
{
	traceL("Connection", this) << "Close" << endl;	
	assert(!_closed);
	assert(_socket.base().refCount() == 1);
	
	_closed = true;	

	Outgoing.emitter() -= delegate(&_socket, &net::Socket::send);	
	Outgoing.close();
	Incoming.close();

	_socket.Recv -= delegate(this, &Connection::onSocketRecv);
	_socket.Error -= delegate(this, &Connection::onSocketError);
	_socket.Close -= delegate(this, &Connection::onSocketClose);
	_socket.close();

	onClose();
		
	traceL("Connection", this) << "Close: Deleting" << endl;	
	//delete this;
	deleteLater<Connection>(this); // destroy it
}


void Connection::setError(const Error& err) 
{ 
	traceL("Connection", this) << "Set error: " << err.message << endl;	
	_socket.setError(err);
	//_error = err;
}


void Connection::onClose()
{
	traceL("Connection", this) << "On close" << endl;	

	Close.emit(this);
}


void Connection::onSocketRecv(void*, net::SocketPacket& packet)
{		
	_timeout.stop();
			
	if (Incoming.emitter().refCount())
		Incoming.write(RawPacket(packet.array(), packet.size()));

	// Handle payload data
	//onPayload(packet.buffer);
}


void Connection::onSocketError(void* sender, const Error& error) 
{
	traceL("Connection", this) << "On socket error" << endl;

	// Handle the socket error locally
	setError(error);
}


void Connection::onSocketClose(void* sender) 
{
	traceL("Connection", this) << "On socket close" << endl;

	// Close the connection if the socket is closed
	close();
}


Request& Connection::request()
{
	return _request;
}

	
Response& Connection::response()
{
	return _response;
}

	
net::Socket& Connection::socket()
{
	return _socket;
}
	

Buffer& Connection::incomingBuffer()
{
	return static_cast<net::TCPBase&>(_socket.base()).buffer();
}

	
bool Connection::closed() const
{
	return _closed;
}

	
bool Connection::shouldSendHeader() const
{
	return _shouldSendHeader;
}


void Connection::shouldSendHeader(bool flag)
{
	_shouldSendHeader = flag;
}

	
bool Connection::expired() const
{
	return _timeout.expired();
}


// -------------------------------------------------------------------
//
ConnectionAdapter::ConnectionAdapter(Connection& connection, http_parser_type type) : 
	_connection(connection),
	_parser(type)
{	
	traceL("ConnectionAdapter", this) << "Creating: " << &connection << endl;

	// Setup the HTTP Parser
	_parser.setObserver(this);
	if (type == HTTP_REQUEST)
		_parser.setRequest(&connection.request());
	else
		_parser.setResponse(&connection.response());
}


ConnectionAdapter::~ConnectionAdapter()
{
	traceL("ConnectionAdapter", this) << "Destroying: " << &_connection << endl;
}


int ConnectionAdapter::send(const char* data, int len, int flags)
{
	traceL("ConnectionAdapter", this) << "Sending: " << len << endl;
	
	try
	{
		// Send headers on initial send
		if (_connection.shouldSendHeader()) {
			int res = _connection.sendHeader();

			// The initial packet may be empty to 
			// push the headers through
			if (len == 0)
				return res;
		}

		// Other packets should not be empty
		assert(len > 0);

		// Send body / chunk
		//if (len < 300) traceL("ConnectionAdapter", this) << "Sending data: " << string(data, len) << endl;
		return socket->base().send(data, len, flags);
	} 
	catch(Exception& exc) 
	{
		errorL("ConnectionAdapter", this) << "Send error: " << exc.message() << endl;

		// Swallow the exception, the socket error will 
		// cause the connection to close on next iteration.
	}
	
	return -1;
}


void ConnectionAdapter::onSocketRecv(Buffer& buf, const net::Address& peerAddr)
{
	traceL("ConnectionAdapter", this) << "On socket recv: " << buf.available() << endl;	
	
	try {
		// Parse incoming HTTP messages
		_parser.parse(buf.data(), buf.available());
	} 
	catch(Exception& exc) {
		errorL("ConnectionAdapter", this) << "HTTP parser error: " << exc.message() << endl;

		if (socket)
			socket->close();
	}	
}


//
// Parser callbacks
//

void ConnectionAdapter::onParserHeader(const string& name, const string& value) 
{
}


void ConnectionAdapter::onParserHeadersEnd() 
{
	traceL("ConnectionAdapter", this) << "On headers end" << endl;	

	_connection.onHeaders();	

	// Set the position to the end of the headers once
	// they have been handled. Subsequent body chunks will
	// now start at the correct position.
	_connection.incomingBuffer().position(_parser._parser.nread);
}


void ConnectionAdapter::onParserChunk(const char* buf, size_t len)
{
	traceL("ClientConnection", this) << "On parser chunk: " << len << endl;	

	// Dispatch the payload
	net::SocketAdapter::onSocketRecv(_connection.incomingBuffer(), socket->peerAddress());
}


void ConnectionAdapter::onParserError(const ParserError& err)
{
	warnL("ConnectionAdapter", this) << "On parser error: " << err.message << endl;	

	// Close the connection on parser error
	_connection.setError(err.message);
	//_connection.close();
}


void ConnectionAdapter::onParserEnd()
{
	traceL("ConnectionAdapter", this) << "On parser end" << endl;	

	_connection.onMessage();
}

	
Parser& ConnectionAdapter::parser()
{
	return _parser;
}


Connection& ConnectionAdapter::connection()
{
	return _connection;
}


} } // namespace scy::http