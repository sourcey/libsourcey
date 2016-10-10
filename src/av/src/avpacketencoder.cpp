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


#include "scy/av/avpacketencoder.h"
#ifdef HAVE_FFMPEG


using std::endl;


namespace scy {
namespace av {


AVPacketEncoder::AVPacketEncoder(const EncoderOptions& options, bool muxLiveStreams) :
    AVEncoder(options),
    PacketProcessor(AVEncoder::emitter),
    _muxLiveStreams(muxLiveStreams),
    _lastVideoPacket(nullptr)
{
}


AVPacketEncoder::AVPacketEncoder(bool muxLiveStreams) :
    AVEncoder(),
    PacketProcessor(AVEncoder::emitter),
    _muxLiveStreams(muxLiveStreams),
    _lastVideoPacket(nullptr)
{
}


AVPacketEncoder::~AVPacketEncoder()
{
}


void AVPacketEncoder::process(IPacket& packet)
{
    Mutex::ScopedLock lock(_mutex);

    TraceS(this) << "processing" << std::endl;

    // We may be receiving either audio or video packets
    auto vPacket = dynamic_cast<VideoPacket*>(&packet);
    auto aPacket = vPacket ? nullptr : dynamic_cast<AudioPacket*>(&packet);
    if (!vPacket && !aPacket)
        throw std::invalid_argument("Unknown media packet type.");

    // Do some special synchronizing for muxing live variable framerate streams
    if (_muxLiveStreams) {
        auto video = AVEncoder::video();
        auto audio = AVEncoder::audio();
        assert(audio && video);
        double audioPts, videoPts;
        int times = 0;
        for (;;) {
            times++;
            assert(times < 10);
            audioPts = audio ? (double)audio->stream->pts.val * audio->stream->time_base.num / audio->stream->time_base.den : 0.0;
            videoPts = video ? (double)video->stream->pts.val * video->stream->time_base.num / video->stream->time_base.den : 0.0;
            if (aPacket) {
                // Write the audio packet when the encoder is ready
                if (!video || audioPts < videoPts) {
                    encode(*aPacket);
                    break;
                }

                // Write dummy video frames until we can encode the audio
                else {
                    // May be null if the first packet was audio, skip...
                    if (!_lastVideoPacket)
                        break;

                    encode(*_lastVideoPacket);
                }
            }
            else if (vPacket) {
                // Write the video packet if the encoder is ready
                if (!audio || audioPts > videoPts)
                    encode(*vPacket);

                if (audio) {
                    // Clone and buffer the last video packet it can be used
                    // as soon as we need an available frame.
                    // used as a filler if the source framerate is inconstant.
                    if (_lastVideoPacket)
                        delete _lastVideoPacket;
                    _lastVideoPacket = reinterpret_cast<scy::av::VideoPacket*>(vPacket->clone());
                }
                break;
            }
        }
    }
    else if (vPacket) {
        encode(*vPacket);
    }
    else if (aPacket) {
        encode(*aPacket);
    }
}


void AVPacketEncoder::encode(VideoPacket& packet)
{
    encodeVideo((unsigned char*)packet.data(), packet.size(), packet.width, packet.height, (std::uint64_t)packet.time);
}


void AVPacketEncoder::encode(AudioPacket& packet)
{
    encodeAudio((unsigned char*)packet.data(), packet.size(), packet.frameSize, (std::uint64_t)packet.time);
}


bool AVPacketEncoder::accepts(IPacket& packet)
{
    return dynamic_cast<av::MediaPacket*>(&packet) != 0;
}


void AVPacketEncoder::onStreamStateChange(const PacketStreamState& state)
{
    TraceS(this) << "On stream state change: " << state << endl;

    Mutex::ScopedLock lock(_mutex);

    switch (state.id()) {
    case PacketStreamState::Active:
        if (!isActive()) {
            TraceS(this) << "Initializing" << endl;
            //if (AVEncoder::options().oformat.video.enabled &&
            //    AVEncoder::options().oformat.audio.enabled)
            //    _muxLiveStreams = true;
            AVEncoder::initialize();
        }
        break;

    // case PacketStreamState::Resetting:
    case PacketStreamState::Stopping:
        if (isActive()) {
            TraceS(this) << "Uninitializing" << endl;
            AVEncoder::flush();
            AVEncoder::uninitialize();
        }
        break;
    //case PacketStreamState::Stopped:
    //case PacketStreamState::Error:
    //case PacketStreamState::None:
    //case PacketStreamState::Closed:
    }

    TraceS(this) << "Stream state change: OK: " << state << endl;
}


} } // namespace scy::av


#endif
