///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup av
/// @{


#ifndef SCY_AV_VideoConverter_H
#define SCY_AV_VideoConverter_H


#include "scy/base.h"

#ifdef HAVE_FFMPEG

#include "scy/av/packet.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}


namespace scy {
namespace av {


struct VideoConverter
{
    VideoConverter();
    virtual ~VideoConverter();

    virtual void create();
    virtual void close();

    virtual AVFrame* convert(AVFrame* iframe);

    SwsContext* ctx;
    AVFrame* oframe;
    VideoCodec iparams;
    VideoCodec oparams;
};


} // namespace av
} // namespace scy


#endif
#endif // SCY_AV_VideoConverter_H


/// @\}
