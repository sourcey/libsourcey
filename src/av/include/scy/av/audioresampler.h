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

// #include "scy/av/types.h"
#include "scy/av/ffmpeg.h"
#include "scy/av/format.h"
// #include "scy/av/fpscounter.h"
//#include "scy/av/iencoder.h"

//#include "scy/mutex.h"

extern "C" {
// #include <libavresample/avresample.h>
#include <libavcodec/avcodec.h>
// #include <libavformat/avformat.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}


namespace scy {
namespace av {


// ---------------------------------------------------------------------
//
// #define SWR_CH_MAX   64
//
// typedef uint8_t *[SWR_CH_MAX] SampleData;


struct AudioResampler
{
    AudioResampler(const AudioCodec& iparams = AudioCodec(), const AudioCodec& oparams = AudioCodec());
    ~AudioResampler();

    void create();
    void close();

    int resample(const std::uint8_t* inSamples, int inNbSamples);
        // Convert the input samples to the outpot format.
        //
        // Return the number of converted samples or zero if samples
        // were internally buffered. An exception will be thrown on error.
        //
        // Converted samples are accessable via the `outSamples` member.

    AudioCodec iparams;        // input audio parameters
    AudioCodec oparams;        // output audio parameters
    struct SwrContext* ctx;    // the conversion context
    std::uint8_t** outSamples; // the output samples buffer
    int outNbSamples;          // the number of samples currently in the output buffer
    int outMaxNbSamples;       // the maximum number of samples that can be stored in the output buffer
    int outSamplesBytes;       // the number of bytes currently in the buffer
};


} } // namespace scy::av


#endif
#endif    // SCY_AV_AudioResampler_H
