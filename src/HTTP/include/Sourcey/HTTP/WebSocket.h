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
#include "Sourcey/HTTP/Client.h"
#include "Poco/Random.h"
/*
#include "Sourcey/Net/Reactor.h"
#include "Sourcey/Net/StatefulSocket.h"

#include "Poco/URI.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SecureStreamSocket.h"
*/


namespace scy {
namespace http {

	
class WebSocketBase;
class WebSocket: public net::Socket
	/// WebSocket is a disposable SSL socket wrapper
	/// for WebSocketBase which can be created on the stack.
	/// See WebSocketBase for implementation details.
{
public:	
	typedef WebSocketBase Base;
	typedef std::vector<WebSocket> List;
	
	enum Mode
	{
		WS_SERVER, /// Server-side WebSocket.
		WS_CLIENT  /// Client-side WebSocket.
	};
	
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
	
	static const std::string WEBSOCKET_GUID;
	static const std::string WEBSOCKET_VERSION;
		/// The WebSocket protocol version supported (13).
	
	WebSocket();
		/// Creates an unconnected WebSocket.

	WebSocket(WebSocketBase* base, bool shared = false);
		/// Creates the Socket and attaches the given SocketBase.
		///
		/// The SocketBase must be a WebSocketBase, otherwise an
		/// exception will be thrown.

	WebSocket(const net::Socket& socket);
		/// Creates the WebSocket with the SocketBase
		/// from another socket. The SocketBase must be
		/// a WebSocketBase, otherwise an exception will be thrown.
	
	WebSocketBase& base() const;
		/// Returns the SocketBase for this socket.
};
	
	
// ---------------------------------------------------------------------
//
class WebSocketParser
	/// This class implements a WebSocket parser according
	/// to the WebSocket protocol described in RFC 6455.
{
public:
	WebSocketParser(WebSocketBase* socketBase, bool mustMaskPayload);
		/// Creates a SocketBase using the given native socket.

	virtual ~WebSocketParser();

	virtual int sendFrame(const char* buffer, int length, int flags);
		/// Sends a WebSocket protocol frame.
		
	virtual int receiveFrame(Buffer& buffer);
		/// Receives a WebSocket protocol frame.
		/// Data is read from the given buffer and raw packet
		/// data is emitted via the WebSocket::Recv signal.
	
	//WebSocket::Mode mode() const;


	//
	/// Client side

	void sendHandshakeRequest(); 

	bool handleHandshakeResponse();
		/// Reads the HTTP handshake response.
		/// Returns true on success, false on resend
		/// request, or throws of error.

	void completeHandshake(http::Response& response);


	//
	/// Server side

	//void updateAcceptHandshake(const char* buffer, int length, int flags);


	bool connected() const;
		/// Return true when the socket is connected 
		/// and the handshake is complete.

protected:
	enum
	{
		FRAME_FLAG_MASK   = 0x80,
		MAX_HEADER_LENGTH = 14
	};

	int frameFlags() const;
		/// Returns the frame flags of the most recently received frame.
		
	bool mustMaskPayload() const;
		/// Returns true if the payload must be masked.
	

private:
	WebSocketBase* _socketBase;
	int _frameFlags;
	bool _mustMaskPayload;
	int _headerState;
	Poco::Random _rnd;
	std::string _key; // client handshake key

	friend class WebSocketBase;
};


//
// inlines
//
inline int WebSocketParser::frameFlags() const
{
	return _frameFlags;
}


inline bool WebSocketParser::mustMaskPayload() const
{
	return _mustMaskPayload;
}


// ---------------------------------------------------------------------
//
class WebSocketBase: public uv::TCPBase	
{
public:	
	WebSocketBase();
	
	virtual bool shutdown(Poco::UInt16 statusCode, const std::string& statusMessage);

	//virtual void close();
		/// Closes the socket.
		///
		/// Shuts down the connection by attempting
		/// an orderly SSL shutdown, then actually
		/// shutting down the TCP connection.
	
	virtual int send(const char* data, int len, int flags = WebSocket::FRAME_BINARY);
		
	int available() const;
		/// Returns the number of bytes available from the
		/// SSL buffer for immediate reading.

	virtual void onConnect(int status);

	virtual void onRead(const char* data, int len);
		/// Reads the raw WS payload

	virtual void onRecv(Buffer& buf);	
		/// Reads and emits application data
		
protected:
	virtual void* instance() { return this; }
	virtual ~WebSocketBase();

protected:
	WebSocketParser _wsParser;

