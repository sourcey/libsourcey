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
	AVFileReader();
	virtual ~AVFileReader();
	
	virtual void open(const std::string& ifile);
	virtual void close();
	
	virtual void start();
	virtual void stop();
	
	virtual std::string error() const;
	
	virtual AVFormatContext* formatCtx() const;
	virtual VideoDecoderContext* video() const;
	virtual AudioDecoderContext* audio() const;

	NullSignal ReadComplete;

protected:
	virtual void run();

protected:		
	mutable Poco::FastMutex	_mutex;
	Poco::Thread			_thread;
	bool					_stop;
	std::string				_ifile;	
	std::string				_error;
	AVFormatContext*		_formatCtx;
	VideoDecoderContext*	_video;
	AudioDecoderContext*	_audio;
};


} } // namespace Sourcey::Media


#endif	// SOURCEY_MEDIA_AVFileReader_H

