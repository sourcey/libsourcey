#ifndef TURN_TCPInitiator_TEST_H
#define TURN_TCPInitiator_TEST_H


#include "scy/logger.h"
#include "scy/signal.h"
#include "scy/timer.h"
#include "scy/time.h"
#include "scy/util.h"
#include "scy/net/tcpsocket.h"
#include "scy/turn/client/tcpclient.h"

#include <iostream>


using namespace std;


namespace scy {


struct TCPInitiator : public turn::TCPClientObserver
{
    int id;
    int nFramesSent;
    int nFramesWanted;
    turn::TCPClient client;
    net::Address lastPeerAddr;
    bool success;
    NullSignal AllocationCreated;
    Signal<void(const net::Address&)> ConnectionCreated;
    Signal<void(bool)> TestComplete;


    TCPInitiator(int id, const turn::Client::Options opts)
        : id(id)
        , nFramesSent(0)
        , nFramesWanted(NUM_ECHO_PACKETS)
        , client(*this, opts)
        , success(false)
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
        try {
            client.addPermission(peerIP);
            client.addPermission("127.0.0.1");
            client.addPermission("192.168.1.1");
            client.initiate();
        } catch (std::exception& exc) {
            ErrorS(this) << id << ": Error: " << exc.what() << std::endl;
        }
    }

    void shutdown()
    {
        client.shutdown();
    }

    void sendPacketToResponder()
    {
        if (nFramesSent++ == nFramesWanted) {
            success = true;
            TestComplete.emit(success);
            return;
        }

        // Send the unix ticks milisecond for checking latency
        // std::string payload;
        // payload.append(":");
        // payload.append(util::itostr(time::ticks()));

        // Send large packets to test throttling
        // payload.append(65536, 'x');
        // payload.append(10000, 'x');

        DebugS(this) << id << ": Sending packet to responder" << endl;
        std::string payload("initiator > responder");
        client.sendData(payload.c_str(), payload.length(), lastPeerAddr);
    }

    void onClientStateChange(turn::Client&, turn::ClientState& state, const turn::ClientState&)
    {
        DebugS(this) << id << ": State change: " << state.toString() << endl;

        switch (state.id()) {
            case turn::ClientState::None:
                break;
            case turn::ClientState::Allocating:
                break;
            case turn::ClientState::Authorizing:
                break;
            case turn::ClientState::Success:
                AllocationCreated.emit();
                break;
            case turn::ClientState::Failed:
                // assert(false);
                success = false;
                TestComplete.emit(success);                 
                break;
        }
    }

    void onRelayConnectionCreated(turn::TCPClient&, const net::TCPSocket::Ptr& socket, const net::Address& peerAddr)
    {
        DebugS(this) << id << ": Connection Created: " << peerAddr << endl;
        
        // Remember the last peer
        lastPeerAddr = peerAddr;
        ConnectionCreated.emit(peerAddr);

        // Send the intial data packet to responder 
        sendPacketToResponder();
    }

    void onRelayConnectionClosed(turn::TCPClient&, const net::TCPSocket::Ptr& socket, const net::Address& peerAddr)
    {
        DebugS(this) << id << ": Connection Closed" << endl;
    }

    void onRelayDataReceived(turn::Client&, const char* data, size_t size, const net::Address& peerAddr)
    {
#if 0
        std::string payload(data, size);
        payload.erase(std::remove(payload.begin(), payload.end(), 'x'),
        payload.end());
        if (payload.length() == 8) {
            uint64_t sentAt = util::strtoi<uint64_t>(payload);
            uint64_t latency = time::ticks() - sentAt;

            DebugS(this) << id << ": Received data from " << peerAddr 
                << ": payload=" << payload << ", latency=" << latency << endl;
        }

        if (size < 150) {
            //std::string payload(data, size);
            std::string payload(data, 8); // read the first packet from joined packets
            uint64_t sentAt = util::strtoi<uint64_t>(payload);
            uint64_t latency = time::ticks() - sentAt;

            DebugS(this) << id << ": Received data from " << peerAddr 
                << ": payload=" << payload << ", latency=" << latency << endl;
        }
        else
            DebugS(this) << id << ": Received dummy data from " << peerAddr << ": size=" << size << endl;

        // Echo back to peer
        DebugS(this) << id << ": Received data from  " << peerAddr << ": " << size << endl;

        client.sendData(data, size, peerAddr);
#endif
        DebugS(this) << id << ": Data received from responder" << endl;

        sendPacketToResponder();
    }

    void onAllocationPermissionsCreated(turn::Client&, const turn::PermissionList& permissions)
    {
        DebugS(this) << id << ": Permissions Created" << endl;
    }
};


} //  namespace scy


#endif // TURN_TCPInitiator_TEST_H