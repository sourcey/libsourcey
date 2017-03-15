///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
///


#ifndef SCY_WebRTC_WebRTCStreamer_Signaler_H
#define SCY_WebRTC_WebRTCStreamer_Signaler_H


#include "scy/application.h"
#include "scy/ipc.h"
#include "scy/net/sslmanager.h"
#include "scy/net/sslsocket.h"
#include "scy/symple/client.h"
#include "scy/util.h"
#include "scy/webrtc/peerconnectionmanager.h"
#include "scy/webrtc/multiplexmediacapturer.h"

#include "config.h"


namespace scy {


class Signaler : public PeerConnectionManager, public Application
{
public:
    Signaler(const smpl::Client::Options& options);
    ~Signaler();

    void startStreaming(const std::string& file, bool loop = true);

protected:

    /// PeerConnectionManager interface
    void sendSDP(PeerConnection* conn, const std::string& type, const std::string& sdp);
    void sendCandidate(PeerConnection* conn, const std::string& mid, int mlineindex, const std::string& sdp);
    void onAddRemoteStream(PeerConnection* conn, webrtc::MediaStreamInterface* stream);
    void onRemoveRemoteStream(PeerConnection* conn, webrtc::MediaStreamInterface* stream);

    void postMessage(const smpl::Message& m);
    void syncMessage(const ipc::Action& action);

    void onPeerConnected(smpl::Peer& peer);
    void onPeerMessage(smpl::Message& m);
    void onPeerDiconnected(const smpl::Peer& peer);

    void onClientStateChange(void* sender, sockio::ClientState& state,
                             const sockio::ClientState& oldState);

protected:
    ipc::SyncQueue<> _ipc;
#if USE_SSL
    smpl::SSLClient _client;
#else
    smpl::TCPClient _client;
#endif
    MultiplexMediaCapturer _capturer;

    std::unique_ptr<rtc::Thread> _networkThread;
    std::unique_ptr<rtc::Thread> _workerThread;
};


} // namespace scy


#endif
