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


#ifndef SCY_AV_AudioResampler_H
#define SCY_AV_AudioResampler_H


#include "scy/base.h"

#ifdef HAVE_FFMPEG

#include "scy/av/types.h"
#include "scy/av/ffmpeg.h"
#include "scy/av/format.h"

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
    AudioResampler(const AudioCodec& iparams = AudioCodec(), const AudioCodec& oparams = AudioCodec());
    ~AudioResampler();

    void open();
    void close();

    int resample(std::uint8_t** inSamples, int inNumSamples);
        // Convert the input samples to the output format.
        // NOTE: Input buffers must be contiguous, therefore only interleaved
        // input formats are accepted at this point.
        //
        // Return the number of converted samples or zero if samples
        // were internally buffered. An exception will be thrown on error.
        //
        // Converted samples are accessable via the `outSamples` class member.

#ifdef HAVE_FFMPEG_SWRESAMPLE
    SwrContext* ctx;           // the conversion context
#else
    AVAudioResampleContext* ctx;
#endif

    AudioCodec iparams;        // input audio parameters
    AudioCodec oparams;        // output audio parameters
    std::uint8_t** outSamples; // the output samples buffer
    int outNumSamples;         // the number of samples currently in the output buffer
    int outBufferSize;         // the number of bytes currently in the buffer
    int maxNumSamples;         // the maximum number of samples that can be stored in the output buffer
    enum AVSampleFormat inSampleFmt;  // input sample format
    enum AVSampleFormat outSampleFmt; // output sample format
};


} } // namespace scy::av


#endif
#endif // SCY_AV_AudioResampler_H
