///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup webrtc
/// @{


#include "scy/webrtc/peerfactorycontext.h"
#include "scy/webrtc/fakeaudiodevicemodule.h"
//#include "webrtc/pc/test/fakeaudiocapturemodule.h"
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
namespace wrtc {


PeerFactoryContext::PeerFactoryContext(
    webrtc::AudioDeviceModule* default_adm,
    cricket::WebRtcVideoEncoderFactory* video_encoder_factory,
    cricket::WebRtcVideoDecoderFactory* video_decoder_factory,
    rtc::scoped_refptr<webrtc::AudioEncoderFactory> audio_encoder_factory,
    rtc::scoped_refptr<webrtc::AudioDecoderFactory> audio_decoder_factory)
    // : audioDeviceManager(default_adm)
{
    // Setup threads
    networkThread = rtc::Thread::CreateWithSocketServer();
    workerThread = rtc::Thread::Create();
    if (!networkThread->Start() || !workerThread->Start())
        throw std::runtime_error("Failed to start WebRTC threads");

    // Create the factory
    if (audio_encoder_factory || audio_decoder_factory) {
        factory = webrtc::CreatePeerConnectionFactory(
            networkThread.get(), workerThread.get(), rtc::Thread::Current(),
            default_adm, audio_encoder_factory, audio_decoder_factory,
            video_encoder_factory, video_decoder_factory);
    }
    else {
        //if (!audioDeviceManager)
        //    audioDeviceManager = wrtc::FakeAudioDeviceModule::Create();
            //audioDeviceManager = FakeAudioCaptureModule::Create();
        factory = webrtc::CreatePeerConnectionFactory(
            networkThread.get(), workerThread.get(), rtc::Thread::Current(),
            default_adm, video_encoder_factory, video_decoder_factory);
            // audioDeviceManager.get(), video_encoder_factory, video_decoder_factory);
        //factory = webrtc::CreatePeerConnectionFactory();
    }
    if (!factory)
        throw std::runtime_error("Failed to create WebRTC factory");
}


void PeerFactoryContext::initCustomNetworkManager()
{
    networkManager.reset(new rtc::BasicNetworkManager());
    socketFactory.reset(new rtc::BasicPacketSocketFactory(networkThread.get()));
}


} } // namespace scy::wrtc


/// @\}
