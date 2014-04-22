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


#ifndef SCY_HTTP_ServerConnection_H
#define SCY_HTTP_ServerConnection_H


#include "scy/timer.h"
#include "scy/packetqueue.h"
#include "scy/net/tcpsocket.h"
#include "scy/net/socketadapter.h"
#include "scy/http/request.h"
#include "scy/http/response.h"
#include "scy/http/parser.h"
#include "scy/http/url.h"

	
namespace scy { 
namespace http {
	

class ConnectionAdapter;
class Connection: public net::SocketAdapter
{
public:	
    Connection(const net::Socket::Ptr& socket);
    virtual ~Connection();
			
	virtual int send(const char* data, std::size_t len, int flags = 0);
		// Sends raw data to the peer.

	virtual int sendHeader();
		// Sends the outdoing HTTP header.

	virtual void close();
		// Closes the connection and scheduled the object for 
		// deferred deletion.
					
	bool closed() const;
		// Returns true if the connection is closed.

	//bool expired() const;
		// Returns true if the server did not give us
		// a proper response within the allotted time.
	
	virtual void onHeaders() = 0;
	virtual void onPayload(const MutableBuffer&) {};
	virtual void onMessage() = 0;
	virtual void onClose(); // not virtual

	bool shouldSendHeader() const;
	void shouldSendHeader(bool flag);
		// Set true to prevent auto-sending HTTP headers.

	void replaceAdapter(net::SocketAdapter* adapter);

	net::Socket::Ptr& socket();
		// Returns the underlying socket pointer.

	Request& request();	
		// The HTTP request headers.

	Response& response();
		// The HTTP response headers.
	
	PacketStream Outgoing; 
		// The Outgoing stream is responsible for packetizing  
		// raw application data into the agreed upon HTTP   
		// format and sending it to the peer.

	PacketStream Incoming; 
		// The Incoming stream is responsible for depacketizing
		// incoming HTTP chunks emitting the payload to
		// delegate listeners.

    virtual http::Message* incomingHeader() = 0;
    virtual http::Message* outgoingHeader() = 0;

protected:	
	void onSocketConnect();
	void onSocketRecv(const MutableBuffer& buffer, const net::Address& peerAddress);
	void onSocketError(const scy::Error& error);
	void onSocketClose();
		
	virtual void setError(const scy::Error& err);
		// Sets the internal error.

protected:
    net::Socket::Ptr _socket;
	SocketAdapter* _adapter;
    Request _request;
    Response _response;
	//Timeout _timeout;
	scy::Error _error;
	bool _closed;
	bool _shouldSendHeader;
	
	friend class Parser;
	friend class ConnectionAdapter;
	friend struct std::default_delete<Connection>;	
};

	
//
// Connection Adapter
//


class ConnectionAdapter: public ParserObserver, public net::SocketAdapter
	// Default HTTP socket adapter for reading and writing HTTP messages
{
public:
    ConnectionAdapter(Connection& connection, http_parser_type type);	
    virtual ~ConnectionAdapter();	
		
	virtual int send(const char* data, std::size_t len, int flags = 0);
	
	Parser& parser();
	Connection& connection();

protected:

	//
	/// SocketAdapter callbacks

	virtual void onSocketRecv(const MutableBuffer& buffer, const net::Address& peerAddress);
	//virtual void onSocketError(const Error& error);
	//virtual void onSocketClose();
		
	//
	/// HTTPParser callbacks

    virtual void onParserHeader(const std::string& name, const std::string& value);
	virtual void onParserHeadersEnd();
	virtual void onParserChunk(const char* buf, std::size_t len);
    virtual void onParserError(const ParserError& err);
	virtual void onParserEnd();	
	
	Connection& _connection;
    Parser _parser;
};


inline bool isExplicitKeepAlive(http::Message* message) 
{	
	const std::string& connection = message->get(http::Message::CONNECTION, http::Message::EMPTY);
	return !connection.empty() && util::icompare(connection, http::Message::CONNECTION_KEEP_ALIVE) == 0;
}


} } // namespace scy::http


#endif
