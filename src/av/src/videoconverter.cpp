///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup av
/// @{


#include "scy/av/videoencoder.h"

#ifdef HAVE_FFMPEG

#include "scy/logger.h"


using std::endl;


namespace scy {
namespace av {


VideoConverter::VideoConverter() :
    ctx(nullptr),
    oframe(nullptr)
{
}


VideoConverter::~VideoConverter()
{
    close();
}


void VideoConverter::create()
{
//#if 0
    TraceS(this) << "Create:"
        << "\n\tInput Width: " << iparams.width
        << "\n\tInput Height: " << iparams.height
        << "\n\tInput Pixel Format: " << iparams.pixelFmt
        << "\n\tOutput Width: " << oparams.width
        << "\n\tOutput Height: " << oparams.height
        << "\n\tOutput Pixel Format: " << oparams.pixelFmt
        << endl;
//#endif

    if (ctx)
        throw std::runtime_error("Conversion context already initialized.");

    oframe = createVideoFrame(av_get_pix_fmt(oparams.pixelFmt.c_str()), oparams.width, oparams.height);

    ctx = sws_getContext(
        iparams.width, iparams.height, av_get_pix_fmt(iparams.pixelFmt.c_str()),
        oparams.width, oparams.height, av_get_pix_fmt(oparams.pixelFmt.c_str()),
        /* SWS_FAST_BILINEAR */SWS_BICUBIC, nullptr, nullptr, nullptr);
    if (!ctx)
        throw std::runtime_error("Invalid conversion context.");

    TraceS(this) << "Create: OK: " << ctx << endl;
}


void VideoConverter::close()
{
    TraceS(this) << "Closing" << endl;

    if (oframe) {
        av_free(oframe);
        oframe = nullptr;
    }

    if (ctx) {
        sws_freeContext(ctx);
        ctx = nullptr;
    }

    TraceS(this) << "Closing: OK" << endl;
}


AVFrame* VideoConverter::convert(AVFrame* iframe)
{
    TraceS(this) << "Convert:"
        << "\n\tIn Format: " << iparams.pixelFmt
        << "\n\tIn Size: " << iframe->width << "x" << iframe->height
        << "\n\tOut Format: " << oparams.pixelFmt
        << "\n\tOut Size: " << oparams.width << "x" << oparams.height
        << "\n\tPTS: " << iframe->pkt_pts
        << endl;

    assert(iframe);
    assert(iframe->data[0]);
    assert(iframe->width == iparams.width);
    assert(iframe->height == iparams.height);

    if (!ctx)
        throw std::runtime_error("Conversion context must be initialized.");

    if (sws_scale(ctx,
        iframe->data, iframe->linesize, 0, iparams.height,
        oframe->data, oframe->linesize) < 0)
        throw std::runtime_error("Pixel format conversion not supported.");

    // Copy input frame properties to output frame
    av_frame_copy_props(oframe, iframe);

    // oframe->format = av_get_pix_fmt(oparams.pixelFmt.c_str()); //ctx->pix_fmt;
    // oframe->width  = oparams.width; //iframe->width;
    // oframe->height = oparams.height; //iframe->height;
    // oframe->pkt_pts = iframe->pkt_pts;
    // oframe->pkt_dts = iframe->pkt_dts;
    // oframe->pts = iframe->pts;
    // oframe->dts = iframe->dts;

    // Set the input PTS or a monotonic value to keep the encoder happy.
    // The actual setting of the PTS is outside the scope of this encoder.
    // oframe->pts = iframe->pts != AV_NOPTS_VALUE ? iframe->pts : ctx->frame_number;

    return oframe;
}


} } // namespace scy::av


#endif

/// @\}
