///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup webrtc
/// @{


#ifndef SCY_PeerConnection_H
#define SCY_PeerConnection_H


#include "webrtc/api/jsep.h"
#include "webrtc/api/peerconnectioninterface.h"
#include "webrtc/api/test/fakeconstraints.h"


namespace scy {


class PeerConnectionManager;


class PeerConnection : public webrtc::PeerConnectionObserver,
                       public webrtc::CreateSessionDescriptionObserver
{
public:
    enum Mode
    {
        Offer, ///< Operating as offerer
        Answer ///< Operating as answerer
    };

    PeerConnection(PeerConnectionManager* manager,
                   const std::string& peerid,
                   const std::string& token,
                   Mode mode);
    virtual ~PeerConnection();

    /// Create the local media stream.
    /// Only necessary when we are creating the offer.
    virtual rtc::scoped_refptr<webrtc::MediaStreamInterface> createMediaStream();

    /// Create the peer connection once configuration, constraints and
    /// streams have been set.
    virtual void createConnection();

    /// Close the peer connection.
    virtual void closeConnection();

    /// Create the offer SDP tos end to the peer.
    /// No offer should be received after creating the offer.
    /// A call to `recvRemoteAnswer` is expected to initiate the session.
    virtual void createOffer();

    /// Receive a remote offer or answer.
    virtual void recvSDP(const std::string& type, const std::string& sdp);

    /// Receive a remote candidate.
    virtual void recvCandidate(const std::string& mid, int mlineindex, const std::string& sdp);

    /// Set a custom PeerConnectionFactory object
    /// Must be done before any streams are initiated
    void setPeerConnectionFactory(
        rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> factory);

    std::string peerid() const;
    std::string token() const;
    webrtc::FakeConstraints& constraints();
    webrtc::PeerConnectionFactoryInterface* factory() const;
    rtc::scoped_refptr<webrtc::PeerConnectionInterface> peerConnection() const;
    rtc::scoped_refptr<webrtc::MediaStreamInterface> stream() const;

protected:
    /// inherited from PeerConnectionObserver
    virtual void OnAddStream(webrtc::MediaStreamInterface* stream);
    virtual void OnRemoveStream(webrtc::MediaStreamInterface* stream);
    virtual void OnIceCandidate(const webrtc::IceCandidateInterface* candidate);
    virtual void OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState new_state);
    virtual void OnIceConnectionChange(webrtc::PeerConnectionInterface::IceConnectionState new_state);
    virtual void OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState new_state);
    virtual void OnRenegotiationNeeded();

    /// inherited from CreateSessionDescriptionObserver
    virtual void OnSuccess(webrtc::SessionDescriptionInterface* desc);
    virtual void OnFailure(const std::string& error);

    virtual int AddRef() const { return 1; }
    virtual int Release() const { return 0; }

protected:
    PeerConnectionManager* _manager;
    std::string _peerid;
    std::string _token;
    Mode _mode;
    webrtc::PeerConnectionInterface::RTCConfiguration _config;
    webrtc::FakeConstraints _constraints;
    rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> _factory;
    rtc::scoped_refptr<webrtc::PeerConnectionInterface> _peerConnection;
    rtc::scoped_refptr<webrtc::MediaStreamInterface> _stream;
};


class DummySetSessionDescriptionObserver
    : public webrtc::SetSessionDescriptionObserver
{
public:
    static DummySetSessionDescriptionObserver* Create()
    {
        return new rtc::RefCountedObject<DummySetSessionDescriptionObserver>();
    }
    virtual void OnSuccess();
    virtual void OnFailure(const std::string& error);

protected:
    DummySetSessionDescriptionObserver() {}
    ~DummySetSessionDescriptionObserver() {}
};


} // namespace scy


#endif


/// @\}
