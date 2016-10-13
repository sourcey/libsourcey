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


#include "scy/av/audiodecoder.h"

#ifdef HAVE_FFMPEG

#include "scy/logger.h"
#include "scy/av/ffmpeg.h"
#include "scy/av/audioresampler.h"


using std::endl;


namespace scy {
namespace av {


//
// Audio Decoder Decoder
//


AudioDecoderContext::AudioDecoderContext() :
    duration(0.0)
{
}


AudioDecoderContext::~AudioDecoderContext()
{
    close();
}


void AudioDecoderContext::create(AVFormatContext* ic, int streamID)
{
    AudioContext::create();

    TraceS(this) << "Create: " << streamID << endl;

    assert(ic);
    assert(streamID >= 0);

    stream = ic->streams[streamID];
    ctx = stream->codec;

    codec = avcodec_find_decoder(ctx->codec_id);
    if (!codec)
        throw std::runtime_error("Could not find audio decoder.");

    if (avcodec_open2(ctx, codec, NULL) < 0)
        throw std::runtime_error("Could not open the audio codec.");

    frame = av_frame_alloc();
    if (!frame)
        throw std::runtime_error("Could not allocate input frame.");
}


void AudioDecoderContext::close()
{
    AudioContext::close();

    duration = 0.0;
    width = -1;
    fp = false;
}


bool AudioDecoderContext::decode(std::uint8_t* data, int size, AVPacket& opacket)
{
    AVPacket ipacket;
    av_init_packet(&ipacket);
    ipacket.stream_index = stream->index;
    ipacket.data = data;
    ipacket.size = size;
    return decode(ipacket, opacket);
}


bool AudioDecoderContext::decode(AVPacket& ipacket, AVPacket& opacket)
{
    assert(ipacket.stream_index == stream->index);
    assert(codec);
    assert(frame);

    int frameDecoded = 0;
    int bytesDecoded = 0;
    // int bytesRemaining = ipacket.size;

    av_init_packet(&opacket);
    opacket.data = nullptr;
    opacket.size = 0;

    av_frame_unref(frame);
    bytesDecoded = avcodec_decode_audio4(ctx, frame, &frameDecoded, &ipacket);
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
    // assert(bytesDecoded == bytesRemaining);

    // while (bytesRemaining) // && !frameDecoded
    // {
    //     av_frame_unref(frame);
    //     bytesDecoded = avcodec_decode_audio4(ctx, frame, &frameDecoded, &ipacket);
    //     if (bytesDecoded < 0) {
    //         ErrorS(this) << "Decoder Error" << endl;
    //         error = "Decoder error";
    //         throw std::runtime_error(error);
    //     }
    //
    //     bytesRemaining -= bytesDecoded;
    // }

    if (frameDecoded) {
        fps.tick();
        initDecodedAudioPacket(stream, ctx, frame, &opacket); //, &ptsSeconds

#if 0
        TraceS(this) << "Decoded Frame:"
            << "\n\tFrame Size: " << opacket.size
            << "\n\tFrame PTS: " << opacket.pts
            << "\n\tInput Frame PTS: " << ipacket.pts
            << "\n\tNo Frame PTS: " << (frame->pts != AV_NOPTS_VALUE)
            << "\n\tDecoder PTS: " << pts
            << endl;
#endif

        return true;
    }

    return false;
}


bool AudioDecoderContext::flush(AVPacket& opacket)
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
        initDecodedAudioPacket(stream, ctx, frame, &opacket); //, &ptsSeconds
        TraceS(this) << "Flushed Audio Frame: " << opacket.pts << endl;
        return true;
    }
    return false;
}


//
// Helper functions
//



} } // namespace scy::av


#endif
