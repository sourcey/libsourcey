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

	// Initializes the audio buffers
	//this->buffer = new unsigned char[MAX_AUDIO_PACKET_SIZE];

	// Add an audio stream that uses the format's default video codec 
	// to the format context's streams[] array.
	this->stream = av_new_stream(oc, 0);
	if (!stream)
		throw Exception("Could not allocate audio stream");

	// Now we'll setup the parameters of AVCodecContext
	codec = stream->codec;
	avcodec_get_context_defaults2(codec, AVMEDIA_TYPE_AUDIO);
	codec->codec_id = oc->oformat->audio_codec;
	codec->codec_type = AVMEDIA_TYPE_AUDIO;
	codec->bit_rate = oparams.bitRate;			// 64000
	codec->sample_rate = oparams.sampleRate;		// 44100
	codec->sample_fmt = AV_SAMPLE_FMT_S16;	
	codec->channels = oparams.channels;	 		// 2
	codec->time_base.num = 1;
	codec->time_base.den = oparams.sampleRate;

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

	// Some formats want stream headers to be separate
	if (oc->oformat->flags & AVFMT_GLOBALHEADER) {
		codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
	}
	*/

	// Find the encoder
	AVCodec* c = avcodec_find_encoder(codec->codec_id);
	if (!c)
		throw Exception("Audio codec not found.");

	if (avcodec_open(codec, c) < 0)
		throw Exception("Unable to open the audio codec.");

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

	/*
	// Set the encoded output frame size
	//int outSize = av_get_bits_per_sample_format(codec->sample_fmt)/8;
	_audioFrameSize = codec->frame_size * codec->channels * 2;

	// The encoder may require a minimum number of raw audio samples for each encoding but we can't
	// guarantee we'll get this minimum each time an audio frame is decoded from the in file so 
	// we use a FIFO to store up incoming raw samples until we have enough for one call to the codec.
	_audioFifo = av_fifo_alloc(MAX_AUDIO_PACKET_SIZE * 2);

	// Allocate a buffer to read OUT of the FIFO into. The FIFO maintains its own buffer internally.
	if ((_audioFifoOutBuffer = (UInt8*)av_malloc(MAX_AUDIO_PACKET_SIZE * 2)) == 0)
		throw Exception("Can't allocate audio FIFO buffer.");
		*/
	
	// NOTE: buffer must be >= AVCODEC_MAX_AUDIO_FRAME_SIZE
	// or some codecs will fail.
    this->bufferSize = AVCODEC_MAX_AUDIO_FRAME_SIZE; //MAX_AUDIO_PACKET_SIZE;
    this->buffer = (UInt8*)av_malloc(this->bufferSize);		
    //this->packet = (AVPacket*)av_mallocz(sizeof(AVPacket));
    //av_init_packet(this->packet);
}


void AudioEncoderContext::close()
{
	Log("debug") << "[AudioEncoderContext" << this << "] Closing Audio" << endl;

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
	int len = avcodec_encode_audio(codec, this->buffer, this->bufferSize, (short*)buffer);
    if (len < 0) {
		error = "Encoder error";
		Log("error") << "[AudioEncoderContext:" << this << "] Encoding Audio: Error: " << error << endl;
		return -1;
    }

	av_init_packet(&opacket);

	if (codec->coded_frame && 
		codec->coded_frame->pts != AV_NOPTS_VALUE) 
		opacket.pts = av_rescale_q(codec->coded_frame->pts, codec->time_base, stream->time_base);
			
	if (codec->coded_frame &&
		codec->coded_frame->key_frame) 
        opacket.flags |= AV_PKT_FLAG_KEY;

	opacket.stream_index = stream->index;
	opacket.data = this->buffer;
	
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
			Log("error") << "[AudioEncoderContext" << this << "] Failed to write audio frame." << endl;
			return false;
		}
	} 
	else {
		Log("warn") << "[AudioEncoderContext" << this << "] Encoded video frame size is NULL; it may have been buffered." << endl;
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

	Log("debug") << "[AudioEncoderContext" << this << "] Input audio frame:\n" 
		<< "Frame Size: " << bufferSize << "\n"
		<< "Buffer Size: " << this->bufferSize << "\n"
		//<< "Duration: " << frameDuration << "\n" 
		<< endl;
		*/

	/*
	Log("debug") << "[AudioEncoderContext" << this << "] Encoded audio frame:" 
		<< "\n\tFrame Size: " << size << "\n"
		<< "\n\tCoded Frame: " << codec->coded_frame << "\n"
		<< "\n\tKey Frame: " << (packet->flags & AV_PKT_FLAG_KEY) << "\n"
		<< endl;
		*/

	/*
	av_fifo_generic_write(_audioFifo, (UInt8 *)buffer, bufferSize, NULL);
	while (av_fifo_size(_audioFifo) >= _audioFrameSize) 
	{
		av_fifo_generic_read(_audioFifo, _audioFifoOutBuffer, _audioFrameSize, NULL);

 		// Encode a frame of AudioOutSize bytes
		int size = avcodec_encode_audio(codec, this->buffer, _audioFrameSize, (short*)_audioFifoOutBuffer);
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
				Log("error") << "[AudioEncoderContext" << this << "] Failed to write audio frame." << endl;
				return false;
			}
		} 
		else {
			Log("warn") << "[AudioEncoderContext" << this << "] Encoded video frame size is NULL; it may have been buffered." << endl;
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
	
	Log("info") << "[AVFileReader:" << this << "] Opening Audio: " << streamID << endl;

	assert(ic);
	assert(streamID >= 0);
	
    stream = ic->streams[streamID];
    codec = stream->codec;

    AVCodec* c = avcodec_find_decoder (codec->codec_id);
    if (c == NULL)
		throw Exception("The audio codec is missing or unsupported.");

    if (avcodec_open(codec, c) < 0)
		throw Exception("Could not open the video codec.");

    switch (codec->sample_fmt) {
    case SAMPLE_FMT_S16:
        width = 16;
        fp = false;
        break;
    case SAMPLE_FMT_S32:
        width = 32;
        fp = false;
        break;
    case SAMPLE_FMT_FLT:
        width = 32;
        fp = true;
        break;
    case SAMPLE_FMT_DBL:
        width = 64;
        fp = true;
        break;
    default:
		throw Exception("Unsupported audio sample format.");
    }
	
	// NOTE: buffer must be >= AVCODEC_MAX_AUDIO_FRAME_SIZE
	// or some codecs will fail.
    bufferSize = AVCODEC_MAX_AUDIO_FRAME_SIZE; //MAX_AUDIO_PACKET_SIZE;
    buffer = (uint8_t*)av_malloc(bufferSize);
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
			Log("error") << "[AVFileReader:" << this << "] Decoding Audio: Error: Wrong stream" << endl;
			return -1;
		}
	
		int outSize = bufferSize;
		int len = avcodec_decode_audio3(codec, (int16_t*)buffer, &outSize, &packet);
		if (len < 0)
			throw Exception("Audio decoder error");

		packet.size -= len;
        packet.data += len;

		if (packet.dts != AV_NOPTS_VALUE) {
			pts = packet.dts;
			pts *= av_q2d(stream->time_base);
		}
	
		return outSize;
	} 
	catch (Exception& exc) 
	{
		error = exc.displayText();
		Log("error") << "[AVFileReader:" << this << "] Decoder Error: " << error << endl;
		exc.rethrow();
	}

	return -1;
}

	
} } // namespace Sourcey::Media