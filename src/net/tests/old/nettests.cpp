

//#include "scy/uvpp.h"
#include "scy/base.h"
#include "scy/logger.h"
#include "scy/net/address.h"
#include "scy/net/tcpsocket.h"
#include "scy/timer.h"
#include "scy/net/reactor.h"

#include "Poco/Stopwatch.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/DNS.h"
#include "Poco/Mutex.h"

#include "SourceyReactorEchoServer.h"
//#include "SourceyAsyncReactorEchoServer.h"
#include "PocoReactorEchoServer.h"
#include "PocoEchoServer.h"
//#include "UVEchoServer.h"


//#include "bak/AsyncLoop.h"

#include "assert.h"

/*
#include "Timer.h"
#include "IP.h"
#include "Poco/RefCountedObject.h"
#include "Poco/Exception.h"
#include "Net/NetException.h"
#include "Net/SocketAddress.h"
#include "Timer.h"
#include "UDPSocket.h"
#include "NetException.h"

#include "scy/net/address.h"


#include "scy/timer.h"
#include "IOStream.h"
#include "AsyncLoop.h"
#include "uv.h"
#include "scy/timer.h"

#include "scy/net/udppacketsocket.h"
#include "scy/net/tcppacketsocket.h"
#include "scy/net/tcpserver.h"
*/


#include <Windows.h>
#include <Iphlpapi.h>

using namespace std;
using namespace Sourcey;
using namespace Sourcey::Net;

using Poco::InvalidArgumentException;


/*
using namespace Sourcey;
using namespace Sourcey::Net;
//using namespace Sourcey;

// Detect Memory Leaks
#ifdef _DEBUG
#include "MemLeakDetect/MemLeakDetect.h"
CMemLeakDetect memLeakDetect;
#endif
*/


namespace Sourcey {
    

struct BenchmarkResult {
    int numSuccess;
    std::string name;
    Poco::Stopwatch sw;
};
    
static int numRequests;
static int numIterations;
static int numInstances;
static Poco::Event ready;
static BenchmarkResult* benchmark;

    
class Tests
{
public:



    Tests()
    {    

        //testThread();
        //testAddress();
        //runTimerTest();
        //runUDPSocketTest();
        runSocketConenctionTest();
    }
    
    // ============================================================================
    //
    // Thread Test
    //
    // ============================================================================    
    
    /*
class BasicReactorSignal: public SignalBase<Reactor&, const Poco::Net::Socket&, void*, void*, ReactorDelegate>
{
public:    
    BasicReactorSignal() {};
    virtual ~BasicReactorSignal() {}

    unsigned events; // pending events
};void*,
    //
    //ReactorSignal ReactorEventSignal;    
    //ReactorSignal BasicReactorSignal;
    

    void onClientSocketConnected(ReactorEvent&)
    {
        Log("trace") << "[Tests:" << this << "] onClientSocketConnected" << std::endl;
    }

    void onBasicReactorSignal() //void*
    {
        Log("trace") << "[Tests:" << this << "] onBasicReactorSignal" << std::endl;
    }

    void testThread()
    {    
        //assert(reactorCallback(this, &Tests::onClientSocketConnected) == reactorCallback(this, &Tests::onClientSocketConnected));
        //Reactor reactor; //(333);
        //reactor.attach();
        //ReactorEventSignal += reactorCallback(this, &Tests::onClientSocketConnected);
        //BasicReactorSignal += reactorCallback(this, &Tests::onBasicReactorSignal);
        //BasicReactorSignal += sdelegate(this, &Tests::onBasicReactorSignal);
    }
*/
    

