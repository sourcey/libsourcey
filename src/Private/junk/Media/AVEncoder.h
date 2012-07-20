#ifndef SPOT_MEDIA_AVEncoder_H
#define SPOT_MEDIA_AVEncoder_H


#include "Sourcey/PacketStream.h"
#include "Sourcey/Media/Types.h"
#include "Sourcey/Media/IEncoder.h"
#include "Sourcey/Media/FPSCounter.h"
#include "Sourcey/Media/VideoContext.h"
#include "Sourcey/Media/AudioContext.h"


#include "Poco/Mutex.h"
#include <fstream>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/fifo.h>
#include <libswscale/swscale.h>
}


namespace Sourcey {
namespace Media {


class AVEncoder: public IPacketEncoder
	/// Audio/Video writer.
	/// Utility class with reusable code. Depends on libavcodec/libavformat.
{
public:
	AVEncoder(const RecorderParams& params);
	AVEncoder(const EncoderParams& params);
	AVEncoder();
	virtual ~AVEncoder();

	//virtual void initialize(const RecorderParams& params);
	virtual void initialize();
	virtual void uninitialize();
	virtual void cleanup();

	virtual void process(IPacket& packet);

	virtual bool encodeAudio(unsigned char* buffer, int bufferSize);
	virtual bool encodeVideo(unsigned char* buffer, int bufferSize, int width, int height);

	void setParams(const RecorderParams& params) { _params = params; };
	RecorderParams& params() { return _params; };
	
	//PolymorphicSignal BroadcastPacket;

protected:
	//virtual void cleanup();					
	virtual void onStreamStateChange(const PacketStreamState& state);

 	// Audio
	void openAudio();
	void closeAudio();

 	// Video
	AVFrame* createVideoFrame(::PixelFormat pixfmt, int width, int height);
	void openVideo();
	void closeVideo();
	
protected:
	static Poco::FastMutex _mutex; // Protects avcodec_open/close().

	RecorderParams	_params;	
	FPSCounter		_fpsCounter;
	AVFormatContext* _formatCtx;
	ByteIOContext*	_outIO;
	unsigned char*  _outIOBuffer; 
	int				_outIOBufferSize; 

	//
 	// Video
	//
	VideoEncoderContext* _video;
	/*
	AVFrame*		_videoOutFrame;
	AVFrame*		_videoInFrame;
	AVStream*		_videoStream;
	SwsContext*		_videoConvCtx;
	UInt8*			_videoBuffer;
	int				_videoBufferSize;
	*/

 	// Calculated video frame presentation timestamp.
	double			_videoPTS;
	Int64			_videoLastPTS;
	clock_t			_videoTime;

	//
 	// Audio
	//
	AudioEncoderContext* _audio;
	/*
	AVStream*		_audioStream;	
	AVFifoBuffer*	_audioFifo;		
	UInt8*			_audioFifoOutBuffer;
	int				_audioFrameSize;
	AVCodec*		_audioCodec;
	UInt8*			_audioBuffer; 
	int				_audioBufferSize;
	int				_audioFrameSize; // AudioOutFrameSize = AudioOutFrameSize * 2 * NumAudioChannels
	*/
	clock_t			_audioTime;

	
};


} } // namespace Sourcey::Media


#endif	// SPOT_MEDIA_AVEncoder_H

