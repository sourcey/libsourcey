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
#include "scy/av/audioresampler.h"

#ifdef HAVE_FFMPEG

#include "scy/logger.h"
#include "scy/av/ffmpeg.h"
#include "scy/av/audioresampler.h"


using std::endl;


namespace scy {
namespace av {


AudioDecoder::AudioDecoder(AVStream* stream) :
    duration(0.0)
{
    this->stream = stream;
}


AudioDecoder::~AudioDecoder()
{
    close();
}


void AudioDecoder::open()
{
    int ret;

    // assert(format);
    assert(stream);

    TraceS(this) << "Create: " << stream->index << endl;

    // stream = st;
    ctx = stream->codec;

    codec = avcodec_find_decoder(ctx->codec_id);
    if (!codec)
        throw std::runtime_error("Cannot find audio decoder.");

    frame = av_frame_alloc();
    if (!frame)
        throw std::runtime_error("Cannot allocate input frame.");

    ret = avcodec_open2(ctx, codec, nullptr);
    if (ret < 0)
        throw std::runtime_error("Cannot open the audio codec: " + averror(ret));
}


void AudioDecoder::close()
{
    AudioContext::close();

    duration = 0.0;
    width = -1;
    fp = false;
}


bool initDecodedAudioPacket(AudioDecoder* dec, const AVFrame* frame, AVPacket* opacket) //const AVStream* stream, , double* pts
{
    // Recreate the resampler only if required
    // dec->recreateResampler();
    if (dec->resampler) {
        if (!dec->resampler->resample(frame->data[0], frame->nb_samples)) {
            // The resampler may buffer frames
            DebugL << "Samples buffered by resampler" << endl;
            return false;
        }

        TraceL << "Resampled audio packet: "
               << frame->nb_samples << " <=> "
               << dec->resampler->outNumSamples << endl;

        opacket->data = frame->data[0];
        opacket->size = av_samples_get_buffer_size(nullptr, dec->oparams.channels, dec->resampler->outNumSamples,
                                                   av_get_sample_fmt(dec->oparams.sampleFmt.c_str()), 0);
    }
    else {
        opacket->data = frame->data[0];
        opacket->size = av_samples_get_buffer_size(nullptr, dec->ctx->channels, frame->nb_samples, dec->ctx->sample_fmt, 0);
    }

    opacket->pts = frame->pkt_pts;
    opacket->dts = frame->pkt_dts; // Decoder PTS values may be out of sequence

    // Local PTS value represented as decimal seconds
    // if (opacket->dts != AV_NOPTS_VALUE) {
    //     *pts = (double)opacket->pts;
    //     *pts *= av_q2d(stream->time_base);
    // }

    assert(opacket->data);
    assert(opacket->size);
    // assert(opacket->dts >= 0);
    // assert(opacket->pts >= 0);

    return true;
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
        return initDecodedAudioPacket(this, frame, &opacket); //, &ptsSeconds

        // TraceS(this) << "Decoded Frame:"
        //     << "\n\tFrame Size: " << opacket.size
        //     << "\n\tFrame PTS: " << opacket.pts
        //     << "\n\tInput Frame PTS: " << ipacket.pts
        //     << "\n\tNo Frame PTS: " << (frame->pts != AV_NOPTS_VALUE)
        //     << "\n\tDecoder PTS: " << pts
        //     << endl;
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
        TraceS(this) << "Flushed audio frame: " << frame->pts << endl;
        // return true;
        return initDecodedAudioPacket(this, frame, &opacket); //, &ptsSeconds
    }
    return false;
}



} } // namespace scy::av


#endif
