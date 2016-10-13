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


#include "scy/av/audioencoder.h"

#ifdef HAVE_FFMPEG

#include "scy/logger.h"
#include "scy/av/ffmpeg.h"
#include "scy/av/audioresampler.h"


using std::endl;


namespace scy {
namespace av {


//
// Audio Encoder Context
//


AudioEncoderContext::AudioEncoderContext(AVFormatContext* format) :
    format(format),
    resampler(nullptr),
    fifo(nullptr),
    outputFrameSize(0)
{
    TraceS(this) << "Create" << endl;
}


AudioEncoderContext::~AudioEncoderContext()
{
    TraceS(this) << "Destroy" << endl;
    close();
}


void AudioEncoderContext::create()
{
    AudioContext::create();

    TraceS(this) << "Create" << endl;
    int error;

    // Find the audio encoder
    if (!(codec = avcodec_find_encoder_by_name(oparams.encoder.c_str()))) {
        if (!format || !(codec = avcodec_find_encoder(format->oformat->audio_codec)))
            throw std::runtime_error("Could not find an audio encoder for: " + oparams.encoder);
    }

    // Allocate stream and AVCodecContext from the AVFormatContext if available
    if (format) {
        format->oformat->audio_codec = codec->id;

        // Create a new audio stream in the output file container.
        if (!(stream = avformat_new_stream(format, codec))) {
            throw std::runtime_error("Could not create the audio stream: Out of memory");
        }

        ctx = stream->codec;
    }

    // Otherwise allocate the standalone AVCodecContext
    else {
        ctx = avcodec_alloc_context3(codec);
        if (!ctx)
            throw std::runtime_error("Cannot allocate encoder context.");
    }

    // Now we'll setup the parameters of AVCodecContext
    avcodec_get_context_defaults3(ctx, codec);
    ctx->bit_rate = oparams.bitRate; //320000;
    ctx->sample_fmt = selectSampleFormat(codec, oparams);
    ctx->sample_rate = oparams.sampleRate;
    ctx->channels = oparams.channels;
    ctx->channel_layout = av_get_default_channel_layout(oparams.channels);

    // // Set the sample rate for the container.
    // ctx->time_base.den = oparams.sampleRate;
    // ctx->time_base.num = 1;

    ctx->strict_std_compliance = FF_COMPLIANCE_EXPERIMENTAL;

    // Some container formats (like MP4) require global headers to be present
    // Mark the encoder so that it behaves accordingly.
    if (format && format->oformat->flags & AVFMT_GLOBALHEADER)
        ctx->flags |= CODEC_FLAG_GLOBAL_HEADER;

    // Open the encoder for the audio stream to use it later.
    if ((error = avcodec_open2(ctx, codec, nullptr)) < 0) {
        throw std::runtime_error("Could not open the audio codec: " + averror(error));
    }

    // Use the encoder's desired frame size for processing.
    outputFrameSize = ctx->frame_size;

    // // Allocate frame containing input raw audio
    // frame = av_frame_alloc();
    // if (!frame) {
    //     throw std::runtime_error("Could not allocate audio frame: Out of memory");
    // }
    // frame->nb_samples     = ctx->frame_size;
    // frame->format         = ctx->sample_fmt;
    // frame->channel_layout = ctx->channel_layout;

    // Create the FIFO buffer based on the specified output sample format.
    // NOTE: We read from a FIFO buffer as many codecs require a FULL
    // `ctx->frame_size` samples for encoding.
    if (!(fifo = av_audio_fifo_alloc(ctx->sample_fmt, ctx->channels, 1))) {
        throw std::runtime_error("Could not allocate FIFO: Out of memory");
    }

    // Update parameters that may have changed
    oparams.sampleFmt = av_get_sample_fmt_name(ctx->sample_fmt);

    // Create a resampler if required
    if (iparams.sampleFmt != oparams.sampleFmt ||
        iparams.sampleRate != oparams.sampleRate ||
        iparams.channels != oparams.channels) {
        resampler = new AudioResampler(iparams, oparams);
        resampler->create();
    }
}


void AudioEncoderContext::close()
{
    TraceS(this) << "Closing" << endl;

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
static void addSamplesToFifo(AVAudioFifo* fifo,
                             const std::uint8_t** convertedSamples,
                             const int convertedFrameSize)
{
    int error;
    assert(fifo);
    assert(convertedSamples);
    assert(convertedSamples[0]);

    // Make the FIFO as large as it needs to be to hold both
    // the old and the new samples.
    if ((error = av_audio_fifo_realloc(fifo, av_audio_fifo_size(fifo) + convertedFrameSize)) < 0) {
        throw std::runtime_error("Could not reallocate FIFO: " + averror(error));
    }

    // Store the new samples in the FIFO buffer.
    if (av_audio_fifo_write(fifo, (void**)convertedSamples, convertedFrameSize) < convertedFrameSize) {
        throw std::runtime_error("Could not write data to FIFO");
    }
}


// Initialize one input frame for writing to the output file.
// The frame will be exactly frame_size samples large.
static AVFrame* initOutputFrame(AVCodecContext *codecContext)
{
    AVFrame *frame;
    int error;

    // Create a new frame to store the audio samples.
    if (!(frame = av_frame_alloc())) {
        throw std::runtime_error("Could not allocate output frame");
    }

    // Set the frame's parameters, especially its size and format.
    // av_frame_get_buffer needs this to allocate memory for the
    // audio samples of the frame.
    // Default channel layouts based on the number of channels
    // are assumed for simplicity.
    frame->nb_samples     = codecContext->frame_size;
    frame->channel_layout = codecContext->channel_layout;
    frame->format         = codecContext->sample_fmt;
    frame->sample_rate    = codecContext->sample_rate;

    // Allocate the samples of the created frame. This call will make
    // sure that the audio frame can hold as many samples as specified.
    if ((error = av_frame_get_buffer(frame, 0)) < 0) {
        av_frame_free(&frame);
        throw std::runtime_error("Could allocate output frame samples: " + averror(error));
    }

    return frame;
}


// Read a single audio frame from the FIFO and encode it.
static bool readAndEncodeFrame(AudioEncoderContext* enc,
                               AVPacket& opacket,
                               const std::int64_t pts)
{
    // Use the encoder's desired frame size for processing.
    int frameSize = enc->ctx->frame_size;
    TraceL << "readAndEncodeFrame: " << frameSize << endl;

    // Make sure that there is one frame worth of samples in the FIFO
    // buffer so that the encoder can do its work.
    // Since the decoder's and the encoder's frame size may differ, we
    // need to FIFO buffer to store as many frames worth of input samples
    // that they make up at least one frame worth of output samples.
    if (av_audio_fifo_size(enc->fifo) < frameSize) {
        TraceL << "No packets in buffer" << endl;
        return false;
    }

    // // Initialize temporary storage for one output frame.
    auto iframe = initOutputFrame(enc->ctx);

    // Read as many samples from the FIFO buffer as required to fill the frame.
    // The samples are stored in the frame temporarily.
    if (av_audio_fifo_read(enc->fifo, (void**)iframe->data, frameSize) < frameSize) {
        av_frame_free(&iframe);
        throw std::runtime_error("Could not read data from FIFO");
    }

    // Set a timestamp based on the sample rate for the container.
    if (pts != AV_NOPTS_VALUE) {
        iframe->pts = pts;
    }
    // else {
    //     iframe->pts = enc->pts;
    //     enc->pts += iframe->nb_samples;
    // }

    bool result = enc->encode(iframe, opacket);
    // bool result = encodeFrame(enc, iframe, opacket, pts);
    // av_frame_free(&iframe);
    return result;
}


bool AudioEncoderContext::encode(const std::uint8_t* samples, const int frameSize, const std::int64_t pts, AVPacket& opacket)
{
    TraceS(this) << "Encoding audio packet: " << frameSize << endl;

    if (resampler) {
        if (!resampler->resample(samples, frameSize)) {
            // The resampler may buffer frames
            TraceS(this) << "Samples buffered by resampler" << endl;
            return false;
        }

        TraceS(this) << "Resampled audio packet: " << frameSize << " <=> " << resampler->outNbSamples << endl;

        // Add the converted input samples to the FIFO buffer for later processing.
        addSamplesToFifo(fifo, (const std::uint8_t**)resampler->outSamples, resampler->outNbSamples);
    }
    else {
        // Add the input samples to the FIFO buffer for later processing.
        addSamplesToFifo(fifo, (const std::uint8_t**)&samples, frameSize);
    }

    return readAndEncodeFrame(this, opacket, pts);
}


bool AudioEncoderContext::encode(AVFrame* iframe, AVPacket& opacket)
{
    TraceS(this) << "Encoding audio frame" << endl;

    int frameEncoded, error;

    // Set the packet data and size so that it is recognized as being empty.
    av_init_packet(&opacket);
    opacket.data = nullptr;
    opacket.size = 0;

    // Encode the audio frame and store it in the temporary packet.
    // The output audio stream encoder is used to do this.
    if ((error = avcodec_encode_audio2(ctx, &opacket, iframe, &frameEncoded)) < 0) {
        throw std::runtime_error("Could not encode audio frame: " + averror(error));
    }

    if (frameEncoded) {
        fps.tick();
        opacket.flags |= AV_PKT_FLAG_KEY;
        if (stream) {
            opacket.stream_index = stream->index;
        //     if (opacket.pts != AV_NOPTS_VALUE)
        //         opacket.pts  = av_rescale_q(opacket.pts, ctx->time_base, stream->time_base);
        //     if (opacket.dts != AV_NOPTS_VALUE)
        //         opacket.dts  = av_rescale_q(opacket.dts, ctx->time_base, stream->time_base);
        //     if (opacket.duration > 0)
        //         opacket.duration = (int)av_rescale_q(opacket.duration, ctx->time_base, stream->time_base);
        }
        TraceL << "Audio frame encoded:\n"
            << "\n\tFrame PTS: " << (iframe ? iframe->pts : 0)
            << "\n\tPTS: " << opacket.pts
            << "\n\tDTS: " << opacket.dts
            << "\n\tDuration: " << opacket.duration
            << endl;
    }
    else {
        TraceL << "No frame encoded" << endl;
    }

    return frameEncoded > 0;
}


bool AudioEncoderContext::flush(AVPacket& opacket)
{
    return encode(nullptr, opacket);
}


} } // namespace scy::av


#endif
