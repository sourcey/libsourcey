///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup av
/// @{


#ifndef SCY_AV_VideoDecoder_H
#define SCY_AV_VideoDecoder_H


#include "scy/base.h"

#ifdef HAVE_FFMPEG

#include "scy/av/types.h"
// #include "scy/av/format.h"
// #include "scy/av/ffmpeg.h"
#include "scy/av/videocontext.h"


namespace scy {
namespace av {


struct VideoDecoder : public VideoContext
{
    VideoDecoder(AVStream* stream);
    virtual ~VideoDecoder();

    virtual void create();
    virtual void open();
    virtual void close();

    virtual bool decode(uint8_t* data, int size); //, AVPacket& opacket
    virtual bool decode(AVPacket& ipacket);            //, AVPacket& opacket
    /// Decodes a the given input packet.
    /// Returns true an output packet was returned,    // false otherwise.

    virtual void flush(); // AVPacket& opacket
    /// Flushes buffered frames.
    /// This method should be called after decoding
    /// until false is returned.
};


} // namespace av
} // namespace scy


#endif
#endif // SCY_AV_VideoDecoder_H


/// @\}
