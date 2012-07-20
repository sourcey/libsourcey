#ifndef SOURCEY_MEDIA_ICaptureFilter_H
#define SOURCEY_MEDIA_ICaptureFilter_H


#include <opencv/cv.h>
#include <opencv/highgui.h>


namespace Sourcey {

		
class ICaptureFilter
	/// Defines a simple media filter interface 
	/// that processes and outputs a single
	/// cv::Mat image.
{
public:
	ICaptureFilter() {};
	virtual ~ICaptureFilter() {};

	virtual void process(const cv::Mat& source, cv::Mat& target) = 0;
};


} // namespace Sourcey


#endif // SOURCEY_MEDIA_ICaptureFilter_H
