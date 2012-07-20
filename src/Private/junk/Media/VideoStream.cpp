#include "Sourcey/Media/VideoStream.h"


using namespace std;
using namespace Poco;
using namespace cv;
using namespace Sourcey;
using namespace Sourcey::Media;


// ============================================================================
//
// Video Stream
//
// ============================================================================
VideoStream::VideoStream(int deviceId, int width, int height) : 
	_deviceId(deviceId),
	_width(width),
	_height(height)
{
}


VideoStream::VideoStream(int deviceId) : 
	_deviceId(deviceId),
	_width(0),
	_height(0)
{
}


cv::Mat VideoStream::get(const int& width, const int& height) 
{
	return cv::Mat(); //MediaFactory::instance()->getFrame(_deviceId, width, height);
}

	
VideoStream& VideoStream::operator >> (cv::Mat& frame)
{
	//if (_width && _height)
	//	frame = MediaFactory::instance()->getFrame(_deviceId, _width, _height);
	//else
	//	frame = MediaFactory::instance()->getFrame(_deviceId);		
	return *this;
}


void VideoStream::setDevice(int deviceId)
{
	_deviceId = deviceId;
}


bool VideoStream::good()
{
	return false; //MediaFactory::instance()->getCapture(_deviceId) != 0;
}