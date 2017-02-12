#include "scy/http/server.h"


namespace scy {


const std::uint16_t HttpPort = 1337;
const net::Address address("0.0.0.0", HttpPort);


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

    std::cout << "HTTP server listening on " << address << std::endl;
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

    std::cout << "HTTP server listening on " << address << std::endl;
    uv::waitForShutdown();
}


void raiseMulticoreBenchmarkServer()
{
    auto loop = uv::createLoop();
    std::thread::id tid(std::this_thread::get_id());

    http::Server srv(address, net::makeSocket<net::TCPSocket>(loop));
    srv.start();

    srv.Connection += [&](http::ServerConnection::Ptr conn) {
        conn->send("hello universe", 14);
        conn->close();
    };

    uv::waitForShutdown([&](void*) {
        srv.shutdown();
    }, nullptr, loop);

    uv::closeLoop(loop);
    delete loop;
}


// Raise a server instance for each CPU core
void runMulticoreBenchmarkServers()
{
    std::vector<Thread*> threads;
    int ncpus = std::thread::hardware_concurrency();
    for (int i = 0; i < ncpus; ++i) {
        threads.push_back(new Thread(std::bind(raiseMulticoreBenchmarkServer)));
    }

    std::cout << "HTTP multicore(" << ncpus << ") server listening on " << address << std::endl;

    for (auto thread : threads) {
        thread->join();
    }
}


} // namespace scy
