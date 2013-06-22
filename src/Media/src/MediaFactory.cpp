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


#include "Sourcey/Media/MediaFactory.h"
#include "Sourcey/Logger.h"

#include "RtAudio.h"

using namespace std;
using namespace Poco;
using namespace Scy;


namespace Scy {
namespace Media {


// ---------------------------------------------------------------------
// Media Factory
//
MediaFactory*	MediaFactory::_instance;
FastMutex		MediaFactory::_mutex;


MediaFactory* MediaFactory::instance() 
{
	if (_instance == NULL) 
	{
		FastMutex::ScopedLock lock(_mutex);
		if (_instance == NULL) {
			_instance = new MediaFactory;
		}
	}
	return _instance;
}


void MediaFactory::initialize() 
{
	instance();
}


void MediaFactory::uninitialize() 
{
	if (_instance) {
		delete _instance;
		_instance = NULL;
	}
}


MediaFactory::MediaFactory()
{	
	//cout << "MediaFactory::MediaFactory" << endl;	
	_devices = DeviceManagerFactory::create();
	_devices->initialize();
}


MediaFactory::~MediaFactory()
{	
	//cout << "MediaFactory::~MediaFactory" << endl;	
	if (_devices) {
		_devices->uninitialize();
		delete _devices;
	}
}


IDeviceManager& MediaFactory::devices() 
{ 
	FastMutex::ScopedLock lock(_mutex);
	return *_devices; 
}


FormatRegistry& MediaFactory::formats() 
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _formats; 
}


void MediaFactory::loadVideo(unsigned flags)
{
	LogDebug("MediaFactory") << "Preloading Video Captures" << endl;
	
	// Depreciated code used to preload captures on application load.
	FastMutex::ScopedLock lock(_mutex);

	// Initialize a VideoCapture object for each available device.
	// The video capture object will begin capturing frames when it's
	// reference count becomes positive.
	vector<Device> devs;
	_devices->getVideoCaptureDevices(devs);
	for (size_t i = 0; i < devs.size(); ++i) {
		try 
		{
			LogTrace("MediaFactory") << "Loading Video: " << devs[0].id << endl;

			// TODO: Receive callback on capture error or closure.
			VideoCapture* capture = new VideoCapture(devs[0].id, flags);
			_map[devs[0].id] = capture;
		} 
		catch (...) 
		{
			LogError("MediaFactory") << "Cannot load video capture." << endl;
			_map[devs[0].id] = NULL;
		}
	}
}


void MediaFactory::unloadVideo()
{
	for (VideoCaptureMap::iterator it = _map.begin(); it != _map.end(); ++it) 
		delete it->second;
	_map.clear();
}


VideoCapture* MediaFactory::getVideoCapture(int deviceId, unsigned flags) 
{
	LogTrace("MediaFactory") << "Get Video Capture: " << deviceId << endl;
	FastMutex::ScopedLock lock(_mutex);
	VideoCaptureMap::iterator it = _map.find(deviceId);
	if (it != _map.end())
		return it->second;

	/// Initialize a VideoCapture if none exists.
	/// This may be error prone if not called from the main
	/// thread, which is why loadVideo() should be called.
	_map[deviceId] = new VideoCapture(deviceId, flags);
	return _map[deviceId];
}


VideoCapture* MediaFactory::createFileCapture(const string& file, unsigned flags) 
{
	LogTrace("MediaFactory") << "Get Video Capture: " << file << endl;
	VideoCapture* capture = new VideoCapture(file, flags);
	return capture;
}


AudioCapture* MediaFactory::createAudioCapture(int deviceId, int channels, int sampleRate, RtAudioFormat format) //, bool destroyOnStop
{
	LogTrace("MediaFactory") << "Create Audio Capture: " << deviceId << endl;
	if (deviceId < 0)
		throw Exception("Invalid audio device ID");
	AudioCapture* capture = new AudioCapture(deviceId, channels, sampleRate, format);
	return capture;
}


} } // namespace Scy::Media