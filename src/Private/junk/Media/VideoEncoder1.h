#ifndef VIDEO_ENCODER_H
#define VIDEO_ENCODER_H


#include "Sourcey/Media/Types.h"

#include "Poco/Mutex.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/fifo.h>
#include <libswscale/swscale.h>
}


namespace Sourcey {
namespace Media {


class VideoEncoder {
	/// Video encoder class with reusable code that
	/// depends on ffmpeg libavcodec/libavformat.
public:		
	VideoEncoder(/*const VideoParams& oParams, const VideoParams& iParams*/);
	virtual ~VideoEncoder();

	bool isInitialized();

	void initEncodeContext(const VideoParams& oParams);
    void resetEncodeContext();

    void initScaleContext(const VideoParams& iParams);
    void resetScaleContext();

    int encode(unsigned char *input, unsigned int inputSize, unsigned char *output, unsigned int outputSize);

    AVCodecContext* getEncoderContext() const { return _encoderContext; }
    double getTimeBase() const { return 1.0/_oParams.fps; }
    double getNow() const { return _now; }	

    static AVFrame* MakeBlackFrame(VideoPixelFormat pixfmt, int width, int height);
    static void FreeFrame(AVFrame* f);
	
private:
	static Poco::FastMutex _mutex;	// Mutex locker to protect avcodec_open/close().
	VideoParams		_iParams;
	VideoParams		_oParams;
    AVFrame*		_iFrame;
    AVFrame*		_oFrame;
    AVCodecContext*	_encoderContext;
    SwsContext*		_resizeContext;
    double			_now;
};


} } // namespace Sourcey::Media


#endif	// VIDEO_ENCODER_H

