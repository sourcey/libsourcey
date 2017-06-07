///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup av
/// @{


#ifndef SCY_AV_MultiplexEncoder_H
#define SCY_AV_MultiplexEncoder_H


#include "scy/base.h"

#ifdef HAVE_FFMPEG

#include "scy/av/audioencoder.h"
#include "scy/av/ffmpeg.h"
#include "scy/av/iencoder.h"
#include "scy/av/packet.h"
#include "scy/av/videoencoder.h"
#include "scy/packetstream.h"
#include <mutex>
#include <fstream>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/fifo.h>
#include <libswscale/swscale.h>
}


namespace scy {
namespace av {


/// This class implements a multiplex audio and video encoder.
class AV_API MultiplexEncoder : public IEncoder
{
public:
    MultiplexEncoder(const EncoderOptions& options = EncoderOptions());
    virtual ~MultiplexEncoder();

    virtual void init() override;
    virtual void uninit() override;
    virtual void cleanup();

    virtual void createVideo();
    virtual void freeVideo();

    /// Encode a single video frame.
    /// All frame values must be set, such as size, pizel format and PTS.
    virtual bool encodeVideo(AVFrame* frame);

    /// Encode a single video frame.
    /// If the frame time is specified it should be the microseconds
    /// offset since the start of the input stream. If no time is specified
    /// a realtime time value will be assigned to the frame.
    virtual bool encodeVideo(uint8_t* buffer, int bufferSize, int width, int height,
                             int64_t time = AV_NOPTS_VALUE);
    virtual bool encodeVideo(uint8_t* data[4], int linesize[4], int width, int height,
                             int64_t time = AV_NOPTS_VALUE);

    virtual void createAudio();
    virtual void freeAudio();

    /// Encode a single audio frame.
    // virtual bool encodeAudio(AVFrame* frame);
    virtual bool encodeAudio(uint8_t* buffer, int numSamples,
                             int64_t time = AV_NOPTS_VALUE);
    virtual bool encodeAudio(uint8_t* data[4], int numSamples,
                             int64_t time = AV_NOPTS_VALUE);

    /// Flush and beffered or queued packets.
    virtual void flush();

    EncoderOptions& options();
    VideoEncoder* video();
    AudioEncoder* audio();

    PacketSignal emitter;

protected:
    bool writeOutputPacket(AVPacket& packet);

    /// Convert input microseconds to the stream time base.
    bool updateStreamPts(AVStream* stream, int64_t* pts);

    void onVideoEncoded(av::VideoPacket& packet);
    void onAudioEncoded(av::AudioPacket& packet);

    EncoderOptions _options;
    AVFormatContext* _formatCtx;
    VideoEncoder* _video;
    AudioEncoder* _audio;
    AVIOContext* _ioCtx;
    uint8_t* _ioBuffer;
    int64_t _pts;
    mutable std::mutex _mutex;
};


} // namespace av
} // namespace scy


#endif
#endif // SCY_AV_MultiplexEncoder_H


/// @\}
