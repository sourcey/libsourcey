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


#include "scy/media/videocapture.h"
#include "scy/media/mediafactory.h"
#include "scy/logger.h"
#include "scy/platform.h"
#include "scy/util.h"


#ifdef HAVE_OPENCV


namespace scy {
namespace av {


inline std::string exceptionMessage(const std::string& reason)
{
	std::stringstream ss;
	ss << reason;
	if (reason.at(reason.length() - 1) != '.')
		ss << ".";
	ss << " Please ensure the device is properly connected and not in use by another application.";
	return ss.str();
}


//
// Video Capture
//


VideoCapture::VideoCapture(int deviceId) : 
	_deviceId(deviceId),
	_capturing(false),
	_opened(false),
	_started(false),
	_stopping(false)
{
	TraceLS(this) << "Create: " << deviceId << std::endl;
	open();
	start();
}


VideoCapture::VideoCapture(const std::string& filename) : 
	_filename(filename),
	_deviceId(-1),
	_capturing(false),
	_opened(false),
	_started(false),
	_stopping(false)
{
	TraceLS(this) << "Create: " << filename << std::endl;
	open();
	start();
}


VideoCapture::~VideoCapture() 
{	
	TraceLS(this) << "Destroy" << std::endl;
	//stop();

	// Ensure we are not inside the thread context
	assert(Thread::currentID() != _thread.tid());

	// Terminate the internal thread
	if (_thread.started()) {
		TraceLS(this) << "Destroy: Terminating thread" << std::endl;		
		_stopping = true;
		_thread.join();
	}

	// Try to release the capture (automatic once unrefed)
	//try { release(); } catch (...) {}

	TraceLS(this) << "Destroy: OK" << std::endl;
}


void VideoCapture::start() 
{
	TraceLS(this) << "Starting" << std::endl;
	{
		Mutex::ScopedLock lock(_mutex);

		if (!_started) { //
			TraceLS(this) << "Initializing thread" << std::endl;

			// The capture must be opened first.
			// open() must be called from the main thread, 
			// where as start() may be called from any thread.
			if (!_opened)
				throw std::runtime_error("The capture must be opened before starting the thread.");
			
			_started = true;
			_stopping = false;
			_capturing = false;
			_counter.reset();	
			_error = "";

			assert(!_thread.started());
			assert(!_thread.running());
			_thread.start(*this);
		}
	}
	while (!_capturing && error().any()) {
		TraceLS(this) << "Starting: Waiting" << std::endl;
		scy::sleep(10);
	}

	TraceLS(this) << "Starting: OK" << std::endl;
}


void VideoCapture::stop() 
{
	TraceLS(this) << "Stopping" << std::endl;

	// Note: This function no longer has any side effects.
	// Once the capture is running, it will continue to do 
	// so until it is either closed or destroyed.

#if 0
	// The capture can only be stopped when the delegate
	// count reaches zero, therefore in order to ensure 
	// stoppage one must first disconnect all signals.

	assert(Thread::currentID() != _thread.tid());
	if (_started && emitter.ndelegates() == 0) { //_thread.started()
		TraceLS(this) << "Terminating thread" << std::endl;		
		_stopping = true;
		_thread.join();
	}
#endif
}


bool VideoCapture::open(bool whiny)
{
	TraceLS(this) << "Open" << std::endl;
	Mutex::ScopedLock lock(_mutex);
	assert(Thread::currentID() != _thread.tid());
	
	if (_opened && _capture.isOpened())
		return true;
	
	_opened = _capture.isOpened() ? true : 
		_filename.empty() ? 
			_capture.open(_deviceId) : 
			_capture.open(_filename);
	
	if (!_opened && whiny)
		throw std::runtime_error(exceptionMessage("Cannot open the video capture device: " + name()));
	
	TraceLS(this) << "Open: " << _opened << std::endl;
	return _opened;
}


void VideoCapture::run() 
{
	try  {	
		// Grab an initial frame
		cv::Mat frame(grab());
		_capturing = true;
		bool empty = true;
		PacketSignal* next = nullptr;

		TraceLS(this) << "Running:"		
			<< "\n\tDevice ID: " << _deviceId
			<< "\n\tFilename: " << _filename
			<< "\n\tWidth: " << width() 
			<< "\n\tHeight: " << height() << std::endl;		

		while (!_stopping) {
			frame = grab();
			//TraceLS(this) << "Frame: " << frame.rows << "x" << frame.cols << std::endl;

			empty = emitter.ndelegates() == 0;
			if (!empty) {
				TraceLS(this) << "Emitting: " << _counter.fps << std::endl;
				MatrixPacket out(&frame);
				emitter.emit(next, out);
			}

			// Update last frame less often while in limbo
			scy::sleep(empty ? 50 : 3);

			// Always call waitKey otherwise all hell breaks loose
			cv::waitKey(3);
		}	
	}
	catch (cv::Exception& exc) {
		_error.exception = std::current_exception(); // cv::Exception extends std::exception
		setError("OpenCV Error: " + exc.err);
	}

	// TODO: We probably should have a MediaException type in order to 
	// differentiate between internal exceptions and exceptions thrown inside
	// the callback signal. The latter represents a serious application error.
	// Currently they both set the capture to error state.
	catch (std::exception& exc) {
		_error.exception = std::current_exception();
		setError(exc.what());
	}
	
	_started = false;
	_capturing = false;

	// Note: Need to release the capture, otherwise this class instance 
	// cannot be reused ie. the next call to open() will fail.
	TraceLS(this) << "Releasing" << std::endl;
	_capture.release();

	TraceLS(this) << "Exiting" << std::endl;
}


cv::Mat VideoCapture::grab()
{	
	assert(Thread::currentID() == _thread.tid());

	Mutex::ScopedLock lock(_mutex);	
	
	// Grab a frame from the capture source
	// If the capture source is invalid, it will set an invalid frame here
	_capture >> _frame;

	// Keep looping the input video if using file input and we reach eof
	if (!_filename.empty() && (!_frame.cols || !_frame.rows)) {
		_capture.release();
		_capture.open(_filename);
		if (!_capture.isOpened()) {
			assert(0 && "invalid frame");
			throw std::runtime_error(exceptionMessage("Cannot grab video frame: Cannot loop video source: " + name()));
		}
		_capture >> _frame;
	}
		
	if (!_capture.isOpened())
		throw std::runtime_error(exceptionMessage("Cannot grab video frame: Device is closed: " + name()));

	if (!_frame.cols || !_frame.rows)
		throw std::runtime_error(exceptionMessage("Cannot grab video frame: Got an invalid frame from device: " + name()));

	_counter.tick();

	return _frame;
}
	

cv::Mat VideoCapture::lastFrame() const
{
	Mutex::ScopedLock lock(_mutex);

	if (!_opened)
		throw std::runtime_error(error().any() ? error().message : 
			exceptionMessage("Cannot grab video frame: Please check device: " + name()));

	if (!_frame.cols && !_frame.rows)
		throw std::runtime_error(exceptionMessage("Cannot grab video frame: Device is closed: " + name()));

	if (_frame.size().area() <= 0)
		throw std::runtime_error(exceptionMessage("Cannot grab video frame: Invalid source frame: " + name()));

	return _frame; // no data is copied
}


void VideoCapture::getFrame(cv::Mat& frame, int width, int height)
{
	TraceLS(this) << "Get frame: " << width << "x" << height << std::endl;
	
	// Don't actually grab a frame here, just copy the current frame
	// If no valid frame is available an exception will be thrown
	cv::Mat lastFrame = this->lastFrame();
	if ((width && lastFrame.cols != width) || 
		(height && lastFrame.rows != height)) {
		cv::resize(lastFrame, frame, cv::Size(width, height));
	}
	else {
		lastFrame.copyTo(frame);
	}
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


#if 0
void VideoCapture::addEmitter(PacketSignal* emitter)
{
	Mutex::ScopedLock lock(_emitMutex);	
	_emitters.push_back(emitter);	
}


void VideoCapture::removeEmitter(PacketSignal* emitter)  
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
#endif


void VideoCapture::setError(const std::string& error)
{
	ErrorLS(this) << "Set error: " << error << std::endl;
	{
		Mutex::ScopedLock lock(_mutex);	
		_error.message = error;
	}
	Error.emit(this, _error);
}


int VideoCapture::width() 
{
	Mutex::ScopedLock lock(_mutex);	
	return int(_capture.get(CV_CAP_PROP_FRAME_WIDTH)); // not const
}


int VideoCapture::height() 
{
	Mutex::ScopedLock lock(_mutex);	
	return int(_capture.get(CV_CAP_PROP_FRAME_HEIGHT)); // not const
}


bool VideoCapture::opened() const
{
	Mutex::ScopedLock lock(_mutex);
	return _opened;
}


bool VideoCapture::running() const 
{
	Mutex::ScopedLock lock(_mutex);
	return _thread.running();
}


int VideoCapture::deviceId() const 
{
	Mutex::ScopedLock lock(_mutex);
	return _deviceId; 
}


std::string	VideoCapture::filename() const 
{
	Mutex::ScopedLock lock(_mutex);
	return _filename; 
}


std::string VideoCapture::name() const 
{
	Mutex::ScopedLock lock(_mutex);
	std::stringstream ss;
	_filename.empty() ? (ss << _deviceId) : (ss << _filename);
	return ss.str();
}


const scy::Error& VideoCapture::error() const 
{
	Mutex::ScopedLock lock(_mutex);
	return _error;
}


double VideoCapture::fps() const
{
	Mutex::ScopedLock lock(_mutex);
	return _counter.fps; 
}


cv::VideoCapture& VideoCapture::capture()
{
	Mutex::ScopedLock lock(_mutex);
	return _capture; 
}


} } // namespace scy::av


#endif