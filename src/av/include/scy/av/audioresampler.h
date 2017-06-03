///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup av
/// @{


#ifndef SCY_AV_AudioResampler_H
#define SCY_AV_AudioResampler_H


#include "scy/base.h"

#ifdef HAVE_FFMPEG

#include "scy/av/ffmpeg.h"
#include "scy/av/format.h"
#include "scy/av/packet.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
#ifdef HAVE_FFMPEG_SWRESAMPLE
#include <libswresample/swresample.h>
#else
#include <libavresample/avresample.h>
#endif
}


namespace scy {
namespace av {


// ---------------------------------------------------------------------
//
struct AudioResampler
{
    AudioResampler(const AudioCodec& iparams = AudioCodec(),
                   const AudioCodec& oparams = AudioCodec());
    ~AudioResampler();

    void open();
    void close();

    /// Convert the input samples to the output format.
    /// NOTE: Input buffers must be contiguous, therefore only interleaved
    /// input formats are accepted at this point.
    ///
    /// Return the number of converted samples or zero if samples
    /// were internally buffered. An exception will be thrown on error.
    ///
    /// Converted samples are accessable via the `outSamples` class member.
    int resample(uint8_t** inSamples, int inNumSamples);

#ifdef HAVE_FFMPEG_SWRESAMPLE
    SwrContext* ctx; ///< the conversion context
#else
    AVAudioResampleContext* ctx;
#endif

    AudioCodec iparams;     ///< input audio parameters
    AudioCodec oparams;     ///< output audio parameters
    uint8_t** outSamples;   ///< the output samples buffer
    int outNumSamples;      ///< the number of samples currently in the output buffer
    int outBufferSize;      ///< the number of bytes currently in the buffer
    int maxNumSamples;      ///< the maximum number of samples that can be stored in
    enum AVSampleFormat inSampleFmt;  ///< input sample format
    enum AVSampleFormat outSampleFmt; ///< output sample format
};


} // namespace av
} // namespace scy


#endif
#endif // SCY_AV_AudioResampler_H


/// @\}
