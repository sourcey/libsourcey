//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is is distributed under a dual license that allows free, 
// open source use and closed source use under a standard commercial
// license.
//
// Non-Commercial Use:
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// Commercial Use:
// Please contact mail@sourcey.com
//


#ifndef SOURCEY_MEDIA_VideoCapture_H
#define SOURCEY_MEDIA_VideoCapture_H


#include "Sourcey/Base.h"
#include "Sourcey/Flaggable.h"
#include "Sourcey/Media/Types.h"
#include "Sourcey/Media/Format.h"
#include "Sourcey/Media/ICapture.h"
#include "Sourcey/Media/FPSCounter.h"

#include "Poco/Thread.h"
#include "Poco/Event.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <iostream>
#include <map>


namespace Scy {
namespace Media {


// ---------------------------------------------------------------------
struct VideoDelegate: public PacketDelegateBase
	/// Polymorphic packet delegate for the VideoPacket type.
{
	typedef double DataT;

	VideoDelegate(double fps = 0.0) : _fps(fps) {};
	VideoDelegate(const VideoDelegate& r) : 
		_counter(r._counter), 
		_fps(r._fps) {};	
	
	virtual bool accepts(void*, IPacket&, Void, Void, Void)
	{
		// Skip frames if we exceed the maximum FPS
		if (_fps) {
			_counter.tick();
			if (_counter.fps > _fps) {
				LogDebug() << "skipping frame" << std::endl;
				return false;
			}
		}		
		return true;
	}
	
protected:
	double		_fps;
	FPSCounter	_counter;
};


DefinePolymorphicDelegateWithArg(videoDelegate, IPacket, VideoDelegate, double, 0)


class VideoCapture: public ICapture, public Poco::Runnable 
	/// Class for capturing video from files or cameras using OpenCV.
	///
	/// Usage:
	/// VideoCapture instances should be created in the main
	/// thread to eliminate unexpected behaviour inherent in OpenCV.
	/// Also avoid creating multiple instances using the same device.
	/// Instead reuse the same instance, preferrably using the 
	/// MediaFactory interface.
	/// 
	/// Windows:
	/// OpenCV HighGUI DirectShow must be compiled with
	/// VI_COM_MULTI_THREADED defined otherwise capture 
	/// there will be CoInitialize conflicts.
{
public:
	enum Flag 
	{
		DestroyOnStop		= 0x01, 
		SyncWithDelegates	= 0x02,
		ErrorState			= 0x04//,
		//		= 0x08,
		//		= 0x10,
		//		= 0x20,
	};

	VideoCapture(int deviceId); 
		/// Creates and opens the given device ID
		/// Should be created in the main thread

	VideoCapture(const std::string& filename);
		/// Creates and opens the given video file
		/// Can be created in any thread

	virtual ~VideoCapture();
	
	virtual void start();
	virtual void stop();
	
	virtual void attach(const PacketDelegateBase& delegate);
	virtual bool detach(const PacketDelegateBase& delegate);
	
	virtual void getFrame(cv::Mat& frame, int width = 0, int height = 0);
	
	virtual bool isOpened() const;
	virtual bool isRunning() const;

	virtual int deviceId() const;
	virtual std::string	filename() const;
	virtual std::string	name() const;
	virtual int width() const;
	virtual int height() const;
	virtual double fps() const;
	virtual Flags flags() const;
	virtual cv::VideoCapture& capture();

protected:	
	virtual bool open();
	virtual void release();
	virtual cv::Mat grab();
	virtual void run();

private:   
	mutable Poco::FastMutex _mutex;

	Poco::Thread		_thread;
	cv::VideoCapture	_capture;
	cv::Mat				_frame;	
	Flags				_flags;	
	int					_deviceId;	// Source device to capture from
	std::string			_filename;	// Source file to capture from
	int					_width;		// Capture width
	int					_height;	// Capture height
	bool                _isImage;	// Source file is an image or not
	bool				_isOpened;
	bool				_stop;
	FPSCounter			_counter;
	Poco::Event			_wakeUp;
};


inline void AllocateOpenCVInputFormat(const VideoCapture* capture, Format& format) 
	/// Allocates an OpenCV compatible input format for
	/// our encoders.
{
	assert(capture);
	//format.type = Format::Video;
	format.name = "OpenCV";
	format.id = "mjpeg";
	format.video.encoder = "mjpeg";
	format.video.pixelFmt = "bgr24";
	format.video.width = capture ? capture->width() : 0;
	format.video.height = capture ? capture->height(): 0;
	format.video.enabled = true;
}


typedef std::map<int, VideoCapture*> VideoCaptureMap;


struct MatPacket: public VideoPacket 
{
	cv::Mat* mat;	// For OpenCV generated packets.

