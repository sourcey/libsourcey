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


#include "Sourcey/Media/MediaFactory.h"
#include "Sourcey/Logger.h"


using namespace std;
using namespace Poco;
using namespace Sourcey;


namespace Sourcey {
namespace Media {


// ---------------------------------------------------------------------
//
// Media Factory
//
// ---------------------------------------------------------------------
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


MediaFactory::MediaFactory() :
	video(this),
	audio(this)
{	
	cout << "MediaFactory::MediaFactory" << endl;	
	_devices = DeviceManagerFactory::create();
	_devices->initialize();
}


MediaFactory::~MediaFactory()
{	
	cout << "MediaFactory::~MediaFactory" << endl;
}


void MediaFactory::loadVideo() 
{
	video.load();
}


void MediaFactory::loadAudio() 
{
	audio.load();
}


void MediaFactory::unloadVideo()
{
	video.unload();
}


void MediaFactory::unloadAudio()
{
	audio.unload();
}


IDeviceManager& MediaFactory::devices() 
{ 
	FastMutex::ScopedLock lock(_mutex);
	return *_devices; 
}


// ---------------------------------------------------------------------
//
// Media Factory Video
//
// ---------------------------------------------------------------------
MediaFactory::Video::Video(MediaFactory* factory) :
	_factory(factory)
{
}


MediaFactory::Video::~Video()
{	
	unload();
}


void MediaFactory::Video::unload()
{
	for (VideoCaptureMap::iterator it = _map.begin(); it != _map.end(); ++it) 
		delete it->second;
	_map.clear();
}


void MediaFactory::Video::load()
{
	// Initialize an idle VideoCapture object for each available device.
	// The video capture object will begin capturing frames when it's
	// reference count becomes positive.
	std::vector<Device> devs;
	_factory->devices().getVideoCaptureDevices(devs);
	for (size_t i = 0; i < devs.size(); ++i) {
		try 
		{
			// TODO: Receive callback on capture error or closure.
			VideoCapture* capture = new VideoCapture(devs[0].id);
			_map[devs[0].id] = capture;
		} 
		catch (...) 
		{
			//Log("error") << exc.displayText() << endl;
			_map[devs[0].id] = 0;
		}
	}
}


// NOTE: Video devices _must_ be initialized from the main thread or they 
// will not be available from other threads. Due to the fact that we are 
// storing initialized devices correct initialization is very important.
VideoCapture* MediaFactory::Video::getCapture(int deviceId) 
{
	FastMutex::ScopedLock lock(_mutex);
	VideoCaptureMap::iterator it = _map.find(deviceId);
	if (it != _map.end())
		return it->second;

	_map[deviceId] = new VideoCapture(deviceId);
	return _map[deviceId];
}


// WARNING: File video captures are started with destroyOnStop set
// to true, meaning that the capture will be destroyed as soon as
// it's reference count reaches 0.
VideoCapture* MediaFactory::Video::getCapture(const std::string& file) 
{
	return new VideoCapture(file, true, true);
}


bool MediaFactory::Video::closeCapture(int deviceId)
{
	FastMutex::ScopedLock lock(_mutex);

	VideoCaptureMap::iterator it = _map.find(deviceId);
	if (it != _map.end()) {
		delete it->second;
		_map.erase(it);
		return true;
	}
	return false;
}


/*
void MediaFactory::Video::onRefCountChange(const void* pSender, int& refCount) {	
	VideoCapture* capture = const_cast<VideoCapture*>(reinterpret_cast<const VideoCapture*>(pSender));

	// We do not want vagabond file captures hanging around
	if (capture->refCount() == 0 &&
		!capture->filename().empty())
		delete 
}
*/


// ---------------------------------------------------------------------
//
// Media Factory Audio
//
// ---------------------------------------------------------------------
MediaFactory::Audio::Audio(MediaFactory* factory) :
	_factory(factory)
{
}


MediaFactory::Audio::~Audio()
{	
	unload();
}


void MediaFactory::Audio::unload()
{
	// Nothing to do... AudioCapture instances are not managed like 
	// VideoCapture instances.
}


void MediaFactory::Audio::load()
{
	// Nothing to do... AudioCapture instances are not managed like 
	// VideoCapture instances.
}


AudioCapture* MediaFactory::Audio::getCapture(int deviceId, int channels, int sampleRate)
{
	return new AudioCapture(deviceId, channels, sampleRate);
}


} } // namespace Sourcey::Media


	/*
	FastMutex::ScopedLock lock(_mutex);
	AudioCaptureMap::iterator it = _map.find(deviceId);
	if (it != _map.end()) {
		return it->second;
	} 
	else {
		Log("debug") << "MediaFactory: Attempting to start capture on unregistered audio device." << endl;
		_map[deviceId] = new AudioCapture(deviceId, 2, 44100);
		return _map[deviceId];
	}
	*/


	//for (AudioCaptureMap::iterator it = _map.begin(); it != _map.end(); ++it) 
	//	delete it->second;
	//_map.clear();

	/*
	// Initialize an idle AudioCapture object for each available device.
	// The audio capture object will begin capturing frames when it's
	// reference count becomes positive.
	std::vector<Device> devs;
	_factory->devices();	
	_factory->devices().getAudioInputDevices(&devs);		
	for (size_t i = 0; i < devs.size(); ++i) {	
		_map[devs[0].id] = new AudioCapture(devs[0].id, 2, 44100);
		// TODO: Receive callback on capture error or closure.
	}
	*/