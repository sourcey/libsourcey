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


void raiseLoadBalancedServer()
{
    auto loop = uv::createLoop();

    http::Server srv(HttpPort, new OurServerResponderFactory, net::makeSocket<net::TCPSocket>(loop));
    srv.start();

    uv::waitForShutdown([&](void*) {
        srv.shutdown();
    }, nullptr, loop);

    uv::closeLoop(loop);
    delete loop;
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


int main(int argc, char** argv)
{
    //Logger::instance().add(new LogChannel("debug", LTrace));
    //Logger::instance().add(new ConsoleChannel("debug", LTrace));
    //Logger::instance().setWriter(new AsyncLogWriter);
    net::SSLManager::initNoVerifyServer();
    {
#if SCY_HAS_KERNEL_SOCKET_LOAD_BALANCING
        Thread t1;
        Thread t2;
        // Thread t3;
        // Thread t4;

        t1.start(std::bind(raiseBenchmarkServer));
        t2.start(std::bind(raiseBenchmarkServer));
        // t3.start(std::bind(raiseLoadBalancedServer));
        // t4.start(std::bind(raiseLoadBalancedServer));

        t1.join();
        t2.join();
        // t3.join();
        // t4.join();
#else
        raiseServer();
#endif
      // raiseServer();
      // raiseBenchmarkServer();
      // libuv::raiseLibuvEchoServer();
    }
    net::SSLManager::instance().shutdown();
    Logger::destroy();
    return 0;
}
