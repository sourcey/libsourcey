///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
///


#include "signaler.h"
#include "util.h"

#include "scy/av/codec.h"
#include "scy/av/format.h"
#include "scy/filesystem.h"
#include "scy/util.h"

#include "scy/webrtc/util.h"

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
    _client += packetSlot(this, &Signaler::onPeerCommand);
    _client += packetSlot(this, &Signaler::onPeerEvent);
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

    smpl::Event e({
      { "name", "ice:sdp" },
      { "sdp", {
        { kSessionDescriptionTypeName, type },
        { kSessionDescriptionSdpName, sdp }
      }}
    });

    postMessage(e);
}


void Signaler::sendCandidate(PeerConnection* conn, const std::string& mid,
                             int mlineindex, const std::string& sdp)
{
    smpl::Event e({
      { "name", "ice:candidate" },
      { "candidate", {
        { kCandidateSdpMidName, mid },
        { kCandidateSdpMlineIndexName, mlineindex},
        { kCandidateSdpName, sdp}
      }}
    });

    postMessage(e);
}


void Signaler::onPeerConnected(smpl::Peer& peer)
{
    if (peer.id() == _client.ourID())
        return;
    DebugL << "Peer connected: " << peer.id() << endl;

    if (PeerConnectionManager::exists(peer.id())) {
        DebugL << "Peer already has a session: " << peer.id() << endl;
    }
}


void Signaler::onPeerCommand(smpl::Command& c)
{
    DebugL << "Peer command: " << c.from().toString() << endl;

    // List available devices for streaming
    if (c.node() == "streaming:devices") {
        json::value devices;
        auto devs = getVideoCaptureDevices();
        for (auto dev : devs) {
            devices.push_back(dev);
        }

        c.setData("devices", devices);
        c.setStatus(200);
        _client.respond(c);
    }

    // List available files for streaming
    else if (c.node() == "streaming:files") {
        json::value files;
        StringVec nodes;
        fs::readdir(getDataDirectory(), nodes);
        for (auto node : nodes) {
            files.push_back(node);
        }

        c.setData("files", files);
        c.setStatus(200);
        _client.respond(c);
    }

    // Start a streaming session
    else if (c.node() == "streaming:start") {
        std::string device = c.data("device").get<std::string>();

        // Stream a file from the filesystem like so:
        // std::string file = c.data("file").get<std::string>();
        // std::string filePath(getDataDirectory());
        // fs::addnode(filePath, file);

        // Create the Peer Connection
        auto conn = new PeerConnection(this, c.from().id, c.id(), PeerConnection::Offer);
        conn->constraints().SetMandatoryReceiveAudio(false);
        conn->constraints().SetMandatoryReceiveVideo(false);
        conn->constraints().SetAllowDtlsSctpDataChannels();

        // Create tracks and device captures
        rtc::scoped_refptr<webrtc::AudioTrackInterface> audio_track(
            _factory->CreateAudioTrack(kAudioLabel, _factory->CreateAudioSource(nullptr)));
        rtc::scoped_refptr<webrtc::VideoTrackInterface> video_track(
            _factory->CreateVideoTrack(kVideoLabel,
                _factory->CreateVideoSource(openWebRtcVideoCaptureDevice(device), nullptr)));

        // Create the media stream and attach device captures
        auto stream = conn->createMediaStream();
        stream->AddTrack(audio_track);
        stream->AddTrack(video_track);

        // Send the Offer SDP
        conn->createConnection();
        conn->createOffer();

        PeerConnectionManager::add(c.from().id, conn);

        c.setStatus(200);
        _client.respond(c);

        //auto conn = new StreamingPeerConnection(this, c.from().id, c.id(), filePath);
        //auto stream = conn->createMediaStream();
        //conn->createConnection();
        //conn->createOffer();
        //PeerConnectionManager::add(c.id(), conn);
        // _client.persistence().add(c.id(), reinterpret_cast<smpl::Message *>(c.clone()), 0);
    }

    // Stop streaming
    else if (c.node() == "streaming:stop") {
        auto conn = PeerConnectionManager::get(c.from().id, false);
        if (conn) {
            DebugL << "Closing peer connection: " << c.from().id << endl;
            conn->closeConnection(); // will be deleted via callback
        }
    }

    // Start a recording session
    // else if (c.node() == "recording:start") {
    //
    //    av::EncoderOptions options;
    //    options.ofile = OUTPUT_FILENAME;
    //    options.oformat = OUTPUT_FORMAT;
    //
    //    auto conn = new RecordingPeerConnection(this, c.from().id, c.id(), options);
    //    conn->constraints().SetMandatoryReceiveVideo(true);
    //    conn->constraints().SetMandatoryReceiveAudio(true);
    //    conn->createConnection();
    //    PeerConnectionManager::add(c.id(), conn);
    //
    //    c.setStatus(200);
    //    _client.respond(c);
    //    // _client.persistence().add(c.id(), reinterpret_cast<smpl::Message *>(c.clone()), 0);
    // }
}


void Signaler::onPeerEvent(smpl::Event& e)
{
    DebugL << "Peer event: " << e.from().toString() << endl;

    if (e.name() == "ice:sdp") {
        recvSDP(e.from().id, e["sdp"]);
    }
    else if (e.name() == "ice:candidate") {
        recvCandidate(e.from().id, e["candidate"]);
    }
}


void Signaler::onPeerMessage(smpl::Message& m)
{
    DebugL << "Peer message: " << m.from().toString() << endl;
}


void Signaler::onPeerDiconnected(const smpl::Peer& peer)
{
    DebugL << "Peer disconnected" << endl;

    // TODO: Loop all and close for peer

    auto conn = get(peer.id(), false);
    if (conn) {
        DebugL << "Closing peer connection: " << peer.id() << endl;
        conn->closeConnection(); // will be deleted via callback
    }
}


void Signaler::onClientStateChange(void* sender, sockio::ClientState& state,
                                   const sockio::ClientState& oldState)
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
            throw std::runtime_error("Cannot connect to Symple server. "
                                     "Did you start the demo app and the "
                                     "Symple server is running on port 4500?");
    }
}


void Signaler::onAddRemoteStream(PeerConnection* conn, webrtc::MediaStreamInterface* stream)
{
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
    // _recorder.reset(); // shutdown the recorder
    PeerConnectionManager::onClosed(conn);
}


void Signaler::onFailure(PeerConnection* conn, const std::string& error)
{
    // _recorder.reset(); // shutdown the recorder
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
