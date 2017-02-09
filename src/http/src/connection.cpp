///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup http
/// @{


#include "scy/http/connection.h"
#include "scy/http/client.h"
#include "scy/http/server.h"
#include "scy/logger.h"
#include "scy/memory.h"

#include <assert.h>


using std::endl;


namespace scy {
namespace http {


Connection::Connection(const net::TCPSocket::Ptr& socket)
    : _socket(socket)
    , _adapter(nullptr)
    //, _timeout(30 * 60 * 1000),
    , _closed(false)
    , _shouldSendHeader(true)
{
    TraceS(this) << "Create: " << _socket << endl;
}


Connection::~Connection()
{
    TraceS(this) << "Destroy" << endl;
    replaceAdapter(nullptr);
    close();
}


int Connection::send(const char* data, std::size_t len, int flags)
{
    TraceS(this) << "Send: " << len << endl;
    assert(!_closed);
    return _adapter->send(data, len, flags);
}


int Connection::sendHeader()
{
    if (!_shouldSendHeader)
        return 0;
    _shouldSendHeader = false;

    assert(outgoingHeader());
    // assert(outgoingHeader()->has("Host"));

    std::ostringstream os;
    outgoingHeader()->write(os);
    std::string head(os.str().c_str(), os.str().length());

    // Send to Socket to bypass the ConnectionAdapter
    return _socket->send(head.c_str(), head.length());
}


void Connection::close()
{
    TraceS(this) << "Close: " << _closed << endl;

    if (_closed)
        return;
    _closed = true;

    if (_socket)
        _socket->close();

    onClose();
}


void Connection::replaceAdapter(net::SocketAdapter* adapter)
{
    TraceS(this) << "Replace adapter: " << adapter << endl;

    if (_adapter) {
        TraceS(this) << "Replace adapter: Delete existing: " << _adapter << endl;
        // Outgoing.emitter.detach(_adapter);
        _socket->removeReceiver(_adapter);
        _adapter->removeReceiver(this);
        _adapter->setSender(nullptr);

        // auto oldAdapter = _adapter;
        // runOnce(_socket->loop(), [oldAdapter]() {
        //     delete oldAdapter;
        // });

        // FIXME: Remove this, get ConnectionAdapter to take shared_ptr instaed
        deleteLater<net::SocketAdapter>(_adapter);
        //_adapter->_connection = nullptr;
        _adapter = nullptr;
    }

    if (adapter) {
        // Attach ourselves to the given ConnectionAdapter
        adapter->addReceiver(this);

        // ConnectionAdapter output goes to the Socket
        adapter->setSender(_socket.get());

        // Attach the ConnectionAdapter to receive Socket callbacks
        // The adapter will process raw packets into HTTP or WebSocket
        // frames depending on the adapter type.
        _socket->addReceiver(adapter);

        // The Outgoing stream pumps data into the ConnectionAdapter,
        // which in turn proxies to the output Socket.
        // Outgoing.emitter += slot(adapter, static_cast<void (net::SocketAdapter::*)(IPacket&)>(
        //                          &net::SocketAdapter::sendPacket));

        _adapter = adapter;
    }
}


void Connection::setError(const scy::Error& err)
{
    TraceS(this) << "Set error: " << err.message << endl;

    _error = err;
}


void Connection::onSocketConnect(net::Socket& socket)
{
    TraceS(this) << "On socket connect" << endl;

    // Only for client connections
}


void Connection::onSocketRecv(net::Socket& socket, const MutableBuffer& buffer, const net::Address& peerAddress)
{
    TraceS(this) << "On socket recv" << endl;

    // Handle payload data
    onPayload(buffer);
}


void Connection::onSocketError(net::Socket& socket, const scy::Error& error)
{
    DebugS(this) << "On socket error: " << error.errorno << ": " << error.message << endl;

    if (error.errorno == UV_EOF) {
        // Close the connection when the other side does
        // close();
    } else {
        // Other errors will set the error state
        setError(error);
    }
}


void Connection::onSocketClose(net::Socket& socket)
{
    TraceS(this) << "On socket close" << endl;

    // Close the connection when the socket closes
    close();
}


Request& Connection::request()
{
    return _request;
}


Response& Connection::response()
{
    return _response;
}


net::TCPSocket::Ptr& Connection::socket()
{
    return _socket;
}


net::SocketAdapter* Connection::adapter() const
{
    return _adapter;
}


bool Connection::closed() const
{
    return _closed;
}


scy::Error Connection::error() const
{
    return _error;
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


ConnectionAdapter::ConnectionAdapter(Connection* connection, http_parser_type type)
    : SocketAdapter(connection->socket().get(), connection)
    , _connection(connection)
    , _parser(type)
{
    TraceS(this) << "Create: " << &connection << endl;
    _parser.setObserver(this);
    if (type == HTTP_REQUEST)
        _parser.setRequest(&connection->request());
    else
        _parser.setResponse(&connection->response());
}


ConnectionAdapter::~ConnectionAdapter()
{
    TraceS(this) << "Destroy: " << _connection << endl;
}


int ConnectionAdapter::send(const char* data, std::size_t len, int flags)
{
    TraceS(this) << "Send: " << len << endl;

    try {
        // Send headers on initial send
        if (_connection &&
            _connection->shouldSendHeader()) {
            int res = _connection->sendHeader();

            // The initial packet may be empty to push the headers through
            if (len == 0)
                return res;
        }

        // Other packets should not be empty
        assert(len > 0);

        // Send body / chunk
        return SocketAdapter::send(data, len, flags);
    } catch (std::exception& exc) {
        ErrorS(this) << "Send error: " << exc.what() << endl;

        // Swallow the exception, the socket error will
        // cause the connection to close on next iteration.
    }

    return -1;
}


void ConnectionAdapter::onSocketRecv(net::Socket& socket, const MutableBuffer& buf, const net::Address&)
{
    TraceS(this) << "On socket recv: " << buf.size() << endl;

    if (_parser.complete()) {
        // Buggy HTTP servers might send late data or multiple responses,
        // in which case the parser state might already be HPE_OK.
        // In this case we discard the late message and log the error here,
        // rather than complicate the app with this error handling logic.
        // This issue was noted using Webrick with Ruby 1.9.
        WarnS(this) << "Dropping late HTTP response: " << buf.str() << endl;
        return;
    }

    // Parse incoming HTTP messages
    _parser.parse(bufferCast<const char*>(buf), buf.size());
}


void ConnectionAdapter::removeReceiver(net::SocketAdapter* adapter)
{
    // Set the parent connection instance to null
    if (_connection &&
        _connection == adapter)
        _connection = nullptr;

    net::SocketAdapter::removeReceiver(adapter);
}


//
// Parser callbacks

void ConnectionAdapter::onParserHeader(const std::string& /* name */,
                                       const std::string& /* value */)
{
}


void ConnectionAdapter::onParserHeadersEnd()
{
    TraceS(this) << "On headers end: " << _parser.upgrade() << endl;

    if (_connection)
        _connection->onHeaders();

    // Set the position to the end of the headers once
    // they have been handled. Subsequent body chunks will
    // now start at the correct position.
    // _connection.incomingBuffer().position(_parser._parser.nread);
}


void ConnectionAdapter::onParserChunk(const char* buf, std::size_t len)
{
    TraceS(this) << "On parser chunk: " << len << endl;

    // Dispatch the payload
    if (_connection) {
        net::SocketAdapter::onSocketRecv(*_connection->socket().get(),
            mutableBuffer(const_cast<char*>(buf), len),
            _connection->socket()->peerAddress());
    }
}


void ConnectionAdapter::onParserError(const scy::Error& err)
{
    WarnL << "On parser error: " << err.message << endl;

#if 0
    // HACK: Handle those peski flash policy requests here
    auto base = dynamic_cast<net::TCPSocket*>(_connection.socket().get());
    if (base && std::string(base->buffer().data(), 22) == "<policy-file-request/>") {

        // Send an all access policy file by default
        // TODO: User specified flash policy
        std::string policy;

        // Add the following headers for HTTP policy response
        // policy += "HTTP/1.1 200 OK\r\nContent-Type: text/x-cross-domain-policy\r\nX-Permitted-Cross-Domain-Policies: all\r\n\r\n";
        policy += "<?xml version=\"1.0\"?><cross-domain-policy><allow-access-from domain=\"*\" to-ports=\"*\" /></cross-domain-policy>";

        TraceS(this) << "Send flash policy: " << policy << endl;
        base->send(policy.c_str(), policy.length() + 1);
    }
#endif

    // Set error and close the connection on parser error
    if (_connection) {
        _connection->setError(err.message);
        _connection->close(); // do we want to force this?
    }
}


void ConnectionAdapter::onParserEnd()
{
    TraceS(this) << "On parser end" << endl;

    if (_connection)
        _connection->onComplete();
}


Parser& ConnectionAdapter::parser()
{
    return _parser;
}


Connection* ConnectionAdapter::connection()
{
    return _connection;
}


//
// HTTP Connection Stream
//


ConnectionStream::ConnectionStream(Connection& connection)
    : _connection(connection)
{
    TraceS(this) << "Create: " << &connection << endl;

    IncomingProgress.sender = this;
    OutgoingProgress.sender = this;

    Incoming.autoStart(true);
    Outgoing.autoStart(true);

    // The Outgoing stream pumps data into the ConnectionAdapter,
    // which in turn proxies to the output Socket.
    Outgoing.emitter += slot(_connection.adapter(),
        static_cast<void (net::SocketAdapter::*)(IPacket&)>(&net::SocketAdapter::sendPacket));

    // The Incoming stream receives data from the ConnectionAdapter.
    _connection.adapter()->Recv += slot(this, &ConnectionStream::onRecv);
}


ConnectionStream::~ConnectionStream()
{
    TraceS(this) << "Destroy" << endl;

    Outgoing.close();
    Incoming.close();
}


int ConnectionStream::send(const char* data, std::size_t len, int flags)
{
    TraceS(this) << "Send: " << len << endl;

    // Send outgoing data to the stream if adapters are attached,
    // or just proxy to the connection.
    if (Outgoing.numAdapters() > 0) {
        Outgoing.write(data, len);
    } else {
        return _connection.send(data, len, flags);
    }
    return len;
}


void ConnectionStream::onRecv(net::Socket& socket, const MutableBuffer& buffer, const net::Address& peerAddress)
{
    // Push received dada onto the Incoming stream.
    Incoming.write(bufferCast<const char*>(buffer), buffer.size());
}


Connection& ConnectionStream::connection()
{
    return _connection;
}


} // namespace http
} // namespace scy


/// @\}
