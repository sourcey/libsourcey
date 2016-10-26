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


#ifndef SCY_AV_AudioEncoder_H
#define SCY_AV_AudioEncoder_H


#include "scy/base.h"

#ifdef HAVE_FFMPEG

#include "scy/av/types.h"
#include "scy/av/ffmpeg.h"
#include "scy/av/format.h"
#include "scy/av/audiocontext.h"
#include "scy/av/audiobuffer.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
// #include <libavutil/opt.h>
#include <libavutil/audio_fifo.h>
}


namespace scy {
namespace av {


struct AudioEncoder: public AudioContext
{
    AudioEncoder(AVFormatContext* format = nullptr);
    virtual ~AudioEncoder();

    virtual void create();
    // virtual void open();
    virtual void close();

    virtual bool encode(/*const */std::uint8_t* samples, const int numSamples, const std::int64_t pts);
        // Encode an arbitrary number of interleaved audio samples.
        //
        // @param samples    The input samples to encode.
        // @param numSamples The number of input samples per channel.
        // @param pts        The input samples presentation timestamp.
        // @param opacket    The output packet data will be encoded to.

    virtual bool encode(AVFrame* iframe);
        // Encode a single AVFrame from the decoder.

    virtual void flush();
        // Flush remaining packets to be encoded.
        // This method should be called once before stream closure.

    av::AudioBuffer fifo;
    AVFormatContext* format;
};


} } // namespace scy::av


#endif
#endif // SCY_AV_AudioEncoder_H
