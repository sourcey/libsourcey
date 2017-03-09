#ifndef TURN_UDPinitiator_TEST_H
#define TURN_UDPinitiator_TEST_H


#include "scy/logger.h"
#include "scy/time.h"
#include "scy/turn/client/udpclient.h"
#include "turnclienttest.h"

#include <iostream>


using namespace std;


namespace scy {


class UDPInitiator : public turn::ClientObserver
{
public:
    int id;
    int nFramesSent;
    int nFramesWanted;
    turn::UDPClient client;
    Timer timer;
    bool success;
    NullSignal AllocationCreated;
    Signal<void(bool)> TestComplete; // unused
    net::Address responderAddress; ///< The responder local socket address for Send indications

    UDPInitiator(int id, const turn::Client::Options& opts)
        : id(id)
        , nFramesSent(0)
        , nFramesWanted(NUM_ECHO_PACKETS)
        , client(*this, opts)
        , timer(1000, 1000)
        , success(false)
    {
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
            ErrorS(this) << id << ": " << exc.what() << std::endl;
        }
    }

    void shutdown()
    {
        client.shutdown();
        timer.stop();
    }

    void sendPacketToResponder()
    {
        if (nFramesSent++ == nFramesWanted) {
            success = true;
            TestComplete.emit(success);
            return;
        }

        DebugS(this) << id << ": Senidng to responder: " << responderAddress << endl;

        // Send large packets to test throttling
        // payload.append(65536, 'x');
        assert(responderAddress.valid());
        std::string payload(createLatencyCheck(1024));
        client.sendData(payload.c_str(), payload.length(), responderAddress);
    }

protected:
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

                // Send the intial data packet to responder 
                sendPacketToResponder();
//#ifdef TEST_INITIATOR_TO_RESPONDER
//                timer.Timeout += slot(this, &UDPInitiator::onSendTimer);
//                timer.start();
//#endif
                break;
            case turn::ClientState::Failed:
                break;
        }
    }

    void onRelayDataReceived(turn::Client&, const char* data, size_t size, const net::Address& peerAddr)
    {
#if 0 && TEST_RESPONDER_TO_INITIATOR
        std::string payload(data, size);
        payload.erase(std::remove(payload.begin(), payload.end(), 'x'), payload.end());
        if (payload.length() == 9) {
            std::uint64_t sentAt = util::strtoi<std::uint64_t>(payload);
            std::uint64_t latency = time::ticks() - sentAt;

            DebugS(this) << id << ": Received data from " << peerAddr
                         << ": payload=" << payload << ", latency=" << latency
                         << endl;
        } else
            DebugS(this) << id << ": Received dummy data from " << peerAddr
                         << ": payloadLength=" << payload.length() << endl;

        if (size < 150) {
            std::string payload(data, size);
            std::uint64_t sentAt = util::strtoi<std::uint64_t>(payload);
            std::uint64_t latency = time::ticks() - sentAt;

            DebugS(this) << id << ": Received data from " << peerAddr << ": payload=" << payload << ", latency=" << latency << endl;
        }
        else
            DebugS(this) << id << ": Received dummy data from " << peerAddr << ": size=" << size << endl;
        
        // Echo back to peer
        // client.sendData(data, size, peerAddr);
#endif
        DebugS(this) << id << ": Received response data from " << peerAddr << ": size=" << size << endl;

        // Send the intial data packet to responder 
        sendPacketToResponder();
    }

    void onAllocationCreated(turn::Client&, const stun::Transaction& transaction) 
    {
        DebugS(this) << id << ": Permissions Created" << endl;
        // AllocationCreated
    }

    void onAllocationPermissionsCreated(turn::Client&, const turn::PermissionList& permissions)
    {
        DebugS(this) << id << ": Permissions Created" << endl;
    }
};


} //  namespace scy


#endif // TURN_UDPinitiator_TEST_H