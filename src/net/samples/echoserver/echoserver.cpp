#include "scy/application.h"
#include "scy/logger.h"
#include "scy/net/sslmanager.h"
#include "tcpechoserver.h"
#include "udpechoserver.h"

using std::endl;
using namespace scy;
using namespace scy::net;

const std::uint16_t TcpPort = 1337;
const std::uint16_t SslPort = 1338;
const std::uint16_t UdpPort = 1339;

struct Servers
{
    UDPEchoServer udp;
    TCPEchoServer tcp;
    SSLEchoServer ssl;
};

static void onShutdown(void* opaque)
{
    auto srvs = reinterpret_cast<Servers*>(opaque);
    srvs->udp.shutdown();
    srvs->tcp.shutdown();
    srvs->ssl.shutdown();
}

int main(int argc, char** argv)
{
    Logger::instance().add(new ConsoleChannel("debug", LTrace));
    // Logger::instance().setWriter(new AsyncLogWriter);
    net::SSLManager::initNoVerifyServer();
    {
        Servers srvs;
        srvs.tcp.start("0.0.0.0", TcpPort);
        srvs.ssl.start("0.0.0.0", SslPort);
        srvs.udp.start("0.0.0.0", UdpPort);

        InfoL << "TCP Server listening on " << srvs.tcp.socket->address()
              << endl;
        InfoL << "SSL Server listening on " << srvs.ssl.socket->address()
              << endl;
        InfoL << "UDP Server listening on " << srvs.udp.socket->address()
              << endl;

        uv::waitForShutdown(onShutdown, &srvs);
    }
    net::SSLManager::instance().shutdown();
    Logger::destroy();
    return 0;
}
