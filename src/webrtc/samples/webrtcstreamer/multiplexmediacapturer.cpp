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


#include "multiplexmediacapturer.h"

#include "scy/logger.h"
#include "scy/filesystem.h"
#include "scy/webrtc/webrtc.h"
#include "scy/av/ffmpeg.h"
#include "scy/av/realtimepacketqueue.h"
#include "scy/av/audioresampler.h"


namespace scy {


MultiplexMediaCapturer::MultiplexMediaCapturer()
    : _capture(std::make_shared<av::MediaCapture>())
    , _audioModule(AudioPacketModule::Create())//,
    // _videoSource(VideoPacketSource::Create())
{
    // _capture->emitter += packetDelegate(_audioModule.get(), &AudioPacketModule::onAudioCaptured);

    _stream.attachSource(_capture, true);
    _stream.attach(std::make_shared<av::RealtimePacketQueue<av::VideoPacket>>(0), 5);
    _stream.emitter += packetDelegate(_audioModule.get(), &AudioPacketModule::onAudioCaptured);
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
        _capture->audio()->oparams.sampleFmt = "s16";
        _capture->audio()->oparams.sampleRate = 44000;
        _capture->audio()->oparams.channels = 2;
        _capture->audio()->recreateResampler();
        // _capture->audio()->resampler->maxNumSamples = 440;
        // _capture->audio()->resampler->variableOutput = false;
    }
    if (_capture->video()) {
        _capture->video()->oparams.pixelFmt = "nv12"; //yuv420p
        // _capture->video()->oparams.width = capture_format.width;
        // _capture->video()->oparams.height = capture_format.height;
    }

    // TODO: Set the video packet source format from the video
}


VideoPacketSource* MultiplexMediaCapturer::createVideoSource()
{
    auto videoSource = new VideoPacketSource();
    _stream.emitter += packetDelegate(videoSource, &VideoPacketSource::onVideoCaptured);
    return videoSource;
}


rtc::scoped_refptr<AudioPacketModule> MultiplexMediaCapturer::getAudioModule()
{
    return _audioModule;
}


void MultiplexMediaCapturer::addMediaTracks(webrtc::PeerConnectionFactoryInterface* factory, webrtc::MediaStreamInterface* stream)
{
    // Create and add the audio stream
    if (_capture->audio()) {
        stream->AddTrack(
            factory->CreateAudioTrack(kAudioLabel,
                factory->CreateAudioSource(nullptr)));
    }

    // Create and add the video stream
    if (_capture->video()) {
        // rtc::scoped_refptr<webrtc::VideoTrackInterface> videoTrack(
        //     factory->CreateVideoTrack(kVideoLabel,
        //         factory->CreateVideoSource(_videoSource, nullptr)));

        stream->AddTrack(
            factory->CreateVideoTrack(kVideoLabel,
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
