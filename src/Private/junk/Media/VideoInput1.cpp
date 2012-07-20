#include "Sourcey/Media/VideoMedia1.h"


using namespace Poco;
using namespace std;
using namespace Sourcey;
using namespace Sourcey::Media;


FastMutex VideoMedia1::_mutex;


VideoMedia1::VideoMedia1(int videoId) : 
	_videoId(videoId), 
	_isInitialized(false)
{
	initDevice();
}


VideoMedia1::~VideoMedia1()
	// NOTE: We can't release the _videoMedia otherwise
	// it will become unavailable for all processes
{
	if (_videoMedia.isOpened()) {
		_videoMedia.release();
	}

	_isInitialized = false;
}


bool VideoMedia1::initDevice() 
{
	if (_isInitialized == true)
		return true;

	// Simultaneous initialization of cv::VideoCapture
	// from multiple threads causes instability. 
	// Let's ensure consecutive access...
	FastMutex::ScopedLock lock(_mutex);

	_videoMedia = cv::VideoCapture(_videoId);
	if (!_videoMedia.isOpened()) {
		printf("VideoMedia1: Error initializing video device #%d\n", _videoId);		
		return false;
	}
	
	printf("VideoMedia1: Video device #%d initialized!\n", _videoId);	
	_isInitialized = true;
	return true;
}


bool VideoMedia1::releaseDevice() 
{
	if (_videoMedia.isOpened()) {
		_videoMedia.release();
		_isInitialized = false;
		return true;
	}
	return false;
}


cv::Mat VideoMedia1::getNextFrame()
{
	cv::Mat frame;

	// If we're not initialized then try to do so.
	if (!_isInitialized || !_videoMedia.isOpened()) {
		cout << "VideoMedia1::getNextFrame() !_isInitialized" << endl;
		if (initDevice() == false)
			return cv::Mat();
	}
	
	try 
	{		
		_videoMedia >> frame;
	} 
	catch(...) 
	{
		printf("VideoMedia1: Error while grabbing frame!");
	}	

	return frame;
}


cv::Mat VideoMedia1::getNextFrame(int width, int height)
{
	cv::Mat frame = getNextFrame();
	cv::Mat resized;
	if (frame.cols && frame.rows)
		cv::resize(frame, resized, cv::Size(width, height));

	return resized;
}


bool VideoMedia1::isInitialized() 
{	
	return _isInitialized;
}