///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup av
/// @{


#ifndef SCY_AV_AudioContext_H
#define SCY_AV_AudioContext_H


#include "scy/base.h"
#include "scy/packetsignal.h"

#ifdef HAVE_FFMPEG

#include "scy/av/ffmpeg.h"
#include "scy/av/format.h"
#include "scy/av/fpscounter.h"
#include "scy/av/types.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}


namespace scy {
namespace av {


struct AudioResampler;


struct AudioContext
{
    AudioContext();
    virtual ~AudioContext();

    /// Initialize the `AVCodecContext` with default values
    virtual void create()= 0;

    /// Open the AVCodecContext
    virtual void open();

    /// Close the AVCodecContext
    virtual void close();


    virtual bool recreateResampler();

    PacketSignal emitter;

    AudioCodec iparams;  ///< input parameters
    AudioCodec oparams;  ///< output parameters
    AVStream* stream;    ///< encoder or decoder stream
    AVCodecContext* ctx; ///< encoder or decoder context
    AVCodec* codec;      ///< encoder or decoder codec
    AVFrame* frame;      ///< last encoded or decoded frame
    AudioResampler* resampler;
    int outputFrameSize; ///< encoder or decoder output frame size
    std::int64_t time;   ///< stream time in nanoseconds
    std::int64_t pts;    ///< encoder current pts
    /// FPSCounter fps;         ///< encoder or decoder fps rate
    std::string error; ///< error message
};


void initAudioCodecFromContext(const AVCodecContext* ctx, AudioCodec& params);
AVSampleFormat selectSampleFormat(AVCodec* codec, av::AudioCodec& params);
bool isSampleFormatSupported(AVCodec* codec, enum AVSampleFormat sampleFormat);
void initDecodedAudioPacket(const AVStream* stream, const AVCodecContext* ctx,
                            const AVFrame* frame, AVPacket* opacket);


} // namespace av
} // namespace scy


#endif
#endif // SCY_AV_AudioContext_H


/// @\}
