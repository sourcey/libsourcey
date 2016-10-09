#include "signaler.h"

#include "scy/webrtc/opencvvideocapturer.h"

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

    auto conn = new PeerConnectionClient(this, peer.id(), PeerConnectionClient::Offer);
    conn->constraints().SetMandatoryReceiveAudio(false);
    conn->constraints().SetMandatoryReceiveVideo(false);
    conn->constraints().SetAllowDtlsSctpDataChannels();

    // Create the media stream and tracks
    rtc::scoped_refptr<webrtc::MediaStreamInterface> stream = conn->createMediaStream();
    rtc::scoped_refptr<webrtc::VideoTrackInterface> videoTrack(
        _factory->CreateVideoTrack(kVideoLabel,
            _factory->CreateVideoSource(new OpenCVVideoCapturer(0), NULL)));
    rtc::scoped_refptr<webrtc::AudioTrackInterface> audioTrack(
        _factory->CreateAudioTrack(kAudioLabel,
            _factory->CreateAudioSource(NULL)));
    stream->AddTrack(videoTrack);
    stream->AddTrack(audioTrack);

    conn->createConnection();
    conn->createOffer();

    PeerConnectionManager::add(peer.id(), conn);
}


void Signaler::onPeerMessage(void*, smpl::Message& m)
{
    DebugL << "Peer message: " << m.from().toString() << endl;

    if (m.isMember("offer")) {
        assert(0 && "offer not supported");
    }
    else if (m.isMember("answer")) {
        recvSDP(m.from().id, m["answer"]);
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
    assert(0 && "not required");
}


void Signaler::onRemoveRemoteStream(const std::string& peerid, webrtc::MediaStreamInterface* stream)
{
    assert(0 && "not required");
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
