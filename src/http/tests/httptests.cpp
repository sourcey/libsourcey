#include "httptests.h"


using std::endl;
using namespace scy;
using namespace scy::test;


int main(int argc, char** argv)
{
    // Logger::instance().add(new ConsoleChannel("debug", LTrace));
    test::initialize();
    net::SSLManager::initNoVerifyClient();

    // Create the static callback context
    static CallbackContext context;

    //
    /// HTTP URL Tests
    //

    describe("url parser", []() {
        http::URL url;
        expect(url.scheme().empty());
        expect(url.authority().empty());
        expect(url.userInfo().empty());
        expect(url.host().empty());
        expect(url.port() == 0);
        expect(url.path().empty());
        expect(url.query().empty());
        expect(url.fragment().empty());

        http::URL url2("HTTP", "localhost", "/home/sourcey/foo.bar");
        expect(url2.scheme() == "http");
        expect(url2.host() == "localhost");
        expect(url2.path() == "/home/sourcey/foo.bar");

        http::URL url3("http", "www.sourcey.com", "/index.html");
        expect(url3.scheme() == "http");
        expect(url3.authority() == "www.sourcey.com");
        expect(url3.host() == "www.sourcey.com");
        expect(url3.path() == "/index.html");

        http::URL url4("http", "www.sourcey.com:8000", "/index.html");
        expect(url4.scheme() == "http");
        expect(url4.authority() == "www.sourcey.com:8000");
        expect(url4.host() == "www.sourcey.com");
        expect(url4.path() == "/index.html");

        http::URL url5("http", "user@www.sourcey.com:8000", "/index.html");
        expect(url5.scheme() == "http");
        expect(url5.userInfo() == "user");
        expect(url5.host() == "www.sourcey.com");
        expect(url5.port() == 8000);
        expect(url5.authority() == "user@www.sourcey.com:8000");
        expect(url5.path() == "/index.html");

        http::URL url6("http", "user@www.sourcey.com:80", "/index.html");
        expect(url6.scheme() == "http");
        expect(url6.userInfo() == "user");
        expect(url6.host() == "www.sourcey.com");
        expect(url6.port() == 80);
        expect(url6.authority() == "user@www.sourcey.com:80");
        expect(url6.path() == "/index.html");

        http::URL url7("http", "www.sourcey.com", "/index.html", "query=test", "fragment");
        expect(url7.scheme() == "http");
        expect(url7.authority() == "www.sourcey.com");
        expect(url7.path() == "/index.html");
        expect(url7.pathEtc() == "/index.html?query=test#fragment");
        expect(url7.query() == "query=test");
        expect(url7.fragment() == "fragment");

        http::URL url8("http", "www.sourcey.com", "/index.html?query=test#fragment");
        expect(url8.scheme() == "http");
        expect(url8.authority() == "www.sourcey.com");
        expect(url8.path() == "/index.html");
        expect(url8.pathEtc() == "/index.html?query=test#fragment");
        expect(url8.query() == "query=test");
        expect(url8.fragment() == "fragment");
    });

    //
    /// HTTP URL Parameters Tests
    //

    describe("url query parameters", []() {
        NVCollection params;
        http::splitURIParameters("/streaming?format=MJPEG&width=400&height=300&encoding=Base64&packetizer=chunked&rand=0.09983996045775712", params);
        for (NVCollection::ConstIterator it = params.begin(); it != params.end(); ++it) {
            DebugL << "URL Parameter: " << it->first << ": " << it->second << endl;
        }

        expect(params.get("format") == "MJPEG");
        expect(params.get("Format") == "MJPEG");
        expect(params.get("width") == "400");
        expect(params.get("WIDTH") == "400");
        expect(params.get("height") == "300");
        expect(params.get("encoding") == "Base64");
        expect(params.get("ENCODING") == "Base64");
        expect(params.get("packetizer") == "chunked");
        expect(params.get("rand") == "0.09983996045775712");
        expect(params.get("RaNd") == "0.09983996045775712");
        expect(params.get("0") == "streaming");
    });

    // //
    // /// Default HTTP Client Connection Test
    // //
    //
    // describe("client connection download", []() {
    //     auto conn = http::Client::instance().createConnection("http://anionu.com/packages/spotinstaller/download/2667/SpotInstaller.exe");
    //     conn->Complete += sdelegate(&context, &CallbackContext::onClientConnectionDownloadComplete);
    //     conn->request().setMethod("GET");
    //     conn->request().setKeepAlive(false);
    //     conn->setReadStream(new std::ofstream("SpotInstaller.exe", std::ios_base::out | std::ios_base::binary));
    //     conn->send();
    //     uv::runDefaultLoop();
    // });
    //
    // describe("secure client connection download", []() {
    //     auto conn = http::Client::instance().createConnection("https://anionu.com/assets/download/25/SpotInstaller.exe");
    //     conn->Complete += sdelegate(&context, &CallbackContext::onClientConnectionDownloadComplete);
    //     conn->request().setMethod("GET");
    //     conn->request().setKeepAlive(false);
    //     conn->setReadStream(new std::ofstream("SpotInstaller.exe", std::ios_base::out | std::ios_base::binary));
    //     conn->send();
    //     uv::runDefaultLoop();
    // });

    describe("client connection", []() {
        auto conn = http::Client::instance().createConnection("http://google.com/");
        conn->Complete += sdelegate(&context, &CallbackContext::onClientConnectionComplete);
        conn->request().setMethod("GET");
        conn->request().setKeepAlive(false);
        // conn->setReadStream(new std::stringstream);
        conn->send();
        uv::runDefaultLoop();
        expect(conn->closed());
        expect(!conn->error().any());
    });

    describe("secure client connection", []() {
        auto conn = http::Client::instance().createConnection("https://google.com/");
        conn->Complete += sdelegate(&context, &CallbackContext::onClientConnectionComplete);
        conn->request().setMethod("GET");
        conn->request().setKeepAlive(false);
        // conn->setReadStream(new std::stringstream);
        conn->send();
        uv::runDefaultLoop();
        expect(conn->closed());
        expect(!conn->error().any());
    });

    // // void runClientConnectionTest()
    // // {
    // //     HTTPEchoTest test;
    // //     test.create<ClientConnection>()->send(); // default GET request
    // //     uv::runDefaultLoop();
    // // }
    // //
    // // void runClientConnectionChunkedTest()
    // // {
    // //     HTTPEchoTest test;
    // //     auto conn = test.create<ClientConnection>(false, "127.0.0.1", TEST_HTTP_PORT);
    // //     conn->request().setKeepAlive(true);
    // //     conn->request().setURI("/chunked");
    // //     //conn->request().body << "BOUNCE" << endl;
    // //     conn->send();
    // //     uv::runDefaultLoop();
    // // }
    // //
    // // void runWebSocketSecureClientConnectionTest()
    // // {
    // //     HTTPEchoTest test;
    // //     auto conn = test.create<WebSocketSecureClientConnection>(false, "127.0.0.1", TEST_HTTPS_PORT);
    // //     conn->request().setURI("/websocket");
    // //     //conn->request().body << "BOUNCE" << endl;
    // //     conn->send();
    // //     uv::runDefaultLoop();
    // // }
    // //
    // // void runWebSocketClientConnectionTest()
    // // {
    // //     HTTPEchoTest test;
    // //     auto conn = test.create<WebSocketClientConnection>(http::URL("127.0.0.1", TEST_HTTP_PORT), false);
    // //     conn->shouldSendHead(false);
    // //     conn->request().setURI("/websocket");
    // //     //conn->request().body << "BOUNCE" << endl;
    // //     conn->send();
    // //     uv::runDefaultLoop();
    // // }

    //
    /// Standalone HTTP Client Connection Test
    //

    describe("standalone client connection", []() {
        http::ClientConnection conn("http://google.com/");
        conn.Headers += sdelegate(&context, &CallbackContext::onStandaloneHTTPClientConnectionHeaders);
        conn.Payload += sdelegate(&context, &CallbackContext::onStandaloneHTTPClientConnectionPayload);
        conn.Complete += sdelegate(&context, &CallbackContext::onStandaloneHTTPClientConnectionComplete);
        // conn.setReadStream(new std::stringstream);
        conn.send(); // send default GET /

        uv::runDefaultLoop();

        expect(conn.closed());
        expect(!conn.error().any());
    });

    //
    /// Server and Client Echo Tests
    //

    describe("websocket client and server", []() {
        HTTPEchoTest test(100);
        test.raiseServer();
        auto conn = test.createConnection("ws", "/websocket");
        conn->send("PING", 4);

        uv::runDefaultLoop();

        expect(conn->closed());
        expect(!conn->error().any());
    });

    describe("http client and server", []() {
        HTTPEchoTest test;
        test.raiseServer();
        auto conn = test.createConnection("http", "/echo");
        conn->request().setMethod("POST");
        conn->request().setContentLength(4);
        conn->send("PING", 4);

        uv::runDefaultLoop();

        expect(conn->closed());
        expect(!conn->error().any());
    });


    //
    /// Google Drive Upload Test
    //

//     describe("google drive multipart upload", []() {
//
//         // https://developers.google.com/drive/web/manage-uploads
//         // Need a current OAuth2 access_token with https://www.googleapis.com/auth/drive.file access scope for this to work
//         std::string accessToken("ya29.1.AADtN_WY53y0jEgN_SWcmfp6VvAQ6asnYqbDi5CKEfzwL7lfNqtbUiLeL4v07b_I");
//         std::string metadata("{ \"title\": \"My File\" }");
//
// #if 0
//         auto conn = http::Client::instance().createConnection("https://www.googleapis.com/drive/v2/files");
//         conn->Complete += sdelegate(&context, &CallbackContext::onAssetUploadComplete);
//         conn->OutgoingProgress += sdelegate(&context, &CallbackContext::onAssetUploadProgress);
//         conn->request().setMethod("POST");
//         conn->request().setContentType("application/json");
//         conn->request().setContentLength(2);
//         conn->request().add("Authorization", "Bearer " + accessToken);
//
//         // Send the request
//         conn->send("{}", 2);
// #endif
//
//         // Create the transaction
//         auto conn = http::Client::instance().createConnection("https://www.googleapis.com/upload/drive/v2/files?uploadType=multipart");
//         conn->request().setMethod("POST");
//         conn->request().setChunkedTransferEncoding(false);
//         conn->request().add("Authorization", "Bearer " + accessToken);
//         conn->Complete += sdelegate(&context, &CallbackContext::onAssetUploadComplete);
//         conn->OutgoingProgress += sdelegate(&context, &CallbackContext::onAssetUploadProgress);
//
//         // Attach a HTML form writer for uploading files
//         auto form = http::FormWriter::create(*conn, http::FormWriter::ENCODING_MULTIPART_RELATED);
//
//         form->addPart("metadata", new http::StringPart(metadata, "application/json; charset=UTF-8"));
//         //form->addPart("file", new http::StringPart("jew", "text/plain"));
//         //form->addPart("file", new http::FilePart("D:/test.txt", "text/plain"));
//         form->addPart("file", new http::FilePart("D:/test.jpg", "image/jpeg"));
//
//         // Send the request
//         conn->send();
//
//         uv::runDefaultLoop();
//     });

    test::runAll();

    return test::finalize();
}


