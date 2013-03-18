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
	_capture(cv::VideoCapture(deviceId)),
	_deviceId(deviceId),
	_width(0),
	_height(0),
	_destroyOnStop(false),
	_isOpened(false),
	_stop(false)
{
	Log("trace") << "[VideoCapture:" << this << "] Creating: " << deviceId << endl;
	check();
	start();
}


VideoCapture::VideoCapture(const string& filename) : 
_thread("VideoCapture"),
	_capture(cv::VideoCapture(filename)),
	_filename(filename),
	_deviceId(-1),
	_width(0),
	_height(0),
	_destroyOnStop(false),
	_isOpened(false),
	_stop(false) 
{
	Log("trace") << "[VideoCapture:" << this << "] Creating: " << filename << endl;
	check();
	start();
}


VideoCapture::~VideoCapture() 
{	
	Log("trace") << "[VideoCapture:" << this << "] Destroying" << endl;

	if (_thread.isRunning()) {
		_stop = true;		
		//_wakeUp.set();

		// Because we are calling join on the thread
		// this destructor must never be called from
		// within a capture callback or we will result
		// in deadlock.
		_thread.join();
	}

	Log("trace") << "[VideoCapture:" << this << "] Destroying: OK" << endl;
}


void VideoCapture::start() 
{
	Log("trace") << "[VideoCapture:" << this << "] Starting" << endl;
	FastMutex::ScopedLock lock(_mutex);

	//_enabled = true;

	if (!_thread.isRunning()) {
		Log("trace") << "[VideoCapture:" << this << "] Initializing Thread" << endl;
		_stop = false;
		_counter.reset();	
		_thread.start(*this);
	}
	//_wakeUp.set();
	Log("trace") << "[VideoCapture:" << this << "] Starting: OK" << endl;
}


void VideoCapture::stop() 
{
	Log("trace") << "[VideoCapture:" << this << "] Stopping" << endl;
	//FastMutex::ScopedLock lock(_mutex);	

	// The capture is never stopped, but when the
	// reference count reaches 0 and destroyOnStop
	// is set we destroy the capture.
	if (refCount() == 0 && destroyOnStop()) {
		delete this;
	}
}


bool VideoCapture::open()
{
	//Log("trace") << "[VideoCapture:" << this << "] Open" << endl;

	FastMutex::ScopedLock lock(_mutex);

	_isOpened = _capture.isOpened() ? true : 
		_filename.empty() ? 
		_capture.open(_deviceId) : 
	_capture.open(_filename);

	if (!_isOpened) {
		stringstream ss;
		ss << "Please check your video device: "; 
		_filename.empty() ? (ss << _deviceId) : (ss << _filename);
		throw Exception(ss.str());
	}

	return _isOpened;
}


void VideoCapture::release()
{
	Log("trace") << "[VideoCapture:" << this << "] Release" << endl;
	FastMutex::ScopedLock lock(_mutex);
	Log("trace") << "[VideoCapture:" << this << "] Release 1" << endl;
	if (_capture.isOpened())
		_capture.release();
	_isOpened = false;
	Log("trace") << "[VideoCapture:" << this << "] Release: OK" << endl;
}


void VideoCapture::grab()
{	
	//Log("trace") << "[VideoCapture:" << this << "] Grab" << endl;

	if (open()) {

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

		_width = _frame.cols;
		_height = _frame.rows;

		// Update our FPS counter.
		_counter.tick();
	}
}


bool VideoCapture::check()
{	
	grab();	

	FastMutex::ScopedLock lock(_mutex);	

	// TODO: Check COM is multithreaded when using dshow highgui.

	// Ensure the captured frame is not empty.
	if (!_frame.cols ||!_frame.rows ) {
		stringstream ss;
		ss << "Please check your video device: "; 
		_filename.empty() ? (ss << _deviceId) : (ss << _filename);
		throw Exception(ss.str());
	}

	return true;
}


void VideoCapture::getFrame(cv::Mat& frame, int width, int height)
{
	Log("trace") << "[VideoCapture:" << this << "] Get Frame: " << width << "x" << height << endl;

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
	PacketDispatcher::attach(delegate);
	Log("trace") << "[VideoCapture:" << this << "] Added Delegate: " << refCount() << endl;
	if (!isRunning()) //refCount == 1
		start();
}


