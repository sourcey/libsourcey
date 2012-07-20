#ifndef MEDIA_MJPEG_PROVIDER_H
#define MEDIA_MJPEG_PROVIDER_H


#include "Sourcey/Media/MediaProvider.h"
#include "Sourcey/Media/Types.h"
#include "Sourcey/Media/VideoCaptureEncoder.h"
#include "Sourcey/Util/Signal.h"


namespace Sourcey {
namespace Media {


// The Media Provider class is one step removed from a Capture
// Encoder class, as it can be used to impliment codec specific 
// packaging capabilities
class MJPEGMediaProvider: public MediaProvider
{
public:
	MJPEGMediaProvider(bool destroyOnStop = false);
	MJPEGMediaProvider(VideoCapture* capture, const VideoParams& params, bool destroyOnStop = false);
	virtual ~MJPEGMediaProvider();

	virtual void start();
	virtual void stop();

	void sendError(const std::string& err);

	//void setParams(const VideoParams& params) { _params = params; };
	//void setVideoCapture(VideoCapture* capture) { _capture = capture; };

	void onVideoReady(void* sender, VideoPacket& packet);

protected:   
	VideoCapture*			_capture;
	VideoCaptureEncoder*	_encoder;
	VideoParams				_params;
	//FPSCounter				_counter;
	int						_quality;
	bool					_destroyOnStop;
};


} } // namespace Sourcey::Media


#endif // MEDIA_PROVIDER_H