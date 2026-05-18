///
//
// icey
// Copyright (c) 2005, icey <https://0state.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//

#pragma once

#include "icy/av/ffmpeg.h"
#include "icy/av/packet.h"
#include "icy/webrtc/mediabridge.h"

#include <filesystem>
#include <memory>
#include <mutex>
#include <optional>
#include <string>

struct AVStream;

namespace icy {
namespace av {
struct AudioDecoder;
class MultiplexPacketEncoder;
struct VideoDecoder;
}

class WhipFileSink
{
public:
    WhipFileSink(std::filesystem::path outputDir, std::string sessionId);
    ~WhipFileSink();

    WhipFileSink(const WhipFileSink&) = delete;
    WhipFileSink& operator=(const WhipFileSink&) = delete;

    void attach(wrtc::MediaBridge& media);
    void close();

    [[nodiscard]] const std::filesystem::path& outputFile() const;
    [[nodiscard]] bool wroteFile() const;

private:
    void ensureVideoDecoder();
    void ensureAudioDecoder();
    void ensureMux(const av::PlanarVideoPacket& packet);

    void onEncodedVideo(av::VideoPacket& packet);
    void onDecodedVideo(av::PlanarVideoPacket& packet);
    void onEncodedAudio(av::AudioPacket& packet);
    void onDecodedAudio(av::PlanarAudioPacket& packet);
    int64_t normalizeTime(int64_t time, std::optional<int64_t>& baseTime);

    std::filesystem::path _outputDir;
    std::filesystem::path _outputFile;
    wrtc::MediaBridge* _media = nullptr;

    av::AVFormatContextHolder _videoDecodeFormat;
    av::AVFormatContextHolder _audioDecodeFormat;
    AVStream* _videoDecodeStream = nullptr;
    AVStream* _audioDecodeStream = nullptr;
    std::unique_ptr<av::VideoDecoder> _videoDecoder;
    std::unique_ptr<av::AudioDecoder> _audioDecoder;
    std::unique_ptr<av::MultiplexPacketEncoder> _mux;

    bool _wroteFile = false;
    bool _waitingForVideoKeyframe = true;
    bool _loggedWaitingForVideoKeyframe = false;
    std::optional<int64_t> _videoBaseTime;
    std::optional<int64_t> _audioBaseTime;
    mutable std::recursive_mutex _mutex;
};

} // namespace icy
