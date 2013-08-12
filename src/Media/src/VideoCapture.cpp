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


#include "Sourcey/Media/VideoCapture.h"
#include "Sourcey/Media/MediaFactory.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Platform.h"
#include "Sourcey/Util.h"


namespace scy {
namespace av {

	
VideoCapture::VideoCapture(int deviceId) : 
	_base(MediaFactory::instance().getVideoCaptureBase(deviceId))
{
	traceL("VideoCapture", this) << "Creating: " << deviceId << std::endl;

	_base->addEmitter(&Emitter);
	_base->duplicate();
}


VideoCapture::VideoCapture(const std::string& filename)
{
	traceL("VideoCapture", this) << "Creating: " << filename << std::endl;

	// The file capture is owned by this instance
	_base = new VideoCaptureBase(filename);
	_base->addEmitter(&Emitter);
}


VideoCapture::VideoCapture(VideoCaptureBase* base) : 
	_base(base) 
{
	traceL("VideoCapture", this) << "Creating: " << base << std::endl;
	_base->addEmitter(&Emitter);
	_base->duplicate();
}


VideoCapture::~VideoCapture() 
{
	traceL("VideoCapture", this) << "Destroying" << std::endl;	
	_base->removeEmitter(&Emitter);
	_base->release();
}


void VideoCapture::start() 
{
	traceL("VideoCapture", this) << "Starting" << std::endl;	
	Emitter.enable(true);
}


void VideoCapture::stop() 
{
	traceL("VideoCapture", this) << "Stopping" << std::endl;
	Emitter.enable(false);
}


void VideoCapture::getFrame(cv::Mat& frame, int width, int height)
{
	//traceL("VideoCapture", this) << "Get frame: " << width << "x" << height << std::endl;
	Mutex::ScopedLock lock(_mutex);	
	
	// Don't actually grab a frame here, just copy the current frame.
	cv::Mat lastFrame = _base->lastFrame();
	if ((width && lastFrame.cols != width) || 
		(height && lastFrame.rows != height))
		cv::resize(lastFrame, frame, cv::Size(width, height));
	else
		lastFrame.copyTo(frame);
}

		
void VideoCapture::getEncoderFormat(Format& iformat) 
{
	iformat.name = "OpenCV";
	//iformat.id = "rawvideo";
	iformat.video.encoder = "rawvideo";
	iformat.video.pixelFmt = "bgr24";
	iformat.video.width = width();
	iformat.video.height = height();
	iformat.video.enabled = true;
}


int VideoCapture::width() 
{
	Mutex::ScopedLock lock(_mutex);
	return _base->width(); 
}


int VideoCapture::height() 
{
	Mutex::ScopedLock lock(_mutex);
	return _base->height(); 
}


bool VideoCapture::opened() const
{
	Mutex::ScopedLock lock(_mutex);
	return _base->opened();
}


bool VideoCapture::running() const 
{
	Mutex::ScopedLock lock(_mutex);
	return _base->running();
}


VideoCaptureBase& VideoCapture::base() 
{
	Mutex::ScopedLock lock(_mutex);
	return *_base;
}



//
// Video Capture Base
//


VideoCaptureBase::VideoCaptureBase(int deviceId) : 
	CountedObject(new DeferredDeleter<VideoCaptureBase>()),
	_thread("VideoCaptureBase"),
	_deviceId(deviceId),
	_capturing(false),
	_opened(false),
	_stopping(false)
{
	traceL("VideoCaptureBase", this) << "Creating: " << deviceId << std::endl;
	open();
	start();
}


VideoCaptureBase::VideoCaptureBase(const std::string& filename) : 
	CountedObject(new DeferredDeleter<VideoCaptureBase>()),
	_thread("VideoCaptureBase"),
	_filename(filename),
	_deviceId(-1),
	_capturing(false),
	_opened(false),
	_stopping(false)
{
	traceL("VideoCaptureBase", this) << "Creating: " << filename << std::endl;
	open();
	start();
}


VideoCaptureBase::~VideoCaptureBase() 
{	
	traceL("VideoCaptureBase", this) << "Destroying" << std::endl;

	if (_thread.running()) {
		_stopping = true;

		// Because we are calling join on the thread
		// this destructor must never be called from
		// within a capture callback or we will result
		// in deadlock.
		_thread.join();
	}

	// Try to release the capture (automatic once unrefed)
	//try { release(); } catch (...) {}

	traceL("VideoCaptureBase", this) << "Destroying: OK" << std::endl;
}


void VideoCaptureBase::start() 
{
	traceL("VideoCaptureBase", this) << "Starting" << std::endl;
	{
		Mutex::ScopedLock lock(_mutex);

		if (!_thread.running()) {
			traceL("VideoCaptureBase", this) << "Initializing thread" << std::endl;
			if (!_opened)
				throw Exception("The capture must be opened before starting the thread.");

			_stopping = false;
			_counter.reset();	
			_thread.start(*this);
		}
	}
	while (!_capturing && error().empty()) {
		traceL("VideoCaptureBase", this) << "Starting: Waiting" << std::endl;
		scy::sleep(20);
	}

	traceL("VideoCaptureBase", this) << "Starting: OK" << std::endl;
}


void VideoCaptureBase::stop() 
{
	traceL("VideoCaptureBase", this) << "Stopping" << std::endl;
	if (_thread.running()) {
		traceL("VideoCaptureBase", this) << "Terminating thread" << std::endl;		
		_stopping = true;
		_thread.join();
	}
}


bool VideoCaptureBase::open()
{
	traceL("VideoCaptureBase", this) << "Open" << std::endl;
	Mutex::ScopedLock lock(_mutex);

	_opened = _capture.isOpened() ? true : 
		_filename.empty() ? 
			_capture.open(_deviceId) : 
			_capture.open(_filename);

	if (!_opened) {
		std::stringstream ss;
		ss << "Cannot open the video capture device: "; 
		_filename.empty() ? (ss << _deviceId) : (ss << _filename);
		throw Exception(ss.str());
	}

	return _opened;
}


void VideoCaptureBase::run() 
{
	try 
	{	
		// Grab an initial frame
		cv::Mat frame = grab();
		_capturing = true;
		bool empty = true;
		PacketSignal* next = nil;

		traceL("VideoCaptureBase", this) << "Running:"		
			<< "\n\tDevice ID: " << _deviceId
			<< "\n\tFilename: " << _filename
			<< "\n\tWidth: " << width() 
			<< "\n\tHeight: " << height() << std::endl;		

		while (!_stopping) 
		{	
			{
				int size = 1; // dummy
				for (int idx = 0; idx < size; idx++) {
					Mutex::ScopedLock lock(_emitMutex); 
					size = _emitters.size(); 
					if ((empty = size == 0) || idx >= size)
						break;
					next = _emitters[idx];
	
					//traceL("VideoCaptureBase", this) << "Emitting: " << idx << ": " << _counter.fps << std::endl;
					next->emit(next, MatrixPacket(&frame));
				}
			}
			
			// Update last frame less often while in limbo
			if (empty || !_stopping) {
				frame = grab();
				scy::sleep(50);
			}

			// Always call waitKey otherwise all hell breaks loose
			cv::waitKey(10);
		}	
	}
	catch (cv::Exception& exc) 
	{
		setError("OpenCV Error: " + exc.err);
		assert(0);
	}
	catch (std::exception& exc) 
	{
		// The exception was not properly handled inside the signal
		// callback scope which represents a serious application error.
		// Time to get out the ol debugger!!!
		setError(exc.what());
		assert(0);
	}
	
	traceL("VideoCaptureBase", this) << "Exiting" << std::endl;
	_capturing = false;
}


cv::Mat VideoCaptureBase::grab()
{	
	Mutex::ScopedLock lock(_mutex);	

	// Grab a frame from the capture source
	_capture >> _frame;

	// If using file input and we reach eof the set
	// behavior is to loop the input video.
	if (!_filename.empty() && (!_frame.cols || !_frame.rows)) {
		_capture.open(_filename);
		_capture >> _frame;
	}
		
	if (!_capture.isOpened())
		throw Exception("Cannot grab video from closed device", "Check video device: " + name());

	_counter.tick();
	//_width = _frame.cols;
	//_height = _frame.rows;

	return _frame;
}
	

cv::Mat VideoCaptureBase::lastFrame() const
{
	Mutex::ScopedLock lock(_mutex);

	if (!_opened)
		throw Exception("Cannot grab last video from closed device", (error().length() ? error() : 
			std::string("Check video device: " + name())));

	if (!_frame.cols && !_frame.rows)
		throw Exception("Cannot grab last video from closed device", "Check video device: " + name());

	return _frame; // no data is copied
}


void VideoCaptureBase::addEmitter(PacketSignal* emitter)
{
	Mutex::ScopedLock lock(_emitMutex);	
	_emitters.push_back(emitter);	
}


void VideoCaptureBase::removeEmitter(PacketSignal* emitter)  
{	
	Mutex::ScopedLock lock(_emitMutex);	
	for (PacketSignalVec::iterator it = _emitters.begin(); it != _emitters.end(); ++it) {
		if (*it == emitter) {
			_emitters.erase(it);
			return;
		}
	}
	assert(0 && "unknown emitter");
}


void VideoCaptureBase::setError(const std::string& error)
{
	errorL("VideoCaptureBase", this) << "Setting error: " << error << std::endl;
	Mutex::ScopedLock lock(_mutex);	
	_error = error;
}


int VideoCaptureBase::width() 
{
	Mutex::ScopedLock lock(_mutex);	
	return _capture.get(CV_CAP_PROP_FRAME_WIDTH); // not const
}


int VideoCaptureBase::height() 
{
	Mutex::ScopedLock lock(_mutex);	
	return _capture.get(CV_CAP_PROP_FRAME_HEIGHT); // not const
}


bool VideoCaptureBase::opened() const
{
	Mutex::ScopedLock lock(_mutex);
	return _opened;
}


bool VideoCaptureBase::running() const 
{
	Mutex::ScopedLock lock(_mutex);
	return _thread.running();
}


int VideoCaptureBase::deviceId() const 
{
	Mutex::ScopedLock lock(_mutex);
	return _deviceId; 
}


std::string	VideoCaptureBase::filename() const 
{
	Mutex::ScopedLock lock(_mutex);
	return _filename; 
}


std::string VideoCaptureBase::name() const 
{
	Mutex::ScopedLock lock(_mutex);
	std::stringstream ss;
	_filename.empty() ? (ss << _deviceId) : (ss << _filename);
	return ss.str();
}


std::string VideoCaptureBase::error() const 
{
	Mutex::ScopedLock lock(_mutex);
	return _error;
}


double VideoCaptureBase::fps() const
{
	Mutex::ScopedLock lock(_mutex);
	return _counter.fps; 
}


cv::VideoCapture& VideoCaptureBase::capture()
{
	Mutex::ScopedLock lock(_mutex);
	return _capture; 
}


} } // namespace scy::av



				/*
					//empty = !!size;
				//while (idx < size) {
					//idx++;
				//ScopedLock lock(_emitMutex); 
				if (!_emitters.empty()) {
					frame = grab();
					MatrixPacket packet(&frame);
			
					_insideCallback = true;
					for (unsigned idx = 0; i < _emitters.size(); i++) {
						traceL("VideoCaptureBase", this) << "Emitting: " << _counter.fps << std::endl;
						_emitters[i]->emit(_emitters[i], packet);
					}
					_insideCallback = false;
					continue;
				}
				*/


