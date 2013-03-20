//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is is distributed under a dual license that allows free, 
// open source use and closed source use under a standard commercial
// license.
//
// Non-Commercial Use:
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// Commercial Use:
// Please contact mail@sourcey.com
//


#include "Sourcey/Media/AudioContext.h"
#include "Sourcey/Logger.h"


using namespace std;
using namespace Poco;
using namespace Sourcey;

namespace Sourcey {
namespace Media {

	
// ---------------------------------------------------------------------
//
// Base Audio Context
//
// ---------------------------------------------------------------------
AudioContext::AudioContext() :
	stream(NULL),
	codec(NULL),
	frame(NULL),
	pts(0.0)
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
	LogTrace() << "[AudioContext:" << this << "] Opening" << endl;	
	assert(ctx);
	assert(codec);

	// Open the audio codec
	if (avcodec_open2(ctx, codec, NULL) < 0)
   		throw Exception("Cannot open the audio codec.");
}
	

void AudioContext::close()
{
	if (frame) {
		av_free(frame);
		frame = NULL;
	}
	
	if (ctx) {
		avcodec_close(ctx);
		ctx = NULL;
	}

	if (stream)	{
		stream = NULL;
		// NOTE: The stream is managed by the AVFormatContext
		//av_freep(stream);
	}
	
	pts = 0.0;
	error = "";
}


// ---------------------------------------------------------------------
//
// Audio Encoder Context
//
// ---------------------------------------------------------------------
AudioEncoderContext::AudioEncoderContext() :
	resampler(NULL),
	samplesPerFrame(0),
	outputBytes(0)
{
}
	

AudioEncoderContext::~AudioEncoderContext()
{
	close();
}


void AudioEncoderContext::create(AVFormatContext* oc)
{
	AudioContext::create();
	
	// Find the audio encoder
	//codec = avcodec_find_encoder_by_name(oparams.encoder.data());
	codec = avcodec_find_encoder(oc->oformat->audio_codec);
	if (!codec)
   		throw Exception("Audio encoder not found: " + oparams.encoder);

	oc->oformat->audio_codec = codec->id;

	stream = avformat_new_stream(oc, codec);
	if (!stream)
		throw Exception("Failed to initialize the audio stream.");

	// Now we'll setup the parameters of AVCodecContext
	ctx = stream->codec;
	avcodec_get_context_defaults3(ctx, codec);
	ctx->codec_id = codec->id;
	ctx->codec_type = AVMEDIA_TYPE_AUDIO;
	ctx->bit_rate = oparams.bitRate;
	ctx->sample_rate = oparams.sampleRate;
	ctx->channels = oparams.channels;
	ctx->sample_fmt = av_get_sample_fmt(oparams.sampleFmt);
	ctx->channel_layout = av_get_default_channel_layout(oparams.channels);
	ctx->time_base.num = 1;
	ctx->time_base.den = oparams.sampleRate;

    // Some formats want stream headers to be separate
    if (oc->oformat->flags & AVFMT_GLOBALHEADER)
        ctx->flags |= CODEC_FLAG_GLOBAL_HEADER;
	
	// Open the codec
	if (avcodec_open2(ctx, codec, NULL) < 0)
		throw Exception("Cannot open the audio codec.");

	// Set the frame size
    samplesPerFrame = ctx->frame_size;
		
	// Set a reasonable maximum output size
	outputBytes = samplesPerFrame
		 * av_get_bytes_per_sample(ctx->sample_fmt)
		 * ctx->channels;
		
    // Create a resampler if required
    if (ctx->sample_fmt != av_get_sample_fmt(iparams.sampleFmt)) {    
		resampler = new AudioResampler();
		resampler->create(iparams, oparams);
    }
}


/*
void AudioEncoderContext::open()
{
	LogDebug() << "[AudioEncoderContext:" << this << "] Opening: " << oparams.toString() << endl;
	
	assert(codec);
	assert(frame);
	AudioContext::open();
		
	// Find the audio encoder
	AVCodec* c = avcodec_find_encoder_by_name(oparams.encoder.data());
	if (!c)
   		throw Exception("Audio encoder not found.");

	// Open the audio codec
	if (avcodec_open2(ctx, codec, NULL) < 0)
   		throw Exception("Cannot open the audio codec.");
}
*/


void AudioEncoderContext::close()
{
	LogDebug() << "[AudioEncoderContext:" << this << "] Closing" << endl;

	AudioContext::close();	
	
	if (resampler) {
		delete resampler;
		resampler = NULL;
	}
	
	/*
	if (buffer) {
		av_free(buffer);
		buffer = NULL;
	}

	if (samples) {
		av_free(samples);
		samples = NULL;
	}
	*/
}


bool AudioEncoderContext::encode(unsigned char* data, int size, AVPacket& opacket)
{
	assert(data);
	assert(size);
    AVPacket ipacket;
    av_init_packet(&ipacket);
    ipacket.stream_index = stream->index;
    ipacket.data = data;
    ipacket.size = size;
	return encode(ipacket, opacket);
}


bool AudioEncoderContext::encode(AVPacket& ipacket, AVPacket& opacket)
{
	LogTrace() << "[AudioEncoderContext:" << this << "] Encoding Audio Packet" << endl;

	assert(ipacket.stream_index == stream->index);
	//assert(frame);
	//assert(buffer);
	//assert(bufferSize);	

	
	UInt8* samples;

	if (resampler) {
		samples = resampler->resample(ipacket.data, ipacket.size);

		// The resampler may buffer data, in which case we return.
		if (samples == NULL)
			return false;		
	
		//assert(resampler->outNbSamples == outputBytes);
	}
	else {
		samples = (UInt8*)ipacket.data;
	}
	
	frame = avcodec_alloc_frame();
	avcodec_get_frame_defaults(frame);
	frame->nb_samples = samplesPerFrame;

	// BUG: Libspeex encoding does not increment ctx->frame_number.
	// Might need to create local frame number value for PTS.
	//frame->pts = ctx->frame_number; // Force a PTS value, no AV_NOPTS_VALUE
    if (avcodec_fill_audio_frame(frame, ctx->channels, ctx->sample_fmt, samples, outputBytes, 0) < 0) {
		error = "Error filling audio frame";
		LogError() << "[AudioEncoderContext:" << this << "] " << error << endl;
        throw Exception(error);
    }
	
    av_init_packet(&opacket);
	opacket.stream_index = stream->index;	
    opacket.data = NULL;
    opacket.size = 0;

	int frameEncoded = 0;
    if (avcodec_encode_audio2(ctx, &opacket, frame, &frameEncoded) < 0) {
		error = "Fatal Encoder Error";
		LogError() << "[AudioEncoderContext:" << this << "] " << error << endl;
		throw Exception(error);
    }
		
	if (frameEncoded) {
		if (opacket.pts != AV_NOPTS_VALUE)
			opacket.pts      = av_rescale_q(opacket.pts,      ctx->time_base, stream->time_base);
		if (opacket.dts != AV_NOPTS_VALUE)
			opacket.dts      = av_rescale_q(opacket.dts,      ctx->time_base, stream->time_base);
		if (opacket.duration > 0)
			opacket.duration = av_rescale_q(opacket.duration, ctx->time_base, stream->time_base);

		LogTrace() << "[AudioEncoderContext:" << this << "] Encoded PTS:\n" 
			//<< "\n\tPTS: " << av_ts2str(opacket.pts)
			//<< "\n\tDTS: " << av_ts2str(opacket.dts)
			//<< "\n\tPTS Time: " << av_ts2timestr(opacket.pts, &stream->time_base)
			//<< "\n\tDTS Time: " << av_ts2timestr(opacket.dts, &stream->time_base)
			<< "\n\tPTS: " << opacket.pts
			<< "\n\tDTS: " << opacket.dts
			<< "\n\tFrame PTS: " << frame->pts
			<< "\n\tDuration: " << opacket.duration
			//<< "\n\tCodec Time Den: " << ctx->time_base.den
			//<< "\n\tCodec Time Num: " << ctx->time_base.num
			//<< "\n\tStream Time Den: " << stream->time_base.den
			//<< "\n\tStream Time Num: " << stream->time_base.num
			<< endl;
	}

	else
		LogError() << "[AudioEncoderContext:" << this << "] No Frame" << endl;
	
    avcodec_free_frame(&frame);

	return frameEncoded > 0;
	
	
	/*
	// Old API
	// TODO: Update to use new avcodec_encode_audio2 API
	int size = avcodec_encode_audio(ctx, this->buffer, outputBytes, (short*)samples);
	if (size < 0) {		
		error = "Fatal Encoder Error";
		LogError() << "[AudioEncoderContext:" << this << "] " << error << endl;
		throw Exception(error);
	}
	
	av_init_packet(&opacket);
	opacket.data = this->buffer;	
	opacket.size = size;

	//if (ctx->coded_frame->pts != AV_NOPTS_VALUE)
	//	opacket.pts = av_rescale_q(ctx->coded_frame->pts,      ctx->time_base, stream->time_base);

	return true;
	*/

	//
	// New API avcodec_encode_audio2 implementation
	// Receiving "extended_data is not set." error message 
	// and some codecs failing to encode. Investigate further
	// before using the following code.
	//
	//int audioInputSampleSize = ctx->frame_size * 2 * ctx->channels; //ctx->frame_size; //ipacket.size; // This is probably incorrect
    //int size = av_samples_get_buffer_size(NULL, ctx->channels,
    //                                          audioInputSampleSize,
    //                                          ctx->sample_fmt, 1);

	//LogTrace() << "[AudioEncoderContext:" << this << "] Encoding Audio Packet: " << audioInputSampleSize << ": " << size << endl;

	//frame->data[0] = ipacket.data; // old way
	//frame->nb_samples = audioInputSampleSize; // http://stackoverflow.com/questions/12967730/how-do-i-create-a-stereo-mp3-file-with-latest-version-of-ffmpeg
	//ret = avcodec_fill_audio_frame(frame, c->channels, c->sample_fmt, pSoundBuffer, size, 1);

	// CHECK https://github.com/FFmpeg/FFmpeg/blob/master/doc/examples/muxing.c
	// https://trac.handbrake.fr/changeset/4838/trunk/libhb/encavcodecaudio.c
	
	/*
	//int 
	samplesPerFrame  = ctx->frame_size;
	int expectedFrameSize = samplesPerFrame  * av_get_bytes_per_sample(ctx->sample_fmt) * ctx->channels;
	frame = avcodec_alloc_frame();

	avcodec_get_frame_defaults(frame);
	 
    //get_audio_frame((Int16*)ipacket.data, samplesPerFrame , ctx->channels);
    //get_audio_frame(samples, samplesPerFrame , ctx->channels);

	frame->nb_samples = samplesPerFrame ; //ctx->frame_size; // * 2 * ctx->channels; // NOTE: Should be input size, nopt output!
	//frame->pts = ctx->frame_number;

	assert(ipacket.size == expectedFrameSize);
    if (avcodec_fill_audio_frame(frame, ctx->channels, ctx->sample_fmt, ipacket.data, ipacket.size, 1) < 0) {
    //if (avcodec_fill_audio_frame(frame, ctx->channels, ctx->sample_fmt, (uint8_t *)samples, expectedFrameSize, 1) < 0) {
		error = "Error filling audio frame";
		LogError() << "[AudioEncoderContext:" << this << "] " << error << endl;
        throw Exception(error);
    }
	
    av_init_packet(&opacket);
	opacket.stream_index = stream->index;	
    opacket.data = NULL; //buffer;
    opacket.size = 0; //bufferSize;
	
	int frameEncoded = 0;
    if (avcodec_encode_audio2(ctx, &opacket, frame, &frameEncoded) < 0) {
		error = "Fatal Encoder Error";
		LogError() << "[AudioEncoderContext:" << this << "] " << error << endl;
		throw Exception(error);
    }
		
	if (frameEncoded) {
		if (opacket.pts != AV_NOPTS_VALUE)
			opacket.pts      = av_rescale_q(opacket.pts,      ctx->time_base, stream->time_base);
		if (opacket.dts != AV_NOPTS_VALUE)
			opacket.dts      = av_rescale_q(opacket.dts,      ctx->time_base, stream->time_base);
		if (opacket.duration > 0)
			opacket.duration = av_rescale_q(opacket.duration, ctx->time_base, stream->time_base);

		LogTrace() << "[AudioEncoderContext:" << this << "] Encoded PTS:\n" 
			//<< "\n\tPTS: " << av_ts2str(opacket.pts)
			//<< "\n\tDTS: " << av_ts2str(opacket.dts)
			//<< "\n\tPTS Time: " << av_ts2timestr(opacket.pts, &stream->time_base)
			//<< "\n\tDTS Time: " << av_ts2timestr(opacket.dts, &stream->time_base)
			<< "\n\tPTS: " << opacket.pts
			<< "\n\tDTS: " << opacket.dts
			<< "\n\tFrame PTS: " << frame->pts
			//<< "\n\tCodec Time Den: " << ctx->time_base.den
			//<< "\n\tCodec Time Num: " << ctx->time_base.num
			//<< "\n\tStream Time Den: " << stream->time_base.den
			//<< "\n\tStream Time Num: " << stream->time_base.num
			<< endl; 
	}
	else
		LogError() << "[AudioEncoderContext:" << this << "] No Frame" << endl;
	
    avcodec_free_frame(&frame);

	return frameEncoded > 0;
	*/
}


// ---------------------------------------------------------------------
//
// Audio Decoder Context
//
// ---------------------------------------------------------------------
AudioDecoderContext::AudioDecoderContext() :
	duration(0.0),
	width(-1),
	fp(false)
{
}
	

AudioDecoderContext::~AudioDecoderContext()
{
	close();
}


void AudioDecoderContext::create(AVFormatContext *ic, int streamID)
{
	AudioContext::create();
	
	LogDebug() << "[AudioDecoderContext:" << this << "] Creating: " << streamID << endl;

	assert(ic);
	assert(streamID >= 0);
	
    stream = ic->streams[streamID];
    ctx = stream->codec;

    codec = avcodec_find_decoder(ctx->codec_id);
	if (!codec)
   		throw Exception("Audio decoder not found.");

    if (avcodec_open(ctx, codec) < 0)
		throw Exception("Could not open the audio codec.");

    switch (ctx->sample_fmt) {
    case AV_SAMPLE_FMT_S16:
        width = 16;
        fp = false;
        break;
    case AV_SAMPLE_FMT_S32:
        width = 32;
        fp = false;
        break;
    case AV_SAMPLE_FMT_FLT:
        width = 32;
        fp = true;
        break;
    case AV_SAMPLE_FMT_DBL:
        width = 64;
        fp = true;
        break;
    default:
		throw Exception("Unsupported audio sample format.");
    }
	
	frame = avcodec_alloc_frame();
	if (!frame)
		throw Exception("Cannot allocate input frame.");
}


/*
void AudioDecoderContext::open()
{
	LogDebug() << "[AudioEncoderContext:" << this << "] Opening" << endl;
	
	assert(codec);
	assert(frame);
	AudioContext::open();
		
	// Find the audio encoder
    AVCodec* c = avcodec_find_decoder(ctx->codec_id);
	if (!c)
   		throw Exception("Audio decoder not found.");

	// Open the audio codec
	if (avcodec_open2(ctx, codec, NULL) < 0)
   		throw Exception("Cannot open the audio codec.");
}
*/


void AudioDecoderContext::close()
{
	AudioContext::close();

	duration = 0.0;	
	width = -1;
	fp = false;
}


bool AudioDecoderContext::decode(UInt8* data, int size, AVPacket& opacket)
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

