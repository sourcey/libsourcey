///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
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
        // assert(current <= total);
        current += nread;

        emit(progress());
    }
};


class SCY_EXTERN ConnectionAdapter;
class SCY_EXTERN Connection : public net::SocketAdapter
{
public:
    Connection(const net::Socket::Ptr& socket);
    virtual ~Connection();

    /// Send raw data to the peer.
    virtual int send(const char* data, std::size_t len, int flags = 0);

    /// Send the outdoing HTTP header.
    virtual int sendHeader();

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

    virtual void onHeaders() = 0;
    virtual void onPayload(const MutableBuffer&){};
    virtual void onMessage() = 0;
    virtual void onClose(); // not virtual

    bool shouldSendHeader() const;

    /// Set true to prevent auto-sending HTTP headers.
    void shouldSendHeader(bool flag);

    /// Assign the new ConnectionAdapter and setup the chain
    /// The flow is: Connection <-> ConnectionAdapter <-> Socket
    void replaceAdapter(net::SocketAdapter* adapter);

    /// Return the underlying socket pointer.
    net::Socket::Ptr& socket();

    /// The HTTP request headers.
    Request& request();

    /// The HTTP response headers.
    Response& response();

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

    virtual http::Message* incomingHeader() = 0;
    virtual http::Message* outgoingHeader() = 0;

    Signal<void(Connection&)> Close;

protected:
    virtual void onSocketConnect(net::Socket& socket);
    virtual void onSocketRecv(net::Socket& socket, const MutableBuffer& buffer,
                              const net::Address& peerAddress);
    virtual void onSocketError(net::Socket& socket, const scy::Error& error);
    virtual void onSocketClose(net::Socket& socket);

    /// Set the internal error.
    /// Note: Setting the error does not `close()` the connection.
    virtual void setError(const scy::Error& err);

protected:
    net::Socket::Ptr _socket;
    SocketAdapter* _adapter;
    Request _request;
    Response _response;
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

/// Default HTTP socket adapter for reading and writing HTTP messages
class SCY_EXTERN ConnectionAdapter : public ParserObserver, public net::SocketAdapter
{
public:
    ConnectionAdapter(Connection& connection, http_parser_type type);
    virtual ~ConnectionAdapter();

    virtual int send(const char* data, std::size_t len, int flags = 0);

    Parser& parser();
    Connection& connection();

protected:
    ///// SocketAdapter callbacks

    virtual void onSocketRecv(net::Socket& socket, const MutableBuffer& buffer,
                              const net::Address& peerAddress);
    // virtual void onSocketError(const Error& error);
    // virtual void onSocketClose();

    //// HTTP Parser callbacks

    virtual void onParserHeader(const std::string& name,
                                const std::string& value);
    virtual void onParserHeadersEnd();
    virtual void onParserChunk(const char* buf, std::size_t len);
    virtual void onParserError(const ParserError& err);
    virtual void onParserEnd();

    Connection& _connection;
    Parser _parser;
};


inline bool isExplicitKeepAlive(http::Message* message)
{
    const std::string& connection =
        message->get(http::Message::CONNECTION, http::Message::EMPTY);
    return !connection.empty() &&
           util::icompare(connection, http::Message::CONNECTION_KEEP_ALIVE) ==
               0;
}


} // namespace http
} // namespace scy


#endif


/// @\}