// //
// /// HTTP Server Test
// //
//
//
// void runHTTPServerTest()
// {
//     http::Server srv(TEST_HTTP_PORT, new OurServerResponderFactory);
//     srv.start();
//
//     app.waitForShutdown(Tests::onKillHTTPServer, &srv);
// }
//
//
// static void onPrintHTTPServerHandle(uv_handle_t* handle, void* arg)
// {
//     //DebugL << "#### Active HTTPServer Handle: " << handle << endl;
//     DebugL << "#### Active HTTPServer Handle: " << handle << endl;
// }
//
//
// static void onKillHTTPServer(void* opaque)
// {
//     DebugL << "Kill Signal: " << opaque << endl;
//
//     // print active handles
//     uv_walk(uv::defaultLoop(), Tests::onPrintHTTPServerHandle, NULL);
//
//     reinterpret_cast<http::Server*>(opaque)->shutdown();
// }


// // ============================================================================
// // HTTP ClientConnection Test
// //
// void runHTTPEchoTest()
// {
//     DebugL << "Starting" << endl;
//
//     // Setup the transaction
//     http::Request req("GET", "http://google.com");
//     http::Response res;
//     http::ClientConnection txn(&req);
//     txn.Complete += sdelegate(this, onComplete);
//     txn.DownloadProgress += sdelegate(this, onIncomingProgress);
//     txn.send();
//
//     // Run the looop
//     app.run();
//     //util::pause();
//
//     DebugL << "Ending" << endl;
// }
//
// void onComplete(void* sender, http::Response& response)
// {
//     DebugL << "On Complete: " << &response << endl;
// }
//
// void onIncomingProgress(void* sender, http::TransferProgress& progress)
// {
//     DebugL << "On Progress: " << progress.progress() << endl;
// }
//
// struct Result {
//     int numSuccess;
//     std::string name;
//     Stopwatch sw;
//
//     void reset() {
//         numSuccess = 0;
//         sw.reset();
//     }
// };
//
// static Result Benchmark;