	/*
	// Skip if exceeding maximum FPS
	if (maxFPS && ipacket.dts != AV_NOPTS_VALUE) {
		double fps = ctx->frame_number / (ipacket.dts * av_q2d(stream->time_base));
		if (fps > maxFPS) {
			LogTrace() << "[AudioDecoderContext:" << this << "] Dropping frame at fps: " << fps << endl;
			return false;
		}
		LogTrace() << "[AudioDecoderContext:" << this << "] Decoding at fps: " << fps << ": " << ctx->frame_number << endl;
	}
	*/

	int frameDecoded = 0;
	int bytesDecoded = 0;
	int bytesRemaining = ipacket.size;
	
	av_init_packet(&opacket);
	opacket.data = NULL;
	opacket.size = 0;
	
	avcodec_get_frame_defaults(frame);
	bytesDecoded = avcodec_decode_audio4(ctx, frame, &frameDecoded, &ipacket);		
	if (bytesDecoded < 0) {
		error = "Decoder error";
		LogError() << "[AudioDecoderContext:" << this << "] " << error << endl;
		throw Exception(error);
	}

	// XXX: Asserting here to make sure below looping 
	// avcodec_decode_video2 is actually redundant.
	// Otherwise we need to reimplement this pseudo code:
	// while(packet->size > 0)
	// {
	// 	 int ret = avcodec_decode_video2(..., ipacket);
	// 	 if(ret < -1)
	//		throw std::exception("error");
	//
	//	 ipacket->size -= ret;
	//	 ipacket->data += ret;
	// }
	assert(bytesDecoded == bytesRemaining);

