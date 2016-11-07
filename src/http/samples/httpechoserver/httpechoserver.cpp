#include "scy/application.h"
#include "scy/logger.h"
#include "scy/net/sslmanager.h"
#include "httpechoserver.h"


using std::endl;
using namespace scy;
using namespace scy::net;


const std::uint16_t HttpPort = 1337;


int main(int argc, char** argv)
{
    Logger::instance().add(new ConsoleChannel("debug", LTrace));
    // Logger::instance().setWriter(new AsyncLogWriter);
    net::SSLManager::initNoVerifyServer();
    {
        http::Server srv(HttpPort, new OurServerResponderFactory);
        srv.start();

        uv::waitForShutdown([](void* opaque) {
            auto srv = reinterpret_cast<http::Server*>(opaque);
            srv->shutdown();
        }, &srv);
    }
    net::SSLManager::instance().shutdown();
    Logger::destroy();
    return 0;
}
