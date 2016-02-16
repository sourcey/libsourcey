#ifndef TURN_UDPresponder_TEST_H
#define TURN_UDPresponder_TEST_H


#include "turnclienttest.h"
#include "scy/net/udpsocket.h"
#include "scy/time.h"
#include "scy/timer.h"
#include "scy/logger.h"


using namespace std;


namespace scy {
namespace turn {


class UDPResponder: public net::SocketAdapter
{
    
public:
    int id;
    net::Address relayedAddr;
    net::UDPSocket socket;    
    Timer timer;

    UDPResponder(int id) : 
        id(id)
    {
        DebugS(this) << id << ": Creating" << endl;
        //net::SocketAdapter::socket = &socket;
        socket.addReceiver(this);

        socket.bind(net::Address("0.0.0.0", 0));
        //socket.bind(net::Address(TURN_AUTHORIZE_PEER_IP, 4020));
        
        DebugS(this) << id << ": Listening on: " << socket.address() << endl;
    }

    virtual ~UDPResponder() 
    { 
        DebugS(this) << id << ": Destroying" << endl;
        socket.addReceiver(this);
        stop(); 
    }

    void start(const net::Address& relayedAddr) 
    {        
        DebugS(this) << id << ": Starting on: " << relayedAddr << endl;
        
        try    {
            this->relayedAddr = relayedAddr;
            
            //socket.bind(net::Address("0.0.0.0", 0));
            //socket.bind(net::Address(TURN_AUTHORIZE_PEER_IP, 4020));
            //socket.bind(net::Address("0.0.0.0", 4020));
            socket.connect(relayedAddr);
        }
        catch (std::exception& exc) {
            errorL("UDPResponder", this) << id << ": ERROR: " << exc.what() << endl;
            assert(false);
        }
    }
    
    void stop() 
    {    
        timer.stop();
        socket.close();
    }    

    void onSendTimer(void*)
    {
        std::string payload(createLatencyCheck(30000));
        socket.send(payload.c_str(), payload.length());        
    }
    
    void onSocketConnect() 
    {
#if TEST_RESPONDER_TO_INITIATOR_LATENCY
        timer.Timeout += sdelegate(this, &UDPResponder::onSendTimer);
        timer.start(0, 100);
#endif
    }
    
    void onSocketRecv(const MutableBuffer& buffer, const net::Address& peerAddress) //net::SocketPacket& packet) 
    {
        std::string payload(bufferCast<const char*>(buffer), buffer.size());
        DebugS(this) << id << ": On recv: " << peerAddress << ": " << payload << std::endl;

        // Echo back to client
        socket.send(payload.c_str(), payload.size(), relayedAddr); // peerAddr
    }

    void onSocketError(const scy::Error& error) 
    {
        DebugS(this) << id << ": On error: " << error.message << std::endl;
    }

    void onSocketClose() 
    {
        DebugS(this) << id << ": On close" << std::endl;
        stop();
    }
};


} } //  namespace scy::turn