    /*
    // ============================================================================
    //
    // Thread Test
    //
    // ============================================================================    
    // A simple thread with 1s sleep
    class BTestThread : public Thread{
    public:
        void run(){
            std::cout<<"\t\t [T]I am going to Sleep"<<std::endl;
            Thread::sleep(2000);
            std::cout<<"\t\t [T]Okay I quit now"<<std::endl;
        }
    };

    // A simple thread which waits a mutex
    class MutexThread : public Thread{
    public:
        Mutex *tex;
    
        MutexThread(Mutex* in_tex){
            tex = in_tex;
        }
    
        void run(){
            std::cout<<"\t\t [T]Lock Mutex"<<std::endl;
            tex->lock();
            std::cout<<"\t\t [T]Mutex Unlocked -> exit"<<std::endl;
        }
    };

    void testThread()
    {    
        BTestThread th;    
        std::cout<<"Execute sleep-thread"<<std::endl;
        th.start();
        //std::cout<<"Wait thread to finish"<<std::endl;
        //Thread::sleep(50); // To be sure that the thread has started before I wait!
        th.wait();
        std::cout<<"Thread has Finished"<<std::endl;
    
        std::cout<<"-----------------------------------------"<<std::endl;
    
        std::cout<<"Create and lock mutex"<<std::endl;
        Mutex tex;
        tex.lock();
        MutexThread mth(&tex);
        std::cout<<"Start the second thread"<<std::endl;
        mth.start();
        std::cout<<"Sleep (the thread is wainting the mutex :)"<<std::endl;
        Thread::sleep(1000);
        std::cout<<"Release the mutex"<<std::endl;
        tex.unlock();
    }
    */

    /*
    // ============================================================================
    //
    // Timer Test
    //
    // ============================================================================
    int numTimerTicks;
    void runTimerTest() 
    {
        numTimerTicks = 5;

        Timer timer;
        timer.OnTimeout += sdelegate(this, &Tests::onOnTimerTimeout);
        timer.start(100, 100);

        RunDefaultLoop;
    }

    void onOnTimerTimeout(void* sender, std::int64_t count)
    {
        Timer* timer = static_cast<Timer*>(sender);

        Log("trace") << "[Tests:" << this << "] Timer: " << count << std::endl;

        if (count == numTimerTicks)
            timer->stop(); // event loop will be released
    }
    */
    
    
    /*
    // ============================================================================
    //
    // UDP Socket Test
    //
    // ============================================================================
    void runUDPSocketTest() 
    {
        Log("trace") << "[Tests:" << this << "] runSocketConenctionTest" << std::endl;
            
        Net::Address addr("127.0.0.1", 1337);

        Net::UDPSocket serverSock;
        serverSock.OnSend += sdelegate(this, &Tests::onUDPSocketServerSend);
        serverSock.OnRecv += sdelegate(this, &Tests::onUDPSocketServerRecv);
        serverSock.bind(addr);

        Net::UDPSocket clientSock;
        clientSock.OnSend += sdelegate(this, &Tests::onUDPSocketClientSend);
        clientSock.OnRecv += sdelegate(this, &Tests::onUDPSocketClientRecv);
        
        clientSock.send("somedata", 8, addr);
        clientSock.send("somedata", 8, addr);
        clientSock.send("somedata", 8, addr);
        
        RunDefaultLoop;
    }
    

    // ----------------------------------------------------------------------------
    //    
    void onUDPSocketClientSend(void* sender, const char* data, int size, const Net::Address& peerAddr)
    {
        Log("trace") << "[Tests:" << this << "] UDPSocket Client Send: " << string(data, size) << ": " << peerAddr << std::endl;
    }
    
    void onUDPSocketClientRecv(void* sender, const char* data, int size, const Net::Address& peerAddr)
    {
        Log("trace") << "[Tests:" << this << "] UDPSocket Client Recv: " << string(data, size) << ": " << peerAddr << std::endl;
    }
    

    // ----------------------------------------------------------------------------
    //    
    void onUDPSocketServerSend(void* sender, const char* data, int size, const Net::Address& peerAddr)
    {
        Log("trace") << "[Tests:" << this << "] UDPSocket Server Send: " << string(data, size) << ": " << peerAddr << std::endl;
    }
    
    void onUDPSocketServerRecv(void* sender, const char* data, int size, const Net::Address& peerAddr)
    {
        Log("trace") << "[Tests:" << this << "] UDPSocket Server Recv: " << string(data, size) << ": " << peerAddr << std::endl;
    }
    */
    
