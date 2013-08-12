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


using namespace std;
using namespace scy;


namespace scy {
namespace av {

	
static Singleton<MediaFactory> singleton;
	

MediaFactory& MediaFactory::instance() 
{
	return *singleton.get();
}

	
void MediaFactory::shutdown()
{
	singleton.destroy();
}


MediaFactory::MediaFactory()
{	
	_devices = DeviceManagerFactory::create();
	_devices->initialize();
}


MediaFactory::~MediaFactory()
{	
	if (_devices) {
		_devices->uninitialize();
		delete _devices;
	}
}


IDeviceManager& MediaFactory::devices() 
{ 
	Mutex::ScopedLock lock(_mutex);
	return *_devices; 
}


FormatRegistry& MediaFactory::formats() 
{ 
	Mutex::ScopedLock lock(_mutex);
	return _formats; 
}


void MediaFactory::loadVideo()
{
	traceL("MediaFactory") << "Preloading video captures" << endl;
	
	// Depreciated code used to preload captures on application load.
	Mutex::ScopedLock lock(_mutex);

	// Initialize a VideoCapture object for each available device.
	// The video capture object will begin capturing frames when it's
	// reference count becomes positive.
	std::vector<Device> devs;
	_devices->getVideoCaptureDevices(devs);
	for (size_t i = 0; i < devs.size(); ++i) {
		try 
		{
			traceL("MediaFactory") << "Loading video: " << devs[0].id << endl;

			// TODO: Receive callback on capture error or closure.
			VideoCaptureBase* base = new VideoCaptureBase(devs[0].id); 
			_videoBases[devs[0].id] = base; 
		} 
		catch (...) 
		{
			errorL("MediaFactory") << "Cannot load video capture" << endl;
		}
	}
}


void MediaFactory::unloadVideo()
{
	for (VideoCaptureBaseMap::iterator it = _videoBases.begin(); it != _videoBases.end(); ++it) {
		assert(it->second->refCount() == 1);
		it->second->release();
	}
	_videoBases.clear();
}


VideoCaptureBase* MediaFactory::getVideoCaptureBase(int deviceId) 
{
	//traceL("MediaFactory") << "Get video capture base: " << deviceId << endl;
	VideoCaptureBase* base;
	VideoCaptureBaseMap::iterator it = _videoBases.find(deviceId);
	if (it != _videoBases.end())
		base = it->second;
	else {	
		// TODO: unique_ptr for exception safe instantiation
		base = new VideoCaptureBase(deviceId);
		_videoBases[deviceId] = base;
	}
	return base;
}


VideoCapture* MediaFactory::createVideoCapture(int deviceId) //, unsigned flags
{
	//traceL("MediaFactory") << "Get video capture: " << deviceId << endl;
	return new VideoCapture(getVideoCaptureBase(deviceId));
}


VideoCapture* MediaFactory::createFileCapture(const string& file)
{
	traceL("MediaFactory") << "Get video capture: " << file << endl;
	
	// TODO: unique_ptr for exception safe instantiation
	VideoCapture* capture = new VideoCapture(file);
	return capture;
}


AudioCapture* MediaFactory::createAudioCapture(int deviceId, int channels, int sampleRate, RtAudioFormat format)
{
	traceL("MediaFactory") << "Create audio capture: " << deviceId << endl;
	if (deviceId < 0)
		throw Exception("Invalid audio device ID");

	// TODO: unique_ptr for exception safe instantiation
	AudioCapture* capture = new AudioCapture(deviceId, channels, sampleRate, format);
	return capture;
}


} } // namespace scy::av