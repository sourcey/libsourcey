#ifndef SOURCEY_MEDIA_AVFileReader_H
#define SOURCEY_MEDIA_AVFileReader_H


#include "Sourcey/PacketDispatcher.h"
#include "Sourcey/IStartable.h"
#include "Sourcey/Media/Types.h"
#include "Sourcey/Media/VideoContext.h"
#include "Sourcey/Media/AudioContext.h"

#include "Poco/Runnable.h"
#include "Poco/Mutex.h"


extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
//#include <libavutil/fifo.h>
//#include <libswscale/swscale.h>
}


namespace Sourcey {
namespace Media {


class AVFileReader: public PacketDispatcher, public IStartable, public Poco::Runnable
	/// Video file decoder class with reusable code that
	/// depends on ffmpeg libavcodec/libavformat.
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

		Options() {
			disableVideo = false;
			disableAudio = false;
			iFramesOnly = false;
			processVideoXFrame = 0;
			processAudioXFrame = 0;
			processVideoXSecs = 0;
			processAudioXSecs = 0;
		}
	};

	AVFileReader(const Options& options = Options());
	virtual ~AVFileReader();
	
	virtual void open(const std::string& ifile);
	virtual void close();
	
	virtual void start();
	virtual void stop();

	virtual void run();
		
	virtual Options& options();
	virtual AVFormatContext* formatCtx() const;
	virtual VideoDecoderContext* video() const;
	virtual AudioDecoderContext* audio() const;
	virtual std::string error() const;

	NullSignal ReadComplete;

protected:		
	mutable Poco::FastMutex	_mutex;
	Poco::Thread			_thread;
	bool					_stop;
	std::string				_ifile;	
	std::string				_error;
	AVFormatContext*		_formatCtx;
	VideoDecoderContext*	_video;
	AudioDecoderContext*	_audio;
	Options					_options;
};


} } // namespace Sourcey::Media


#endif	// SOURCEY_MEDIA_AVFileReader_H

