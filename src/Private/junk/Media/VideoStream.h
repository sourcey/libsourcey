#ifndef VIDEO_STREAM_H
#define VIDEO_STREAM_H


#include "Sourcey/Media/MediaFactory.h"

#include "Poco/Foundation.h"
#include "Poco/Thread.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <iostream>
#include <map>


namespace Sourcey {
namespace Media {


/// The VideoStream class utilizes C++ stream operators to provide a
/// user friendly access point to the MediaFactory class functionality 
/// for Spot and it's Plugin API.
class VideoStream
{
public:
	VideoStream(int deviceId, int width, int height);
	VideoStream(int deviceId=0);
	
	virtual void setDevice(int deviceId);

	virtual VideoStream& operator >> (cv::Mat& frame);
	virtual cv::Mat get(const int& width, const int& height);
	virtual bool good();

protected:
	int _deviceId;
	int _width;
	int _height;
};


} } // namespace Sourcey::Media


#endif // VIDEO_STREAM_H
