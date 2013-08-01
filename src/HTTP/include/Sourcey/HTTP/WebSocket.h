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


#ifndef SOURCEY_NET_WebSocket_H
#define SOURCEY_NET_WebSocket_H


#include "Sourcey/Base.h"
#include "Sourcey/Buffer.h"
#include "Sourcey/Net/Socket.h"
#include "Sourcey/Net/TCPSocket.h"
#include "Sourcey/HTTP/Request.h"
#include "Sourcey/HTTP/Response.h"
#include "Sourcey/HTTP/Parser.h"
#include "Poco/Random.h"


namespace scy {
namespace http {


class WebSocketAdapter;
class WebSocket: public net::Socket
{
public:	
	typedef WebSocketAdapter Base;
	typedef std::vector<WebSocket> List;
	
	enum Mode
	{
		WS_SERVER, /// Server-side WebSocket.
		WS_CLIENT  /// Client-side WebSocket.
	};
	
	enum FrameFlags
		// Frame header flags.
	{
		FRAME_FLAG_FIN  = 0x80, /// FIN bit: final fragment of a multi-fragment message.
		FRAME_FLAG_RSV1 = 0x40, /// Reserved for future use. Must be zero.
		FRAME_FLAG_RSV2 = 0x20, /// Reserved for future use. Must be zero.
		FRAME_FLAG_RSV3 = 0x10, /// Reserved for future use. Must be zero.
	};

	enum FrameOpcodes
		// Frame header opcodes.
	{
		FRAME_OP_CONT    = 0x00, /// Continuation frame.
		FRAME_OP_TEXT    = 0x01, /// Text frame.
		FRAME_OP_BINARY  = 0x02, /// Binary frame.
		FRAME_OP_CLOSE   = 0x08, /// Close connection.
		FRAME_OP_PING    = 0x09, /// Ping frame.
		FRAME_OP_PONG    = 0x0a, /// Pong frame.
		FRAME_OP_BITMASK = 0x0f  /// Bit mask for opcodes. 
	};
	
	enum SendFlags
		// Combined header flags and opcodes for use with writeFrame().
	{
		FRAME_TEXT   = FRAME_FLAG_FIN | FRAME_OP_TEXT,
			// Use this for sending a single text (UTF-8) payload frame.
		FRAME_BINARY = FRAME_FLAG_FIN | FRAME_OP_BINARY
			// Use this for sending a single binary payload frame.
	};
	
	enum StatusCodes
		// StatusCodes for CLOSE frames sent with shutdown().
	{
		WS_NORMAL_CLOSE            = 1000,
		WS_ENDPOINT_GOING_AWAY     = 1001,
		WS_PROTOCOL_ERROR          = 1002,
		WS_PAYLOAD_NOT_ACCEPTABLE  = 1003,
		WS_RESERVED                = 1004,
		WS_RESERVED_NO_STATUS_CODE = 1005,
		WS_RESERVED_ABNORMAL_CLOSE = 1006,
		WS_MALFORMED_PAYLOAD       = 1007,
		WS_POLICY_VIOLATION        = 1008,
		WS_PAYLOAD_TOO_BIG         = 1009,
		WS_EXTENSION_REQUIRED      = 1010,
		WS_UNEXPECTED_CONDITION    = 1011,
		WS_RESERVED_TLS_FAILURE    = 1015
	};
	
	enum ErrorCodes
		// These error codes can be obtained from a WebSocketException
		// to determine the exact cause of the error.
	{
		WS_ERR_NO_HANDSHAKE                   = 1,
			// No Connection: Upgrade or Upgrade: websocket header in handshake request.
		WS_ERR_HANDSHAKE_NO_VERSION           = 2,
			// No Sec-WebSocket-Version header in handshake request.
		WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION  = 3,
			// Unsupported WebSocket version requested by client.
		WS_ERR_HANDSHAKE_NO_KEY               = 4,
			// No Sec-WebSocket-Key header in handshake request.
		WS_ERR_HANDSHAKE_ACCEPT               = 5,
			// No Sec-WebSocket-Accept header or wrong value.
		WS_ERR_UNAUTHORIZED                   = 6,
			// The server rejected the username or password for authentication.
		WS_ERR_PAYLOAD_TOO_BIG                = 10,
			// Payload too big for supplied buffer.
		WS_ERR_INCOMPLETE_FRAME               = 11
			// Incomplete frame received.
	};
	
