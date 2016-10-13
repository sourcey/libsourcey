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


VideoDecoderContext::VideoDecoderContext()
{
}


VideoDecoderContext::~VideoDecoderContext()
{
    close();
}


void VideoDecoderContext::create(AVFormatContext *ic, int streamID)
{
    TraceS(this) << "Create: " << streamID << endl;
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

    /*
    while (bytesRemaining) { // && !frameDecoded
        //TraceS(this) << "Decoding: " << ipacket.pts << endl;
        bytesRemaining -= bytesDecoded;
    }
    */

    if (frameDecoded) {
        fps.tick();
        initDecodedVideoPacket(stream, ctx, frame, &opacket, &pts);
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
        TraceS(this) << "Flushed video frame: " << opacket.pts << endl;
        return true;
    }
    return false;
}


} } // namespace scy::av


#endif
