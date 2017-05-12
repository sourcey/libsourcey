///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup webrtc
/// @{


#ifndef SCY_WebRTC_PeerConnectionFactory_H
#define SCY_WebRTC_PeerConnectionFactory_H


#include "scy/webrtc/webrtc.h"

#include "webrtc/pc/peerconnectionfactory.h"


namespace scy {


/// Extended PeerConnectionFactory that allows access to private 
/// methods that are restricted by the base implementation.
class PeerConnectionFactory : public webrtc::PeerConnectionFactory 
{
public:
    /// Create a new instance of PeerConnectionFactoryInterface 
    /// using default parameters.
    static rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> create();

    /// Create a new instance of PeerConnectionFactoryInterface.
    ///
    /// |network_thread|, |worker_thread| and |signaling_thread| are
    /// the only mandatory parameters.
    ///
    /// If non-null, a reference is added to |default_adm|, and ownership of
    /// |video_encoder_factory| and |video_decoder_factory| is transferred to the
    /// returned factory.
    /// TODO(deadbeef): Use rtc::scoped_refptr<> and std::unique_ptr<> to make this
    /// ownership transfer and ref counting more obvious.
    static rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> create(
        rtc::Thread* network_thread,
        rtc::Thread* worker_thread,
        rtc::Thread* signaling_thread,
        webrtc::AudioDeviceModule* default_adm,
        rtc::scoped_refptr<webrtc::AudioEncoderFactory> audio_encoder_factory,
        rtc::scoped_refptr<webrtc::AudioDecoderFactory> audio_decoder_factory,
        cricket::WebRtcVideoEncoderFactory* video_encoder_factory,
        cricket::WebRtcVideoDecoderFactory* video_decoder_factory,
        rtc::scoped_refptr<webrtc::AudioMixer> audio_mixer);

    /// Create a Peer Connection.
    // virtual rtc::scoped_refptr<webrtc::PeerConnectionInterface> CreatePeerConnection(
    //     const webrtc::PeerConnectionInterface::RTCConfiguration& configuration,
    //     std::unique_ptr<cricket::PortAllocator> allocator,
    //     std::unique_ptr<rtc::RTCCertificateGeneratorInterface> cert_generator,
    //     webrtc::PeerConnectionObserver* observer) override;

    /// Restrict the port range used by Peer Connections.
    // void setPortRange(int minPort, int maxPort);

    rtc::NetworkManager* networkManager();
    rtc::PacketSocketFactory* socketFactory();

protected:
    PeerConnectionFactory(
        rtc::scoped_refptr<webrtc::AudioEncoderFactory> audio_encoder_factory,
        rtc::scoped_refptr<webrtc::AudioDecoderFactory> audio_decoder_factory);

    PeerConnectionFactory(
        rtc::Thread* network_thread,
        rtc::Thread* worker_thread,
        rtc::Thread* signaling_thread,
        webrtc::AudioDeviceModule* default_adm,
        rtc::scoped_refptr<webrtc::AudioEncoderFactory> audio_encoder_factory,
        rtc::scoped_refptr<webrtc::AudioDecoderFactory> audio_decoder_factory,
        cricket::WebRtcVideoEncoderFactory* video_encoder_factory,
        cricket::WebRtcVideoDecoderFactory* video_decoder_factory,
        rtc::scoped_refptr<webrtc::AudioMixer> audio_mixer);

    virtual ~PeerConnectionFactory();

    bool initialize();

protected:
    std::unique_ptr<rtc::BasicNetworkManager> _networkManager;
    std::unique_ptr<rtc::BasicPacketSocketFactory> _socketFactory;
    // int _minPort;
    // int _maxPort;
};


} // namespace scy


#endif


/// @\}
