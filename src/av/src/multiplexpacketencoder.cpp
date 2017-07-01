///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup av
/// @{


#include "scy/av/multiplexpacketencoder.h"
#ifdef HAVE_FFMPEG


using std::endl;


namespace scy {
namespace av {


MultiplexPacketEncoder::MultiplexPacketEncoder(const EncoderOptions& options)
    : MultiplexEncoder(options)
    , PacketProcessor(MultiplexEncoder::emitter)
{
}


MultiplexPacketEncoder::~MultiplexPacketEncoder()
{
}


#if 0
void MultiplexPacketEncoder::process(IPacket& packet)
{
    std::lock_guard<std::mutex> guard(_mutex);

    LTrace("processing")

    // We may be receiving either audio or video packets
    auto vPacket = dynamic_cast<VideoPacket*>(&packet);
    auto aPacket = vPacket ? nullptr : dynamic_cast<AudioPacket*>(&packet);
    if (!vPacket && !aPacket)
        throw std::invalid_argument("Unknown media packet type.");

    // Do some special synchronizing for muxing live variable framerate streams
    if (_muxLiveStreams) {
        auto video = MultiplexEncoder::video();
        auto audio = MultiplexEncoder::audio();
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
#endif


void MultiplexPacketEncoder::process(IPacket& packet)
{
    std::lock_guard<std::mutex> guard(_mutex);

    LTrace("Processing")

    // We may be receiving either audio or video packets
    auto vPacket = dynamic_cast<VideoPacket*>(&packet);
    auto aPacket = vPacket ? nullptr : dynamic_cast<AudioPacket*>(&packet);
    if (!vPacket && !aPacket)
        throw std::invalid_argument("Unknown media packet type.");

    if (vPacket) {
        encode(*vPacket);
    } else if (aPacket) {
        encode(*aPacket);
    }
}

void MultiplexPacketEncoder::encode(VideoPacket& packet)
{
    auto planar = dynamic_cast<av::PlanarVideoPacket*>(&packet);
    if (planar) {
        encodeVideo(planar->buffer, planar->linesize, planar->width,
            planar->height, planar->time);
    }
    else {
        encodeVideo((uint8_t*)packet.data(), int(packet.size()), packet.width,
            packet.height, packet.time);
    }
}


void MultiplexPacketEncoder::encode(AudioPacket& packet)
{
    auto planar = dynamic_cast<av::PlanarAudioPacket*>(&packet);
    if (planar) {
        encodeAudio(planar->buffer, int(planar->numSamples), planar->time);
    }
    else {
        encodeAudio((uint8_t*)packet.data(), int(packet.numSamples), packet.time);
    }
}


bool MultiplexPacketEncoder::accepts(IPacket* packet)
{
    return dynamic_cast<av::MediaPacket*>(packet) != 0;
}


void MultiplexPacketEncoder::onStreamStateChange(const PacketStreamState& state)
{
    LTrace("On stream state change: ", state)

    std::lock_guard<std::mutex> guard(_mutex);

    switch (state.id()) {
        case PacketStreamState::Active:
            if (!isActive()) {
                LTrace("Initializing")
                // if (MultiplexEncoder::options().oformat.video.enabled &&
                //    MultiplexEncoder::options().oformat.audio.enabled)
                //    _muxLiveStreams = true;
                MultiplexEncoder::init();
            }
            break;

        // case PacketStreamState::Resetting:
        case PacketStreamState::Stopping:
            if (isActive()) {
                LTrace("Uninitializing")
                MultiplexEncoder::flush();
                MultiplexEncoder::uninit();
            }
            break;
            // case PacketStreamState::Stopped:
            // case PacketStreamState::Error:
            // case PacketStreamState::None:
            // case PacketStreamState::Closed:
    }

    LTrace("Stream state change: OK: ", state)
}


} // namespace av
} // namespace scy


#endif


/// @\}
