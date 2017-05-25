 #include "scy/base.h"
#include "scy/logger.h"
#include "scy/net/address.h"
#include "scy/net/sslcontext.h"
#include "scy/net/sslmanager.h"
#include "scy/net/sslsocket.h"
#include "scy/net/tcpsocket.h"
#include "scy/net/udpsocket.h"
#include "scy/net/socketemitter.h"
#include "scy/test.h"
#include "scy/time.h"

#include "../samples/echoserver/tcpechoserver.h"
#include "clientsockettest.h"


using std::endl;
using namespace scy;
using namespace scy::test;


int main(int argc, char** argv)
{
    // Logger::instance().add(new ConsoleChannel("debug", LTrace));
    test::init();

    net::SSLManager::initNoVerifyServer();
    net::SSLManager::initNoVerifyClient();

    //describe("timer test", [&]() {
    //    //for (int a = 0; a < 1000; a++) {
    //    //    Idler idler;
    //    //    idler.start([&]() {
    //    //        idler.cancel();
    //    //    });
    //    //    idler.handle().ref();

    //    //    uv::runDefaultLoop();
    //    //}

    //    for (int a = 0; a < 100; a++) {
    //        Timer idler(1, 1);
    //        idler.start([&]() {
    //            std::cout << "timer" << endl;
    //            idler.stop();
    //        });
    //        idler.handle().ref();

    //        uv::runDefaultLoop();
    //    }
    //});

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
            net::Address sa4("192.168.1.100", "f00bar");
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
        srv.server->unref();

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
        srv.server->unref();

        net::ClientSocketTest<net::SSLSocket> test(1338);
        test.run();
        uv::runDefaultLoop();

        expect(test.passed);
    });


    // =========================================================================
    // DNS Resolver Test
    //
    describe("dns resolver test", []() {
        net::resolveDNS("sourcey.com", 80, [&](const net::DNSResult& dns) {
            expect(dns.success());
        });
        net::resolveDNS("thishadbetternotexizt.com", 8888, [&](const net::DNSResult& dns) {
            expect(!dns.success());
        });
        uv::runDefaultLoop();
    });


    // =========================================================================
    // TCP Socket Error Test
    //
    describe("tcp socket error test", []() {
        {
            net::TCPSocket socket;
            socket.connect("192.169.7.888", 4500); // Connection refused
            uv::runDefaultLoop();
            expect(socket.error().any());
        }
        {
            net::TCPSocket socket;
            socket.connect("hostthatdoesntexist.what", 80); // DNS resolution will fail
            uv::runDefaultLoop();
            expect(socket.error().any());
        }
    });


    // =========================================================================
    // TCP Socket Reconnection Test
    //
    describe("tcp socket reconnection test", []() {
        int connected = 0;
        net::SocketEmitter socket(std::make_shared<net::TCPSocket>());
        socket->connect("sourcey.com", 80);
        socket.Connect += [&](net::Socket& sock) {
            connected++;
            sock.close();
        };
        socket.Close += [&](net::Socket& sock) {
            // connect again on close
            if (connected == 1) {
                sock.connect("sourcey.com", 80);
            }
        };

        uv::runDefaultLoop();
        expect(connected == 2);
    });


    // =========================================================================
    // UDP Socket Reconnection Test
    //
    describe("udp socket reconnection test", []() {
        int connected = 0;
        net::SocketEmitter socket(std::make_shared<net::UDPSocket>());
        socket->connect("sourcey.com", 80);
        socket.Connect += [&](net::Socket& sock) {
            connected++;
            sock.close();
        };
        socket.Close += [&](net::Socket& sock) {
            // connect again on close
            if (connected == 1) {
                sock.connect("sourcey.com", 80);
            }
        };

        uv::runDefaultLoop();
        expect(connected == 2);
    });


    test::runAll();

    return test::finalize();
}
