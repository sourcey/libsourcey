//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// LibSourcey is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//


#ifndef SOURCEY_MEDIA_VideoCapture_H
#define SOURCEY_MEDIA_VideoCapture_H


#include "Sourcey/Base.h"
#include "Sourcey/Mutex.h"
#include "Sourcey/Flaggable.h"
#include "Sourcey/Interfaces.h"
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


namespace scy {
namespace av {


// ---------------------------------------------------------------------
//
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
				traceL() << "skipping frame" << std::endl;
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
		/// Settings flags for different operational modes
	{
		DestroyOnStop		= 0x01, 
		SyncWithDelegates	= 0x02,
		WaitForDelegates	= 0x04,
		ErrorState			= 0x10
	};

	VideoCapture(int deviceId, unsigned flags = 0); 
		/// Creates and opens the given device
		/// Should be created in the main thread

	VideoCapture(const std::string& filename, unsigned flags = 0);
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
	virtual std::string	error() const;
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
	virtual void setError(const std::string& error);

private:   
	mutable Mutex _mutex;

	cv::VideoCapture _capture;
	cv::Mat		_frame;		// Current video image
	int		_deviceId;		// Source device to capture from
	int		_width;			// Capture width
	int		_height;		// Capture height
	bool	_isImage;		// Source file is an image or not
	bool	_isOpened;
	bool	_stopping;
	Flags	_flags;	
	std::string	_error;		// Error message if any
	std::string	_filename;	// Source file to capture from if any
	FPSCounter	_counter;
	Poco::Event	_capturing;
	Poco::Thread _thread;
};


inline void setVideoCaptureInputFormat(const VideoCapture* capture, Format& format) 
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

	MatPacket(char* data = NULL,
			  int size = 0,
			  int width = 0,
			  int height = 0,
			  double time = 0) :
		VideoPacket(data, size, width, height, time),
		mat(nullptr) {};

	MatPacket(cv::Mat* mat, double time = 0) :
		VideoPacket((char*)mat->data, mat->rows*mat->step, mat->cols, mat->rows, time), //mat->total()
		mat(mat) {}

	virtual IPacket* clone() const {
		return new MatPacket(*this);
	}	

	virtual const char* className() const { return "MatPacket"; }
}; 


} // namespace av
} // namespace scy


#endif // SOURCEY_MEDIA_VideoCapture_H