	/*
	while (bytesRemaining) // && !frameDecoded
	{
		avcodec_get_frame_defaults(frame);
		bytesDecoded = avcodec_decode_audio4(ctx, frame, &frameDecoded, &ipacket);		
		if (bytesDecoded < 0) {
			LogError() << "[AudioDecoderContext:" << this << "] Decoder Error" << endl;
			error = "Decoder error";
			throw Exception(error);
		}

		bytesRemaining -= bytesDecoded;
	}
	*/

	if (frameDecoded) {
		InitDecodedAudioPacket(stream, ctx, frame, &opacket, &pts);

		/*
		LogTrace() << "[AudioDecoderContext:" << this << "] Decoded Frame:" 
			<< "\n\tFrame Size: " << opacket.size
			<< "\n\tFrame PTS: " << opacket.pts
			<< "\n\tInput Frame PTS: " << ipacket.pts
			<< "\n\tNo Frame PTS: " << (frame->pts != AV_NOPTS_VALUE)
			<< "\n\tDecoder PTS: " << pts
			<< endl;
			*/

		return true;
	}

	return false;
}
    

bool AudioDecoderContext::flush(AVPacket& opacket)
{	
	AVPacket ipacket;
	av_init_packet(&ipacket);
	ipacket.data = NULL;
	ipacket.size = 0;

	av_init_packet(&opacket);
	opacket.data = NULL;
	opacket.size = 0;
	
	int frameDecoded = 0;
	avcodec_decode_video2(ctx, frame, &frameDecoded, &ipacket);

	if (frameDecoded) {
		InitDecodedAudioPacket(stream, ctx, frame, &opacket, &pts);
		LogDebug() << "[AudioDecoderContext:" << this << "] Flushed Audio Frame: " << opacket.pts << endl;
		return true;
	}
	return false;
}


// ---------------------------------------------------------------------
//
// Audio Conversion Context
//
// ---------------------------------------------------------------------
AudioResampler::AudioResampler() :
	ctx(NULL),
	outBuffer(NULL),
	outNbSamples(0)
{
}
	

AudioResampler::~AudioResampler()
{	
	free();
}


void AudioResampler::create(const AudioCodec& iparams, const AudioCodec& oparams) //, AVCodecContext* ocontext, int encFrameSize
{
	LogTrace() << "[AudioResampler:" << this << "] Creating:" 
		<< "\n\tInput Sample Rate: " << iparams.sampleRate
		<< "\n\tOutput Sample Rate:: " << oparams.sampleRate
		<< endl;

    if (ctx)
        throw Exception("Resample context already initialized");

	Int64 outChLayout = av_get_default_channel_layout(oparams.channels);
	enum AVSampleFormat outSampleFmt = av_get_sample_fmt(oparams.sampleFmt);

    Int64 inChLayout  = av_get_default_channel_layout(iparams.channels);
    enum AVSampleFormat inSampleFmt  = av_get_sample_fmt(iparams.sampleFmt);

    ctx = swr_alloc_set_opts(ctx,
            outChLayout, outSampleFmt, oparams.sampleRate,
            inChLayout,  inSampleFmt,  iparams.sampleRate,
            0, NULL);
    if (ctx == NULL)
        throw Exception("Cannot configure resampler context");

	char inChBuf[128], outChBuf[128];
	av_get_channel_layout_string(inChBuf,  sizeof(inChBuf),  -1, inChLayout);
	av_get_channel_layout_string(outChBuf, sizeof(outChBuf), -1, outChLayout);

	LogTrace() << "[AudioResampler:" << this << "] Resampler Options:\n" 
		<< "\n\tIn Channel Layout: " << inChBuf
		<< "\n\tIn Sample Rate: " << iparams.sampleRate
		<< "\n\tIn Sample Fmt: " << av_get_sample_fmt_name(inSampleFmt)
		<< "\n\tOut Channel Layout: " << outChBuf
		<< "\n\tOut Sample Rate: " << oparams.sampleRate
		<< "\n\tOut Sample Fmt: " << av_get_sample_fmt_name(outSampleFmt)
		<< endl; 

    if (ctx == NULL) {
        throw Exception("Cannot create resampler context");
    }

    if (swr_init(ctx)) {
        throw Exception("Cannot initialise resampler");
    }

	this->iparams = iparams;
	this->oparams = oparams;
	
	LogTrace() << "[AudioResampler:" << this << "] Creating: OK" << endl;
}
	

