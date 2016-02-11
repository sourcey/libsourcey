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


#include "scy/media/audiocontext.h"

#ifdef HAVE_FFMPEG

#include "scy/media/ffmpeg.h"
#include "scy/logger.h"


using std::endl;
using namespace scy;


namespace scy {
namespace av {


//
// Base Audio Context
//


AudioContext::AudioContext() :
    stream(nullptr),
    codec(nullptr),
    frame(nullptr),
    pts(0)
{
}


AudioContext::~AudioContext()
{
    close();
}


void AudioContext::create()
{
}


void AudioContext::open()
{
    TraceLS(this) << "Opening" << endl;
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
// Audio Encoder Context
//


AudioEncoderContext::AudioEncoderContext(AVFormatContext* format) :
    format(format),
    resampler(nullptr),
    fifo(nullptr),
    outputFrameSize(0)
{
    TraceLS(this) << "Create" << endl;
}


AudioEncoderContext::~AudioEncoderContext()
{
    TraceLS(this) << "Destroy" << endl;
    close();
}


void AudioEncoderContext::create()
{
    AudioContext::create();

    TraceLS(this) << "Create" << endl;
    int error;

    // Find the audio encoder
    if (!(codec = avcodec_find_encoder_by_name(oparams.encoder.c_str()))) {
        if (!(codec = avcodec_find_encoder(format->oformat->audio_codec)))
            throw std::runtime_error("Could not find an audio encoder for: " + oparams.encoder);
    }

    format->oformat->audio_codec = codec->id;

    // Create a new audio stream in the output file container.
    if (!(stream = avformat_new_stream(format, codec))) {
        throw std::runtime_error("Could not create the audio stream: Out of memory");
    }

    // Now we'll setup the parameters of AVCodecContext
    ctx = stream->codec;
    avcodec_get_context_defaults3(ctx, codec);
    ctx->channels = oparams.channels;
    ctx->channel_layout = av_get_default_channel_layout(oparams.channels);
    ctx->sample_rate = oparams.sampleRate;
    ctx->sample_fmt = av_get_sample_fmt(oparams.sampleFmt); // TODO: check exists in codec->sample_fmts[0];
    ctx->bit_rate = oparams.bitRate; //320000;

    // Set the sample rate for the container.
    ctx->time_base.den = oparams.sampleRate;
    ctx->time_base.num = 1;

    ctx->strict_std_compliance = FF_COMPLIANCE_EXPERIMENTAL;

    // Some container formats (like MP4) require global headers to be present
    // Mark the encoder so that it behaves accordingly.
    if (format->oformat->flags & AVFMT_GLOBALHEADER)
        ctx->flags |= CODEC_FLAG_GLOBAL_HEADER;

    // Open the encoder for the audio stream to use it later.
    if ((error = avcodec_open2(ctx, codec, nullptr)) < 0) {
        throw std::runtime_error("Could not open the audio codec: " + averror(error));
    }

    // Use the encoder's desired frame size for processing.
    outputFrameSize = ctx->frame_size;

    // Create the FIFO buffer based on the specified output sample format.
    if (!(fifo = av_audio_fifo_alloc(ctx->sample_fmt, ctx->channels, 1))) {
        throw std::runtime_error("Could not allocate FIFO: Out of memory");
    }

    // Create a resampler if required
    if (ctx->sample_fmt != av_get_sample_fmt(iparams.sampleFmt) ||
        ctx->sample_rate != iparams.sampleRate ||
        ctx->channels != iparams.channels) {
        resampler = new AudioResampler();
        resampler->create(iparams, oparams);
    }
}


void AudioEncoderContext::close()
{
    TraceLS(this) << "Closing" << endl;

    AudioContext::close();

    if (resampler) {
        delete resampler;
        resampler = nullptr;
    }

    if (fifo) {
        av_audio_fifo_free(fifo);
        fifo = nullptr;
    }
}


// Add converted input audio samples to the FIFO buffer for later processing.
static void addSamplesToFifo(AVAudioFifo *fifo,
                             const std::uint8_t **convertedSamples,
                             const int frame_size)
{
    int error;

    // Make the FIFO as large as it needs to be to hold both
    // the old and the new samples.
    if ((error = av_audio_fifo_realloc(fifo, av_audio_fifo_size(fifo) + frame_size)) < 0) {
        throw std::runtime_error("Could not reallocate FIFO: " + averror(error));
    }

    // Store the new samples in the FIFO buffer.
    if (av_audio_fifo_write(fifo, (void **)convertedSamples, frame_size) < frame_size) {
        throw std::runtime_error("Could not write data to FIFO");
    }
}

// Initialize one input frame for writing to the output file.
// The frame will be exactly frame_size samples large.
static void initOutputFrame(AVFrame **frame,
                            AVCodecContext *outputCodecContext//,
                            //int frame_size
                          )
{
    int error;

    // Create a new frame to store the audio samples.
    if (!(*frame = av_frame_alloc())) {
        throw std::runtime_error("Could not allocate output frame");
    }

    // Set the frame's parameters, especially its size and format.
    // av_frame_get_buffer needs this to allocate memory for the
    // audio samples of the frame.
    // Default channel layouts based on the number of channels
    // are assumed for simplicity.
    (*frame)->nb_samples     = outputCodecContext->frame_size;
    (*frame)->channel_layout = outputCodecContext->channel_layout;
    (*frame)->format         = outputCodecContext->sample_fmt;
    (*frame)->sample_rate    = outputCodecContext->sample_rate;

    // Allocate the samples of the created frame. This call will make
    // sure that the audio frame can hold as many samples as specified.
    if ((error = av_frame_get_buffer(*frame, 0)) < 0) {
        // fprintf(stderr, "Could allocate output frame samples (error '%s')\n",
        //         get_error_text(error));
        // av_frame_free(frame);
        // return error;
        av_frame_free(frame);
        throw std::runtime_error("Could allocate output frame samples: " + averror(error));
    }
}


static bool encodeFrame(AudioEncoderContext* enc,
                        AVFrame* oframe,
                        AVPacket& opacket,
                        const std::int64_t pts)
{
    // Set the packet data and size so that it is recognized as being empty.
    opacket.data = nullptr;
    opacket.size = 0;

    // Set a timestamp based on the sample rate for the container.
    if (oframe) {
        if (pts != AV_NOPTS_VALUE) {
            oframe->pts = pts;
        }
        else {
            oframe->pts = enc->pts;
            enc->pts += oframe->nb_samples;
        }
    }

    int frameEncoded, error;

    // Encode the audio frame and store it in the temporary packet.
    // The output audio stream encoder is used to do this.
    if ((error = avcodec_encode_audio2(enc->ctx, &opacket, oframe, &frameEncoded)) < 0) {
        // fprintf(stderr, "Could not encode frame (error '%s')\n",
        //         get_error_text(error));
        //av_packet_unref(&opacket);
        //return false;
        throw std::runtime_error("Could not encode audio frame: " + averror(error));
    }

    if (frameEncoded) {
        enc->fps.tick();
        opacket.stream_index = enc->stream->index;
        opacket.flags |= AV_PKT_FLAG_KEY;
        if (opacket.pts != AV_NOPTS_VALUE)
            opacket.pts  = av_rescale_q(opacket.pts, enc->ctx->time_base, enc->stream->time_base);
        if (opacket.dts != AV_NOPTS_VALUE)
            opacket.dts  = av_rescale_q(opacket.dts, enc->ctx->time_base, enc->stream->time_base);
        if (opacket.duration > 0)
            opacket.duration = (int)av_rescale_q(opacket.duration, enc->ctx->time_base, enc->stream->time_base);

        TraceL << "Audio frame encoded:\n"
            //<< "\n\tPTS: " << av_ts2str(opacket.pts)
            //<< "\n\tDTS: " << av_ts2str(opacket.dts)
            //<< "\n\tPTS Time: " << av_ts2timestr(opacket.pts, &stream->time_base)
            //<< "\n\tDTS Time: " << av_ts2timestr(opacket.dts, &stream->time_base)
            << "\n\tFrame PTS: " << (oframe ? oframe->pts : 0)
            << "\n\tPTS: " << opacket.pts
            << "\n\tDTS: " << opacket.dts
            << "\n\tDuration: " << opacket.duration
            //<< "\n\tFPS: " << enc->fps
            //<< "\n\tCodec Time Den: " << ctx->time_base.den
            //<< "\n\tCodec Time Num: " << ctx->time_base.num
            //<< "\n\tStream Time Den: " << stream->time_base.den
            //<< "\n\tStream Time Num: " << stream->time_base.num
            << endl;
    }
    else {
        TraceL << "No frame encoded" << endl;
    }

    return !!frameEncoded;
}


// Read a single audio frame from the FIFO and encode it.
static bool readAndEncodeFrame(AudioEncoderContext* enc,
                               AVPacket& opacket,
                               const std::int64_t pts)
{
    // Use the encoder's desired frame size for processing.
    int frameSize = enc->ctx->frame_size;

    // Make sure that there is one frame worth of samples in the FIFO
    // buffer so that the encoder can do its work.
    // Since the decoder's and the encoder's frame size may differ, we
    // need to FIFO buffer to store as many frames worth of input samples
    // that they make up at least one frame worth of output samples.
    if (av_audio_fifo_size(enc->fifo) < frameSize) {
        TraceL << "No packets in buffer" << endl;
        return false;
    }

    // Temporary storage of the output samples of the frame written to the file.
    AVFrame *oframe;

    // Initialize temporary storage for one output frame.
    initOutputFrame(&oframe, enc->ctx);

    // Read as many samples from the FIFO buffer as required to fill the frame.
    // The samples are stored in the frame temporarily.
    if (av_audio_fifo_read(enc->fifo, (void **)oframe->data, frameSize) < frameSize) {
        av_frame_free(&oframe);
        throw std::runtime_error("Could not read data from FIFO");
    }

    return encodeFrame(enc, oframe, opacket, pts);
}


bool AudioEncoderContext::encode(AVFrame* iframe, AVPacket& opacket)
{
    return encode(iframe->data[0], iframe->pts, iframe->nb_samples, opacket);
}


bool AudioEncoderContext::encode(const std::uint8_t* samples, const int frameSize, const std::int64_t pts, AVPacket& opacket)
{
    TraceLS(this) << "Encoding Audio Packet: " << frameSize << endl;

    if (resampler) {
        std::uint8_t** convertedSamples = nullptr;
        int convertedFrameSize;
        if (!resampler->resample(samples, frameSize, convertedSamples, convertedFrameSize)) {
            // The resampler may buffer frames
            TraceLS(this) << "Samples buffered by resampler" << endl;
            return false;
        }

        // Add the converted input samples to the FIFO buffer for later processing.
        addSamplesToFifo(fifo, (const std::uint8_t**)convertedSamples, convertedFrameSize);
    }
    else {
       // Add the input samples to the FIFO buffer for later processing.
       addSamplesToFifo(fifo, (const std::uint8_t**)samples, frameSize);
    }

    return readAndEncodeFrame(this, opacket, pts);

    // Encode one frame worth of audio samples.
    // if (encode_audio_frame(oframe, output_format_context,
    //                        ctx, &data_written)) {
    //     av_frame_free(&oframe);
    //     return AVERROR_EXIT;
    // }
    // av_frame_free(&oframe);

    // assert(ipacket.stream_index == stream->index);
    // //assert(ipacket.size == outputFrameSize);
    // //assert(frame);
    // //assert(buffer);
    //
    // int frameEncoded = 0;
    // std::uint8_t* samples = ipacket.data; // fifoBuffer
    //
    // if (resampler) {
    //     samples = resampler->resample(ipacket.data, ipacket.size);
    //     if (samples == nullptr) // The resampler may buffer frames
    //         return false;
    //
    //     assert(resampler->outNbSamples == outputFrameSize);
    // }
    //
    // frame = av_frame_alloc();
    // av_frame_unref(frame);
    // frame->nb_samples = inputFrameSize;
    // frame->pts = AV_NOPTS_VALUE; //ipacket.pts; //ctx->frame_number; //
    //
    // // BUG: Libspeex encoding does not increment ctx->frame_number.
    // // Might need to create local frame number value for PTS.
    // // frame->pts = ctx->frame_number; // Force a PTS value, no AV_NOPTS_VALUE
    // if (avcodec_fill_audio_frame(frame, ctx->channels, ctx->sample_fmt, samples, outputFrameSize, 0) < 0) {
    //     error = "Error filling audio frame";
    //     ErrorLS(this) << error << endl;
    //     throw std::runtime_error(error);
    // }
    //
    // av_init_packet(&opacket);
    // opacket.data = nullptr;
    // opacket.size = 0;
    // // opacket.pts = ctx->frame_number;
    //
    // if (avcodec_encode_audio2(ctx, &opacket, frame, &frameEncoded) < 0) {
    //     // TODO: Use av_strerror
    //     error = "Fatal encoder error";
    //     ErrorLS(this) << error << endl;
    //     throw std::runtime_error(error);
    // }


    // return frameEncoded > 0;
}


bool AudioEncoderContext::flush(AVPacket& opacket)
{
    return encodeFrame(this, nullptr, opacket, AV_NOPTS_VALUE);
}


//
// Audio Decoder Context
//


AudioDecoderContext::AudioDecoderContext() :
    duration(0.0)
{
}


AudioDecoderContext::~AudioDecoderContext()
{
    close();
}


void AudioDecoderContext::create(AVFormatContext *ic, int streamID)
{
    AudioContext::create();

    TraceLS(this) << "Create: " << streamID << endl;

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
        ErrorLS(this) << "" << error << endl;
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

    // while (bytesRemaining) // && !frameDecoded
    // {
    //     av_frame_unref(frame);
    //     bytesDecoded = avcodec_decode_audio4(ctx, frame, &frameDecoded, &ipacket);
    //     if (bytesDecoded < 0) {
    //         ErrorLS(this) << "Decoder Error" << endl;
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
        TraceLS(this) << "Decoded Frame:"
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
        TraceLS(this) << "Flushed Audio Frame: " << opacket.pts << endl;
        return true;
    }
    return false;
}


//
// Audio Resampler
//


AudioResampler::AudioResampler() :
    ctx(nullptr)
{
}


AudioResampler::~AudioResampler()
{
    close();
}


void AudioResampler::create(const AudioCodec& iparams, const AudioCodec& oparams)
{
    if (ctx)
        throw std::runtime_error("Resample context already initialized");

    std::int64_t outChLayout = av_get_default_channel_layout(oparams.channels);
    enum AVSampleFormat outSampleFmt = av_get_sample_fmt(oparams.sampleFmt);

    std::int64_t inChLayout  = av_get_default_channel_layout(iparams.channels);
    enum AVSampleFormat inSampleFmt  = av_get_sample_fmt(iparams.sampleFmt);

    char inChBuf[128], outChBuf[128];
    av_get_channel_layout_string(inChBuf,  sizeof(inChBuf),  -1, inChLayout);
    av_get_channel_layout_string(outChBuf, sizeof(outChBuf), -1, outChLayout);

    TraceLS(this) << "Create audio resampler:\n"
        << "\n\tIn Channel Layout: " << inChBuf
        << "\n\tIn Sample Rate: " << iparams.sampleRate
        << "\n\tIn Sample Fmt: " << av_get_sample_fmt_name(inSampleFmt)
        << "\n\tOut Channel Layout: " << outChBuf
        << "\n\tOut Sample Rate: " << oparams.sampleRate
        << "\n\tOut Sample Fmt: " << av_get_sample_fmt_name(outSampleFmt)
        << endl;

    if ((ctx = swr_alloc_set_opts(nullptr,
            outChLayout, outSampleFmt, oparams.sampleRate,
            inChLayout,  inSampleFmt,  iparams.sampleRate,
            0, nullptr)) == nullptr)
        throw std::runtime_error("Could not allocate resample context");

    // Open the resampler with the specified parameters.
    int error;
    if ((error = swr_init(ctx)) < 0) {
        fprintf(stderr, "Could not open resample context\n");
        swr_free(&ctx);
        ctx = nullptr;
        throw std::runtime_error("Could not open resample context: " + averror(error));
        //return error;
    }

    this->iparams = iparams;
    this->oparams = oparams;

    TraceLS(this) << "Create: OK" << endl;
}


void AudioResampler::close()
{
    TraceLS(this) << "Closing" << endl;

    if (ctx) {
        swr_free(&ctx);
        ctx = nullptr;
    }

    // if (outBuffer) {
    //     av_freep(&outBuffer);
    //     outBuffer = nullptr;
    // }

    TraceLS(this) << "Closing: OK" << endl;
}


bool AudioResampler::resample(const std::uint8_t* inSamples, int inNbSamples, std::uint8_t**& outSamples, int& outNbSamples)
{
    if (!ctx)
        throw std::runtime_error("Conversion context must be initialized.");

    outNbSamples = av_rescale_rnd(
        swr_get_delay(ctx, (std::int64_t)iparams.sampleRate) + (std::int64_t)inNbSamples,
        (std::int64_t)oparams.sampleRate, (std::int64_t)iparams.sampleRate, AV_ROUND_UP);

    int error;

    if (!(outSamples = (std::uint8_t**)calloc(oparams.channels, sizeof(*outSamples)))) {
        // fprintf(stderr, "Could not allocate converted input sample pointers: Out of memory\n");
        // return false; //AVERROR(ENOMEM);
        throw std::runtime_error("Could not allocate converted input sample pointers: Out of memory");
    }

    // Allocate memory for the samples of all channels in one consecutive
    // block for convenience.
    if ((error = av_samples_alloc(outSamples, nullptr,
                                  oparams.channels, outNbSamples,
                                  av_get_sample_fmt(oparams.sampleFmt), 0)) < 0) {
        av_freep(&(outSamples)[0]);
        free(outSamples);
        // fprintf(stderr,
        //         "Could not allocate converted input samples (error '%s')\n",
        //         get_error_text(error));
        //return false;
        throw std::runtime_error("Could not allocate converted input samples: " + averror(error));
    }

    // Convert the samples using the resampler.
    if ((error = swr_convert(ctx, outSamples, outNbSamples,
              (const std::uint8_t**)&inSamples, inNbSamples)) < 0) {
        //fprintf(stderr, "Could not convert input samples (error '%s')\n",
        //        get_error_text(error));
        //return false;
        throw std::runtime_error("Could not resample audio: " + averror(error));
    }

    TraceLS(this) << "Resampling audio frame:"
        << "\n\tIn Nb Smaples: " << inNbSamples
        << "\n\tIn Channels: " << iparams.channels
        << "\n\tIn Sample Rate: " << iparams.sampleRate
        << "\n\tIn Sample Fmt: " << iparams.sampleFmt //av_get_sample_fmt_name((AVSampleFormat)iparams.sampleFmt)
        << "\n\tOut Nb Samples: " << outNbSamples
        << "\n\tOut Channels: " << oparams.channels
        << "\n\tOut Sample Rate: " << oparams.sampleRate
        << "\n\tOut Sample Fmt: " << oparams.sampleFmt //av_get_sample_fmt_name((AVSampleFormat)oparams.sampleFmt)
        << endl;

    return true;
}


//
// Helper functions
//


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
