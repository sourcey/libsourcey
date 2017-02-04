#include "httpechoserver.h"
#include "scy/application.h"
#include "scy/logger.h"
#include "scy/net/sslmanager.h"


using std::endl;
using namespace scy;
using namespace scy::net;


const std::uint16_t HttpPort = 1337;


void raiseServer()
{
    http::Server srv(HttpPort, new OurServerResponderFactory);
    srv.start();

    uv::waitForShutdown([](void* opaque) {
        auto srv = reinterpret_cast<http::Server*>(opaque);
        srv->shutdown();
    }, &srv);
}


int main(int argc, char** argv)
{
    Logger::instance().add(new ConsoleChannel("debug", LTrace));
    Logger::instance().setWriter(new AsyncLogWriter);
    net::SSLManager::initNoVerifyServer();
    {
        raiseServer();

        // TODO: Test multicore server implementation
        // https://github.com/libuv/libuv/issues/730
#if SCY_HAS_KERNEL_SOCKET_LOAD_BALANCING
    		// Thread t1;
        // Thread t2;
        // Thread t3;
        // Thread t4;

    		// t1.start(std::bind(raiseServer));
        // t2.start(std::bind(raiseServer));
        // t3.start(std::bind(raiseServer));
        // t4.start(std::bind(raiseServer));

        // t1.join();
        // t2.join();
        // t3.join();
        // t4.join();
#else
#endif
    }
    net::SSLManager::instance().shutdown();
    Logger::destroy();
    return 0;
}