	friend class WebSocketParser;
};




		
	/*
	
	//, int flags // char* buffer, int length
	//int readNBytes(char* buffer, int bytes);
	virtual SocketImpl* acceptConnection(SocketAddress& clientAddr);
	virtual void connect(const SocketAddress& address);
	virtual void connect(const SocketAddress& address, const Poco::Timespan& timeout);
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
	virtual void setSendTimeout(const Poco::Timespan& timeout);	
	virtual Poco::Timespan getSendTimeout();
	virtual void setReceiveTimeout(const Poco::Timespan& timeout);
	virtual Poco::Timespan getReceiveTimeout();
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
		/// Signals that the socket is validated
		/// and ready to send data.
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
		/// Signals that the socket is validated
		/// and ready to send data.
};


// ---------------------------------------------------------------------
//
template <class SocketBaseT>
class WebSocketBase: public SocketBaseT
{
public:
	WebSocketBase(Reactor& reactor) :
		SocketBaseT(reactor)
	{  
	}

	WebSocketBase(Reactor& reactor, const std::string& uri) :
		SocketBaseT(reactor),
		_headerState(0),
		_uri(uri)
	{
	}

	virtual ~WebSocketBase()
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
			log("warn") << "WebSocket Closing Error: " << exc.displayText() << std::endl;
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
		return send(buf.data(), buf.size());
	}

	virtual void recv(Buffer& buffer) 
	{
		log("trace") << "On Data: " << buffer.size() << std::endl;
	
		if (buffer.size() == 0 || (
			buffer.size() == 2 && buffer.data()[0] == 0xff && buffer.data()[1] == 0x00)) {
			log("debug") << "Recv Close" << std::endl;
			close();
			return;
		}
	
		// Read Messages
		else if (_headerState == 2) {
			log("trace") << "Parsing Messages: " << buffer.size() << std::endl;
		
			while (!buffer.eof()) {
				std::string message;
				UInt8 start;
				buffer.readUInt8(start);

				if (start != 0x00) {
					log("warn") << "Message contains bad start code" << std::endl;
					return;
				}

				if (buffer.readToNext(message, static_cast<unsigned char>(0xff)) == 0) {
					buffer.readString(message, buffer.remaining() - 1);
				}
			
				log("trace") << "Parsed Message: " << message << std::endl;
				Buffer msgBuf(message.data(), message.size());
				packetize(msgBuf);
				buffer++;
			}
			return;
		}

		// Read Initial HTTP Header
		else if (_headerState == 0) { //stateEquals(ClientState::Connected)

			std::string request(buffer.data(), buffer.size());
			log("debug") << "Parsing Header: " << request << std::endl;			
			if (strncmp(request.data(), "HTTP/1.1 101 ", 13) == 0) {
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
		if (_headerState == 1 && buffer.remaining() >= 16) {
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
		Poco::FastMutex::ScopedLock lock(_mutex);
		_protocol = proto;
	}
	
	virtual void setCookie(const std::string& cookie)
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		_cookie = cookie;
	}
		
	virtual UInt16 port()
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		return _uri.getPort() ? _uri.getPort() : (_uri.getScheme() == "wss" ? 443 : 80);
	}

	virtual const char* className() const { return "WebSocketBase"; }
	

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
	mutable Poco::FastMutex _mutex;

	Poco::URI _uri;
	std::string _protocol;
	std::string _cookie;
	int _headerState;
};


typedef WebSocketBase< Net::SocketBase< ::Poco::Net::StreamSocket, TCP, Net::IWebSocket > >  WebSocket;
typedef WebSocketBase< Net::SocketBase< ::Poco::Net::SecureStreamSocket, SSLTCP, Net::IWebSocket > >  SSLWebSocket;
typedef WebSocketBase< Net::PacketSocketBase< ::Poco::Net::StreamSocket, TCP, Net::IPacketWebSocket > >  PacketWebSocket;
typedef WebSocketBase< Net::PacketSocketBase< ::Poco::Net::SecureStreamSocket, SSLTCP, Net::IPacketWebSocket > >  SSLPacketWebSocket;
*/


} } // namespace scy::Net


#endif //  SOURCEY_NET_WebSocket_H





/*
class WebSocket: public SocketT
{
public:
	WebSocket(Reactor& reactor);
	WebSocket(Reactor& reactor, const Poco::URI& uri);
	virtual ~WebSocket();
	
	void connect(const Poco::URI& uri);
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
	
	Poco::URI _uri;
	std::string _protocol;
	std::string _cookie;
	int _headerState;
};
*/