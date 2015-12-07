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


#ifndef SCY_MEDIA_AudioContext_H
#define SCY_MEDIA_AudioContext_H


#include "scy/base.h"

#ifdef HAVE_FFMPEG

#include "scy/media/types.h"
#include "scy/media/format.h"
#include "scy/media/fpscounter.h"
//#include "scy/media/iencoder.h"

//#include "scy/mutex.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/audio_fifo.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h> //#include "avresample.h"
}


namespace scy {
namespace av {


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
    Int64 pts;              // encoder current pts
    FPSCounter fps;         // encoder or decoder fps rate
    std::string error;      // error message
};


// ---------------------------------------------------------------------
//
struct AudioResampler;
struct AudioEncoderContext: public AudioContext
{
    AudioEncoderContext(AVFormatContext* format);
    virtual ~AudioEncoderContext();

    virtual void create();
    //virtual void open();
    virtual void close();

    virtual bool encode(AVFrame* iframe, AVPacket& opacket);
      // Encode a single AVFrame from the decoder.

    virtual bool encode(const UInt8* samples, const int frameSize, const Int64 pts, AVPacket& opacket);
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
    AVAudioFifo       *fifo;
    AudioCodec        iparams;
    AudioCodec        oparams;
    //int                inputFrameSize;
    int                outputFrameSize;
};


// ---------------------------------------------------------------------
//
struct AudioDecoderContext: public AudioContext
{
    AudioDecoderContext();
    virtual ~AudioDecoderContext();

    virtual void create(AVFormatContext *ic, int streamID);
    //virtual void open();
    virtual void close();

    virtual bool decode(UInt8* data, int size, AVPacket& opacket);
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
    bool fp;    // Floating-point sample representation
};


// ---------------------------------------------------------------------
//
struct AudioResampler
{
    AudioResampler();
    virtual ~AudioResampler();

    virtual void create(const AudioCodec& iparams, const AudioCodec& oparams);
    virtual void close();

    virtual bool resample(const UInt8* inSamples, int inNbSamples, UInt8**& outSamples, int& outNbSamples);

    struct SwrContext* ctx;
    AudioCodec iparams;
    AudioCodec oparams;
};


void initDecodedAudioPacket(const AVStream* stream, const AVCodecContext* ctx, const AVFrame* frame, AVPacket* opacket);


} } // namespace scy::av


#endif
#endif    // SCY_MEDIA_AudioContext_H



    //double maxFPS;
        // Maximum decoding FPS.
        // FPS is calculated from ipacket PTS.
        // Extra frames will be dropped.
    //virtual void reset();
        // Decodes a single frame from the provided packet.
        // Returns the size of the decoded frame.
        // IMPORTANT: In order to ensure all data is decoded from the
        // input packet, this method should be looped until the input
        // packet size is 0.
        // Example:
        //    while (packet.size > 0) {
        //        decode(packet);
        //    }
/*
//, AVCodecContext* ocontext, int encFrameSize
    //AVFrame* iframe
    //AVFrame* oframe;
    //struct AVResampleContext* ctx;
    //int inNbSmaples;
    //int outFrameSize; // output frame size
*/

    // Internal data
    //AVCodec*            codec;
    //AVPacket*            packet;

    // The output frame size for encoding and decoding.
    //int                    frameSize;

    //int                offset;

    // Exposed properties
    /*
    int bitRate;
    int sampleRate;
    int bitsPerSample;
    int channels;
    */
