///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup av
/// @{


#include "scy/av/audioresampler.h"

#ifdef HAVE_FFMPEG

#include "scy/logger.h"
#include "scy/util.h"


using std::endl;


namespace scy {
namespace av {


AudioResampler::AudioResampler(const AudioCodec& iparams,
                               const AudioCodec& oparams)
    : ctx(nullptr)
    , iparams(iparams)
    , oparams(oparams)
    , outSamples(nullptr)
    , outNumSamples(0)
    , outBufferSize(0)
    , maxNumSamples(0)
    , inSampleFmt(AV_SAMPLE_FMT_NONE)
    , outSampleFmt(AV_SAMPLE_FMT_NONE)
{
}


AudioResampler::~AudioResampler()
{
    close();
}


void AudioResampler::open()
{
    if (ctx)
        throw std::runtime_error("Resample context already initialized");

    int64_t inChLayout = av_get_default_channel_layout(iparams.channels);
    int64_t outChLayout = av_get_default_channel_layout(oparams.channels);

    char inChBuf[128], outChBuf[128];
    av_get_channel_layout_string(inChBuf, sizeof(inChBuf), -1, inChLayout);
    av_get_channel_layout_string(outChBuf, sizeof(outChBuf), -1, outChLayout);

    inSampleFmt = av_get_sample_fmt(iparams.sampleFmt.c_str());
    outSampleFmt = av_get_sample_fmt(oparams.sampleFmt.c_str());

    STrace << "Create audio resampler:\n"
                 << "\n\tIn Nb Channels: " << iparams.channels
                 << "\n\tIn Channel Layout: " << inChBuf
                 << "\n\tIn Sample Rate: " << iparams.sampleRate
                 << "\n\tIn Sample Fmt: " << iparams.sampleFmt
                 << "\n\tOut Nb Channels: " << oparams.channels
                 << "\n\tOut Channel Layout: " << outChBuf
                 << "\n\tOut Sample Rate: " << oparams.sampleRate
                 << "\n\tOut Lample Fmt: " << oparams.sampleFmt << std::endl;

    assert(iparams.channels);
    assert(oparams.channels);
    assert(iparams.sampleRate);
    assert(oparams.sampleRate);
    assert(!iparams.sampleFmt.empty());
    assert(!oparams.sampleFmt.empty());
    assert(inChLayout);
    assert(outChLayout);
    assert(inSampleFmt != AV_SAMPLE_FMT_NONE);
    assert(outSampleFmt != AV_SAMPLE_FMT_NONE);

#ifdef HAVE_FFMPEG_SWRESAMPLE
    ctx = swr_alloc();
#else
    ctx = avresample_alloc_context();
#endif
    if (!ctx) {
        throw std::runtime_error("Cannot allocate resample context");
    }

    av_opt_set_int(ctx, "in_channel_layout", inChLayout, 0);
    av_opt_set_int(ctx, "out_channel_layout", outChLayout, 0);
    av_opt_set_int(ctx, "in_sample_rate", iparams.sampleRate, 0);
    av_opt_set_int(ctx, "out_sample_rate", oparams.sampleRate, 0);
    av_opt_set_sample_fmt(ctx, "in_sample_fmt", inSampleFmt, 0);
    av_opt_set_sample_fmt(ctx, "out_sample_fmt", outSampleFmt, 0);

// Open the resampler context.
#ifdef HAVE_FFMPEG_SWRESAMPLE
    int ret = swr_init(ctx);
#else
    int ret = avresample_open(ctx);
#endif
    if (ret < 0) {
        close();
        throw std::runtime_error("Cannot initialize resample context: " + averror(ret));
    }

    LTrace("Create: OK")
}


void AudioResampler::close()
{
    LTrace("Closing")

    if (ctx) {
#ifdef HAVE_FFMPEG_SWRESAMPLE
        swr_free(&ctx);
#else
        avresample_free(&ctx);
#endif
        ctx = nullptr;
    }

    if (outSamples) {
        av_freep(&(outSamples)[0]);
        //av_free(outSamples);
        outSamples = nullptr;
    }

    outNumSamples = 0;
    maxNumSamples = 0;
    outBufferSize = 0;
}


int AudioResampler::resample(uint8_t** inSamples, int inNumSamples)
{
    if (!ctx)
        throw std::runtime_error("Conversion context must be initialized.");

    int ret, requiredNumSamples;

    // Compute the output number of samples
    // https://github.com/FFmpeg/FFmpeg/blob/master/doc/examples/resampling_audio.c
    requiredNumSamples = av_rescale_rnd(
#ifdef HAVE_FFMPEG_SWRESAMPLE
        swr_get_delay(ctx, (int64_t)iparams.sampleRate) +
#else
        avresample_get_delay(ctx) +
#endif
            (int64_t)inNumSamples, (int64_t)oparams.sampleRate,
            (int64_t)iparams.sampleRate, AV_ROUND_UP);

    // Resize the output buffer if required
    if (requiredNumSamples > maxNumSamples) {
        ret = av_samples_alloc_array_and_samples(
            &outSamples, nullptr, oparams.channels, requiredNumSamples,
            outSampleFmt, 0);
        if (ret < 0) {
            throw std::runtime_error("Cannot allocate buffer for converted output samples: " + averror(ret));
        }

        LTrace("Resizing resampler buffer: ", outBufferSize)
        maxNumSamples = requiredNumSamples;
    }

    assert(requiredNumSamples);
    assert(maxNumSamples);

    // Convert the samples using the resampler.
#ifdef HAVE_FFMPEG_SWRESAMPLE
    ret = swr_convert(ctx, outSamples, maxNumSamples,
                      (const uint8_t**)/*&*/inSamples, inNumSamples);
#else
    ret = avresample_convert(ctx, outSamples, 0, maxNumSamples,
                             (uint8_t**)/*&*/inSamples, 0, inNumSamples);
#endif
    if (ret < 0) {
        close();
        throw std::runtime_error("Cannot convert input samples: " + averror(ret));
    }

    // Set the actual number of output samples
    outNumSamples = ret;

    // Set the output buffer size in bytes.
    // This may be useful for implementations that need to know the size of
    // converted output samples in bytes, such as for writing to files.
    outBufferSize = av_samples_get_buffer_size(nullptr, oparams.channels,
                                               outNumSamples, outSampleFmt, 0);

    // STrace << "Resampled audio frame:"
    //        << "\n\tIn Nb Samples: " << inNumSamples
    //        << "\n\tIn Channels: " << iparams.channels
    //        << "\n\tIn Sample Rate: " << iparams.sampleRate
    //        << "\n\tIn Sample Fmt: " << iparams.sampleFmt
    //        << "\n\tOut Nb Samples: " << outNumSamples
    //        << "\n\tOut Max Nb Samples: " << maxNumSamples
    //        << "\n\tOut Buffer Size: " << outBufferSize
    //        << "\n\tOut Channels: " << oparams.channels
    //        << "\n\tOut Sample Rate: " << oparams.sampleRate
    //        << "\n\tOut Lample Fmt: " << oparams.sampleFmt << std::endl;

    // assert(outNumSamples > 0);
    // assert(outBufferSize > 0);

    return ret;
}


#if 0
int AudioResampler::resample(const uint8_t* inSamples, int inNumSamples)
{
    if (!ctx)
        throw std::runtime_error("Conversion context must be initialized.");

    int error, outLinesize, requiredNumSamples;

    // Compute output number of samples
    // https://github.com/FFmpeg/FFmpeg/blob/master/doc/examples/resampling_audio.c
    requiredNumSamples = av_rescale_rnd(
#ifdef HAVE_FFMPEG_SWRESAMPLE
        swr_get_delay(ctx, (int64_t)iparams.sampleRate) +
#else
        avresample_get_delay(ctx) +
#endif
        (int64_t)inNumSamples,
        (int64_t)oparams.sampleRate, (int64_t)iparams.sampleRate, AV_ROUND_UP);

    requiredNumSamples = 440;

    // Resize the output buffer if required
    if (requiredNumSamples > maxNumSamples) {
        error = av_samples_alloc_array_and_samples(&outSamples, &outLinesize,
                                                   oparams.channels,
                                                   requiredNumSamples,
                                                   av_get_sample_fmt(oparams.sampleFmt.c_str()), 0);
        if (error < 0) {
            throw std::runtime_error("Cannot allocate buffer for converted output samples: " + averror(error));
        }

        maxNumSamples = requiredNumSamples;
    }

    assert(requiredNumSamples);
    assert(maxNumSamples);

    // Convert the samples using the resampler.
#ifdef HAVE_FFMPEG_SWRESAMPLE
    outNumSamples = swr_convert(ctx, outSamples, maxNumSamples, (const uint8_t**)&inSamples, inNumSamples);
#else
    outNumSamples = avresample_convert(ctx, outSamples, 0, maxNumSamples, (uint8_t**)&inSamples, 0, inNumSamples);
#endif
    if (outNumSamples < 0) {
        close();
        throw std::runtime_error("Cannot convert input samples: " + averror(outNumSamples));
    }

    // Set the output buffer size in bytes.
    // This may be useful for implementations that need to know the size of
    // converted output samples in bytes, such as for writing to files.
    outBufferSize = av_samples_get_buffer_size(&outLinesize,
                                                 oparams.channels,
                                                 outNumSamples,
                                                 av_get_sample_fmt(oparams.sampleFmt.c_str()), 1);
    assert(outBufferSize > 0);

    STrace << "Resampled audio frame:"
        << "\n\tIn Nb Samples: " << inNumSamples
        << "\n\tIn Channels: " << iparams.channels
        << "\n\tIn Sample Rate: " << iparams.sampleRate
        << "\n\tIn Sample Fmt: " << iparams.sampleFmt
        << "\n\tOut Nb Samples: " << outNumSamples
        << "\n\tOut Max Nb Samples: " << maxNumSamples
        << "\n\tOut Line Size: " << outLinesize
        << "\n\tOut Buffer Size: " << outBufferSize
        << "\n\tOut Channels: " << oparams.channels
        << "\n\tOut Sample Rate: " << oparams.sampleRate
        << "\n\tOut Sample Fmt: " << oparams.sampleFmt
        << endl;

    return outNumSamples;
}
#endif


} // namespace av
} // namespace scy


#endif


/// @\}
