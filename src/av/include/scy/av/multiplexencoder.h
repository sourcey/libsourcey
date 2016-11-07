///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup av
/// @{


#ifndef SCY_AV_MultiplexEncoder_H
#define SCY_AV_MultiplexEncoder_H


#include "scy/base.h"

#ifdef HAVE_FFMPEG

#include "scy/packetstream.h"
#include "scy/av/types.h"
#include "scy/av/ffmpeg.h"
#include "scy/av/iencoder.h"
#include "scy/av/videoencoder.h"
#include "scy/av/audioencoder.h"
#include "scy/mutex.h"
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
class MultiplexEncoder: public IEncoder
{
public:
    MultiplexEncoder(const EncoderOptions& options = EncoderOptions());
    virtual ~MultiplexEncoder();

    virtual void initialize();
    virtual void uninitialize();
    virtual void cleanup();

    virtual void createVideo();
    virtual void freeVideo();
    virtual bool encodeVideo(AVFrame* frame);    // Encode a single video frame
    /// If the time is specified it should be the nanosecond offset since
    /// the start of the input stream. The value will be converted to the
    /// stream time base internally.
    /// If no time is specified a realtime time value will be assigned to
    /// the frame.
    virtual bool encodeVideo(std::uint8_t* buffer, int bufferSize, int width, int height, std::int64_t time = AV_NOPTS_VALUE);


    virtual void createAudio();
    virtual void freeAudio();    /// virtual bool encodeAudio(AVFrame* frame);    // Encode a single audio frame
    virtual bool encodeAudio(std::uint8_t* buffer, int numSamples, std::int64_t time = AV_NOPTS_VALUE);

    /// Flush and beffered or queued packets.
    virtual void flush();


    EncoderOptions& options();
    VideoEncoder* video();
    AudioEncoder* audio();

    PacketSignal emitter;

protected:
    bool writeOutputPacket(AVPacket& packet);
    void updateStreamPts(AVStream* stream, std::int64_t* pts);

    void onVideoEncoded(av::VideoPacket& packet);
    void onAudioEncoded(av::AudioPacket& packet);

    //static Mutex _mutex; // Protects avcodec_open/close()

    EncoderOptions _options;
    AVFormatContext* _formatCtx;
    VideoEncoder* _video;
    AudioEncoder* _audio;
    AVIOContext* _ioCtx;
    std::uint8_t* _ioBuffer;
    int _ioBufferSize;
    std::uint64_t _pts;
};


// bool writeOutputPacket(AVFormatContext *formatCtx, AVPacket& packet);


} } // namespace scy::av


#endif
#endif // SCY_AV_MultiplexEncoder_H

/// @\}
