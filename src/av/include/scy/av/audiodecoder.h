///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup av
/// @{


#ifndef SCY_AV_AudioDecoder_H
#define SCY_AV_AudioDecoder_H


#include "scy/base.h"

#ifdef HAVE_FFMPEG

#include "scy/av/types.h"
// #include "scy/av/ffmpeg.h"
// #include "scy/av/format.h"
#include "scy/av/audiocontext.h"

// extern "C" {
// #include <libavcodec/avcodec.h>
// #include <libavformat/avformat.h>
// // #include <libavutil/opt.h>
// // #include <libavutil/audio_fifo.h>
// }


namespace scy {
namespace av {


struct AudioDecoder : public AudioContext
{
    AudioDecoder(AVStream* stream); // AVFormatContext* format,
    virtual ~AudioDecoder();

    virtual void create();
    virtual void close();

    virtual bool decode(std::uint8_t* data, int size); //, AVPacket& opacket
    virtual bool decode(AVPacket& ipacket);            //, AVPacket& opacket
    /// Decodes a the given input packet.
    /// Returns true an output packet was returned,    // false otherwise.

    virtual void flush(); // AVPacket& opacket
    /// Flushes buffered frames.
    /// This method should be called once after decoding.
};


} // namespace av
} // namespace scy


#endif
#endif // SCY_AV_AudioDecoder_H


/// @\}