    // ============================================================================
    //
    // Socket Conenction Test
    //
    // ============================================================================
    
    //static int numRequestsSent;
    //static int numRequestsReceived;

    //ReactorSignal ReactorEventSignal;    
    //ReactorSignal BasicReactorSignal;
    

    void onClientSocketConnected()
    {
        //reactor.detach(sock, reactorCallback(this, &Tests::onClientSocketConnected, SocketWritable));
    }


    void runSocketConenctionTest() 
    {
        Log("trace") << "[Tests:" << this << "] runPacketSignalReceivers" << std::endl;
        
    //AsyncReactor areactor;
    Reactor reactor;
    Poco::Net::StreamSocket sock;

        reactor.attach(sock, reactorCallback(this, &Tests::onClientSocketConnected, SocketWritable));
        //reactor.attach(sock, reactorCallback(this, &Tests::onClientSocketConnected, SocketWritable));
        //reactor.attach(sock, reactorCallback(this, &Tests::onClientSocketConnected, SocketWritable));
        reactor.detach(sock, reactorCallback(this, &Tests::onClientSocketConnected, SocketWritable));
        reactor.detach(sock, reactorCallback(this, &Tests::onClientSocketConnected, SocketWritable));
        reactor.detach(sock, reactorCallback(this, &Tests::onClientSocketConnected, SocketWritable));
    
        system("pause");
        return;

        numInstances = 60;
        numIterations = 1;
        
        short port = 1333;
        Net::TCPSocket* socket = NULL;
        Poco::Runnable* server = NULL;
        vector<BenchmarkResult*> results;
        
            
        /*        
        reactor.attach(sock, reactorCallback(this, &Tests::onClientSocketConnected, SocketWritable));
        //reactor.attach(sock, reactorCallback(this, &Tests::onClientSocketConnected, SocketWritable));
        //reactor.attach(sock, reactorCallback(this, &Tests::onClientSocketConnected, SocketWritable));
            
        sock.connect(Poco::Net::SocketAddress("google.com", 80));
        

        reactor.attach();
        _reactor.attach(_impl, Poco::Observer<TCPSocket, ReadableNotification>(*this, &TCPSocket::onReadable));
        _reactor.attach(_impl, Poco::Observer<TCPSocket, WritableNotification>(*this, &TCPSocket::onWritable));
        _reactor.attach(_impl, Poco::Observer<TCPSocket, ErrorNotification>(*this, &TCPSocket::onError));    

        */

        for (unsigned n = 0; n < 1; n++)
        {    

            if (socket)
                delete socket;
            if (server)
                delete server;
        
            benchmark = new BenchmarkResult;
            benchmark->numSuccess = 0;
            results.push_back(benchmark);

            Timer::getDefault().start(TimerCallback<Tests>(this, &Tests::onTimeout, 8000));
            
            //server = new SourceyAsyncReactorEchoServer(port, areactor);
            //benchmark->name = "SourceyReactorEchoServer";
            

            server = new SourceyReactorEchoServer(port, reactor);
            benchmark->name = "SourceyReactorEchoServer";
            // raise server
            //if (n == 0) {
            //server = new PocoReactorEchoServer(port);
            //benchmark->name = "PocoReactorEchoServer";
            //} else if (n == 1) {
            //server = new PocoEchoServer(port);
            //benchmark->name = "PocoEchoServer";
            //} else if (n == 2) {
            //server = new UVEchoServer(port); // 1562500 microsecs
            //benchmark->name = "UVEchoServer";
            //}            
            //
                
            //AsyncLoop run;

            Log("debug") << "[Tests:" << this << "] connecting... " << endl;    
        
            for (unsigned n = 0; n < numInstances; n++)
            {    
                //numRequests
                socket = new Net::TCPSocket(reactor);
                socket->StateChanged += sdelegate(this, &Tests::onClientSocketStateChanged);                
                socket->DataReceived += sdelegate(this, &Tests::onClientSocketResponseReceived);
                //socket->StateChanged += sdelegate(this, &Tests::onClientSocketStateChanged);
                //reactor.attach(socket->impl(), reactorCallback(this, &Tests::onClientSocketConnected, SocketWritable));
                //socket->OnConnected += sdelegate(this, &Tests::onClientSocketConnected);
                //socket->OnRead += sdelegate(this, &Tests::onClientSocketResponseReceived);
                try 
                {
                    socket->connect(Poco::Net::SocketAddress("127.0.0.1", port));
                    //socket->connect(Poco::Net::SocketAddress("google.com", 80));    
                }
                catch (Poco::Net::NetException& exc)
                {
                    Log("error") << "[Tests:" << this << "] Net Error: " << exc.displayText() << endl;
                    benchmark->numSuccess++;
                    //delete socket;
                    //throw exc;
                }
                //Sleep(100);
            }

            ready.wait();

            //assert(false);
            
            // wait for success or timeout
            //benchmark->sw.start();

            benchmark->sw.start();
            //RunDefaultLoop;
            //delete socket;
            delete server;

            Timer::getDefault().stop(TimerCallback<Tests>(this, &Tests::onTimeout, 3000));

            // print stats
        }
        
        for (unsigned i = 0; i < results.size(); i++) {
            Log("info") << "[Tests:" << this << "] Benchamer Result:" 
                    << "\n\tName: " << benchmark->name
                    //<< "\n\tnumEchoServiceHandler: " << numEchoServiceHandler
                    //<< "\n\tnumReadableNotification: " << numReadableNotification
                    << "\n\tSuccessful: " << benchmark->numSuccess
                    << "\n\tElapsed: " << benchmark->sw.elapsed()
                    << endl;
        }
        
        /*
            socket->send("hitme", 5);
            
        // raise server

        socket->connect(Net::Address("127.0.0.1", 1337));        
        for (unsigned i = 0; i < numIterations; i++) {
            socket->send("hitme", 5);
        }

        system("pause");
        socket->close();

        // print stats
        
        system("pause");
        
        // raise server

        socket->connect(Net::Address("127.0.0.1", 1337));        
        for (unsigned i = 0; i < numIterations; i++) {
            socket->send("hitme", 5);
        }        
        
        system("pause");
        socket->close();

        // print stats
        
        system("pause");
        */
    }


