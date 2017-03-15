///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup webrtc
/// @{


#include "scy/webrtc/peerconnection.h"
#include "scy/logger.h"
#include "scy/webrtc/peerconnectionmanager.h"


using std::endl;


namespace scy {


PeerConnection::PeerConnection(PeerConnectionManager* manager,
                               const std::string& peerid,
                               const std::string& token,
                               Mode mode)
    : _manager(manager)
    , _peerid(peerid)
    , _token(token)
    , _mode(mode)
    , _factory(manager->factory())
    , _peerConnection(nullptr)
    , _stream(nullptr)
{
    // webrtc::PeerConnectionInterface::IceServer stun;
    // stun.uri = kGoogleStunServerUri;
    // _config.servers.push_back(stun);

    // _constraints.SetMandatoryReceiveAudio(true);
    // _constraints.SetMandatoryReceiveVideo(true);
    // _constraints.SetAllowDtlsSctpDataChannels(); // triggers error if CreateAnswer
}


PeerConnection::~PeerConnection()
{
    DebugL << _peerid << ": Destroying" << endl;
    // closeConnection();

    if (_peerConnection) {
        _peerConnection->Close();
    }
}


rtc::scoped_refptr<webrtc::MediaStreamInterface> PeerConnection::createMediaStream()
{
    assert(_mode == Offer);
    assert(_factory);
    assert(!_stream);
    _stream = _factory->CreateLocalMediaStream(kStreamLabel);
    return _stream;
}


void PeerConnection::createConnection()
{
    assert(_factory);
    _peerConnection = _factory->CreatePeerConnection(_config, &_constraints,
                                                     nullptr, nullptr, this);

    if (_stream) {
        if (!_peerConnection->AddStream(_stream)) {
            throw std::runtime_error("Adding stream to PeerConnection failed");
        }
    }
}


void PeerConnection::closeConnection()
{
    DebugL << _peerid << ": Closing @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;

    if (_peerConnection) {
        _peerConnection->Close();
    } 
    else {
        // Call onClosed if no connection has been
        // made so callbacks are always run.
        _manager->onClosed(this);
    }
}


void PeerConnection::createOffer()
{
    assert(_mode == Offer);
    assert(_peerConnection);

    _peerConnection->CreateOffer(this, &_constraints);
}


void PeerConnection::recvSDP(const std::string& type, const std::string& sdp)
{
    DebugL << _peerid << ": Receive " << type << ": " << sdp << endl;

    webrtc::SdpParseError error;
    webrtc::SessionDescriptionInterface* desc(
        webrtc::CreateSessionDescription(type, sdp, &error));
    if (!desc) {
        throw std::runtime_error("Can't parse remote SDP: " + error.description);
    }
    _peerConnection->SetRemoteDescription(
        DummySetSessionDescriptionObserver::Create(), desc);

    if (type == "offer") {
        assert(_mode == Answer);
        _peerConnection->CreateAnswer(this, &_constraints);
    } else {
        assert(_mode == Offer);
    }
}


void PeerConnection::recvCandidate(const std::string& mid, int mlineindex,
                                   const std::string& sdp)
{
    webrtc::SdpParseError error;
    std::unique_ptr<webrtc::IceCandidateInterface> candidate(
        webrtc::CreateIceCandidate(mid, mlineindex, sdp, &error));
    if (!candidate) {
        throw std::runtime_error("Can't parse remote candidate: " + error.description);
    }
    _peerConnection->AddIceCandidate(candidate.get());
}


void PeerConnection::OnSignalingChange(
    webrtc::PeerConnectionInterface::SignalingState new_state)
{
    DebugL << _peerid << ": On signaling state change: " << new_state << endl;

    switch (new_state) {
        case webrtc::PeerConnectionInterface::kStable:
            _manager->onStable(this);
            break;
        case webrtc::PeerConnectionInterface::kClosed:
            _manager->onClosed(this);
            break;
        case webrtc::PeerConnectionInterface::kHaveLocalOffer:
        case webrtc::PeerConnectionInterface::kHaveRemoteOffer:
        case webrtc::PeerConnectionInterface::kHaveLocalPrAnswer:
        case webrtc::PeerConnectionInterface::kHaveRemotePrAnswer:
            break;
    }
}


void PeerConnection::OnIceConnectionChange(
    webrtc::PeerConnectionInterface::IceConnectionState new_state)
{
    DebugL << _peerid << ": On ICE connection change: " << new_state << endl;
}


void PeerConnection::OnIceGatheringChange(
    webrtc::PeerConnectionInterface::IceGatheringState new_state)
{
    DebugL << _peerid << ": On ICE gathering change: " << new_state << endl;
}


void PeerConnection::OnRenegotiationNeeded()
{
    DebugL << _peerid << ": On renegotiation needed" << endl;
}


void PeerConnection::OnAddStream(webrtc::MediaStreamInterface* stream)
{
    assert(_mode == Answer);

    DebugL << _peerid << ": On add stream" << endl;
    _manager->onAddRemoteStream(this, stream);
}


void PeerConnection::OnRemoveStream(webrtc::MediaStreamInterface* stream)
{
    assert(_mode == Answer);

    DebugL << _peerid << ": On remove stream" << endl;
    _manager->onRemoveRemoteStream(this, stream);
}


void PeerConnection::OnIceCandidate(
    const webrtc::IceCandidateInterface* candidate)
{
    std::string sdp;
    if (!candidate->ToString(&sdp)) {
        ErrorL << _peerid << ": Failed to serialize candidate" << endl;
        assert(0);
        return;
    }

    _manager->sendCandidate(this, candidate->sdp_mid(),
                            candidate->sdp_mline_index(), sdp);
}


void PeerConnection::OnSuccess(webrtc::SessionDescriptionInterface* desc)
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

    _manager->sendSDP(this, desc->type(), sdp);
}


void PeerConnection::OnFailure(const std::string& error)
{
    ErrorL << _peerid << ": On failure: " << error << endl;

    _manager->onFailure(this, error);
}


void PeerConnection::setPeerConnectionFactory(
    rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> factory)
{
    assert(!_factory); // should not be already set via PeerConnectionManager
    _factory = factory;
}


std::string PeerConnection::peerid() const
{
    return _peerid;
}


std::string PeerConnection::token() const
{
    return _token;
}


webrtc::FakeConstraints& PeerConnection::constraints()
{
    return _constraints;
}


webrtc::PeerConnectionFactoryInterface* PeerConnection::factory() const
{
    return _factory.get();
}


rtc::scoped_refptr<webrtc::PeerConnectionInterface> PeerConnection::peerConnection() const
{
    return _peerConnection;
}


rtc::scoped_refptr<webrtc::MediaStreamInterface> PeerConnection::stream() const
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


/// @\}
