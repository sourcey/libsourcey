#ifndef Net_ServerWebSocket_INCLUDED
#define Net_ServerWebSocket_INCLUDED


#include "Sourcey/Types.h"
#include "Poco/Net/Net.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/HTTPCredentials.h"

namespace Poco {
namespace Net {
	class WebSocketImpl;
	class HTTPServerRequest;
	class HTTPServerResponse;
	class HTTPClientSession;
}}

namespace Scy {
namespace Net {


class ServerWebSocket: public Poco::Net::StreamSocket
	/// This class implements a WebSocket according
	/// to the WebSocket protocol specification in RFC 6455.
	///
	/// Both client-side and server-side WebSockets
	/// are supported.
	///
	/// Server-side WebSockets are usually created from within
	/// a HTTPRequestHandler.
	///
	/// Note that special frames like PING must be handled at
	/// application level. In the case of a PING, a PONG message
	/// must be returned.
{
public:
	/*
	enum Mode
	{
		WS_SERVER, /// Server-side WebSocket.
		WS_CLIENT  /// Client-side WebSocket.
	};
	*/
	
	enum FrameFlags
		/// Frame header flags.
	{
		FRAME_FLAG_FIN  = 0x80, /// FIN bit: final fragment of a multi-fragment message.
		FRAME_FLAG_RSV1 = 0x40, /// Reserved for future use. Must be zero.
		FRAME_FLAG_RSV2 = 0x20, /// Reserved for future use. Must be zero.
		FRAME_FLAG_RSV3 = 0x10, /// Reserved for future use. Must be zero.
	};

	enum FrameOpcodes
		/// Frame header opcodes.
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
		/// Combined header flags and opcodes for use with sendFrame().
	{
		FRAME_TEXT   = FRAME_FLAG_FIN | FRAME_OP_TEXT,
			/// Use this for sending a single text (UTF-8) payload frame.
		FRAME_BINARY = FRAME_FLAG_FIN | FRAME_OP_BINARY
			/// Use this for sending a single binary payload frame.
	};
	
	enum StatusCodes
		/// StatusCodes for CLOSE frames sent with shutdown().
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
		/// These error codes can be obtained from a WebSocketException
		/// to determine the exact cause of the error.
	{
		WS_ERR_NO_HANDSHAKE                   = 1,
			/// No Connection: Upgrade or Upgrade: websocket header in handshake request.
		WS_ERR_HANDSHAKE_NO_VERSION           = 2,
			/// No Sec-WebSocket-Version header in handshake request.
		WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION  = 3,
			/// Unsupported WebSocket version requested by client.
		WS_ERR_HANDSHAKE_NO_KEY               = 4,
			/// No Sec-WebSocket-Key header in handshake request.
		WS_ERR_HANDSHAKE_ACCEPT               = 5,
			/// No Sec-WebSocket-Accept header or wrong value.
		WS_ERR_UNAUTHORIZED                   = 6,
			/// The server rejected the username or password for authentication.
		WS_ERR_PAYLOAD_TOO_BIG                = 10,
			/// Payload too big for supplied buffer.
		WS_ERR_INCOMPLETE_FRAME               = 11
			/// Incomplete frame received.
	};
	
	ServerWebSocket(Poco::Net::StreamSocket& socket, Poco::Net::HTTPRequest& request);
	//ServerWebSocket(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);

	//ServerWebSocket(Poco::Net::WebSocketImpl* impl);
		/// Creates an unconnected stream socket.

	ServerWebSocket(Poco::Net::WebSocketImpl* impl);
		/// Creates an unconnected stream socket.
		
	virtual ~ServerWebSocket();
		/// Destroys the StreamSocket.

	void shutdown();
		/// Sends a Close control frame to the server end of
		/// the connection to initiate an orderly shutdown
		/// of the connection.

	void shutdown(Poco::UInt16 statusCode, const std::string& statusMessage = "");
		/// Sends a Close control frame to the server end of
		/// the connection to initiate an orderly shutdown
		/// of the connection.
	
	int sendFrame(const void* buffer, int length, int flags = FRAME_TEXT);
		/// Sends the contents of the given buffer through
		/// the socket as a single frame.
		/// 
		/// Values from the FrameFlags, FrameOpcodes and SendFlags enumerations
		/// can be specified in flags.
		///
		/// Returns the number of bytes sent, which may be
		/// less than the number of bytes specified.
		///
		/// Certain socket implementations may also return a negative
		/// value denoting a certain condition.

	int receiveFrame(void* buffer, int length, int& flags);
		/// Receives a frame from the socket and stores it
		/// in buffer. Up to length bytes are received. If
		/// the frame's payload is larger, a WebSocketException
		/// is thrown and the WebSocket connection must be 
		/// terminated.
		///
		/// Returns the number of bytes received. 
		/// A return value of 0 means a graceful shutdown 
		/// of the connection from the peer.
		///
		/// Throws a TimeoutException if a receive timeout has
		/// been set and nothing is received within that interval.
		/// Throws a NetException (or a subclass) in case of other errors.
		///
		/// The frame flags and opcode (FrameFlags and FrameOpcodes)
		/// is stored in flags.

	static const std::string WEBSOCKET_VERSION;
		/// The WebSocket protocol version supported (13).
		
	static Poco::Net::WebSocketImpl* accept(Poco::Net::StreamSocket& socket, Poco::Net::HTTPRequest& request);

	//static Poco::Net::WebSocketImpl* accept(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
		/// Handles the WebSocket handshake and creates the 
		/// WebSocketImpl instance.
		/// An exception will be thrown on error.
	
protected:
	static std::string computeAccept(const std::string& key);
	static std::string createKey();
		 
	static UInt32 extractNumber(const char *keyValue) {
		UInt32 value= 0;
		while (*keyValue) {
			if (isdigit(*keyValue)) {
				value= value*10 + (*keyValue-'0');
			};
			keyValue++;
		};
		return value;
	}
 
	static UInt32 countSpaces(const char *keyValue) {
		UInt32 count= 0;
		while (*keyValue) {
			if (*keyValue == 0x20) {
				count++;
			}
			keyValue++;
		}
		return count;
	}
	 
	static UInt32 computeKeyPart(const char *keyValue) {
		if (keyValue != NULL) {
			printf("key:%s:\n", keyValue);
			UInt32 keyPart= extractNumber(keyValue) / countSpaces(keyValue);
			printf("keyPart: %08x\n", keyPart);
			return keyPart;
		}
		// No key value
		return 0;
	}

	
	static void hexStringToBinary(const char *hex, char *binary) {
		while (*hex) {
			char c; 
			c= *hex++;
			*binary= (c < '@' ? c-'0' : c-'a'+10) << 4;
			c= *hex++;
			*binary |= (c < '@' ? c-'0' : c-'a'+10);
			binary++;
		}
	}
	/*
	*/
	
	static const std::string WEBSOCKET_GUID;
};


} } // namespace Scy::Net


