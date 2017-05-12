///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup webrtc
/// @{


#include "scy/webrtc/peerconnectionfactory.h"
#include "scy/logger.h"

#include "webrtc/api/peerconnectionfactoryproxy.h"
#include "webrtc/api/peerconnectionproxy.h"
#include "webrtc/api/audio_codecs/builtin_audio_decoder_factory.h"
#include "webrtc/modules/audio_coding/codecs/builtin_audio_encoder_factory.h"
#include "webrtc/p2p/base/basicpacketsocketfactory.h"
#include "webrtc/p2p/client/basicportallocator.h"
#include "webrtc/pc/peerconnection.h"


using std::endl;


namespace scy {


rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> 
PeerConnectionFactory::create() 
{
    rtc::scoped_refptr<PeerConnectionFactory> factory(
        new rtc::RefCountedObject<PeerConnectionFactory>(
            webrtc::CreateBuiltinAudioEncoderFactory(),
            webrtc::CreateBuiltinAudioDecoderFactory()));

    RTC_CHECK(rtc::Thread::Current() == factory->signaling_thread());
    // The signaling thread is the current thread so we can
    // safely call Initialize directly.
    if (!factory->initialize()) {
        return nullptr;
    }
    return webrtc::PeerConnectionFactoryProxy::Create(factory->signaling_thread(), factory);
}


rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> 
PeerConnectionFactory::create(
    rtc::Thread* network_thread,
    rtc::Thread* worker_thread,
    rtc::Thread* signaling_thread,
    webrtc::AudioDeviceModule* default_adm,
    rtc::scoped_refptr<webrtc::AudioEncoderFactory> audio_encoder_factory,
    rtc::scoped_refptr<webrtc::AudioDecoderFactory> audio_decoder_factory,
    cricket::WebRtcVideoEncoderFactory* video_encoder_factory,
    cricket::WebRtcVideoDecoderFactory* video_decoder_factory,
    rtc::scoped_refptr<webrtc::AudioMixer> audio_mixer) 
{
    rtc::scoped_refptr<PeerConnectionFactory> factory(
        new rtc::RefCountedObject<PeerConnectionFactory>(
            network_thread, worker_thread, signaling_thread, default_adm,
            audio_encoder_factory, audio_decoder_factory, video_encoder_factory,
            video_decoder_factory, audio_mixer));

    // Call Initialize synchronously but make sure it is executed on
    // |signaling_thread|.
    webrtc::MethodCall0<PeerConnectionFactory, bool> call(
        factory.get(), &PeerConnectionFactory::initialize);
    bool result = call.Marshal(RTC_FROM_HERE, signaling_thread);

    if (!result) {
        return nullptr;
    }
    return webrtc::PeerConnectionFactoryProxy::Create(signaling_thread, factory);
}


PeerConnectionFactory::PeerConnectionFactory(
    rtc::scoped_refptr<webrtc::AudioEncoderFactory> audio_encoder_factory,
    rtc::scoped_refptr<webrtc::AudioDecoderFactory> audio_decoder_factory)
    : webrtc::PeerConnectionFactory(audio_encoder_factory, 
                                    audio_decoder_factory) 
{
}


PeerConnectionFactory::PeerConnectionFactory(
    rtc::Thread* network_thread,
    rtc::Thread* worker_thread,
    rtc::Thread* signaling_thread,
    webrtc::AudioDeviceModule* default_adm,
    rtc::scoped_refptr<webrtc::AudioEncoderFactory> audio_encoder_factory,
    rtc::scoped_refptr<webrtc::AudioDecoderFactory> audio_decoder_factory,
    cricket::WebRtcVideoEncoderFactory* video_encoder_factory,
    cricket::WebRtcVideoDecoderFactory* video_decoder_factory,
    rtc::scoped_refptr<webrtc::AudioMixer> audio_mixer)
    : webrtc::PeerConnectionFactory(network_thread, 
        worker_thread, signaling_thread, default_adm, 
        audio_encoder_factory, audio_decoder_factory, 
        video_encoder_factory, video_decoder_factory, 
        audio_mixer) 
    // , _minPort(0)
    // , _maxPort(0) 
{
}
    

PeerConnectionFactory::~PeerConnectionFactory()
{
}


bool PeerConnectionFactory::initialize() 
{
    _networkManager.reset(new rtc::BasicNetworkManager());
    if (!_networkManager) {
        return false;
    }

    _socketFactory.reset(new rtc::BasicPacketSocketFactory(network_thread()));
    if (!_socketFactory) {
        return false;
    }

    // Initialize the base implementation.
    // NOTE: This will cause double initialization of underlying 
    // network manager and socket factory, but it's cleaner than  
    // cloning the entire factory.
    return webrtc::PeerConnectionFactory::Initialize();
}


rtc::NetworkManager* PeerConnectionFactory::networkManager()
{
    return _networkManager.get();
}


rtc::PacketSocketFactory* PeerConnectionFactory::socketFactory()
{
    return _socketFactory.get();
}


// rtc::scoped_refptr<webrtc::PeerConnectionInterface>
// PeerConnectionFactory::CreatePeerConnection(
//     const webrtc::PeerConnectionInterface::RTCConfiguration& configuration,
//     std::unique_ptr<cricket::PortAllocator> allocator,
//     std::unique_ptr<rtc::RTCCertificateGeneratorInterface> cert_generator,
//     webrtc::PeerConnectionObserver* observer) 
// {
//     RTC_DCHECK(signaling_thread()->IsCurrent());

//     if (!cert_generator.get()) {
//         // No certificate generator specified, use the default one.
//         cert_generator.reset(
//             new rtc::RTCCertificateGenerator(signaling_thread(), network_thread()));
//     }

//     if (!allocator) {
//         allocator.reset(new cricket::BasicPortAllocator(
//             _networkManager.get(), _socketFactory.get()));
//         if (_minPort || _maxPort) {
//             allocator->SetPortRange(_minPort, _maxPort);
//             assert(0);
//         }
//     }
//     std::cout << "AAAAAAAAAAAA: " << this << std::endl;
//     std::cout << "AAAAAAAAAAAA: " << allocator.get() << std::endl;
//     std::cout << "AAAAAAAAAAAA: " << _minPort << std::endl;
//     std::cout << "AAAAAAAAAAAA: " << _maxPort << std::endl;
//     assert(0);
//     network_thread()->Invoke<void>(
//         RTC_FROM_HERE, rtc::Bind(&cricket::PortAllocator::SetNetworkIgnoreMask,
//                                  allocator.get(), options().network_ignore_mask));

//     rtc::scoped_refptr<webrtc::PeerConnection> pc(
//         new rtc::RefCountedObject<webrtc::PeerConnection>(this));

//     if (!pc->Initialize(configuration, std::move(allocator),
//                       std::move(cert_generator), observer)) {
//         return nullptr;
//     }
//     return webrtc::PeerConnectionProxy::Create(signaling_thread(), pc);
// }
    

// void PeerConnectionFactory::setPortRange(int minPort, int maxPort)
// {
//     _minPort = minPort;
//     _maxPort = maxPort;


//     std::cout << "##################################AAAAAAAAAAAA: " << this << std::endl;
//     std::cout << "##################################AAAAAAAAAAAA: " << _minPort << std::endl;
// }


} // namespace scy


/// @\}
