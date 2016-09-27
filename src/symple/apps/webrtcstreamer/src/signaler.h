#ifndef SCY_Signaler_H
#define SCY_Signaler_H


// #include "peerconnectionmanager.h"
#include "videostreamcapturer.h"
#include "scy/application.h"
#include "scy/symple/client.h"
#include "scy/net/sslsocket.h"
#include "scy/net/sslmanager.h"
#include "scy/ipc.h"
#include "scy/util.h"

#include <iostream>
#include <string>


// using std::endl;
// using namespace scy;


namespace scy {


// This sample application imlements WebRTC media session
// signaling using the Symple protocol.
// This method makes it dead easy to view media streams
// originating from your native C++ apps in a web browser.
// The view the media stream ensure the Symple server is running
// and fire up symple/client/tests/symple.webrtc.test.html
// in your browser.


class PeerConnectionClient;


class Signaler: public Application
{
    PointerCollection<std::string, PeerConnectionClient> manager;
    ipc::SyncQueue<> ipc;
#if USE_SSL
    smpl::SSLClient client;
#else
    smpl::TCPClient client;
#endif

    // PeerConnectionManager manager;

public:
    Signaler(const smpl::Client::Options& options);
    ~Signaler();

    void broadcastSDP(const std::string& type, const std::string sdp);
    void broadcastCandidate(const std::string& mid, int mlineindex, const std::string sdp);

  protected:
    void postMessage(const smpl::Message& m);
    void syncMessage(const ipc::Action& action);

    void onPeerConnected(void*, smpl::Peer& peer);
    void onPeerMessage(void*, smpl::Message& m);
    void onPeerDiconnected(void*, const smpl::Peer& peer);

    void onClientStateChange(void* sender, sockio::ClientState& state, const sockio::ClientState& oldState);
};


} // namespace scy


#endif
