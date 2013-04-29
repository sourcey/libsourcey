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


#include "Sourcey/Media/VideoCapture.h"
#include "Sourcey/Logger.h"
#include "Poco/Format.h"


using namespace std;
using namespace Poco;


namespace Sourcey {
namespace Media {


VideoCapture::VideoCapture(int deviceId) : 
	_thread("VideoCapture"),
	_deviceId(deviceId),
	_width(0),
	_height(0),
	_isOpened(false),
	_stop(false)
{
	LogTrace("VideoCapture", this) << "Creating: " << deviceId << endl;
	open();
	start();
}


VideoCapture::VideoCapture(const string& filename) : 
	_thread("VideoCapture"),
	_filename(filename),
	_deviceId(-1),
	_width(0),
	_height(0),
	_isOpened(false),
	_stop(false) 
{
	LogTrace("VideoCapture", this) << "Creating: " << filename << endl;
	open();
	start();
}


VideoCapture::~VideoCapture() 
{	
	LogTrace("VideoCapture", this) << "Destroying" << endl;

	if (_thread.isRunning()) {
		_stop = true;
		//_wakeUp.set();

		// Because we are calling join on the thread
		// this destructor must never be called from
		// within a capture callback or we will result
		// in deadlock.
		_thread.join();
	}

	// Try to release the capture.
	//try { release(); } catch (...) {}

	LogTrace("VideoCapture", this) << "Destroying: OK" << endl;
}


void VideoCapture::start() 
{
	LogTrace("VideoCapture", this) << "Starting" << endl;
	FastMutex::ScopedLock lock(_mutex);

	if (!_thread.isRunning()) {
		LogTrace("VideoCapture", this) << "Initializing Thread" << endl;
		if (!_isOpened)
			throw Exception("The capture must be opened before starting the thread.");

		_stop = false;
		_counter.reset();	
		_thread.start(*this);
	}
	LogTrace("VideoCapture", this) << "Starting: OK" << endl;
}


void VideoCapture::stop() 
{
	LogTrace("VideoCapture", this) << "Stopping" << endl;

	if (_thread.isRunning()) {
		LogTrace("VideoCapture", this) << "Terminating Thread" << endl;		
		_stop = true;
		_thread.join();
	}

	if (flags().has(DestroyOnStop))
		delete this;
}


bool VideoCapture::open()
{
	//LogTrace("VideoCapture", this) << "Open" << endl;

	FastMutex::ScopedLock lock(_mutex);

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
	LogTrace("VideoCapture", this) << "Release" << endl;
	FastMutex::ScopedLock lock(_mutex);
	if (_capture.isOpened())
		_capture.release();
	_isOpened = false;
	LogTrace("VideoCapture", this) << "Release: OK" << endl;
}


cv::Mat VideoCapture::grab()
{	
	//LogTrace("VideoCapture", this) << "Grab" << endl;

	//if (open()) {

		FastMutex::ScopedLock lock(_mutex);	

		// Grab a frame from the capture source.
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

		// Update our FPS counter.
		_counter.tick();

		return _frame;
	//}
}


/*
bool VideoCapture::check()
{	
	grab();	

	FastMutex::ScopedLock lock(_mutex);	

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


void VideoCapture::getFrame(cv::Mat& frame, int width, int height)
{
	LogTrace("VideoCapture", this) << "Get Frame: " << width << "x" << height << endl;

	FastMutex::ScopedLock lock(_mutex);	

	// Don't actually grab a frame here, just copy the current frame.

	if (_isOpened) {
		if ((width && _frame.cols != width) || 
			(height && _frame.rows != height)) {
				cv::resize(_frame, frame, cv::Size(width, height));
		} 
		else
			_frame.copyTo(frame);
	}
	else 
		throw Exception("Please check your video device.");
}


void VideoCapture::attach(const PacketDelegateBase& delegate)
{
	PacketEmitter::attach(delegate);
	LogTrace("VideoCapture", this) << "Added Delegate: " << refCount() << endl;
	if (!isRunning() && flags().has(SyncWithDelegates)) //refCount == 1
		start();
}


bool VideoCapture::detach(const PacketDelegateBase& delegate) 
{
	if (PacketEmitter::detach(delegate)) {
		LogTrace("VideoCapture", this) << "Removed Delegate: " << refCount() << endl;
		if (refCount() == 0 && flags().has(SyncWithDelegates)) //isRunning() && 
			stop();
		return true;
	}
	return false;
}


void VideoCapture::run() 
{
	LogTrace() << "Video Capture Initializing:"		
		<< "\n\tPID: " << this
		<< "\n\tDevice ID: " << _deviceId
		<< "\n\tFilename: " << _filename
		<< "\n\tWidth: " << _width
		<< "\n\tHeight: " << _height
		<< endl;
	
	cv::Mat frame = grab();
	while (!_stop) {
		try {	
			if (refCount() > 0) {
				frame = grab();
				MatPacket packet(&frame);	
				if (!_stop && packet.width && packet.height) {
					/*
					LogTrace("VideoCapture", this) << "Dispatch: " 
					<< "\n\tFPS: " << _counter.fps
					<< "\n\tWidth: " << packet.width
					<< "\n\tHeight: " << packet.height
					<< "\n\tSize: " << packet.size()
					<< endl;
					*/

					emit(this, packet);
					//LogTrace("VideoCapture", this) << "Dispatch: OK" << endl;
				}
				
				// Wait 5ms for the CPU to breathe.
				Thread::sleep(5);
			}

			// Wait 50ms eash iter while we are waiting for delegates.
			else
				Thread::sleep(50);

			// NOTE: Failing to call waitKey inside the capture loop
			// causes strange issues and slowdowns with windows system
			// calls such as ShellExecuteEx and friends.
			cv::waitKey(5);
		}
		catch (cv::Exception& exc) 
		{
			LogError("VideoCapture", this) << "OpenCV Error: " << exc.err << endl;
			break;
		}
		catch (Poco::Exception& exc) 
		{
			LogError("VideoCapture", this) << "Error: " << exc.displayText() << endl;
			break;
		}
		catch (...) 
		{
			LogError("VideoCapture", this) << "Caught Unknown Exception" << endl;
			break;
		}
	}
	

	LogTrace("VideoCapture", this) << "Exiting" << endl;
}


Flags VideoCapture::flags() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _flags;
}


bool VideoCapture::isOpened() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _isOpened;
}


bool VideoCapture::isRunning() const 
{
	FastMutex::ScopedLock lock(_mutex);
	return _thread.isRunning();
}


int VideoCapture::deviceId() const 
{
	FastMutex::ScopedLock lock(_mutex);
	return _deviceId; 
}


string	VideoCapture::filename() const 
{
	FastMutex::ScopedLock lock(_mutex);
	return _filename; 
}


string VideoCapture::name() const 
{
	FastMutex::ScopedLock lock(_mutex);
	stringstream ss;
	_filename.empty() ? (ss << _deviceId) : (ss << _filename);
	return ss.str();
}


int VideoCapture::width() const 
{
	FastMutex::ScopedLock lock(_mutex);
	return _width; 
}


int VideoCapture::height() const 
{
	FastMutex::ScopedLock lock(_mutex);
	return _height; 
}


double VideoCapture::fps() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _counter.fps; 
}


cv::VideoCapture& VideoCapture::capture()
{
	FastMutex::ScopedLock lock(_mutex);
	return _capture; 
}


} } // namespace Sourcey::Media