	MatPacket(unsigned char* data = NULL,
				int size = 0,
				int width = 0,
				int height = 0,
				double time = 0) :
		VideoPacket(data, size, width, height, time),
		mat(NULL) {};

	MatPacket(cv::Mat* mat, double time = 0) :
		VideoPacket((unsigned char*)mat->data, mat->total(), mat->cols, mat->rows, time),
		mat(mat) {};

	//void init(cv::Mat* mat) const {
	//	data = 
	//}	

	virtual IPacket* clone() const {
		return new MatPacket(*this);
	}	

	virtual const char* className() const { return "MatPacket"; }
}; 



} // namespace Media
} // namespace Scy


#endif // SOURCEY_MEDIA_VideoCapture_H



/*
template <class C>
class VideoCallback: public Callback<C, const VideoPacket, false>
	/// This template class implements an adapter that sits between
	/// a Notifier and an object receiving notifications from it.
{
public:
	typedef Callback<C, const VideoPacket, false> CallbackBase;

	VideoCallback(C* object, Method method, double fps = 0) : //ICaptureFilter* filter = NULL, 
	  Callback(object, method), _fps(fps) {} //_filter(filter), 

	VideoCallback(const Callback& r) : 
		Callback(r._object, r._method), 
		_filter(r._filter),
		_fps(r._fps){}, _fps(fps)
	
	virtual ICallback* clone() const { return new VideoCallback(*this); }
	virtual bool accepts(IPolymorphic& data) 
	{	
		// Skip frames if we exceed the maximum FPS
		if (_fps) {
			if (_counter.started()) {
				_counter.endFrame();
				//std::LogDebug() << "counter fps: " << _counter.fps << std::endl;
				if (_counter.fps > _fps) {
					//std::LogDebug() << "skipping frame" << std::endl;
					return false;
				}
			}
			_counter.startFrame();
		}

		return CallbackBase::accepts(data);
	}
	
private:
	VideoCallback();

	double		_fps;
	FPSCounter	_counter;
	//ICaptureFilter* _filter;
	//C*     _object;
	//Method _method;
};
*/


/*
DefinePolymorphicDelegateWithArg(videoDelegate, VideoPacket, VideoDelegate)
template <class C, typename VideoPacket>
static Delegate<C, 
	VideoDelegate,
	DelegateCallback<C, 8, false, VideoPacket>, IPolymorphic&
> PolymorphicDelegate(C* pObj, void (C::*Method)(VideoPacket&), double fps = 0, int priority = 0) 
{
	return Delegate<C, 
		VideoDelegate,
		DelegateCallback<C, 8, false, VideoPacket>, IPolymorphic&
	>(pObj, Method, fps, priority);
}


template <class C, typename VideoPacket>
static Delegate<C, 
	VideoDelegate,
	DelegateCallback<C, 8, true, VideoPacket>, IPolymorphic&
> PolymorphicDelegate(C* pObj, void (C::*Method)(Void, VideoPacket&), double fps = 0, int priority = 0) 
{
	return Delegate<C, 
		VideoDelegate,
		DelegateCallback<C, 8, true, VideoPacket>, IPolymorphic&
	>(pObj, Method, fps, priority);
}
*/