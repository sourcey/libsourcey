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
AudioContext::AudioContext()
{
	reset();
}
	

AudioContext::~AudioContext()
{	
	close();
}


void AudioContext::reset()
{
	stream = NULL;
	codec = NULL;
		
	bufferSize = 0;
	buffer = NULL;
		
	/*
	bitRate = -1;
	sampleRate = -1;
	bitsPerSample = -1;
	channels = -1;
	*/
	frameNum = 0;

	pts = 0.0;	

	error = "";
}
	

void AudioContext::close()
{
	if (codec)
		avcodec_close(codec);

    if (buffer)
        av_free(buffer);
}


void AudioContext::open()
{
}


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


// ---------------------------------------------------------------------
//
// Audio Encoder Context
//
// ---------------------------------------------------------------------
AudioEncoderContext::AudioEncoderContext()
{
}
	

AudioEncoderContext::~AudioEncoderContext()
{
}


void AudioEncoderContext::open(AVFormatContext *oc) //, const AudioCodec& params
{
	AudioContext::open();
	
	// Find the video encoder
	AVCodec* c = avcodec_find_encoder(oc->oformat->audio_codec); //codec->codec_id
	if (!c)
   		throw Exception("Audio codec encoder not found.");
	
	stream = avformat_new_stream(oc, c);
	if (!stream)
		throw Exception("Failed to initialize the audio stream.");	

    //st->id = 1;
	
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

	// Add an audio stream that uses the format's default video codec 
	// to the format context's streams[] array.
	stream = av_new_stream(oc, 0);
	if (!stream)
		throw Exception("Could not allocate audio stream");
	*/

	// Now we'll setup the parameters of AVCodecContext
	codec = stream->codec;
	avcodec_get_context_defaults3(codec, c);
	codec->codec_id = oc->oformat->audio_codec;
	codec->codec_type = AVMEDIA_TYPE_AUDIO;
	codec->bit_rate = oparams.bitRate;			// 64000	
	codec->sample_rate = oparams.sampleRate;		// 44100
	codec->sample_fmt = AV_SAMPLE_FMT_S16;
	codec->channels = oparams.channels;	 		// 2
	codec->time_base.num = 1;
	codec->time_base.den = oparams.sampleRate; //25; //20; //25; //25; //
	
	//codec->bit_rate = 32000; //oparams.bitRate * 1000 / 25; 
	//codec->bit_rate = oparams.bitRate * 1000 / 25; 
	//codec->time_base.den = 1000;


    // Some formats want stream headers to be separate
    if (oc->oformat->flags & AVFMT_GLOBALHEADER)
        codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
	
	// Open the codec
	if (avcodec_open2(codec, c, NULL) < 0)
		throw Exception("Unable to open the audio codec.");
		
	// NOTE: buffer must be >= AVCODEC_MAX_AUDIO_FRAME_SIZE
	// or some codecs will fail.
    bufferSize = AVCODEC_MAX_AUDIO_FRAME_SIZE; //MAX_AUDIO_PACKET_SIZE;
    buffer = (UInt8*)av_malloc(this->bufferSize);	
	
	/*
	if (codec->channels == 6)
		codec->channel_layout = AV_CH_LAYOUT_5POINT1;

	if (codec->codec_id == CODEC_ID_AAC) {		
		codec->profile = FF_PROFILE_AAC_LOW;		
	}
	else if (codec->codec_id == CODEC_ID_AMR_NB) {
		codec->sample_rate = 8000;
		codec->channels = 1;
	}

	// Find the encoder
	//avcodec_find_encoder_by_name
	AVCodec* c = avcodec_find_encoder(codec->codec_id);
	if (!c)
		throw Exception("Audio codec not found.");	
	
	// Set the frame size
	if (codec->frame_size <= 1) {
		codec->frame_size = MAX_AUDIO_PACKET_SIZE / codec->channels;
		switch (codec->codec_id) {
		case CODEC_ID_PCM_S16LE:
		case CODEC_ID_PCM_S16BE:
		case CODEC_ID_PCM_U16LE:
		case CODEC_ID_PCM_U16BE:
			codec->frame_size >>= 1;
			break;
		default:
			break;
		}
	}

	// Set the encoded output frame size
	//int outSize = av_get_bits_per_sample_format(codec->sample_fmt)/8;
	_audioOutSize = codec->frame_size * 2 * codec->channels;

	// The encoder may require a minimum number of raw audio samples for each encoding but we can't
	// guarantee we'll get this minimum each time an audio frame is decoded from the in file so 
	// we use a FIFO to store up incoming raw samples until we have enough for one call to the codec.
	_audioFifo = av_fifo_alloc(MAX_AUDIO_PACKET_SIZE * 2);

	// Allocate a buffer to read OUT of the FIFO into. The FIFO maintains its own buffer internally.
	if ((_audioFifoOutBuffer = (UInt8*)av_malloc(MAX_AUDIO_PACKET_SIZE * 2)) == 0)
		throw Exception("Can't allocate audio FIFO buffer.");
		*/	
}