	static const std::string WEBSOCKET_GUID;
	static const std::string WEBSOCKET_VERSION;
		// The WebSocket protocol version supported (13).
	
	WebSocket();
		// Creates an unconnected WebSocket.

	WebSocket(net::SocketBase* base, bool shared = false);
		// Creates the Socket and attaches the given SocketBase.
		//
		// The SocketBase must be a WebSocketAdapter, otherwise an
		// exception will be thrown.

	WebSocket(const net::Socket& socket);
		// Creates the WebSocket with the SocketBase
		// from another socket. The SocketBase must be
		// a WebSocketAdapter, otherwise an exception will be thrown.

	WebSocketAdapter& adapter() const;
		// Returns the WebSocketAdapter for this socket.
	
	virtual bool shutdown(UInt16 statusCode, const std::string& statusMessage);

	http::Request& request();
	http::Response& response();
	
protected:
	http::Request _request;
	http::Response _response;
};
	
	
// ---------------------------------------------------------------------
//
class WebSocketFramer
	/// This class implements a WebSocket parser according
	/// to the WebSocket protocol described in RFC 6455.
{
public:
	WebSocketFramer(WebSocket::Mode mode);
		// Creates a SocketBase using the given SocketBase.

	virtual ~WebSocketFramer();

	virtual int writeFrame(const char* buffer, int length, int flags, Buffer& frame);
		// Writes a WebSocket protocol frame from the given data.
		
	virtual int readFrame(Buffer& buffer);
		// Receives a WebSocket protocol frame.
		//
		// The buffer's read position and size will be set to match
		// the actual beginning and end of the contained payload.
		// This enables us to work with the socket buffer directly
		// without copying any data.
	
	//
	/// Server side

	void acceptRequest(http::Request& request, http::Response& response);
	
	//
	/// Client side

	void sendHandshakeRequest(); 
		// Sends the initial WS handshake HTTP request.
		
	void createHandshakeRequest(http::Request& request); 
		// Appends the WS hanshake HTTP request hearers.
	
	bool checkHandshakeResponse(http::Response& response);
		// Checks the veracity the HTTP handshake response.
		// Returns true on success, false if the request should 
		// be resent (in case of authentication), or throws on error.

	void completeHandshake(http::Response& response);
		// Verifies the handshake response or thrown and exception.

	bool handshakeComplete() const;
		// Return true when the handshake has completed successfully.

protected:
	int frameFlags() const;
		// Returns the frame flags of the most recently received frame.
		// Set by readFrame()
		
	bool mustMaskPayload() const;
		// Returns true if the payload must be masked.	
		// Used by writeFrame()
		
	WebSocket::Mode mode() const;
	
	enum
	{
		FRAME_FLAG_MASK   = 0x80,
		MAX_HEADER_LENGTH = 14
	};

private:
	WebSocket::Mode _mode;
	int _frameFlags;
	bool _mustMaskPayload;
	int _headerState;
	Poco::Random _rnd;
	std::string _key; // client handshake key

	friend class WebSocketAdapter;
};


// ---------------------------------------------------------------------
//
class WebSocketAdapter: public net::SocketAdapter
{
public:	
	WebSocketAdapter(net::Socket* socket, WebSocket::Mode mode, http::Request& request, http::Response& response); 
	WebSocketAdapter(WebSocket::Mode mode, http::Request& request, http::Response& response);
	
