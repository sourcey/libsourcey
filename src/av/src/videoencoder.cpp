///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:    LGPL-2.1+
//
/// @addtogroup av
/// @{


#include "scy/av/videoencoder.h"

#ifdef HAVE_FFMPEG

#include "scy/logger.h"


using std::endl;


namespace scy {
namespace av {


VideoEncoder::VideoEncoder(AVFormatContext* format)
    : format(format)
{
}


VideoEncoder::~VideoEncoder()
{
    close();
}


void VideoEncoder::create() //, const VideoCodec& params
{
    // Find the video encoder
    codec = avcodec_find_encoder_by_name(oparams.encoder.c_str());
    if (!codec) {
        if (format)
            codec = avcodec_find_encoder(format->oformat->video_codec);
        if (!codec)
            throw std::runtime_error("Video encoder not found: " +
                                     oparams.encoder);
    }

    // Allocate stream and AVCodecContext from the AVFormatContext if available
    if (format) {
        format->oformat->video_codec = codec->id;

        // Add a video stream that uses the format's default video
        // codec to the format context's streams[] array.
        stream = avformat_new_stream(format, codec);
        if (!stream)
            throw std::runtime_error("Cannot create video stream.");

        // // Testing realtime streams
        // //
        // http://stackoverflow.com/questions/16768794/muxing-from-video-and-video-files-with-ffmpeg
        // stream->time_base.den = 1000; //realtime_ ? 1000 : fps_.num;
        // stream->time_base.num = 1; //realtime_ ? 1: fps_.den;
        //
        // stream->r_frame_rate.num = oparams.fps;
        // stream->r_frame_rate.den = 1;
        // stream->avg_frame_rate.den = 1;
        // stream->avg_frame_rate.num = oparams.fps;

        ctx = stream->codec;
    }

    // Otherwise allocate the standalone AVCodecContext
    else {
        ctx = avcodec_alloc_context3(codec);
        if (!ctx)
            throw std::runtime_error("Cannot allocate encoder context.");
    }

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
    assert(oparams.bitRate);
    ctx->bit_rate = oparams.bitRate;
    ctx->bit_rate_tolerance =
        oparams.bitRate * 1000; // needed when time_base.num > 1

    // Emit one intra frame every ten
    ctx->gop_size = 10;

    // Set some defaults for codecs of note.
    // Also set optimal output pixel formats if the
    // default AV_PIX_FMT_YUV420P was given.
    switch (ctx->codec_id) {
        case AV_CODEC_ID_H264:
            // TODO: Use oparams.quality to determine profile?
            av_opt_set(ctx->priv_data, "preset", "slow",
                       0); // veryfast // slow // baseline
            break;
        case AV_CODEC_ID_MJPEG:
        case AV_CODEC_ID_LJPEG:
            if (ctx->pix_fmt == AV_PIX_FMT_YUV420P)
                ctx->pix_fmt = AV_PIX_FMT_YUVJ420P;

            // Use high quality JPEG
            // TODO: Use oparams.quality to determine values
            // ctx->mb_lmin        = ctx->lmin = ctx->qmin * FF_QP2LAMBDA;
            // ctx->mb_lmax        = ctx->lmax = ctx->qmax * FF_QP2LAMBDA;
            ctx->flags = CODEC_FLAG_QSCALE;
            ctx->global_quality = ctx->qmin * FF_QP2LAMBDA;
            break;
        case AV_CODEC_ID_MPEG2VIDEO:
            ctx->max_b_frames = 2;
            break;
        case AV_CODEC_ID_MPEG1VIDEO:
        case AV_CODEC_ID_MSMPEG4V3:
            // Needed to avoid using macroblocks in which some codecs overflow
            // this doesn't happen with normal video, it just happens here as
            // the
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

    // Some formats want stream headers to be separate
    if (format && format->oformat->flags & AVFMT_GLOBALHEADER)
        ctx->flags |= CODEC_FLAG_GLOBAL_HEADER;

    // Allocate the input frame
    frame = createVideoFrame(av_get_pix_fmt(iparams.pixelFmt.c_str()),
                             iparams.width, iparams.height);
    if (!frame)
        throw std::runtime_error("Cannot allocate input frame.");

    int ret = avcodec_open2(ctx, codec, nullptr);
    if (ret < 0)
        throw std::runtime_error("Cannot open the video codec: " +
                                 averror(ret));

    // Update any output parameters that might have changed
    initVideoCodecFromContext(stream, ctx, oparams);
}


void VideoEncoder::close()
{
    TraceS(this) << "Closing" << endl;

    VideoContext::close();

#if 0
    if (buffer) {
        av_free(buffer);
        buffer = nullptr;
    }

    // Free the stream
    if (stream && format && format->nb_streams) {
        for (unsigned int i = 0; i < format->nb_streams; i++) {
            if (format->streams[i] == stream) {
                TraceS(this) << "Closing: Removing stream: " << stream << endl;
                av_freep(&format->streams[i]->codec);
                av_freep(&format->streams[i]);
                stream = nullptr;
                format->nb_streams--;
            }
        }
    }
#endif
}


bool VideoEncoder::encode(unsigned char* data, int size, std::int64_t pts)
{
    assert(data);
    assert(size);
    // assert(ipacket.data);
    assert(frame);
    assert(codec);

    // Populate the input frame with date from the given buffer.
    // NOTE: This only works with contiguous buffers
    frame->data[0] = reinterpret_cast<std::uint8_t*>(data);

    // TODO: assert size and update conversion context if required

    frame->pts = pts;

    AVPacket opacket;
    return encode(frame); //, opacket

    // AVPacket ipacket;
    // av_init_packet(&ipacket);
    // ipacket.data = data;
    // ipacket.size = size;
    // ipacket.pts = pts;
    // if (stream)
    //     ipacket.stream_index = stream->index;
    // return encode(ipacket, opacket);
}


// bool VideoEncoder::encode(AVPacket& ipacket, AVPacket& opacket)
// {
//     assert(ipacket.data);
//     assert(frame);
//     assert(codec);
//
//     // Populate the input frame with date from the given buffer.
//     // NOTE: This only works with contiguous buffers
//     frame->data[0] = reinterpret_cast<std::uint8_t*>(ipacket.data);
//
//     // TODO: Correctly set the input frame PTS
//     //
//     http://thompsonng.blogspot.com.au/2011/09/ffmpeg-avinterleavedwriteframe-return.html
//     //
//     http://stackoverflow.com/questions/6603979/ffmpegavcodec-encode-video-setting-pts-h264
//     // (1 / oparams.fps) * sample rate * frame number
//     frame->pts = ipacket.pts;
//
//     // _options.iformat.video.width = iframe->width;
//     // _options.iformat.video.height = iframe->height;
//     // iparams.width = frame->width;
//     // iparams.height = frame->height;
//
//     // avpicture_fill((AVPicture *)frame, (std::uint8_t*)ipacket.data,
//     //    av_get_pix_fmt(iparams.pixelFmt), iparams.width, iparams.height);
//
//     return encode(frame, opacket);
// }


void emitPacket(VideoEncoder* enc, AVPacket& opacket)
{
    // auto pixelFmt = av_get_pix_fmt(enc->oparams.pixelFmt.c_str());
    // assert(av_pix_fmt_count_planes(pixelFmt) == 1 && "planar formats not
    // supported");

    // enc->time = enc->frame->pkt_pts > 0 ?
    // static_cast<std::int64_t>(enc->frame->pkt_pts *
    // av_q2d(enc->stream->time_base) * 1000) : 0;
    // enc->pts = enc->frame->pkt_pts;

    // Compute stream time in milliseconds
    if (enc->stream && opacket.pts >= 0) {
        enc->time = static_cast<std::int64_t>(
            opacket.pts * av_q2d(enc->stream->time_base) * 1000);
    }
    enc->pts = opacket.pts;

    assert(opacket.data);
    assert(opacket.size);
    // assert(opacket.pts >= 0);
    // assert(opacket.dts >= 0);

    VideoPacket video(opacket.data, opacket.size, enc->ctx->coded_frame->width,
                      enc->ctx->coded_frame->height, enc->time);
    video.source = &opacket;
    video.opaque = enc;
    enc->emitter.emit(/*enc, */ video);
}


bool VideoEncoder::encode(AVFrame* iframe)
{
    // assert(iframe);
    // assert(iframe->data[0]);
    assert(ctx);
    assert(codec);

    AVPacket opacket;
    av_init_packet(&opacket);
    opacket.data = nullptr; // using encoder assigned buffer
    opacket.size = 0;

    int frameEncoded = 0;
    int ret =
        avcodec_encode_video2(ctx, &opacket, convert(iframe), &frameEncoded);
    if (ret < 0) {
        error = "Video encoder error: " + averror(ret);
        ErrorS(this) << error << endl;
        throw std::runtime_error(error);
    }

    if (frameEncoded) {
        // fps.tick();
        if (ctx->coded_frame->key_frame)
            opacket.flags |= AV_PKT_FLAG_KEY;
        if (stream) {
            opacket.stream_index = stream->index;
            // if (opacket.pts != AV_NOPTS_VALUE)
            //     opacket.pts = av_rescale_q(opacket.pts, ctx->time_base,
            //     stream->time_base);
            // if (opacket.dts != AV_NOPTS_VALUE)
            //     opacket.dts = av_rescale_q(opacket.dts, ctx->time_base,
            //     stream->time_base);
            // if (opacket.duration > 0)
            //     opacket.duration = (int)av_rescale_q(opacket.duration,
            //     ctx->time_base, stream->time_base);

            TraceS(this) << "Encoded frame:"
                         << "\n\tScaled PTS: " << opacket.pts
                         << "\n\tScaled DTS: " << opacket.dts
                         << "\n\tScaled Duration: " << opacket.duration << endl;

            emitPacket(this, opacket);
        }

        return true;
    }

    av_packet_unref(&opacket);

    return false;
}


void VideoEncoder::flush()
{
    do {
        // :)
    } while (encode(nullptr));
}


} // namespace av
} // namespace scy


#endif


/// @\}
