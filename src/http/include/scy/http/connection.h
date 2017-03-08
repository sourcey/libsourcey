///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup http
/// @{


#ifndef SCY_HTTP_ServerConnection_H
#define SCY_HTTP_ServerConnection_H


#include "scy/http/parser.h"
#include "scy/http/request.h"
#include "scy/http/response.h"
#include "scy/http/url.h"
#include "scy/net/socketadapter.h"
#include "scy/net/tcpsocket.h"
#include "scy/packetqueue.h"
#include "scy/timer.h"


namespace scy {
namespace http {


class SCY_EXTERN ConnectionStream;
class SCY_EXTERN ConnectionAdapter;
class SCY_EXTERN Connection : public net::SocketAdapter
{
public:
    typedef std::shared_ptr<Connection> Ptr;

    Connection(const net::TCPSocket::Ptr& socket = std::make_shared<net::TCPSocket>());
    virtual ~Connection();

    virtual void onHeaders() = 0;
    virtual void onPayload(const MutableBuffer&) = 0;
    virtual void onComplete() = 0;
    virtual void onClose() = 0;

    /// Send raw data to the peer.
    virtual std::size_t send(const char* data, std::size_t len, int flags = 0);

    /// Send the outdoing HTTP header.
    virtual std::size_t sendHeader();

    /// Close the connection and schedule the object for
    /// deferred deletion.
    virtual void close();

    /// Return true if the connection is closed.
    bool closed() const;

    /// Return the error object if any.
    scy::Error error() const;

    /// Return true if the server did not give us
    /// a proper response within the allotted time.
    /// bool expired() const;

    /// Return true if headers should be automatically sent.
    bool shouldSendHeader() const;

    /// Set true to prevent auto-sending HTTP headers.
    void shouldSendHeader(bool flag);

    /// Assign the new ConnectionAdapter and setup the chain
    /// The flow is: Connection <-> ConnectionAdapter <-> Socket
    virtual void replaceAdapter(net::SocketAdapter* adapter);

    /// Return the underlying socket pointer.
    net::TCPSocket::Ptr& socket();

    /// Return the underlying adapter pointer.
    net::SocketAdapter* adapter() const;

    /// The HTTP request headers.
    Request& request();

    /// The HTTP response headers.
    Response& response();

    virtual http::Message* incomingHeader() = 0;
    virtual http::Message* outgoingHeader() = 0;

protected:
    /// Set the internal error.
    /// Note: Setting the error does not `close()` the connection.
    virtual void setError(const scy::Error& err);

    /// net::SocketAdapter interface
    virtual void onSocketConnect(net::Socket& socket);
    virtual void onSocketRecv(net::Socket& socket, const MutableBuffer& buffer, const net::Address& peerAddress);
    virtual void onSocketError(net::Socket& socket, const scy::Error& error);
    virtual void onSocketClose(net::Socket& socket);

protected:
    net::TCPSocket::Ptr _socket;
    net::SocketAdapter* _adapter;
    Request _request;
    Response _response;
    scy::Error _error;
    bool _closed;
    bool _shouldSendHeader;

    friend class ConnectionStream;
    friend class ConnectionAdapter;
};


//
// Connection Adapter
//


/// Default HTTP socket adapter for reading and writing HTTP messages
class SCY_EXTERN ConnectionAdapter : public ParserObserver, public net::SocketAdapter
{
public:
    ConnectionAdapter(Connection* connection, http_parser_type type);
    virtual ~ConnectionAdapter();

    virtual std::size_t send(const char* data, std::size_t len, int flags = 0);

    Parser& parser();
    Connection* connection();

protected:
    /// SocketAdapter interface
    virtual void onSocketRecv(net::Socket& socket, const MutableBuffer& buffer, const net::Address& peerAddress);
    // virtual void onSocketError(const Error& error);
    // virtual void onSocketClose();

    /// HTTP Parser interface
    virtual void onParserHeader(const std::string& name, const std::string& value);
    virtual void onParserHeadersEnd(bool upgrade);
    virtual void onParserChunk(const char* buf, std::size_t len);
    virtual void onParserError(const scy::Error& err);
    virtual void onParserEnd();

    Connection* _connection;
    Parser _parser;
};


//
// Progress Signal
//


// HTTP progress signal for upload and download progress notifications.
class SCY_EXTERN ProgressSignal : public Signal<void(const double&)>
{
public:
    void* sender;
    std::uint64_t current;
    std::uint64_t total;

    ProgressSignal()
        : sender(nullptr)
        , current(0)
        , total(0)
    {
    }

    double progress() const { return (current / (total * 1.0)) * 100; }

    void update(int nread)
    {
        assert(current <= total);
        current += nread;
        emit(progress());
    }
};


//
// Connection Stream
//


/// Packet stream wrapper for a HTTP connection.
class SCY_EXTERN ConnectionStream : public net::SocketAdapter
{
public:
    ConnectionStream(Connection::Ptr connection);
    virtual ~ConnectionStream();

    /// Send data via the Outgoing stream.
    std::size_t send(const char* data, std::size_t len, int flags = 0);

    /// Return a reference to the underlying connection.
    Connection::Ptr connection();

    /// The Outgoing stream is responsible for packetizing
    /// raw application data into the agreed upon HTTP
    /// format and sending it to the peer.
    PacketStream Outgoing;

    /// The Incoming stream emits incoming HTTP packets
    /// for processing by the application.
    ///
    /// This is useful for example when writing incoming data to a file.
    PacketStream Incoming;

    ProgressSignal IncomingProgress; ///< Fired on download progress
    ProgressSignal OutgoingProgress; ///< Fired on upload progress

protected:
    //void onRecv(net::Socket& socket, const MutableBuffer& buffer, const net::Address& peerAddress);
    virtual void onSocketRecv(net::Socket& socket, const MutableBuffer& buffer, const net::Address& peerAddress);

    Connection::Ptr _connection;
};


} // namespace http
} // namespace scy


#endif


/// @\}
