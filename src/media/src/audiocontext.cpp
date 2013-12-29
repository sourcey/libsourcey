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

#include "scy/logger.h"


using std::endl;
using namespace scy;


namespace scy {
namespace av {

	
//
//	Base Audio Context
//


AudioContext::AudioContext() :
	stream(nullptr),
	codec(nullptr),
	frame(nullptr),
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
	TraceL << "[AudioContext: " << this << "] Opening" << endl;	
	assert(ctx);
	assert(codec);

	// Open the audio codec
	if (avcodec_open2(ctx, codec, nullptr) < 0)
   		throw std::runtime_error("Cannot open the audio codec.");
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

	if (stream)	{
		stream = nullptr;
		// Note: The stream is managed by the AVFormatContext
		//av_freep(stream);
	}
	
	pts = 0.0;
	error = "";
}


//
// Audio Encoder Context
//


AudioEncoderContext::AudioEncoderContext(AVFormatContext* format) :
	format(format),
	resampler(nullptr),
	inputFrameSize(0),
	outputFrameSize(0)//,
	//buffer(nullptr),
	//bufferSize(0)
{
}
	

AudioEncoderContext::~AudioEncoderContext()
{
	close();
}


void AudioEncoderContext::create()
{
	AudioContext::create();

	TraceLS(this) << "Create" << endl;	
	
	// Find the audio encoder
	codec = avcodec_find_encoder_by_name(oparams.encoder.c_str());
	if (!codec) {
		codec = avcodec_find_encoder(format->oformat->audio_codec);
		if (!codec)
   			throw std::runtime_error("Audio encoder not found: " + oparams.encoder);
	}

	format->oformat->audio_codec = codec->id;

	stream = avformat_new_stream(format, codec);
	if (!stream)
		throw std::runtime_error("Cannot initialize the audio stream.");

	// Now we'll setup the parameters of AVCodecContext
	ctx = stream->codec;
	avcodec_get_context_defaults3(ctx, codec);
	ctx->strict_std_compliance = FF_COMPLIANCE_EXPERIMENTAL;
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
    if (format->oformat->flags & AVFMT_GLOBALHEADER)
        ctx->flags |= CODEC_FLAG_GLOBAL_HEADER;
	
	// Open the codec
	if (avcodec_open2(ctx, codec, nullptr) < 0)
		throw std::runtime_error("Cannot open the audio codec.");

	// Set the frame sizes
    inputFrameSize = ctx->frame_size;
	outputFrameSize = av_samples_get_buffer_size(
		nullptr, ctx->channels, inputFrameSize, ctx->sample_fmt, 1);
		
	// Allocate the encode buffer
	// XXX: Disabling in favor of encoder manged buffer
    //bufferSize = outputFrameSize * 2; //avpicture_get_size(ctx->pix_fmt, ctx->width, ctx->height);
    //buffer = (unsigned char*)av_malloc(outputFrameSize);
			
	// The encoder may require a minimum number of raw audio samples for
	// each encoding but we can't guarantee we'll get this minimum each 
	// time an audio frame is decoded from the in file so 
	// we use a FIFO to store up incoming raw samples until we have enough
	// for one call to the codec.
	//fifo = av_fifo_alloc(outputFrameSize * 2);

	// Allocate a buffer to read OUT of the FIFO into. 
	// The FIFO maintains its own buffer internally.
	//fifoBuffer = (UInt8*)av_malloc(outputFrameSize * 2);

    // Create a resampler if required
    if (ctx->sample_fmt != av_get_sample_fmt(iparams.sampleFmt)) {    
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
	
	/*
	if (fifo) {
		av_fifo_free(fifo);
		fifo = nullptr;
	}
	
	if (fifoBuffer) {
		av_free(fifoBuffer);
		fifoBuffer = nullptr;
	}

	if (buffer) {
		av_free(buffer);
		buffer = nullptr;
	}
	
	// Free the stream
	if (stream && format && format->nb_streams) {		
		for (unsigned int i = 0; i < format->nb_streams; i++) {
			if (format->streams[i] == stream) {		
				TraceLS(this) << "Closing: Removing Stream: " << stream << endl;
				av_freep(&format->streams[i]->codec);
				av_freep(&format->streams[i]);
				stream = nullptr;
				format->nb_streams--;
			}
		}
	}
	*/
}


bool AudioEncoderContext::encode(unsigned char* data, int size, Int64 pts, AVPacket& opacket)
{
	assert(data);
	assert(size);
    AVPacket ipacket;
    av_init_packet(&ipacket);
    ipacket.stream_index = stream->index;
    ipacket.data = data;
    ipacket.size = size;
    ipacket.pts = pts;
	return encode(ipacket, opacket);
}


bool AudioEncoderContext::encode(AVPacket& ipacket, AVPacket& opacket)
{
	TraceLS(this) << "Encoding Audio Packet: " << ipacket.size << endl;

	assert(ipacket.stream_index == stream->index);
	assert(ipacket.size == outputFrameSize);	
	//assert(frame);
	//assert(buffer);
	
	int frameEncoded = 0;		
	UInt8* samples = ipacket.data; // fifoBuffer

	if (resampler) {
		samples = resampler->resample(ipacket.data, ipacket.size);
		if (samples == nullptr) // The resampler may buffer frames
			return false;
	
		assert(resampler->outNbSamples == outputFrameSize);
	}
	
	frame = avcodec_alloc_frame();
	avcodec_get_frame_defaults(frame);
	frame->nb_samples = inputFrameSize;
	frame->pts = ipacket.pts;

	// BUG: Libspeex encoding does not increment ctx->frame_number.
	// Might need to create local frame number value for PTS.
	// frame->pts = ctx->frame_number; // Force a PTS value, no AV_NOPTS_VALUE
	if (avcodec_fill_audio_frame(frame, ctx->channels, ctx->sample_fmt, samples, outputFrameSize, 0) < 0) {
		error = "Error filling audio frame";
		ErrorLS(this) << error << endl;
		throw std::runtime_error(error);
	}
	
	av_init_packet(&opacket);
	opacket.data = nullptr;
	opacket.size = 0;
	opacket.pts = ctx->frame_number;

	if (avcodec_encode_audio2(ctx, &opacket, frame, &frameEncoded) < 0) {
		// TODO: Use av_strerror
		error = "Fatal encoder error";
		ErrorLS(this) << error << endl;
		throw std::runtime_error(error);
	}
		
	if (frameEncoded) {	
		fps.tick();
		opacket.stream_index = stream->index;	
		opacket.flags |= AV_PKT_FLAG_KEY;
		if (opacket.pts != AV_NOPTS_VALUE)
			opacket.pts      = av_rescale_q(opacket.pts,      ctx->time_base, stream->time_base);
		if (opacket.dts != AV_NOPTS_VALUE)
			opacket.dts      = av_rescale_q(opacket.dts,      ctx->time_base, stream->time_base);
		if (opacket.duration > 0)
			opacket.duration = (int)av_rescale_q(opacket.duration, ctx->time_base, stream->time_base);
		
		/*
		TraceLS(this) << "Encoded PTS:\n" 
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
			*/
	}
	else
		DebugLS(this) << "No frame encoded" << endl;	

	return frameEncoded > 0;
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
   		throw std::runtime_error("Audio decoder not found.");

    if (avcodec_open2(ctx, codec, NULL) < 0)
		throw std::runtime_error("Could not open the audio codec.");
	
	frame = avcodec_alloc_frame();
	if (!frame)
		throw std::runtime_error("Cannot allocate input frame.");
}


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

	int frameDecoded = 0;
	int bytesDecoded = 0;
	int bytesRemaining = ipacket.size;
	
	av_init_packet(&opacket);
	opacket.data = nullptr;
	opacket.size = 0;
	
	avcodec_get_frame_defaults(frame);
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
	// 	 int ret = avcodec_decode_video2(..., ipacket);
	// 	 if(ret < -1)
	//		throw std::runtime_error("error");
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
			ErrorLS(this) << "Decoder Error" << endl;
			error = "Decoder error";
			throw std::runtime_error(error);
		}

