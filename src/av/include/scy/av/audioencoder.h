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

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/opt.h>
#include <libavutil/audio_fifo.h>
}


namespace scy {
namespace av {


struct AudioResampler;


struct AudioEncoderContext: public AudioContext
{
    AudioEncoderContext(AVFormatContext* format = nullptr);
    virtual ~AudioEncoderContext();

    virtual void create();
    //virtual void open();
    virtual void close();

    virtual bool encode(AVFrame* iframe, AVPacket& opacket);
      // Encode a single AVFrame from the decoder.

    virtual bool encode(const std::uint8_t* samples, const int numSamples, const std::int64_t pts, AVPacket& opacket);
      // Encode a single frame of interleaved or planar audio.
      //
      // @param samples    The input samples to encode.
      // @param numSamples The number of input samples per channel.
      // @param pts        The input samples presentation timestamp.
      // @param opacket    The output packet data will be encoded to.

    virtual bool flush(AVPacket& opacket);
      // Flush remaining packets to be encoded.
      // This method should be called repeatedly before stream close until
      // it returns false.

    AVFormatContext* format;
    AudioResampler*  resampler;
    AVAudioFifo*     fifo;
    AudioCodec       iparams;
    AudioCodec       oparams;
    //int              inputFrameSize;
    int              outputFrameSize;
};


} } // namespace scy::av


#endif
#endif    // SCY_AV_AudioEncoder_H
