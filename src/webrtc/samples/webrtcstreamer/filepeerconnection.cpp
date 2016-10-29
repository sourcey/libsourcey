//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// LibSourcey is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#include "filepeerconnection.h"
#include "videopacketsource.h"
#include "scy/webrtc/peerconnectionmanager.h"
#include "scy/logger.h"


using std::endl;


namespace scy {


FilePeerConnection::FilePeerConnection(PeerConnectionManager* manager, const std::string& peerid, Mode mode) :
    PeerConnection(manager, peerid, mode)
{
    // Setup a PeerConnectionFactory with our custom ADM
    // TODO: Setup threads properly:
    auto networkThread = rtc::Thread::CreateWithSocketServer().release();
    auto workerThread = rtc::Thread::Create().release();
    auto signalingThread = rtc::Thread::Current();
    _factory = webrtc::CreatePeerConnectionFactory(
                  networkThread,
                  workerThread,
                  signalingThread,
                  _capturer.getAudioModule(),
                  nullptr, nullptr);
    networkThread->Start();
    workerThread->Start();
    // signalingThread->Start();

    _constraints.SetMandatoryReceiveAudio(false);
    _constraints.SetMandatoryReceiveVideo(false);
    _constraints.SetAllowDtlsSctpDataChannels();
}


FilePeerConnection::~FilePeerConnection()
{
}


rtc::scoped_refptr<webrtc::MediaStreamInterface> FilePeerConnection::createMediaStream()
{
    assert(_mode == Offer);
    assert(_factory);
    assert(!_stream);
    // assert(!_capture);
    _stream = _factory->CreateLocalMediaStream(kStreamLabel);

    _capturer.openFile("test.mp4");
    _capturer.addMediaTracks(_factory, _stream);

    return _stream;
}


// void FilePeerConnection::OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState new_state)
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
//     // assert(0);
//     // DebugL << _peerid << ": kHaveRemotePrAnswer: " << webrtc::PeerConnectionInterface::kHaveRemotePrAnswer << endl;
//     // DebugL << _peerid << ": kHaveLocalPrAnswer: " << webrtc::PeerConnectionInterface::kHaveLocalPrAnswer << endl;
//     // DebugL << _peerid << ": kHaveRemoteOffer: " << webrtc::PeerConnectionInterface::kHaveRemoteOffer << endl;
//     // DebugL << _peerid << ": kHaveLocalOffer: " << webrtc::PeerConnectionInterface::kHaveLocalOffer << endl;
//
//     PeerConnection::OnSignalingChange(new_state);
// }

void FilePeerConnection::OnIceConnectionChange(webrtc::PeerConnectionInterface::IceConnectionState new_state)
{
    DebugL << _peerid << ": On ICE gathering change: " << new_state << endl;

    switch(new_state) {
    case webrtc::PeerConnectionInterface::kIceConnectionNew:
    case webrtc::PeerConnectionInterface::kIceConnectionChecking:
    case webrtc::PeerConnectionInterface::kIceConnectionConnected:
        break;
    case webrtc::PeerConnectionInterface::kIceConnectionCompleted:
        _capturer.start();
        // _capture->start();
        break;
    case webrtc::PeerConnectionInterface::kIceConnectionFailed:
    case webrtc::PeerConnectionInterface::kIceConnectionDisconnected:
    case webrtc::PeerConnectionInterface::kIceConnectionClosed:
    case webrtc::PeerConnectionInterface::kIceConnectionMax:
        _capturer.stop();
        // _capture->stop();
        break;
    }

    PeerConnection::OnIceConnectionChange(new_state);
}


//
//
// void PeerConnection::createConnection()
// {
//     assert(_factory);
//
//     _peerConnection = _factory->CreatePeerConnection(
//         _config, &_constraints, nullptr, nullptr, this);
//
//     if (_stream) {
//         if (!_peerConnection->AddStream(_stream)) {
//             throw std::runtime_error("Adding stream to PeerConnection failed");
//         }
//     }
// }
//
//
// void PeerConnection::closeConnection()
// {
//     DebugL << _peerid << ": Closing" << endl;
//
//     if (_peerConnection) {
//         _peerConnection->Close();
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
//     assert(_peerConnection);
//
//     _peerConnection->CreateOffer(this, &_constraints);
// }
//
//
// void PeerConnection::recvSDP(const std::string& type, const std::string& sdp)
// {
//     DebugL << _peerid << ": Receive " << type << ": " << sdp << endl;
//
//     webrtc::SdpParseError error;
//     webrtc::SessionDescriptionInterface* desc(webrtc::CreateSessionDescription(type, sdp, &error));
//     if (!desc) {
//         throw std::runtime_error("Can't parse remote SDP: " + error.description);
//     }
//     _peerConnection->SetRemoteDescription(DummySetSessionDescriptionObserver::Create(), desc);
//
//     if (type == "offer") {
//         assert(_mode == Answer);
//         _peerConnection->CreateAnswer(this, &_constraints);
//     }
//     else {
//         assert(_mode == Offer);
//     }
// }
//
//
// void PeerConnection::recvCandidate(const std::string& mid, int mlineindex, const std::string& sdp)
// {
//     webrtc::SdpParseError error;
//     std::unique_ptr<webrtc::IceCandidateInterface> candidate(webrtc::CreateIceCandidate(mid, mlineindex, sdp, &error));
//     if (!candidate) {
//         throw std::runtime_error("Can't parse remote candidate: " + error.description);
//     }
//     _peerConnection->AddIceCandidate(candidate.get());
// }
//
//

//
//
// void PeerConnection::OnIceConnectionChange(webrtc::PeerConnectionInterface::IceConnectionState new_state)
// {
//     DebugL << _peerid << ": On ICE connection change: " << new_state << endl;
// }
//
//
// void PeerConnection::OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState new_state)
// {
//     DebugL << _peerid << ": On ICE gathering change: " << new_state << endl;
// }
//
//
// void PeerConnection::OnRenegotiationNeeded()
// {
//     DebugL << _peerid << ": On renegotiation needed" << endl;
// }
//
//
// void PeerConnection::OnAddStream(webrtc::MediaStreamInterface* stream)
// {
//     assert(_mode == Answer);
//
//     DebugL << _peerid << ": On add stream" << endl;
//     _manager->onAddRemoteStream(this, stream);
// }
//
//
// void PeerConnection::OnRemoveStream(webrtc::MediaStreamInterface* stream)
// {
//     assert(_mode == Answer);
//
//     DebugL << _peerid << ": On remove stream" << endl;
//     _manager->onRemoveRemoteStream(this, stream);
// }
//
//
// void PeerConnection::OnIceCandidate(const webrtc::IceCandidateInterface* candidate)
// {
//     std::string sdp;
//     if (!candidate->ToString(&sdp)) {
//         ErrorL << _peerid << ": Failed to serialize candidate" << endl;
//         assert(0);
//         return;
//     }
//
//     _manager->sendCandidate(this, candidate->sdp_mid(), candidate->sdp_mline_index(), sdp);
// }
//
//
// void PeerConnection::OnSuccess(webrtc::SessionDescriptionInterface* desc)
// {
//     DebugL << _peerid << ": Set local description" << endl;
//     _peerConnection->SetLocalDescription(
//         DummySetSessionDescriptionObserver::Create(), desc);
//
//     // Send an SDP offer to the peer
//     std::string sdp;
//     if (!desc->ToString(&sdp)) {
//         ErrorL << _peerid << ": Failed to serialize local sdp" << endl;
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
//     ErrorL << _peerid << ": On failure: " << error << endl;
//
//     _manager->onFailure(this, error);
// }
//
//
// void PeerConnection::setPeerConnectionFactory(rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> factory)
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
// rtc::scoped_refptr<webrtc::PeerConnectionInterface> PeerConnection::peerConnection() const
// {
//     return _peerConnection;
// }
//
//
// rtc::scoped_refptr<webrtc::MediaStreamInterface> PeerConnection::stream() const
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
//     DebugL << "On SDP parse success" << endl;
// }
//
//
// void DummySetSessionDescriptionObserver::OnFailure(const std::string& error)
// {
//     ErrorL << "On SDP parse error: " << error << endl;
//     assert(0);
// }


} // namespace scy
