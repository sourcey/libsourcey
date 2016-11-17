///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup av
/// @{


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

    virtual void create();    /// virtual void open();
    virtual void close();    /// Encode an arbitrary number of interleaved audio samples.
    ///
    /// @param samples    The input samples to encode.
    /// @param numSamples The number of input samples per channel.
    /// @param pts        The input samples presentation timestamp.
    /// @param opacket    The output packet data will be encoded to.
    virtual bool encode(/*const */std::uint8_t* samples, const int numSamples, const std::int64_t pts);

    /// Encode a single AVFrame from the decoder.
    virtual bool encode(AVFrame* iframe);

    /// Flush remaining packets to be encoded.
    /// This method should be called once before stream closure.
    virtual void flush();


    av::AudioBuffer fifo;
    AVFormatContext* format;
};


} // namespace av
} // namespace scy


#endif
#endif // SCY_AV_AudioEncoder_H

/// @\}
