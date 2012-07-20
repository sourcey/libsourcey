#include "Sourcey/Media/VideoMedia2.h"


using namespace Poco;
using namespace std;
using namespace Sourcey;
using namespace Sourcey::Media;


FastMutex VideoMedia2::_mutex;


VideoMedia2::VideoMedia2(int videoId) : 
	_videoId(videoId),
	_videoMedia(NULL), 
	_isInitialized(false)
{
	initDevice();
}


VideoMedia2::~VideoMedia2()
{
	FastMutex::ScopedLock lock(_mutex);

	if (_videoMedia)
		cvReleaseCapture(&_videoMedia);
}


bool VideoMedia2::initDevice() 
{	
	if (_isInitialized == true)
		return true;

	// Simultaneous initialization of cv::VideoCapture
	// from multiple threads causes instability. 
	// Let's ensure consecutive access...
	FastMutex::ScopedLock lock(_mutex);

	if ((_videoMedia = cvCreateCameraCapture(_videoId)) == NULL) {
		printf("VideoMedia2: Error initializing video device #%d\n", _videoId);		
		return false;
	}
	
	printf("VideoMedia2: Video device #%d initialized!\n", _videoId);	
	_isInitialized = true;
	return true;
}


bool VideoMedia2::releaseDevice() 
{
	if (_isInitialized == true) {
		cvReleaseCapture(&_videoMedia);
		_isInitialized = false;
		return true;
	}

	return false;
}


cv::Mat VideoMedia2::getNextFrame()
{
	FastMutex::ScopedLock lock(_mutex);

	cv::Mat frame;
	
	try 
	{	
		// If we're not initialized then try to do so.
		if (!_isInitialized) {
			if (initDevice() == false)
				throw 0;
		}

		if (!cvGrabFrame(_videoMedia)) {
			printf("VideoMedia2: Error while grabbing frame!");
			throw 0;
		}
		
		frame = cv::Mat(cvRetrieveFrame(_videoMedia));
	} 
	catch(...) 
	{
		printf("VideoMedia: Error while grabbing frame!");
	}	

	return frame;
}


cv::Mat VideoMedia2::getNextFrame(int width, int height)
{
	cv::Mat frame = getNextFrame();
	cv::Mat resized;
	if (frame.cols && frame.rows)
		cv::resize(frame, resized, cv::Size(width, height));

	return resized;
}


bool VideoMedia2::isInitialized() 
{	
	return _isInitialized;
}