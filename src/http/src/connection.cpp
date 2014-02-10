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


Connection::Connection(const net::Socket::Ptr& socket) : 
	_socket(socket ? socket : std::make_shared<net::TCPSocket>()), 
	_adapter(nullptr),
	//_timeout(30 * 60 * 1000), // 30 secs
	_closed(false),
	_shouldSendHeader(true)
{	
	TraceLS(this) << "Create: " << _socket << endl;
}

	
Connection::~Connection() 
{	
	TraceLS(this) << "Destroy" << endl;	
	replaceAdapter(nullptr);
	//assert(_closed);
	close(); // don't want pure virtual on onClose.
	           // the shared pointer is being destroyed,
               // no need for close() anyway
	TraceLS(this) << "Destroy: OK" << endl;	
}


int Connection::send(const char* data, std::size_t len, int flags)
{
	TraceLS(this) << "Send: " << len << endl;
	assert(!_closed);
	assert(Outgoing.active());
	Outgoing.write(data, len);
	return len; 
}


#if 0
int Connection::send(const std::string& data, int flags) //
{
	TraceLS(this) << "Send: " << data.length() << endl;
	assert(Outgoing.active());
	Outgoing.write(data.c_str(), data.length());
	
	// Can't send to socket as may not be connected
	//return _socket->send(buf.c_str(), buf.length(), flags);
	return data.length(); // fixme
}
#endif


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

	//_timeout.start();	
	//TraceLS(this) << "Send header: " << head << endl; // remove me

	// Send to base to bypass the ConnectionAdapter
	return _socket->send(head.c_str(), head.length());
}


void Connection::close()
{
	TraceLS(this) << "Close: " << _closed << endl;	
	if (_closed) return;
	_closed = true;	
	
	TraceLS(this) << "Close 1: " << _closed << endl;	
	//Outgoing.emitter.detach(_socket->recvAdapter());	
	//Outgoing.close();
	//Incoming.close();
	
	_socket->close();

	// Note that this must not be pure virtual since
	// close() may be called via the destructor.
	onClose();
}


void Connection::replaceAdapter(net::SocketAdapter* adapter)
{
	TraceLS(this) << "Replace adapter: " << adapter << endl;	

	if (_adapter) {
		Outgoing.emitter.detach(_adapter);
		_socket->removeReceiver(_adapter);
		delete _adapter;
		_adapter = nullptr;
	}
	
	// Assign the new ConnectionAdapter and setup the chain
	// The flow is: Connection <-> ConnectionAdapter <-> Socket
	if (adapter) {
		// Attach ourselves to the given ConnectionAdapter (should already be set)
		//assert(adapter->recvAdapter() == this);
		//assert(adapter->sendAdapter() == _socket.get());
		adapter->addReceiver(this);

		// ConnectionAdapter output goes to the Socket
		adapter->setSender(_socket.get());

		// Attach the ConnectionAdapter to receive Socket callbacks
		// The adapter will process raw packets into HTTP or WebSocket 
		// frames depending on the adapter rype.
		_socket->addReceiver(adapter);
		
		// The Outgoing stream pumps data into the ConnectionAdapter,
		// which in turn proxies to the output Socket
		Outgoing.emitter += delegate(adapter, &net::SocketAdapter::sendPacket);
		//Outgoing.emitter += delegate((net::Socket*)_socket.get(), &net::Socket::sendPacket);
	}


	/*
	// Free current adapter
	net::SocketAdapter* current = _socket->recvAdapter();
	if (current && freeExisting) {
		Outgoing.emitter.detach(current);
		assert(current->recvAdapter() == this);
		assert(_socket->recvAdapter() == current);
		current->addReceiver(nullptr, false); // don't delete ourselves
		current->setSendAdapter(nullptr, false); // don't delete the Socket
		_socket->addReceiver(nullptr, true);  // delete current adapter
	}

	// Assign the new ConnectionAdapter and setup the chain
	// The flow is: Connection <-> ConnectionAdapter <-> Socket
	if (adapter) {
		// Attach ourselves to the given ConnectionAdapter (should already be set)
		assert(adapter->recvAdapter() == this);
		assert(adapter->sendAdapter() == _socket.get());
		adapter->addReceiver(this, false);

		// ConnectionAdapter output goes to the Socket
		adapter->setSendAdapter(_socket.get(), false);

		// Attach the ConnectionAdapter to receive Socket callbacks
		// The adapter will process raw packets into HTTP or WebSocket 
		// frames depending on the adapter rype.
		_socket->addReceiver(adapter, true); // already deleted existing, 
		                                         // just in case
		
		// The Outgoing stream pumps data into the ConnectionAdapter,
		// which in turn proxies to the output Socket
		Outgoing.emitter += sdelegate(static_cast<net::SocketAdapter*>(adapter),
			&net::SocketAdapter::sendPacket);
	}
	*/
}


void Connection::setError(const scy::Error& err) 
{ 
	TraceLS(this) << "Set error: " << err.message << endl;	
	
	//_socket->setError(err);
	_error = err;
	
	// Note: Setting the error does not call close()
}


