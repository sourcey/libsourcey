///
//
// icey
// Copyright (c) 2005, icey <https://0state.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//

#include "whip-file-sink.h"

#include "icy/av/audiodecoder.h"
#include "icy/av/multiplexpacketencoder.h"
#include "icy/av/videodecoder.h"
#include "icy/logger.h"
#include "icy/packetsignal.h"

#include <stdexcept>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/channel_layout.h>
}

namespace icy {
namespace {

constexpr AVRational kVideoTimeBase{1, 90000};
constexpr AVRational kAudioTimeBase{1, 48000};

void setStereoLayout(AVCodecParameters* params)
{
#if LIBAVUTIL_VERSION_INT >= AV_VERSION_INT(57, 28, 100)
    av_channel_layout_default(&params->ch_layout, 2);
#else
    params->channels = 2;
    params->channel_layout = av_get_default_channel_layout(2);
#endif
}

} // namespace

WhipFileSink::WhipFileSink(std::filesystem::path outputDir, std::string sessionId)
    : _outputDir(std::move(outputDir))
    , _outputFile(_outputDir / ("whip-" + sessionId + ".mp4"))
{
}


WhipFileSink::~WhipFileSink()
{
    close();
}


void WhipFileSink::attach(wrtc::MediaBridge& media)
{
    _media = &media;
    media.videoReceiver().emitter += packetSlot(this, &WhipFileSink::onEncodedVideo);
    media.audioReceiver().emitter += packetSlot(this, &WhipFileSink::onEncodedAudio);
}


void WhipFileSink::close()
{
    auto* media = _media;
    _media = nullptr;
    if (media) {
        media->videoReceiver().emitter -= this;
        media->audioReceiver().emitter -= this;
    }

    if (_videoDecoder) {
        try {
            _videoDecoder->flush();
        }
        catch (const std::exception& exc) {
            LDebug("Ignoring video decoder flush error: ", exc.what());
        }
        _videoDecoder->emitter -= this;
    }

    if (_audioDecoder) {
        try {
            _audioDecoder->flush();
        }
        catch (const std::exception& exc) {
            LDebug("Ignoring audio decoder flush error: ", exc.what());
        }
        _audioDecoder->emitter -= this;
    }

    std::lock_guard lock(_mutex);
    if (_mux) {
        try {
            _mux->flush();
        }
        catch (const std::exception& exc) {
            LDebug("Ignoring mux flush error: ", exc.what());
        }
    }
    _mux.reset();
    _videoDecoder.reset();
    _audioDecoder.reset();
    _videoDecodeFormat.reset();
    _audioDecodeFormat.reset();
    _videoDecodeStream = nullptr;
    _audioDecodeStream = nullptr;
    _videoBaseTime.reset();
    _audioBaseTime.reset();
}


const std::filesystem::path& WhipFileSink::outputFile() const
{
    return _outputFile;
}


bool WhipFileSink::wroteFile() const
{
    std::lock_guard lock(_mutex);
    return _wroteFile;
}


void WhipFileSink::ensureVideoDecoder()
{
    if (_videoDecoder)
        return;

    _videoDecodeFormat.reset(avformat_alloc_context());
    if (!_videoDecodeFormat)
        throw std::runtime_error("Cannot allocate video decoder format context");

    _videoDecodeStream = avformat_new_stream(_videoDecodeFormat.get(), nullptr);
    if (!_videoDecodeStream)
        throw std::runtime_error("Cannot allocate video decoder stream");

    _videoDecodeStream->time_base = kVideoTimeBase;
    _videoDecodeStream->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
    _videoDecodeStream->codecpar->codec_id = AV_CODEC_ID_H264;

    _videoDecoder = std::make_unique<av::VideoDecoder>(_videoDecodeStream);
    _videoDecoder->emitter += packetSlot(this, &WhipFileSink::onDecodedVideo);
    _videoDecoder->create();
    _videoDecoder->open();
}


void WhipFileSink::ensureAudioDecoder()
{
    if (_audioDecoder)
        return;

    _audioDecodeFormat.reset(avformat_alloc_context());
    if (!_audioDecodeFormat)
        throw std::runtime_error("Cannot allocate audio decoder format context");

    _audioDecodeStream = avformat_new_stream(_audioDecodeFormat.get(), nullptr);
    if (!_audioDecodeStream)
        throw std::runtime_error("Cannot allocate audio decoder stream");

    _audioDecodeStream->time_base = kAudioTimeBase;
    _audioDecodeStream->codecpar->codec_type = AVMEDIA_TYPE_AUDIO;
    _audioDecodeStream->codecpar->codec_id = AV_CODEC_ID_OPUS;
    _audioDecodeStream->codecpar->sample_rate = 48000;
    setStereoLayout(_audioDecodeStream->codecpar);

    _audioDecoder = std::make_unique<av::AudioDecoder>(_audioDecodeStream);
    _audioDecoder->emitter += packetSlot(this, &WhipFileSink::onDecodedAudio);
    _audioDecoder->create();
    _audioDecoder->open();
}


void WhipFileSink::ensureMux(const av::PlanarVideoPacket& packet)
{
    if (_mux)
        return;

    std::filesystem::create_directories(_outputDir);

    av::EncoderOptions options;
    options.ofile = _outputFile.string();
    options.iformat = av::Format("WebRTC decoded input", "raw",
        av::VideoCodec("decoded", packet.width, packet.height, 30.0,
                       0, 0, packet.pixelFmt),
        av::AudioCodec("decoded", 2, 48000, 0, "fltp"));
    options.oformat = av::Format("MP4", "mp4",
        av::VideoCodec("H264", "libx264", packet.width, packet.height, 30.0),
        av::AudioCodec("AAC", "aac", 2, 48000, 128000, "fltp"));

    _mux = std::make_unique<av::MultiplexPacketEncoder>(options);
    _mux->init();
    _wroteFile = true;
    LInfo("WHIP recording started: ", _outputFile.string());
}


void WhipFileSink::onEncodedVideo(av::VideoPacket& packet)
{
    std::lock_guard lock(_mutex);
    ensureVideoDecoder();
    if (!_videoDecoder || !_videoDecodeStream)
        return;

    auto ffpacket = av::makeOwnedPacket(packet,
                                        _videoDecodeStream->index,
                                        _videoDecodeStream->time_base);
    try {
        const bool decoded = _videoDecoder->decode(*ffpacket);
        if (decoded) {
            _waitingForVideoKeyframe = false;
            _loggedWaitingForVideoKeyframe = false;
        }
    }
    catch (const std::exception&) {
        if (_waitingForVideoKeyframe && !_loggedWaitingForVideoKeyframe) {
            LInfo("Waiting for a decodable H.264 keyframe");
            _loggedWaitingForVideoKeyframe = true;
        }
    }
}


void WhipFileSink::onDecodedVideo(av::PlanarVideoPacket& packet)
{
    std::lock_guard lock(_mutex);
    packet.time = normalizeTime(packet.time, _videoBaseTime);
    ensureMux(packet);
    if (_mux)
        _mux->encode(packet);
}


void WhipFileSink::onEncodedAudio(av::AudioPacket& packet)
{
    std::lock_guard lock(_mutex);
    ensureAudioDecoder();
    if (!_audioDecoder || !_audioDecodeStream)
        return;

    auto ffpacket = av::makeOwnedPacket(packet,
                                        _audioDecodeStream->index,
                                        _audioDecodeStream->time_base);
    try {
        (void)_audioDecoder->decode(*ffpacket);
    }
    catch (const std::exception& exc) {
        LDebug("Ignoring Opus decode error: ", exc.what());
    }
}


void WhipFileSink::onDecodedAudio(av::PlanarAudioPacket& packet)
{
    std::lock_guard lock(_mutex);
    if (_mux) {
        packet.time = normalizeTime(packet.time, _audioBaseTime);
        _mux->encode(packet);
    }
}


int64_t WhipFileSink::normalizeTime(int64_t time, std::optional<int64_t>& baseTime)
{
    if (time == AV_NOPTS_VALUE)
        return time;

    if (!baseTime)
        baseTime = time;

    const auto normalized = time - *baseTime;
    return normalized > 0 ? normalized : 0;
}

} // namespace icy
