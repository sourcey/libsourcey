#include "httpechoserver.h"
#include "libuvhttpserver.h"
#include "scy/logger.h"
#include "scy/net/sslmanager.h"


using namespace scy;


int main(int argc, char** argv)
{
// #ifdef __unix__
    // char cores_string[10];
    // sprintf(cores_string, "%d", 4); //std::thread::hardware_concurrency());
    int ncores = std::thread::hardware_concurrency();
    std::cout << "threadpool size: " << ncores << std::endl;
    setenv("UV_THREADPOOL_SIZE", std::to_string(ncores).c_str(), 1);
// #endif

    //Logger::instance().add(new NullChannel("null"));
    //Logger::instance().add(new ConsoleChannel("debug", LTrace));
    //Logger::instance().setWriter(new AsyncLogWriter);
    //net::SSLManager::initNoVerifyServer();
    {
        //raiseBenchmarkServer();
        //runMulticoreBenchmarkServers();
        //runMulticoreEchoServers();
        //rlibuv::raiseBenchmarkServer();

        // NOTE: For best performance the HTTP server should be compiled on
        // Linux kernel 3.9 or newer in RELEASE mode with ENABLE_LOGGING=OFF
// #if SCY_HAS_KERNEL_SOCKET_LOAD_BALANCING
        // runMulticoreBenchmarkServers();
// #else
        raiseBenchmarkServer();
// #endif
    }
    //net::SSLManager::instance().shutdown();
    Logger::destroy();
    return 0;
}
