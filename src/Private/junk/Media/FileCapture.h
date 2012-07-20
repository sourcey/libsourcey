#ifndef FILE_CAPTURE_H
#define FILE_CAPTURE_H


#include "Sourcey/Media/Common.h"
#include "Sourcey/Media/VideoCapture.h"

#include "Poco/Foundation.h"
#include "Poco/Thread.h"
#include "Poco/BasicEvent.h"
#include "Poco/format.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <iostream>
#include <map>


namespace Sourcey {
namespace Media {


	/*
/// This is the base class for all instantiations of
/// the VideoCallback template.
class AbstractVideoCallback
{
public:
	AbstractVideoCallback() {};
	virtual ~AbstractVideoCallback() {};
	
	virtual void invoke(cv::Mat& frame) const = 0;
	virtual AbstractVideoCallback* clone() const = 0;
	virtual void* object() const = 0;
};


typedef std::vector<AbstractVideoCallback*> VideoCallbackList;


/// This template class implements an adapter that sits between
/// a FileCapture and an object's method invoked by the capture.
template <class C> 
class VideoCallback: public AbstractVideoCallback
{
public:
	typedef void (C::*Callback)(cv::Mat&);

	VideoCallback(C& object, Callback method) : 
		_object(&object), _method(method) {}
	VideoCallback(const VideoCallback& callback) : 
		_object(callback._object), _method(callback._method) {}

	void invoke(cv::Mat& frame) const
	{
		(_object->*_method)(frame);
	}

	AbstractVideoCallback* clone() const
	{
		return new VideoCallback(*this);
	}

	void* object() const
	{
		return _object;
	}

private:
	VideoCallback() {};

	C*       _object;
	Callback _method;
};
*/


class FileCapture : public VideoCapture
{
public:
	FileCapture(std::string filename);
	//virtual ~FileCapture();

public:	
	//void start(const AbstractVideoCallback& method);
	//void stop(const void* object);
	//bool isRunning() const;
	//int refCount() const;
	//std::string filename() const;
	//int width() const;
	//int height() const;

protected:	
	//void run();

private:   
	//VideoCallbackList		_callbacks;
	//Poco::Thread			_thread;
	cv::VideoCapture		_capture;	
	std::string				_filename;
	//int						_width;		// The default capture width
	//int						_height;	// The default capture height
	//bool					_stop;
	//static Poco::FastMutex	_mutex;
};


typedef std::map<int, FileCapture*> FileCaptureMap;


} } // namespace Sourcey::Media


#endif // FILE_CAPTURE_H
