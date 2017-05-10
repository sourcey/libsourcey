///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
///


#ifndef SCY_Zippy_WebRTC_Signaler_H
#define SCY_Zippy_WebRTC_Signaler_H


#include "scy/application.h"
#include "scy/ipc.h"
#include "scy/net/sslmanager.h"
#include "scy/net/sslsocket.h"
#include "scy/symple/client.h"
#include "scy/webrtc/peerconnectionmanager.h"
//#include "scy/webrtc/streamingpeerconnection.h"
//#include "scy/webrtc/recordingpeerconnection.h"


namespace scy {


class Signaler : public PeerConnectionManager, public Application
{
public:
    Signaler(const smpl::Client::Options& options);
    ~Signaler();

protected:

    /// PeerConnectionManager interface
    void sendSDP(PeerConnection* conn, const std::string& type, const std::string& sdp);
    void sendCandidate(PeerConnection* conn, const std::string& mid, int mlineindex, const std::string& sdp);
    void onAddRemoteStream(PeerConnection* conn, webrtc::MediaStreamInterface* stream);
    void onRemoveRemoteStream(PeerConnection* conn, webrtc::MediaStreamInterface* stream);
    void onStable(PeerConnection* conn);
    void onClosed(PeerConnection* conn);
    void onFailure(PeerConnection* conn, const std::string& error);

    void postMessage(const smpl::Message& m);
    void syncMessage(const ipc::Action& action);

    void onPeerConnected(smpl::Peer& peer);
    void onPeerCommand(smpl::Command& m);
    void onPeerEvent(smpl::Event& m);
    void onPeerMessage(smpl::Message& m);
    void onPeerDiconnected(const smpl::Peer& peer);

    void onClientStateChange(void* sender, sockio::ClientState& state, const sockio::ClientState& oldState);

protected:
#if USE_SSL
    smpl::SSLClient _client;
#else
    smpl::TCPClient _client;
#endif
    ipc::SyncQueue<> _ipc;
};


} // namespace scy


#endif // SCY_Zippy_WebRTC_Signaler_H