    void onTimeout(TimerCallback<Tests>& timer)
    {
        // took too long...
        ready.set();
    }        
    
    
    void onClientSocketStateChanged(void* sender, Net::ConnectionState& state, const Net::ConnectionState&)
    {
        Log("debug") << "[Tests:" << this << "] Server Connection State Changed: " << state.toString() << endl;    
        Net::TCPSocket* socket = reinterpret_cast<Net::TCPSocket*>(sender);    
    
        switch (state.id()) {
        case Net::ConnectionState::Connected:
            socket->send("hey", 3);
            break;
        
        case Net::ConnectionState::Disconnected: 
        case Net::ConnectionState::Error:
            //socket->unbindEvents();
            //assert(false);
            delete socket;
            break;
        }
    }
    
    void onClientSocketResponseReceived(void* sender, Buffer&) 
    {        
        Net::TCPSocket* socket = reinterpret_cast<Net::TCPSocket*>(sender);    
        
        //throw;

        benchmark->numSuccess++;
        Log("debug") << "[Tests:" << socket << "] !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Response Received: " << benchmark->numSuccess << "/" << numInstances << endl;    //: " << " << string(data, size)
        if (benchmark->numSuccess == numInstances) {
            //uv_unref(DefaultLoop);
            //delete socket;            
            ready.set();
        }
    }
    
    
        /*
        if (status != 0)
            return;

        Net::TCPSocket* socket = reinterpret_cast<Net::TCPSocket*>(sender);    
        for (unsigned i = 0; i < numIterations; i++) {
            socket->send("hitme", 5);
        }    

    void onClientSocketConnected(ReactorEvent& event)
    {
        Log("trace") << "[Tests:" << this << "] onClientSocketConnected" << std::endl;
        //throw;
        //reactor.detach(event.socket, reactorCallback(this, &Tests::onClientSocketConnected, SocketWritable));
        //reactor.attach(event.socket, reactorCallback(this, &Tests::onClientSocketResponseReceived, SocketReadable));
        //event.socket.impl()->sendBytes("hitme", 5);
    }


    void onClientSocketResponseReceived(ReactorEvent& event)
    {
        //Log("trace") << "[Tests:" << this << "] onClientSocketResponseReceived" << std::endl;
        //assert(0);
        reactor.detach(event.socket, reactorCallback(this, &Tests::onClientSocketResponseReceived, SocketReadable));
        
        benchmark->numSuccess++;
        Log("debug") << "[Tests:" << socket << "] Response Received: " << benchmark->numSuccess << "/" << numInstances << endl;    //: " << " << string(data, size)
        if (benchmark->numSuccess == numInstances) {
            //uv_unref(DefaultLoop);
            //delete socket;            
            ready.set();
        }
    }

    void onClientSocketConnected(void* sender, int status) 
    {
        if (status != 0)
            return;

        Net::TCPSocket* socket = reinterpret_cast<Net::TCPSocket*>(sender);    
        for (unsigned i = 0; i < numIterations; i++) {
            socket->send("hitme", 5);
        }    
    }
        */

    
        /*
    void onClientSocketResponseReceived(void* sender, char* data, int size) 
    {
        Net::TCPSocket* socket = reinterpret_cast<Net::TCPSocket*>(sender);    

        benchmark->numSuccess++;
        Log("debug") << "[Tests:" << socket << "] Response Received: " << benchmark->numSuccess << "/" << numInstances << ": " << string(data, size) << endl;    
        if (benchmark->numSuccess == numInstances) {
            //uv_unref(DefaultLoop);
            //delete socket;            
            ready.set();
        }
    }
        */
    