#endif // Net_ServerWebSocket_INCLUDED

		
	
	/*
	ServerWebSocket(HTTPServerRequest& request, HTTPServerResponse& response);
		/// Creates a server-side WebSocket from within a
		/// HTTPRequestHandler.
		/// 
		/// First verifies that the request is a valid WebSocket upgrade
		/// request. If so, completes the handshake by sending
		/// a proper 101 response.
		///
		/// Throws an exception if the request is not a proper WebSocket
		/// upgrade request.
		
	WebSocket(HTTPClientSession& cs, HTTPRequest& request, HTTPResponse& response);
		/// Creates a client-side WebSocket, using the given
		/// HTTPClientSession and HTTPRequest for the initial handshake 
		/// (HTTP Upgrade request).
		///
		/// Additional HTTP headers for the initial handshake request
		/// (such as Origin or Sec-WebSocket-Protocol) can be given
		/// in the request object.
		///
		/// The result of the handshake can be obtained from the response
		/// object.

	WebSocket(HTTPClientSession& cs, HTTPRequest& request, HTTPResponse& response, HTTPCredentials& credentials);
		/// Creates a client-side WebSocket, using the given
		/// HTTPClientSession and HTTPRequest for the initial handshake 
		/// (HTTP Upgrade request).
		///
		/// The given credentials are used for authentication
		/// if requested by the server.
		///
		/// Additional HTTP headers for the initial handshake request
		/// (such as Origin or Sec-WebSocket-Protocol) can be given
		/// in the request object.
		///
		/// The result of the handshake can be obtained from the response
		/// object.
		*/

	//WebSocket& operator = (const Socket& socket);
		/// Assignment operator.
		///
		/// Releases the socket's SocketImpl and
		/// attaches the SocketImpl from the other socket and
		/// increments the reference count of the SocketImpl.	
	//Mode mode() const;
		/// Returns WS_SERVER if the WebSocket is a server-side
		/// WebSocket, or WS_CLIENT otherwise.
	//static WebSocketImpl* connect(HTTPClientSession& cs, HTTPRequest& request, HTTPResponse& response, HTTPCredentials& credentials);
	//static WebSocketImpl* completeHandshake(HTTPClientSession& cs, HTTPResponse& response, const std::string& key);