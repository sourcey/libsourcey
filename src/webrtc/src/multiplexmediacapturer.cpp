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


namespace scy {


MultiplexMediaCapturer::MultiplexMediaCapturer()
    : _capture(std::make_shared<av::MediaCapture>())
    , _audioModule(AudioPacketModule::Create()) //,
// _videoSource(VideoPacketSource::Create())
{
    // _capture->emitter += packetSlot(_audioModule.get(),
    // &AudioPacketModule::onAudioCaptured);

    _stream.attachSource(_capture, true);
    _stream.attach(
        std::make_shared<av::RealtimePacketQueue<av::VideoPacket>>(0), 5);
    _stream.emitter+=
        packetSlot(_audioModule.get(), &AudioPacketModule::onAudioCaptured);
}


MultiplexMediaCapturer::~MultiplexMediaCapturer()
{
    // if (_videoTrack)
    //     _videoTrack->RemoveSink(this);
    //
    // if (_audioTrack)
    //     _audioTrack->RemoveSink(this);
}


void MultiplexMediaCapturer::openFile(const std::string& file)
{
    _capture->openFile(file);
    if (_capture->audio()) {
        _capture->audio()->oparams.sampleFmt= "s16";
        _capture->audio()->oparams.sampleRate= 44000;
        _capture->audio()->oparams.channels= 2;
        _capture->audio()->recreateResampler();
        // _capture->audio()->resampler->maxNumSamples = 440;
        // _capture->audio()->resampler->variableOutput = false;
    }
    if (_capture->video()) {
        _capture->video()->oparams.pixelFmt= "nv12"; // yuv420p
        // _capture->video()->oparams.width = capture_format.width;
        // _capture->video()->oparams.height = capture_format.height;
    }

    // TODO: Set the video packet source format from the video
}


VideoPacketSource* MultiplexMediaCapturer::createVideoSource()
{
    auto videoSource= new VideoPacketSource();
    _stream.emitter+=
        packetSlot(videoSource, &VideoPacketSource::onVideoCaptured);
    return videoSource;
}


rtc::scoped_refptr<AudioPacketModule> MultiplexMediaCapturer::getAudioModule()
{
    return _audioModule;
}


void MultiplexMediaCapturer::addMediaTracks(
    webrtc::PeerConnectionFactoryInterface* factory,
    webrtc::MediaStreamInterface* stream)
{
    // Create and add the audio stream
    if (_capture->audio()) {
        stream->AddTrack(factory->CreateAudioTrack(
            kAudioLabel, factory->CreateAudioSource(nullptr)));
    }

    // Create and add the video stream
    if (_capture->video()) {
        stream->AddTrack(factory->CreateVideoTrack(
            kVideoLabel,
            factory->CreateVideoSource(createVideoSource(), nullptr)));
    }
}


void MultiplexMediaCapturer::start()
{
    // _capture->start();
    _stream.start();
}


void MultiplexMediaCapturer::stop()
{
    // _capture->stop();
    _stream.stop();
}


} // namespace scy


/// @\}
