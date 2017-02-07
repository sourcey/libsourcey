#include "scy/base.h"
#include "scy/logger.h"
#include "scy/net/address.h"
#include "scy/net/sslcontext.h"
#include "scy/net/sslmanager.h"
#include "scy/net/sslsocket.h"
#include "scy/net/tcpsocket.h"
#include "scy/net/udpsocket.h"
#include "scy/test.h"
#include "scy/time.h"

#include "../samples/echoserver/tcpechoserver.h"
#include "clientsockettest.h"


using std::endl;
using namespace scy;
using namespace scy::test;


int main(int argc, char** argv)
{
    Logger::instance().add(new ConsoleChannel("debug", LTrace));
    test::initialize();

    net::SSLManager::initNoVerifyServer();
    net::SSLManager::initNoVerifyClient();

    // =========================================================================
    // Address Test
    //
    describe("address", []() {
        net::Address sa1("192.168.1.100", 100);
        expect(sa1.host() == "192.168.1.100");
        expect(sa1.port() == 100);

        net::Address sa2("192.168.1.100", "100");
        expect(sa2.host() == "192.168.1.100");
        expect(sa2.port() == 100);

        net::Address sa3("192.168.1.100", "ftp");
        expect(sa3.host() == "192.168.1.100");
        expect(sa3.port() == 21);

        net::Address sa7("192.168.2.120:88");
        expect(sa7.host() == "192.168.2.120");
        expect(sa7.port() == 88);

        net::Address sa8("[192.168.2.120]:88");
        expect(sa8.host() == "192.168.2.120");
        expect(sa8.port() == 88);

        try {
            net::Address sa3("192.168.1.100", "f00bar");
            expect(0 && "bad service name - must throw");
        } catch (std::exception&) {
        }

        try {
            net::Address sa6("192.168.2.120", "80000");
            expect(0 && "invalid port - must throw");
        } catch (std::exception&) {
        }

        try {
            net::Address sa5("192.168.2.260", 80);
            expect(0 && "invalid address - must throw");
        } catch (std::exception&) {
        }

        try {
            net::Address sa9("[192.168.2.260:", 88);
            expect(0 && "invalid address - must throw");
        } catch (std::exception&) {
        }

        try {
            net::Address sa9("[192.168.2.260]");
            expect(0 && "invalid address - must throw");
        } catch (std::exception&) {
        }
    });

    // =========================================================================
    // TCP Socket Test
    //
    describe("tcp socket test", []() {
        net::TCPEchoServer srv;
        srv.start("127.0.0.1", 1337);
        srv.socket->unref();

        net::ClientSocketTest<net::TCPSocket> test(1337);
        test.run();
        uv::runDefaultLoop();

        expect(test.passed);
    });

    // =========================================================================
    // SSL Socket Test
    //
    describe("ssl socket test", []() {
        net::SSLEchoServer srv;
        srv.start("127.0.0.1", 1338);
        srv.socket->unref();

        net::ClientSocketTest<net::SSLSocket> test(1338);
        test.run();
        uv::runDefaultLoop();

        expect(test.passed);
    });

    test::runAll();

    return test::finalize();
}
