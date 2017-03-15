///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
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
#include "scy/av/types.h"
#include "scy/av/videocontext.h"


namespace scy {
namespace av {


struct VideoEncoder : public VideoContext
{
    VideoEncoder(AVFormatContext* format = nullptr);
    virtual ~VideoEncoder();

    virtual void create();
    // virtual void open();
    virtual void close();

    virtual bool encode(unsigned char* data, int size,
                        int64_t pts); //, AVPacket& opacket
    /// Encode a single video frame.
    /// The pts argument should be in stream base time format.
    /// virtual bool encode(AVPacket& ipacket, AVPacket& opacket);    ///
    /// virtual bool encode(AVFrame* iframe, AVPacket& opacket);    /// virtual
    /// bool flush(AVPacket& opacket);

    virtual bool encode(AVFrame* iframe); //, AVPacket& opacket
    /// Encode a single AVFrame.
    /// Flush remaining packets to be encoded.
    /// This method should be called once before stream closure.
    virtual void flush();


    AVFormatContext* format; /// uint8_t* buffer;    /// int bufferSize;
};


} // namespace av
} // namespace scy


#endif
#endif // SCY_AV_VideoEncoder_H


/// @\}
