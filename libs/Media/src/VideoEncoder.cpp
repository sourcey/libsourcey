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


#include "Sourcey/Media/VideoEncoder.h"
#include "Sourcey/Logger.h"


using namespace std;
using namespace Poco;
using namespace Sourcey;


namespace Sourcey {
namespace Media {


FastMutex VideoEncoder::_mutex;


VideoEncoder::VideoEncoder(const EncoderParams& params) : 
	_params(params), _oFrame(avcodec_alloc_frame()), _iFrame(avcodec_alloc_frame()),
	_outBuffer(new unsigned char[MAX_VIDEO_PACKET_SIZE]),
	_encoderContext(0), _resizeContext(0), _now(0) 
{	
	Log("debug") << "[VideoEncoder" << this << "] Creating" << endl;
}


VideoEncoder::~VideoEncoder() 
{
	Log("debug") << "[VideoEncoder" << this << "] Destroying" << endl;

	setState(this, EncoderState::Stopped);
    cleanup();
}



bool VideoEncoder::isReady()
{
	return stateEquals(EncoderState::Ready) 
		&& _encoderContext != NULL
		&& _resizeContext != NULL; 
}



void VideoEncoder::initialize() 
{
	Log("debug") << "[VideoEncoder" << this << "] Initializing" << endl;

	assert(!isReady());

    try {
		// Lock our mutex during avcodec_init
		{
			FastMutex::ScopedLock lock(_mutex);
			//avcodec_init();
			//avcodec_register_all();
		}

		initEncodeContext();
		initScaleContext();
		
		Log("debug") << "[VideoEncoder" << this << "] Initializing: OK" << endl;
		setState(this, EncoderState::Ready);		
		assert(isReady());
    }
    catch (Exception& exc) {	
		Log("error") << "[VideoEncoder" << this << "]" << exc.displayText() << endl;
		
		cleanup();
		setStateMessage(exc.displayText());
		setState(this, EncoderState::Failed);	
		exc.rethrow();
    }
}


void VideoEncoder::initEncodeContext()
{
    if (_encoderContext)
        throw Exception("Encoder context already initialized.");

	//_params.oformat.video = oCodec;
	//Log("debug") << "[VideoEncoder" << this << "] Encoder Output Parameters:\n" << _params.oformat.video.toString() << endl;

    AVCodecContext *oContext = avcodec_alloc_context();
    if (!oContext)
        throw Exception("Cannot allocate encoder context.");

    oContext->codec_type = AVMEDIA_TYPE_VIDEO;
    oContext->width = _params.oformat.video.width;
    oContext->height = _params.oformat.video.height;
    oContext->gop_size = _params.oformat.video.fps; //12;
    //oContext->pix_fmt = static_cast<PixelFormat>(_params.oformat.video.pixfmt);	    	
    oContext->time_base.den = _params.oformat.video.fps;
    oContext->time_base.num = 1;
	//oContext->bit_rate = 400000;	
    oContext->bit_rate = _params.oformat.video.bitRate;
    oContext->has_b_frames = 0;
    oContext->max_b_frames = 0;
    oContext->me_method = 1;

	// H264 specifics
	if (_params.oformat.video.id  == Codec::H264) {
		oContext->me_range = 5;
		oContext->max_qdiff = 5;
		oContext->qmin = 20;
		oContext->qmax = 30;
		oContext->qcompress = 0.6f;
		oContext->qblur = 0.1f;
		oContext->gop_size = 3;
		oContext->max_b_frames = 1;
		oContext->flags = CODEC_FLAG_LOW_DELAY;
	}

	// MJPEG specifics
	else if (_params.oformat.video.id == Codec::MJPEG) {
		//oCodec.pixfmt = Media::YUVJ420P;	
		oContext->pix_fmt = (::PixelFormat)PixelFormat::YUVJ420P; 
		oContext->flags |= CODEC_FLAG_QSCALE;
		oContext->global_quality = 1;
	} 
	else {
		oContext->pix_fmt = (::PixelFormat)PixelFormat::YUV420P; 
	}
	
	//avcodec_find_encoder_by_name ("png")
    AVCodec *oCodec = avcodec_find_encoder(static_cast<CodecID>(_params.oformat.video.id));
	{		
		// Lock our mutex during avcodec_open
		FastMutex::ScopedLock lock(_mutex);
		if (avcodec_open(oContext, oCodec) < 0)
			throw Exception("Cannot open output codec.");
	}

    _encoderContext = oContext;	
}


void VideoEncoder::resetEncodeContext()
{	
    if (_encoderContext) {
		// Lock our mutex during avcodec_close
		FastMutex::ScopedLock lock(_mutex);
        avcodec_close(_encoderContext);
        av_free(_encoderContext);
		_encoderContext = NULL;
    }
}


void VideoEncoder::initScaleContext() //const VideoCodec& iCodec
{
    if (!_encoderContext)
        throw Exception("Encoder context must be initialized before the conversion context.");

    if (_resizeContext)
        throw Exception("Conversion context already initialized.");

	//_params.iformat.video = iCodec;
	//Log("debug") << "[VideoEncoder" << this << "] Scale Parameters:\n" << _params.iformat.video.toString() << endl;

    avpicture_alloc(reinterpret_cast<AVPicture*>(_iFrame), 
		(::PixelFormat)_params.iformat.video.pixfmt, 
		_params.iformat.video.width, _params.iformat.video.height);
    avpicture_alloc(reinterpret_cast<AVPicture*>(_oFrame), 
		_encoderContext->pix_fmt, _encoderContext->width, _encoderContext->height);
	_resizeContext = sws_getContext(_params.iformat.video.width, _params.iformat.video.height, 
		(::PixelFormat)_params.iformat.video.pixfmt,
        _encoderContext->width, _encoderContext->height, _encoderContext->pix_fmt, 
		/* SWS_FAST_BILINEAR */SWS_BICUBIC, NULL, NULL, NULL);
    if (!_resizeContext) 
        throw Exception("Invalid conversion context.");
}


void VideoEncoder::resetScaleContext()
{
	if (_resizeContext) {
		sws_freeContext(_resizeContext);
		_resizeContext = NULL;
	}
}


void VideoEncoder::uninitialize() 
{
    cleanup();
	setState(this, EncoderState::None);
}


void VideoEncoder::cleanup()
{
	resetEncodeContext();
	resetScaleContext();

    if (_iFrame) {
        av_free(_iFrame);
		_iFrame = NULL;
	}

    if (_oFrame) {
        av_free(_oFrame);
		_oFrame = NULL;
	}

	if (_outBuffer) {
		delete _outBuffer;
		_outBuffer = NULL;
	}
}

	
bool VideoEncoder::accepts(IPacket& packet) 
{ 
	return dynamic_cast<VideoPacket*>(&packet) != 0; 
}


void VideoEncoder::process(IPacket& packet)
{ 
	VideoPacket* videoPacket = dynamic_cast<VideoPacket*>(&packet);
	if (videoPacket) {		
		encode(videoPacket->data(), videoPacket->size());
	}
}

int VideoEncoder::encode(unsigned char *input, int inputSize) ///*, unsigned char *output, unsigned int outputSize*/
{
	//Log("debug") << "[VideoEncoder" << this << "] Encode frame: " << inputSize << endl;

	if (!input || !_encoderContext || !_outBuffer || !_oFrame) {
		Log("debug") << "[VideoEncoder" << this << "] Unable to encode frame." << endl;
		return -1;
	}

	// HACK: To speed the things up a bit we just assign
	// the pointer data to _iFrame->data[0].
	//memcpy(_iFrame->data[0], (UInt8*)data, size);
	_iFrame->data[0] = (UInt8*)input;

	if (_resizeContext && sws_scale(
		_resizeContext, 
		_iFrame->data, _iFrame->linesize, 0, _params.iformat.video.height,
		_oFrame->data, _oFrame->linesize) < 0) {
		Log("error") << "[VideoEncoder" << this << "] Pixel format conversion not supported." << endl;
		//throw Exception("AVWriter: Pixel format conversion not supported.");
		return -1;
	}
	
	// PTS value will increment by 1 for input each frame at defined FPS value.
	// PTS value will need to be dynamically generated for variable FPS rates.
	_counter.tick();
	double fpsDiff = _params.oformat.video.fps / _counter.fps;
	_now += 1.0 / fpsDiff;
	_oFrame->pts = _now;
		
	/*
	Log("debug") << "[AVEncoder" << this << "] FPS Difference: " << fpsDiff << endl;
	Log("debug") << "[AVEncoder" << this << "] Calculated PTS: " << _videoPTS << endl;
	Log("debug") << "[AVEncoder" << this << "] Using PTS: " << _videoOutFrame->pts << endl;
	Log("debug") << "[AVEncoder" << this << "] Current FPS: " << _counter.fps << endl;
	*/

	int size = avcodec_encode_video(_encoderContext, _outBuffer, MAX_VIDEO_PACKET_SIZE, _oFrame);
	if (size) {

		VideoPacket packet(_outBuffer, size, 
			_params.oformat.video.width,
			_params.oformat.video.height,
			_oFrame->pts);

		dispatch(this, packet);
	}

    return size;
}


AVFrame* VideoEncoder::MakeBlackFrame(PixelFormat::ID pixfmt, int width, int height)
{
    AVFrame* f = avcodec_alloc_frame();
    unsigned long size = avpicture_get_size((::PixelFormat)pixfmt, width, height);
    UInt8 *buffer = (UInt8*)av_malloc(size*sizeof(UInt8));

    memset(buffer, 0, size);
    avpicture_fill((AVPicture *)f, buffer, (::PixelFormat)pixfmt, width, height);

    return f;
}


void VideoEncoder::FreeFrame(AVFrame* f)
{
    av_free(f->data[0]);
    av_free(f);
}


} } // namespace Sourcey::Media