void AudioEncoderContext::close()
{
	Log("debug") << "[AudioEncoderContext:" << this << "] Closing Audio" << endl;

	AudioContext::close();	
	
  	//if (packet)
  	//	av_free_packet(packet);

	/*
	if (codec) {
		avcodec_close(codec);
		codec = NULL;
	}

	if (this->buffer) {
		delete this->buffer;
		this->buffer = NULL;
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
}

	
void AudioEncoderContext::reset() 
{
	AudioContext::reset();

	//packet = NULL;
	//frameSize = 0;
}


int AudioEncoderContext::encode(unsigned char* buffer, int bufferSize, AVPacket& opacket)
{	
	/*
	Log("trace") << "[AudioEncoderContext:" << this << "] Encoding Audio Packet" << endl;
	
			Log("trace") << "Encoding Audio Packet:\n"
				<< "\n\CODEC_ID_MP2: " << CODEC_ID_MP2
				<< "\n\CODEC_ID_MP3: " << CODEC_ID_MP3
				<< "\n\CODEC_ID_AAC: " << CODEC_ID_AAC
				<< "\n\CODEC_ID_AC3: " << CODEC_ID_AC3
				<< endl;
				*/

	int len = avcodec_encode_audio(codec, this->buffer, this->bufferSize, (short*)buffer);
    if (len < 0) {
		error = "Encoder error";
		Log("error") << "[AudioEncoderContext:" << this << "] Encoding Audio: Error: " << error << endl;
		return -1;
    }

	av_init_packet(&opacket);
	
	/*
	if (frameNum == 0)
		pts = 0;
	else
		pts += sqrt((double)frameNum);

	//if (codec->coded_frame->pts != AV_NOPTS_VALUE) 
	//	opacket.pts = av_rescale_q(codec->coded_frame->pts, codec->time_base, stream->time_base);
	opacket.pts = pts;
		
		Log("trace") << "[AudioEncoderContext:" << this << "] Encoding AUDIO Packet:\n"
			//<< "\n\AV_NOPTS_VALUE: " << (codec->coded_frame->pts != AV_NOPTS_VALUE)
			//<< "\n\codec->coded_frame: " << codec->coded_frame
			//<< "\n\codec->coded_frame->pts: " << codec->coded_frame->pts
			<< "\n\codec->time_base: " << codec->time_base.den
			<< "\n\codec->time_base num: " << codec->time_base.num
			<< "\n\stream->time_base: " << stream->time_base.den
			<< "\n\stream->time_base num: " << stream->time_base.num
			//<< "\n\opacket.pts: " << opacket.pts
			<< endl;
			*/

	if (codec->coded_frame->key_frame) 
        opacket.flags |= AV_PKT_FLAG_KEY;

	opacket.stream_index = stream->index;
	opacket.data = this->buffer;
	opacket.size = len;
	
	return len;

	/*, 
	//AVPacket packet;
	av_init_packet(&packet);

	if (codec->coded_frame && 
		codec->coded_frame->pts != AV_NOPTS_VALUE) 
		packet->pts = av_rescale_q(codec->coded_frame->pts, codec->time_base, stream->time_base);
			
	if (codec->coded_frame &&
		codec->coded_frame->key_frame) 
        packet->flags |= AV_PKT_FLAG_KEY;

	packet->stream_index = stream->index;
	packet->data = this->buffer;
	packet->size = len;

		AVPacket& outpacket

	 	// Write the encoded frame to the output file
		int result = av_interleaved_write_frame(oc, &packet);
		if (result != 0) {
			Log("error") << "[AudioEncoderContext:" << this << "] Failed to write audio frame." << endl;
			return false;
		}
	} 
	else {
		Log("warn") << "[AudioEncoderContext:" << this << "] Encoded video frame size is NULL; it may have been buffered." << endl;
		return false;
	}

	if (!buffer || !bufferSize) 
		throw Exception("Unable to encode audio frame.");

	if (bufferSize > this->bufferSize) 
		throw Exception("Audio frame too big.");
	
	// Lock the mutex while encoding
	//FastMutex::ScopedLock lock(_mutex);	

	//if (!isReady())
	//	throw Exception("The encoder is not initialized.");
	
	assert(buffer);
	assert(bufferSize);
	assert(stream);

	Log("debug") << "[AudioEncoderContext:" << this << "] Input audio frame:\n" 
		<< "Frame Size: " << bufferSize << "\n"
		<< "Buffer Size: " << this->bufferSize << "\n"
		//<< "Duration: " << frameDuration << "\n" 
		<< endl;
		*/

	/*
	Log("debug") << "[AudioEncoderContext:" << this << "] Encoded audio frame:" 
		<< "\n\tFrame Size: " << size << "\n"
		<< "\n\tCoded Frame: " << codec->coded_frame << "\n"
		<< "\n\tKey Frame: " << (packet->flags & AV_PKT_FLAG_KEY) << "\n"
		<< endl;
		*/

	/*
	av_fifo_generic_write(_audioFifo, (UInt8 *)buffer, bufferSize, NULL);
	while (av_fifo_size(_audioFifo) >= _audioOutSize) 
	{
		av_fifo_generic_read(_audioFifo, _audioFifoOutBuffer, _audioOutSize, NULL);

 		// Encode a frame of AudioOutSize bytes
		int size = avcodec_encode_audio(codec, this->buffer, _audioOutSize, (short*)_audioFifoOutBuffer);
		if (size) {
			AVPacket packet;
			av_init_packet(&packet);
			if (codec->coded_frame && 
				codec->coded_frame->pts != AV_NOPTS_VALUE) 
				packet->pts = av_rescale_q(codec->coded_frame->pts, codec->time_base, stream->time_base);
			
			if (codec->coded_frame &&
				codec->coded_frame->key_frame) 
                packet->flags |= AV_PKT_FLAG_KEY;

            //if (codec->coded_frame->key_frame)
            //    packet->flags |= PKT_FLAG_KEY;
			//}
			//packet->flags |= AV_PKT_FLAG_KEY;
			packet->stream_index = stream->index;
			packet->data = this->buffer;
			packet->size = size;

	 		// Write the encoded frame to the output file
			int result = av_interleaved_write_frame(oc, &packet);
			if (result != 0) {
				Log("error") << "[AudioEncoderContext:" << this << "] Failed to write audio frame." << endl;
				return false;
			}
		} 
		else {
			Log("warn") << "[AudioEncoderContext:" << this << "] Encoded video frame size is NULL; it may have been buffered." << endl;
			return false;
		}
	}

	return true;
	*/
}


