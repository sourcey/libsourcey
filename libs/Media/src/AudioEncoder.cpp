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


#include "Sourcey/Media/AudioEncoder.h"
#include "Sourcey/Logger.h"

#include "assert.h"


using namespace std;
using namespace Poco;
using namespace Sourcey;


namespace Sourcey {
namespace Media {


FastMutex AudioEncoder::_mutex;


AudioEncoder::AudioEncoder(const EncoderParams& params) : 
	_params(params),
	_encoderContext(NULL),
	_now(0)
{	
	Log("trace") << "[AudioEncoder" << this << "] Creating" << endl;
}


AudioEncoder::~AudioEncoder()
{
	Log("trace") << "[AudioEncoder" << this << "] Destroying" << endl;
	
	setState(this, EncoderState::Stopped);
    cleanup();
}


bool AudioEncoder::isReady()
{
	return stateEquals(EncoderState::Ready) 
		&& _encoderContext != NULL; 
}


void AudioEncoder::initialize()
{
	assert(!isReady());

    try {
		{
			// Lock our mutex during avcodec_init
			FastMutex::ScopedLock lock(_mutex);
			//avcodec_init();
			//avcodec_register_all();
		}

		if (_encoderContext)
			throw Exception("Encoder context already initialized.");

		AVCodecContext *ocontext = avcodec_alloc_context();
		if (!ocontext)
			throw Exception("Unable to allocate encoder context.");	

		// Now we'll setup the parameters of AVCodecContext
		avcodec_get_context_defaults2(ocontext, AVMEDIA_TYPE_AUDIO);
		ocontext->codec_id = static_cast<CodecID>(_params.oformat.audio.id); //_formatCtx->oformat->audio_codec;
		ocontext->codec_type = AVMEDIA_TYPE_AUDIO;
		ocontext->bit_rate = _params.oformat.audio.bitRate;				// 64000
		if (ocontext->codec_id == CODEC_ID_AMR_NB) {
			ocontext->sample_rate = 8000;
			ocontext->channels = 1;
		} else {
			ocontext->sample_rate = _params.oformat.audio.sampleRate;	// 44100
			ocontext->channels = _params.oformat.audio.channels;	 		// 2
		}

		AVCodec *oCodec = avcodec_find_encoder(static_cast<CodecID>(_params.oformat.audio.id));
		if (avcodec_open(ocontext, oCodec) < 0)
			throw Exception("Unable to open output audio codec.");

		// Set the frame size
		if (ocontext->frame_size <= 1) {
			ocontext->frame_size = MAX_AUDIO_PACKET_SIZE / ocontext->channels;
			switch(ocontext->codec_id) {
			case CODEC_ID_PCM_S16LE:
			case CODEC_ID_PCM_S16BE:
			case CODEC_ID_PCM_U16LE:
			case CODEC_ID_PCM_U16BE:
				ocontext->frame_size >>= 1;
				break;
			default:
				break;
			}
		} else {
			ocontext->frame_size = ocontext->frame_size;
		}

		// Set the encoded output frame size
		//int outSize = av_get_bits_per_sample_format(ocontext->sample_fmt)/8;
		_outSize = ocontext->frame_size * 2 * ocontext->channels;

		// Can we get away with a buffer of AudioOutSize bytes??
		_outBuffer = new UInt8[MAX_AUDIO_PACKET_SIZE];

		// The encoder may require a minimum number of raw audio samples for each encoding but we can't
		// guarantee we'll get this minimum each time an audio frame is decoded from the in file so 
		// we use a FIFO to store up incoming raw samples until we have enough for one call to the codec.
		_fifo = av_fifo_alloc(2 * MAX_AUDIO_PACKET_SIZE);

		// Allocate a buffer to read OUT of the FIFO into. The FIFO maintains its own buffer internally.
		if ((_fifoBuffer = (UInt8*)av_malloc(2 * MAX_AUDIO_PACKET_SIZE)) == 0)
			throw Exception("AVWriter: Can't allocate buffer to read into from audio FIFO.");

		_encoderContext = ocontext;

		setState(this, EncoderState::Ready);		
    }
    catch (Exception& exc) {	
		Log("error") << "[AudioEncoder" << this << "]" << exc.displayText() << endl;

		cleanup();
		setStateMessage(exc.displayText());
		setState(this, EncoderState::Failed);	
		exc.rethrow();
    }
}


void AudioEncoder::uninitialize()
{
    cleanup();
	setState(this, EncoderState::None);
}


void AudioEncoder::cleanup()
{
	if (_fifoBuffer) {
		av_free(_fifoBuffer);
		_fifoBuffer = NULL;
	}
	if (_fifo) {
		av_fifo_free(_fifo);
		_fifo = NULL;
	}
    if (_encoderContext) {
		// Lock our mutex during avcodec_close
		FastMutex::ScopedLock lock(_mutex);
        avcodec_close(_encoderContext);
        av_free(_encoderContext);
		_encoderContext = NULL;
   }
}

	
bool AudioEncoder::accepts(IPacket& packet) 
{ 
	return dynamic_cast<AudioPacket*>(&packet) != 0; 
}


void AudioEncoder::process(IPacket& packet)
{ 
	AudioPacket* audioPacket = dynamic_cast<AudioPacket*>(&packet);
	if (audioPacket) {		
		encode(audioPacket->data(), audioPacket->size());
	}
}


int AudioEncoder::encode(unsigned char *input, int inputSize)
{
	assert(stateEquals(EncoderState::Ready));
	assert(input);
	assert(inputSize);

    if (!input || !inputSize)
        return 0;

	if (inputSize > MAX_AUDIO_PACKET_SIZE) 
		throw Exception("Audio frame is too big.");

	// TODO: PTS time tracking
    //_now += 1.0/_params.oformat.audio.fps;

	int size = -1;
	av_fifo_generic_write(_fifo, (UInt8*)input, inputSize, NULL);
	while (av_fifo_size(_fifo) >= _outSize// && 
		//bytesEncoded + _outSize < outputSize &&
		//bytesEncoded >= 0
		) 
	{
		av_fifo_generic_read(_fifo, _fifoBuffer, _outSize, NULL);

 		// Encode a frame of AudioOutSize bytes
		size = avcodec_encode_audio(_encoderContext, _outBuffer, MAX_AUDIO_PACKET_SIZE, (const short*)_fifoBuffer);

		//AudioPacket(_outBuffer, size);
		//packet.data = _outBuffer;
		//packet.size = size;
		//packet.time = time(0); // Set to pts?
		AudioPacket packet(_outBuffer, size);
		dispatch(this, packet);
	}

	return size;
}


} } // namespace Sourcey::Media
