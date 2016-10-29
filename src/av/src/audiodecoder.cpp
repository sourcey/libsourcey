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


AudioDecoder::AudioDecoder(AVStream* stream)
{
    this->stream = stream;
}


AudioDecoder::~AudioDecoder()
{
    close();
}


void AudioDecoder::create()
{
    assert(stream);

    TraceS(this) << "Create: " << stream->index << endl;

    ctx = stream->codec;

    codec = avcodec_find_decoder(ctx->codec_id);
    if (!codec)
        throw std::runtime_error("Cannot find audio decoder.");

    frame = av_frame_alloc();
    if (!frame)
        throw std::runtime_error("Cannot allocate input frame.");

    int ret = avcodec_open2(ctx, codec, nullptr);
    if (ret < 0)
        throw std::runtime_error("Cannot open the audio codec: " + averror(ret));

    // Set the default input and output parameters are set here once the codec
    // context has been opened. The output sample format, channels or sample
    // rate can be modified after this call and a conversion context will be
    // created on the next call to open() to output the desired format.
    initAudioCodecFromContext(ctx, iparams);
    initAudioCodecFromContext(ctx, oparams);

    // Default to s16 output (planar formats not currently supported)
    oparams.sampleFmt = "s16";
}


void AudioDecoder::close()
{
    AudioContext::close();
}


bool emitPacket(AudioDecoder* dec) //, const AVFrame* frame, AVPacket& opacket
{
    auto sampleFmt = av_get_sample_fmt(dec->oparams.sampleFmt.c_str());
    assert(av_sample_fmt_is_planar(sampleFmt) == 0 && "planar formats not supported");

    dec->time = dec->frame->pkt_pts > 0 ? static_cast<std::int64_t>(dec->frame->pkt_pts * av_q2d(dec->stream->time_base) * AV_TIME_BASE) : 0;
    dec->pts = dec->frame->pkt_pts;

    if (dec->resampler) {
        if (!dec->resampler->resample((std::uint8_t**)dec->frame->data, dec->frame->nb_samples)) {
            DebugL << "Samples buffered by resampler" << endl;
            return false;
        }

        AudioPacket audio(dec->resampler->outSamples[0],
                          dec->resampler->outBufferSize,
                          dec->resampler->outNumSamples,
                          dec->time);
        dec->outputFrameSize = dec->resampler->outNumSamples;
        dec->emitter.emit(dec, audio);
        // opacket.data = dec->resampler->outSamples[0];
        // opacket.size = dec->resampler->outBufferSize;
    }
    else {
        int size = av_samples_get_buffer_size(nullptr, dec->ctx->channels,
                                                       dec->frame->nb_samples,
                                                       dec->ctx->sample_fmt, 0);
        AudioPacket audio(dec->frame->data[0],
                          size,
                          dec->outputFrameSize,
                          dec->time);
        dec->outputFrameSize = dec->frame->nb_samples;
        dec->emitter.emit(dec, audio);
        // opacket.data = dec->frame->data[0];
        // opacket.size = av_samples_get_buffer_size(nullptr, dec->ctx->channels,
        //                                                    dec->frame->nb_samples,
        //                                                    dec->ctx->sample_fmt, 0);
    }

    // opacket.pts = dec->frame->pkt_pts; // Decoder PTS values may be out of sequence
    // opacket.dts = dec->frame->pkt_dts;

    // Compute stream time in milliseconds
    // dec->time = opacket.pts > 0 ? static_cast<std::int64_t>(opacket.pts * av_q2d(dec->stream->time_base) * 1000) : 0;
    // dec->pts = opacket.pts;

    // assert(opacket.data);
    // assert(opacket.size);
    // assert(opacket.pts >= 0);
    // assert(opacket.dts >= 0);

    return true;
}


bool AudioDecoder::decode(std::uint8_t* data, int size) //, AVPacket& opacket
{
    AVPacket ipacket;
    av_init_packet(&ipacket);
    ipacket.data = data;
    ipacket.size = size;
    if (stream)
        ipacket.stream_index = stream->index;
    return decode(ipacket); //, opacket
}


bool AudioDecoder::decode(AVPacket& ipacket) //, AVPacket& opacket
{
    assert(ctx);
    assert(codec);
    assert(frame);
    assert(!stream || ipacket.stream_index == stream->index);

    // av_init_packet(&opacket);
    // opacket.data = nullptr;
    // opacket.size = 0;

    // av_frame_unref(frame);
    // int frameDecoded = 0;
    // int bytesDecoded = 0;
    // int bytesRemaining = ipacket.size;

    int ret, frameDecoded = 0;
    while (ipacket.size > 0) {
        ret = avcodec_decode_audio4(ctx, frame, &frameDecoded, &ipacket);
        if (ret < 0) {
            error = "Audio decoder error: " + averror(ret);
            ErrorS(this) << error << endl;
            throw std::runtime_error(error);
        }

        if (frameDecoded) {
            // assert(bytesDecoded == ipacket.size);
            // assert(bytesDecoded == )

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
            // return
            emitPacket(this); //, frame, opacket, &ptsSeconds
        }

        ipacket.size -= ret;
        ipacket.data += ret;
    }
    assert(ipacket.size == 0);

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

    return !!frameDecoded;
}


void AudioDecoder::flush() //AVPacket& opacket
{
    AVPacket ipacket;
    av_init_packet(&ipacket);
    ipacket.data = nullptr;
    ipacket.size = 0;

    // av_init_packet(&opacket);
    // opacket.data = nullptr;
    // opacket.size = 0;

    int frameDecoded = 0;
    av_frame_unref(frame);
    avcodec_decode_audio4(ctx, frame, &frameDecoded, &ipacket);

    if (frameDecoded) {
        TraceS(this) << "Flushed audio frame: " << frame->pts << endl;
        assert(0);
        emitPacket(this); //, frame, opacket, &ptsSeconds
    }
}


// void AudioContext::process(IPacket& packet)
// {
//     TraceS(this) << "Process" << endl;
//
//     auto apacket = dynamic_cast<AudioPacket*>(&packet);
//     if (!apacket)
//         throw std::runtime_error("Unknown audio packet type.");
//
//     decode(apacket->data(), apacket->size());
// }


} } // namespace scy::av


#endif
