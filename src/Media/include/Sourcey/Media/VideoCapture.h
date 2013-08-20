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


#include "Sourcey/Types.h"
#include "Sourcey/Mutex.h"
#include "Sourcey/Bitwise.h"
#include "Sourcey/Interface.h"
#include "Sourcey/Media/Types.h"
#include "Sourcey/Media/Format.h"
#include "Sourcey/Media/ICapture.h"
#include "Sourcey/Media/FPSCounter.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <iostream>
#include <map>


namespace scy {
namespace av {
	

class VideoCaptureBase;
	/// The reference counted video capture implementation.
	/// Helps us overcome the multithread capture limitation with OpenCV.


//
// Video Capture
//


class VideoCapture: public ICapture
	/// Class for capturing video from cameras and files using OpenCV.
{
public:
	VideoCapture(int deviceId);
		// Creates and opens the given device
		// Captures can be instantiated from thread, providing the  
		// VideoCaptureBase for the given deviceId was already initialized
		// in the main thread.

	VideoCapture(const std::string& filename);
		// Creates and opens the given video file
		// Can be created in any thread

	VideoCapture(VideoCaptureBase* base);
		// Creates the video capture using the given base instance.

	virtual ~VideoCapture();
	
	virtual void start();
	virtual void stop();
	
	bool opened() const;
		// True when the system device is open.

	bool running() const;
		// True when the thread is running. 

	int width();
	int height();

	VideoCaptureBase& base();
				
	void getFrame(cv::Mat& frame, int width = 0, int height = 0);

	virtual void getEncoderFormat(Format& iformat);

protected:
	mutable Mutex _mutex;

	VideoCaptureBase* _base;
};
	

//
// Video Capture Base
//


class VideoCaptureBase: public SharedObject, public basic::Runnable
	/// Class for capturing video from cameras and files using OpenCV.
	/// Do not use this class directly, use VideoCapture instead.
	///
	/// Limitations:
	/// OpenCV doesn't support multi-thread capturing so VideoCaptureBase
	/// instances should be created in the main thread.
	/// File captures do not have this limitation.
	/// Also avoid creating multiple instances using the same device.
	/// Instead reuse the same instance, preferably using the 
	/// MediaFactory interface.
	/// 
	/// Windows:
	/// OpenCV HighGUI DirectShow must be compiled with VI_COM_MULTI_THREADED
	/// defined otherwise capture there will be CoInitialize conflicts
	/// with the DeviceManager.
{
public:
	VideoCaptureBase(int deviceId); 
		// Creates and opens the given device
		// Should be created in the main thread

	VideoCaptureBase(const std::string& filename);
		// Creates and opens the given video file
		// Can be created in any thread
	
	virtual void start();
	virtual void stop();
	
	bool opened() const;
		// True when the system device is open.

	bool running() const;
		// True when the internal thread is running. 
	
	int deviceId() const;
	std::string	filename() const;
	std::string	name() const;
	std::string	error() const;
	double fps() const;
	int width();
	int height();
	cv::Mat lastFrame() const;
	cv::VideoCapture& capture();

protected:	
	virtual ~VideoCaptureBase();

	bool open();
	cv::Mat grab();
	virtual void run();

	void setError(const std::string& error);
	
	virtual void addEmitter(PacketSignal* emitter);
	virtual void removeEmitter(PacketSignal* emitter);
	
	friend class VideoCapture;
	friend class MediaFactory;
	friend struct std::default_delete<VideoCaptureBase>;	

private:   
	mutable Mutex _mutex;
	mutable Mutex _emitMutex;

	bool _opened;
	bool _stopping;
	bool _capturing;
	int _deviceId;			// Source device to capture from
	cv::Mat _frame;			// Current video image
	std::string	_error;		// Error message if any
	std::string	_filename;	// Source file to capture from if any
	FPSCounter _counter;
	cv::VideoCapture _capture;
	PacketSignalVec _emitters;
	Thread _thread;
};


typedef std::map<int, VideoCaptureBase*> VideoCaptureBaseMap;
	

//
// Matrix Packet
//


struct MatrixPacket: public VideoPacket 
{
	cv::Mat* mat; // For OpenCV generated packets.
				  // TODO: Use stream offset time instead of process time 
				  // for consistency with AudioCapture for realtime pts calculation
	
	MatrixPacket(cv::Mat* mat, double time = scy::getProcessTime()) :
		VideoPacket((char*)mat->data, mat->rows*mat->step, mat->cols, mat->rows, time),
		mat(mat) {}

	MatrixPacket(char* data = nullptr,
			  int size = 0,
			  int width = 0,
			  int height = 0,
			  double time = scy::getProcessTime()) :
		VideoPacket(data, size, width, height, time),
		mat(nullptr) {};

	virtual IPacket* clone() const {
		return new MatrixPacket(*this);
	}	

	virtual const char* className() const { return "MatrixPacket"; }
}; 


} } // namespace scy::av


#endif // SOURCEY_MEDIA_VideoCapture_H



/*
//
// Video Delegate
//


struct VideoDelegate: public PacketDelegateBase
	/// Polymorphic packet delegate for the VideoPacket type.
{
	typedef double DataT;

	VideoDelegate(double fps = 0.0) : _fps(fps) {};
	VideoDelegate(const VideoDelegate& r) : 
		_counter(r._counter), 
		_fps(r._fps) {};	
	
	virtual bool accepts(void*, IPacket&, void*, Void, Void)
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
*/


	/* //, unsigned flags = 0 //, unsigned flags = 0//, public basic::Runnable 
	enum Flag 
		// Settings for different operational modes
	{
		DestroyOnStop		= 0x01, 
		SyncWithDelegates	= 0x02,
		WaitForDelegates	= 0x04
	};
	*/


	/*
	bool opened() const;
	bool running() const;
	
	int deviceId() const;
	std::string	filename() const;
	std::string	name() const;
	std::string	error() const;
	int width();
	int height();
	double fps() const;
	Flaggable flags() const;
	cv::VideoCapture& capture();
			
	void getFrame(cv::Mat& frame, int width = 0, int height = 0);

	virtual void getEncoderFormat(Format& iformat);

protected:	
	bool open();
	void release();
	cv::Mat grab();
	virtual void run();

	void setError(const std::string& error);

private:   
	mutable Mutex _mutex;

	cv::VideoCapture _capture;
	cv::Mat _frame;			// Current video image
	int _deviceId;			// Source device to capture from
	int _width;				// Capture width
	int _height;			// Capture height
	bool _isImageSource;			// Source file is an image or not
	bool _opened;
	bool _stopping;
	bool _capturing;
	Flaggable _flags;	
	FPSCounter _counter;
	std::string	_error;		// Error message if any
	std::string	_filename;	// Source file to capture from if any
	Thread _thread;
	*/


	//void release();
	/*, unsigned flags = 0
	//int _width;				// Capture width
	//int _height;			// Capture height
	//Flaggable _flags;	
	//Flaggable flags() const;
	enum Flag 
		// Settings for different operational modes
	{
		DestroyOnStop		= 0x01, 
		SyncWithDelegates	= 0x02,
		WaitForDelegates	= 0x04
	};
	*/