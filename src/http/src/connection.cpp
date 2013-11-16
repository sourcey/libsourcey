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


#include "scy/http/connection.h"
#include "scy/http/server.h"
#include "scy/http/client.h"
#include "scy/logger.h"
#include "scy/memory.h"

#include <assert.h>


using std::endl;


namespace scy { 
namespace http {


Connection::Connection(const net::Socket& socket) : 
	_socket(socket), 
	_timeout(30 * 60 * 1000), // 30 secs
	_closed(false),
	_shouldSendHeader(true)
{	
	traceL("Connection", this) << "Create: " << &_socket << endl;

	_socket.Recv += delegate(this, &Connection::onSocketRecv);
	_socket.Error += delegate(this, &Connection::onSocketError);
	_socket.Close += delegate(this, &Connection::onSocketClose);

	// Use an event loop Idler as the PacketStream async source
	//auto idler = std::make_shared<Idler>(*socket.base().loop());
	//, scy::deleter::Dispose<Idler>()
	//Incoming.setRunner(std::make_shared<Idler>(socket.base().loop()));
	//Outgoing.setRunner(std::make_shared<Idler>(socket.base().loop()));

	// Attach the outgoing stream to the socket
	Outgoing.emitter += delegate(&_socket, &net::Socket::send);
}

	
Connection::~Connection() 
{	
	traceL("Connection", this) << "Destroy" << endl;

	// Connections must be close()d
	assert(_closed);
}


void Connection::sendData(const char* buf, std::size_t len) //, int flags
{
	traceL("Connection", this) << "Send: " << len << endl;
	assert(Outgoing.active());
	Outgoing.write(buf, len);

	// Can't send to socket as may not be connected
	//return _socket.send(buf, len, flags);
	//return len; // fixme
}


void Connection::sendData(const std::string& buf) //, int flags
{
	traceL("Connection", this) << "Send: " << buf.length() << endl;
	assert(Outgoing.active());
	Outgoing.write(buf.c_str(), buf.length());
	
	// Can't send to socket as may not be connected
	//return _socket.send(buf.c_str(), buf.length(), flags);
	//return buf.length(); // fixme
}


int Connection::sendHeader()
{
	if (!_shouldSendHeader)
		return 0;
	_shouldSendHeader = false;

	assert(outgoingHeader());
	//assert(outgoingHeader()->has("Host"));
	
	std::ostringstream os;
	outgoingHeader()->write(os);
	std::string head(os.str().c_str(), os.str().length());

	_timeout.start();	
	traceL("Connection", this) << "Send header: " << head << endl; // remove me

	// Send to base to bypass the ConnectionAdapter
	return _socket.base().send(head.c_str(), head.length());
}


void Connection::close()
{
	traceL("Connection", this) << "Close" << endl;	
	assert(!_closed);
	assert(_socket.base().refCount() == 1);
	
	_closed = true;	

	Outgoing.emitter -= delegate(&_socket, &net::Socket::send);	
	Outgoing.close();
	Incoming.close();

	_socket.Recv -= delegate(this, &Connection::onSocketRecv);
	_socket.Error -= delegate(this, &Connection::onSocketError);
	_socket.Close -= delegate(this, &Connection::onSocketClose);
	_socket.close();

	onClose();
		
	//delete this;
	deleteLater<Connection>(this); // destroy it
}


void Connection::setError(const Error& err) 
{ 
	traceL("Connection", this) << "Set error: " << err.message << endl;	
	
	//_socket.setError(err);
	_error = err;
	
	// Note: Setting the error does not call close()
}


void Connection::onClose()
{
	traceL("Connection", this) << "On close" << endl;	

	Close.emit(this);
}


void Connection::onSocketRecv(void*, net::SocketPacket& packet)
{		
	_timeout.stop();
			
	if (Incoming.emitter.refCount()) {
		//RawPacket p(packet.data(), packet.size());
		//Incoming.write(p);
		Incoming.write(packet.data(), packet.size());
	}

	// Handle payload data
	onPayload(mutableBuffer(packet.data(), packet.size()));
}


void Connection::onSocketError(void*, const Error& error) 
{
	traceL("Connection", this) << "On socket error" << endl;

	// Handle the socket error locally
	setError(error);
}


void Connection::onSocketClose(void*) 
{
	traceL("Connection", this) << "On socket close" << endl;

	// Close the connection when the socket closes
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
	

/*
Buffer& Connection::incomingBuffer()
{
	return static_cast<net::TCPBase&>(_socket.base()).buffer();
}
*/

	
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


//
// HTTP Client Connection Adapter
//


ConnectionAdapter::ConnectionAdapter(Connection& connection, http_parser_type type) : 
	_connection(connection),
	_parser(type)
{	
	traceL("ConnectionAdapter", this) << "Create: " << &connection << endl;
	_parser.setObserver(this);
	if (type == HTTP_REQUEST)
		_parser.setRequest(&connection.request());
	else
		_parser.setResponse(&connection.response());
}


ConnectionAdapter::~ConnectionAdapter()
{
	traceL("ConnectionAdapter", this) << "Destroy: " << &_connection << endl;
}


int ConnectionAdapter::send(const char* data, int len, int flags)
{
	traceL("ConnectionAdapter", this) << "Send: " << len << endl;
	
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
		//if (len < 300)
		//	traceL("ConnectionAdapter", this) << "Send data: " << std::string(data, len) << endl;
		//else
		//	traceL("ConnectionAdapter", this) << "Send long data: " << std::string(data, 300) << endl;
		return socket->base().send(data, len, flags);
	} 
	catch (std::exception& exc) 
	{
		errorL("ConnectionAdapter", this) << "Send error: " << exc.what() << endl;

		// Swallow the exception, the socket error will 
		// cause the connection to close on next iteration.
	}
	
	return -1;
}


void ConnectionAdapter::onSocketRecv(const MutableBuffer& buf, const net::Address& /* peerAddr */)
{
	traceL("ConnectionAdapter", this) << "On socket recv: " << buf.size() << endl;	
	
	if (_parser.complete()) {
		// Buggy HTTP servers might send late data or multiple responses,
		// in which case the parser state might already be HPE_OK.
		// In this case we discard the late message and log the error here,
		// rather than complicate the app with this error handling logic.
		// This issue noted using Webrick with Ruby 1.9.
		warnL("ConnectionAdapter", this) << "Discarding late response: " << 
			std::string(bufferCast<const char*>(buf), 
				std::min<std::size_t>(150, buf.size())) << endl;
		return;
	}

	// Parse incoming HTTP messages
	_parser.parse(bufferCast<const char*>(buf), buf.size());
}


//
// Parser callbacks
//

void ConnectionAdapter::onParserHeader(const std::string& /* name */, const std::string& /* value */) 
{
}


void ConnectionAdapter::onParserHeadersEnd() 
{
	traceL("ConnectionAdapter", this) << "On headers end" << endl;	

	_connection.onHeaders();	

	// Set the position to the end of the headers once
	// they have been handled. Subsequent body chunks will
	// now start at the correct position.
	//_connection.incomingBuffer().position(_parser._parser.nread); // should be redundant
}


void ConnectionAdapter::onParserChunk(const char* buf, std::size_t len)
{
	traceL("ClientConnection", this) << "On parser chunk: " << len << endl;	

	// Dispatch the payload
	net::SocketAdapter::onSocketRecv(mutableBuffer(const_cast<char*>(buf), len), socket->peerAddress());
}


void ConnectionAdapter::onParserError(const ParserError& err)
{
	warnL("ConnectionAdapter", this) << "On parser error: " << err.message << endl;	

	// HACK: Handle those peski flash policy requests here
	auto base = dynamic_cast<net::TCPBase*>(&_connection.socket().base());
	if (base && std::string(base->buffer().data(), 22) == "<policy-file-request/>") {
		
		// Send an all access policy file by default
		// TODO: User specified flash policy
		std::string policy;

		// Add the following headers for HTTP policy response
		// policy += "HTTP/1.1 200 OK\r\nContent-Type: text/x-cross-domain-policy\r\nX-Permitted-Cross-Domain-Policies: all\r\n\r\n";
		policy += "<?xml version=\"1.0\"?><cross-domain-policy><allow-access-from domain=\"*\" to-ports=\"*\" /></cross-domain-policy>";

		traceL("ConnectionAdapter", this) << "Send flash policy: " << policy << endl;
		base->send(policy.c_str(), policy.length() + 1);
	}

	// Set error and close the connection on parser error
	_connection.setError(err.message);
	_connection.close(); // do we want to force this?
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


	
	/*
	try {
	} 
	catch (std::exception& exc) {
		errorL("ConnectionAdapter", this) << "HTTP parser error: " << exc.what() << endl;

		if (socket)
			socket->close();
	}	
	*/
