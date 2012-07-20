#include "Sourcey/Media/FileCapture.h"


using namespace std;
using namespace Poco;


namespace Sourcey {
namespace Media {


// ============================================================================
//
// File Capture
//
// ============================================================================
//FastMutex FileCapture::_mutex;


FileCapture::FileCapture(std::string filename) : 
	VideoCapture(-1),
	//_thread("FileCapture"),
	_capture(cv::VideoCapture(filename)),
	_filename(filename)
	//_width(0),
	//_height(0),
	//_stop(false)
{
	/*
	// Attempt to capture one frame and read some information
	cv::Mat frame;
	_capture >> frame;
	_width = frame.cols;
	_height = frame.rows;
	_capture.release();

	// assert(_width > 0);
	// assert(_height > 0);
	if (!_width || !_height)
		throw Exception(format("Unable to initialize capture for video device #%d", _filename));
		*/
}

/*
FileCapture::~FileCapture() 
{	
	if (_thread.isRunning()) {
		_stop = true;
		_thread.join();
	}
	while (!_callbacks.empty()) 
		delete _callbacks.back(), _callbacks.pop_back();
}


void FileCapture::start(const AbstractVideoCallback& method) 
{
	FastMutex::ScopedLock lock(_mutex);
	_callbacks.push_back(method.clone());
	cout << "[FileCapture] start(): " << _callbacks.size() << endl;
	if (!_thread.isRunning() && !_callbacks.empty()) {
		_stop = false;
		_thread.start(*this);
	}
}


void FileCapture::stop(const void* object)
{	
	FastMutex::ScopedLock lock(_mutex);

	for (VideoCallbackList::iterator it = _callbacks.begin(); 
		it != _callbacks.end(); ++it) {		
		if ((*it)->object() == object) {
			delete *it;
			_callbacks.erase(it);
			cout << "[FileCapture] stop(): STOPPED:" << object << endl;
			break;
		}
	}
	cout << "[FileCapture] stop(): " << _callbacks.size() << endl;
}


bool FileCapture::isRunning() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _thread.isRunning();
}


int FileCapture::filename() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _filename; 
}


int FileCapture::width() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _width; 
}


int FileCapture::height() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _height; 
}


int FileCapture::refCount() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _callbacks.size(); 
}


void FileCapture::run()
{
	cout << "Video Capture Initializing:"		
		<< "\n\tPID: " << this
		<< "\n\tDevice ID: " << _filename
		<< "\n\tWidth: " << _width
		<< "\n\tHeight: " << _height
		<< endl;

	cv::Mat frame;
	while (!_stop && !_callbacks.empty())
	{
		try 
		{
			if (!_capture.isOpened())
				_capture.open(_filename);
			_capture >> frame;

			for (unsigned int i = 0; i < _callbacks.size(); ++i)
			{
				FastMutex::ScopedLock lock(_mutex);
				//cout << "FileCapture: callback" << endl;
				_callbacks[i]->invoke(frame);
				//cout << "FileCapture: callback AFTER" << endl;
			}

			Thread::sleep(10);
			//cv::waitKey(10);
		} 
		catch (...) 
		{
			cout << "FileCapture: ERROR" << endl;
		}
	}
	
	if (_capture.isOpened())
		_capture.release();

	cout << "FileCapture: Exiting..." << endl;
}
*/


} } // namespace Sourcey::Media
