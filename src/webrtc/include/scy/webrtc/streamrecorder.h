///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup webrtc
/// @{


#ifndef SCY_WebRTC_StreamRecorder_H
#define SCY_WebRTC_StreamRecorder_H


#include "scy/av/multiplexencoder.h"

#include "webrtc/api/peerconnectioninterface.h"


namespace scy {


class StreamRecorder : public rtc::VideoSinkInterface<cricket::VideoFrame>,
                       public webrtc::AudioTrackSinkInterface
{
public:
    StreamRecorder(const av::EncoderOptions& options);
    ~StreamRecorder();

    void setVideoTrack(webrtc::VideoTrackInterface* track);
    void setAudioTrack(webrtc::AudioTrackInterface* track);

    void onPacketEncoded(av::MediaPacket& packet);

    /// VideoSinkInterface implementation
    void OnFrame(const cricket::VideoFrame& frame) override;

    /// AudioTrackSinkInterface implementation
    void OnData(const void* audio_data, int bits_per_sample, int sample_rate,
                size_t number_of_channels, size_t number_of_frames) override;

protected:
    av::MultiplexEncoder _encoder;
    rtc::scoped_refptr<webrtc::VideoTrackInterface> _videoTrack;
    rtc::scoped_refptr<webrtc::AudioTrackInterface> _audioTrack;
    bool _awaitingVideo;
    bool _awaitingAudio;
};


} // namespace scy


#endif


/// @\}