void AudioResampler::free()
{
	LogTrace() << "[AudioResampler:" << this << "] Closing" << endl;

	//if (oframe) {
	//	av_free(oframe);
	//	oframe = NULL;
	//}
	
	if (ctx) {
		swr_free(&ctx);
		ctx = NULL;
	}

	if (outBuffer) {
		av_freep(&outBuffer);
		outBuffer = NULL;
	}

	LogTrace() << "[AudioResampler:" << this << "] Closing: OK" << endl;
}


UInt8* AudioResampler::resample(UInt8* inSamples, int inNbSamples)
{
    if (!ctx)
        throw Exception("Conversion context must be initialized.");	

	/* swresample.h
	 *
	 * @code
	 * SwrContext *swr = swr_alloc();
	 * av_opt_set_int(swr, "in_channel_layout",  AV_CH_LAYOUT_5POINT1, 0);
	 * av_opt_set_int(swr, "out_channel_layout", AV_CH_LAYOUT_STEREO,  0);
	 * av_opt_set_int(swr, "in_sample_rate",     48000,                0);
	 * av_opt_set_int(swr, "out_sample_rate",    44100,                0);
	 * av_opt_set_sample_fmt(swr, "in_sample_fmt",  AV_SAMPLE_FMT_FLTP, 0);
	 * av_opt_set_sample_fmt(swr, "out_sample_fmt", AV_SAMPLE_FMT_S16,  0);
	 * @endcode
	 *
	 * @code
	 * uint8_t **input;
	 * int in_samples;
	 *
	 * while (get_input(&input, &in_samples)) {
	 *     uint8_t *output;
	 *     int out_samples = av_rescale_rnd(swr_get_delay(swr, 48000) +
	 *                                      in_samples, 44100, 48000, AV_ROUND_UP);
	 *     av_samples_alloc(&output, NULL, 2, out_samples,
	 *                      AV_SAMPLE_FMT_S16, 0);
	 *     out_samples = swr_convert(swr, &output, out_samples,
	 *                                      input, in_samples);
	 *     handle_output(output, out_samples);
	 *     av_freep(&output);
	 *  }
	 *  @endcode
	 */

	outNbSamples = av_rescale_rnd(
		swr_get_delay(ctx, iparams.sampleRate) + inNbSamples, 
		oparams.sampleRate, iparams.sampleRate, AV_ROUND_UP);

	if (outBuffer) {
		av_freep(&outBuffer);
		outBuffer = NULL;
	}

	/*
	LogTrace() << "[AudioResampler:" << this << "] Resampling:\n" 
		<< "\n\tIn Nb Smaples 1: " << inNbSamples
		<< "\n\tIn Channels: " << iparams.channels
		<< "\n\tIn Sample Rate: " << iparams.sampleRate
		<< "\n\tIn Sample Fmt: " << av_get_sample_fmt_name((AVSampleFormat)iparams.sampleFmt)
		<< "\n\tOut Nb Samples: " << outNbSamples
		<< "\n\tOut Channels: " << oparams.channels
		<< "\n\tOut Sample Rate: " << oparams.sampleRate
		<< "\n\tOut Sample Fmt: " << av_get_sample_fmt_name((AVSampleFormat)oparams.sampleFmt)
		<< endl; 
		*/
	
	av_samples_alloc(&outBuffer, NULL, oparams.channels, outNbSamples,
                       av_get_sample_fmt(oparams.sampleFmt), 0);
	
    outNbSamples = swr_convert(ctx, &this->outBuffer, outNbSamples,
             (const UInt8**)&inSamples, inNbSamples);
    if (outNbSamples < 0)
        throw Exception("Cannot resample audio");

	assert(outNbSamples == inNbSamples);

	return outBuffer;
}


