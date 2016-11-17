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
    // Logger::instance().add(new ConsoleChannel("debug", LTrace));
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
        srv.socket->unref();
        srv.start("127.0.0.1", 1337);

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
        srv.socket->unref();
        srv.start("127.0.0.1", 1338);

        net::ClientSocketTest<net::SSLSocket> test(1338);
        test.run();
        uv::runDefaultLoop();

        expect(test.passed);
    });

    test::runAll();

    return test::finalize();
}

// namespace scy {
// namespace net {
//
//
// #define TEST_SSL 1
//
//
// class Tests
// {
// public:
//     Application app;
//
//     Tests()
//     {
// #ifdef _MSC_VER
//         _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
// #endif
//         {
//
// #if TEST_SSL
//         SSLManager::initNoVerifyClient();
//
//             // Raise a SSL echo server
//             //Handle<SSLEchoServer> sslServer(new SSLEchoServer(1338, true),
//             false);
//             //sslServer->run();
// #endif
//
//             // Raise a TCP echo server
//             //Handle<TCPEchoServer> tcpServer(new TCPEchoServer(1337, true),
//             false); //true
//             //tcpServer->run();
//
//             //runAddressTest();
//             //runTCPSocketTest();
//             runUDPSocketTest();
//
// #if TEST_SSL
//             //runSSLSocketTest();
// #endif
//
// #if TEST_SSL
//             // Shutdown SSL
//             SSLManager::instance().shutdown();
// #endif
//
//             // Shutdown the garbage collector so we can free memory.
//             //GarbageCollector::instance().shutdown();
//
//             // Run the final cleanup
//             runCleanup();
//         }
//     }
//
//
//     //
//     =========================================================================
//     // UDP Socket Test
//     //
//     int UDPPacketSize;
//     int UDPNumPacketsWanted;
//     int UDPNumPacketsReceived;
//     net::Address udpServerAddr;
//     net::UDPSocket* udpClientSock;
//     /*
//     net::UDPSocket* serverSock;
//     net::Address serverBindAddr;
//     net::Address clientBindAddr;
//     net::Address clientSendAddr;
//     */
//
//     void runUDPSocketTest()
//     {
//         // Notes: Sending over home wireless network via
//         // ADSL to US server round trip stays around 200ms
//         // when sending 1450kb packets at 50ms intervals.
//         // At 40ms send intervals latency increated to around 400ms.
//
//         TraceL << "UDP Socket Test: Starting" << endl;
//
//         //UDPPacketSize = 10000;
//         UDPPacketSize = 1450;
//         UDPNumPacketsWanted = 100;
//         UDPNumPacketsReceived = 0;
//
//         //serverBindAddr.swap(net::Address("0.0.0.0", 1337));     //
//         //udpServerAddr.swap(net::Address("74.207.248.97", 1337));     //
//         //udpServerAddr.swap(net::Address("127.0.0.1", 1337));     //
//
//         //clientBindAddr.swap(net::Address("0.0.0.0", 1338));
//         //clientSendAddr.swap(net::Address("58.7.41.244", 1337));     //
//         //clientSendAddr.swap(net::Address("127.0.0.1", 1337));     //
//
//         //net::UDPSocket serverSock;
//         //serverSock.Recv += sdelegate(this, &Tests::onUDPSocketServerRecv);
//         //serverSock.bind(serverBindAddr);
//         //this->serverSock = &serverSock;
//
//         net::UDPSocket clientSock;
//         //clientSock.Recv += sdelegate(this, &Tests::onUDPClientSocketRecv);
//         expect(0 && "fixme");
//         clientSock.bind(net::Address("0.0.0.0", 0));
//         clientSock.connect(udpServerAddr);
//         this->udpClientSock = &clientSock;
//
//         //for (unsigned i = 0; i < UDPNumPacketsWanted; i++)
//         //    clientSock.send("bounce", 6, serverBindAddr);
//
//         // Start the send timer
//         Timer timer;
//         timer.Timeout += sdelegate(this, &Tests::onUDPClientSendTimer);
//         timer.start(50, 50);
//         timer.handle().ref();
//
//         uv::runDefaultLoop();
//
//         //this->serverSock = nullptr;
//         this->udpClientSock = nullptr;
//     }
//
//     /*
//     void onUDPSocketServerRecv(void* sender, net::SocketPacket& packet)
//     {
//         std::string payload(packet.data(), packet.size());
//         DebugL << "UDPSocket server recv from "
//             << packet.info->peerAddress << ": payloadLength=" <<
//             payload.length() << endl;
//
//         // Send the unix ticks milisecond for checking RTT
//         //payload.assign(util::itostr(time::ticks()));
//
//         // Relay back to the client to check RTT
//         //packet.info->socket->send(packet, packet.info->peerAddress);
//         //packet.info->socket->send(payload.c_str(), payload.length(),
//         packet.info->peerAddress);
//
//         packet.info->socket->send(payload.c_str(), payload.length(),
//         clientSendAddr);
//
//     }
//     */
//
//     void onUDPClientSendTimer(void*)
//     {
//       /*
//         std::string payload(util::itostr(time::ticks()));
//         payload.append(UDPPacketSize - payload.length(), 'x');
//         udpClientSock->send(payload.c_str(), payload.length(),
//         udpServerAddr);
//         */
//         std::string payload;
//         payload.append(UDPPacketSize, 'x');
//         udpClientSock->send(payload.c_str(), payload.length(),
//         udpServerAddr);
//     }
//
//     void onUDPClientSocketRecv(void* sender, net::SocketPacket& packet)
//     {
//         std::string payload(packet.data(), packet.size());
//
//         DebugL << "UDPSocket recv from " << packet.info->peerAddress << ": "
//             << "payload=" << payload.length()
//             << endl;
//
//       /*
//         std::string payload(packet.data(), packet.size());
//         payload.erase(std::remove(payload.begin(), payload.end(), 'x'),
//         payload.end());
//         std::uint64_t sentAt = util::strtoi<std::uint64_t>(payload);
//         std::uint64_t latency = time::ticks() - sentAt;
//
//         DebugL << "UDPSocket recv from " << packet.info->peerAddress << ": "
//             << "payload=" << payload.length() << ", "
//             << "latency=" << latency
//             << endl;
//             */
//
//
//         /*
//         UDPNumPacketsReceived++;
//         if (UDPNumPacketsReceived == UDPNumPacketsWanted) {
//
//             // Close the client socket dereferencing the main loop.
//             packet.info->socket->close();
//
//             // The server socket is still active so unref the loop once
//             // to cause the destruction of both the socket instances.
//             app.stop();
//         }
//         */
//     }
//
//     void uv::runDefaultLoop() {
//         DebugL << "#################### Running" << endl;
//         app.run();
//         DebugL << "#################### Ended" << endl;
//     }
//
//     void runCleanup() {
//         DebugL << "#################### Finalizing" << endl;
//         app.finalize();
//         DebugL << "#################### Exiting" << endl;
//     }
// };
//
//
// } // namespace net
// } // namespace scy
//
//
// int main(int argc, char** argv)
// {
//     // Logger::instance().add(new ConsoleChannel("debug", LTrace));
//     // Logger::instance().setWriter(new AsyncLogWriter);
//     // {
//     //     net::Tests run;
//     // }
//     // Logger::destroy();
//     // return 0;
// }
