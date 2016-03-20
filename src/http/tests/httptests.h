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


#ifndef SCY_HTTP_Tests_H
#define SCY_HTTP_Tests_H


#include "scy/base.h"
#include "scy/test.h"
#include "scy/async.h"
#include "scy/timer.h"
#include "scy/idler.h"
#include "scy/net/sslmanager.h"
#include "scy/net/sslcontext.h"
#include "scy/http/server.h"
#include "scy/http/connection.h"
#include "scy/http/client.h"
#include "scy/http/websocket.h"
#include "scy/http/packetizers.h"
#include "scy/http/form.h"
#include "scy/http/util.h"
#include "scy/http/url.h"


using std::cout;
using std::cerr;
using std::endl;
using scy::test::Test;


#define TEST_HTTP_PORT 1337
#define TEST_HTTPS_PORT 1338


namespace scy {


//
/// Generic Callback Context
//

struct CallbackContext
{
    void onClientConnectionComplete(void* sender, const http::Response& response)
    {
        auto conn = reinterpret_cast<http::ClientConnection*>(sender);

        TraceL << "Server response: "
            << response << conn->readStream<std::stringstream>()->str() << endl;
    }


    void onClientConnectionDownloadComplete(void* sender, const http::Response& response)
    {
        // auto conn = reinterpret_cast<http::ClientConnection*>(sender);

        TraceL << "Server response: " << response << endl;
    }

    void onStandaloneHTTPClientConnectionHeaders(void*, http::Response& res)
    {
        DebugL << "On response headers: " << res << endl;
    }

    //void onStandaloneHTTPClientConnectionPayload(void*, Buffer& buf)
    //{
    //    expect(0);
    //    DebugL << "On response payload: " << buf.size() << endl;
    //}

    void onStandaloneHTTPClientConnectionComplete(void* sender, const http::Response& response)
    {
        auto self = reinterpret_cast<http::ClientConnection*>(sender);
        DebugL << "On response complete"
            << response << self->readStream<std::stringstream>()->str() << endl;
        self->close();
    }

    void onAssetUploadProgress(void* sender, const double& progress)
    {
        DebugL << "Upload Progress:" << progress << endl;
    }

    void onAssetUploadComplete(void* sender, const http::Response& response)
    {
        auto conn = reinterpret_cast<http::ClientConnection*>(sender);

        DebugL << "Transaction Complete:"
            << "\n\tRequest Head: " << conn->request()
            << "\n\tResponse Head: " << response
            //<< "\n\tResponse Body: " << trans->incomingBuffer()
            << endl;

        //expect(response.success());
    }
};

//
/// HTTP Server test helpers
//


struct RandomDataSource: public Idler
{
    PacketSignal signal;

    virtual void onIdle()
    {
        RawPacket packet("hello", 5);
        signal.emit(this, packet);
    }
};


class BasicResponder: public http::ServerResponder
    /// Basic server responder (make echo?)
{
public:
    BasicResponder(http::ServerConnection& conn) :
        http::ServerResponder(conn)
    {
        DebugL << "Creating" << endl;
    }

    void onRequest(http::Request& request, http::Response& response)
    {
        DebugL << "On complete" << endl;

        response.setContentLength(14);  // headers will be auto flushed

        connection().send("hello universe", 14);
        connection().close();
    }
};


class ChunkedResponder: public http::ServerResponder
    /// Chunked responder which broadcasts random data.
{
public:
    RandomDataSource dataSource;
    bool gotHeaders;
    bool gotRequest;
    bool gotClose;

    ChunkedResponder(http::ServerConnection& conn) :
        http::ServerResponder(conn),
        gotHeaders(false),
        gotRequest(false),
        gotClose(false)
    {
        //conn.Outgoing.attach(new http::ChunkedAdapter(conn)); //"text/html"
        //conn.Outgoing.attachSource(&dataSource.signal, false);
        //dataSource.signal += sdelegate(&conn.socket(), &Socket::send);
    }

    ~ChunkedResponder()
    {
        expect(gotHeaders);
        expect(gotRequest);
        expect(gotClose);
    }

    void onHeaders(http::Request& request)
    {
        gotHeaders = true;
    }

    void onRequest(http::Request& request, http::Response& response)
    {
        gotRequest = true;

        connection().response().set("Access-Control-Allow-Origin", "*");
        connection().response().set("Content-Type", "text/html");
        connection().response().set("Transfer-Encoding", "chunked");

        // headers pushed through automatically
        //connection().sendHeader();

        // Start shooting data at the client
        //dataSource.start();
        expect(0);
    }

    void onClose()
    {
        DebugL << "On connection close" << endl;
        gotClose = true;
        dataSource.cancel();
    }
};


class WebSocketResponder: public http::ServerResponder
{
public:
    bool gotPayload;
    bool gotClose;

    WebSocketResponder(http::ServerConnection& conn) :
        http::ServerResponder(conn),
        gotPayload(false),
        gotClose(false)
    {
        DebugL << "Creating" << endl;
    }

    ~WebSocketResponder()
    {
        DebugL << "destroy" << endl;
        expect(gotPayload);
        expect(gotClose);
    }

    void onPayload(const Buffer& body)
    {
        DebugL << "On payload: " << body.size() << endl;

        gotPayload = true;

        // Enco the request back to the client
        connection().send(body.data(), body.size());
    }

