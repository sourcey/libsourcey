///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup av
/// @{


#include "scy/av/videocontext.h"

#ifdef HAVE_FFMPEG

#include "scy/logger.h"


using std::endl;


namespace scy {
namespace av {


VideoContext::VideoContext()
    : stream(nullptr)
    , ctx(nullptr)
    , codec(nullptr)
    , frame(nullptr)
    , conv(nullptr)
    , time(0)
    , pts(AV_NOPTS_VALUE)
    , seconds(0)
{
    LTrace("Create")
    initializeFFmpeg();
}


VideoContext::~VideoContext()
{
    LTrace("Destroy")

    close();
    uninitializeFFmpeg();
}


void VideoContext::create()
{
}


void VideoContext::open()
{
    SDebug << "Open: "
                 << "\n\tInput: " << iparams.toString()
                 << "\n\tOutput: " << oparams.toString() << endl;

    assert(ctx);
    assert(avcodec_is_open(ctx) && "avcodec_open2 must be called");
    assert(codec);

    // NOTE: The codec must be opened prior to calling open()
    // // Open the video codec
    // if (avcodec_open2(ctx, codec, nullptr) < 0)
    //     throw std::runtime_error("Cannot open the video codec.");

    // Create the video conversion context if required
    recreateConverter();
}


void VideoContext::close()
{
    LTrace("Closing")

    if (frame) {
        av_free(frame);
        frame = nullptr;
    }

    if (ctx) {
        avcodec_close(ctx);
        ctx = nullptr;
    }

    if (conv) {
        delete conv;
        conv = nullptr;
    }

    // Streams are managed differently by the external impl
    // if (stream)    {
    //     stream = nullptr;
    //     Note: The stream is managed by the AVFormatContext
    //     av_freep(stream);
    //}

    time = 0;
    pts = AV_NOPTS_VALUE;
    seconds = 0;
    error = "";

    LTrace("Closing: OK")
}


AVFrame* VideoContext::convert(AVFrame* iframe) //, VideoCodec& cparams
{
    // While flushing the input frame may be null
    if (!iframe)
        return nullptr;

    assert(iframe->width == iparams.width);
    assert(iframe->height == iparams.height);

    // Recreate the video conversion context on the fly
    // if the input resolution changes.

    //if (iframe->width != /*conv->*/ oparams.width ||
    //    iframe->height != /*conv->*/ oparams.height ||
    //    iframe->format != /*conv->*/ av_get_pix_fmt(oparams.pixelFmt.c_str())) {
    //if (iframe->width != conv->iparams.width ||
    //    iframe->height != conv->iparams.height ||
    //    iframe->format != av_get_pix_fmt(conv->iparams.pixelFmt.c_str()) ||
    //    oparams.width != conv->oparams.width ||
    //    oparams.height != conv->oparams.height ||
    //    av_get_pix_fmt(oparams.pixelFmt.c_str()) != av_get_pix_fmt(conv->oparams.pixelFmt.c_str())) {
        iparams.width = iframe->width;
        iparams.height = iframe->height;
        iparams.pixelFmt = av_get_pix_fmt_name((AVPixelFormat)iframe->format);
        recreateConverter();
    //}

    // Return the input frame if no conversion is required
    if (!conv)
        return iframe;

    // // Set the input PTS or a monotonic value to keep the encoder happy.
    // // The actual setting of the PTS is outside the scope of this encoder.
    // cframe->pts = iframe->pts != AV_NOPTS_VALUE ? iframe->pts : ctx->frame_number;

    // Convert the input frame and return the result
    return conv->convert(iframe);
}


bool VideoContext::recreateConverter()
{
    // if (conv)
    //     throw std::runtime_error("Conversion context already exists.");

    // NOTE: the input output `width`, `height`, and `pixelFmt` parameters work
    // slightly differently for encoders and decoders.
    // For encoders `iparams` is the picture format from the application and
    // `oparams` is the picture format passed into the encoder.
    // For decoders `iparams` is the picture format from the decoder and
    // `oparams` is the picture format passed into the application.

    // Check if conversion is required
    // This check if only for when unitialized
    // If iparams or oparams is changed after initialization a conversion
    // context must be created
    if (!conv &&
        iparams.width == oparams.width &&
        iparams.height == oparams.height &&
        iparams.pixelFmt == oparams.pixelFmt) {
        return false;
    }

    // Check if the conversion context needs to be recreated
    if (conv && (conv->iparams.width == iparams.width &&
                 conv->iparams.height == iparams.height &&
                 conv->iparams.pixelFmt == iparams.pixelFmt) &&
        (conv->oparams.width == oparams.width &&
         conv->oparams.height == oparams.height &&
         conv->oparams.pixelFmt == oparams.pixelFmt)) {
        return false;
    }

    // Recreate the conversion context
    LDebug("Recreating video conversion context")
    if (conv)
        delete conv;
    conv = new VideoConverter();
    conv->iparams = iparams;
    conv->oparams = oparams;
    conv->create();
    return true;
}


//
// Helper functions
//


AVFrame* createVideoFrame(AVPixelFormat pixelFmt, int width, int height)
{
    auto picture = av_frame_alloc();
    if (!picture)
        return nullptr;

    // TODO: Replace with AVFrameHolder
    int size = av_image_get_buffer_size(pixelFmt, width, height, 16);
    auto buffer = reinterpret_cast<uint8_t*>(av_malloc(size));
    if (!buffer) {
        av_free(picture);
        return nullptr;
    }

    av_image_fill_arrays(picture->data, picture->linesize, buffer, pixelFmt, width, height, 1);

    // FFmpeg v3.1.4 does not set width and height values for us anymore
    picture->width = width;
    picture->height = height;
    picture->format = pixelFmt;

    return picture;
}


AVFrame* cloneVideoFrame(AVFrame* source)
{
    AVFrame* copy = av_frame_alloc();
    copy->format = source->format;
    copy->width = source->width;
    copy->height = source->height;
    copy->channels = source->channels;
    copy->channel_layout = source->channel_layout;
    copy->nb_samples = source->nb_samples;
    av_frame_get_buffer(copy, 32);
    av_frame_copy(copy, source);
    av_frame_copy_props(copy, source);
    return copy;
}


void initVideoCodecFromContext(const AVStream* stream, const AVCodecContext* ctx, VideoCodec& params)
{
    params.enabled = true;
    params.encoder = avcodec_get_name(ctx->codec_id);
    params.pixelFmt = av_get_pix_fmt_name(ctx->pix_fmt);
    params.width = ctx->width;
    params.height = ctx->height;
    params.sampleRate = ctx->sample_rate;
    params.bitRate = ctx->bit_rate;
    if (stream && stream->r_frame_rate.num) {
        params.fps = stream->r_frame_rate.num / stream->r_frame_rate.den;
    }
}

AVPixelFormat selectPixelFormat(AVCodec* codec, VideoCodec& params)
{
    enum AVPixelFormat compatible = AV_PIX_FMT_NONE;
    enum AVPixelFormat requested = av_get_pix_fmt(params.pixelFmt.c_str());
    int nplanes = av_pix_fmt_count_planes(requested);
    const enum AVPixelFormat* p = codec->pix_fmts;
    while (*p != AV_PIX_FMT_NONE) {
        if (compatible == AV_PIX_FMT_NONE && (nplanes == 0 || av_pix_fmt_count_planes(*p) == nplanes))
            compatible = *p; // or use the first compatible format
        if (*p == requested)
            return requested; // always try to return requested format
        p++;
    }
    if (compatible == AV_PIX_FMT_NONE)
        compatible = p[0];
    return compatible;
}


} // namespace av
} // namespace scy


#endif


/// @\}
