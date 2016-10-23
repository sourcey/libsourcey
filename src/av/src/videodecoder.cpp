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


#include "scy/av/videodecoder.h"

#ifdef HAVE_FFMPEG

#include "scy/logger.h"
#include "scy/av/ffmpeg.h"


using std::endl;


namespace scy {
namespace av {


VideoDecoder::VideoDecoder()
{
}


VideoDecoder::~VideoDecoder()
{
    close();
}


void VideoDecoder::create(AVFormatContext* format, AVStream* st)
{
    assert(format);
    assert(st);

    TraceS(this) << "Create: " << st->index << endl;
    VideoContext::create();

    stream = st;
    ctx = stream->codec;

    codec = avcodec_find_decoder(ctx->codec_id);
    if (!codec)
        throw std::runtime_error("Video codec missing or unsupported.");

    if (avcodec_open2(this->ctx, codec, nullptr) < 0)
        throw std::runtime_error("Cannot open the audio codec.");

    frame = av_frame_alloc();
    if (frame == nullptr)
        throw std::runtime_error("Cannot allocate video input frame.");

    // NOTE: The input and output format is set here once codec defaults have
    // set. If the output parameters pixel format, width or height is modified
    // then a conversion context will be created to output the desired format
    // on the next call to decode().
    initVideoCodecFromContext(ctx, iparams);
    initVideoCodecFromContext(ctx, oparams);
}


void VideoDecoder::close()
{
    VideoContext::close();
}


void initDecodedVideoPacket(const AVFrame* frame, const VideoCodec& oparams, AVPacket& opacket) //const AVStream* stream, const AVCodecContext* ctx, double* pts
{
    opacket.data = frame->data[0];
    // opacket->size = avpicture_get_size(ctx->pix_fmt, ctx->width, ctx->height);
    opacket.size = av_image_get_buffer_size(av_get_pix_fmt(oparams.pixelFmt.c_str()), oparams.width, oparams.height, 16);
    opacket.dts = frame->pkt_dts; // Decoder PTS values can be unordered
    opacket.pts = frame->pkt_pts;

    // ctx->pix_fmt
    // // Local PTS value represented as decimal seconds
    // if (opacket->dts != AV_NOPTS_VALUE) {
    //     *pts = (double)opacket->pts;
    //     *pts *= av_q2d(stream->time_base);
    // }

    assert(opacket.data);
    assert(opacket.size);
    // assert(opacket.dts >= 0);
    // assert(opacket.pts >= 0);

    // TraceL << "[VideoContext] Init Decoded Frame Pcket:"
    //     << "\n\tFrame DTS: " << frame->pkt_dts
    //     << "\n\tFrame PTS: " << frame->pkt_pts
    //     << "\n\tPacket Size: " << opacket->size
    //     << "\n\tPacket DTS: " << opacket->dts
    //     << "\n\tPacket PTS: " << opacket->pts
    //     << endl;
}


bool VideoDecoder::decode(std::uint8_t* data, int size, AVPacket& opacket)
{
    AVPacket ipacket;
    av_init_packet(&ipacket);
    ipacket.stream_index = stream->index;
    ipacket.data = data;
    ipacket.size = size;
    return decode(ipacket, opacket);
}


bool VideoDecoder::decode(AVPacket& ipacket, AVPacket& opacket)
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
        ErrorS(this) << "" << error << endl;
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

    // while (bytesRemaining) { // && !frameDecoded
    //     //TraceS(this) << "Decoding: " << ipacket.pts << endl;
    //     bytesRemaining -= bytesDecoded;
    // }

    if (frameDecoded) {
        fps.tick();
        initDecodedVideoPacket(convert(frame), oparams, opacket); // stream, ctx, &pts
#if 0
        TraceS(this) << "Decoded Frame:"
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


bool VideoDecoder::flush(AVPacket& opacket)
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
        initDecodedVideoPacket(convert(frame), oparams, opacket); // stream, ctx, &pts
        TraceS(this) << "Flushed video frame: " << opacket.pts << endl;
        return true;
    }
    return false;
}


} } // namespace scy::av


#endif