// ---------------------------------------------------------------------
//
// Inlines & Helpers
//
// ---------------------------------------------------------------------		  
void InitDecodedAudioPacket(const AVStream* stream, const AVCodecContext* ctx, const AVFrame* frame, AVPacket* opacket, double* pts)
{		
	opacket->data = frame->data[0];
	opacket->size = av_samples_get_buffer_size(NULL, ctx->channels, frame->nb_samples, ctx->sample_fmt, 1);		
	opacket->dts = frame->pkt_dts; // Decoder PTS values can be unordered
	opacket->pts = frame->pkt_pts;	
		
	// Local PTS value represented as decimal seconds
    if (opacket->dts != AV_NOPTS_VALUE) {
		*pts = opacket->pts;
		*pts *= av_q2d(stream->time_base);
	}

	assert(opacket->data);
	assert(opacket->size);
	assert(opacket->dts >= 0);
	assert(opacket->pts >= 0);	

	/*	
	// ffplay.c
	if (decoder_reorder_pts == -1) {
        *pts = av_frame_get_best_effort_timestamp(frame);
    } else if (decoder_reorder_pts) {
        *pts = frame->pkt_pts;
    } else {
        *pts = frame->pkt_dts;
    }

    if (*pts == AV_NOPTS_VALUE) {
        *pts = 0;
    }
	*/
}

	
} } // namespace Sourcey::Media



		
		/*
		opacket.data = frame->data[0];
		opacket.size = av_samples_get_buffer_size(NULL, ctx->channels, frame->nb_samples, ctx->sample_fmt, 1);		
		opacket.dts = frame->pkt_dts; // Decoder PTS values can be unordered
		opacket.pts = frame->pkt_pts;

		assert(opacket.data);
		assert(opacket.size);
		assert(opacket.dts >= 0);
		assert(opacket.pts >= 0);		
		
		// Local PTS value represented as decimal seconds
        if (opacket.dts != AV_NOPTS_VALUE) {
			pts = opacket.pts;
			pts *= av_q2d(stream->time_base);
		}
		
		// Use the input packet timestamp if the
		// frame PTS wasn't set by the decoder.
		if (frame->pts == AV_NOPTS_VALUE)
			frame->pts = ipacket.dts;
		opacket.pts = frame->pts;
		
		// Local pts value represented as second decimal.
		pts = opacket.pts;
		pts *= av_q2d(stream->time_base);
		
		// http://dranger.com/ffmpeg/ffmpegtutorial_all.html
		//if (ipacket.dts != AV_NOPTS_VALUE) {
		//}

		// NOTE: avcodec_decode_audio4 does not seem to set the PTS
		// for output frame so we just use the input frame PTS value.
		frame->pts = ipacket.pts;
		if (frame->pts != AV_NOPTS_VALUE) {
			opacket.pts = frame->pts;
			pts = frame->pts;
			pts *= av_q2d(stream->time_base);
		}
		*/
	/*
    if (inSampleFmt == AV_SAMPLE_FMT_NONE || 
		outSampleFmt == AV_SAMPLE_FMT_NONE)
        throw Exception("Conversion not required."); // TODO: nothing

    if (inSampleFmt == outSampleFmt // same input and output format
            && inChLayout == outChLayout // same number of channels
			&& iparams.sampleRate == oparams.sampleRate) // same samplerate		
        throw Exception("Conversion not required."); // TODO: nothing
		*/

    /* Check if we are given a context with similar options and do not
     * reallocate it in this case 
    if (*s) {
            Int64 c_outChLayout,
                     c_inChLayout,
                    c_outSampleFmt,
                     c_inSampleFmt,
                    c_out_sample_rate,
                     c_in_sample_rate;

            av_opt_get_int(*s, "ocl", 0, &c_outChLayout  );
            av_opt_get_int(*s, "osf", 0, &c_outSampleFmt );
            av_opt_get_int(*s, "osr", 0, &c_out_sample_rate);
            av_opt_get_int(*s, "icl", 0, &c_inChLayout   );
            av_opt_get_int(*s, "isf", 0, &c_inSampleFmt  );
            av_opt_get_int(*s, "isr", 0, &c_in_sample_rate );

            if(
               c_outChLayout   == outChLayout                &&
                c_inChLayout   ==  inChLayout                &&
               c_outSampleFmt  == outSampleFmt               &&
                c_inSampleFmt  ==  inSampleFmt               &&
               c_out_sample_rate == filter->fmt_out.audio.i_rate &&
                c_in_sample_rate == filter->fmt_in.audio.i_rate
              )
                return VLC_SUCCESS;
    }*/

	
	/*
	// Find the audio encoder.
	// Some audio encoders have separate floating and fixed
	// point encoders so we need to manually specify the fixed
	// point encoder until we can control the sample_fmt via
	// configuration.
	AVCodec* c = NULL;
	if (oc->oformat->audio_codec == CODEC_ID_AC3)
		c = avcodec_find_encoder_by_name("ac3_fixed");
	else
		c = avcodec_find_encoder(oc->oformat->audio_codec);
		*/
		
	//ctx->bit_rate = 32000; //oparams.bitRate * 1000 / 25; 
	//ctx->bit_rate = oparams.bitRate * 1000 / 25; 
	//ctx->time_base.den = 1000;
	// Allocate the output samples buffer
	// NOTE: buffer must be >= AVCODEC_MAX_AUDIO_FRAME_SIZE
	// or some codecs will fail.
    //bufferSize = AVCODEC_MAX_AUDIO_FRAME_SIZE; //MAX_AUDIO_PACKET_SIZE;
    //buffer = (UInt8*)av_malloc(bufferSize);			
	// Allocate the input samples buffer
    //samples = (Int16*)av_malloc(outputBytes);
	


/*
double AudioContext::pts()
{

	if (packet && stream) {
		double pts = (packet->dts != AV_NOPTS_VALUE) ? packet->dts : 0.0;
		pts *= av_q2d(stream->time_base);
		return pts;
	}
	return 0.0;
}
*/

