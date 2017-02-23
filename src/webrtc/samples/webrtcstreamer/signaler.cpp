///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
///


#include <iostream>
#include <string>

#include "scy/webrtc/streamingpeerconnection.h"
#include "scy/webrtc/audiopacketmodule.h"
#include "scy/webrtc/multiplexmediacapturer.h"
#include "scy/webrtc/videopacketsource.h"
#include "signaler.h"

#include "webrtc/api/mediastreamtrackproxy.h"
#include "webrtc/api/peerconnectionfactory.h"


using std::endl;


namespace scy {


Signaler::Signaler(const smpl::Client::Options& options)
    : _client(options)
{
    _client.StateChange += slot(this, &Signaler::onClientStateChange);
    _client.roster().ItemAdded += slot(this, &Signaler::onPeerConnected);
    _client.roster().ItemRemoved += slot(this, &Signaler::onPeerDiconnected);
    _client += packetSlot(this, &Signaler::onPeerMessage);
    _client.connect();
}


Signaler::~Signaler()
{
}


void Signaler::sendSDP(PeerConnection* conn, const std::string& type,
                       const std::string& sdp)
{
    assert(type == "offer" || type == "answer");
    smpl::Message m;
    Json::Value desc;
    desc[kSessionDescriptionTypeName] = type;
    desc[kSessionDescriptionSdpName] = sdp;
    m[type] = desc;

    postMessage(m);

    // assert(type == "offer" || type == "answer");
    // smpl::Message m;
    // Json::Value desc;
    // desc[kSessionDescriptionTypeName] = type;
    // desc[kSessionDescriptionSdpName] = sdp;
    // m[type] = desc;
    //
    // postMessage(m);
}


void Signaler::sendCandidate(PeerConnection* conn, const std::string& mid,
                             int mlineindex, const std::string& sdp)
{
    smpl::Message m;
    Json::Value desc;
    desc[kCandidateSdpMidName] = mid;
    desc[kCandidateSdpMlineIndexName] = mlineindex;
    desc[kCandidateSdpName] = sdp;
    m["candidate"] = desc;

    postMessage(m);
}


void Signaler::onPeerConnected(smpl::Peer& peer)
{
    if (peer.id() == _client.ourID())
        return;
    DebugL << "Peer connected: " << peer.id() << endl;

    if (PeerConnectionManager::exists(peer.id())) {
        DebugL << "Peer already has session: " << peer.id() << endl;
        return;
    }

    // auto conn = new PeerConnection(this, peer.id(), PeerConnection::Offer);
    // conn->constraints().SetMandatoryReceiveAudio(false);
    // conn->constraints().SetMandatoryReceiveVideo(false);
    // conn->constraints().SetAllowDtlsSctpDataChannels();

    auto conn = new StreamingPeerConnection(this, peer.id(), "test.mp4", "");

    // Create the media stream
    rtc::scoped_refptr<webrtc::MediaStreamInterface> stream =
        conn->createMediaStream();

    conn->createConnection();
    conn->createOffer();

    PeerConnectionManager::add(peer.id(), conn);
}


void Signaler::onPeerMessage(smpl::Message& m)
{
    DebugL << "Peer message: " << m.from().toString() << endl;

    if (m.isMember("offer")) {
        assert(0 && "offer not supported");
    } else if (m.isMember("answer")) {
        recvSDP(m.from().id, m["answer"]);
    } else if (m.isMember("candidate")) {
        recvCandidate(m.from().id, m["candidate"]);
    }
    // else assert(0 && "unknown event");
}


void Signaler::onPeerDiconnected(const smpl::Peer& peer)
{
    DebugL << "Peer disconnected" << endl;

    auto conn = PeerConnectionManager::remove(peer.id());
    if (conn) {
        DebugL << "Deleting peer connection: " << peer.id() << endl;
        delete conn;
    }
}


void Signaler::onClientStateChange(void* sender, sockio::ClientState& state,
                                   const sockio::ClientState& oldState)
{
    DebugL << "Client state changed from " << oldState << " to " << state
           << endl;

    switch (state.id()) {
        case sockio::ClientState::Connecting:
            break;
        case sockio::ClientState::Connected:
            break;
        case sockio::ClientState::Online:
            break;
        case sockio::ClientState::Error:
            throw std::runtime_error("Cannot connect to Symple server. "
                                     "Did you start the demo app and the "
                                     "Symple server is running on port 4500?");
    }
}


void Signaler::onAddRemoteStream(PeerConnection* conn,
                                 webrtc::MediaStreamInterface* stream)
{
    assert(0 && "not required");
}


void Signaler::onRemoveRemoteStream(PeerConnection* conn,
                                    webrtc::MediaStreamInterface* stream)
{
    assert(0 && "not required");
}


void Signaler::postMessage(const smpl::Message& m)
{
    _ipc.push(new ipc::Action(
        std::bind(&Signaler::syncMessage, this, std::placeholders::_1),
        m.clone()));
}


void Signaler::syncMessage(const ipc::Action& action)
{
    auto m = reinterpret_cast<smpl::Message*>(action.arg);
    _client.send(*m);
    delete m;
}


} // namespace scy
