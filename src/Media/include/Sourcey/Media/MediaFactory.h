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


#ifndef SOURCEY_MediaFactory_H
#define SOURCEY_MediaFactory_H


#include "Sourcey/Media/Types.h"
#include "Sourcey/Media/VideoCapture.h"
#include "Sourcey/Media/AudioCapture.h"
#include "Sourcey/Media/FormatRegistry.h"

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


namespace scy {
namespace av {


class MediaFactory
	/// The MediaFactory class is a singleton for device
	/// enumeration and instantiating audio/video captures. 
{
public:
	static MediaFactory* instance();
	static void initialize();	
	static void uninitialize();
		
	void loadVideo(unsigned flags = 0);
		/// Preload a VideoCapture instance for each camera.
		/// This method should be called on application initialization,
		/// from the main thread.
		/// This will ensure captures are always available to the
		/// application using getVideoCapture(), from any thread.
	
	void unloadVideo();
		/// Destroy all managed VideoCapture instances.

	virtual VideoCapture* getVideoCapture(int deviceId, unsigned flags = 0);
		/// Gets or creates a VideoCapture from given device ID.
		/// Camera captures are managed internally, and must 
		/// NOT be destroyed after use.

	virtual VideoCapture* createFileCapture(const std::string& file, unsigned flags = 0);
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
	
	IDeviceManager& devices();	
	FormatRegistry& formats();	

protected:
	MediaFactory();
	MediaFactory(MediaFactory const&){};
	MediaFactory& operator=(MediaFactory const&){};
	~MediaFactory();
	
	IDeviceManager*			_devices;
	FormatRegistry			_formats;	
	std::map<int, VideoCapture*> _map;

	static MediaFactory*	_instance;
	static Poco::FastMutex	_mutex;
};


} } // namespace scy::av


#endif