/*
00619 static void do_audio_out(AVFormatContext *s, OutputStream *ost,
00620                          AVFrame *frame)
00621 {
00622     AVCodecContext *enc = ost->st->codec;
00623     AVPacket pkt;
00624     int got_packet = 0;
00625 
00626     av_init_packet(&pkt);
00627     pkt.data = NULL;
00628     pkt.size = 0;
00629 
00630     if (!check_recording_time(ost))
00631         return;
00632 
00633     if (frame->pts == AV_NOPTS_VALUE || audio_sync_method < 0)
00634         frame->pts = ost->sync_opts;
00635     ost->sync_opts = frame->pts + frame->nb_samples;
00636 
00637     av_assert0(pkt.size || !pkt.data);
00638     update_benchmark(NULL);
00639     if (avcodec_encode_audio2(enc, &pkt, frame, &got_packet) < 0) {
00640         av_log(NULL, AV_LOG_FATAL, "Audio encoding failed (avcodec_encode_audio2)\n");
00641         exit(1);
00642     }
00643     update_benchmark("encode_audio %d.%d", ost->file_index, ost->index);
00644 
00645     if (got_packet) {
00646         if (pkt.pts != AV_NOPTS_VALUE)
00647             pkt.pts      = av_rescale_q(pkt.pts,      enc->time_base, ost->st->time_base);
00648         if (pkt.dts != AV_NOPTS_VALUE)
00649             pkt.dts      = av_rescale_q(pkt.dts,      enc->time_base, ost->st->time_base);
00650         if (pkt.duration > 0)
00651             pkt.duration = av_rescale_q(pkt.duration, enc->time_base, ost->st->time_base);
00652 
00653         if (debug_ts) {
00654             av_log(NULL, AV_LOG_INFO, "encoder -> type:audio "
00655                    "pkt_pts:%s pkt_pts_time:%s pkt_dts:%s pkt_dts_time:%s\n",
00656                    av_ts2str(pkt.pts), av_ts2timestr(pkt.pts, &ost->st->time_base),
00657                    av_ts2str(pkt.dts), av_ts2timestr(pkt.dts, &ost->st->time_base));
00658         }
00659 
00660         audio_size += pkt.size;
00661         write_frame(s, &pkt, ost);
00662 
00663         av_free_packet(&pkt);
00664     }
00665 }
*/


		
	/*


static float t, tincr, tincr2;
void get_audio_frame(Int16 *samples, int frame_size, int nb_channels)
{
    int j, i, v;
    int16_t *q;

    q = samples;
    for (j = 0; j < frame_size; j++) {
        v = (int)(sin(t) * 10000);
        for (i = 0; i < nb_channels; i++)
            *q++ = v;
        t     += tincr;
        tincr += tincr2;
    }
}
	int r;
    int nb_samples = ctx->frame_size; //10;ctx->frame_size; //1000; //24; //
	AVSampleFormat format = AV_SAMPLE_FMT_S16;
	int sample_rate = 44100;

	int channel_layout = AV_CH_LAYOUT_STEREO;
    int nb_channels = av_get_channel_layout_nb_channels(channel_layout);
    //int bytes_per_sample = av_get_bytes_per_sample(AV_SAMPLE_FMT_S16) * nb_channels;
    int bufsize = av_samples_get_buffer_size(NULL, nb_channels, nb_samples,
                                             format, 1);
	
	LogTrace() << "[AudioEncoderContext:" << this << "] Encoding Audio Packet: ctx->frame_size: " << ctx->frame_size << endl;
	LogTrace() << "[AudioEncoderContext:" << this << "] Encoding Audio Packet: ctx->sample_fmt: " << ctx->sample_fmt << endl;
	LogTrace() << "[AudioEncoderContext:" << this << "] Encoding Audio Packet: nb_channels: " << nb_channels << endl;
	LogTrace() << "[AudioEncoderContext:" << this << "] Encoding Audio Packet: nb_samples: " << nb_samples << endl;
	LogTrace() << "[AudioEncoderContext:" << this << "] Encoding Audio Packet: bufsize: " << bufsize << endl;

    int i;
 
    //uint8_t inbuf[40]; //bufsize];
    uint8_t *input = (uint8_t *)ipacket.data; //new uint8_t(AVCODEC_MAX_AUDIO_FRAME_SIZE); //// { inbuf };
    //uint8_t outbuf[40]; //bufsize];
    uint8_t *output; // = new uint8_t(AVCODEC_MAX_AUDIO_FRAME_SIZE); // = { outbuf };
	
    for (i = 0; i < AVCODEC_MAX_AUDIO_FRAME_SIZE; i++)
        input[i] = i;

 	LogTrace() << "[AudioEncoderContext:" << this << "] Encoding Audio Packet: AV_SAMPLE_FMT_S16: " << av_get_bytes_per_sample(AV_SAMPLE_FMT_S16) << endl;

    struct SwrContext* swr_ctx =
        swr_alloc_set_opts(NULL,
                           channel_layout,
                           format,
                           sample_rate,
                           channel_layout,
                           format,
                           sample_rate,
                           0, NULL); 	
	
	LogTrace() << "[AudioEncoderContext:" << this << "] Encoding Audio Packet: AV_SAMPLE_FMT_S16P: " << av_get_bytes_per_sample(AV_SAMPLE_FMT_S16P) << endl;

    swr_init(swr_ctx);
		
	LogTrace() << "[AudioEncoderContext:" << this << "] Encoding Audio Packet: AV_SAMPLE_FMT_S16P: " << av_get_bytes_per_sample(AV_SAMPLE_FMT_S16P) << endl;

	

	 * @code
 * uint8_t **input;
 * int in_samples;
 *
 * while (get_input(&input, &in_samples)) {
 *     uint8_t *output;
 *     int out_samples = av_rescale_rnd(swr_get_delay(swr, 48000) +
 *                                      in_samples, 44100, 48000, AV_ROUND_UP);
 *     av_samples_alloc(&output, NULL, 2, out_samples,
 *                      AV_SAMPLE_FMT_S16, 0);
 *     out_samples = swr_convert(swr, &output, out_samples,
 *                                      input, in_samples);
 *     handle_output(output, out_samples);
 *     av_freep(&output);
 *  }
 *  @endcode
 */
	
	/*
    int output_nb_samples = av_rescale_rnd(swr_get_delay(swr_ctx, sample_rate) +
                                       nb_samples, sample_rate, sample_rate, AV_ROUND_UP);
	av_samples_alloc(&output, NULL, 2, output_nb_samples,
                       AV_SAMPLE_FMT_S16, 0);

 	LogTrace() << "[AudioEncoderContext:" << this << "] Encoding Audio Packet: output_nb_samples: " << output_nb_samples << endl;

    //for (i = 0; i < sizeof(inbuf); i++)
    //for (i = 0; i < bufsize; i++)
    //    input[i] = i;
 
    // Buffer input

	
	
	//int out_count = sizeof(is- >audio_buf2) / is->audio_tgt.channels / av_get_bytes_per_sample(is->audio_tgt.fmt);
	//int out_count = sizeof(is->audio_buf2) / is->audio_tgt.channels / av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);


    if ((r = swr_convert(swr_ctx, &output, output_nb_samples, // / 2 / 2 / 2
                         (const uint8_t**)&input, nb_samples)) < 0) //
        assert(0); // -1;

	av_freep(&output);
    //if ((r = swr_convert(swr_ctx, &output, output_nb_samples / 2,
    //                     (const uint8_t**)&input, nb_samples)) < 0) //
     //   assert(0); // -1;


    //output[0] = &outbuf[r * bytes_per_sample];
    //output_nb_samples -= r;
 
    // Drain buffer
    //while ((r = swr_convert(swr_ctx, output, output_nb_samples, NULL, 0)) > 0) {
    //    output[0] = &outbuf[r * bytes_per_sample];
    //    output_nb_samples -= r;
    //}
 
    // Resample should have been a straight copy
    //for (i = 0; i < sizeof(inbuf); i++) {
		//LogTrace() << "[AudioEncoderContext:" << this << "] Encoding Audio Packet:" << inbuf[i] << ": " << outbuf[i] << endl;
        //if (inbuf[i] != outbuf[i])
        //    fprintf(stderr, "Byte %d does not match %x != %x\n", i, inbuf[i], outbuf[i]);
    //}
    fprintf(stderr, "Finished\n");
	
	assert(0);
	*/


	/*

    // encode a single tone sound
	int i= 0;
	int j= 0;
    t = 0;
    tincr = 2 * M_PI * 440.0 / ctx->sample_rate;
    for(i=0;i<200;i++) {
        for(j=0;j<samplesPerFrame ;j++) {
            samples[2*j] = (int)(sin(t) * 10000);
            samples[2*j+1] = samples[2*j];
            t += tincr;
        }
        // encode the samples
        //out_size = avcodec_encode_audio(c, outbuf, outbuf_size, samples);
        //fwrite(outbuf, 1, out_size, f);
    }
	*/
		
	//LogError() << "[AudioEncoderContext:" << this << "] Old API" << endl;

	//if (resampler) {
	//	ipacket.data = resampler->resample(ipacket.data);
	//}


	//this->outFrameSize = encFrameSize;

	//
	//this->outNbSamples = encFrameSize * av_get_bytes_per_sample((AVSampleFormat)oparams.sampleFmt) * oparams.channels;

	// Calculate the input frame size from the encode frame size
	//this->inNbSmaples = (int)av_rescale_rnd(this->outNbSamples, iparams.sampleRate, oparams.sampleRate, AV_ROUND_UP);
	
	// Allocate output buffer
	// see: http://patches.audiolan.org/patch/266/
    //this->outBuffer = (UInt8*)av_malloc(this->outNbSamples * this->outFrameSize);	
    //this->outBuffer = (UInt8*)av_malloc(AVCODEC_MAX_AUDIO_FRAME_SIZE);	


	//inFrameSize * av_get_bytes_per_sample((AVSampleFormat)iparams.sampleFmt) * iparams.channels;
		
		//(int)av_rescale_rnd(inNbSmaples, oparams.sampleRate, iparams.sampleRate, AV_ROUND_UP);

	//int audioInputSampleSize = ctx->frame_size * 2 * ctx->channels; 
	//this->inNbSmaples = inFrameSize = av_get_bytes_per_sample((AVSampleFormat)iparams.sampleFmt) * iparams.channels;
	//this->outNbSamples = (int)av_rescale_rnd(inNbSmaples, oparams.sampleRate, iparams.sampleRate, AV_ROUND_UP);
	//int outFrameSize
	//int outNbSamples = av_rescale_rnd(in->i_nb_samples, filter->fmt_out.audio.i_rate, filter->fmt_in.audio.i_rate, AV_ROUND_UP);
    //const size_t framesize = filter->fmt_out.audio.i_bytes_per_frame;
    //out = block_Alloc (outNbSamples * framesize);
	
	/*
	ctx = av_resample_init( 
		oparams.sampleRate,		// out rate
		iparams.sampleRate,		// in rate
		16,						// filter length
		10,						// phase count
		0,						// linear FIR filter
		1.0);					// cutoff frequency
	
    if (!ctx) 
        throw Exception("Invalid conversion context.");
	*/
	//this->ocontext = ocontext;
	


