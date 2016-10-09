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


#ifndef SCY_MEDIA_AudioResampler_H
#define SCY_MEDIA_AudioResampler_H


#include "scy/base.h"

#ifdef HAVE_FFMPEG

// #include "scy/media/types.h"
#include "scy/media/ffmpeg.h"
#include "scy/media/format.h"
// #include "scy/media/fpscounter.h"
//#include "scy/media/iencoder.h"

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
struct AudioResampler
{
    AudioResampler();
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

    struct SwrContext* ctx;

    AudioCodec iparams;        // input audio parameters
    AudioCodec oparams;        // output audio parameters

    std::uint8_t** outSamples; // the output samples buffer
    int outNbSamples;          // the number of samples currently in the output buffer
    int outMaxNbSamples;       // the maximum number of samples that can be stored in the output buffer
    int outSamplesBytes;       // the number of bytes currently in the buffer
};


} } // namespace scy::av


#endif
#endif    // SCY_MEDIA_AudioResampler_H
