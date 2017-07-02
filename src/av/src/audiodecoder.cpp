///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup av
/// @{


#include "scy/av/audiodecoder.h"
#include "scy/av/audioresampler.h"

#ifdef HAVE_FFMPEG

#include "scy/av/audioresampler.h"
#include "scy/av/ffmpeg.h"
#include "scy/logger.h"


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
    LTrace("Create: ", stream->index)

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

    // Default to s16 interleaved output.
    // oparams.sampleFmt = "s16";
}


void AudioDecoder::close()
{
    AudioContext::close();
}


bool emitPacket(AudioDecoder* dec)
{
    //auto sampleFmt = av_get_sample_fmt(dec->oparams.sampleFmt.c_str());
    //assert(av_sample_fmt_is_planar(sampleFmt) == 0 && "planar formats not supported");

    // Set the decoder time in microseconds
    // This value represents the number of microseconds
    // that have elapsed since the brginning of the stream.
    dec->time = dec->frame->pkt_pts > 0 ? static_cast<int64_t>(dec->frame->pkt_pts *
                av_q2d(dec->stream->time_base) * AV_TIME_BASE) : 0;

    // Set the decoder pts in stream time base
    dec->pts = dec->frame->pkt_pts;

    // Set the decoder seconds since stream start
    // http://stackoverflow.com/questions/6731706/syncing-decoded-video-using-ffmpeg
    dec->seconds = (dec->frame->pkt_dts - dec->stream->start_time) * av_q2d(dec->stream->time_base);

    if (dec->resampler) {
        if (!dec->resampler->resample((uint8_t**)dec->frame->extended_data, dec->frame->nb_samples)) {
            LDebug("Samples buffered by resampler")
            return false;
        }

        // AudioPacket audio(dec->resampler->outSamples[0],
        //                  dec->resampler->outBufferSize,
        //                  dec->resampler->outNumSamples, dec->time);
        // dec->outputFrameSize = dec->resampler->outNumSamples;
        // dec->emitter.emit(audio);
        // opacket.data = dec->resampler->outSamples[0];
        // opacket.size = dec->resampler->outBufferSize;

        PlanarAudioPacket audio(dec->resampler->outSamples,
            dec->oparams.channels, dec->resampler->outNumSamples,
            dec->oparams.sampleFmt, dec->time);
        dec->outputFrameSize = dec->resampler->outNumSamples;
        dec->emitter.emit(audio);
        assert(audio.size() == dec->resampler->outBufferSize);
    }
    else {
        // int size = av_samples_get_buffer_size(nullptr, dec->ctx->channels,
        //                                      dec->frame->nb_samples,
        //                                      dec->ctx->sample_fmt, 0);
        // AudioPacket audio(dec->frame->data[0], size, dec->outputFrameSize, dec->time);
        // dec->outputFrameSize = dec->frame->nb_samples;
        // dec->emitter.emit(audio);

        PlanarAudioPacket audio(dec->frame->data,
            dec->oparams.channels, dec->frame->nb_samples,
            dec->oparams.sampleFmt, dec->time);
        dec->outputFrameSize = dec->frame->nb_samples;
        dec->emitter.emit(audio);
    }

    // opacket.pts = dec->frame->pkt_pts; // Decoder PTS values may be out of sequence
    // opacket.dts = dec->frame->pkt_dts;

    // assert(opacket.data);
    // assert(opacket.size);
    // assert(opacket.pts >= 0);
    // assert(opacket.dts >= 0);

    return true;
}


// bool AudioDecoder::decode(uint8_t* data, int size)
// {
//    AVPacket ipacket;
//    av_init_packet(&ipacket);
//    ipacket.data = data;
//    ipacket.size = size;
//    if (stream)
//        ipacket.stream_index = stream->index;
//    return decode(ipacket);
// }


bool AudioDecoder::decode(AVPacket& ipacket)
{
    assert(ctx);
    assert(codec);
    assert(frame);
    assert(!stream || ipacket.stream_index == stream->index);

    // Convert input packet to codec time base
    // ipacket.dts = av_rescale_q_rnd(ipacket.dts,
    //    stream->time_base,
    //    stream->codec->time_base,
    //    AVRounding(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
    // ipacket.pts = av_rescale_q_rnd(ipacket.pts,
    //    stream->time_base,
    //    stream->codec->time_base,
    //    AVRounding(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));

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
            LError(error)
            throw std::runtime_error(error);
        }

        if (frameDecoded) {
            // assert(bytesDecoded == ipacket.size);

            // STrace << "Decoded frame:"
            //     << "\n\tFrame Size: " << opacket.size
            //     << "\n\tFrame PTS: " << opacket.pts
            //     << "\n\tInput Frame PTS: " << ipacket.pts
            //     << "\n\tInput Bytes: " << ipacket.size
            //     << "\n\tBytes Decoded: " << bytesDecoded
            //     << "\n\tFrame PTS: " << frame->pts
            //     << "\n\tDecoder PTS: " << pts
            //     << endl;

            // fps.tick();
            emitPacket(this);
        }

        ipacket.size -= ret;
        ipacket.data += ret;
    }
    assert(ipacket.size == 0);
    return !!frameDecoded;
}


void AudioDecoder::flush()
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
        LTrace("Flushed audio frame: ", frame->pts)
        assert(0);
        emitPacket(this);
    }
}


// void AudioContext::process(IPacket& packet)
// {
//     LTrace("Process")
//
//     auto apacket = dynamic_cast<AudioPacket*>(&packet);
//     if (!apacket)
//         throw std::runtime_error("Unknown audio packet type.");
//
//     decode(apacket->data(), apacket->size());
// }


} // namespace av
} // namespace scy


#endif


/// @\}
