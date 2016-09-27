#ifndef SCY_PeerConnectionClient_H
#define SCY_PeerConnectionClient_H


#include "webrtc/api/peerconnectioninterface.h"
#include "webrtc/api/jsep.h"
#include "webrtc/api/datachannelinterface.h"
#include "webrtc/api/test/fakeconstraints.h"


namespace scy {


class Signaler;


class PeerConnectionClient :
    public webrtc::PeerConnectionObserver,
    public webrtc::CreateSessionDescriptionObserver {
public:
    PeerConnectionClient(Signaler& client, const std::string& peerid);
    ~PeerConnectionClient();

    bool initConnection();
    bool closeConnection();

    void createOffer();

    // bool handleRemoteOffer(const std::string& type, const std::string sdp);
    bool handleRemoteAnswer(const std::string& type, const std::string sdp);
    bool handleRemoteCandidate(const std::string& mid, int mlineindex, const std::string sdp);

    // bool InitConnection();
    // void CreateOffer();

    // bool OnRemoteOffer(const std::string& type, const std::string sdp);
    // bool OnRemoteAnswer(const std::string& type, const std::string sdp);
    // bool OnRemoteCandidate(const std::string& mid, int mlineindex, const std::string sdp);

    // virtual const webrtc::SessionDescriptionInterface* local_description() const {
    //         return peer_connection_->local_description();
    // }

protected:
    // cricket::VideoCapturer* OpenVideoCaptureDevice();

    // // inherited from PeerConnectionObserver
    // virtual void OnError();
    // virtual void OnStateChange(webrtc::PeerConnectionObserver::StateType state_changed);
    // virtual void OnAddStream(webrtc::MediaStreamInterface* stream);
    // virtual void OnRemoveStream(webrtc::MediaStreamInterface* stream);
    // virtual void OnIceCandidate(const webrtc::IceCandidateInterface* candidate);
    //
    // // inherited from CreateSessionDescriptionObserver
    // virtual void OnSuccess(webrtc::SessionDescriptionInterface* desc);
    // virtual void OnFailure(const std::string &error);
    // virtual int AddRef() { return 1; }
    // virtual int Release() { return 0; }

    // inherited from PeerConnectionObserver
    // virtual void OnError();
    virtual void OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState new_state);
    // virtual void OnStateChange(webrtc::PeerConnectionObserver::StateType state_changed);

    virtual void OnAddStream(webrtc::MediaStreamInterface* stream);
    virtual void OnRemoveStream(webrtc::MediaStreamInterface* stream);
    virtual void OnIceCandidate(const webrtc::IceCandidateInterface* candidate);
    virtual void OnIceConnectionChange(webrtc::PeerConnectionInterface::IceConnectionState new_state);
    virtual void OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState new_state);
    virtual void OnRenegotiationNeeded();

    // inherited from CreateSessionDescriptionObserver
    virtual void OnSuccess(webrtc::SessionDescriptionInterface* desc);
    virtual void OnFailure(const std::string& error);

    virtual int AddRef() const { return 1; }
    virtual int Release() const { return 0; }

private:
    Signaler& _client;
    std::string _peerid;

    webrtc::PeerConnectionInterface::RTCConfiguration config_;
    // webrtc::PeerConnectionInterface::IceServers servers_;
    webrtc::PeerConnectionInterface::IceServer server_;
    webrtc::FakeConstraints constraints_;
    // webrtc::FakeConstraints sdpConstraints_;
    // webrtc::FakeConstraints mediaConstraints_;
    rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> peer_connection_factory_;
    rtc::scoped_refptr<webrtc::PeerConnectionInterface> peer_connection_;
    rtc::scoped_refptr<webrtc::MediaStreamInterface> stream_;
};


class DummySetSessionDescriptionObserver :
    public webrtc::SetSessionDescriptionObserver {
public:
    static DummySetSessionDescriptionObserver* Create() {
        return new rtc::RefCountedObject<DummySetSessionDescriptionObserver>();
    }
    virtual void OnSuccess() {}
    virtual void OnFailure(const std::string& error) { assert(0); }

protected:
    DummySetSessionDescriptionObserver() {}
    ~DummySetSessionDescriptionObserver() {}
};


} // namespace scy


#endif
