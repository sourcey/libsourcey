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


namespace Sourcey {
namespace Media {


class MediaFactory
	/// The MediaFactory class is a singleton for instantiating
	/// and managing audio and video captures. 
	///
	/// Video:
	/// The MediaFactory ensures that only a single VideoCapture
	/// instance exists for each device at any given time. This is
	/// a limitation imposed by the underlying architecture.
	///
	/// Audio:
	/// The MediaFactory just provides a convenient interface for
	/// instantiating AudioCaptures. Since no such limitation like
	/// for video applies nothing else is required.
	///
	/// Devices:
	/// @see DeviceManager
{
public:
	static MediaFactory* instance();
	static void initialize();	
	static void uninitialize();

	void loadVideo();
	void loadAudio();
	void unloadVideo();
	void unloadAudio();

	IDeviceManager& devices();

	class Video
		/// This class manages Video for the MediaFactory.
	{
	public:
		Video(MediaFactory* factory);	
		~Video();

		void load();
		void unload();

		virtual VideoCapture* getCapture(int deviceId);
			/// Returns a reference the VideoCapture for the given device ID.
			/// If no reference exists then a VideoCapture will be instantiated.
			/// VideoCapture instances MUST not be programmatically destroyed.

		virtual VideoCapture* getCapture(const std::string& file);
			/// Instantiates a VideoCapture from a video file source.
			/// The VideoCapture will be automatically destroyed when
			/// the delegate reference count reaches 0;
		
		virtual bool closeCapture(int deviceId);
			/// Closes the VideoCapture for the given device ID.

	protected:
		MediaFactory*	_factory;
		VideoCaptureMap	_map;

	} video;

	class Audio
		/// This class manages Audio for the MediaFactory.
	{
	public:
		Audio(MediaFactory* factory);	
		~Audio();	

		void load();
		void unload();

		virtual AudioCapture* getCapture(int deviceId, int channels = DEFAULT_AUDIO_CHANNELS, int sampleRate = DEFAULT_AUDIO_SAMPLE_RATE, RtAudioFormat format = RTAUDIO_SINT16);
			///

	protected:
		MediaFactory*	_factory;
		//AudioCaptureMap	_map;

	} audio;

protected:
	MediaFactory();									// Private so that it can not be called
	//MediaFactory(MediaFactory const&){};			// Copy constructor is private
	MediaFactory& operator=(MediaFactory const&){};	// Assignment operator is private
	~MediaFactory();

	static MediaFactory*	_instance;
	static Poco::FastMutex	_mutex;
	IDeviceManager*			_devices;
};


} } // namespace Sourcey::Media


#endif
