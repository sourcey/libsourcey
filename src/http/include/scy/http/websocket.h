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


#ifndef SCY_NET_WebSocket_H
#define SCY_NET_WebSocket_H


#include "scy/base.h"
#include "scy/buffer.h"
#include "scy/net/socket.h"
#include "scy/net/socketadapter.h"
#include "scy/net/tcpsocket.h"
#include "scy/http/request.h"
#include "scy/http/response.h"
#include "scy/http/parser.h"
#include "scy/random.h"


namespace scy {
namespace http {
	class Connection;
namespace ws {
	
		
enum Mode
{
	ServerSide, /// Server-side WebSocket.
	ClientSide  /// Client-side WebSocket.
};
	
enum class FrameFlags
	/// Frame header flags.
{
	Fin  = 0x80, /// FIN bit: final fragment of a multi-fragment message.
	Rsv1 = 0x40, /// Reserved for future use. Must be zero.
	Rsv2 = 0x20, /// Reserved for future use. Must be zero.
	Rsv3 = 0x10, /// Reserved for future use. Must be zero.
};
	
enum class Opcode
	/// Frame header opcodes.
{
	Continuation	= 0x00, /// Continuation frame.
	Text			= 0x01, /// Text frame.
	Binary			= 0x02, /// Binary frame.
	Close			= 0x08, /// Close connection.
	Ping			= 0x09, /// Ping frame.
	Pong			= 0x0a, /// Pong frame.
	Bitmask			= 0x0f  /// Bit mask for opcodes. 
};
	
enum SendFlags
	/// Combined header flags and opcodes for identifying 
	/// the payload type of sent frames.
{
	Text   = unsigned(ws::FrameFlags::Fin) | unsigned(ws::Opcode::Text),
	Binary = unsigned(ws::FrameFlags::Fin) | unsigned(ws::Opcode::Binary)
};
	
enum StatusCodes
	/// StatusCodes for CLOSE frames sent with shutdown().
{
	StatusNormalClose			= 1000,
	StatusEndpointGoingAway		= 1001,
	StatusProtocolError			= 1002,
	StatusPayloadNotAcceptable	= 1003,
	StatusReserved              = 1004,
	StatusReservedNoStatusCode	= 1005,
	StatusReservedAbnormalClose	= 1006,
	StatusMalformedPayload		= 1007,
	StatusPolicyViolation		= 1008,
	StatusPayloadTooBig			= 1009,
	StatusExtensionRequired		= 1010,
	StatusUnexpectedCondition	= 1011,
	StatusReservedTLSFailure	= 1015
};
	
enum ErrorCodes
	/// These error codes can be obtained from WebSocket exceptions
	/// to determine the exact cause of the error.
{
	ErrorNoHandshake             = 1,
		/// No Connection: Upgrade or Upgrade: websocket header in handshake request.
	ErrorHandshakeNoVersion      = 2,
		/// No Sec-WebSocket-Version header in handshake request.
	ErrorHandshakeUnsupportedVersion = 3,
		/// Unsupported WebSocket version requested by client.
	ErrorHandshakeNoKey          = 4,
		/// No Sec-WebSocket-Key header in handshake request.
	ErrorHandshakeAccept         = 5,
		/// No Sec-WebSocket-Accept header or wrong value.
	ErrorNotAuthorized            = 6,
		/// The server rejected the username or password for authentication.
	ErrorPayloadTooBig           = 10,
		/// Payload too big for supplied buffer.
	ErrorIncompleteFrame         = 11
		/// Incomplete frame received.
};
	
static const char* ProtocolGuid = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
static const char* ProtocolVersion = "13";
	// The WebSocket protocol version supported (13).

	
//
// WebSocket Framer
//


class WebSocketFramer
	/// This class implements a WebSocket parser according
	/// to the WebSocket protocol described in RFC 6455.
{
public:
	WebSocketFramer(ws::Mode mode);
		// Creates a Socket using the given Socket.

	virtual ~WebSocketFramer();

	virtual std::size_t writeFrame(const char* data, std::size_t len, int flags, BitWriter& frame);
		// Writes a WebSocket protocol frame from the given data.
		
	virtual UInt64 readFrame(BitReader& frame, char*& payload); //Buffer& buffer, const char* buffer, int length, 
		// Reads a single WebSocket frame from the given buffer (frame).
		//
		// The actual payload length is returned, and the beginning of the
		// payload buffer will be assigned in the second (payload) argument.
		// No data is copied.
		//
		// If the frame is invalid or too big an exception will be thrown.
	
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
		
	ws::Mode mode() const;
	
	enum
	{
		FRAME_FLAG_MASK   = 0x80,
		MAX_HEADER_LENGTH = 14
	};

private:
	ws::Mode _mode;
	int _frameFlags;
	int _headerState;
	bool _maskPayload;
	Random _rnd;
	std::string _key; // client handshake key

	friend class WebSocketAdapter;
};


//
// WebSocket Adapter
//


class WebSocketAdapter: public net::SocketAdapter
{
public:	
	WebSocketAdapter(const net::Socket::Ptr& socket, ws::Mode mode, http::Request& request, http::Response& response); 
	//WebSocketAdapter(ws::Mode mode, http::Request& request, http::Response& response);
	
