///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup av
/// @{


#include "scy/av/videodecoder.h"

#ifdef HAVE_FFMPEG

#include "scy/av/ffmpeg.h"
#include "scy/logger.h"


using std::endl;


namespace scy {
namespace av {


VideoDecoder::VideoDecoder(AVStream* stream)
{
    this->stream = stream;
}


VideoDecoder::~VideoDecoder()
{
    close();
}


void VideoDecoder::create()
{
    assert(stream);
    TraceS(this) << "Create: " << stream->index << endl;

    ctx = stream->codec;

    codec = avcodec_find_decoder(ctx->codec_id);
    if (!codec)
        throw std::runtime_error("Video codec missing or unsupported.");

    frame = av_frame_alloc();
    if (frame == nullptr)
        throw std::runtime_error("Cannot allocate video input frame.");

    int ret = avcodec_open2(ctx, codec, nullptr);
    if (ret < 0)
        throw std::runtime_error("Cannot open the audio codec: " + averror(ret));

    // Set the default input and output parameters are set here once the codec
    // context has been opened. The output pixel format, width or height can be
    // modified after this call and a conversion context will be created on the
    // next call to open() to output the desired format.
    initVideoCodecFromContext(stream, ctx, iparams);
    initVideoCodecFromContext(stream, ctx, oparams);

    // Default to bgr24 output.
    // Note: This can be removed when planar formats are fully supported via
    // the av::VideoPacket interface.
    // oparams.pixelFmt = "bgr24";
}


void VideoDecoder::open()
{
    VideoContext::open();
}


void VideoDecoder::close()
{
    VideoContext::close();
}


inline void emitPacket(VideoDecoder* dec, AVFrame* frame) //, AVPacket& opacket
{
    // Compute stream time in microseconds
    dec->time = frame->pkt_pts > 0 ? static_cast<int64_t>(frame->pkt_pts *
                                                av_q2d(dec->stream->time_base) *
                                                AV_TIME_BASE) : 0;
    dec->pts = frame->pkt_pts;

    // Handle planar video formats
    if (av_pix_fmt_count_planes(static_cast<AVPixelFormat>(frame->format)) >= 3) {
        PlanarVideoPacket video(frame->data[0], frame->data[1], frame->data[2],
                                frame->linesize[0], frame->linesize[1], frame->linesize[2], 
                                frame->width, frame->height, dec->time);
        video.source = frame;
        video.opaque = dec;

        //memcpy(frame->data[0], inputBufferY, frame->linesize[0] * frame->height);
        //memcpy(frame->data[1], inputBufferU, frame->linesize[1] * frame->height / 2);
        //memcpy(frame->data[2], inputBufferV, frame->linesize[2] * frame->height / 2);

        TraceL << "Decoded planar video frame:"
            << "\n\tFrame DTS: " << frame->pkt_dts
            << "\n\tFrame PTS: " << frame->pkt_pts
            << "\n\tPacket time: " << video.time
            << endl;

        dec->emitter.emit(video);
    }

    // Handle linear formats
    else {
        size_t size = av_image_get_buffer_size(static_cast<AVPixelFormat>(frame->format),
                                               dec->oparams.width, dec->oparams.height, 16);
        VideoPacket video(frame->data[0], size, frame->width, frame->height, dec->time);
        video.source = frame;
        video.opaque = dec;

        TraceL << "Decoded video frame:"
            << "\n\tFrame DTS: " << frame->pkt_dts
            << "\n\tFrame PTS: " << frame->pkt_pts
            << "\n\tPacket time: " << video.time
            << endl;

        dec->emitter.emit(video);
    }


    // dec->time = opacket.pts > 0 ? static_cast<int64_t>(opacket.pts *
    //     av_q2d(dec->stream->time_base) * 1000) : 0;
    // dec->pts = opacket.pts;

    // opacket.data = frame->data[0];
    // opacket.size = frame->pkt_size; //av_image_get_buffer_size(pixelFmt, dec->oparams.width, dec->oparams.height, 16);
    // opacket.pts = frame->pkt_pts; // Decoder PTS values can be unordered
    // opacket.dts = frame->pkt_dts;
    //
    // // // Local PTS value represented as decimal seconds
    // // if (opacket->dts != AV_NOPTS_VALUE) {
    // //     *pts = (double)opacket->pts;
    // //     *pts *= av_q2d(stream->time_base);
    // // }
    //
    // // Compute stream time in milliseconds
    // dec->time = opacket.pts > 0 ? static_cast<int64_t>(opacket.pts *
    // av_q2d(dec->stream->time_base) * 1000) : 0;
    // dec->pts = opacket.pts;
    //
    // assert(opacket.data);
    // assert(opacket.size);
    // // assert(opacket.dts >= 0);
    // assert(opacket.pts >= 0);
}


bool VideoDecoder::decode(uint8_t* data, int size) //, AVPacket& opacket
{
    AVPacket ipacket;
    av_init_packet(&ipacket);
    ipacket.stream_index = stream->index;
    ipacket.data = data;
    ipacket.size = size;
    return decode(ipacket); //, opacket
}


bool VideoDecoder::decode(AVPacket& ipacket) //, AVPacket& opacket
{
    assert(ctx);
    assert(codec);
    assert(frame);
    assert(!stream || ipacket.stream_index == stream->index);

    int ret, frameDecoded = 0;
    while (ipacket.size > 0) {
        ret = avcodec_decode_video2(ctx, frame, &frameDecoded, &ipacket);
        if (ret < 0) {
            error = "Audio decoder error: " + averror(ret);
            ErrorS(this) << error << endl;
            throw std::runtime_error(error);
        }

        if (frameDecoded) {
            // assert(bytesDecoded == ipacket.size);

            // TraceS(this) << "Decoded frame:"
            //     << "\n\tFrame Size: " << opacket.size
            //     << "\n\tFrame PTS: " << opacket.pts
            //     << "\n\tInput Frame PTS: " << ipacket.pts
            //     << "\n\tInput Bytes: " << ipacket.size
            //     << "\n\tBytes Decoded: " << bytesDecoded
            //     << "\n\tFrame PTS: " << frame->pts
            //     << "\n\tDecoder PTS: " << pts
            //     << endl;

            // fps.tick();
            emitPacket(this, convert(frame)); //, frame, opacket, &ptsSeconds
        }

        ipacket.size -= ret;
        ipacket.data += ret;
    }
    assert(ipacket.size == 0);

    // int frameDecoded = 0;
    // int bytesDecoded = 0;
    // int bytesRemaining = ipacket.size;

    // av_init_packet(&opacket);
    // opacket.data = nullptr;
    // opacket.size = 0;
    //
    // bytesDecoded = avcodec_decode_video2(ctx, frame, &frameDecoded,
    // &ipacket);
    // if (bytesDecoded < 0) {
    //     error = "Decoder error";
    //     ErrorS(this) << "" << error << endl;
    //     throw std::runtime_error(error);
    // }

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
    // assert(bytesDecoded == bytesRemaining);

    // while (bytesRemaining) { // && !frameDecoded
    //     //TraceS(this) << "Decoding: " << ipacket.pts << endl;
    //     bytesRemaining -= bytesDecoded;
    // }

    //     if (frameDecoded) {
    //         // fps.tick();
    //
    //         // assert(frame->pkt_pts >= 0);
    //         emitPacket(this, convert(frame), opacket); // stream, ctx, &pts,
    //         oparams
    // #if 0
    //         TraceS(this) << "Decoded Frame:"
    //             << "\n\tPTS: " << pts
    //             << "\n\tPacket Size: " << opacket.size
    //             << "\n\tPacket PTS: " << opacket.pts
    //             << "\n\tPacket DTS: " << opacket.dts
    //             << "\n\tFrame Packet PTS: " << frame->pkt_pts
    //             << "\n\tFrame Packet DTS: " << frame->pkt_dts
    //             << "\n\tFrame Size: " << ctx->frame_size
    //             << endl;
    // #endif
    //
    //         return true;
    //     }
    //     return false;
    return !!frameDecoded;
}


void VideoDecoder::flush() // AVPacket& opacket
{
    AVPacket ipacket;
    av_init_packet(&ipacket);
    ipacket.data = nullptr;
    ipacket.size = 0;

    // av_init_packet(&opacket);
    // opacket.data = nullptr;
    // opacket.size = 0;

    int frameDecoded = 0;
    do {
        avcodec_decode_video2(ctx, frame, &frameDecoded, &ipacket);
        if (frameDecoded) {
            TraceS(this) << "Flushed video frame" << endl;
            emitPacket(this, convert(frame)); //, opacket stream, ctx, &pts, oparams
            // return true;
        }
    } while (frameDecoded);
}


} // namespace av
} // namespace scy


#endif


/// @\}
