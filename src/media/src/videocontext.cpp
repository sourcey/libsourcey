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


#include "scy/media/videocontext.h"

#ifdef HAVE_FFMPEG

#include "scy/logger.h"


using std::endl;
using namespace scy;


namespace scy {
namespace av {


VideoContext::VideoContext() :
    stream(nullptr),
    codec(nullptr),
    frame(nullptr),
    ctx(nullptr),
    pts(0.0)
{
    TraceLS(this) << "Create" << endl;
    //reset();
}


VideoContext::~VideoContext()
{
    TraceLS(this) << "Destroy" << endl;

    //assert((!frame && !codec && !stream) && "video context must be closed");
    close();
}


void VideoContext::create()
{
}


void VideoContext::open()
{
    TraceLS(this) << "Opening" << endl;
    assert(ctx);
    assert(codec);

    // Open the video codec
    if (avcodec_open2(ctx, codec, nullptr) < 0)
           throw std::runtime_error("Cannot open the video codec.");
}


void VideoContext::close()
{
    TraceLS(this) << "Closing" << endl;

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

    TraceLS(this) << "Closing: OK" << endl;
}


// ---------------------------------------------------------------------
// Video Encoder Context
//
VideoEncoderContext::VideoEncoderContext(AVFormatContext* format) :
    format(format),
    conv(nullptr),
    buffer(nullptr),
    bufferSize(0)
{
}


VideoEncoderContext::~VideoEncoderContext()
{
    close();
}


void VideoEncoderContext::create() //, const VideoCodec& params
{
    TraceLS(this) << "Create: "
        << "\n\tInput: " << iparams.toString()
        << "\n\tOutput: " << oparams.toString()
        << endl;

    VideoContext::create();

    // Find the video encoder
    codec = avcodec_find_encoder_by_name(oparams.encoder.c_str());
    if (!codec) {
        codec = avcodec_find_encoder(format->oformat->video_codec);
        if (!codec)
               throw std::runtime_error("Video encoder not found.");
    }

    format->oformat->video_codec = codec->id;

    // Add a video stream that uses the format's default video
    // codec to the format context's streams[] array.
    stream = avformat_new_stream(format, codec);
    if (!stream)
        throw std::runtime_error("Cannot create video stream.");

    /*
    // fixme: testing realtime streams
    // http://stackoverflow.com/questions/16768794/muxing-from-audio-and-video-files-with-ffmpeg
    stream->time_base.den = 1000; //realtime_ ? 1000 : fps_.num;
    stream->time_base.num = 1; //realtime_ ? 1: fps_.den;

    stream->r_frame_rate.num = oparams.fps;
    stream->r_frame_rate.den = 1;
    stream->avg_frame_rate.den = 1;
    stream->avg_frame_rate.num = oparams.fps;
    */

    ctx = stream->codec;

    initVideoEncoderContext(ctx, codec, oparams);

    // Create the video conversion context if needed
    createConverter();

    // Some formats want stream headers to be separate
    if (format->oformat->flags & AVFMT_GLOBALHEADER)
        ctx->flags |= CODEC_FLAG_GLOBAL_HEADER;

    // Allocate the input frame
    frame = createVideoFrame(av_get_pix_fmt(iparams.pixelFmt), iparams.width, iparams.height);
    if (!frame)
        throw std::runtime_error("Cannot allocate input frame.");

    // Allocate the encode buffer
    // XXX: Disabling in favor of encoder manged buffer
    //bufferSize = avpicture_get_size(ctx->pix_fmt, ctx->width, ctx->height);
    //buffer = (std::uint8_t*)av_malloc(bufferSize);
}


void VideoEncoderContext::createConverter()
{
    if (conv)
        throw std::runtime_error("A conversion context already exists.");

    // Create the conversion context
    if (iparams.width != oparams.width ||
        iparams.height != oparams.height ||
        strcmp(iparams.pixelFmt, oparams.pixelFmt) != 0) {
        conv = new VideoConversionContext();
        conv->create(iparams, oparams);
    }
}


void VideoEncoderContext::freeConverter()
{
    if (conv) {
        delete conv;
        conv = nullptr;
    }

    //if (frame) {
    //    av_free(frame);
    //    frame = nullptr;
    //}
}


void VideoEncoderContext::close()
{
    TraceLS(this) << "Closing" << endl;

    VideoContext::close();

    freeConverter();

#if 0
    if (buffer) {
        av_free(buffer);
        buffer = nullptr;
    }

    // Free the stream
    if (stream && format && format->nb_streams) {
        for (unsigned int i = 0; i < format->nb_streams; i++) {
            if (format->streams[i] == stream) {
                TraceLS(this) << "Closing: Removing stream: " << stream << endl;
                av_freep(&format->streams[i]->codec);
                av_freep(&format->streams[i]);
                stream = nullptr;
                format->nb_streams--;
            }
        }
    }
#endif
}


bool VideoEncoderContext::encode(unsigned char* data, int size, std::int64_t pts, AVPacket& opacket)
{
    assert(data);
    assert(size);
    AVPacket ipacket;
    av_init_packet(&ipacket);
    ipacket.stream_index = stream->index;
    ipacket.data = data;
    ipacket.size = size;
    ipacket.pts = pts;
    return encode(ipacket, opacket);
}


bool VideoEncoderContext::encode(AVPacket& ipacket, AVPacket& opacket)
{
    assert(ipacket.data);
    assert(stream);
    assert(frame);
    assert(codec);
    frame->pts = ipacket.pts;
    frame->data[0] = (std::uint8_t*)ipacket.data;
    //avpicture_fill((AVPicture *)frame, (std::uint8_t*)ipacket.data,
    //    av_get_pix_fmt(iparams.pixelFmt), iparams.width, iparams.height);

    return encode(frame, opacket);
}


bool VideoEncoderContext::encode(AVFrame* iframe, AVPacket& opacket)
{
    assert(frame);
    assert(frame->data[0]);
    assert(codec);

    AVFrame* oframe = conv ? conv->convert(iframe) : iframe;

    // Set the input PTS or a monotonic value to keep the encoder happy.
    // The actual setting of the PTS is outside the scope of the encoder.
    oframe->pts = iframe->pts != AV_NOPTS_VALUE ? iframe->pts : ctx->frame_number;

    oframe->format = ctx->pix_fmt;
    //oframe->width  = width;
    //oframe->height = height;

    av_init_packet(&opacket);
    opacket.stream_index = stream->index;
    opacket.data = nullptr; // using encoder assigned buffer
    opacket.size = 0;
    //opacket.data = this->buffer;
    //opacket.size = this->bufferSize;

    int frameEncoded = 0;

    if (avcodec_encode_video2(ctx, &opacket, oframe, &frameEncoded) < 0) {
        // TODO: Use av_strerror
        error = "Fatal encoder error";
        ErrorLS(this) << error << endl;
        throw std::runtime_error(error);
    }

    if (frameEncoded) {
        fps.tick();
        if (ctx->coded_frame->key_frame)
            opacket.flags |= AV_PKT_FLAG_KEY;
        if (opacket.pts != AV_NOPTS_VALUE)
            opacket.pts = av_rescale_q(opacket.pts, ctx->time_base, stream->time_base);
        if (opacket.dts != AV_NOPTS_VALUE)
            opacket.dts = av_rescale_q(opacket.dts, ctx->time_base, stream->time_base);
        if (opacket.duration > 0)
            opacket.duration = (int)av_rescale_q(opacket.duration, ctx->time_base, stream->time_base);

        TraceLS(this) << "Encoded Frame:"
            << "\n\tScaled PTS: " << opacket.pts
            << "\n\tScaled DTS: " << opacket.dts
            << "\n\tScaled Duration: " << opacket.duration
            << endl;
    }

    return frameEncoded > 0;
}


bool VideoEncoderContext::flush(AVPacket& opacket)
{
    av_init_packet(&opacket);
    opacket.data = nullptr;
    opacket.size = 0;

    int frameEncoded = 0;
    if (avcodec_encode_video2(ctx, &opacket, nullptr, &frameEncoded) < 0) {
        // TODO: Use av_strerror
        error = "Fatal encoder error";
        ErrorLS(this) << error << endl;
        throw std::runtime_error(error);
    }

    if (frameEncoded) {
        if (ctx->coded_frame->key_frame)
            opacket.flags |= AV_PKT_FLAG_KEY;
        if (opacket.pts != AV_NOPTS_VALUE)
            opacket.pts = av_rescale_q(opacket.pts, ctx->time_base, stream->time_base);
        if (opacket.dts != AV_NOPTS_VALUE)
            opacket.dts = av_rescale_q(opacket.dts, ctx->time_base, stream->time_base);
        if (opacket.duration > 0)
            opacket.duration = (int)av_rescale_q(opacket.duration, ctx->time_base, stream->time_base);
        TraceLS(this) << "Flushed Video Frame: " << opacket.pts << endl;
        return true;
    }
    return false;
}


//
// Video Codec Encoder Context
//


VideoCodecEncoderContext::VideoCodecEncoderContext() :
    conv(nullptr),
    buffer(nullptr),
    bufferSize(0)
{
}


VideoCodecEncoderContext::~VideoCodecEncoderContext()
{
    close();
}


void VideoCodecEncoderContext::create()
{
    TraceLS(this) << "Create: "
        << "\n\tInput: " << iparams.toString()
        << "\n\tOutput: " << oparams.toString()
        << endl;

    VideoContext::create();

    // Find the video encoder
    codec = avcodec_find_encoder_by_name(oparams.encoder.c_str());
    if (!codec)
           throw std::runtime_error("Video encoder not found.");

    ctx = avcodec_alloc_context3(codec);
    if (!ctx)
        throw std::runtime_error("Cannot allocate encoder context.");

    initVideoEncoderContext(ctx, codec, oparams);

    // Allocate the conversion context
    if (iparams.width != oparams.width ||
        iparams.height != oparams.height ||
        strcmp(iparams.pixelFmt, oparams.pixelFmt) == 0) {
        conv = new VideoConversionContext();
        conv->create(iparams, oparams);
    }

    // Allocate the input frame
    frame = createVideoFrame(ctx->pix_fmt, iparams.width, iparams.height);
    if (!frame)
        throw std::runtime_error("Cannot allocate input frame.");

    // Allocate the encode buffer
    // XXX: Disabling in favor of encoder manged buffer
    //bufferSize = avpicture_get_size(ctx->pix_fmt, ctx->width, ctx->height);
    //buffer = (std::uint8_t*)av_malloc(bufferSize);
}


void VideoCodecEncoderContext::close()
{
    TraceLS(this) << "Closing" << endl;

    VideoContext::close();

    if (conv) {
        delete conv;
        conv = nullptr;
    }

    if (buffer) {
        av_free(buffer);
        buffer = nullptr;
    }
}


bool VideoCodecEncoderContext::encode(unsigned char* data, int size, AVPacket& opacket)
{
    assert(data);
    assert(size);
    AVPacket ipacket;
    av_init_packet(&ipacket);
    ipacket.data = data;
    ipacket.size = size;
    return encode(ipacket, opacket);
}


bool VideoCodecEncoderContext::encode(AVPacket& ipacket, AVPacket& opacket)
{
    assert(stream == nullptr);
    assert(ipacket.data);
    assert(frame);
    assert(conv);

    frame->data[0] = (std::uint8_t*)ipacket.data;

    // TODO: Correctly set the input frame PTS
    // http://thompsonng.blogspot.com.au/2011/09/ffmpeg-avinterleavedwriteframe-return.html
    // http://stackoverflow.com/questions/6603979/ffmpegavcodec-encode-video-setting-pts-h264
    // (1 / oparams.fps) * sample rate * frame number
    frame->pts = ctx->frame_number;

    return encode(frame, opacket);
}


bool VideoCodecEncoderContext::encode(AVFrame* iframe, AVPacket& opacket)
{
    TraceLS(this) << "Encoding Video Packet" << endl;

    AVFrame* oframe = conv ? conv->convert(iframe) : iframe;
    oframe->pts = iframe->pts;

    av_init_packet(&opacket);
    opacket.data = nullptr;
    opacket.size = 0;
    //opacket.data = this->buffer; // use our buffer, not ffmpeg assigned
    //opacket.size = this->bufferSize;

    int frameEncoded = 0;
    if (avcodec_encode_video2(ctx, &opacket, oframe, &frameEncoded) < 0) {
        error = "Fatal encoder error";
        ErrorLS(this) << "Fatal encoder error" << endl;
        throw std::runtime_error(error);
    }

    if (frameEncoded) {
        fps.tick();
        if (ctx->coded_frame->key_frame)
            opacket.flags |= AV_PKT_FLAG_KEY;

        // TraceLS(this) << "Encoded PTS:\n"
        //     << "\n\tPTS: " << opacket.pts
        //     << "\n\tDTS: " << opacket.dts
        //     << endl;
    }

    return frameEncoded > 0;
}


//
// Video Decoder Context
//


VideoDecoderContext::VideoDecoderContext()
{
}


VideoDecoderContext::~VideoDecoderContext()
{
    close();
}


void VideoDecoderContext::create(AVFormatContext *ic, int streamID)
{
    TraceLS(this) << "Create: " << streamID << endl;
    VideoContext::create();

    assert(ic);
    assert(streamID >= 0);

    this->stream = ic->streams[streamID];
    this->ctx = this->stream->codec;

    codec = avcodec_find_decoder(this->ctx->codec_id);
    if (!codec)
        throw std::runtime_error("Video codec missing or unsupported.");

    this->frame = av_frame_alloc();
    if (this->frame == nullptr)
        throw std::runtime_error("Could not allocate video input frame.");
}


void VideoDecoderContext::close()
{
    VideoContext::close();
}


bool VideoDecoderContext::decode(std::uint8_t* data, int size, AVPacket& opacket)
{
    AVPacket ipacket;
    av_init_packet(&ipacket);
    ipacket.stream_index = stream->index;
    ipacket.data = data;
    ipacket.size = size;
    return decode(ipacket, opacket);
}


bool VideoDecoderContext::decode(AVPacket& ipacket, AVPacket& opacket)
{
    assert(ipacket.stream_index == stream->index);

    int frameDecoded = 0;
    int bytesDecoded = 0;
    int bytesRemaining = ipacket.size;

    av_init_packet(&opacket);
    opacket.data = nullptr;
    opacket.size = 0;

    bytesDecoded = avcodec_decode_video2(ctx, frame, &frameDecoded, &ipacket);
    if (bytesDecoded < 0) {
        error = "Decoder error";
        ErrorLS(this) << "" << error << endl;
        throw std::runtime_error(error);
    }

    // XXX: Asserting here to make sure below looping
    // avcodec_decode_video2 is actually redundant.
    // Otherwise we need to reimplement this pseudo code:
    // while(packet->size > 0)
    // {
    //      int ret = avcodec_decode_video2(..., ipacket);
    //      if(ret < -1)
    //        throw std::runtime_error("error");
    //
    //     ipacket->size -= ret;
    //     ipacket->data += ret;
    // }
    assert(bytesDecoded == bytesRemaining);

    /*
    while (bytesRemaining) { // && !frameDecoded
        //TraceLS(this) << "Decoding: " << ipacket.pts << endl;
        bytesRemaining -= bytesDecoded;
    }
    */

    if (frameDecoded) {
        fps.tick();
        initDecodedVideoPacket(stream, ctx, frame, &opacket, &pts);
#if 0
        TraceLS(this) << "Decoded Frame:"
            << "\n\tPTS: " << pts
            << "\n\tPacket Size: " << opacket.size
            << "\n\tPacket PTS: " << opacket.pts
            << "\n\tPacket DTS: " << opacket.dts
            << "\n\tFrame Packet PTS: " << frame->pkt_pts
            << "\n\tFrame Packet DTS: " << frame->pkt_dts
            << "\n\tFrame Size: " << ctx->frame_size
            << endl;
#endif

        return true;
    }
    return false;
}


bool VideoDecoderContext::flush(AVPacket& opacket)
{
    AVPacket ipacket;
    av_init_packet(&ipacket);
    ipacket.data = nullptr;
    ipacket.size = 0;

    av_init_packet(&opacket);
    opacket.data = nullptr;
    opacket.size = 0;

    int frameDecoded = 0;
    avcodec_decode_video2(ctx, frame, &frameDecoded, &ipacket);
    if (frameDecoded) {
        initDecodedVideoPacket(stream, ctx, frame, &opacket, &pts);
        TraceLS(this) << "Flushed Video Frame: " << opacket.pts << endl;
        return true;
    }
    return false;
}


//
// Video Conversion Context
//


VideoConversionContext::VideoConversionContext() :
    oframe(nullptr),
    ctx(nullptr)
{
}


VideoConversionContext::~VideoConversionContext()
{
    free();
}


void VideoConversionContext::create(const VideoCodec& iparams, const VideoCodec& oparams)
{
//#if 0
    TraceLS(this) << "Create:"
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

    //assert(av_get_pix_fmt(oparams.pixelFmt) == );

    oframe = av_frame_alloc();
    oframe->width = oparams.width;
    oframe->height = oparams.height;
    avpicture_alloc(reinterpret_cast<AVPicture*>(oframe),
        av_get_pix_fmt(oparams.pixelFmt), oparams.width, oparams.height);
    ctx = sws_getContext(
        iparams.width, iparams.height, av_get_pix_fmt(iparams.pixelFmt),
        oparams.width, oparams.height, av_get_pix_fmt(oparams.pixelFmt),
        /* SWS_FAST_BILINEAR */SWS_BICUBIC, nullptr, nullptr, nullptr);
    if (!ctx)
        throw std::runtime_error("Invalid conversion context.");

    this->iparams = iparams;
    this->oparams = oparams;

    TraceLS(this) << "Create: OK: " << ctx << endl;
}


void VideoConversionContext::free()
{
    TraceLS(this) << "Closing" << endl;

    if (oframe) {
        av_free(oframe);
        oframe = nullptr;
    }

    if (ctx) {
        sws_freeContext(ctx);
        ctx = nullptr;
    }

    TraceLS(this) << "Closing: OK" << endl;
}


AVFrame* VideoConversionContext::convert(AVFrame* iframe)
{
    TraceLS(this) << "Convert: " << ctx << endl;

    assert(iframe);
    assert(iframe->data[0]);

    if (!ctx)
        throw std::runtime_error("Conversion context must be initialized.");

    if (sws_scale(ctx,
        iframe->data, iframe->linesize, 0, iparams.height,
        oframe->data, oframe->linesize) < 0)
        throw std::runtime_error("Pixel format conversion not supported.");

    return oframe;
}


//
// Helper functions
//


AVFrame* createVideoFrame(AVPixelFormat pixelFmt, int width, int height)
{
    AVFrame* picture = av_frame_alloc();
    if (!picture)
        return nullptr;

    int size = avpicture_get_size(pixelFmt, width, height);
    std::uint8_t* buffer = (std::uint8_t*)av_malloc(size);
    if (!buffer) {
        av_free(picture);
        return nullptr;
        }

        avpicture_fill(reinterpret_cast<AVPicture*>(picture), buffer, pixelFmt, width, height);

        return picture;

        // AVFrame* frame = av_frame_alloc();
        // if (!frame) {
        //         fprintf(stderr, "Could not allocate video frame\n");
        //         //exit(1);
        //         return nullptr;
        // }
        // frame->format = pixelFmt;
        // frame->width  = width;
        // frame->height = height;
        // // the image can be allocated by any means and av_image_alloc() is
        //     // just the most convenient way if av_malloc() is to be used
        // int ret = av_image_alloc(frame->data, frame->linesize, width, height,
        //                                          (enum AVPixelFormat)pixelFmt, 32);
        // if (ret < 0) {
        //         fprintf(stderr, "Could not allocate raw picture buffer\n");
        //         //exit(1);
        //         return nullptr;
        // }
        // return frame;
}


void initVideoEncoderContext(AVCodecContext* ctx, AVCodec* codec, VideoCodec& oparams)
{
    assert(oparams.enabled);

    avcodec_get_context_defaults3(ctx, codec);
    ctx->codec_id = codec->id;
    ctx->codec_type = AVMEDIA_TYPE_VIDEO;
    ctx->pix_fmt = av_get_pix_fmt(oparams.pixelFmt);
    ctx->frame_number = 0;

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

    // Emit one intra frame every twelve frames at most
    ctx->gop_size = 12; // oparams.fps;

    // Set some defaults for codecs of note.
    // Also set optimal output pixel formats if the
    // default AV_PIX_FMT_YUV420P was given.
    switch (ctx->codec_id) {
    case AV_CODEC_ID_H264:
        // TODO: Use oparams.quality to determine profile
        av_opt_set(ctx->priv_data, "preset", "veryfast", 0); // slow // baseline
        break;
    case AV_CODEC_ID_MJPEG:
    case AV_CODEC_ID_LJPEG:
        if (ctx->pix_fmt == AV_PIX_FMT_YUV420P)
            ctx->pix_fmt = AV_PIX_FMT_YUVJ420P;

        // Use high quality JPEG
        // TODO: Use oparams.quality to determine values
        ctx->mb_lmin        = ctx->lmin = ctx->qmin * FF_QP2LAMBDA;
        ctx->mb_lmax        = ctx->lmax = ctx->qmax * FF_QP2LAMBDA;
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
    opacket->size = avpicture_get_size(ctx->pix_fmt, ctx->width, ctx->height);
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

    /*
    TraceL << "[VideoContext] Init Decoded Frame Pcket:"
        << "\n\tFrame DTS: " << frame->pkt_dts
        << "\n\tFrame PTS: " << frame->pkt_pts
        << "\n\tPacket Size: " << opacket->size
        << "\n\tPacket DTS: " << opacket->dts
        << "\n\tPacket PTS: " << opacket->pts
        << endl;
    */
}


void initVideoCodecFromContext(const AVCodecContext* ctx, VideoCodec& params)
{
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


} } // namespace scy::av


#endif