        //uv_unref(DefaultLoop);
        //RunDefaultLoop;

        /*
        Log("info") << "[Tests:" << this << "] On Completion Timer:" 
                << "\n\tSent: " << numRequestsSent
                << "\n\tRequests: " << numRequestsReceived
                << "\n\tResponses: " << numResponsesReceived
                << endl;
        
        //assert(numRequestsSent == numRequestsReceived);
        //assert(numRequestsSent == numResponsesReceived);
        */
    /*    
    
    void onTimer(TimerCallback<Tests>& timer)
    {
        socket->send("grr", 3);
    }

    // ----------------------------------------------------------------------------
    //

    void onClientSocketResponseReceived(void* sender, DataPacket& packet) 
    {
        Net::TCPPacketSocket* socket = reinterpret_cast<Net::TCPPacketSocket*>(sender);    

        Log("debug") << "[Tests:" << this << "] Response Received: " << packet.size << ": " << socket->peerAddr() << endl;    
        socket->removeReceiver(packetDelegate<Tests, DataPacket>(this, &Tests::onClientSocketResponseReceived));
        numSuccess++;
        if (numSuccess == numIterations) 

    }

    void onClientSocketStateChanged(void* sender, Net::ConnectionState& state, const Net::ConnectionState&)
    {
        Log("debug") << "[Tests:" << this << "] Client Connection State Changed: " << state.toString() << endl;    

        Net::TCPPacketSocket* socket = reinterpret_cast<Net::TCPPacketSocket*>(sender);    
    
        switch (state.id()) {
        case Net::ConnectionState::Connected:

            // send some data when we are writable
            socket->send("hey", 3);
            break;
        
        case Net::ConnectionState::Disconnected: 
        case Net::ConnectionState::Error:
            assert(false);
            break;
        }
    }

    void onClientSocketResponseReceived(void* sender, DataPacket& packet) 
    {
        Net::TCPPacketSocket* socket = reinterpret_cast<Net::TCPPacketSocket*>(sender);    

        Log("debug") << "[Tests:" << this << "] Response Received: " << packet.size << ": " << socket->peerAddr() << endl;    
        socket->removeReceiver(packetDelegate<Tests, DataPacket>(this, &Tests::onClientSocketResponseReceived));
        numResponsesReceived++;
    }
    
    // ----------------------------------------------------------------------------
    //
    void onServerSocketallocated(void* sender, Net::TCPPacketSocket* socket)
    {
        Log("debug") << "[Tests:" << this << "] TCP Socket allocated: " << socket->peerAddr() << endl;
        socket->StateChanged += sdelegate(this, &Tests::onServerSocketStateChanged);
        socket->addReceiver(packetDelegate<Tests, DataPacket>(this, &Tests::onServerSocketRequestReceived));    
    }
    
    void onServerSocketStateChanged(void* sender, Net::ConnectionState& state, const Net::ConnectionState&)
    {
        Log("debug") << "[Tests:" << this << "] Server Connection State Changed: " << state.toString() << endl;    

        Net::TCPPacketSocket* socket = reinterpret_cast<Net::TCPPacketSocket*>(sender);    
    
        switch (state.id()) {
        case Net::ConnectionState::Connected:
            //socket->send("hey", 3);
            break;
        
        case Net::ConnectionState::Disconnected: 
        case Net::ConnectionState::Error:
            assert(false);
            break;
        }
    }

    void onServerSocketRequestReceived(void* sender, DataPacket& packet) 
    {
        Net::TCPPacketSocket* socket = reinterpret_cast<Net::TCPPacketSocket*>(sender);    

        Log("debug") << "[Tests:" << this << "] Packet Received: " << packet.size << ": " << socket->peerAddr() << endl;    
        socket->removeReceiver(packetDelegate<Tests, DataPacket>(this, &Tests::onServerSocketRequestReceived));
        numRequestsReceived++;

        // echo it back
        socket->send(packet);
    }
    */

};
        