/*
	//avcodec_get_frame_defaults(frame);
	
    //if (avcodec_fill_audio_frame(frame, ctx->channels, ctx->sample_fmt, ipacket.data, AUDIO_INBUF_SIZE, 1) < 0) { //AUDIO_INBUF_SIZE
    if (avcodec_fill_audio_frame(frame, ctx->channels, ctx->sample_fmt, ipacket.data, ipacket.size, 1) < 0) { //AUDIO_INBUF_SIZE
        //av_log(NULL, AV_LOG_FATAL, "Audio encoding failed\n");
        //exit(1);

		error = "Unable To Fill Frame";
		LogError() << "[AudioEncoderContext:" << this << "] " << error << endl;
		throw Exception(error);
    }
	
    //frame->nb_samples  = AUDIO_INBUF_SIZE / (ctx->channels * av_get_bytes_per_sample(ctx->sample_fmt));
    //frame->nb_samples  = ipacket.size / (ctx->channels * av_get_bytes_per_sample(ctx->sample_fmt));
	*/
		
	// Set the output frame size.
	//frameSize = ctx->frame_size * 2 * ctx->channels;
	//

	/*
	if (ctx->channels == 6)
		ctx->channel_layout = AV_CH_LAYOUT_5POINT1;

	if (ctx->codec_id == CODEC_ID_AAC) {		
		ctx->profile = FF_PROFILE_AAC_LOW;		
	}
	else if (ctx->codec_id == CODEC_ID_AMR_NB) {
		ctx->sample_rate = 8000;
		ctx->channels = 1;
	}

	// Find the encoder
	//avcodec_find_encoder_by_name
	AVCodec* c = avcodec_find_encoder(ctx->codec_id);
	if (!c)
		throw Exception("Audio codec not found.");	
	
	// Set the frame size
	if (ctx->frame_size <= 1) {
		ctx->frame_size = MAX_AUDIO_PACKET_SIZE / ctx->channels;
		switch (ctx->codec_id) {
		case CODEC_ID_PCM_S16LE:
		case CODEC_ID_PCM_S16BE:
		case CODEC_ID_PCM_U16LE:
		case CODEC_ID_PCM_U16BE:
			ctx->frame_size >>= 1;
			break;
		default:
			break;
		}
	}

	// Set the encoded output frame size
	//int frameDecoded = av_get_bits_per_sample_format(ctx->sample_fmt)/8;
	_audioOutSize = ctx->frame_size * 2 * ctx->channels;

	// The encoder may require a minimum number of raw audio samples for each encoding but we can't
	// guarantee we'll get this minimum each time an audio frame is decoded from the in file so 
	// we use a FIFO to store up incoming raw samples until we have enough for one call to the codec.
	_audioFifo = av_fifo_alloc(MAX_AUDIO_PACKET_SIZE * 2);

	// Allocate a buffer to read OUT of the FIFO into. The FIFO maintains its own buffer internally.
	if ((_audioFifoOutBuffer = (UInt8*)av_malloc(MAX_AUDIO_PACKET_SIZE * 2)) == 0)
		throw Exception("Cannot allocate audio FIFO buffer.");
		*/	
  	//if (packet)
  	//	av_free_packet(packet);

	/*
	if (codec) {
		avcodec_close(codec);
		codec = NULL;
	}
   // if (params.enabled)			delete _audioParams;
	if (_audioFifoOutBuffer) {
		av_free(_audioFifoOutBuffer);
		_audioFifoOutBuffer = NULL;
	}
	if (_audioFifo) {	
		av_fifo_free(_audioFifo);
		_audioFifo = NULL;
	}
	*/
		/*	
		LogTrace() << "[AudioDecoderContext:" << this << "] check frame_size: " << ctx->frame_size << endl;
		LogTrace() << "[AudioDecoderContext:" << this << "] check channels: " << ctx->channels << endl;
		LogTrace() << "[AudioDecoderContext:" << this << "] check size: " << (ctx->frame_size * 2 * ctx->channels) << endl;
		LogTrace() << "[AudioDecoderContext:" << this << "] frame->nb_samples: " << frame->nb_samples << endl;		
		LogTrace() << "[AudioDecoderContext:" << this << "] opacket.size: " << opacket.size << endl;
	
		LogTrace() << "[AudioDecoderContext:" << this << "] bytesTotal: " << ipacket.size << endl;
		LogTrace() << "[AudioDecoderContext:" << this << "] frameDecoded: " << opacket.size << endl;
		LogTrace() << "[AudioDecoderContext:" << this << "] bytesRemaining: " << bytesRemaining << endl;
		LogTrace() << "[AudioDecoderContext:" << this << "] bytesDecoded: " << bytesDecoded << endl;
		LogTrace() << "[AudioDecoderContext:" << this << "] frameDecoded: " << frameDecoded << endl;

		if (ipacket.pts != AV_NOPTS_VALUE) {
			opacket.pts = ipacket.pts;
			opacket.pts *= av_q2d(stream->time_base);
		}	
		if (ipacket.dts != AV_NOPTS_VALUE) {
			opacket.dts = ipacket.dts;
			opacket.dts *= av_q2d(stream->time_base);
		}
		*/

	/*
	int frameDecoded = bufferSize;
	int len = avcodec_decode_audio3(ctx, (int16_t*)buffer, &frameDecoded, &ipacket);
	if (len < 0) {
		error = "Decoder error";
		LogError() << "[AudioDecoderContext:" << this << "] Decoding Audio: Error: " << error << endl;
		return -1;
	}
	//if (len < 0)
	//	throw Exception("Audio decoder error");

	ipacket.size -= len;
    ipacket.data += len;
		
	//LogTrace() << "[AudioDecoderContext:" << this << "] Decoder DTS: " << ipacket.dts << endl;
	//LogTrace() << "[AudioDecoderContext:" << this << "] Decoder Time Base: " << stream->time_base.den << endl;

	if (ipacket.pts != AV_NOPTS_VALUE) {
		pts = ipacket.pts;
		pts *= av_q2d(stream->time_base);
	}

	//LogTrace() << "[AudioDecoderContext:" << this << "] Decoder PTS: " << ipacket.pts << endl;
	//LogTrace() << "[AudioDecoderContext:" << this << "] Decoder PTS 1: " << pts << endl;
	
	return frameDecoded;
	*/

	/*
	try 
	{
	} 
	catch (Exception& exc) 
	{
		error = exc.displayText();
		LogError() << "[AudioDecoderContext:" << this << "] Decoder Error: " << error << endl;
		exc.rethrow();
	}
	return -1;
	*/



	
	/*

	//av_init_packet(&opacket);
	//opacket.data = NULL;
	//opacket.size = 0;

	if (frameEncoded) {
		//opacket.data = frame->data[0];	
		/opacket.size = av_samples_get_buffer_size(NULL, ctx->channels, frame->nb_samples, ctx->sample_fmt, 1);
		
		if (ipacket.pts != AV_NOPTS_VALUE) {
			opacket.pts = ipacket.pts;
			opacket.pts *= av_q2d(stream->time_base);
		}	
		if (ipacket.dts != AV_NOPTS_VALUE) {
			opacket.dts = ipacket.dts;
			opacket.dts *= av_q2d(stream->time_base);
		}

		return true;
	}
	if (frameNum == 0)
		pts = 0;
	else
		pts += sqrt((double)frameNum);

	//if (ctx->coded_frame->pts != AV_NOPTS_VALUE) 
	//	opacket.pts = av_rescale_q(ctx->coded_frame->pts, ctx->time_base, stream->time_base);
	opacket.pts = pts;
		
		LogTrace() << "[AudioEncoderContext:" << this << "] Encoding AUDIO Packet:\n"
			//<< "\n\AV_NOPTS_VALUE: " << (ctx->coded_frame->pts != AV_NOPTS_VALUE)
			//<< "\n\ctx->coded_frame: " << ctx->coded_frame
			//<< "\n\ctx->coded_frame->pts: " << ctx->coded_frame->pts
			<< "\n\ctx->time_base: " << ctx->time_base.den
			<< "\n\ctx->time_base num: " << ctx->time_base.num
			<< "\n\stream->time_base: " << stream->time_base.den
			<< "\n\stream->time_base num: " << stream->time_base.num
			//<< "\n\opacket.pts: " << opacket.pts
			<< endl;

	if (ctx->coded_frame->key_frame) 
        opacket.flags |= AV_PKT_FLAG_KEY;

	opacket.stream_index = stream->index;
	opacket.data = this->buffer;
	opacket.size = len;
	
	return len;
			*/

	
	/*
    c = st->codec;
	
    c->sample_fmt = AV_SAMPLE_FMT_S16;
    c->bit_rate = 64000;
    c->sample_rate = 44100;
    c->channels = 2;

    // some formats want stream headers to be separate
    if (oc->oformat->flags & AVFMT_GLOBALHEADER)
        c->flags |= CODEC_FLAG_GLOBAL_HEADER;

	// Initializes the audio buffers
	//this->buffer = new unsigned char[MAX_AUDIO_PACKET_SIZE];

	// Add an audio stream that uses the format's default audio codec 
	// to the format context's streams[] array.
	stream = av_new_stream(oc, 0);
	if (!stream)
		throw Exception("Could not allocate audio stream");
	*/
	/*, 
	//AVPacket packet;
	av_init_packet(&packet);

	if (ctx->coded_frame && 
		ctx->coded_frame->pts != AV_NOPTS_VALUE) 
		packet->pts = av_rescale_q(ctx->coded_frame->pts, ctx->time_base, stream->time_base);
			
	if (ctx->coded_frame &&
		ctx->coded_frame->key_frame) 
        packet->flags |= AV_PKT_FLAG_KEY;

	packet->stream_index = stream->index;
	packet->data = this->buffer;
	packet->size = len;

		AVPacket& outpacket

	 	// Write the encoded frame to the output file
		int result = av_interleaved_write_frame(oc, &packet);
		if (result != 0) {
			LogError() << "[AudioEncoderContext:" << this << "] Failed to write audio frame." << endl;
			return false;
		}
	} 
	else {
		LogWarn() << "[AudioEncoderContext:" << this << "] Encoded audio frame size is NULL; it may have been buffered." << endl;
		return false;
	}

	if (!buffer || !bufferSize) 
		throw Exception("Cannot encode audio frame.");

	if (bufferSize > this->bufferSize) 
		throw Exception("Audio frame too big.");
	
	// Lock the mutex while encoding
	//FastMutex::ScopedLock lock(_mutex);	

	//if (!isReady())
	//	throw Exception("The encoder is not initialized.");
	
	assert(buffer);
	assert(bufferSize);
	assert(stream);

	LogDebug() << "[AudioEncoderContext:" << this << "] Input audio frame:\n" 
		<< "Frame Size: " << bufferSize << "\n"
		<< "Buffer Size: " << this->bufferSize << "\n"
		//<< "Duration: " << frameDuration << "\n" 
		<< endl;
		*/

	/*
	LogDebug() << "[AudioEncoderContext:" << this << "] Encoded audio frame:" 
		<< "\n\tFrame Size: " << size << "\n"
		<< "\n\tCoded Frame: " << ctx->coded_frame << "\n"
		<< "\n\tKey Frame: " << (packet->flags & AV_PKT_FLAG_KEY) << "\n"
		<< endl;
		*/

	/*
	av_fifo_generic_write(_audioFifo, (UInt8 *)buffer, bufferSize, NULL);
	while (av_fifo_size(_audioFifo) >= _audioOutSize) 
	{
		av_fifo_generic_read(_audioFifo, _audioFifoOutBuffer, _audioOutSize, NULL);

 		// Encode a frame of AudioOutSize bytes
		int size = avcodec_encode_audio(ctx, this->buffer, _audioOutSize, (short*)_audioFifoOutBuffer);
		if (size) {
			AVPacket packet;
			av_init_packet(&packet);
			if (ctx->coded_frame && 
				ctx->coded_frame->pts != AV_NOPTS_VALUE) 
				packet->pts = av_rescale_q(ctx->coded_frame->pts, ctx->time_base, stream->time_base);
			
			if (ctx->coded_frame &&
				ctx->coded_frame->key_frame) 
                packet->flags |= AV_PKT_FLAG_KEY;

            //if (ctx->coded_frame->key_frame)
            //    packet->flags |= PKT_FLAG_KEY;
			//}
			//packet->flags |= AV_PKT_FLAG_KEY;
			packet->stream_index = stream->index;
			packet->data = this->buffer;
			packet->size = size;

	 		// Write the encoded frame to the output file
			int result = av_interleaved_write_frame(oc, &packet);
			if (result != 0) {
				LogError() << "[AudioEncoderContext:" << this << "] Failed to write audio frame." << endl;
				return false;
			}
		} 
		else {
			LogWarn() << "[AudioEncoderContext:" << this << "] Encoded audio frame size is NULL; it may have been buffered." << endl;
			return false;
		}
	}

	return true;
	*/