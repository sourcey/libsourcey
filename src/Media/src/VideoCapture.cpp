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
#include "Sourcey/Logger.h"
#include "Poco/Format.h"


using namespace std;
//using namespace Poco;

// TODO: Inner loop timeout recovery procedure

namespace scy {
namespace av {


VideoCapture::VideoCapture(int deviceId, unsigned flags) : 
	_thread("VideoCapture"),
	_deviceId(deviceId),
	_width(0),
	_height(0),
	_flags(flags),
	_capturing(false),
	_isOpened(false),
	_stopping(false)
{
	traceL("VideoCapture", this) << "Creating: " << deviceId << endl;
	open();
	start();
}


VideoCapture::VideoCapture(const string& filename, unsigned flags) : 
	_thread("VideoCapture"),
	_filename(filename),
	_deviceId(-1),
	_width(0),
	_height(0),
	_flags(flags),
	_capturing(false),
	_isOpened(false),
	_stopping(false) 
{
	traceL("VideoCapture", this) << "Creating: " << filename << endl;
	open();
	start();
}


VideoCapture::~VideoCapture() 
{	
	traceL("VideoCapture", this) << "Destroying" << endl;

	if (_thread.isRunning()) {
		_stopping = true;
		//_wakeUp.set();

		// Because we are calling join on the thread
		// this destructor must never be called from
		// within a capture callback or we will result
		// in deadlock.
		_thread.join();
	}

	// Try to release the capture.
	//try { release(); } catch (...) {}

	traceL("VideoCapture", this) << "Destroying: OK" << endl;
}


void VideoCapture::start() 
{
	traceL("VideoCapture", this) << "Starting" << endl;
	{
		Mutex::ScopedLock lock(_mutex);

		if (!_thread.isRunning()) {
			traceL("VideoCapture", this) << "Initializing Thread" << endl;
			if (!_isOpened)
				throw Exception("The capture must be opened before starting the thread.");

			_stopping = false;
			_counter.reset();	
			_thread.start(*this);
		}
	}
		
	// Wait for up to 1 second for initialization.
	// Since the actual capture is already open,
	// all we need to do is wait for the thread.	
	traceL("VideoCapture", this) << "Starting: Wait" << endl;
	_capturing.tryWait(1000);
	traceL("VideoCapture", this) << "Starting: OK" << endl;
}


void VideoCapture::stop() 
{
	traceL("VideoCapture", this) << "Stopping" << endl;
	if (_thread.isRunning()) {
		traceL("VideoCapture", this) << "Terminating Thread" << endl;		
		_stopping = true;
		_thread.join();
	}

	if (flags().has(DestroyOnStop))
		delete this;
}


bool VideoCapture::open()
{
	traceL("VideoCapture", this) << "Open" << endl;
	Mutex::ScopedLock lock(_mutex);

	_isOpened = _capture.isOpened() ? true : 
		_filename.empty() ? 
			_capture.open(_deviceId) : 
			_capture.open(_filename);

	if (!_isOpened) {
		stringstream ss;
		ss << "Cannot open video capture, please check device: "; 
		_filename.empty() ? (ss << _deviceId) : (ss << _filename);
		throw Exception(ss.str());
	}

	return _isOpened;
}


void VideoCapture::release()
{
	traceL("VideoCapture", this) << "Release" << endl;
	Mutex::ScopedLock lock(_mutex);
	if (_capture.isOpened())
		_capture.release();
	_isOpened = false;
	traceL("VideoCapture", this) << "Release: OK" << endl;
}


void VideoCapture::run() 
{
	try 
	{	
		bool hasDelegates = false;
		bool syncWithDelegates = flags().has(SyncWithDelegates);
		traceL("VideoCapture", this) << "Running:"		
			<< "\n\tDevice ID: " << _deviceId
			<< "\n\tFilename: " << _filename
			<< "\n\tWidth: " << _width
			<< "\n\tHeight: " << _height
			<< "\n\tSyncWithDelegates: " << syncWithDelegates
			<< endl;
	
		cv::Mat frame = grab();
		while (!_stopping) 
		{	
			// NOTE: Failing to call waitKey inside the capture 
			// loop causes strange issues and slowdowns with windows 
			// system calls such as ShellExecuteEx and friends.
			cv::waitKey(5);

			// Grab a frame if we have delegates or aren't syncing with them
			hasDelegates = refCount() > 0;
			if (hasDelegates || (!hasDelegates && !syncWithDelegates)) {

				//cv::Mat resized;
				//cv::resize(frame, resized, cv::Size(300, 200));				
				//MatPacket packet(&resized);
				
				frame = grab();
				MatPacket packet(&frame);
				if (hasDelegates && packet.width && packet.height) { //!_stopping && 
					//traceL("VideoCapture", this) << "Emitting: " << _counter.fps << endl;
					emit(packet);					
					//traceL("VideoCapture", this) << "Emitting: OK" << endl;
				}
				
				// Wait 5ms for the CPU to breathe
				Poco::Thread::sleep(5);
			}

			// Wait 50ms each iter while in limbo
			else
				Poco::Thread::sleep(50);
		}	
	}
	catch (cv::Exception& exc) 
	{
		setError("OpenCV Error: " + exc.err);
	}
	catch (Exception& exc) 
	{
		// If we make it here an exception was not properly 
		// handled inside the signal callback scope which
		// represents a serious application error.
		assert(0);
		setError(exc.message());
	}
	catch (...) 
	{
		assert(0);
		setError("Unknown error.");
	}
	
	_capturing.reset();
	traceL("VideoCapture", this) << "Exiting" << endl;
}


void VideoCapture::getFrame(cv::Mat& frame, int width, int height)
{
	traceL("VideoCapture", this) << "Get Frame: " << width << "x" << height << endl;
	Mutex::ScopedLock lock(_mutex);	

	// Don't actually grab a frame here, just copy the current frame.
	// NOTE: If the WaitForDelegates flag is set, and no delegates are 
	// listening the current frame will not update, consider removing the flag.
	if (!_isOpened)
		throw Exception("Video capture failed: " + (error().length() ? error() : string("Video device not open.")));

	if (!_frame.cols && !_frame.rows)
		throw Exception("Video capture failed: Empty video frame.");

	if ((width && _frame.cols != width) || 
		(height && _frame.rows != height))
		cv::resize(_frame, frame, cv::Size(width, height));
	else
		_frame.copyTo(frame);
}


void VideoCapture::attach(const PacketDelegateBase& delegate)
{
	PacketSignal::attach(delegate);
	traceL("VideoCapture", this) << "Added Delegate: " << refCount() << endl;
	if (!isRunning() && flags().has(SyncWithDelegates)) //refCount == 1
		start();
}


bool VideoCapture::detach(const PacketDelegateBase& delegate) 
{
	if (PacketSignal::detach(delegate)) {
		traceL("VideoCapture", this) << "Removed Delegate: " << refCount() << endl;
		if (refCount() == 0 && flags().has(SyncWithDelegates)) //isRunning() && 
			stop();
		return true;
	}
	return false;
}


cv::Mat VideoCapture::grab()
{	
	//traceL("VideoCapture", this) << "Grabing" << endl;
	Mutex::ScopedLock lock(_mutex);	

	// Grab a frame from the capture source
	_capture >> _frame;

	// If we are using a file input and we read to the
	// end of the file subsequent frames will be empty.
	// Just keep looping the input video.
	if (!_filename.empty() && (!_frame.cols || !_frame.rows)) {
		_capture.open(_filename);
		_capture >> _frame;
	}
		
	if (!_capture.isOpened())
		throw Exception("The capture is closed.");

	_width = _frame.cols;
	_height = _frame.rows;

	// Send the capturing event to notify the main thread
	_capturing.set();
	_counter.tick();

	return _frame;
}


void VideoCapture::setError(const string& error)
{
	errorL("VideoCapture", this) << error << endl;
	Mutex::ScopedLock lock(_mutex);	
	_error = error;
}


bool VideoCapture::isOpened() const
{
	Mutex::ScopedLock lock(_mutex);
	return _isOpened;
}


bool VideoCapture::isRunning() const 
{
	Mutex::ScopedLock lock(_mutex);
	return _thread.isRunning();
}


int VideoCapture::deviceId() const 
{
	Mutex::ScopedLock lock(_mutex);
	return _deviceId; 
}


string	VideoCapture::filename() const 
{
	Mutex::ScopedLock lock(_mutex);
	return _filename; 
}


string VideoCapture::name() const 
{
	Mutex::ScopedLock lock(_mutex);
	stringstream ss;
	_filename.empty() ? (ss << _deviceId) : (ss << _filename);
	return ss.str();
}


string VideoCapture::error() const 
{
	Mutex::ScopedLock lock(_mutex);
	return _error;
}


int VideoCapture::width() const 
{
	Mutex::ScopedLock lock(_mutex);
	return _width; 
}


int VideoCapture::height() const 
{
	Mutex::ScopedLock lock(_mutex);
	return _height; 
}


double VideoCapture::fps() const
{
	Mutex::ScopedLock lock(_mutex);
	return _counter.fps; 
}


Flags VideoCapture::flags() const
{
	Mutex::ScopedLock lock(_mutex);
	return _flags;
}


cv::VideoCapture& VideoCapture::capture()
{
	Mutex::ScopedLock lock(_mutex);
	return _capture; 
}


} } // namespace scy::av





/*
bool VideoCapture::check()
{	
	grab();	

	Mutex::ScopedLock lock(_mutex);	

	// TODO: Check COM is multithreaded when using dshow highgui.

	// Ensure the captured frame is not empty.
	if (!frame.cols ||!frame.rows ) {
		stringstream ss;
		ss << "Cannot open video capture, please check device: "; 
		_filename.empty() ? (ss << _deviceId) : (ss << _filename);
		throw Exception(ss.str());
	}

	return true;
}
*/