/*
void VideoCaptureBase::release()
{
	traceL("VideoCaptureBase", this) << "Release" << std::endl;
	Mutex::ScopedLock lock(_mutex);
	if (_capture.opened())
		_capture.release();
	_opened = false;
	traceL("VideoCaptureBase", this) << "Release: OK" << std::endl;
}
*/

			/*
			// Grab a frame if we have delegates or aren't syncing with them
			hasDelegates = emitter().refCount() > 0;
			if (hasDelegates || (!hasDelegates && !syncWithDelegates)) {
				frame = grab();
				MatrixPacket packet(&frame);
				if (hasDelegates && packet.width && packet.height && !_stopping) {
					traceL("VideoCaptureBase", this) << "Emitting: " << _counter.fps << std::endl;
					emit(packet);
				}				
				continue;
			}
			*/


/*
void VideoCaptureBase::attach(const PacketDelegateBase& delegate)
{
	PacketSignal::attach(delegate);
	traceL("VideoCaptureBase", this) << "Added Delegate: " << refCount() << std::endl;
	if (!running() && flags().has(SyncWithDelegates)) //refCount == 1
		start();
}


bool VideoCaptureBase::detach(const PacketDelegateBase& delegate) 
{
	if (PacketSignal::detach(delegate)) {
		traceL("VideoCaptureBase", this) << "Removed Delegate: " << refCount() << std::endl;
		if (refCount() == 0 && flags().has(SyncWithDelegates)) //running() && 
			stop();
		return true;
	}
	return false;
}
*/


/*
Flaggable VideoCaptureBase::flags() const
{
	Mutex::ScopedLock lock(_mutex);
	return _flags;
}
*/


/*
bool VideoCaptureBase::check()
{	
	grab();	

	Mutex::ScopedLock lock(_mutex);	

	// TODO: Check COM is multithreaded when using dshow highgui.

	// Ensure the captured frame is not empty.
	if (!frame.cols ||!frame.rows ) {
		std::stringstream ss;
		ss << "Cannot open video capture, please check device: "; 
		_filename.empty() ? (ss << _deviceId) : (ss << _filename);
		throw Exception(ss.str());
	}

	return true;
}
*/