        /*
        Net::Reactor reactor;
        Net::TCPPacketServer server(reactor);
        server.bind(Net::Address("127.0.0.1", 1337));
        server.Socketallocated += sdelegate(this, &Tests::onServerSocketallocated);
        //EchoServerThread s;
        //EchoServer s(4000);        
        //system("pause");        

        Timer::getDefault().start(TimerCallback<Tests>(this, &Tests::onTimeout, 5000));
        
        numRequestsSent = 1;
        numRequestsReceived = 0;
        numResponsesReceived = 0;
        for (unsigned i = 0; i < numRequestsSent; i++) {    
            Net::TCPClientSocket* socket = new Net::TCPClientSocket();
            socket->connect(Net::Address("127.0.0.1", 1337));
            //socket->connect(Net::Address("127.0.0.1", 9977)); //
            Net::TCPPacketSocket* socket = new Net::TCPPacketSocket();
            socket->connect(Net::Address("127.0.0.1", 1337)); //99774000
            //socket->connect(Net::Address("127.0.0.1", 9977)); //
            socket->addReceiver(packetDelegate<Tests, DataPacket>(this, &Tests::onClientSocketResponseReceived));        
            socket->StateChanged += sdelegate(this, &Tests::onClientSocketStateChanged);
            //socket->close();
            //delete socket;
            Sleep(10);
        }    
        Net::TCPClientSocket* socket = new Net::TCPClientSocket();
        socket->connect(Net::Address("127.0.0.1", 1337));
        for (unsigned i = 0; i < 100; i++) {    
            socket->send("grr", 3);
        }
        
        system("pause");        

        //Net::AsyncLoop reactor;

        //for (unsigned i = 0; i < 100; i++) {    
        //    socket->send("grr", 3);
        //}
        
        Net::TCPSocket server;
        server.bind(Net::Address("127.0.0.1", 1337));
        server.listen();
        */


} // namespace Sourcey


int main(int argc, char** argv) 
{    
    //uv_tcp_t handle_;
    //Logger::instance().add(new ConsoleChannel("debug", TraceLevel));
    Logger::instance().add(new FileChannel("debug", "", "log", NULL, 12 * 3600, TraceLevel)); //d:/"%H:%M:%S"
    {

        Sourcey::Tests app;
        //app.run(argc, argv);
        //Sourcey::TURN::RelayServer app;
        //app.run(); //argc, argv
    }
    
    system("pause");
    Sourcey::Logger::destroy();
    return 0;
}