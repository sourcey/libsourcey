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
#include "Sourcey/Mutex.h"

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
	/// The MediaFactory class is a singleton manager for audio/video
	/// captures, device enumeration and encoder media formats. 
{
public:
	VideoCapture* createVideoCapture(int deviceId);
		// Creates a VideoCapture instance for given device ID.
		//
		// If the VideoCaptureBase already exists for this camera then this method
		// can be used to create VideoCaptures in any thread.
		//
		// If the VideoCaptureBase has not been created for this camera yet it will
		// be created now, but take case since VideoCaptureBase instances should
		// only be initialized from the main thread (OpenCV limitation).
		// You can also lazy load video cameras using loadVideo()

	VideoCapture* createFileCapture(const std::string& file);
		// Creates a VideoCapture from given source file.
		// File captures can be created in any thread.

	virtual AudioCapture* createAudioCapture(int deviceId, 
		int channels = DEFAULT_AUDIO_CHANNELS, 
		int sampleRate = DEFAULT_AUDIO_SAMPLE_RATE, 
		RtAudioFormat format = RTAUDIO_SINT16);
		// Creates an AudioCapture from given options.
		
	void loadVideo();
		// Preloads a VideoCapture instance for each available camera.
		// This method can be called from the main thread to lazy load 
		// video device captures. Alternatively you can call createVideoCapture()
		// This will ensure captures are always available to the
		// application using createVideoCapture(), from any thread.
	
	void unloadVideo();
		// Destroys all managed VideoCaptureBase instances.
	
	IDeviceManager& devices();	
		// Returns the device manager instance.

	FormatRegistry& formats();	
		// Returns all registered media formats.

	static MediaFactory& instance();
		// Returns the default MediaFactory singleton.

	static void shutdown();
		// Shuts down the MediaFactory and deletes the singleton instance.

protected:
	MediaFactory();
	MediaFactory(MediaFactory const&){};
	MediaFactory& operator=(MediaFactory const&){};
	~MediaFactory();

	VideoCaptureBase* getVideoCaptureBase(int deviceId);
	
	friend class Singleton<MediaFactory>;
	friend class VideoCapture;
	
	mutable Mutex _mutex;

	IDeviceManager* _devices;
	FormatRegistry	_formats;	
	std::map<int, VideoCaptureBase*> _videoBases;
};


} } // namespace scy::av


#endif
