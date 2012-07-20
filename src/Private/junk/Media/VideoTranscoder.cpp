#include "Sourcey/Media/VideoTranscoder.h"

#include <iostream>
#include <cstring>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavcodec/opt.h>
}

// We know that ffmpeg might read more than the size of
// the input, how much is defined in FF_INPUT_BUFFER_PADDING_SIZE
// this is usually 8, but as we don't always include FFMPEG headers
// we declare it large enough.
#define FFMPEG_INPUT_PADDING_SIZE 16

/*
// Make ffmpeg happy without C99 stdint
#ifndef INT64_C
# define INT64_C(a) ((int64_t)(a ## LL))
#endif
*/


using namespace std;


namespace Sourcey {
namespace Media {

bool VideoTranscoder::init_done_ = false;
int VideoTranscoder::initialize()
{
    if (!VideoTranscoder::init_done_) {
        avcodec_init();
        avcodec_register_all();
        VideoTranscoder::init_done_ = true;
        return 1;
    }

    return 0;
}

VideoTranscoder::VideoTranscoder()
: root(VideoTranscoder::initialize()),
  decode_context_(0), _iFrame(avcodec_alloc_frame()),
  _encoderContext(0), _oFrame(avcodec_alloc_frame()),
  _resizeContext(0), _now(0)
{
}

VideoTranscoder::~VideoTranscoder()
{
    if (decode_context_) {
        avcodec_close(decode_context_);
        av_free(decode_context_);
    }

    if (_encoderContext) {
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

int VideoTranscoder::initEncodeContext(VideoCodecID codec_id, unsigned int ow, unsigned int oh, unsigned long bit_rate)
{
    if(_encoderContext)
        return 2;

    #if ENABLE_DEBUG
      std::cerr << "Initializing Encoder" << std::endl;
    #endif

    AVCodecContext *ocontext = avcodec_alloc_context();
    if (!ocontext)
        return 0;

    if ( (ow == 0) && (oh == 0) ) {
        if (ow == 0)
          ow = decode_context_->width;
        if (oh == 0)
          oh = decode_context_->height;
    }

    ocontext->codec_type = CODEC_TYPE_VIDEO;
    ocontext->width = ow;
    ocontext->height = oh;
    ocontext->gop_size = 12;
    ocontext->pix_fmt = PIX_FMT_YUV420P;
    ocontext->time_base.den = 25;
    ocontext->time_base.num = 1;
    ocontext->bit_rate = bit_rate;
    ocontext->has_b_frames = 0;
    ocontext->max_b_frames = 0;
    ocontext->me_method = 1;

	AVCodec *oCodec = avcodec_find_encoder(static_cast<CodecID>(codec_id));
	if ( avcodec_open(ocontext, oCodec) < 0 ) {
	  std::cerr << "Unable to open output codec" << std::endl;
	  return 0;
	}

    _encoderContext = ocontext;
    return 1;
}

void VideoTranscoder::resetEncodeContext()
{
    av_free(_encoderContext);
    _encoderContext = 0;
}

unsigned long VideoTranscoder::encode(AVFrame* input, unsigned char *output, unsigned int outputSize)
{
    if (!input)
        return 0;

    input->pts = AV_NOPTS_VALUE;
    _now += 1.0/25;
    return avcodec_encode_video(_encoderContext, output, outputSize, input);
}

int VideoTranscoder::initDecodeContext(VideoCodecID codec_id, unsigned char *extradata, unsigned long extradata_len)
{
    if(decode_context_)
        return 2;

    #if ENABLE_DEBUG
      std::cerr << "Initializing Decoder" << std::endl;
    #endif

    AVCodec *iCodec = avcodec_find_decoder(static_cast<CodecID>(codec_id));
    if (!iCodec) {
        std::cerr << "Unable to find input codec" << std::endl;
        return 0;
    }

    AVCodecContext *icontext = avcodec_alloc_context();
    if (!icontext)
        return 0;

    avcodec_get_context_defaults(icontext);
    if (extradata) {
        icontext->extradata = static_cast<unsigned char*>(calloc(1, extradata_len + FFMPEG_INPUT_PADDING_SIZE));
        icontext->extradata_size = extradata_len;
        memcpy(icontext->extradata, extradata, extradata_len);
    }

    if ( avcodec_open(icontext, iCodec) < 0 ) {
        std::cerr << "Unable to open input codec" << std::endl;
        return 0;
    }

    decode_context_ = icontext;
    return 1;
}

AVFrame* VideoTranscoder::decode(unsigned char *input, unsigned int input_size)
{
    int got_pic = 0;
    avcodec_decode_video(decode_context_, _iFrame, &got_pic, input, input_size);
    if (!got_pic)
        return 0;
    return _iFrame;
}

void VideoTranscoder::resetScaleContext()
{
    av_free(_resizeContext);
    _resizeContext = 0;
}

int VideoTranscoder::initScaleContext(int width, int height)
{
	cout << "VideoTranscoder::initScaleContext()" << endl;
    if (_resizeContext)
        return 2;

	cout << "VideoTranscoder::initScaleContext(): 1" << endl;
    if ((width == 0) && (height == 0) ) {
        if (width == 0)
          width = decode_context_->width;
        if (height == 0)
          height = decode_context_->height;
    }

	cout << "VideoTranscoder::initScaleContext(): 2" << endl;
    if (!_resizeContext) {
        avpicture_alloc(reinterpret_cast<AVPicture*>(_oFrame), PIX_FMT_YUV420P, width, height);
	cout << "VideoTranscoder::initScaleContext(): 21" << endl;
        _resizeContext = sws_getContext(decode_context_->width, decode_context_->height, decode_context_->pix_fmt,
                                         width, height, PIX_FMT_YUV420P, SWS_FAST_BILINEAR, NULL, NULL, NULL);
	cout << "VideoTranscoder::initScaleContext(): 22" << endl;
    }

	cout << "VideoTranscoder::initScaleContext(): 3" << endl;
    if (!_resizeContext) {
        std::cerr << "Invalid Conversion Context" << std::endl;
        return 0;
    }

    return 1;
}

AVFrame* VideoTranscoder::scaleFrame(AVFrame* frame)
{
    int r = sws_scale(_resizeContext, frame->data, frame->linesize, 0, decode_context_->height,
                      _oFrame->data, _oFrame->linesize);
    if (r < 0)
        return 0;
    return _oFrame;
}

AVFrame* VideoTranscoder::MakeBlackFrame(int width, int height)
{
    AVFrame* f = avcodec_alloc_frame();
    unsigned long size = avpicture_get_size(PIX_FMT_YUV420P, width, height);
    uint8_t *buffer = (uint8_t*)av_malloc(size*sizeof(uint8_t));

    memset(buffer, 0, size);
    avpicture_fill((AVPicture *)f, buffer, PIX_FMT_YUV420P, width, height);

    return f;
}

void VideoTranscoder::FreeFrame(AVFrame* f)
{
    av_free(f->data[0]);
    av_free(f);
}

} } // namespace Sourcey::Media

