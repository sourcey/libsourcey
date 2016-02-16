#ifndef TURN_TCPinitiator_TEST_H
#define TURN_TCPinitiator_TEST_H


#include "scy/turn/client/tcpclient.h"
#include "scy/timer.h"
#include "scy/signal.h"
#include "scy/logger.h"

#include <iostream>


using namespace std;


namespace scy {
namespace turn {


struct TCPInitiator: public TCPClientObserver
{
    int id;
    int frames;
    bool success;
    TCPClient client;
    net::Address lastPeerAddr;
    
    NullSignal AllocationCreated;
    Signal<const net::Address&>    ConnectionCreated;
    Signal<bool> TestComplete;

    TCPInitiator(int id, const Client::Options opts) : 
        id(id), client(*this, opts), success(false) 
    {
        DebugS(this) << id << ": Creating" << endl;
    }

    virtual ~TCPInitiator()
    {
        DebugS(this) << id << ": Destroying" << endl;
    }

    void initiate(const std::string& peerIP) 
    {
        DebugS(this) << id << ": Initializing" << endl;
        try    {
            client.addPermission(peerIP);
            client.addPermission("127.0.0.1");        
            client.addPermission("192.168.1.1");            
            client.initiate();
        } 
        catch (std::exception& exc) {
            ErrorS(this) << id << ": Error: " << exc.what() << std::endl;
        }
    }

    void onClientStateChange(turn::Client& client, turn::ClientState& state, const turn::ClientState&) 
    {
        DebugS(this) << id << ": State change: " << state.toString() << endl;

        switch(state.id()) {
        case ClientState::None:                
            break;
        case ClientState::Allocating:                
            break;
        case ClientState::Authorizing:
            //success = true;
            //TestComplete.emit(this, success);
            //client.terminate();
            break;
        case ClientState::Success:
            AllocationCreated.emit(this); //, *this->client
            success = true;
            //TestComplete.emit(this, success);
            //client.terminate();
            break;
        case ClientState::Failed:
            //assert(false);
            success = false;
            TestComplete.emit(this, success);
            break;
        //case ClientState::Terminated:                
        //    break;
        }
    }
    
    void onRelayConnectionCreated(TCPClient& client, const net::TCPSocket& socket, const net::Address& peerAddr) //std::uint32_t connectionID, 
    {
        DebugS(this) << id << ": Connection Created: " << peerAddr << endl;
                
        // Send the intial data packet to peer
        //client.sendData("hello peer", 10, peerAddr);

        // Remember the last peer
        lastPeerAddr = peerAddr;
        ConnectionCreated.emit(this, peerAddr);
    }

    void onRelayConnectionClosed(TCPClient& client, const net::TCPSocket& socket, const net::Address& peerAddr) 
    {
        DebugS(this) << id << ": Connection Closed" << endl;
    }

    void onRelayDataReceived(turn::Client& client, const char* data, std::size_t size, const net::Address& peerAddr)
    {        
        std::string payload(data, size);
        
        /*
        payload.erase(std::remove(payload.begin(), payload.end(), 'x'), payload.end());
        if (payload.length() == 8) {
            std::uint64_t sentAt = util::strtoi<std::uint64_t>(payload);
            std::uint64_t latency = time::ticks() - sentAt;

            DebugS(this) << id << ": Received data from " << peerAddr << ": payload=" << payload << ", latency=" << latency << endl;
        }
        */

        /*
        if (size < 150) {
            //std::string payload(data, size);
            std::string payload(data, 8); // read the first packet from joined packets
            std::uint64_t sentAt = util::strtoi<std::uint64_t>(payload);
            std::uint64_t latency = time::ticks() - sentAt;

            DebugS(this) << id << ": Received data from " << peerAddr << ": payload=" << payload << ", latency=" << latency << endl;
        }
        else
            DebugS(this) << id << ": Received dummy data from " << peerAddr << ": size=" << size << endl;
        */
        
        DebugS(this) << id << ": Received data from  " << peerAddr << ": " << std::string(data, size)  << endl;
        // Echo back to peer
        client.sendData(data, size, peerAddr);
    }
    
    void onAllocationPermissionsCreated(turn::Client& client, const turn::PermissionList& permissions)
    {
        DebugS(this) << id << ": Permissions Created" << endl;
    }
};


} } //  namespace scy::turn


#endif // TURN_TCPinitiator_TEST_H



    

    /*
    bool onConnectionAttempt(TCPClient& client, std::uint32_t connectionID, const net::Address& peerAddr) 
    { 
        DebugS(this) << "TCPInitiator: " << id << ": Connection Attempt: " << peerAddr << endl;
        return true; 
    };
    */

    /*
    void onConnectionBindError(TCPClient& client, std::uint32_t connectionID)
    {
        DebugS(this) << "TCPInitiator: " << id << ": Connection Error: " << connectionID << endl;
        if (_dataSocket) {
            _dataSocket->StateChange -= sdelegate(this, &TCPInitiator::onDataSocketStateChange);
            _dataSocket->detach(packetDelegate<TCPInitiator, RawPacket>(this, &TCPInitiator::onRawPacketReceived, 102));
            _dataSocket = NULL;
        }
    }
        
    void onRawPacketReceived(void* sender, RawPacket& packet)
    {
        //DebugS(this) << "TCPInitiator: " << id << ": Data Packet Received: " << std::string(packet.data, packet.size) << endl;
        DebugS(this) << "TCPInitiator: " << id << ": Data Packet Received: " << packet.size << endl;
    }

    void onDataSocketStateChange(void* sender, Net::ClientState& state, const Net::ClientState&)
    {
        DebugS(this) << "Connection state change: " << state.toString() << endl;    
    
        switch (state.id()) {
        case Net::ClientState::Disconnected: 
        //case Net::ClientState::Error: 
            _dataSocket = NULL;
            break;
        }
    }
    */
