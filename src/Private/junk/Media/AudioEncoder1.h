#ifndef AUDIO_ENCODER_H
#define AUDIO_ENCODER_H


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


class AudioEncoder
	/// Audio encoder.
	/// Utility class with reusable code. Depends on libavcodec/libavformat.
{
public:		
	AudioEncoder();
	virtual ~AudioEncoder();

	bool isInitialized();

	void initEncodeContext(const AudioParams& oParams);
    void resetEncodeContext();
    int encode(unsigned char *input, unsigned int inputSize, unsigned char *output, unsigned int outputSize);

    AVCodecContext* getEncoderContext() const { return _encoderContext; }

    double getNow() const { return _now; }	
	
private:
	static Poco::FastMutex _mutex;	// Mutex locker to protect avcodec_open/close().
	AudioParams		_iParams;
	AudioParams		_oParams;
	AVFifoBuffer*	_fifo;		
	uint8_t*		_fifoBuffer;
	int				_audioOutSize;
    AVCodecContext*	_encoderContext;
    SwsContext*		_resizeContext;
    double			_now;
};


} } // namespace Sourcey::Media


#endif	// AUDIO_ENCODER_H

