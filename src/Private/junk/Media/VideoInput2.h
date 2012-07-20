#ifndef VIDEO_INPUT2_H
#define VIDEO_INPUT2_H


//#include "Common.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "Poco/Mutex.h"


namespace Sourcey {
namespace Media {


/**
 * Derived class that implements VideoMedia2Base.
 * It uses a single-stream behavior to retrieve one stream of Mat(s) from 
 * an hardware device, a webcam or a framegrabber.
 */
class VideoMedia2
{
public:
	/**
	 * Constructor.
	 * @param videoId Identify the camera between other grabbing inputs devices.
	 */
	VideoMedia2(int videoId);
	virtual ~VideoMedia2();

	virtual bool initDevice();
	virtual bool releaseDevice();
	virtual bool isInitialized();

	/**
	 * Get the next image in the stream.
	 * Depends on OpenCV grabbing context.
	 * @return The next image in the stream.
	 * @note The returned cv::Mat must be freed by the caller.
	 */ 
	virtual cv::Mat getNextFrame();
	virtual cv::Mat getNextFrame(int width, int height);
	
private:
	/** Specify whether the class is loading or not. */					
	static bool _isLoading;

	/** Specify whether the class is initialized or not. */					
	bool _isInitialized;
	
	/** Camera number mapped on system. */
	int _videoId;
	
	/** OpenCV capture structure. */
	CvCapture *_videoMedia;
	
	/** Mutex locker to protect device initialization. */
	static Poco::FastMutex _mutex;
};


} } // namespace Sourcey::Media


#endif	//_VIDEO_INPUT_H_

