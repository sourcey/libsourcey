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


#include "scy/av/videocontext.h"

#ifdef HAVE_FFMPEG

#include "scy/logger.h"


using std::endl;


namespace scy {
namespace av {


VideoContext::VideoContext() :
    stream(nullptr),
    ctx(nullptr),
    codec(nullptr),
    frame(nullptr),
    pts(0.0)
{
    TraceS(this) << "Create" << endl;
    //reset();
    initializeFFmpeg();
}


VideoContext::~VideoContext()
{
    TraceS(this) << "Destroy" << endl;

    //assert((!frame && !codec && !stream) && "video context must be closed");
    close();
    uninitializeFFmpeg();
}


void VideoContext::create()
{
}


void VideoContext::open()
{
    TraceS(this) << "Opening" << endl;
    assert(ctx);
    assert(codec);

    // Open the video codec
    if (avcodec_open2(ctx, codec, nullptr) < 0)
           throw std::runtime_error("Cannot open the video codec.");
}


void VideoContext::close()
{
    TraceS(this) << "Closing" << endl;

    if (frame) {
        av_free(frame);
        frame = nullptr;
    }

    if (ctx) {
        avcodec_close(ctx);
        ctx = nullptr;
    }

    // Streams are managed differently by each impl
    //if (stream)    {
        //stream = nullptr;
        // Note: The stream is managed by the AVFormatContext
        //av_freep(stream);
    //}

    pts = 0.0;
    error = "";

    TraceS(this) << "Closing: OK" << endl;
}


//
// Helper functions
//


AVFrame* createVideoFrame(AVPixelFormat pixelFmt, int width, int height)
{
    auto picture = av_frame_alloc();
    if (!picture)
        return nullptr;

    int size = av_image_get_buffer_size(pixelFmt, width, height, 16);
    auto buffer = reinterpret_cast<std::uint8_t*>(av_malloc(size));
    if (!buffer) {
        av_free(picture);
        return nullptr;
    }

    av_image_fill_arrays(picture->data, picture->linesize, buffer, pixelFmt, width, height, 1);

    // FFmpeg v3.1.4 does not set width and height values for us anymore
    picture->width = width;
    picture->height = height;

    return picture;
}


void initVideoEncoderContext(AVCodecContext* ctx, AVCodec* codec, VideoCodec& oparams)
{
    assert(oparams.enabled);

    avcodec_get_context_defaults3(ctx, codec);
    ctx->codec_id = codec->id;
    ctx->codec_type = AVMEDIA_TYPE_VIDEO;
    ctx->pix_fmt = av_get_pix_fmt(oparams.pixelFmt.c_str());
    ctx->frame_number = 0;
    ctx->max_b_frames = 1;

    // Resolution must be a multiple of two
    ctx->width = oparams.width;
    ctx->height = oparams.height;

    // For fixed-fps content timebase should be 1/framerate
    // and timestamp increments should be identically 1.
    ctx->time_base.den = (int)oparams.fps;
    ctx->time_base.num = 1;

    // Define encoding parameters
    ctx->bit_rate = oparams.bitRate;
    ctx->bit_rate_tolerance = oparams.bitRate * 1000; // needed when time_base.num > 1

    // Emit one intra frame every ten
    ctx->gop_size = 10;

    // Set some defaults for codecs of note.
    // Also set optimal output pixel formats if the
    // default AV_PIX_FMT_YUV420P was given.
    switch (ctx->codec_id) {
    case AV_CODEC_ID_H264:
        // TODO: Use oparams.quality to determine profile?
        av_opt_set(ctx->priv_data, "preset", "slow", 0); // veryfast // slow // baseline
        break;
    case AV_CODEC_ID_MJPEG:
    case AV_CODEC_ID_LJPEG:
        if (ctx->pix_fmt == AV_PIX_FMT_YUV420P)
            ctx->pix_fmt = AV_PIX_FMT_YUVJ420P;

        // Use high quality JPEG
        // TODO: Use oparams.quality to determine values
        // ctx->mb_lmin        = ctx->lmin = ctx->qmin * FF_QP2LAMBDA;
        // ctx->mb_lmax        = ctx->lmax = ctx->qmax * FF_QP2LAMBDA;
        ctx->flags          = CODEC_FLAG_QSCALE;
        ctx->global_quality = ctx->qmin * FF_QP2LAMBDA;
        break;
    case AV_CODEC_ID_MPEG2VIDEO:
        ctx->max_b_frames = 2;
        break;
    case AV_CODEC_ID_MPEG1VIDEO:
    case AV_CODEC_ID_MSMPEG4V3:
        // Needed to avoid using macroblocks in which some codecs overflow
         // this doesn't happen with normal video, it just happens here as the
         // motion of the chroma plane doesn't match the luma plane
         // avoid FFmpeg warning 'clipping 1 dct coefficients...'
        ctx->mb_decision = 2;
    break;
    case AV_CODEC_ID_JPEGLS:
        // AV_PIX_FMT_BGR24 or GRAY8 depending on if color...
        if (ctx->pix_fmt == AV_PIX_FMT_YUV420P)
            ctx->pix_fmt = AV_PIX_FMT_BGR24;
        break;
    case AV_CODEC_ID_HUFFYUV:
        if (ctx->pix_fmt == AV_PIX_FMT_YUV420P)
            ctx->pix_fmt = AV_PIX_FMT_YUV422P;
        break;
    default:
      break;
    }

    // Update any modified values
    oparams.pixelFmt = av_get_pix_fmt_name(ctx->pix_fmt);
}


void initDecodedVideoPacket(const AVStream* stream, const AVCodecContext* ctx, const AVFrame* frame, AVPacket* opacket, double* pts)
{
    opacket->data = frame->data[0];
    // opacket->size = avpicture_get_size(ctx->pix_fmt, ctx->width, ctx->height);
    opacket->size = av_image_get_buffer_size(ctx->pix_fmt, ctx->width, ctx->height, 16);
    opacket->dts = frame->pkt_dts; // Decoder PTS values can be unordered
    opacket->pts = frame->pkt_pts;

    // Local PTS value represented as decimal seconds
    if (opacket->dts != AV_NOPTS_VALUE) {
        *pts = (double)opacket->pts;
        *pts *= av_q2d(stream->time_base);
    }

    assert(opacket->data);
    assert(opacket->size);
    //assert(opacket->dts >= 0);
    //assert(opacket->pts >= 0);

    // TraceL << "[VideoContext] Init Decoded Frame Pcket:"
    //     << "\n\tFrame DTS: " << frame->pkt_dts
    //     << "\n\tFrame PTS: " << frame->pkt_pts
    //     << "\n\tPacket Size: " << opacket->size
    //     << "\n\tPacket DTS: " << opacket->dts
    //     << "\n\tPacket PTS: " << opacket->pts
    //     << endl;
}


void initVideoCodecFromContext(const AVCodecContext* ctx, VideoCodec& params)
{
    params.enabled = true;
    params.encoder = avcodec_get_name(ctx->codec_id);
    params.pixelFmt = av_get_pix_fmt_name(ctx->pix_fmt);
    params.width = ctx->width;
    params.height = ctx->height;
    params.sampleRate = ctx->sample_rate;
    params.bitRate = ctx->bit_rate;
    params.fps =
        ctx->time_base.den /
        ctx->time_base.num;
}


void printAvailableEncoders(std::ostream& ost, const char* delim)
{
    AVCodec* p = av_codec_next(NULL);
    while (p) {
        if (av_codec_is_encoder(p))
            ost << p->name << delim;
        p = p->next;
    }
}


} } // namespace scy::av


#endif
