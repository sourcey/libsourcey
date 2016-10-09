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

#include "scy/webrtc/peerconnectionclient.h"
#include "scy/webrtc/peerconnectionmanager.h"
#include "scy/logger.h"

#include "webrtc/modules/video_capture/video_capture_factory.h"
#include "webrtc/media/engine/webrtcvideocapturerfactory.h"


using std::endl;


namespace scy {


PeerConnectionClient::PeerConnectionClient(PeerConnectionManager* manager, const std::string& peerid, Mode mode) :
    _manager(manager),
    _peerid(peerid),
    _mode(mode),
    _peerConnection(nullptr),
    _stream(nullptr)
{
    // webrtc::PeerConnectionInterface::IceServer stun;
    // stun.uri = kGoogleStunServerUri;
    // _config.servers.push_back(stun);

    // _constraints.SetMandatoryReceiveAudio(true);
    // _constraints.SetMandatoryReceiveVideo(true);
    // _constraints.SetAllowDtlsSctpDataChannels(); // triggers error on CreateAnswer
}


PeerConnectionClient::~PeerConnectionClient()
{
    DebugL << _peerid << ": Destroying" << endl;
    closeConnection();
}


rtc::scoped_refptr<webrtc::MediaStreamInterface> PeerConnectionClient::createMediaStream()
{
    assert(_mode == Offer);
    assert(!_stream);
    _stream = _manager->factory()->CreateLocalMediaStream(kStreamLabel);
    return _stream;
}


void PeerConnectionClient::createConnection()
{
    _peerConnection = _manager->factory()->CreatePeerConnection(
        _config, &_constraints, NULL, NULL, this);

    if (_stream) {
        if (!_peerConnection->AddStream(_stream)) {
            throw std::runtime_error("Adding stream to PeerConnection failed");
        }
    }
}


void PeerConnectionClient::closeConnection()
{
    if (_peerConnection) {
        _peerConnection->Close();
    }
}


void PeerConnectionClient::createOffer()
{
    assert(_mode == Offer);
    assert(_peerConnection);

    _peerConnection->CreateOffer(this, &_constraints);
}


void PeerConnectionClient::recvSDP(const std::string& type, const std::string& sdp)
{
    DebugL << _peerid << ": Receive " << type << ": " << sdp << endl;

    webrtc::SdpParseError error;
    webrtc::SessionDescriptionInterface* desc(webrtc::CreateSessionDescription(type, sdp, &error));
    if (!desc) {
        throw std::runtime_error("Can't parse remote SDP: " + error.description);
    }
    _peerConnection->SetRemoteDescription(DummySetSessionDescriptionObserver::Create(), desc);

    if (type == "offer") {
        assert(_mode == Answer);
        _peerConnection->CreateAnswer(this, &_constraints);
    }
    else {
        assert(_mode == Offer);
    }
}


void PeerConnectionClient::recvCandidate(const std::string& mid, int mlineindex, const std::string& sdp)
{
    webrtc::SdpParseError error;
    std::unique_ptr<webrtc::IceCandidateInterface> candidate(webrtc::CreateIceCandidate(mid, mlineindex, sdp, &error));
    if (!candidate) {
        throw std::runtime_error("Can't parse remote candidate: " + error.description);
    }
    _peerConnection->AddIceCandidate(candidate.get());
}


void PeerConnectionClient::OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState new_state)
{
    DebugL << _peerid << ": On signaling state change: " << new_state << endl;
    DebugL << "PeerConnection signaling state change: " << new_state << endl;

    switch(new_state) {
    case webrtc::PeerConnectionInterface::kStable:
        // session_.addRef();
        break;
    case webrtc::PeerConnectionInterface::kClosed:
        // session_.removeRef();
        break;
    case webrtc::PeerConnectionInterface::kHaveLocalOffer:
        case webrtc::PeerConnectionInterface::kHaveRemoteOffer:
        case webrtc::PeerConnectionInterface::kHaveLocalPrAnswer:
        case webrtc::PeerConnectionInterface::kHaveRemotePrAnswer:
        break;
    }
}


void PeerConnectionClient::OnIceConnectionChange(webrtc::PeerConnectionInterface::IceConnectionState new_state)
{
    DebugL << _peerid << ": On ICE connection change: " << new_state << endl;
}


void PeerConnectionClient::OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState new_state)
{
    DebugL << _peerid << ": On ICE gathering change: " << new_state << endl;
}


void PeerConnectionClient::OnRenegotiationNeeded()
{
    DebugL << _peerid << ": On renegotiation needed" << endl;
}


void PeerConnectionClient::OnAddStream(webrtc::MediaStreamInterface* stream)
{
    assert(_mode == Answer);

    DebugL << _peerid << ": On add stream" << endl;
    _manager->onAddRemoteStream(_peerid, stream);
}


void PeerConnectionClient::OnRemoveStream(webrtc::MediaStreamInterface* stream)
{
    assert(_mode == Answer);

    DebugL << _peerid << ": On remove stream" << endl;
    _manager->onRemoveRemoteStream(_peerid, stream);
}


void PeerConnectionClient::OnIceCandidate(const webrtc::IceCandidateInterface* candidate)
{
    std::string sdp;
    if (!candidate->ToString(&sdp)) {
        ErrorL << _peerid << ": Failed to serialize candidate" << endl;
        assert(0);
        return;
    }

    _manager->sendCandidate(_peerid, candidate->sdp_mid(), candidate->sdp_mline_index(), sdp);
}


void PeerConnectionClient::OnSuccess(webrtc::SessionDescriptionInterface* desc)
{
    DebugL << _peerid << ": Set local description" << endl;
    _peerConnection->SetLocalDescription(
        DummySetSessionDescriptionObserver::Create(), desc);

    // Send an SDP offer to the peer
    std::string sdp;
    if (!desc->ToString(&sdp)) {
        ErrorL << _peerid << ": Failed to serialize local sdp" << endl;
        assert(0);
        return;
    }

    _manager->sendSDP(_peerid, desc->type(), sdp);
}


void PeerConnectionClient::OnFailure(const std::string &error)
{
    ErrorL << _peerid << ": On failure: " << error << endl;
    assert(0 && "not implemented");
}


webrtc::FakeConstraints& PeerConnectionClient::constraints()
{
    return _constraints;
}


rtc::scoped_refptr<webrtc::PeerConnectionInterface> PeerConnectionClient::peerConnection() const
{
    return _peerConnection;
}


rtc::scoped_refptr<webrtc::MediaStreamInterface> PeerConnectionClient::stream() const
{
    return _stream;
}


//
// Dummy Set Session Description Observer
//


void DummySetSessionDescriptionObserver::OnSuccess()
{
    DebugL << "On SDP parse success" << endl;
}


void DummySetSessionDescriptionObserver::OnFailure(const std::string& error)
{
    ErrorL << "On SDP parse error: " << error << endl;
    assert(0);
}


} // namespace scy
