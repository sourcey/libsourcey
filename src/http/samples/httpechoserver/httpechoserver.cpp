#include "httpechoserver.h"
#include "libuvechoserver.h"
#include "scy/application.h"
#include "scy/logger.h"
#include "scy/net/sslmanager.h"


using std::endl;
using namespace scy;
using namespace scy::net;


const std::uint16_t HttpPort = 1337;
const net::Address address("0.0.0.0", HttpPort);


#if 0

void raiseServer()
{
    http::Server srv(HttpPort, new OurServerResponderFactory);
    srv.start();

    uv::waitForShutdown([&](void*) {
        srv.shutdown();
    });
}

#endif


void raiseEchoServer()
{
    http::Server srv(address);
    srv.start();

    srv.Connection += [](http::ServerConnection::Ptr conn) {
        conn->Payload += [](http::ServerConnection& conn, const MutableBuffer& buffer) {
            conn.send(bufferCast<const char*>(buffer), buffer.size());
            conn.close();
        };
    };

    std::cout << "HTTP server listening on " << address << endl;
    uv::waitForShutdown();
}


void raiseBenchmarkServer()
{
    http::Server srv(address);
    srv.start();

    srv.Connection += [&](http::ServerConnection::Ptr conn) {
        conn->send("hello universe", 14);
        conn->close();
    };

    std::cout << "HTTP server listening on " << address << endl;
    uv::waitForShutdown();
}


void raiseMulticoreBenchmarkServer()
{
    auto loop = uv::createLoop();
    std::thread::id tid(std::this_thread::get_id());

    http::Server srv(address, net::makeSocket<net::TCPSocket>(loop));
    srv.start();

    srv.Connection += [&](http::ServerConnection::Ptr conn) {
        std::cout << "request on " << tid << endl;
        conn->send("hello universe", 14);
        conn->close();
    };

    uv::waitForShutdown([&](void*) {
        srv.shutdown();
    }, nullptr, loop);

    uv::closeLoop(loop);
    delete loop;
}



void runMulticoreBenchmarkServers()
{
    Thread t1;
    Thread t2;
    Thread t3;
    Thread t4;

    t1.start(std::bind(raiseMulticoreBenchmarkServer));
    t2.start(std::bind(raiseMulticoreBenchmarkServer));
    t3.start(std::bind(raiseMulticoreBenchmarkServer));
    t4.start(std::bind(raiseMulticoreBenchmarkServer));

    std::cout << "HTTP multicore server listening on " << address << endl;
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}


int main(int argc, char** argv)
{
    //Logger::instance().add(new LogChannel("debug", LTrace));
    Logger::instance().add(new ConsoleChannel("debug", LTrace));
    //Logger::instance().setWriter(new AsyncLogWriter);
    net::SSLManager::initNoVerifyServer();
    {
        // NOTE: For best performance the http server should be compiled on
        // linux kernel 3.9 or newer, and with DISABLE_LOGGING=ON.
#if SCY_HAS_KERNEL_SOCKET_LOAD_BALANCING
        runMulticoreBenchmarkServers();
#else
        raiseServer();
        // raiseBenchmarkServer();
        // libuv::raiseLibuvEchoServer();
#endif
    }
    net::SSLManager::instance().shutdown();
    Logger::destroy();
    return 0;
}
