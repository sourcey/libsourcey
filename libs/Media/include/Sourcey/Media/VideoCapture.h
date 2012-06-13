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


namespace Sourcey {
namespace Media {

	
struct VideoDelegate: public PacketDelegateBase
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
				Log("debug") << "skipping frame" << std::endl;
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


// ---------------------------------------------------------------------
//
class VideoCapture: public ICapture, public Poco::Runnable 
{
public:
	VideoCapture(int deviceId, bool checkDevice = true, bool destroyOnStop = false);
	VideoCapture(const std::string& filename, bool checkDevice = true, bool destroyOnStop = false);
	virtual ~VideoCapture();
	
	virtual void start();
	virtual void stop();
	
	virtual void attach(const PacketDelegateBase& delegate);
	virtual void detach(const PacketDelegateBase& delegate);
	
	virtual void getFrame(cv::Mat& frame, int width = 0, int height = 0);
	
	virtual bool isOpened() const;
	virtual bool isRunning() const;

	virtual int deviceId() const;
	virtual std::string	filename() const;
	virtual std::string	name() const;
	virtual int width() const;
	virtual int height() const;
	virtual double fps() const;

protected:	
	virtual bool open();
	virtual void release();
	virtual void grab();
	virtual bool check();	
	virtual void run();

private:   
	Poco::Thread		_thread;
	cv::VideoCapture	_capture;
	cv::Mat				_frame;	
	FPSCounter			_counter;
	int					_deviceId;	// For video devices
	std::string			_filename;	// For video files
	int					_width;		// The default capture width
	int					_height;	// The default capture height
	bool				_stop;
	bool				_isOpened;	// Reliable replacement for OpenCV
	bool				_destroyOnStop;
	Poco::Event			_wakeUp;
	static Poco::FastMutex	_mutex;
};


inline void AllocateOpenCVInputFormat(const VideoCapture* capture, Format& format) 
	/// Allocates an OpenCV compatible input format for
	/// our encoders.
{
	assert(capture);
	format.label = "OpenCV";
	format.id = Format::Raw;
	format.type = Format::Video;
	format.video.pixfmt = PixelFormat::BGR24;
	format.video.width = capture ? capture->width() : 0;
	format.video.height = capture ? capture->height(): 0;
	format.video.enabled = true;
}


typedef std::map<int, VideoCapture*> VideoCaptureMap;


} // namespace Media
} // namespace Sourcey


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
				//std::Log("debug") << "counter fps: " << _counter.fps << std::endl;
				if (_counter.fps > _fps) {
					//std::Log("debug") << "skipping frame" << std::endl;
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
> polymorphicDelegate(C* pObj, void (C::*Method)(VideoPacket&), double fps = 0, int priority = 0) 
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
> polymorphicDelegate(C* pObj, void (C::*Method)(Void, VideoPacket&), double fps = 0, int priority = 0) 
{
	return Delegate<C, 
		VideoDelegate,
		DelegateCallback<C, 8, true, VideoPacket>, IPolymorphic&
	>(pObj, Method, fps, priority);
}
*/