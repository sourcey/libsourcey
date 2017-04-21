///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup webrtc
/// @{


#include "scy/webrtc/multiplexmediacapturer.h"

#include "scy/av/audioresampler.h"
#include "scy/av/ffmpeg.h"
#include "scy/av/realtimepacketqueue.h"
#include "scy/filesystem.h"
#include "scy/logger.h"
#include "scy/webrtc/webrtc.h"
#include "webrtc/media/engine/webrtcvideocapturerfactory.h"
#include "webrtc/modules/video_capture/video_capture_factory.h"


namespace scy {


MultiplexMediaCapturer::MultiplexMediaCapturer()
    : _videoCapture(std::make_shared<av::MediaCapture>())
    , _audioModule(AudioPacketModule::Create())
{
    _stream.attachSource(_videoCapture, true);
    //_stream.attach(std::make_shared<av::RealtimePacketQueue<av::MediaPacket>>(0), 5);
    //_stream.attach(std::make_shared<av::RealtimePacketQueue<av::PlanarVideoPacket>>(0), 5);
    _stream.emitter += packetSlot(_audioModule.get(), &AudioPacketModule::onAudioCaptured);
}


MultiplexMediaCapturer::~MultiplexMediaCapturer()
{
}


void MultiplexMediaCapturer::openFile(const std::string& file, bool loop)
{
    // Open the capture file
    _videoCapture->setLoopInput(loop);
    _videoCapture->setRealtimePlayback(true);
    _videoCapture->openFile(file);

    // Set the output settings
    if (_videoCapture->audio()) {
        _videoCapture->audio()->oparams.sampleFmt = "s16";
        _videoCapture->audio()->oparams.sampleRate = 44000;
        _videoCapture->audio()->oparams.channels = 2;
        _videoCapture->audio()->recreateResampler();
        // _videoCapture->audio()->resampler->maxNumSamples = 440;
        // _videoCapture->audio()->resampler->variableOutput = false;
    }

    // Convert to yuv420p for easy comsumption by WebRTC
    if (_videoCapture->video()) {
        _videoCapture->video()->oparams.pixelFmt = "yuv420p"; // nv12
        // _videoCapture->video()->oparams.width = capture_format.width;
        // _videoCapture->video()->oparams.height = capture_format.height;
    }
}


VideoPacketSource* MultiplexMediaCapturer::createVideoSource()
{
    assert(_videoCapture->video());
    auto oparams = _videoCapture->video()->oparams;
    auto source = new VideoPacketSource(oparams.width, oparams.height, 
                                        oparams.fps, cricket::FOURCC_I420);
    source->setPacketSource(&_stream.emitter); // nullified on VideoPacketSource::Stop
    return source;
} 


rtc::scoped_refptr<AudioPacketModule> MultiplexMediaCapturer::getAudioModule()
{
    return _audioModule;
}


// TEST: Open VideoCaptureDevice using the WebRTC way 
#if 0
std::unique_ptr<cricket::VideoCapturer> openVideoDefaultWebRtcCaptureDevice() 
{
    std::vector<std::string> deviceNames;
    {
        std::unique_ptr<webrtc::VideoCaptureModule::DeviceInfo> info(
            webrtc::VideoCaptureFactory::CreateDeviceInfo());
        if (!info) {
            return nullptr;
        }
        int numDevicess = info->NumberOfDevices();
        assert(numDevicess > 0);
        for (int i = 0; i < numDevicess; ++i) {
            const uint32_t kSize = 256;
            char name[kSize] = { 0 };
            char id[kSize] = { 0 };
            if (info->GetDeviceName(i, name, kSize, id, kSize) != -1) {
                deviceNames.push_back(name);
            }
        }
    }

    cricket::WebRtcVideoDeviceCapturerFactory factory;
    std::unique_ptr<cricket::VideoCapturer> capturer;
    for (const auto& name : deviceNames) {
        capturer = factory.Create(cricket::Device(name, 0));
        if (capturer) {
            break;
        }
    }

    assert(capturer);
    return capturer;
}
#endif


void MultiplexMediaCapturer::addMediaTracks(
    webrtc::PeerConnectionFactoryInterface* factory,
    webrtc::MediaStreamInterface* stream)
{
    // This capturer is multicast, meaning it can be used as the source 
    // for multiple PeerConnection objects.
    //
    // KLUDGE: Pixel format conversion should happen on the 
    // VideoPacketSource rather than on the decoder becasue different  
    // peers may request different optimal output video sizes.

    // Create and add the audio stream
    if (_videoCapture->audio()) {
        stream->AddTrack(factory->CreateAudioTrack(
            kAudioLabel, factory->CreateAudioSource(nullptr)));
    }

    // Create and add the video stream
    if (_videoCapture->video()) {
        stream->AddTrack(factory->CreateVideoTrack(
            kVideoLabel, factory->CreateVideoSource(createVideoSource(), nullptr)));
    }

    // Default WebRTC video stream
    // stream->AddTrack(factory->CreateVideoTrack(
    //     kVideoLabel, factory->CreateVideoSource(openVideoDefaultWebRtcCaptureDevice(), nullptr)));
}


void MultiplexMediaCapturer::start()
{
    // _videoCapture->start();
    _stream.start();
}


void MultiplexMediaCapturer::stop()
{
    // _videoCapture->stop();
    _stream.stop();
}


} // namespace scy


/// @\}