	virtual int send(const char* data, std::size_t len, int flags = 0); // flags = ws::Text || ws::Binary
	virtual int send(const char* data, std::size_t len, const net::Address& peerAddr, int flags = 0); // flags = ws::Text || ws::Binary
	
	virtual bool shutdown(UInt16 statusCode, const std::string& statusMessage);		
	
	net::Socket::Ptr socket;
		// Pointer to the underlying socket.
		// Sent data will be proxied to this socket.

	//
	/// Client side

	virtual void sendClientRequest();
	virtual void handleClientResponse(const MutableBuffer& buffer); 
	//virtual void prepareClientRequest(http::Request& request);
	//virtual void verifyClientResponse(http::Response& response);
	
	//
	/// Server side

	virtual void handleServerRequest(const MutableBuffer& buffer);
	//virtual void sendConnectResponse(); 
	//virtual void verifyServerRequest(http::Request& request);
	//virtual void prepareClientResponse(http::Response& response);

	virtual void onSocketConnect();
	virtual void onSocketRecv(const MutableBuffer& buffer, const net::Address& peerAddress);
	virtual void onSocketClose();

protected:
	virtual ~WebSocketAdapter();

	friend class WebSocketFramer;

	WebSocketFramer framer;	
	http::Request& _request;
	http::Response& _response;
};


//
// WebSocket
//


class WebSocket: public WebSocketAdapter
	/// Standalone WebSocket class.
{
public:		
	typedef std::vector<WebSocket> Vec;
	
	WebSocket(const net::Socket::Ptr& socket);
		// Creates the WebSocket with the given Socket.
		// The Socket should be a TCPSocket or a SSLSocket, 
		// depending on the protocol used (ws or wss).

	virtual ~WebSocket();

	http::Request& request();
	http::Response& response();
	
protected:
	http::Request _request;
	http::Response _response;
};


//
// WebSocket Connection Adapter
//


class ConnectionAdapter: public WebSocketAdapter
	/// WebSocket class which belongs to a HTTP Connection.
{
public:	
	ConnectionAdapter(Connection& connection, ws::Mode mode);
	virtual ~ConnectionAdapter();

protected:
	Connection& _connection;
};


} } } // namespace scy::http::ws


#endif //  SCY_NET_WebSocket_H


	//WebSocketAdapter(const net::Socket::Ptr& socket, ws::Mode mode, http::Request& request, http::Response& response); 
	
	//WebSocket();
		// Creates an unconnected WebSocket.

	//WebSocket(net::Socket* base, bool shared = false);
		// Creates the Socket and attaches the given Socket.
		//
		// The Socket must be a WebSocketAdapter, otherwise an
		// exception will be thrown.
	//net::Socket::Ptr _socket;
	//WebSocket(const net::Socket::Ptr& socket);

	//WebSocketAdapter& adapter() const;
		// Returns the WebSocketAdapter for this socket.
		
	//net::Socket& socket();
		// Returns the underlying TCP or SSL socket.
	
	//virtual bool shutdown(UInt16 statusCode, const std::string& statusMessage);


 //socket = nullptr, http::Request* request = nullptr

	//http::Request* request;

/*
// ---------------------------------------------------------------------
//
class ClientConnection;
class WebSocketClientAdapter: public WebSocketAdapter
{
public:	
	WebSocketClientAdapter(ClientConnection& connection); //socket = nullptr, http::Request* request = nullptr
	
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
	WebSocketServerAdapter(ServerConnection& connection); //net::Socket::Ptr socket = nullptr

	virtual void onSocketRecv(const MutableBuffer& buffer, const net::Address& peerAddress);

protected:
	virtual ~WebSocketServerAdapter();

	ServerConnection& _connection;
};
*/
	
	
	//Signal<http::Request&> PrepareClientRequest;
	//Signal<http::Response&> VerifyClientResponse;

	//Signal<http::Request&> VerifyServerRequest;
	//Signal<http::Response&> PrepareServerResponse;

	//virtual http::Request createrequest();
		/// Returns a reference to the externally managed   
		/// HTTP request object.

	//virtual http::Response& response();
		/// Returns a reference to  the externally managed   
		/// HTTP response object.	
	
	//virtual void setRequest(http::Request* request);
		/// Sets the externally managed HTTP request 
		/// object for client WS connection.

	//virtual void setResponse(http::Response* response);
		/// Sets the externally managed HTTP response 
		/// object for server WS connection.
