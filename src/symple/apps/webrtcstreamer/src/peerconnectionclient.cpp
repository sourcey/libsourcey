#include "peerconnectionclient.h"
#include "signaler.h"

#include "webrtc/modules/video_capture/video_capture_factory.h"
#include "webrtc/media/engine/webrtcvideocapturerfactory.h"


using std::endl;


namespace scy {


const char kStunServerUri[] = "stun:stun.l.google.com:19302";
// const char kStunServerUri[] = "stun:127.0.0.1:3478";
// const char kRelayServerUri[] = "turn:127.0.0.1:3478";
const char kAudioLabel[] = "audio_label";
const char kVideoLabel[] = "video_label";
const char kStreamLabel[] = "stream_label";


//
// Peer Connection Client
//


PeerConnectionClient::PeerConnectionClient(Signaler& client, const std::string& peerid) :
    _client(client), _peerid(peerid)
{
    webrtc::PeerConnectionInterface::IceServer stun;
    stun.uri = kStunServerUri;
    config_.servers.push_back(stun);

    // We are send only for now
    constraints_.SetMandatoryReceiveAudio(false);
    constraints_.SetMandatoryReceiveVideo(false);
    constraints_.SetAllowDtlsSctpDataChannels();

    // TODO: move to Signaler
    peer_connection_factory_ = webrtc::CreatePeerConnectionFactory();
}


PeerConnectionClient::~PeerConnectionClient()
{
}


bool PeerConnectionClient::initConnection()
{
    peer_connection_ = peer_connection_factory_->CreatePeerConnection(
        config_, &constraints_, NULL, NULL, this);

    rtc::scoped_refptr<webrtc::VideoTrackInterface> video_track(
        peer_connection_factory_->CreateVideoTrack(kVideoLabel,
            peer_connection_factory_->CreateVideoSource(new VideoStreamCapturer(0), NULL)));

    rtc::scoped_refptr<webrtc::AudioTrackInterface> audio_track(
        peer_connection_factory_->CreateAudioTrack(kAudioLabel,
            peer_connection_factory_->CreateAudioSource(NULL)));

    stream_ = peer_connection_factory_->CreateLocalMediaStream(kStreamLabel);
    stream_->AddTrack(video_track);
    stream_->AddTrack(audio_track);

    if (!peer_connection_->AddStream(stream_)) {
        ErrorL << "Adding stream to PeerConnection failed" << endl;
        assert(0);
        return false;
    }
    return true;
}


bool PeerConnectionClient::closeConnection()
{
    if (peer_connection_) {
        peer_connection_->Close();
        return true;
    }
    return false;
}


void PeerConnectionClient::createOffer()
{
    peer_connection_->CreateOffer(this, &constraints_);
}


// bool PeerConnectionClient::handleRemoteOffer(const std::string& type, const std::string sdp)
// {
//     webrtc::SdpParseError error;
//     webrtc::SessionDescriptionInterface* desc(webrtc::CreateSessionDescription(type, sdp, &error));
//     if (!desc) {
//         ErrorL << "Can't parse remote offer: " << error.description << endl;
//         assert(0);
//         return false;
//     }
//     peer_connection_->SetRemoteDescription(DummySetSessionDescriptionObserver::Create(), desc);
//     peer_connection_->CreateAnswer(this, &constraints_);
//     return true;
// }


bool PeerConnectionClient::handleRemoteAnswer(const std::string& type, const std::string sdp)
{
    webrtc::SdpParseError error;
    webrtc::SessionDescriptionInterface* desc(webrtc::CreateSessionDescription(type, sdp, &error));
    if (!desc) {
        ErrorL << "Can't parse remote answer: " << error.description << endl;
        assert(0);
        return false;
    }
    peer_connection_->SetRemoteDescription(DummySetSessionDescriptionObserver::Create(), desc);
    return true;
}


bool PeerConnectionClient::handleRemoteCandidate(const std::string& mid, int mlineindex, const std::string sdp)
{
    webrtc::SdpParseError error;
    std::unique_ptr<webrtc::IceCandidateInterface> candidate(webrtc::CreateIceCandidate(mid, mlineindex, sdp, &error));
    if (!candidate) {
        ErrorL << "Can't parse remote candidate message: " << error.description << endl;
        assert(0);
        return false;
    }
    peer_connection_->AddIceCandidate(candidate.get());
    return true;
}


void PeerConnectionClient::OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState new_state)
{
    DebugL << _peerid << ": On signaling state change: " << new_state << endl;
}


void PeerConnectionClient::OnIceConnectionChange(webrtc::PeerConnectionInterface::IceConnectionState new_state)
{
    // assert(false);
    DebugL << _peerid << ": On ICE connection change: " << new_state << endl;
}


void PeerConnectionClient::OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState new_state)
{
    // assert(false);
    DebugL << _peerid << ": On ICE gathering change: " << new_state << endl;
}


void PeerConnectionClient::OnRenegotiationNeeded()
{
    DebugL << _peerid << ": On renegotiation needed" << endl;
}


void PeerConnectionClient::OnAddStream(webrtc::MediaStreamInterface* stream)
{
    DebugL << _peerid << ": On add stream" << endl;
    assert(false);
}


void PeerConnectionClient::OnRemoveStream(webrtc::MediaStreamInterface* stream)
{
    DebugL << _peerid << ": On remove stream" << endl;
}