// ---------------------------------------------------------------------
//
// Audio Decoder Context
//
// ---------------------------------------------------------------------
AudioDecoderContext::AudioDecoderContext()
{
}
	

AudioDecoderContext::~AudioDecoderContext()
{
}

void AudioDecoderContext::open(AVFormatContext *ic, int streamID)
{
	AudioContext::open();
	
	Log("info") << "[AudioDecoderContext:" << this << "] Opening Audio: " << streamID << endl;

	assert(ic);
	assert(streamID >= 0);
	
    stream = ic->streams[streamID];
    codec = stream->codec;

    AVCodec* c = avcodec_find_decoder(codec->codec_id);
    if (c == NULL)
		throw Exception("The audio codec is missing or unsupported.");

    if (avcodec_open(codec, c) < 0)
		throw Exception("Could not open the video codec.");

    switch (codec->sample_fmt) {
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
	
	// NOTE: buffer must be >= AVCODEC_MAX_AUDIO_FRAME_SIZE
	// or some codecs will fail.
    bufferSize = AVCODEC_MAX_AUDIO_FRAME_SIZE; //MAX_AUDIO_PACKET_SIZE;
    buffer = (UInt8*)av_malloc(bufferSize);
    //packet = (AVPacket*)av_mallocz(sizeof(AVPacket));
    //av_init_packet(packet);
    //offset = 0;
    //return codec;
}


void AudioDecoderContext::close()
{
	AudioContext::close();
}


	
void AudioDecoderContext::reset() 
{
	AudioContext::reset();
	
	duration = 0.0;	
	width = -1;
	fp = false;
}


int AudioDecoderContext::decode(AVPacket& packet)
{
	try 
	{
		if (packet.stream_index != stream->index) {
			Log("error") << "[AudioDecoderContext:" << this << "] Decoding Audio: Error: Wrong stream" << endl;
			return -1;
		}
	
		int outSize = bufferSize;
		int len = avcodec_decode_audio3(codec, (int16_t*)buffer, &outSize, &packet);
		if (len < 0)
			throw Exception("Audio decoder error");

		packet.size -= len;
        packet.data += len;
		
		//Log("trace") << "[AudioDecoderContext:" << this << "] Decoder DTS: " << packet.dts << endl;
		//Log("trace") << "[AudioDecoderContext:" << this << "] Decoder Time Base: " << stream->time_base.den << endl;

		if (packet.dts != AV_NOPTS_VALUE) {
			pts = packet.dts;
			pts *= av_q2d(stream->time_base);
		}

		Log("trace") << "[AudioDecoderContext:" << this << "] Decoder PTS: " << packet.pts << endl;
		Log("trace") << "[AudioDecoderContext:" << this << "] Decoder PTS 1: " << pts << endl;
	
		return outSize;
	} 
	catch (Exception& exc) 
	{
		error = exc.displayText();
		Log("error") << "[AudioDecoderContext:" << this << "] Decoder Error: " << error << endl;
		exc.rethrow();
	}

	return -1;
}

	
} } // namespace Sourcey::Media