	virtual int send(const char* data, int len, int flags = WebSocket::FRAME_TEXT);
	virtual int send(const char* data, int len, const net::Address& peerAddr, int flags = WebSocket::FRAME_TEXT);
	
	virtual bool shutdown(UInt16 statusCode, const std::string& statusMessage);		
	
	//
	/// Client side

	virtual void sendClientRequest();
	virtual void handleClientResponse(Buffer& buffer); 
	//virtual void prepareClientRequest(http::Request& request);
	//virtual void verifyClientResponse(http::Response& response);
	
	//
	/// Server side

	virtual void handleServerRequest(Buffer& buffer);
	//virtual void sendConnectResponse(); 
	//virtual void verifyServerRequest(http::Request& request);
	//virtual void prepareClientResponse(http::Response& response);

	virtual void onSocketConnect();
	virtual void onSocketRecv(Buffer& buffer, const net::Address& peerAddr);

protected:
	virtual ~WebSocketAdapter();

protected:
	WebSocketFramer framer;
	
	http::Request& _request;
	http::Response& _response;

	friend class WebSocketFramer;
};

	
	
	//Signal<http::Request&> PrepareClientRequest;
	//Signal<http::Response&> VerifyClientResponse;

	//Signal<http::Request&> VerifyServerRequest;
	//Signal<http::Response&> PrepareServerResponse;

	//virtual http::Request createrequest();
		// Returns a reference to the externally managed   
		// HTTP request object.

	//virtual http::Response& response();
		// Returns a reference to  the externally managed   
		// HTTP response object.	
	
	//virtual void setRequest(http::Request* request);
		// Sets the externally managed HTTP request 
		// object for client WS connection.

	//virtual void setResponse(http::Response* response);
		// Sets the externally managed HTTP response 
		// object for server WS connection.


// ---------------------------------------------------------------------
//
class Connection;
class WebSocketConnectionAdapter: public WebSocketAdapter
{
public:	
	WebSocketConnectionAdapter(Connection& connection, WebSocket::Mode mode); //socket = nil, http::Request* request = nil

protected:
	virtual ~WebSocketConnectionAdapter();

	Connection& _connection;
};


	//http::Request* request;

/*
// ---------------------------------------------------------------------
//
class ClientConnection;
class WebSocketClientAdapter: public WebSocketAdapter
{
public:	
	WebSocketClientAdapter(ClientConnection& connection); //socket = nil, http::Request* request = nil
	
	virtual void sendClientRequest(http::Request& request);

	//http::Request* request;

protected:
	virtual ~WebSocketClientAdapter();

	ClientConnection& _connection;
};


// ---------------------------------------------------------------------
//
class ServerConnection;
class WebSocketServerAdapter: public WebSocketAdapter
{
public:	
	WebSocketServerAdapter(ServerConnection& connection); //net::Socket* socket = nil

	virtual void onSocketRecv(Buffer& buffer, const net::Address& peerAddr);

protected:
	virtual ~WebSocketServerAdapter();

	ServerConnection& _connection;
};
*/



} } // namespace scy::Net


#endif //  SOURCEY_NET_WebSocket_H



