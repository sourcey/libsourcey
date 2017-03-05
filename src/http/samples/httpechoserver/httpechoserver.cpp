#include "httpechoserver.h"
#include "libuvhttpserver.h"
#include "scy/logger.h"
#include "scy/net/sslmanager.h"


using namespace scy;


int main(int argc, char** argv)
{
    Logger::instance().add(new NullChannel("null"));
    //Logger::instance().add(new ConsoleChannel("debug", LTrace));
    //Logger::instance().setWriter(new AsyncLogWriter);
    net::SSLManager::initNoVerifyServer();
    {
        //runMulticoreBenchmarkServers();
        //runMulticoreEchoServers();
        //libuv::raiseBenchmarkServer();

        // NOTE: For best performance the http server should be compiled on
        // linux kernel 3.9 or newer, and with ENABLE_LOGGING=OFF.
#if SCY_HAS_KERNEL_SOCKET_LOAD_BALANCING
        runMulticoreBenchmarkServers();
#else
        raiseBenchmarkServer();
        // raiseEchoServer();
#endif
    }
    net::SSLManager::instance().shutdown();
    Logger::destroy();
    return 0;
}
