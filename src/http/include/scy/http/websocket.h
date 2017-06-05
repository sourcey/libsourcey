///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup http
/// @{


#ifndef SCY_NET_WebSocket_H
#define SCY_NET_WebSocket_H


#include "scy/base.h"
#include "scy/buffer.h"
#include "scy/http/parser.h"
#include "scy/http/request.h"
#include "scy/http/response.h"
#include "scy/http/connection.h"
#include "scy/net/socketemitter.h"
#include "scy/net/socket.h"
#include "scy/net/tcpsocket.h"
#include "scy/random.h"


namespace scy {
namespace http {
namespace ws {


enum Mode
{
    ServerSide,  ///< Server-side WebSocket.
    ClientSide   ///< Client-side WebSocket.
};


/// Frame header flags.
enum class FrameFlags
{
    Fin = 0x80,  ///< FIN bit: final fragment of a multi-fragment message.
    Rsv1 = 0x40, ///< Reserved for future use. Must be zero.
    Rsv2 = 0x20, ///< Reserved for future use. Must be zero.
    Rsv3 = 0x10, ///< Reserved for future use. Must be zero.
};


/// Frame header opcodes.
enum class Opcode
{
    Continuation = 0x00, ///< Continuation frame.
    Text = 0x01,         ///< Text frame.
    Binary = 0x02,       ///< Binary frame.
    Close = 0x08,        ///< Close connection.
    Ping = 0x09,         ///< Ping frame.
    Pong = 0x0a,         ///< Pong frame.
    Bitmask = 0x0f       ///< Bit mask for opcodes.
};


/// Combined header flags and opcodes for identifying
/// the payload type of sent frames.
enum SendFlags
{
    Text = unsigned(ws::FrameFlags::Fin) | unsigned(ws::Opcode::Text),
    Binary = unsigned(ws::FrameFlags::Fin) | unsigned(ws::Opcode::Binary)
};


/// StatusCodes for CLOSE frames sent with shutdown().
enum StatusCodes
{
    StatusNormalClose = 1000,
    StatusEndpointGoingAway = 1001,
    StatusProtocolError = 1002,
    StatusPayloadNotAcceptable = 1003,
    StatusReserved = 1004,
    StatusReservedNoStatusCode = 1005,
    StatusReservedAbnormalClose = 1006,
    StatusMalformedPayload = 1007,
    StatusPolicyViolation = 1008,
    StatusPayloadTooBig = 1009,
    StatusExtensionRequired = 1010,
    StatusUnexpectedCondition = 1011,
    StatusReservedTLSFailure = 1015
};


/// These error codes can be obtained from WebSocket exceptions
/// to determine the exact cause of the error.
enum ErrorCodes
{
    ErrorNoHandshake = 1, ///< No Connection: Upgrade or Upgrade: websocket header in handshake request.
    ErrorHandshakeNoVersion = 2, ///< No Sec-WebSocket-Version header in handshake request.
    ErrorHandshakeUnsupportedVersion = 3, ///< Unsupported WebSocket version requested by client.
    ErrorHandshakeNoKey = 4, ///< No Sec-WebSocket-Key header in handshake request.
    ErrorHandshakeAccept = 5, ///< No Sec-WebSocket-Accept header or wrong value.
    ErrorUnauthorized = 6, ///< The server rejected the username or password for authentication.
    ErrorPayloadTooBig = 10,  ///< Payload too big for supplied buffer.
    ErrorIncompleteFrame = 11 ///< Incomplete frame received.
};


static std::string ProtocolGuid = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

/// The WebSocket protocol version supported (13).
static std::string ProtocolVersion = "13";


//
// WebSocket Framer
//

/// This class implements a WebSocket parser according
/// to the WebSocket protocol described in RFC 6455.
class HTTP_API WebSocketFramer
{
public:
    /// Creates a Socket using the given Socket.
    WebSocketFramer(ws::Mode mode);

    virtual ~WebSocketFramer();