/* 

	//virtual void onSocketConnect();
	//virtual void onSocketRecv(Buffer& buffer, const net::Address& peerAddr);
	
	//SocketBase* socketBase, bool mustMaskPayload
	//uv::TCPBase	
	//Buffer& incomingBuffer();

	//virtual void onConnect(int status);
	//virtual void onRead(const char* data, int len);
	//virtual void onRecv(Buffer& buf);	


	//virtual void* instance() { return this; }	

		// Reads the raw WS payload
		// Receives and emits the framed payload

	//virtual void close();
		// Closes the socket.
		//
		// Shuts down the connection by attempting
		// an orderly SSL shutdown, then actually
		// shutting down the TCP connection.
		
	//int available() const;
		// Returns the number of bytes available from the
		// SSL buffer for immediate reading.

// ---------------------------------------------------------------------
//
class WebSocketReceiver
	/// This class implements a WebSocket parser according
	/// to the WebSocket protocol described in RFC 6455.
{
public:
	WebSocketFramer(WebSocketAdapter* socketBase, bool mustMaskPayload);
		// Creates a SocketBase using the given native socket.

	virtual ~WebSocketFramer();

	virtual int writeFrame(const char* buffer, int length, int flags, Buffer& frame);
		// Writes a WebSocket protocol frame from the given data.
		
	virtual int readFrame(Buffer& buffer);
		// Receives a WebSocket protocol frame.
		//
		// The buffer's read position and size will be set to match
		// the actual beginning and end of the contained payload.
		// This enables us to work with the socket buffer directly
		// without copying any data.

	
	//WebSocket::Mode mode() const;


	//
	/// Server side

	//void updateAcceptHandshake(const char* buffer, int length, int flags);


	bool connected() const;
		// Return true when the socket is connected 
		// and the handshake is complete.

protected:
	int frameFlags() const;
		// Returns the frame flags of the most recently received frame.
		
	bool mustMaskPayload() const;
		// Returns true if the payload must be masked.	
	
	enum
	{
		FRAME_FLAG_MASK   = 0x80,
		MAX_HEADER_LENGTH = 14
	};

private:
	WebSocketAdapter* _socketBase;
	WebSocketFramer framer;
	int _frameFlags;
	bool _mustMaskPayload;
	int _headerState;
	Poco::Random _rnd;
	std::string _key; // client handshake key

	friend class WebSocketAdapter;
};
*/



	
	/*
	
	//WebSocketAdapter& base() const;
		// Returns the SocketBase for this socket.
	virtual int send(const char* data, int len, int flags = WebSocket::FRAME_BINARY);
	virtual int send(const char* data, int len, const net::Address& peerAddr, int flags = WebSocket::FRAME_BINARY);
	*/



		
	/*
	
	//, int flags // char* buffer, int length
	//int readNBytes(char* buffer, int bytes);
	virtual SocketImpl* acceptConnection(SocketAddress& clientAddr);
	virtual void connect(const SocketAddress& address);
	virtual void connect(const SocketAddress& address, const Timespan& timeout);
	virtual void connectNB(const SocketAddress& address);
	virtual void bind(const SocketAddress& address, bool reuseAddress = false);
	virtual void bind6(const SocketAddress& address, bool reuseAddress = false, bool ipV6Only = false);
	virtual void listen(int backlog = 64);
	virtual void close();
	virtual void shutdownReceive();
	virtual void shutdownSend();
	virtual void shutdown();
	virtual int sendTo(const char* buffer, int length, const SocketAddress& address, int flags = 0);
	virtual int receiveFrom(char* buffer, int length, SocketAddress& address, int flags = 0);
	virtual void sendUrgent(unsigned char data);
	virtual bool secure() const;
	virtual void setSendTimeout(const Timespan& timeout);	
	virtual Timespan getSendTimeout();
	virtual void setReceiveTimeout(const Timespan& timeout);
	virtual Timespan getReceiveTimeout();
	*/