		bytesRemaining -= bytesDecoded;
	}
	*/

	if (frameDecoded) {	
		fps.tick();
		initDecodedAudioPacket(stream, ctx, frame, &opacket, &pts);

		/*
		TraceLS(this) << "Decoded Frame:" 
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
	ipacket.data = nullptr;
	ipacket.size = 0;

	av_init_packet(&opacket);
	opacket.data = nullptr;
	opacket.size = 0;
	
	int frameDecoded = 0;
	avcodec_decode_video2(ctx, frame, &frameDecoded, &ipacket);

	if (frameDecoded) {
		initDecodedAudioPacket(stream, ctx, frame, &opacket, &pts);
		TraceLS(this) << "Flushed Audio Frame: " << opacket.pts << endl;
		return true;
	}
	return false;
}


//
// Audio Resampler
//


AudioResampler::AudioResampler() :
	ctx(nullptr),
	outBuffer(nullptr),
	outNbSamples(0)
{
}
	

AudioResampler::~AudioResampler()
{	
	free();
}


void AudioResampler::create(const AudioCodec& iparams, const AudioCodec& oparams)
{
	TraceLS(this) << "Create:" 
		<< "\n\tInput Sample Rate: " << iparams.sampleRate
		<< "\n\tOutput Sample Rate:: " << oparams.sampleRate
		<< endl;

    if (ctx)
        throw std::runtime_error("Resample context already initialized");

	Int64 outChLayout = av_get_default_channel_layout(oparams.channels);
	enum AVSampleFormat outSampleFmt = av_get_sample_fmt(oparams.sampleFmt);

    Int64 inChLayout  = av_get_default_channel_layout(iparams.channels);
    enum AVSampleFormat inSampleFmt  = av_get_sample_fmt(iparams.sampleFmt);

    ctx = swr_alloc_set_opts(ctx,
            outChLayout, outSampleFmt, oparams.sampleRate,
            inChLayout,  inSampleFmt,  iparams.sampleRate,
            0, nullptr);
    if (ctx == nullptr)
        throw std::runtime_error("Cannot configure resampler context");

	char inChBuf[128], outChBuf[128];
	av_get_channel_layout_string(inChBuf,  sizeof(inChBuf),  -1, inChLayout);
	av_get_channel_layout_string(outChBuf, sizeof(outChBuf), -1, outChLayout);

	/*
	TraceLS(this) << "Resampler Options:\n" 
		<< "\n\tIn Channel Layout: " << inChBuf
		<< "\n\tIn Sample Rate: " << iparams.sampleRate
		<< "\n\tIn Sample Fmt: " << av_get_sample_fmt_name(inSampleFmt)
		<< "\n\tOut Channel Layout: " << outChBuf
		<< "\n\tOut Sample Rate: " << oparams.sampleRate
		<< "\n\tOut Sample Fmt: " << av_get_sample_fmt_name(outSampleFmt)
		<< endl; 
		*/

    if (ctx == nullptr) {
        throw std::runtime_error("Cannot create resampler context");
    }

    if (swr_init(ctx)) {
        throw std::runtime_error("Cannot initialise resampler");
    }

	this->iparams = iparams;
	this->oparams = oparams;
	
	TraceLS(this) << "Create: OK" << endl;
}
	

