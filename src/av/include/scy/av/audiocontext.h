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


#ifndef SCY_AV_AudioContext_H
#define SCY_AV_AudioContext_H


#include "scy/base.h"

#ifdef HAVE_FFMPEG

#include "scy/av/types.h"
#include "scy/av/ffmpeg.h"
#include "scy/av/format.h"
#include "scy/av/fpscounter.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/opt.h>
#include <libavutil/audio_fifo.h>
}


namespace scy {
namespace av {


struct AudioResampler;


struct AudioContext
{
    AudioContext();
    virtual ~AudioContext();

    virtual void create();
        // Initialize the AVCodecContext with default values

    virtual void open();
        // Open the AVCodecContext

    virtual void close();
        // Close the AVCodecContext

    virtual double ptsSeconds();
        // Current pts in decimal seconds

    AVStream* stream;       // encoder or decoder stream
    AVCodecContext* ctx;    // encoder or decoder context
    AVCodec* codec;         // encoder or decoder codec
    AVFrame* frame;         // last encoded or decoded frame
    std::int64_t pts;       // encoder current pts
    FPSCounter fps;         // encoder or decoder fps rate
    std::string error;      // error message
};


// ---------------------------------------------------------------------
//
struct AudioEncoderContext: public AudioContext
{
    AudioEncoderContext(AVFormatContext* format = nullptr);
    virtual ~AudioEncoderContext();

    virtual void create();
    //virtual void open();
    virtual void close();

    virtual bool encode(AVFrame* iframe, AVPacket& opacket);
      // Encode a single AVFrame from the decoder.

    virtual bool encode(const std::uint8_t* samples, const int frameSize, const std::int64_t pts, AVPacket& opacket);
      // Encode a single audio frame.
      // @param samples   The input samples to encode.
      // @param frameSize The input frame size as specified by the input audio codec.
      // @param pts       The input samples presentation timestamp.
      // @param opacket   The output packet data will be encoded to.

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


// ---------------------------------------------------------------------
//
struct AudioDecoderContext: public AudioContext
{
    AudioDecoderContext();
    virtual ~AudioDecoderContext();

    virtual void create(AVFormatContext* ic, int streamID);
    //virtual void open();
    virtual void close();

    virtual bool decode(std::uint8_t* data, int size, AVPacket& opacket);
    virtual bool decode(AVPacket& ipacket, AVPacket& opacket);
        // Decodes a the given input packet.
        // Returns true an output packet was returned,
        // false otherwise.

    virtual bool flush(AVPacket& opacket);
        // Flushes buffered frames.
        // This method should be called after decoding
        // until false is returned.

    double duration;
    int width;    // Number of bits used to store a sample
    bool fp;      // Floating-point sample representation
};


// void isSampleFormatSupported(AVCodec* codec, const std::string& sampleFormat);
bool isSampleFormatSupported(AVCodec* codec, enum AVSampleFormat sampleFormat);
// AVSampleFormat getSupportedSampleFormat(AVCodec* codec, const std::string& sampleFormat);
AVSampleFormat selectSampleFormat(AVCodec* codec, av::AudioCodec& params);
void initDecodedAudioPacket(const AVStream* stream, const AVCodecContext* ctx, const AVFrame* frame, AVPacket* opacket);


} } // namespace scy::av


#endif
#endif    // SCY_AV_AudioContext_H
