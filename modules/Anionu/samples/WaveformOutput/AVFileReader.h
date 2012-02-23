#ifndef SOURCEY_MEDIA_AVFileReader_H
#define SOURCEY_MEDIA_AVFileReader_H


#include "Sourcey/Base/IPacketBroadcaster.h"
#include "Sourcey/Base/IRunnable.h"
#include "Sourcey/Media/Types.h"
#include "Sourcey/Media/VideoContext.h"
#include "Sourcey/Media/AudioContext.h"

#include "Poco/Mutex.h"

/*
#include "Sourcey/Media/FPSCounter.h"
#include "Sourcey/Media/IEncoder.h"
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/fifo.h>
#include <libswscale/swscale.h>
}
*/


namespace Sourcey {
namespace Media {

	
/*
struct VideoContext
{
    //int					streamID;
	AVStream*			stream;
	AVCodecContext*		cx;
	AVCodec*			codec;
	AVPacket*			packet;
	AVFrame*			iframe;
	AVFrame*			oframe;
	struct SwsContext*	convertCtx;

    int					bufferSize;
	uint8_t*			buffer;
	
    std::string error;

	VideoContext()
	{
		init();
	}
		
	~VideoContext()
	{	
		close();
	}
		
	void init()
	{		
		stream = NULL;
		cx = NULL;
		codec = NULL;
		convertCtx = NULL;
		iframe = NULL;
		oframe = NULL;
		
		bufferSize = 0;
		buffer = NULL;

		error = "";
	}
		
	void close()
	{	
		if (iframe)
			av_free(iframe);
	
		if (oframe)
			av_free(oframe);
	
  		if (packet)
  			av_free_packet(packet);
  	
  		if (convertCtx)
			sws_freeContext(convertCtx);

		if (cx)
			avcodec_close(cx);

        if (buffer)
           av_free(buffer);
	}

	virtual double pts()
	{
		if (packet && stream) {
			double pts = (packet->dts != AV_NOPTS_VALUE) ? packet->dts : 0.0;
			pts *= av_q2d(stream->time_base);
			return pts;
		}
		return 0.0;
	}
};


    // Exposed properties 
    std::string file_name;
    std::string codec_name;
    std::string error;
    int bit_rate;
    int sample_rate;
    int bits_per_sample;
    int channels;
    double duration;
    uint8_t *buffer;
    Int64 frames_per_interval;
    Int64 error_per_interval;
    Int64 error_base;
	 :
		stream(NULL),
		cx(NULL),
		codec(NULL),
		convertCtx(NULL),
		bufferSize(0),
		buffer(NULL),
		width(-1),
		fp(false)
	*/


class AVFileReader: public IPacketBroadcaster, public IRunnable, public Poco::Runnable 
	/// Video encoder class with reusable code that
	/// depends on ffmpeg libavcodec/libavformat.
{
public:		
	AVFileReader();
	virtual ~AVFileReader();
	
	virtual void open(const std::string& file);
	//virtual void openVideo(int streamID);
	//virtual void openAudio(int streamID);
	virtual void close();
	
	virtual void start();
	virtual void stop();
	
	//virtual int readNextFrame();
	//virtual int decodeVideo(AVPacket& packet); //VideoPacket& packet
	//virtual int decodeAudio(AVPacket& packet); //AudioPacket& packet
	
	//virtual int populateMatrix(cv::Mat& mat); //VideoPacket& packet

	//virtual double fps();
	//virtual double duration();

protected:
	virtual void run();

protected:		
	Poco::Thread		_thread;
	Poco::FastMutex		_mutex;
	bool				_stop;
	std::string			_file;
	//UInt32				_duration;
	
	AVFormatContext*	_formatCtx;	

	//
	// Video Stuff
	//
	//int					_video->streamID; // The index of the Video Stream.
	VideoDecoderContext*		_video;
	//cv::Mat _cvMatrix;

	
	//
	// Audio Stuff
	//	
	AudioDecoderContext*		_audio;
	/*
	int					_audio->streamID; // The index of the Audio Stream.
	AVStream*			_audioStream;
	AVCodecContext*		_audio;
	AVCodec*			_audioCodec;
	uint8_t*			_audioBuffer;
	*/
};


} } // namespace Sourcey::Media


#endif	// SOURCEY_MEDIA_AVFileReader_H