void Connection::onSocketConnect()
{
	TraceLS(this) << "On socket connect" << endl;
}


void Connection::onSocketRecv(const MutableBuffer& buffer, const net::Address& peerAddress)
{		
	TraceLS(this) << "On socket recv" << endl;
	//_timeout.stop();
			
	if (Incoming.emitter.ndelegates()) {
		//RawPacket p(packet.data(), packet.size());
		//Incoming.write(p);
		Incoming.write(bufferCast<const char*>(buffer), buffer.size());
	}

	// Handle payload data
	onPayload(buffer); //mutableBuffer(bufferCast<const char*>(buf)
}


void Connection::onSocketError(const scy::Error& error) 
{
	TraceLS(this) << "On socket error" << endl;

	// Handle the socket error locally
	setError(error);
}


void Connection::onSocketClose() 
{
	TraceLS(this) << "On socket close" << endl;

	// Close the connection when the socket closes
	close();
}


void Connection::onClose()
{
	TraceLS(this) << "On close" << endl;	

	Close.emit(this);
}


Request& Connection::request()
{
	return _request;
}

	
Response& Connection::response()
{
	return _response;
}

	
net::Socket::Ptr& Connection::socket()
{
	return _socket; //.get();
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



//
// HTTP Client Connection Adapter
//


ConnectionAdapter::ConnectionAdapter(Connection& connection, http_parser_type type) : 
	SocketAdapter(connection.socket().get(), &connection),
	_connection(connection),
	_parser(type)
{	
	TraceLS(this) << "Create: " << &connection << endl;
	_parser.setObserver(this);
	if (type == HTTP_REQUEST)
		_parser.setRequest(&connection.request());
	else
		_parser.setResponse(&connection.response());
}


ConnectionAdapter::~ConnectionAdapter()
{
	TraceLS(this) << "Destroy: " << &_connection << endl;
}


int ConnectionAdapter::send(const char* data, std::size_t len, int flags)
{
	TraceLS(this) << "Send: " << len << endl;
	
	try {
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
		//	TraceLS(this) << "Send data: " << std::string(data, len) << endl;
		//else
		//	TraceLS(this) << "Send long data: " << std::string(data, 300) << endl;
		//return this->socket->send(data, len, flags);
		return SocketAdapter::send(data, len, flags);
	} 
	catch (std::exception& exc) {
		ErrorLS(this) << "Send error: " << exc.what() << endl;

		// Swallow the exception, the socket error will 
		// cause the connection to close on next iteration.
	}
	
	return -1;
}


void ConnectionAdapter::onSocketRecv(const MutableBuffer& buf, const net::Address& /* peerAddr */)
{
	TraceLS(this) << "On socket recv: " << buf.size() << endl;	
	
	if (_parser.complete()) {
		// Buggy HTTP servers might send late data or multiple responses,
		// in which case the parser state might already be HPE_OK.
		// In this case we discard the late message and log the error here,
		// rather than complicate the app with this error handling logic.
		// This issue noted using Webrick with Ruby 1.9.
		WarnL << "Discarding late response: " << 
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
	TraceLS(this) << "On headers end" << endl;	

	_connection.onHeaders();	

	// Set the position to the end of the headers once
	// they have been handled. Subsequent body chunks will
	// now start at the correct position.
	//_connection.incomingBuffer().position(_parser._parser.nread); // should be redundant
}


void ConnectionAdapter::onParserChunk(const char* buf, std::size_t len)
{
	TraceLS(this) << "On parser chunk: " << len << endl;	

	// Dispatch the payload
	net::SocketAdapter::onSocketRecv(mutableBuffer(const_cast<char*>(buf), len), 
		_connection.socket()->peerAddress());
}


void ConnectionAdapter::onParserError(const ParserError& err)
{
	WarnL << "On parser error: " << err.message << endl;	

	// HACK: Handle those peski flash policy requests here
	auto base = dynamic_cast<net::TCPSocket*>(_connection.socket().get());
	if (base && std::string(base->buffer().data(), 22) == "<policy-file-request/>") {
		
		// Send an all access policy file by default
		// TODO: User specified flash policy
		std::string policy;

		// Add the following headers for HTTP policy response
		// policy += "HTTP/1.1 200 OK\r\nContent-Type: text/x-cross-domain-policy\r\nX-Permitted-Cross-Domain-Policies: all\r\n\r\n";
		policy += "<?xml version=\"1.0\"?><cross-domain-policy><allow-access-from domain=\"*\" to-ports=\"*\" /></cross-domain-policy>";

		TraceLS(this) << "Send flash policy: " << policy << endl;
		base->send(policy.c_str(), policy.length() + 1);
	}

	// Set error and close the connection on parser error
	_connection.setError(err.message);
	_connection.close(); // do we want to force this?
}


void ConnectionAdapter::onParserEnd()
{
	TraceLS(this) << "On parser end" << endl;	

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
		ErrorLS(this) << "HTTP parser error: " << exc.what() << endl;

		if (socket)
			socket->close();
	}	
	*/
