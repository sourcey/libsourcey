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

#ifndef SCY_WebRTC_StreamRecorder_H
#define SCY_WebRTC_StreamRecorder_H


#include "scy/av/multiplexencoder.h"

#include "webrtc/api/peerconnectioninterface.h"


namespace scy {


class StreamRecorder
    : public rtc::VideoSinkInterface<cricket::VideoFrame>,
      public webrtc::AudioTrackSinkInterface
{
public:
    StreamRecorder(const av::EncoderOptions& options);
    ~StreamRecorder();

    void setVideoTrack(webrtc::VideoTrackInterface* track);
    void setAudioTrack(webrtc::AudioTrackInterface* track);

    void onPacketEncoded(av::MediaPacket& packet);

    // VideoSinkInterface implementation
    void OnFrame(const cricket::VideoFrame& frame) override;

    // AudioTrackSinkInterface implementation
    void OnData(const void* audio_data,
                int bits_per_sample,
                int sample_rate,
                size_t number_of_channels,
                size_t number_of_frames) override;

protected:
    av::MultiplexEncoder _encoder;
    rtc::scoped_refptr<webrtc::VideoTrackInterface> _videoTrack;
    rtc::scoped_refptr<webrtc::AudioTrackInterface> _audioTrack;
    bool _awaitingVideo;
    bool _awaitingAudio;
};


} // namespace scy


#endif
