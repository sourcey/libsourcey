#include "Sourcey/Media/VideoMedia.h"


using namespace Poco;
using namespace std;
using namespace Sourcey;
using namespace Sourcey::Media;


FastMutex VideoMedia::_mutex;


VideoMedia::VideoMedia(int videoId) : 
	_videoId(videoId),
	_videoMedia(NULL), 
	_isInitialized(false)
{
	initDevice();
}


VideoMedia::~VideoMedia()
	// NOTE: We can't release the _videoMedia otherwise
	// it will become unavailable for all processes
{
	//if (_videoMedia->isOpened()) {
	//	_videoMedia->release();
	//}
	if (_videoMedia) {
		delete _videoMedia;
	}

	_isInitialized = false;
}


bool VideoMedia::initDevice() 
{
	if (_isInitialized == true)
		return true;

	// Simultaneous initialization of cv::VideoCapture
	// from multiple threads causes instability. 
	// Let's ensure consecutive access...
	FastMutex::ScopedLock lock(_mutex);

	_videoMedia = new cv::VideoCapture(_videoId);
	if (!_videoMedia->isOpened()) {
		printf("VideoMedia: Error initializing video device #%d\n", _videoId);		
		return false;
	}
	
	printf("VideoMedia: Video device #%d initialized!\n", _videoId);	
	_isInitialized = true;
	return true;
}


bool VideoMedia::releaseDevice() 
{
	if (_videoMedia->isOpened()) {
		_videoMedia->release();
		_isInitialized = false;
		return true;
	}
	return false;
}


cv::Mat VideoMedia::getNextFrame()
{
	cv::Mat frame;

	// If we're not initialized then try to do so.
	if (!_isInitialized || !_videoMedia->isOpened()) {
		cout << "VideoMedia::getNextFrame() !_isInitialized" << endl;
		if (initDevice() == false)
			return cv::Mat();
	}
	
	try 
	{		
		*_videoMedia >> frame;
	} 
	catch(...) 
	{
		printf("VideoMedia: Error while grabbing frame!");
	}	

	return frame;
}


cv::Mat VideoMedia::getNextFrame(int width, int height)
{
	cv::Mat frame = getNextFrame();
	cv::Mat resized;
	if (frame.cols && frame.rows)
		cv::resize(frame, resized, cv::Size(width, height));

	return resized;
}


bool VideoMedia::isInitialized() 
{	
	return _isInitialized;
}