/*
class IWebSocket: public ISocket
{
public:
	virtual ~IWebSocket() {};
	
	virtual void connect(const std::string& uri) = 0;
	virtual void connect() = 0;
	virtual void close() = 0;
	
	virtual void setProtocol(const std::string& proto) = 0;
	virtual void setCookie(const std::string& cookie) = 0;

	virtual UInt16 port() = 0;

	NullSignal Online;
		// Signals that the socket is validated
		// and ready to send data.
};
	

// ---------------------------------------------------------------------
//
class IPacketWebSocket: public IPacketSocket
{
public:
	virtual ~IPacketWebSocket() {};
	
	virtual void connect(const std::string& uri) = 0;
	virtual void connect() = 0;
	virtual void close() = 0;
	
	virtual void setProtocol(const std::string& proto) = 0;
	virtual void setCookie(const std::string& cookie) = 0;

	virtual UInt16 port() = 0;

	NullSignal Online;
		// Signals that the socket is validated
		// and ready to send data.
};


// ---------------------------------------------------------------------
//
template <class SocketBaseT>
class WebSocketAdapter: public SocketBaseT
{
public:
	WebSocketAdapter(Reactor& reactor) :
		SocketBaseT(reactor)
	{  
	}

	WebSocketAdapter(Reactor& reactor, const std::string& uri) :
		SocketBaseT(reactor),
		_headerState(0),
		_uri(uri)
	{
	}

	virtual ~WebSocketAdapter()
	{
	}

	virtual void connect(const std::string& uri)
	{
		_uri = uri;
		connect();
	}

	virtual void connect()
	{
		_headerState = 0;
	
		log("debug") << "Web Socket Connecting to " << _uri.toString() << std::endl;	

		// Will throw on error
		SocketBaseT::connect(Address(_uri.getHost(), port()));
	}

	virtual void close()
	{
		log("debug") << "WebSocket Closing" << std::endl;
		_headerState = 0;
	
		try {
			if (isConnected()) {
				Poco::Net::SocketStream ss(*this);
				ss << 0xff;
				ss << 0x00;
				ss.flush();
			}
		}
		catch (Exception& exc) {
			// Not a fatal error
			log("warn") << "WebSocket Closing Error: " << exc.message() << std::endl;
		}

		SocketBaseT::close();
	}

	void sendHandshake()
	{
		log("debug") << "Sending Handshake" << std::endl;

		Poco::Net::SocketStream ss(*this);
	
#define WS_KEY_ONE		"18x 6]8vM;54 *(5:  {   U1]8  z [  8"
#define WS_KEY_TWO		"1_ tx7X d  <  nw  334J702) 7]o}` 0"
#define WS_KEY_THREE	" Tm[K T2u"

		ss << "GET " << _uri.getPathAndQuery() << " HTTP/1.1" << "\r\n";
		ss << "Upgrade: WebSocket" << "\r\n";
		ss << "Connection: Upgrade" << "\r\n";
		ss << "Host: " << _uri.getUserInfo() << "\r\n";
		ss << "Origin: " << _uri.toString() << "\r\n";
		ss << "Sec-WebSocket-Key1:" << WS_KEY_ONE << "\r\n";
		ss << "Sec-WebSocket-Key2:" << WS_KEY_TWO << "\r\n";
		if (!_protocol.empty()) {
			ss << "Sec-WebSocket-Protocol: " << _protocol << "\r\n";
		}
		if (!_cookie.empty()) {
			ss << "Cookie: " << _cookie << "\r\n";
		}
		ss << "\r\n";
		ss << WS_KEY_THREE;
		ss.flush();
		
		//onHandshake();
	}

	virtual int send(const char* data, int size)
	{
		//log("trace") << "Send: " << (std::string(data, size)) << std::endl;	
		Poco::Net::SocketStream ss(*this);
		ss.put(static_cast<unsigned char>(0x00));
		ss.write(data, size);
		ss.put(static_cast<unsigned char>(0xff));
		ss.flush();
		return size;
	}

	virtual int send(const IPacket& packet)
	{
		Buffer buf;
		packet.write(buf);
		return send(buf.data(), buf.available());
	}

	virtual void recv(Buffer& buffer) 
	{
		log("trace") << "On Data: " << buffer.available() << std::endl;
	
		if (buffer.available() == 0 || (
			buffer.available() == 2 && buffer.data()[0] == 0xff && buffer.data()[1] == 0x00)) {
			log("debug") << "Recv Close" << std::endl;
			close();
			return;
		}
	
		// Read Messages
		else if (_headerState == 2) {
			log("trace") << "Parsing Messages: " << buffer.available() << std::endl;
		
			while (!buffer.eof()) {
				std::string message;
				UInt8 start;
				buffer.getUInt8(start);

				if (start != 0x00) {
					log("warn") << "Message contains bad start code" << std::endl;
					return;
				}

				if (buffer.readToNext(message, static_cast<unsigned char>(0xff)) == 0) {
					buffer.readString(message, buffer.available() - 1);
				}
			
				log("trace") << "Parsed Message: " << message << std::endl;
				Buffer msgBuf(message.c_str(), message.size());
				packetize(msgBuf);
				buffer++;
			}
			return;
		}

		// Read Initial HTTP Header
		else if (_headerState == 0) { //stateEquals(ClientState::Connected)

			std::string request(buffer.c_str(), buffer.available());
			log("debug") << "Parsing Header: " << request << std::endl;			
			if (strncmp(request.c_str(), "HTTP/1.1 101 ", 13) == 0) {
				log("debug") << "Received HTTP Response" << std::endl;	
				size_t pos = pos = request.find("\r\n\r\n");
				if (pos != std::string::npos) {
					buffer.setPosition(pos + 4);
					_headerState = 1;
				}
				else {
					setError("Invalid response header");
					return;
				}
			}
		}

		// Read the Digest
		// NOTE: Occasionally the digest is received after the header
		// so we need to handle this here.
		if (_headerState == 1 && buffer.available() >= 16) {
			std::string replyDigest;
			buffer.readString(replyDigest, 16);
			log("debug") << "Reply Digest: " << replyDigest << std::endl;
			_headerState = 2;

			// TODO: Validate Digest

			onOnline();
		}
	}
	
	virtual void setProtocol(const std::string& proto)
	{
		ScopedLock lock(_mutex);
		_protocol = proto;
	}
	
	virtual void setCookie(const std::string& cookie)
	{
		ScopedLock lock(_mutex);
		_cookie = cookie;
	}
		
	virtual UInt16 port()
	{
		ScopedLock lock(_mutex);
		return _uri.getPort() ? _uri.getPort() : (_uri.getScheme() == "wss" ? 443 : 80);
	}

	virtual const char* className() const { return "WebSocketAdapter"; }
	

protected:	
	virtual void onConnect()
	{
		log("debug") << "Web Socket Connected" << std::endl;
		SocketBaseT::onConnect();
		sendHandshake();
	}

	virtual void onOnline()
	{
		log("debug") << "Web Socket Online" << std::endl;

		Online.emit(static_cast<SocketBaseT::InterfaceT*>(this));
	}


protected:	
	mutable Mutex _mutex;

	http::URL _uri;
	std::string _protocol;
	std::string _cookie;
	int _headerState;
};


typedef WebSocketAdapter< Net::SocketBase< ::Poco::Net::StreamSocket, TCP, http::WebSocket > >  WebSocket;
typedef WebSocketAdapter< Net::SocketBase< ::Poco::Net::SecureStreamSocket, SSLTCP, http::WebSocket > >  SSLWebSocket;
typedef WebSocketAdapter< Net::PacketSocketBase< ::Poco::Net::StreamSocket, TCP, std::stringacketWebSocket > >  PacketWebSocket;
typedef WebSocketAdapter< Net::PacketSocketBase< ::Poco::Net::SecureStreamSocket, SSLTCP, std::stringacketWebSocket > >  SSLPacketWebSocket;
*/




/*
class WebSocket: public SocketT
{
public:
	WebSocket(Reactor& reactor);
	WebSocket(Reactor& reactor, const http::URL& uri);
	virtual ~WebSocket();
	
	void connect(const http::URL& uri);
	void connect();
	void close();
	
	virtual int send(const char* data, int size);
	virtual int send(const IPacket& packet);
	
	void setProtocol(const std::string& proto);
	void setCookie(const std::string& cookie);

protected:
	virtual void sendHandshake();
	virtual void onConnect();

	virtual void recv(Buffer& buffer);

	//virtual void onReadable(SocketEvent& event);
	//virtual void onError(SocketEvent& event);
	
	http::URL _uri;
	std::string _protocol;
	std::string _cookie;
	int _headerState;
};
*/