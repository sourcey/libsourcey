#include "signaler.h"

#include "scy/util.h"

#include <iostream>
#include <string>


using std::endl;


namespace scy {


Signaler::Signaler(const smpl::Client::Options& options) :
    _client(options)
{
    _client.StateChange += sdelegate(this, &Signaler::onClientStateChange);
    _client.roster().ItemAdded += sdelegate(this, &Signaler::onPeerConnected);
    _client.roster().ItemRemoved += sdelegate(this, &Signaler::onPeerDiconnected);
    _client += smpl::messageDelegate(this, &Signaler::onPeerMessage);
    _client.connect();
}


Signaler::~Signaler()
{
}


void Signaler::sendSDP(const std::string& peerid, const std::string& type, const std::string& sdp)
{
    assert(type == "offer" || type == "answer");
    smpl::Message m;
    Json::Value desc;
    desc[kSessionDescriptionTypeName] = type;
    desc[kSessionDescriptionSdpName] = sdp;
    m[type] = desc;

    postMessage(m);
}


void Signaler::sendCandidate(const std::string& peerid, const std::string& mid, int mlineindex, const std::string& sdp)
{
    smpl::Message m;
    Json::Value desc;
    desc[kCandidateSdpMidName] = mid;
    desc[kCandidateSdpMlineIndexName] = mlineindex;
    desc[kCandidateSdpName] = sdp;
    m["candidate"] = desc;

    postMessage(m);
}


void Signaler::onPeerConnected(void*, smpl::Peer& peer)
{
    if (peer.id() == _client.ourID()) return;
    DebugL << "Peer connected: " << peer.id()  << endl;

    if (PeerConnectionManager::exists(peer.id())) {
        DebugL << "Peer already has session: " << peer.id() << endl;
        return;
    }

    auto conn = new PeerConnectionClient(this, peer.id(), PeerConnectionClient::Answer);
    conn->constraints().SetMandatoryReceiveVideo(false);
    conn->constraints().SetMandatoryReceiveAudio(true);
    conn->createConnection();

    PeerConnectionManager::add(peer.id(), conn);
}


void Signaler::onPeerMessage(void*, smpl::Message& m)
{
    DebugL << "Peer message: " << m.from().toString() << endl;

    if (m.isMember("offer")) {
        recvSDP(m.from().id, m["offer"]);
    }
    else if (m.isMember("answer")) {
        assert(0 && "answer not supported");
    }
    else if (m.isMember("candidate")) {
        recvCandidate(m.from().id, m["candidate"]);
    }
    // else assert(0 && "unknown event");
}


void Signaler::onPeerDiconnected(void*, const smpl::Peer& peer)
{
    DebugL << "Peer disconnected" << endl;

    auto conn = PeerConnectionManager::remove(peer.id());
    if (conn) {
        DebugL << "Deleting peer connection: " << peer.id() << endl;
        delete conn;
    }
}


void Signaler::onClientStateChange(void* sender, sockio::ClientState& state, const sockio::ClientState& oldState)
{
    DebugL << "Client state changed from " << oldState << " to " << state << endl;

    switch (state.id()) {
    case sockio::ClientState::Connecting:
        break;
    case sockio::ClientState::Connected:
        break;
    case sockio::ClientState::Online:
        break;
    case sockio::ClientState::Error:
        throw std::runtime_error("Cannot connect to Symple server. " \
            "Did you start the demo app and the Symple server is running on port 4500?");
    }
}


void Signaler::onAddRemoteStream(const std::string& peerid, webrtc::MediaStreamInterface* stream)
{
    // TODO: Should be instance on PeerConnectionClient
    _recorder.reset(new StreamRecorder());

    auto videoTracks = stream->GetVideoTracks();
    // if (videoTracks.empty()) {
    //     assert(0 && "no video tracks");
    //     return;
    // }
    if (!videoTracks.empty())
        _recorder->setVideoTrack(videoTracks[0]);

    auto audioTracks = stream->GetAudioTracks();
    assert(!audioTracks.empty());
    if (!audioTracks.empty())
        _recorder->setAudioTrack(audioTracks[0]);

    // _recorder

    // _remoteRenderer.reset(new ImageSequenceRecorder(videoTracks[0], "webrtcrecorder"));

    // Audio info here: http://stackoverflow.com/questions/24160034/webrtc-library-remote-audio-rendering-via-addsink
    // webrtc::AudioSinkInterface
    // /home/kam/sourcey/webrtcbuilds/out/src/webrtc/api/call/audio_sink.h

    // https://github.com/AnyRTC/AnyRTC-RTMP/blob/3136d0201693aade484d89e6b1639b52f4f7c6ca/AnyCore/avcodec.h
}


void Signaler::onRemoveRemoteStream(const std::string& peerid, webrtc::MediaStreamInterface* stream)
{
    assert(0 && "free streams");
}


void Signaler::postMessage(const smpl::Message& m)
{
    _ipc.push(new ipc::Action(std::bind(&Signaler::syncMessage, this, std::placeholders::_1), m.clone()));
}


void Signaler::syncMessage(const ipc::Action& action)
{
    auto m = reinterpret_cast<smpl::Message*>(action.arg);
    _client.send(*m);
    delete m;
}


} // namespace scy
