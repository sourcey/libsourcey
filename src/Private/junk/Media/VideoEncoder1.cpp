#include "Sourcey/Media/VideoEncoder.h"


using namespace std;
using namespace Poco;
using namespace Sourcey;


namespace Sourcey {
namespace Media {


// ============================================================================
//
// Video Encoder
//
// ============================================================================
FastMutex VideoEncoder::_mutex;


VideoEncoder::VideoEncoder(/*const VideoParams& oParams, const VideoParams& iParams*/) : 
	_oFrame(avcodec_alloc_frame()), _iFrame(avcodec_alloc_frame()),
	//_oParams(oParams), _iParams(iParams),
	_encoderContext(0), _resizeContext(0), _now(0) 
{	
	cout << "VideoEncoder::VideoEncoder()" << endl;
	// Lock our mutex during avcodec_init
	FastMutex::ScopedLock lock(_mutex);
	avcodec_init();
	avcodec_register_all();
}


VideoEncoder::~VideoEncoder() 
{
	cout << "VideoEncoder::~VideoEncoder()" << endl;
    if (_encoderContext) {
		// Lock our mutex during avcodec_close
		FastMutex::ScopedLock lock(_mutex);
        avcodec_close(_encoderContext);
        av_free(_encoderContext);
    }

    if (_iFrame)
        av_free(_iFrame);

    if (_oFrame)
        av_free(_oFrame);

    if (_resizeContext)
        sws_freeContext(_resizeContext);
}


bool VideoEncoder::isInitialized()
{
	return _encoderContext != NULL 
		&& _resizeContext != NULL; 
}


void VideoEncoder::initEncodeContext(const VideoParams& oParams) 
{
    if (_encoderContext)
        throw Exception("Encoder context already initialized.");

	_oParams = oParams;

    AVCodecContext *oContext = avcodec_alloc_context();
    if (!oContext)
        throw Exception("Unable to allocate encoder context.");

    oContext->codec_type = CODEC_TYPE_VIDEO;
    oContext->width = _oParams.width;
    oContext->height = _oParams.height;
    oContext->gop_size = 12;
    oContext->pix_fmt = static_cast<PixelFormat>(_oParams.pixfmt);	    	
    oContext->time_base.den = _oParams.fps;
    oContext->time_base.num = 1;
	//oContext->bit_rate = 400000;	
    oContext->bit_rate = _oParams.bitRate;
    oContext->has_b_frames = 0;
    oContext->max_b_frames = 0;
    oContext->me_method = 1;

	// HACK: High quality MJPEG streams
	if (_oParams.codec == MJPEG) {
		oContext->flags |= CODEC_FLAG_QSCALE;
		oContext->global_quality = 1;
		oContext->pix_fmt = static_cast<PixelFormat>(Media::YUVJ420P);	  
	}

    AVCodec *oCodec = avcodec_find_encoder(static_cast<CodecID>(_oParams.codec));
    if (avcodec_open(oContext, oCodec) < 0)
        throw Exception("Unable to open output codec.");

    _encoderContext = oContext;
}


void VideoEncoder::resetEncodeContext()
{
    av_free(_encoderContext);
    _encoderContext = 0;
}


void VideoEncoder::initScaleContext(const VideoParams& iParams)
{
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
									 /* SWS_FAST_BILINEAR */SWS_BICUBIC, NULL, NULL, NULL);
    if (!_resizeContext) 
        throw Exception("Invalid conversion context.");
}



void VideoEncoder::resetScaleContext()
{
    //av_free(_resizeContext);
	sws_freeContext(_resizeContext);
    _resizeContext = 0;
}


int VideoEncoder::encode(unsigned char *input, unsigned int inputSize, unsigned char *output, unsigned int outputSize)
{
    if (!input)
        return 0;

    //input->pts = AV_NOPTS_VALUE;
    _now += 1.0/_oParams.fps;

	// This is a hack to speed the things up a bit. We just assign the pointer data
	// to _iFrame->data[0] without using a memcpy.
	//memcpy(_iFrame->data[0], (UInt8*)data, size);
	_iFrame->data[0] = (UInt8*)input;

	if (_resizeContext && sws_scale(
		_resizeContext, 
		_iFrame->data, _iFrame->linesize, 0, _iParams.height,
		_oFrame->data, _oFrame->linesize) < 0) 
		//throw Exception("AVWriter: Pixel format conversion not supported.");
		return 0;

    return avcodec_encode_video(_encoderContext, output, outputSize, _oFrame);
}


AVFrame* VideoEncoder::MakeBlackFrame(VideoPixelFormat pixfmt, int width, int height)
{
    AVFrame* f = avcodec_alloc_frame();
    unsigned long size = avpicture_get_size(static_cast<PixelFormat>(pixfmt), width, height);
    UInt8 *buffer = (UInt8*)av_malloc(size*sizeof(UInt8));

    memset(buffer, 0, size);
    avpicture_fill((AVPicture *)f, buffer, static_cast<PixelFormat>(pixfmt), width, height);

    return f;
}


void VideoEncoder::FreeFrame(AVFrame* f)
{
    av_free(f->data[0]);
    av_free(f);
}


} } // namespace Sourcey::Media