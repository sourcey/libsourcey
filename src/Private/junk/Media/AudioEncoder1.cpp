#include "Sourcey/Media/AudioEncoder.h"

#include "assert.h"


using namespace std;
using namespace Poco;
using namespace Sourcey;


namespace Sourcey {
namespace Media {


// ============================================================================
//
// Audio Encoder
//
// ============================================================================
FastMutex AudioEncoder::_mutex;


AudioEncoder::AudioEncoder() : 
	_encoderContext(0),
	_now(0)
{	
	cout << "AudioEncoder::AudioEncoder()" << endl;
	{
		// Lock our mutex during avcodec_init
		FastMutex::ScopedLock lock(_mutex);
		avcodec_init();
		avcodec_register_all();
	}
}


AudioEncoder::~AudioEncoder()
{
	cout << "AudioEncoder::~AudioEncoder()" << endl;
	if (_fifoBuffer)	av_free(_fifoBuffer);
	if (_fifo)			av_fifo_free(_fifo);
    if (_encoderContext) {
		// Lock our mutex during avcodec_close
		FastMutex::ScopedLock lock(_mutex);
        avcodec_close(_encoderContext);
        av_free(_encoderContext);
   }

    //if (_iFrame)
    //    av_free(_iFrame);

    //if (_oFrame)
    //    av_free(_oFrame);

    //if (_resizeContext)
    //    sws_freeContext(_resizeContext);
}


bool AudioEncoder::isInitialized()
{
	return _encoderContext != NULL 
		&& _resizeContext != NULL; 
}


void AudioEncoder::initEncodeContext(const AudioParams& oParams) 
{
    if (_encoderContext)
        throw Exception("Encoder context already initialized.");

	_oParams = oParams;

    AVCodecContext *ocontext = avcodec_alloc_context();
    if (!ocontext)
        throw Exception("Unable to allocate encoder context.");	

	// Now we'll setup the parameters of AVCodecContext
	avcodec_get_context_defaults2(ocontext, CODEC_TYPE_AUDIO);
	ocontext->codec_id = static_cast<CodecID>(_oParams.codec); //_formatCtx->oformat->audio_codec;
	ocontext->codec_type = CODEC_TYPE_AUDIO;
	ocontext->bit_rate = _oParams.bitRate;					// 64000
	if (ocontext->codec_id == CODEC_ID_AMR_NB) {
		ocontext->sample_rate = 8000;
		ocontext->channels = 1;
	} else {
		ocontext->sample_rate = _oParams.sampleRate;	// 44100
		ocontext->channels = _oParams.channels;	 	// 2
	}

	// Find the encoder
	//_audioCodec = avcodec_find_encoder(ocontext->codec_id);
	//if (!_audioCodec)
	//	throw Exception("AVWriter: Audio codec not found.");

	// Open the codec
	//if (avcodec_open(ocontext, _audioCodec) < 0)
	//	throw Exception("AVWriter: Unable to open the audio codec.");

    AVCodec *oCodec = avcodec_find_encoder(static_cast<CodecID>(_oParams.codec));
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
	int outSize = 2;
	_audioOutSize = ocontext->frame_size * outSize * ocontext->channels;

	// The encoder may require a minimum number of raw audio samples for each encoding but we can't
	// guarantee we'll get this minimum each time an audio frame is decoded from the in file so 
	// we use a FIFO to store up incoming raw samples until we have enough for one call to the codec.
	_fifo = av_fifo_alloc(2 * MAX_AUDIO_PACKET_SIZE);

	// Allocate a buffer to read OUT of the FIFO into. The FIFO maintains its own buffer internally.
	if ((_fifoBuffer = (uint8_t*)av_malloc(2 * MAX_AUDIO_PACKET_SIZE)) == 0)
		throw Exception("AVWriter: Can't allocate buffer to read into from audio FIFO.");

	/*
    AVCodecContext *ocontext = avcodec_alloc_context();
    if (!ocontext)
        throw Exception("Unable to allocate encoder context.");

    ocontext->codec_type = CODEC_TYPE_AUDIO;
    ocontext->width = _oParams.width;
    ocontext->height = _oParams.height;
    ocontext->gop_size = 12;
    ocontext->pix_fmt = static_cast<PixelFormat>(_oParams.pixfmt);	    	
    ocontext->time_base.den = _oParams.fps;
    ocontext->time_base.num = 1;
	//ocontext->bit_rate = 400000;	
    ocontext->bit_rate = _oParams.bitRate;
    ocontext->has_b_frames = 0;
    ocontext->max_b_frames = 0;
    ocontext->me_method = 1;

	// HACK: High quality MJPEG streams
	if (_oParams.codec == MJPEG) {
		ocontext->flags |= CODEC_FLAG_QSCALE;
		ocontext->global_quality = 1;
	}

    AVCodec *oCodec = avcodec_find_encoder(static_cast<CodecID>(_oParams.codec));
    if (avcodec_open(ocontext, oCodec) < 0)
        throw Exception("Unable to open output codec.");
		*/

    _encoderContext = ocontext;
}


void AudioEncoder::resetEncodeContext()
{
    av_free(_encoderContext);
    _encoderContext = NULL;
}


/*
void AudioEncoder::initScaleContext(const AudioParams& iParams)
{
	cout << "AudioEncoder::initScaleContext()" << endl;

    if (!_encoderContext)
        throw Exception("Encoder context must be initialized before the conversion context.");

    if (_resizeContext)
        throw Exception("Conversion context already initialized.");

	_iParams = iParams;

    avpicture_alloc(reinterpret_cast<AVPicture*>(_iFrame), 
		static_cast<PixelFormat>(_iParams.pixfmt), _iParams.width, _iParams.height);
    avpicture_alloc(reinterpret_cast<AVPicture*>(_oFrame), 
		_encoderContext->pix_fmt, _encoderContext->width, _encoderContext->height);
	_resizeContext = sws_getContext(_iParams.width, _iParams.height, static_cast<PixelFormat>(_iParams.pixfmt),
                                     _encoderContext->width, _encoderContext->height, _encoderContext->pix_fmt, 
									 SWS_BICUBIC, NULL, NULL, NULL);
    if (!_resizeContext) 
        throw Exception("Invalid conversion context.");
}



void AudioEncoder::resetScaleContext()
{
    //av_free(_resizeContext);
	sws_freeContext(_resizeContext);
    _resizeContext = 0;
}
*/


int AudioEncoder::encode(unsigned char *input, unsigned int inputSize, unsigned char *output, unsigned int outputSize)
{
	assert(input);
	assert(inputSize);
	//assert(inputSize == _audioOutSize);
	//assert(_isInitialized);

    if (!input && !inputSize)
        return 0;

	if (inputSize > MAX_AUDIO_PACKET_SIZE) 
		throw Exception("Audio frame is too big.");

	// TODO: PTS time tracking
    //_now += 1.0/_oParams.fps;

	int bytesEncoded = 0;
	av_fifo_generic_write(_fifo, (uint8_t *)input, inputSize, NULL);
	while (av_fifo_size(_fifo) >= _audioOutSize && 
		bytesEncoded + _audioOutSize < outputSize &&
		bytesEncoded >= 0) 
	{
		av_fifo_generic_read(_fifo, _fifoBuffer, _audioOutSize, NULL);

 		// Encode a frame of AudioOutSize bytes
		bytesEncoded += avcodec_encode_audio(_encoderContext, output + bytesEncoded, outputSize - bytesEncoded, (short*)_fifoBuffer);
	}

	return bytesEncoded;
}


} } // namespace Sourcey::Media