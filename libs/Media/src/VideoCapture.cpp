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


FastMutex VideoCapture::_mutex;

//
// Windows OpenCV Notes
//
// OpenCV's cv::VideoCapture class does not work well accross thread
// boundaries. For instance, if we initialize a cv::VideoCapture in
// the main thread of of two applications and read from a child thread,
// the read thread of the second application to result in a deadlock.
// However, if both application initialize the capture and read from
// the same thread the second application's capture will output green
// frames until the first application releases the capture.
//	

VideoCapture::VideoCapture(int deviceId, bool checkDevice, bool destroyOnStop) : 
	_thread("VideoCapture"),
	//_capture(cv::VideoCapture(deviceId)),
	_destroyOnStop(destroyOnStop),
	_deviceId(deviceId),
	_width(0),
	_height(0),
	_isOpened(false),
	//_enabled(false),
	_stop(false)
{
	Log("trace") << "[VideoCapture:" << this << "] Initializing: " << deviceId << endl;
	//if (checkDevice)
	//	this->checkDevice();

	start();
}


VideoCapture::VideoCapture(const std::string& filename, bool checkDevice, bool destroyOnStop) : 
	_thread("VideoCapture"),
	//_capture(cv::VideoCapture(filename)),
	_destroyOnStop(destroyOnStop),
	_filename(filename),
	_deviceId(-1),
	_width(0),
	_height(0),
	_isOpened(false),
	//_enabled(false),
	_stop(false) 
{
	Log("trace") << "[VideoCapture:" << this << "] Initializing: " << filename << endl;
	//if (checkDevice)
	//	this->checkDevice();

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


bool VideoCapture::checkDevice()
{
	FastMutex::ScopedLock lock(_mutex);

	// Attempt to capture one frame and read some information
	bool isOpen = _capture.isOpened() ? true : 
		_filename.empty() ? 
			_capture.open(_deviceId) : 
			_capture.open(_filename);		
	
	Log("trace") << "[VideoCapture:" << this << "] Checking Device: " << isOpen << endl;

	if (isOpen) {
		_capture >> _frame;
		_width = _frame.cols;
		_height = _frame.rows;

		// NOTE: Releasing here is necessary because OpenCV
		// frames need to be read inside the same thread 
		// that the capture was opened in.
		_capture.release();
	}
		
	if (!isOpen || !_width || !_height) {
		stringstream ss;
		ss << "Please check your video device: "; 
		_filename.empty() ? (ss << _deviceId) : (ss << _filename);
		throw Exception(ss.str());
	}

	return true;
}


cv::Mat VideoCapture::getFrame(int width, int height)
{
	Log("trace") << "[VideoCapture:" << this << "] Get Frame: " << width << "x" << height << endl;

	FastMutex::ScopedLock lock(_mutex);

	cv::Mat resized;

	if (isOpened()) {
		if (_frame.cols != width || 
			_frame.rows != height) {
			Log("trace") << "[VideoCapture:" << this << "] Get Frame: Resizing..." << endl;
			cv::resize(_frame, resized, cv::Size(width, height));
		} else {
			Log("trace") << "[VideoCapture:" << this << "] Get Frame: Copying..." << endl;
			_frame.copyTo(resized);
		}
	}
	else 
		throw Exception("Please check your video device.");

	return resized;
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
	FastMutex::ScopedLock lock(_mutex);	

	// The Video Capture will be disabled when
	// when the reference count reaches 0.
	if (refCount() == 0 && _destroyOnStop) {		
		Log("trace") << "[VideoCapture:" << this << "] Terminating" << endl;
		delete this;
	}

	Log("trace") << "[VideoCapture:" << this << "] Stopping: OK" << endl;
}


void VideoCapture::attach(const PacketDelegateBase& delegate)
{
	PacketDispatcher::attach(delegate);
	Log("trace") << "[VideoCapture:" << this << "] Added Delegate: " << refCount() << endl;
	//if (!isRunning()) //refCount == 1
		start();
}


void VideoCapture::detach(const PacketDelegateBase& delegate) 
{
	PacketDispatcher::detach(delegate);
	Log("trace") << "[VideoCapture:" << this << "] Removed Delegate: " << refCount() << endl;
	if (refCount() == 0) //isRunning() && 
		stop();
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

	while (!_stop) {
		if (refCount() > 0) {
			try 
			{	
				VideoPacket packet;
				{	
					FastMutex::ScopedLock lock(_mutex);		

					// Stop may be true after we enter this scope.
					if (_stop)
						break;
					
					// Attempt to re-open the capture device if closed.
					_isOpened = _capture.isOpened();
					if (_deviceId >= 0 && !_isOpened) {
						_isOpened = _capture.open(_deviceId);
					}

					// Break the loop if the capture failed. 
					// Perhaps another application is using the device
					// or it has been unplugged?
					if (!_isOpened) {				
						Log("error") << "[VideoCapture:" << this << "] Please check your video camera: " << _deviceId << endl;	
						_stop = true;
						break;
					}

					// Grab a frame from the capture source.
					_capture >> _frame;
					_width = _frame.cols;
					_height = _frame.rows;

					// If we are using a file input and we read to the
					// end of the file subsequent frames will be empty.
					// Just keep looping the input video.
					if (!_filename.empty() && (!_frame.cols || !_frame.rows)) {
						_capture.open(_filename);
						_capture >> _frame;
					}

					// Update our FPS counter.
					_counter.tick();

					packet = VideoPacket(&_frame);
				}
		
				if (!_stop && packet.width && packet.height) {
					/*
					Log("trace") << "[VideoCapture:" << this << "] Broadcasting: " 
						<< "\n\tFPS: " << _counter.fps
						<< "\n\tWidth: " << packet.width
						<< "\n\tHeight: " << packet.height
						<< "\n\tSize: " << packet.size()
						<< endl;
						*/

					dispatch(this, packet);
					//Log("trace") << "[VideoCapture:" << this << "] Broadcasting: OK" << endl;
				}
			} 
			catch (cv::Exception& exc) 
			{
				Log("error") << "[VideoCapture:" << this << "] Error: " << exc.err << endl;

				// TODO: Stop the capture?
			}
		}
		
		// NOTE: Failing to call waitKey inside the capture loop
		// causes strange issues and slowdowns with windows system
		// calls such as ShellExecuteEx and friends.
		cv::waitKey(5);
				
		// Release the CPU.
		Thread::sleep(5);
		
		/*
		// If the capture is disabled keep on looping so we can keep
		// the cv capture alive.
		if (refCount() == 0) {
			Log("trace") << "[VideoCapture:" << this << "] Sleeping" << endl;
			_wakeUp.wait();
			Log("trace") << "[VideoCapture:" << this << "] Waking Up" << endl;
		}	
		*/
	}

	Log("trace") << "[VideoCapture:" << this << "] Exiting" << endl;
	
	// Attempt to free the capture.
	{	
		FastMutex::ScopedLock lock(_mutex);		
		if (_capture.isOpened()) {
			_capture.release();
		}
		_isOpened = false;
	}

	Log("trace") << "[VideoCapture:" << this << "] Exiting: OK" << endl;
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


std::string	VideoCapture::filename() const 
{
	FastMutex::ScopedLock lock(_mutex);
	return _filename; 
}


int VideoCapture::deviceId() const 
{
	FastMutex::ScopedLock lock(_mutex);
	return _deviceId; 
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


} } // namespace Sourcey::Media