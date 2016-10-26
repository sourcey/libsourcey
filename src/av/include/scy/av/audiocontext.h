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
#include "scy/packetsignal.h"

#ifdef HAVE_FFMPEG

#include "scy/av/types.h"
#include "scy/av/ffmpeg.h"
#include "scy/av/format.h"
#include "scy/av/fpscounter.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
// #include <libavutil/opt.h>
// #include <libavutil/audio_fifo.h>
}


namespace scy {
namespace av {


struct AudioResampler;


struct AudioContext
{
    AudioContext();
    virtual ~AudioContext();

    virtual void create() = 0;
        // Initialize the AVCodecContext with default values

    virtual void open();
        // Open the AVCodecContext

    virtual void close();
        // Close the AVCodecContext

    virtual bool recreateResampler();

    PacketSignal emitter;

    AudioCodec iparams;     // input parameters
    AudioCodec oparams;     // output parameters
    AVStream* stream;       // encoder or decoder stream
    AVCodecContext* ctx;    // encoder or decoder context
    AVCodec* codec;         // encoder or decoder codec
    AVFrame* frame;         // last encoded or decoded frame
    AudioResampler* resampler;
    int outputFrameSize;    // encoder or decoder output frame size
    std::int64_t time;      // stream time in milliseconds
    std::int64_t pts;       // encoder current pts
    // FPSCounter fps;         // encoder or decoder fps rate
    std::string error;      // error message
};


void initAudioCodecFromContext(const AVCodecContext* ctx, AudioCodec& params);
AVSampleFormat selectSampleFormat(AVCodec* codec, av::AudioCodec& params);
bool isSampleFormatSupported(AVCodec* codec, enum AVSampleFormat sampleFormat);
void initDecodedAudioPacket(const AVStream* stream, const AVCodecContext* ctx, const AVFrame* frame, AVPacket* opacket);


} } // namespace scy::av


#endif
#endif // SCY_AV_AudioContext_H
