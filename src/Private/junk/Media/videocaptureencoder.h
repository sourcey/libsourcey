#ifndef VIDEO_CAPTURE_ENCODER_H
#define VIDEO_CAPTURE_ENCODER_H


#include "Sourcey/Media/Types.h"
#include "Sourcey/Media/VideoCapture.h"
#include "Sourcey/Media/VideoEncoder.h"
#include "Sourcey/Media/FPSCounter.h"
#include "Sourcey/Base/Encoder.h"
#include "Sourcey/Base/Stateful.h"

#include "Sourcey/Base/Signal.h"


namespace Sourcey {
namespace Media {


class VideoCaptureEncoder: public Encoder {
public:
	VideoCaptureEncoder(VideoCapture* capture, const VideoParams& params, bool destroyOnStop = false);
	VideoCaptureEncoder(bool destroyOnStop = false);
	virtual ~VideoCaptureEncoder();

	virtual void start();
	virtual void stop();

	VideoParams& params() { return _params; };
	
	void onVideoCapture(const VideoPacket& packet);

private:
	VideoParams		_params;
	VideoEncoder	_encoder; 
	VideoCapture*	_capture;
	unsigned char*	_buffer;
	FPSCounter		_counter;
	bool			_destroyOnStop;
};


} } // namespace Sourcey::Media


#endif // VIDEO_CAPTURE_ENCODER_H