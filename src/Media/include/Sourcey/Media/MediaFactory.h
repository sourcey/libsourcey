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


#ifndef SOURCEY_MediaFactory_H
#define SOURCEY_MediaFactory_H


#include "Sourcey/Media/Types.h"
#include "Sourcey/Media/VideoCapture.h"
#include "Sourcey/Media/AudioCapture.h"

#include "Poco/Foundation.h"
#include "Poco/Mutex.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <iostream>
#include <map>

#include "Sourcey/Media/DeviceManager.h"
#if !defined(USING_FAKE_DeviceManager) && defined(WIN32)
#include "Sourcey/Media/DeviceManager_WIN32.h"
#elif !defined(USING_FAKE_DeviceManager) && defined(MACOS)
#include "Sourcey/Media/DeviceManager_MAC.h"
#elif !defined(USING_FAKE_DeviceManager) && defined(LINUX)
#include "Sourcey/Media/DeviceManager_LINUX.h"
#else
#include "Sourcey/Media/DeviceManager_FAKE.h"
#endif


namespace Scy {
namespace Media {


class MediaFactory
	/// The MediaFactory class is a singleton for device
	/// enumeration and instantiating audio/video captures. 
{
public:
	static MediaFactory* instance();
	static void initialize();	
	static void uninitialize();

	IDeviceManager& devices();	
		
	void loadVideo();
		/// Preload a VideoCapture instance for each camera.
		/// This method should be called on application initialization,
		/// from the main thread.
		/// This will ensure captures are always available to the
		/// application using getVideoCapture(), from any thread.
	
	void unloadVideo();
		/// Destroy all managed VideoCapture instances.

	virtual VideoCapture* getVideoCapture(int deviceId);
		/// Gets or creates a VideoCapture from given device ID.
		/// Camera captures are managed internally, and must 
		/// NOT be destroyed after use.

	virtual VideoCapture* createVideoCapture(const std::string& file, bool destroyOnStop = false);
		/// Creates a VideoCapture from given source file.
		/// Unline camera captures, file captures must be destroyed after use.
		/// Setting destroyOnStop to true will automatically delete the
		/// instance when it is stopped, or packet delegate count reaches 0.

	virtual AudioCapture* createAudioCapture(int deviceId, 
		int channels = DEFAULT_AUDIO_CHANNELS, 
		int sampleRate = DEFAULT_AUDIO_SAMPLE_RATE, 
		RtAudioFormat format = RTAUDIO_SINT16); //bool destroyOnStop = false
		/// Creates an AudioCapture from given params.
		/// The instance must be destroyed after use.
		/// Setting destroyOnStop to true will automatically delete the
		/// instance when it is stopped, or packet delegate count reaches 0.

protected:
	MediaFactory();
	MediaFactory(MediaFactory const&){};
	MediaFactory& operator=(MediaFactory const&){};
	~MediaFactory();
	
	IDeviceManager*			_devices;
	std::map<int, VideoCapture*> _map;

	static MediaFactory*	_instance;
	static Poco::FastMutex	_mutex;
};


} } // namespace Scy::Media


#endif
