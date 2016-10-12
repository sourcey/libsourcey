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


#include "scy/av/audioresampler.h"

#ifdef HAVE_FFMPEG

#include "scy/logger.h"
#include "scy/util.h"


using std::endl;


namespace scy {
namespace av {


//
// Audio Resampler
//


AudioResampler::AudioResampler(const AudioCodec& iparams, const AudioCodec& oparams) :
    ctx(nullptr),
    iparams(iparams),
    oparams(oparams),
    outSamples(nullptr),
    outNbSamples(0),
    outMaxNbSamples(0),
    outSamplesBytes(0)
{
}


AudioResampler::~AudioResampler()
{
    close();
}


void AudioResampler::create()
{
    if (ctx)
        throw std::runtime_error("Resample context already initialized");

    std::int64_t inChLayout  = av_get_default_channel_layout(iparams.channels);
    std::int64_t outChLayout = av_get_default_channel_layout(oparams.channels);

    char inChBuf[128], outChBuf[128];
    av_get_channel_layout_string(inChBuf,  sizeof(inChBuf),  -1, inChLayout);
    av_get_channel_layout_string(outChBuf, sizeof(outChBuf), -1, outChLayout);

    enum AVSampleFormat inSampleFmt  = av_get_sample_fmt(iparams.sampleFmt.c_str());
    enum AVSampleFormat outSampleFmt = av_get_sample_fmt(oparams.sampleFmt.c_str());

    TraceS(this) << "Create audio resampler:\n"
        << "\n\tIn Nb Channels: " << iparams.channels
        << "\n\tIn Channel Layout: " << inChBuf
        << "\n\tIn Sample Rate: " << iparams.sampleRate
        << "\n\tIn Sample Fmt: " << iparams.sampleFmt
        << "\n\tOut Nb Channels: " << oparams.channels
        << "\n\tOut Channel Layout: " << outChBuf
        << "\n\tOut Sample Rate: " << oparams.sampleRate
        << "\n\tOut Sample Fmt: " << oparams.sampleFmt
        << endl;

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
        throw std::runtime_error("Could not allocate resample context");
    }

    av_opt_set_int(ctx, "in_channel_layout", inChLayout, 0);
    av_opt_set_int(ctx, "out_channel_layout", outChLayout, 0);
    av_opt_set_int(ctx, "in_sample_rate", iparams.sampleRate, 0);
    av_opt_set_int(ctx, "out_sample_rate", oparams.sampleRate, 0);
    av_opt_set_sample_fmt(ctx, "in_sample_fmt", inSampleFmt, 0);
    av_opt_set_sample_fmt(ctx, "out_sample_fmt", outSampleFmt, 0);

    // Open the resampler context.
#ifdef HAVE_FFMPEG_SWRESAMPLE
    int error = swr_init(ctx);
#else
    int error = avresample_open(ctx);
#endif
    if (error < 0) {
        close();
        throw std::runtime_error("Could not initialize resample context: " + averror(error));
    }

    TraceS(this) << "Create: OK" << endl;
}


void AudioResampler::close()
{
    TraceS(this) << "Closing" << endl;

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
        free(outSamples);
        outSamples = nullptr;
    }

    outNbSamples = 0;
    outMaxNbSamples = 0;
    outSamplesBytes = 0;
}


int AudioResampler::resample(const std::uint8_t* inSamples, int inNbSamples)
{
    if (!ctx)
        throw std::runtime_error("Conversion context must be initialized.");

    int error, outLinesize, maxNbSamples;

    // Compute output number of samples
    // https://github.com/FFmpeg/FFmpeg/blob/master/doc/examples/resampling_audio.c
    maxNbSamples = av_rescale_rnd(
#ifdef HAVE_FFMPEG_SWRESAMPL
        swr_get_delay(ctx, (std::int64_t)iparams.sampleRate) +
#else
        avresample_get_delay(ctx) +
#endif
        (std::int64_t)inNbSamples,
        (std::int64_t)oparams.sampleRate, (std::int64_t)iparams.sampleRate, AV_ROUND_UP);

    // Resize the output buffer if required
    if (maxNbSamples > outMaxNbSamples) {
        error = av_samples_alloc_array_and_samples(&outSamples, &outLinesize,
                                                   oparams.channels,
                                                   maxNbSamples,
                                                   av_get_sample_fmt(oparams.sampleFmt.c_str()), 0);
        if (error < 0) {
            throw std::runtime_error("Could not allocate buffer for converted output samples: " + averror(error));
        }

        outMaxNbSamples = maxNbSamples;
    }

    assert(maxNbSamples);
    assert(outMaxNbSamples);

    // Convert the samples using the resampler.
#ifdef HAVE_FFMPEG_SWRESAMPLE
    outNbSamples = swr_convert(ctx, outSamples, outMaxNbSamples, (const std::uint8_t**)&inSamples, inNbSamples);
#else
    outNbSamples = avresample_convert(ctx, outSamples, 0, outMaxNbSamples, (std::uint8_t**)&inSamples, 0, inNbSamples);
#endif
    if (outNbSamples < 0) {
        close();
        throw std::runtime_error("Could not convert input samples: " + averror(outNbSamples));
    }

    // Set the output buffer size in bytes.
    // This may be useful for implementations that need to know the size of
    // converted output samples in bytes, such as for writing to files.
    outSamplesBytes = av_samples_get_buffer_size(&outLinesize,
                                                 oparams.channels,
                                                 outNbSamples,
                                                 av_get_sample_fmt(oparams.sampleFmt.c_str()), 1);
    assert(outSamplesBytes > 0);

    TraceS(this) << "Resampled audio frame:"
        << "\n\tIn Nb Samples: " << inNbSamples
        << "\n\tIn Channels: " << iparams.channels
        << "\n\tIn Sample Rate: " << iparams.sampleRate
        << "\n\tIn Sample Fmt: " << iparams.sampleFmt
        << "\n\tOut Nb Samples: " << outNbSamples
        // << "\n\tOut Max Nb Samples: " << outMaxNbSamples
        // << "\n\tOut Line Size: " << outLinesize
        // << "\n\tOut Buffer Size: " << outSamplesBytes
        << "\n\tOut Channels: " << oparams.channels
        << "\n\tOut Sample Rate: " << oparams.sampleRate
        << "\n\tOut Sample Fmt: " << oparams.sampleFmt
        << endl;

    return outNbSamples;
}


} } // namespace scy::av


#endif
