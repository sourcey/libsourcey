///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup av
/// @{


#ifndef SCY_AV_VideoEncoder_H
#define SCY_AV_VideoEncoder_H


#include "scy/base.h"

#ifdef HAVE_FFMPEG

#include "scy/av/ffmpeg.h"
#include "scy/av/format.h"
#include "scy/av/packet.h"
#include "scy/av/videocontext.h"


namespace scy {
namespace av {


struct VideoEncoder : public VideoContext
{
    VideoEncoder(AVFormatContext* format = nullptr);
    virtual ~VideoEncoder();

    virtual void create() override;
    virtual void close() override;

    /// Encode a single video frame.
    /// The pts argument should be in stream base time format.
    /// This method is for interleaved video formats.
    virtual bool encode(uint8_t* data, int size, int64_t pts);

    /// Encode a single video frame.
    /// The pts argument should be in stream base time format.
    /// This method is for planar video formats.
    virtual bool encode(uint8_t* data[4], int linesize[4], int64_t pts);

    /// Encode a single AVFrame.
    virtual bool encode(AVFrame* iframe);

    /// Flush remaining packets to be encoded.
    /// This method should be called once before stream closure.
    virtual void flush();

    AVFormatContext* format;
};


} // namespace av
} // namespace scy


#endif
#endif // SCY_AV_VideoEncoder_H


/// @\}
