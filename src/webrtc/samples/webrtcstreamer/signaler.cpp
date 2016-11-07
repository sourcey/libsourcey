///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup webrtc
/// @{

#include <iostream>
#include <string>

#include "signaler.h"
#include "filepeerconnection.h"
#include "videopacketsource.h"
#include "audiopacketmodule.h"
#include "multiplexmediacapturer.h"

#include "webrtc/api/peerconnectionfactory.h"
#include "webrtc/api/mediastreamtrackproxy.h"


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


void Signaler::sendSDP(PeerConnection* conn, const std::string& type, const std::string& sdp)
{
    assert(type == "offer" || type == "answer");
    smpl::Message m;
    Json::Value desc;
    desc[kSessionDescriptionTypeName] = type;
    desc[kSessionDescriptionSdpName] = sdp;
    m[type] = desc;

    postMessage(m);
}


void Signaler::sendCandidate(PeerConnection* conn, const std::string& mid, int mlineindex, const std::string& sdp)
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

    // auto conn = new PeerConnection(this, peer.id(), PeerConnection::Offer);
    // conn->constraints().SetMandatoryReceiveAudio(false);
    // conn->constraints().SetMandatoryReceiveVideo(false);
    // conn->constraints().SetAllowDtlsSctpDataChannels();

    auto conn = new FilePeerConnection(this, peer.id(), PeerConnection::Offer);
    // conn->constraints().SetMandatoryReceiveAudio(false);
    // conn->constraints().SetMandatoryReceiveVideo(false);
    // conn->constraints().SetAllowDtlsSctpDataChannels();

    // conn->setPeerConnectionFactory(
    //     webrtc::CreatePeerConnectionFactory(
    //         rtc::Thread::Current(),
    //         rtc::Thread::Current(),
    //         rtc::Thread::Current(),
    //         AudioPacketModule::Create(),
    //         nullptr, nullptr));

    // av::Device device;

    // Create the media stream
    rtc::scoped_refptr<webrtc::MediaStreamInterface> stream = conn->createMediaStream();

    // Create and add the audio stream
    // TODO: Add custom FFmpegAudioCapturer
    // if (av::DeviceManager::instance().getDefaultMicrophone(device)) {
        // InfoL << "Using audio device: " << device.name << endl;
        // rtc::scoped_refptr<webrtc::AudioTrackInterface> audioTrack(
        //     conn->factory()->CreateAudioTrack(kAudioLabel,
        //         conn->factory()->CreateAudioSource(nullptr)));

        // Thread here: https://groups.google.com/forum/#!topic/discuss-webrtc/d1fSm8Ns0Ww
        // CreateAudioSource returns LocalAudioSource
        // LocalAudioSource extends AudioSourceInterface
        //  - LocalAudioSource->AddSink(AudioTrackSinkInterface*) // does nothing
        //  - AudioSourceInterface->RegisterAudioObserver(AudioObserver* observer) // data comes from here? (only used in receiver code, not source?)

        // All happens at top of rtpreceiver.cc
        // AudioRtpSender()
        // AudioTrackInterface* track
        // sink_adapter_(new LocalAudioSinkAdapter())
        // track_->RegisterObserver(this);
        // track_->AddSink(sink_adapter_.get()); // VIDEO DOESN'T DO THIS

        // Asuumptions:
        // - LocalAudioSource has nothing to do with media capture
        // - need a custom AudioTrackInterface
        //   - AudioRtpSender will track_->AddSink the LocalAudioSinkAdapter, but we need
        //     to override this and add out own LocalAudioSinkAdapter

        // TODO:
        // - Custom AudioTrackInterface and LocalAudioSinkAdapter
        // - Override AudioTrackInterface->AddSink and prevent webrtc setting default LocalAudioSinkAdapter
        //   - At this point attach out own LocalAudioSinkAdapter and BOOM

        // rtc::scoped_refptr<AudioSourceInterface>
        // PeerConnectionFactory::CreateAudioSource(
        //     const MediaConstraintsInterface* constraints) {
        //   RTC_DCHECK(signaling_thread_->IsCurrent());
        //   rtc::scoped_refptr<LocalAudioSource> source(
        //       LocalAudioSource::Create(options_, constraints));
        //   return source;
        // }

        // rtc::scoped_refptr<AudioTrackInterface>
        // PeerConnectionFactory::CreateAudioTrack(const std::string& id,
        //                                         AudioSourceInterface* source) {
        //   RTC_DCHECK(signaling_thread_->IsCurrent());
        //   rtc::scoped_refptr<AudioTrackInterface> track(AudioTrack::Create(id, source));
        //   return AudioTrackProxy::Create(signaling_thread_, track);
        // }

        // rtc::scoped_refptr<AudioTrackInterface> track(AudioTrack::Create(id, source));
        // return AudioTrackProxy::Create(signaling_thread_, track);

        // rtc::Thread* PeerConnectionFactory::signaling_thread()
        //   return VideoTrackProxy::Create(signaling_thread_, worker_thread_, track);

        // static_cast<webrtc::PeerConnectionFactory*>(_factory.get())->signaling_thread();

        // rtc::scoped_refptr<webrtc::AudioTrackInterface> audioTrack(webrtc::AudioTrackProxy::Create( //webrtc::
        //     rtc::Thread::Current()/*signaling_thread_*/,
        //         webrtc::CustomAudioTrack::Create(kAudioLabel,
        //             _factory->CreateAudioSource(nullptr))));

              // rtc::scoped_refptr<LocalAudioSource> source(
              //     LocalAudioSource::Create(options_, constraints));

        // stream->AddTrack(audioTrack);
    // }

    // Create and add the video stream
    // if (av::DeviceManager::instance().getDefaultCamera(device)) {
    //     InfoL << "Using video device: " << device.name << endl;
    //     rtc::scoped_refptr<webrtc::VideoTrackInterface> videoTrack(
    //         _factory->CreateVideoTrack(kVideoLabel,
    //             _factory->CreateVideoSource(new VideoPacketSource(device.id), nullptr)));
    //     stream->AddTrack(videoTrack);
    // }

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


void Signaler::onAddRemoteStream(PeerConnection* conn, webrtc::MediaStreamInterface* stream)
{
    assert(0 && "not required");
}


void Signaler::onRemoveRemoteStream(PeerConnection* conn, webrtc::MediaStreamInterface* stream)
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
