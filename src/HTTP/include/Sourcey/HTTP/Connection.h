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


#ifndef SOURCEY_HTTP_ServerConnection_H
#define SOURCEY_HTTP_ServerConnection_H


#include "Sourcey/Timer.h"
#include "Sourcey/PacketQueue.h"
#include "Sourcey/Net/TCPSocket.h"
#include "Sourcey/HTTP/Request.h"
#include "Sourcey/HTTP/Response.h"
#include "Sourcey/HTTP/Parser.h"
#include "Sourcey/HTTP/URL.h"
//#include "Sourcey/HTTP/ConnectionAdapters.h"

	
namespace scy { 
namespace http {
	

class ConnectionAdapter;
class Connection
{
public:	
    Connection(const net::Socket& socket);
			
	virtual int sendData(const char* buf, std::size_t len, int flags = 0);
	virtual int sendData(const std::string& buf, int flags = 0);
		/// Sends a raw data to the peer.

	virtual int sendHeader();
		/// Sends the outdoing HTTP header.

	virtual void close();
		/// Closes the connection and scheduled the object for 
		/// deferred deletion.
		//
		/// The connection pointer should no longer be accessed
		/// once closed.
					
	bool closed() const;
		/// Returns true if the connection is closed.

	bool expired() const;
		/// Returns true if the remote service did not give us
		/// a proper response within the allotted time frame.
	
	virtual void onHeaders() = 0;
	virtual void onPayload(const MutableBuffer&) {};
	virtual void onMessage() = 0;
	virtual void onClose() = 0;

	bool shouldSendHeader() const;
	void shouldSendHeader(bool flag);
		/// Provides a means to prevent default sending of HTTP headers.

	net::Socket& socket();
		/// Return the connection's underlying socket.

	Buffer& incomingBuffer();
		/// The incoming SocketBase buffer. 
		//
		/// The buffer will be overwritten after each socket read,
		/// so if you want to capture raw headers then you can
		/// access the incomingBuffer() on onHeaders(), similarly
		/// if you want raw chunks then access it via onPayload().

	Request& request();	
		/// The HTTP request headers.

	Response& response();
		/// The HTTP response headers.
	
	PacketStream Outgoing; 
		/// The Outgoing stream is responsible for packetizing  
		/// raw application data into the agreed upon HTTP   
		/// format and sending it to the peer.
		//
		/// TODO: send() method should pipe though the 
		/// stream is active (has delegates and procs) 
		/// TODO: Use PacketStream alternative

	PacketStream Incoming; 
		/// The Incoming stream is responsible for depacketizing
		/// incoming HTTP chunks emitting the payload to
		/// delegate listeners.
		//
		/// TODO: send() method should pipe though the 
		/// stream is active (has delegates and procs) 
		/// TODO: Use PacketStream alternative

    virtual http::Message* incomingHeader() = 0;
    virtual http::Message* outgoingHeader() = 0;
	
	NullSignal Close;	
		/// Fires when the connection is closed.

protected:	
    virtual ~Connection();

	void onSocketRecv(void*, net::SocketPacket& packet);
	void onSocketError(void*, const Error& error);
	void onSocketClose(void*);
		
	virtual void setError(const Error& err);

protected:
    Request _request;
    Response _response;
    net::Socket _socket;	
	Timeout _timeout;
	Buffer _outgoing;
	Error _error;

	bool _closed;
	bool _shouldSendHeader;
	
	friend class Parser;
	friend class ConnectionAdapter;
	//friend class DefaultDeleter<Connection>;
	friend struct std::default_delete<Connection>;	
};


	//virtual void onPayload(const MutableBuffer& buffer) = 0;
	
	//virtual bool flush();
		/// Flushes any outgoing HTTP data.
		//
		/// Outgoing HTTP Headers will be sent on the first call
		/// to flush(). If you want to prevent headers from being 
		/// sent set shouldSendHeader to false.
	//Buffer& outgoingBuffer();
		/// The outgoing body payload buffer.
		//
		/// HTTP body/chunked data to send to the peer should be 
		/// written to this buffer.
	
	//ConnectionAdapter* _adapter;
    //net::SocketAdapter* _adapter;
	
	
	//void setHTTPAdapter(net::SocketAdapter* adapter);

	/*
	//PacketStream* _inputStream;
	//PacketStream* _outputStream;
		*/
	//Error _error;
	
	//PacketStream Incoming; 
	//PacketStream Incoming; 
		/// Receiver
		/// The incoming packet stream.
		/// TODO: Replace Payload with this signal and rename to Inbound/Data/Receiver/Outgoing

	//PacketStream* Outgoing; // Outgoing
		/// The outgoing packet stream.

	
// -------------------------------------------------------------------
//	
class ConnectionAdapter: public ParserObserver, public net::SocketAdapter
	/// Default HTTP socket adapter for reading and writing HTTP messages
{
public:
    ConnectionAdapter(Connection& connection, http_parser_type type);	
    virtual ~ConnectionAdapter();	
		
	virtual int send(const char* data, int len, int flags = 0);
	
	Parser& parser();
	Connection& connection();

protected:

	//
	/// Socket emitter callbacks
	//virtual void onSocketConnect() {};
	virtual void onSocketRecv(const MutableBuffer& buf, const net::Address& peerAddr);
	//virtual void onSocketError(const Error& error) override;
	//virtual void onSocketClose();
		
	//
	/// Parser callbacks
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