///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
///


#include "signaler.h"

#include "scy/util.h"

#include <iostream>
#include <string>


using std::endl;


namespace scy {


Signaler::Signaler(const smpl::Client::Options& options)
    : PeerConnectionManager(webrtc::CreatePeerConnectionFactory())
    , _client(options)
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


void Signaler::sendSDP(PeerConnection* conn, const std::string& type, const std::string& sdp)
{
    assert(type == "offer" || type == "answer");
    smpl::Message m;
    json::value desc;
    desc[kSessionDescriptionTypeName] = type;
    desc[kSessionDescriptionSdpName] = sdp;
    m[type] = desc;

    postMessage(m);
}


void Signaler::sendCandidate(PeerConnection* conn, const std::string& mid, int mlineindex, const std::string& sdp)
{
    smpl::Message m;
    json::value desc;
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
    DebugA("Peer connected: ", peer.id())

    if (PeerConnectionManager::exists(peer.id())) {
        DebugA("Peer already has session: ", peer.id())
        return;
    }

    auto conn = new PeerConnection(this, peer.id(), "", PeerConnection::Answer);
    conn->constraints().SetMandatoryReceiveVideo(true);
    conn->constraints().SetMandatoryReceiveAudio(true);
    conn->createConnection();

    PeerConnectionManager::add(peer.id(), conn);
}


void Signaler::onPeerMessage(smpl::Message& m)
{
    DebugA("Peer message: ", m.from().toString())

    if (m.find("offer") != m.end()) {
        recvSDP(m.from().id, m["offer"]);
    } else if (m.find("answer") != m.end()) {
        assert(0 && "answer not supported");
    } else if (m.find("candidate") != m.end()) {
        recvCandidate(m.from().id, m["candidate"]);
    }
    // else assert(0 && "unknown event");
}


void Signaler::onPeerDiconnected(const smpl::Peer& peer)
{
    DebugL << "Peer disconnected" << endl;

    auto conn = get(peer.id());
    if (conn) {
        DebugA("Closing peer connection: ", peer.id())
        conn->closeConnection(); // will be deleted via callback
    }
}


void Signaler::onClientStateChange(void* sender, sockio::ClientState& state, const sockio::ClientState& oldState)
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


void Signaler::onAddRemoteStream(PeerConnection* conn, webrtc::MediaStreamInterface* stream)
{
    // TODO: StreamRecorder should be a member of PeerConnection

    av::EncoderOptions options;
    options.ofile = OUTPUT_FILENAME;
    options.oformat = OUTPUT_FORMAT;

    _recorder.reset(new StreamRecorder(options));

    auto videoTracks = stream->GetVideoTracks();
    if (!videoTracks.empty())
        _recorder->setVideoTrack(videoTracks[0]);

    auto audioTracks = stream->GetAudioTracks();
    if (!audioTracks.empty())
        _recorder->setAudioTrack(audioTracks[0]);
}


void Signaler::onRemoveRemoteStream(PeerConnection* conn, webrtc::MediaStreamInterface* stream)
{
    assert(0 && "free streams");
}


void Signaler::onStable(PeerConnection* conn)
{
}


void Signaler::onClosed(PeerConnection* conn)
{
    _recorder.reset(); // shutdown the recorder
    PeerConnectionManager::onClosed(conn);
}


void Signaler::onFailure(PeerConnection* conn, const std::string& error)
{
    _recorder.reset(); // shutdown the recorder
    PeerConnectionManager::onFailure(conn, error);
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
