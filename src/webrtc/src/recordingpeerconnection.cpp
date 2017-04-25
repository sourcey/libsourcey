///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup webrtc
/// @{


#include "scy/webrtc/recordingpeerconnection.h"

#ifdef HAVE_FFMPEG

#include "scy/logger.h"
#include "scy/webrtc/peerconnectionmanager.h"
#include "scy/webrtc/videopacketsource.h"


using std::endl;


namespace scy {


RecordingPeerConnection::RecordingPeerConnection(PeerConnectionManager* manager,
                                                 const std::string& peerid,
                                                 const std::string& token,
                                                 const av::EncoderOptions options)
    : PeerConnection(manager, peerid, token, PeerConnection::Offer)
    , _recorder(new StreamRecorder(options))
{
    // _constraints.SetMandatoryReceiveAudio(false);
    // _constraints.SetMandatoryReceiveVideo(false);
    // _constraints.SetAllowDtlsSctpDataChannels();
}


RecordingPeerConnection::~RecordingPeerConnection()
{
}


// rtc::scoped_refptr<webrtc::MediaStreamInterface> RecordingPeerConnection::createMediaStream()
// {
//     assert(_mode == Offer);
//     assert(_factory);
//     assert(!_stream);
//     // assert(!_capture);
//     _stream = _factory->CreateLocalMediaStream(kStreamLabel);
//
//     _capturer.openFile(_file);
//     _capturer.addMediaTracks(_factory, _stream);
//
//     return _stream;
// }


void RecordingPeerConnection::OnAddStream(webrtc::MediaStreamInterface* stream)
{
    assert(_mode == Answer);

    DebugA(_peerid, ": On add stream")
    _manager->onAddRemoteStream(this, stream);

    // Set tracks
    auto videoTracks = stream->GetVideoTracks();
    if (!videoTracks.empty())
        _recorder->setVideoTrack(videoTracks[0]);

    auto audioTracks = stream->GetAudioTracks();
    if (!audioTracks.empty())
        _recorder->setAudioTrack(audioTracks[0]);
}


void RecordingPeerConnection::OnRemoveStream(webrtc::MediaStreamInterface* stream)
{
    assert(_mode == Answer);

    DebugA(_peerid, ": On remove stream")
    _manager->onRemoveRemoteStream(this, stream);

    assert(0 && "free streams");
}


// void RecordingPeerConnection::OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState new_state)
// {
//     switch(new_state) {
//     case webrtc::PeerConnectionInterface::kStable:
//         break;
//     case webrtc::PeerConnectionInterface::kClosed:
//         _recorder.reset(); // shutdown the recorder
//         break;
//     case webrtc::PeerConnectionInterface::kHaveLocalOffer:
//     case webrtc::PeerConnectionInterface::kHaveRemoteOffer:
//         break;
//     case webrtc::PeerConnectionInterface::kHaveLocalPrAnswer:
//     case webrtc::PeerConnectionInterface::kHaveRemotePrAnswer:
//         _capture->start();
//         break;
//     }
//
//     PeerConnection::OnSignalingChange(new_state);
// }


void RecordingPeerConnection::OnIceConnectionChange(
    webrtc::PeerConnectionInterface::IceConnectionState new_state)
{
    DebugA(_peerid, ": On ICE gathering change: ", new_state)

    switch (new_state) {
        case webrtc::PeerConnectionInterface::kIceConnectionNew:
        case webrtc::PeerConnectionInterface::kIceConnectionChecking:
        case webrtc::PeerConnectionInterface::kIceConnectionConnected:
            break;
        case webrtc::PeerConnectionInterface::kIceConnectionCompleted:
            break;
        case webrtc::PeerConnectionInterface::kIceConnectionFailed:
        case webrtc::PeerConnectionInterface::kIceConnectionDisconnected:
        case webrtc::PeerConnectionInterface::kIceConnectionClosed:
        case webrtc::PeerConnectionInterface::kIceConnectionMax:
            break;
    }

    PeerConnection::OnIceConnectionChange(new_state);
}


} // namespace scy


#endif // HAVE_FFMPEG