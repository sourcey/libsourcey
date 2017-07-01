///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup webrtc
/// @{


#include "scy/webrtc/streamingpeerconnection.h"
#include "scy/logger.h"
#include "scy/webrtc/peerconnectionmanager.h"
#include "scy/webrtc/videopacketsource.h"


using std::endl;


namespace scy {
namespace wrtc {


StreamingPeerConnection::StreamingPeerConnection(PeerConnectionManager* manager,
                                                 const std::string& peerid,
                                                 const std::string& token,
                                                 const std::string& file)
    : PeerConnection(manager, peerid, token, PeerConnection::Offer)
    , _file(file)
    , _networkThread(rtc::Thread::CreateWithSocketServer())
    , _workerThread(rtc::Thread::Create().release())
    //, _networkThread(rtc::Thread::CreateWithSocketServer().release())
    //, _workerThread(rtc::Thread::Create().release())
{
    // Setup a PeerConnectionFactory with our custom ADM
    // TODO: Setup threads properly:
    // _networkThread = rtc::Thread::CreateWithSocketServer(); //.release();
    // _workerThread = rtc::Thread::Create(); //.release();
    // _signalingThread = rtc::Thread::Current();

    // FIXME: Dont create new threads for each factory

    if (!_networkThread->Start() || !_workerThread->Start()) {
        throw std::runtime_error("Failed to start threads");
    }

    _factory = webrtc::CreatePeerConnectionFactory(
        _networkThread.get(), _workerThread.get(), rtc::Thread::Current(),
        _capturer.getAudioModule(), nullptr, nullptr);

    // https://bugs.chromium.org/p/webrtc/issues/detail?id=2388
    _constraints.SetMandatoryReceiveAudio(false);
    _constraints.SetMandatoryReceiveVideo(false);
    _constraints.SetAllowDtlsSctpDataChannels(); // allow cross-browser
}


StreamingPeerConnection::~StreamingPeerConnection()
{
    // https://github.com/pristineio/webrtc-mirror/blob/32001ef124f5082651c661965dc5d75d7f06a57b/talk/examples/android/src/org/appspot/apprtc/AppRTCDemoActivity.java#L473
    //this->
    //_factory->
    //this->Release();
    //_stream->Release();
    //_factory->Release();
    //_factory->Release();
    //_factory->Release();
    //delete _factory.release();
}


rtc::scoped_refptr<webrtc::MediaStreamInterface> StreamingPeerConnection::createMediaStream()
{
    assert(_mode == Offer);
    assert(_factory);
    assert(!_stream);
    // assert(!_capture);
    _stream = _factory->CreateLocalMediaStream(kStreamLabel);

    _capturer.openFile(_file);
    _capturer.addMediaTracks(_factory, _stream);

    return _stream;
}


// void StreamingPeerConnection::OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState new_state)
// {
//     switch(new_state) {
//     case webrtc::PeerConnectionInterface::kStable:
//         break;
//     case webrtc::PeerConnectionInterface::kClosed:
//         _capture->stop();
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


void StreamingPeerConnection::OnIcePeerChange(
    webrtc::PeerConnectionInterface::IcePeerState new_state)
{
    LDebug(_peerid, ": On ICE gathering change: ", new_state)

    switch (new_state) {
        case webrtc::PeerConnectionInterface::kIcePeerNew:
        case webrtc::PeerConnectionInterface::kIcePeerChecking:
        case webrtc::PeerConnectionInterface::kIcePeerConnected:
            break;
        case webrtc::PeerConnectionInterface::kIcePeerCompleted:
            _capturer.start();
            break;
        case webrtc::PeerConnectionInterface::kIcePeerFailed:
        case webrtc::PeerConnectionInterface::kIcePeerDisconnected:
        case webrtc::PeerConnectionInterface::kIcePeerClosed:
        case webrtc::PeerConnectionInterface::kIcePeerMax:
            _capturer.stop();
            break;
    }

    PeerConnection::OnIcePeerChange(new_state);
}


// void PeerConnection::createPeer()
// {
//     assert(_factory);
//
//     _peerPeer = _factory->CreatePeerConnection(
//         _config, &_constraints, nullptr, nullptr, this);
//
//     if (_stream) {
//         if (!_peerPeer->AddStream(_stream)) {
//             throw std::runtime_error("Adding stream to PeerConnection
//             failed");
//         }
//     }
// }
//
//
// void PeerConnection::closePeer()
// {
//     LDebug(_peerid, ": Closing")
//
//     if (_peerPeer) {
//         _peerPeer->Close();
//     }
//     else {
//         // Call onClosed if no connection has been
//         // made so callbacks are always run.
//         _manager->onClosed(this);
//     }
// }
//
//
// void PeerConnection::createOffer()
// {
//     assert(_mode == Offer);
//     assert(_peerPeer);
//
//     _peerPeer->CreateOffer(this, &_constraints);
// }
//
//
// void PeerConnection::recvSDP(const std::string& type, const std::string& sdp)
// {
//     LDebug(_peerid, ": Receive ", type, ": ", sdp)
//
//     webrtc::SdpParseError error;
//     webrtc::SessionDescriptionInterface*
//     desc(webrtc::CreateSessionDescription(type, sdp, &error));
//     if (!desc) {
//         throw std::runtime_error("Can't parse remote SDP: " +
//         error.description);
//     }
//     _peerPeer->SetRemoteDescription(DummySetSessionDescriptionObserver::Create(),
//     desc);
//
//     if (type == "offer") {
//         assert(_mode == Answer);
//         _peerPeer->CreateAnswer(this, &_constraints);
//     }
//     else {
//         assert(_mode == Offer);
//     }
// }
//
//
// void PeerConnection::recvCandidate(const std::string& mid, int mlineindex,
// const std::string& sdp)
// {
//     webrtc::SdpParseError error;
//     std::unique_ptr<webrtc::IceCandidateInterface>
//     candidate(webrtc::CreateIceCandidate(mid, mlineindex, sdp, &error));
//     if (!candidate) {
//         throw std::runtime_error("Can't parse remote candidate: " +
//         error.description);
//     }
//     _peerPeer->AddIceCandidate(candidate.get());
// }
//
//
// void
// PeerConnection::OnIcePeerChange(webrtc::PeerConnectionInterface::IcePeerState
// new_state)
// {
//     LDebug(_peerid, ": On ICE connection change: ", new_state)
// }
//
//
// void
// PeerConnection::OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState
// new_state)
// {
//     LDebug(_peerid, ": On ICE gathering change: ", new_state)
// }
//
//
// void PeerConnection::OnRenegotiationNeeded()
// {
//     LDebug(_peerid, ": On renegotiation needed")
// }
//
//
// void PeerConnection::OnAddStream(webrtc::MediaStreamInterface* stream)
// {
//     assert(_mode == Answer);
//
//     LDebug(_peerid, ": On add stream")
//     _manager->onAddRemoteStream(this, stream);
// }
//
//
// void PeerConnection::OnRemoveStream(webrtc::MediaStreamInterface* stream)
// {
//     assert(_mode == Answer);
//
//     LDebug(_peerid, ": On remove stream")
//     _manager->onRemoveRemoteStream(this, stream);
// }
//
//
// void PeerConnection::OnIceCandidate(const webrtc::IceCandidateInterface*
// candidate)
// {
//     std::string sdp;
//     if (!candidate->ToString(&sdp)) {
//         LError(_peerid, ": Failed to serialize candidate")
//         assert(0);
//         return;
//     }
//
//     _manager->sendCandidate(this, candidate->sdp_mid(),
//     candidate->sdp_mline_index(), sdp);
// }
//
//
// void PeerConnection::OnSuccess(webrtc::SessionDescriptionInterface* desc)
// {
//     LDebug(_peerid, ": Set local description")
//     _peerPeer->SetLocalDescription(
//         DummySetSessionDescriptionObserver::Create(), desc);
//
//     // Send an SDP offer to the peer
//     std::string sdp;
//     if (!desc->ToString(&sdp)) {
//         LError(_peerid, ": Failed to serialize local sdp")
//         assert(0);
//         return;
//     }
//
//     _manager->sendSDP(this, desc->type(), sdp);
// }
//
//
// void PeerConnection::OnFailure(const std::string& error)
// {
//     LError << _peerid(": On failure: ", error)
//
//     _manager->onFailure(this, error);
// }
//
//
// void
// PeerConnection::setPeerConnectionFactory(rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface>
// factory)
// {
//     assert(!_factory); // should not be already set via PeerConnectionManager
//     _factory = factory;
// }
//
//
// std::string PeerConnection::peerid() const
// {
//     return _peerid;
// }
//
//
// webrtc::FakeConstraints& PeerConnection::constraints()
// {
//     return _constraints;
// }
//
//
// webrtc::PeerConnectionFactoryInterface* PeerConnection::factory() const
// {
//     return _factory.get();
// }
//
//
// rtc::scoped_refptr<webrtc::PeerConnectionInterface>
// PeerConnection::peerPeer() const
// {
//     return _peerPeer;
// }
//
//
// rtc::scoped_refptr<webrtc::MediaStreamInterface> PeerConnection::stream()
// const
// {
//     return _stream;
// }
//
//
// //
// // Dummy Set Session Description Observer
// //
//
//
// void DummySetSessionDescriptionObserver::OnSuccess()
// {
//     LDebug("On SDP parse success")
// }
//
//
// void DummySetSessionDescriptionObserver::OnFailure(const std::string& error)
// {
//     LError("On SDP parse error: ", error)
//     assert(0);
// }


} } // namespace scy::wrtc