bool VideoCapture::detach(const PacketDelegateBase& delegate) 
{
	if (PacketDispatcher::detach(delegate)) {
		Log("trace") << "[VideoCapture:" << this << "] Removed Delegate: " << refCount() << endl;
		if (refCount() == 0) //isRunning() && 
			stop();
		return true;
	}
	return false;
}


void VideoCapture::run() 
{
	Log("trace") << "Video Capture Initializing:"		
		<< "\n\tPID: " << this
		<< "\n\tDevice ID: " << _deviceId
		<< "\n\tFilename: " << _filename
		<< "\n\tWidth: " << _width
		<< "\n\tHeight: " << _height
		<< endl;

	try 
	{	
		while (!_stop) {
			try 
			{	
				if (refCount() > 0) {
					grab();
					MatPacket packet(&_frame);		
					if (!_stop && packet.width && packet.height) {
						/*
						Log("trace") << "[VideoCapture:" << this << "] Dispatch: " 
						<< "\n\tFPS: " << _counter.fps
						<< "\n\tWidth: " << packet.width
						<< "\n\tHeight: " << packet.height
						<< "\n\tSize: " << packet.size()
						<< endl;
						*/

						dispatch(this, packet);
						//Log("trace") << "[VideoCapture:" << this << "] Dispatch: OK" << endl;
					}
				}
			} 
			catch (cv::Exception& exc) 
			{
				Log("error") << "[VideoCapture:" << this << "] OpenCV Error: " << exc.err << endl;
				// TODO: Stop the capture?
			}
			catch (Poco::Exception& exc) 
			{
				Log("error") << "[VideoCapture:" << this << "] Error: " << exc.displayText() << endl;
				exc.rethrow();
			}
			catch (...) 
			{
				Log("error") << "[VideoCapture:" << this << "] Caught Unknown Exception" << endl;
				throw;
			}
			//}

			// NOTE: Failing to call waitKey inside the capture loop
			// causes strange issues and slowdowns with windows system
			// calls such as ShellExecuteEx and friends.
			cv::waitKey(5);

			// Release the CPU.
			Thread::sleep(5);
		}

		Log("trace") << "[VideoCapture:" << this << "] Exiting" << endl;
	}
	catch (...) 
	{
		// Exit
	}

	// Always try to release the capture.
	try { release(); } catch (...) {}

	Log("trace") << "[VideoCapture:" << this << "] Exiting: OK" << endl;
}


void VideoCapture::setDestroyOnStop(bool flag)
{
	FastMutex::ScopedLock lock(_mutex);
	_destroyOnStop = flag;
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


string	VideoCapture::name() const 
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


bool VideoCapture::destroyOnStop() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _destroyOnStop; 
}


cv::VideoCapture& VideoCapture::capture()
{
	FastMutex::ScopedLock lock(_mutex);
	return _capture; 
}


} } // namespace Sourcey::Media



/*
{	
open();

FastMutex::ScopedLock lock(_mutex);		

// Stop may be true after we enter this scope.
if (_stop)
break;

// Break the loop if the capture failed. 
// Perhaps another application is using the device
// or it has been unplugged?
if (!_isOpened) {				
Log("error") << "[VideoCapture:" << this << "] Please check your video camera" << endl;	
_stop = true;
break;
}

// Attempt to re-open the capture device if closed.
//_isOpened = _capture.isOpened();
//if (_deviceId >= 0 && !_isOpened) {
//	_isOpened = _capture.open(_deviceId);
//}

packet = VideoPacket(&_frame);
}
*/

/*
// If the capture is disabled keep on looping so we can keep
// the cv capture alive.
if (refCount() == 0) {
Log("trace") << "[VideoCapture:" << this << "] Sleeping" << endl;
_wakeUp.wait();
Log("trace") << "[VideoCapture:" << this << "] Waking Up" << endl;
}	
*/


/*
_capture >> _frame;
_width = _frame.cols;
_height = _frame.rows;

bool isOpen = _capture.isOpened() ? true : 
_filename.empty() ? 
_capture.open(_deviceId) : 
_capture.open(_filename);	

if (_isOpened) {
*/

// NOTE: Releasing here is necessary because OpenCV
// frames need to be read inside the same thread 
// that the capture was opened in.
// NOTE1: We now require MT Com initialization under
// windows so this is no longer an issue.
//_capture.release();

/*
return true;
}

return false;
*/