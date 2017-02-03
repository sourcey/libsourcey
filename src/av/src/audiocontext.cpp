///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:    LGPL-2.1+
//
/// @addtogroup av
/// @{


#include "scy/av/audiocontext.h"
#include "scy/av/audioresampler.h"


#ifdef HAVE_FFMPEG

#include "scy/av/ffmpeg.h"
#include "scy/logger.h"


using std::endl;


namespace scy {
namespace av {


AudioContext::AudioContext()
    : // PacketProcessor(this->emitter),
    stream(nullptr)
    , codec(nullptr)
    , frame(nullptr)
    , resampler(nullptr)
    , outputFrameSize(0)
    , time(0)
    , pts(AV_NOPTS_VALUE)
{
    initializeFFmpeg();
}


AudioContext::~AudioContext()
{
    close();
    uninitializeFFmpeg();
}


void AudioContext::open()
{
    TraceS(this) << "Open: "
                 << "\n\tInput: " << iparams.toString()
                 << "\n\tOutput: " << oparams.toString() << endl;

    assert(ctx);
    assert(avcodec_is_open(ctx) && "avcodec_open2 must be called");
    assert(codec);

    // // Open the audio codec
    // if (avcodec_open2(ctx, codec, nullptr) < 0)
    //      throw std::runtime_error("Cannot open the audio codec.");

    // Create the resampler if resampling is required
    if (iparams.channels != oparams.channels ||
        iparams.sampleRate != oparams.sampleRate ||
        iparams.sampleFmt != oparams.sampleFmt) {
        recreateResampler();
    }
}


void AudioContext::close()
{
    if (frame) {
        // av_free(frame);
        av_frame_free(&frame);
        frame = nullptr;
    }

    if (ctx) {
        avcodec_close(ctx);
        ctx = nullptr;
    }

    if (stream) {
        // The stream pointer is managed by the AVFormatContext
        stream = nullptr;
    }

    if (resampler) {
        delete resampler;
        resampler = nullptr;
    }

    time = 0;
    pts = 0;
    // error = "";
}


// double AudioContext::ptsSeconds()
// {
//     double val = 0.0;
//
//     // Local PTS value represented as decimal seconds
//     // if (opacket->dts != AV_NOPTS_VALUE) {
//     //     *pts = (double)opacket->pts;
//     //     *pts *= av_q2d(stream->time_base);
//     // }
//
//     // Local PTS value represented as decimal seconds
//     if (stream && pts > 0 && pts != AV_NOPTS_VALUE) {
//         val = (double)pts;
//         val *= av_q2d(stream->time_base);
//     }
//
//     return val;
// }


bool AudioContext::recreateResampler()
{
    // if (resampler)
    //     throw std::runtime_error("Conversion context already exists.");

    // NOTE: the input output `channels`, `sampleRate`, and `sampleFmt`
    // parameters work
    // slightly differently for encoders and decoders.
    // For encoders `iparams` is the picture format from the application and
    // `oparams` is the picture format passed into the encoder.
    // For decoders `iparams` is the picture format from the decoder and
    // `oparams` is the picture format passed into the application.

    // // Check if resampler is required
    // if (iparams.channels == oparams.channels &&
    //     iparams.sampleRate == oparams.sampleRate &&
    //     iparams.sampleFmt == oparams.sampleFmt) {
    //     return false;
    // }
    //
    // // Check if the resampler context needs to be recreated
    // if (resampler && (
    //     resampler->iparams.channels == iparams.channels &&
    //     resampler->iparams.sampleRate == iparams.sampleRate &&
    //     resampler->iparams.sampleFmt == iparams.sampleFmt) && (
    //     resampler->oparams.channels == oparams.channels &&
    //     resampler->oparams.sampleRate == oparams.sampleRate &&
    //     resampler->oparams.sampleFmt == oparams.sampleFmt)) {
    //     return false;
    // }

    // Recreate the resampler context
    DebugL << "Recreating audio resampler context" << endl;
    if (resampler)
        delete resampler;
    resampler = new AudioResampler();
    resampler->iparams = iparams;
    resampler->oparams = oparams;
    resampler->open();
    return true;
}


// bool AudioContext::accepts(IPacket& packet)
// {
//     return dynamic_cast<AudioPacket*>(&packet) != 0;
// }


//
// Helper functions
//


void initAudioCodecFromContext(const AVCodecContext* ctx, AudioCodec& params)
{
    params.enabled = true;
    params.encoder = avcodec_get_name(ctx->codec_id);
    params.sampleFmt = av_get_sample_fmt_name(ctx->sample_fmt);
    params.channels = ctx->channels;
    params.sampleRate = ctx->sample_rate;
    params.bitRate = ctx->bit_rate;
}


bool isSampleFormatSupported(AVCodec* codec, enum AVSampleFormat sampleFormat)
{
    const enum AVSampleFormat* p = codec->sample_fmts;
    while (*p != AV_SAMPLE_FMT_NONE) {
        if (*p == sampleFormat)
            return true;
        p++;
    }
    return false;
}


AVSampleFormat selectSampleFormat(AVCodec* codec, av::AudioCodec& params)
{
    enum AVSampleFormat compatible = AV_SAMPLE_FMT_NONE;
    enum AVSampleFormat requested = av_get_sample_fmt(params.sampleFmt.c_str());
    bool planar = av_sample_fmt_is_planar(requested);
    const enum AVSampleFormat* p = codec->sample_fmts;
    while (*p != AV_SAMPLE_FMT_NONE) {
        if (compatible == AV_SAMPLE_FMT_NONE &&
            av_sample_fmt_is_planar(*p) == planar)
            compatible = *p; // or use the first compatible format
        if (*p == requested)
            return requested; // always try to return requested format
        p++;
    }
    return compatible;
}


} // namespace av
} // namespace scy


#endif


/// @\}