#endif // TURN_UDPresponder_TEST_H


        
    /*
    void sendLatencyCheck()
    {        
        std::string payload;
        
        // Send the unix ticks milisecond for checking latency
        payload.append(util::itostr(time::ticks()));

        // Send large packets to test throttling
        payload.append(30000, 'x'); // 1024 // 65536

        // Send it
        socket.send(payload.c_str(), payload.length());
    }
    */
    
    /*
        // Send some early media to client
        sendLatencyCheck();

    int id;
    net::Address relayedAddr;
    Net::UDPPacketSocket* socket;
    Net::Reactor& reactor;
    Runner& runner;

public:
    UDPResponder(int id): //, Net::Reactor& reactor, Runner& runner
        id(id), socket(NULL) {} //, reactor(reactor), runner(runner)
    virtual ~UDPResponder() {}

    void start(const net::Address& relayedAddr) 
    {        
        debugL() << "UDPResponder: " << id << ": Starting on: " << relayedAddr << endl;
        
        try    {
            assert(!socket);
            this->relayedAddr = relayedAddr;
            socket = new Net::UDPPacketSocket(reactor, runner);
            socket->attach(packetDelegate<UDPResponder, stun::Message>(this, &UDPResponder::onSTUNPacketReceived, 101));
            socket->attach(packetDelegate<UDPResponder, RawPacket>(this, &UDPResponder::onRawPacketReceived, 102));
            socket->connect(relayedAddr);

            Timer::getDefault().start(TimerCallback<UDPResponder>(this, &UDPResponder::onTimer, 2000, 2000));
        }
        catch (std::exception& exc) {
            Log("error") << "UDPResponder: " << id << ": ERROR: " << exc.what() << endl;
        }
    }
    
    void stop() 
    {        
        Timer::getDefault().stop(TimerCallback<UDPResponder>(this, &UDPResponder::onTimer));
        if (socket) {    
            socket->detach(packetDelegate<UDPResponder, stun::Message>(this, &UDPResponder::onSTUNPacketReceived, 101));
            socket->detach(packetDelegate<UDPResponder, RawPacket>(this, &UDPResponder::onRawPacketReceived, 102));
            delete socket;
            socket = NULL;
        }
    }
    
protected:    

    void onRelayConnectionDataReceived(turn::Client& client, const char* data, std::size_t size, const net::Address& peerAddr)
    {
        debugL() << "UDPInitiator: " << id << ": Received Data: " << std::string(data, size) << endl;

        // echo it back...
        client.sendData(data, size, peerAddr);
    }
    
    void onPermissionsCreated(turn::Client& client, const turn::PermissionList& permissions)
    {
        debugL() << "UDPInitiator: " << id << ": Permissions Created" << endl;
    }

    void onTimer(TimerCallback<UDPResponder>& timer)
    {
        debugL() << "UDPResponder: " << id << ": On Timer: Sending data to: " << relayedAddr << endl;
        assert(socket);
        socket->send("hey baby!", 9, relayedAddr);
    }
    
    void onRawPacketReceived(void* sender, RawPacket& packet)
    {
        // echoed from client...
        debugL() << "UDPResponder: " << id << ": Data Packet Received: " << packet.size() << endl;
    }

    void onSTUNPacketReceived(void* sender, stun::Message& message)
    {
        assert(false); // responder is unaware of STUN
        debugL() << "UDPResponder: " << id << ": STUN Packet Received: " << message.toString() << endl;
    }
    */

    /* //,  // << remoteAddr << endl;
        //const net::Address& localAddr, const net::Address& remoteAddr) 
     //,  // << remoteAddr << endl;//: public scy::turn::ClientObserver
        //const net::Address& localAddr, const net::Address& remoteAddr) 
protected:    
      
        
    void setRelayedAddr()
    {
    }
    virtual void onAllocationCreated(turn::Client* client, turn::Client* allocation)
    {
        debugL() << "UDPResponder: " << id << ": Allocation Created: " << transaction << endl;
        client->sendCreatePermissionRequest(_peerIP);
    }


    virtual void onAllocationRemoving(turn::Client* client, turn::Client* allocation)
    {
        debugL() << "UDPResponder: " << id << ": Allocation Deleted: " << transaction << endl;
    }


    virtual void onPermissionsCreated(turn::Client* client, turn::Client* allocation)
    {
        debugL() << "UDPResponder: " << id << ": Permissions Created: " << transaction << endl;
    }


    virtual void onDataReceived(turn::Client* client, const char* data, int size)
    {
        debugL() << "UDPResponder: " << id << ": Received Data: " << std::string(data, size) << endl;
    }
    
    virtual void onAllocationFailed(Client* client, int errorCode)
    {        
        debugL() << "UDPResponder: " << id << ": Allocation Failed: " << errorCode << endl;
    }
    
    void onTransactionResponse(Client* client, stun::Transaction* transaction)
    {        
        debugL() << "TURN Initiator: Transaction Response: " << transaction << endl;
    }    
    
    void onAllocationPermissionsCreated(Client* client, Client* allocation)
    {
        debugL() << "TURN Initiator: Permissions Created: " << allocation << endl;
    }    
    */

        /*
        try    {
            Client::Options opts;
            opts.serverAddr = scy::net::Address("127.0.0.1", 3478);
            opts.timeout = 10000;
            opts.username = "user";
            opts.password = "E94rKjRkC3nqyj8UvoYW";

            client = new Client(*this, opts);
            client->sendAllocateRequest();

            //util::pause();
        }
        catch (std::exception& exc) {
            Log("error") << "UDPResponder: " << id << ": Error: " << exc.what() << endl;
        }
        */