void AudioResampler::free()
{
	TraceLS(this) << "Closing" << endl;

	//if (oframe) {
	//	av_free(oframe);
	//	oframe = nullptr;
	//}
	
	if (ctx) {
		swr_free(&ctx);
		ctx = nullptr;
	}

	if (outBuffer) {
		av_freep(&outBuffer);
		outBuffer = nullptr;
	}

	TraceLS(this) << "Closing: OK" << endl;
}


UInt8* AudioResampler::resample(UInt8* inSamples, int inNbSamples)
{
    if (!ctx)
        throw std::runtime_error("Conversion context must be initialized.");	

	outNbSamples = av_rescale_rnd(
		swr_get_delay(ctx, (int64_t)iparams.sampleRate) + (int64_t)inNbSamples, 
		(int64_t)oparams.sampleRate, (int64_t)iparams.sampleRate, AV_ROUND_UP);

	if (outBuffer) {
		av_freep(&outBuffer);
		outBuffer = nullptr;
	}

	/*
	TraceLS(this) << "Resampling:\n" 
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
	
	av_samples_alloc(&outBuffer, nullptr, oparams.channels, outNbSamples,
                       av_get_sample_fmt(oparams.sampleFmt), 0);
	
    outNbSamples = swr_convert(ctx, &this->outBuffer, outNbSamples,
             (const UInt8**)&inSamples, inNbSamples);
    if (outNbSamples < 0)
        throw std::runtime_error("Cannot resample audio");

	assert(outNbSamples == inNbSamples);

	return outBuffer;
}


//
// Helpers functions
//	


void initDecodedAudioPacket(const AVStream* stream, const AVCodecContext* ctx, const AVFrame* frame, AVPacket* opacket, double* pts)
{		
	opacket->data = frame->data[0];
	opacket->size = av_samples_get_buffer_size(nullptr, ctx->channels, frame->nb_samples, ctx->sample_fmt, 1);		
	opacket->dts = frame->pkt_dts; // Decoder PTS values may be out of sequence
	opacket->pts = frame->pkt_pts;	
		
	// Local PTS value represented as decimal seconds
    if (opacket->dts != AV_NOPTS_VALUE) {
		*pts = (double)opacket->pts;
		*pts *= av_q2d(stream->time_base);
	}

	assert(opacket->data);
	assert(opacket->size);
	assert(opacket->dts >= 0);
	assert(opacket->pts >= 0);	
}

	
} } // namespace scy::av


#endif
