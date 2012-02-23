//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2002 Sourcey
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


#include "Sourcey/Media/CaptureFactory.h"
#include "Sourcey/Logger.h"


using namespace std;
using namespace Poco;
using namespace Sourcey;


namespace Sourcey {
namespace Media {


// ---------------------------------------------------------------------
//
// Capture Factory
//
// ---------------------------------------------------------------------
CaptureFactory*	CaptureFactory::_instance;
FastMutex		CaptureFactory::_mutex;


CaptureFactory* CaptureFactory::instance() 
{
	if (_instance == NULL) 
	{
		FastMutex::ScopedLock lock(_mutex);
		if (_instance == NULL) {
			_instance = new CaptureFactory;
		}
	}
	return _instance;
}


void CaptureFactory::initialize() 
{
	instance();
}


void CaptureFactory::uninitialize() 
{
	if (_instance) {
		delete _instance;
		_instance = NULL;
	}
}


CaptureFactory::CaptureFactory() :
	video(this),
	audio(this)
{	
	//Log("debug") << "CaptureFactory::CaptureFactory" << endl;	
	_devices->initialize();
}


CaptureFactory::~CaptureFactory()
{	
	//Log("debug") << "CaptureFactory::~CaptureFactory" << endl;
}


void CaptureFactory::loadVideo() 
{
	video.load();
}


void CaptureFactory::loadAudio() 
{
	audio.load();
}


void CaptureFactory::unloadVideo()
{
	video.unload();
}


void CaptureFactory::unloadAudio()
{
	audio.unload();
}


DeviceManager& CaptureFactory::devices() 
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _devices; 
}


// ---------------------------------------------------------------------
//
// Media Factory Video
//
// ---------------------------------------------------------------------
CaptureFactory::Video::Video(CaptureFactory* factory) :
	_factory(factory)
{
}


CaptureFactory::Video::~Video()
{	
	unload();
}


void CaptureFactory::Video::unload()
{
	for (VideoCaptureMap::iterator it = _map.begin(); it != _map.end(); ++it) 
		delete it->second;
	_map.clear();
}


void CaptureFactory::Video::load()
{
	// Initialize an idle VideoCapture object for each available device.
	// The video capture object will begin capturing frames when it's
	// reference count becomes positive.
	std::vector<Device> devs;
	_factory->devices()->getVideoInputDevices(devs);
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
VideoCapture* CaptureFactory::Video::getCapture(int deviceId) 
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
VideoCapture* CaptureFactory::Video::getCapture(const std::string& file) 
{
	return new VideoCapture(file, true, true);
}


bool CaptureFactory::Video::closeCapture(int deviceId)
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
void CaptureFactory::Video::onRefCountChange(const void* pSender, int& refCount) {	
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
CaptureFactory::Audio::Audio(CaptureFactory* factory) :
	_factory(factory)
{
}


CaptureFactory::Audio::~Audio()
{	
	unload();
}


void CaptureFactory::Audio::unload()
{
	// Nothing to do... AudioCapture instances are not managed like 
	// VideoCapture instances.
}


void CaptureFactory::Audio::load()
{
	// Nothing to do... AudioCapture instances are not managed like 
	// VideoCapture instances.
}


AudioCapture* CaptureFactory::Audio::getCapture(int deviceId, int channels, int sampleRate)
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
		Log("debug") << "CaptureFactory: Attempting to start capture on unregistered audio device." << endl;
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
	_factory->devices()->getAudioInputDevices(&devs);		
	for (size_t i = 0; i < devs.size(); ++i) {	
		_map[devs[0].id] = new AudioCapture(devs[0].id, 2, 44100);
		// TODO: Receive callback on capture error or closure.
	}
	*/