void PeerConnectionClient::OnIceCandidate(const webrtc::IceCandidateInterface* candidate)
{
    std::string sdp;
    if (!candidate->ToString(&sdp)) {
        ErrorL << _peerid << ": Failed to serialize candidate" << endl;
        assert(0);
        return;
    }

    _client.broadcastCandidate(candidate->sdp_mid(), candidate->sdp_mline_index(), sdp);
}


void PeerConnectionClient::OnSuccess(webrtc::SessionDescriptionInterface* desc)
{
    DebugL << _peerid << ": Set local description" << endl;
    peer_connection_->SetLocalDescription(
        DummySetSessionDescriptionObserver::Create(), desc);

    // Send an SDP offer to the peer
    std::string sdp;
    if (!desc->ToString(&sdp)) {
        ErrorL << _peerid << ": Failed to serialize local sdp" << endl;
        assert(0);
        return;
    }

    _client.broadcastSDP(desc->type(), sdp);
}


void PeerConnectionClient::OnFailure(const std::string &error)
{
    ErrorL << _peerid << ": On failure: " << error << endl;
    assert(0);
}


} // namespace scy




// void PeerConnectionClient::OnError()
// {
//     ErrorL << "PeerConnection error" << endl;
//     assert(0);
// }


// bool PeerConnectionClient::InitConnection()
// {
//     peer_connection_ = peer_connection_factory_->CreatePeerConnection(servers_, &constraints_, NULL, this);
//
//     // TODO: Override audio capture.
//     // This calls PeerConnectionFactory::CreateAudioSource_s which instantiates LocalAudioSource
//     // Set breakpoint to figure out the mess: void WebRtcVoiceEngine::Process(int channel,
//     rtc::scoped_refptr<webrtc::AudioTrackInterface> audio_track(
//       peer_connection_factory_->CreateAudioTrack(kAudioLabel,
//         peer_connection_factory_->CreateAudioSource(NULL)));
//
//     rtc::scoped_refptr<webrtc::VideoTrackInterface> video_track(
//       peer_connection_factory_->CreateVideoTrack(kVideoLabel,
//         peer_connection_factory_->CreateVideoSource(OpenVideoCaptureDevice(), NULL)));
//
//     stream_ = peer_connection_factory_->CreateLocalMediaStream(kStreamLabel);
//     stream_->AddTrack(audio_track);
//     stream_->AddTrack(video_track);
//     if (!peer_connection_->AddStream(stream_, &constraints_)) {
//         ErrorL << "Adding stream to PeerConnection failed" << endl;
//         assert(0);
//     }
//
//     return true;
// }
//
// void PeerConnectionClient::CreateOffer()
// {
//     peer_connection_->CreateOffer(this, &constraints_);
// }
//
//
// bool PeerConnectionClient::OnRemoteOffer(const std::string& type, const std::string sdp)
// {
//     webrtc::SessionDescriptionInterface* desc(webrtc::CreateSessionDescription(type, sdp));
//     if (!desc) return false;
//     peer_connection_->SetRemoteDescription(DummySetSessionDescriptionObserver::Create(), desc);
//     peer_connection_->CreateAnswer(this, &constraints_);
//     return true;
// }
//
//
// bool PeerConnectionClient::OnRemoteAnswer(const std::string& type, const std::string sdp)
// {
//     webrtc::SessionDescriptionInterface* desc(webrtc::CreateSessionDescription(type, sdp));
//     if (!desc) return false;
//     peer_connection_->SetRemoteDescription(DummySetSessionDescriptionObserver::Create(), desc);
//     return true;
// }
//
//
// bool PeerConnectionClient::OnRemoteCandidate(const std::string& mid, int mlineindex, const std::string sdp)
// {
//     rtc::scoped_ptr<webrtc::IceCandidateInterface> candidate(webrtc::CreateIceCandidate(mid, mlineindex, sdp));
//     return candidate.get() && peer_connection_->AddIceCandidate(candidate.get());
// }
//
//
// void PeerConnectionClient::OnError()
// {
//     ErrorL << _peerid << ": PeerConnection error" << endl;
//     assert(0);
// }


// cricket::VideoCapturer* PeerConnectionClient::OpenVideoCaptureDevice()
// {
//     rtc::scoped_ptr<cricket::DeviceManagerInterface> dev_manager(
//       cricket::DeviceManagerFactory::Create());
//     if (!dev_manager->Init()) {
//         ErrorL << "Can't create device manager" << endl;
//         assert(0);
//         return NULL;
//     }
//
//     // Add our OpenCV VideoCapturer factory
//     cricket::DeviceManager* device_manager = static_cast<cricket::DeviceManager*>(dev_manager.get());
//     device_manager->set_device_video_capturer_factory(new VideoStreamCapturerFactory());
//
//     std::vector<cricket::Device> devs;
//     if (!dev_manager->GetVideoCaptureDevices(&devs)) {
//         ErrorL << "Can't enumerate video devices" << endl;
//         assert(0);
//         return NULL;
//     }
//     std::vector<cricket::Device>::iterator dev_it = devs.begin();
//     cricket::VideoCapturer* capturer = NULL;
//     for (; dev_it != devs.end(); ++dev_it) {
//         capturer = dev_manager->CreateVideoCapturer(*dev_it);
//         if (capturer != NULL)
//             break;
//     }
//     return capturer;
// }