    /// Writes a WebSocket protocol frame from the given data.
    virtual size_t writeFrame(const char* data, size_t len, int flags, BitWriter& frame);

    /// Reads a single WebSocket frame from the given buffer (frame).
    ///
    /// The actual payload length is returned, and the beginning of the
    /// payload buffer will be assigned in the second (payload) argument.
    /// No data is copied.
    ///
    /// If the frame is invalid or too big an exception will be thrown.
    /// Return true when the handshake has completed successfully.
    virtual uint64_t readFrame(BitReader& frame, char*& payload);

    bool handshakeComplete() const;

    //
    /// Server side

    void acceptServerRequest(http::Request& request, http::Response& response);

    //
    /// Client side

    /// Sends the initial WS handshake HTTP request.
    /// void sendHandshakeRequest();

    /// Appends the WS hanshake HTTP request hearers.
    void createClientHandshakeRequest(http::Request& request);

    /// Checks the veracity the HTTP handshake response.
    /// Returns true on success, false if the request should
    /// be resent (in case of authentication), or throws on error.
    bool checkClientHandshakeResponse(http::Response& response);

    /// Verifies the handshake response or thrown and exception.
    void completeClientHandshake(http::Response& response);

protected:
    /// Returns the frame flags of the most recently received frame.
    /// Set by readFrame()
    int frameFlags() const;

    /// Returns true if the payload must be masched.
    /// Used by writeFrame()
    bool mustMaskPayload() const;

    ws::Mode mode() const;

    enum
    {
        FRAME_FLAG_MASK = 0x80,
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


class HTTP_API WebSocketAdapter : public net::SocketEmitter
{
public:
    WebSocketAdapter(const net::Socket::Ptr& socket, ws::Mode mode,
                     http::Request& request, http::Response& response);

    virtual ssize_t send(const char* data, size_t len, int flags = 0) override; // flags = ws::Text || ws::Binary
    virtual ssize_t send(const char* data, size_t len, const net::Address& peerAddr, int flags = 0) override; // flags = ws::Text || ws::Binary

    virtual bool shutdown(uint16_t statusCode, const std::string& statusMessage);

    /// Pointer to the underlying socket.
    /// Sent data will be proxied to this socket.
    net::Socket::Ptr socket;

    //
    /// Client side

    virtual void sendClientRequest();
    virtual void handleClientResponse(const MutableBuffer& buffer, const net::Address& peerAddr);
    // virtual void prepareClientRequest(http::Request& request);
    // virtual void verifyClientResponse(http::Response& response);

    //
    /// Server side

    virtual void handleServerRequest(const MutableBuffer& buffer, const net::Address& peerAddr);
    // virtual void sendConnectResponse();
    // virtual void verifyServerRequest(http::Request& request);
    // virtual void prepareClientResponse(http::Response& response);

    virtual void onSocketConnect(net::Socket& socket) override;
    virtual void onSocketRecv(net::Socket& socket, const MutableBuffer& buffer, const net::Address& peerAddress) override;
    virtual void onSocketClose(net::Socket& socket) override;

    virtual void onHandshakeComplete();

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


/// Standalone WebSocket class.
class HTTP_API WebSocket : public WebSocketAdapter
{
public:
    typedef std::vector<WebSocket> Vec;

    /// Creates the WebSocket with the given Socket.
    /// The Socket should be a TCPSocket or a SSLSocket,
    /// depending on the protocol used (ws or wss).
    WebSocket(const net::Socket::Ptr& socket);

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


/// WebSocket class which belongs to a HTTP Connection.
class HTTP_API ConnectionAdapter : public WebSocketAdapter
{
public:
    ConnectionAdapter(Connection* connection, ws::Mode mode);
    virtual ~ConnectionAdapter();

    // virtual ssize_t sendHeader();

    virtual void onHandshakeComplete();

protected:
    Connection* _connection;
};


} // namespace ws
} // namespace http
} // namespace scy


#endif // SCY_NET_WebSocket_H


/// @\}
