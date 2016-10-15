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


AudioDecoder::AudioDecoder() :
    duration(0.0)
{
}


AudioDecoder::~AudioDecoder()
{
    close();
}


void AudioDecoder::create(AVFormatContext* format, AVStream* stream)
{
    AudioContext::create();

    assert(format);
    assert(stream);

    TraceS(this) << "Create: " << stream->index << endl;

    this->stream = stream;
    this->ctx = stream->codec;

    codec = avcodec_find_decoder(this->ctx->codec_id);
    if (!codec)
        throw std::runtime_error("Could not find audio decoder.");

    if (avcodec_open2(ctx, codec, nullptr) < 0)
        throw std::runtime_error("Could not open the audio codec.");

    frame = av_frame_alloc();
    if (!frame)
        throw std::runtime_error("Could not allocate input frame.");
}


void AudioDecoder::close()
{
    AudioContext::close();

    duration = 0.0;
    width = -1;
    fp = false;
}


void initDecodedAudioPacket(const AVStream* stream, const AVCodecContext* ctx, const AVFrame* frame, AVPacket* opacket) //, double* pts
{
    opacket->data = frame->data[0];
    opacket->size = av_samples_get_buffer_size(nullptr, ctx->channels, frame->nb_samples, ctx->sample_fmt, 0);
    opacket->pts = frame->pkt_pts;
    opacket->dts = frame->pkt_dts; // Decoder PTS values may be out of sequence

    // Local PTS value represented as decimal seconds
    // if (opacket->dts != AV_NOPTS_VALUE) {
    //     *pts = (double)opacket->pts;
    //     *pts *= av_q2d(stream->time_base);
    // }

    assert(opacket->data);
    assert(opacket->size);
    assert(opacket->dts >= 0);
    assert(opacket->pts >= 0);
}


bool AudioDecoder::decode(std::uint8_t* data, int size, AVPacket& opacket)
{
    AVPacket ipacket;
    av_init_packet(&ipacket);
    ipacket.stream_index = stream->index;
    ipacket.data = data;
    ipacket.size = size;
    return decode(ipacket, opacket);
}


bool AudioDecoder::decode(AVPacket& ipacket, AVPacket& opacket)
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


bool AudioDecoder::flush(AVPacket& opacket)
{
    AVPacket ipacket;
    av_init_packet(&ipacket);
    ipacket.data = nullptr;
    ipacket.size = 0;

    av_init_packet(&opacket);
    opacket.data = nullptr;
    opacket.size = 0;

    int frameDecoded = 0;
    av_frame_unref(frame);
    avcodec_decode_audio4(ctx, frame, &frameDecoded, &ipacket);

    if (frameDecoded) {
        initDecodedAudioPacket(stream, ctx, frame, &opacket); //, &ptsSeconds
        TraceS(this) << "Flushed audio frame: " << opacket.pts << endl;
        return true;
    }
    return false;
}



} } // namespace scy::av


#endif
