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


#ifndef SCY_MEDIA_AVInputReader_H
#define SCY_MEDIA_AVInputReader_H


#include "scy/base.h"

#ifdef HAVE_FFMPEG

#include "scy/packetsignal.h"
#include "scy/interface.h"
#include "scy/interface.h"
#include "scy/media/types.h"
#include "scy/media/ffmpeg.h"
#include "scy/media/icapture.h"
#include "scy/media/videocontext.h"
#include "scy/media/audiocontext.h"
#include "scy/mutex.h"


extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#ifdef LIBAVDEVICE_VERSION
#include <libavdevice/avdevice.h>
#endif
}


namespace scy {
namespace av {


class AVInputReader: public ICapture, public async::Runnable
	/// Video capture and file input decoder class with reusable
	/// code that depends on ffmpeg libavcodec/libavformat.
{
public:
	struct Options
	{
		bool disableVideo;
		bool disableAudio;
		bool iFramesOnly;			// Process i-frames only (does not restrict audio)
		int processVideoXFrame;		// Process every Xth video frame
		int processAudioXFrame;		// Process every Xth audio frame
		double processVideoXSecs;	// Process video frame every X seconds
		double processAudioXSecs;	// Process audio frame every X seconds

		// Device Input
		std::string deviceEngine;	// The device capture engine"
									//		Windows: vfwcap, dshow
									//		Linux: v4l(video4linux2 or video4linux), dv1394
		std::string deviceStandard; // Linux only: 'pal', 'secam' or 'ntsc'.

		Options() {
			disableVideo = false;
			disableAudio = false;
			iFramesOnly = false;
			processVideoXFrame = 0;
			processAudioXFrame = 0;
			processVideoXSecs = 0;
			processAudioXSecs = 0;

#ifdef WIN32
			deviceEngine = "vfwcap";
#else
			deviceEngine = "v4l";
			deviceStandard = "ntsc";
#endif
		}
	};

	AVInputReader(const Options& options = Options());
	virtual ~AVInputReader();

	virtual void openFile(const std::string& file);
#ifdef LIBAVDEVICE_VERSION
	virtual void openDevice(int deviceID, int width = 0, int height = 0, double framerate = 0);
	virtual void openDevice(const std::string& device, int width = 0, int height = 0, double framerate = 0);
#endif
	virtual void openStream(const char* filename, AVInputFormat* inputFormat, AVDictionary** formatParams);
	virtual void close();

	virtual void start();
	virtual void stop();

	virtual void run();

	virtual void getEncoderFormat(Format& iformat) {};

	virtual Options& options();
	virtual AVFormatContext* formatCtx() const;
	virtual VideoDecoderContext* video() const;
	virtual AudioDecoderContext* audio() const;
	virtual std::string error() const;

	NullSignal ReadComplete;

protected:
	mutable Mutex	_mutex;
	Thread _thread;
	//std::string _ifile;
	std::string _error;
	Options _options;
	AVFormatContext* _formatCtx;
	VideoDecoderContext* _video;
	AudioDecoderContext* _audio;
	bool _stopping;
};


} } // namespace scy::av


#endif
#endif	// SCY_MEDIA_AVInputReader_H