    void onClose()
    {
        DebugL << "On connection close" << endl;
        gotClose = true;
    }
};


class OurServerResponderFactory: public http::ServerResponderFactory
// A Server Responder Factory for testing the HTTP server
{
public:
    http::ServerResponder* createResponder(http::ServerConnection& conn)
    {
        std::ostringstream os;
        conn.request().write(os);
        std::string headers(os.str().data(), os.str().length());
        DebugL << "Incoming Request: " << headers << endl; // remove me

        if (conn.request().getURI() == "/chunked")
            return new ChunkedResponder(conn);
        else if (conn.request().getURI() == "/websocket")
            return new WebSocketResponder(conn);
        else
            return new BasicResponder(conn);
    }
};


//
/// Socket test helpers
//

template <typename SocketT>
class SocketClientEchoTest
    /// Helper class for testing sockets (TCP, SLL, UDP)
{
public:
    SocketT socket;
    net::Address address;

    SocketClientEchoTest(const net::Address& addr) : //, bool ghost = false
        address(addr)
    {
        DebugL << "Creating: " << addr << endl;

        socket.Recv += sdelegate(this, &SocketClientEchoTest::onRecv);
        socket.Connect += sdelegate(this, &SocketClientEchoTest::onConnect);
        socket.Error += sdelegate(this, &SocketClientEchoTest::onError);
        socket.Close += sdelegate(this, &SocketClientEchoTest::onClose);
    }

    ~SocketClientEchoTest()
    {
        DebugL << "destroy" << endl;

        expect(socket.base().refCount() == 1);
    }

    void start()
    {
        // Create the socket instance on the stack.
        // When the socket is closed it will unref the main loop
        // causing the test to complete successfully.
        socket.connect(address);
        expect(socket.base().refCount() == 1);
    }

    void stop()
    {
        //socket.close();
        socket.shutdown();
    }

    void onConnect(void* sender)
    {
        DebugL << "connected" << endl;
        expect(sender == &socket);
        socket.send("client > server", 15, http::ws::SendFlags::Text);
    }

    void onRecv(void* sender, const MutableBuffer& buffer, const net::Address& peerAddress)
    {
        expect(sender == &socket);
        std::string data(buffer.cdata(), buffer.size());
        DebugL << "recv: " << data << endl;

        // Check for return packet echoing sent data
        if (data == "client > server") {
            DebugL << "got return packet" << endl;

            // Send the shutdown command to close the connection gracefully.
            // The peer disconnection will trigger an error callback which
            // will result is socket closure.
            socket.base().shutdown();
        }
        else
            expect(0 && "not echo response"); // fail...
    }

    void onError(void* sender, const Error& err)
    {
        ErrorL << "on error: " << err.message << endl;
        expect(sender == &socket);
    }

    void onClose(void* sender)
    {
        DebugL << "on close" << endl;
        expect(sender == &socket);
    }
};


//
/// HTTP Client Tests
//


struct HTTPClientTest
    /// Initializes a polymorphic HTTP client connection for
    /// testing callbacks, and also optionally raises the server.
{
    http::Server server;
    http::Client client;
    http::ClientConnection* conn;
    RandomDataSource dataSource;
    int numSuccess;

    HTTPClientTest() :
        server(TEST_HTTP_PORT, new OurServerResponderFactory),
        conn(0),
        numSuccess(0)
    {
    }

    template<class ConnectionT>
    ConnectionT* create(const http::URL& url, bool raiseServer = true)
    {
        if (raiseServer)
            server.start();

        conn = (http::ClientConnection*)client.createConnectionT<ConnectionT>(url);
        conn->Connect += sdelegate(this, &HTTPClientTest::onConnect);
        conn->Headers += sdelegate(this, &HTTPClientTest::onHeaders);
        conn->Incoming.emitter += sdelegate(this, &HTTPClientTest::onPayload); // fix syntax
        //conn->Payload += sdelegate(this, &HTTPClientTest::onPayload);
        conn->Complete += sdelegate(this, &HTTPClientTest::onComplete);
        conn->Close += sdelegate(this, &HTTPClientTest::onClose);
        return (ConnectionT*)conn;
    }

    void shutdown()
    {
        // Stop the client and server to release the loop
        server.shutdown();
        client.shutdown();
    }

    void onConnect(void*)
    {
        DebugL << "On connect" <<  endl;

        // Bounce backwards and forwards a few times :)
        //conn->write("BOUNCE", 6);

        // Start the output stream when the socket connects.
        //dataSource.conn = this->conn;
        //dataSource.start();
    }

    void onHeaders(void*, http::Response& res)
    {
        DebugL << "On headers" <<  endl;

        // Bounce backwards and forwards a few times :)
        //conn->write("BOUNCE", 6);
    }

    void onPayload(void*, IPacket& packet)
    {
        DebugL << "On payload: " << packet.size() << endl;
    }

    // void onPayload(void*, Buffer& buf)
    // {
    //     DebugL << "On response payload: " << buf << endl;
    //
    //     if (buf.toString() == "BOUNCE")
    //         numSuccess++;
    //
    //     DebugL << "On response payload: " << buf << ": " << numSuccess << endl;
    //     if (numSuccess >= 100) {
    //
    //         DebugL << "SUCCESS: " << numSuccess << endl;
    //         conn->close();
    //     }
    //     else
    //         conn->send(string("BOUNCE"), 6);
    // }

    void onComplete(void*, const http::Response& res)
    {
        std::ostringstream os;
        res.write(os);
        DebugL << "Response complete: " << os.str() << endl;
    }

    void onClose(void*)
    {
        DebugL << "Connection closed" << endl;
        shutdown();
    }
};


} // namespace scy


#endif // SCY_HTTP_Tests_H
