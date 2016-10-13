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


#include "scy/av/audiocontext.h"

#ifdef HAVE_FFMPEG

#include "scy/logger.h"
#include "scy/av/ffmpeg.h"


using std::endl;


namespace scy {
namespace av {


AudioContext::AudioContext() :
    stream(nullptr),
    codec(nullptr),
    frame(nullptr),
    pts(0)
{
    initializeFFmpeg();
}


AudioContext::~AudioContext()
{
    close();
    uninitializeFFmpeg();
}


void AudioContext::create()
{
}


void AudioContext::open()
{
    TraceS(this) << "Opening" << endl;
    assert(ctx);
    assert(codec);

    // Open the audio codec
    if (avcodec_open2(ctx, codec, nullptr) < 0)
           throw std::runtime_error("Could not open the audio codec.");
}


void AudioContext::close()
{
    if (frame) {
        av_free(frame);
        frame = nullptr;
    }

    if (ctx) {
        avcodec_close(ctx);
        ctx = nullptr;
    }

    if (stream)    {
        stream = nullptr;
        // Note: The stream is managed by the AVFormatContext
        //av_freep(stream);
    }

    pts = 0;
    //ptsSeconds = 0.0;
    error = "";
}


double AudioContext::ptsSeconds()
{
    double val = 0.0;

    // Local PTS value represented as decimal seconds
    // if (opacket->dts != AV_NOPTS_VALUE) {
    //     *pts = (double)opacket->pts;
    //     *pts *= av_q2d(stream->time_base);
    // }

    // Local PTS value represented as decimal seconds
    if (stream && pts > 0 && pts != AV_NOPTS_VALUE) {
        val = (double)pts;
        val *= av_q2d(stream->time_base);
    }

    return val;
}


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
    const enum AVSampleFormat *p = codec->sample_fmts;
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
    const enum AVSampleFormat *p = codec->sample_fmts;
    while (*p != AV_SAMPLE_FMT_NONE) {
        if (compatible == AV_SAMPLE_FMT_NONE && av_sample_fmt_is_planar(*p) == planar)
            compatible = *p;  // or use the first compatible format
        if (*p == requested)
            return requested; // always try to return requested format
        p++;
    }
    return compatible;
}


void initDecodedAudioPacket(const AVStream* stream, const AVCodecContext* ctx, const AVFrame* frame, AVPacket* opacket) //, double* pts
{
    opacket->data = frame->data[0];
    opacket->size = av_samples_get_buffer_size(nullptr, ctx->channels, frame->nb_samples, ctx->sample_fmt, 0);
    opacket->dts = frame->pkt_dts; // Decoder PTS values may be out of sequence
    opacket->pts = frame->pkt_pts;

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


} } // namespace scy::av


#endif
