#ifndef VIDEO_CAPTURE_ENCODER_H
#define VIDEO_CAPTURE_ENCODER_H


#include "Sourcey/Media/Types.h"
#include "Sourcey/Media/Capture.h"
#include "Sourcey/Media/VideoEncoder.h"
#include "Sourcey/Media/FPSCounter.h"
#include "Sourcey/Util/Stateful.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "Sourcey/Util/Signal.h"
#include "Poco/DateTimeFormatter.h"

#include "Poco/Timestamp.h"


namespace Sourcey {
namespace Media {


struct CaptureEncoderState: StateT {
	enum Type {
		Idle,
		Encoding,
		Error
	};
	std::string str(unsigned int id) const { 	
		switch(id) {
		case Idle: return "Idle";
		case Encoding: return "Encoding";
		case Error: return "Error";
		}
		return "undefined"; 
	};
};


class CaptureEncoder: public Stateful<CaptureEncoderState> {
public:
	CaptureEncoder(Capture* capture) :
		_capture(capture) {
		assert(_capture);	
		setState(CaptureEncoderState::Idle);
	};
	virtual ~CaptureEncoder();

	virtual void start() = 0;
	virtual void stop() = 0;

	Signal<MediaPacket&> MediaEncoded;	

private:
	Capture*	_capture;
};


} } // namespace Sourcey::Media


#endif // VIDEO_CAPTURE_ENCODER_H