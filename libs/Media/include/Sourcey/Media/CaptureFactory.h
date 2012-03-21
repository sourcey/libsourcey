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


#ifndef SOURCEY_CaptureFactory_H
#define SOURCEY_CaptureFactory_H


#include "Sourcey/Media/VideoCapture.h"
#include "Sourcey/Media/AudioCapture.h"
#include "Sourcey/Media/DeviceManager.h"
#include "Sourcey/Media/Types.h"

#include "Poco/Foundation.h"
#include "Poco/Thread.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <iostream>
#include <map>


namespace Sourcey {
namespace Media {


class CaptureFactory
	/// The CaptureFactory class is a singleton for instantiating
	/// and managing audio and video captures. 
	///
	/// Video:
	/// The CaptureFactory ensures that only a single VideoCapture
	/// instance exists for each device at any given time. This is
	/// a limitation imposed by the underlying architecture.
	///
	/// Audio:
	/// The CaptureFactory just provides a convenient interface for
	/// instantiating AudioCaptures. Since no such limitation like
	/// for video applies nothing else is required.
	///
	/// Devices:
	/// @see DeviceManager
{
public:
	static CaptureFactory* instance();
	static void initialize();	
	static void uninitialize();

	void loadVideo();
	void loadAudio();
	void unloadVideo();
	void unloadAudio();

	DeviceManager& devices();

	class Video
		/// This class manages Video for the CaptureFactory.
	{
	public:
		Video(CaptureFactory* factory);	
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
		CaptureFactory*	_factory;
		VideoCaptureMap	_map;

	} video;

	class Audio
		/// This class manages Audio for the CaptureFactory.
	{
	public:
		Audio(CaptureFactory* factory);	
		~Audio();	

		void load();
		void unload();

		virtual AudioCapture* getCapture(int deviceId, int channels = DEFAULT_AUDIO_CHANNELS, int sampleRate = DEFAULT_AUDIO_SAMPLE_RATE);
			///

	protected:
		CaptureFactory*	_factory;
		//AudioCaptureMap	_map;

	} audio;

protected:
	CaptureFactory();									// Private so that it can not be called
	//CaptureFactory(CaptureFactory const&){};			// Copy constructor is private
	CaptureFactory& operator=(CaptureFactory const&){};	// Assignment operator is private
	~CaptureFactory();

	static CaptureFactory*	_instance;
	static Poco::FastMutex	_mutex;
	DeviceManager			_devices;
};


} } // namespace Sourcey::Media


#endif
