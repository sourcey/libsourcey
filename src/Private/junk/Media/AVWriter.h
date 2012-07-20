#ifndef AV_WRITER_H
#define AV_WRITER_H


#include "Sourcey/Media/Types.h"

#include "Poco/Mutex.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/fifo.h>
#include <libswscale/swscale.h>
}


namespace Sourcey {
namespace Media {


/// Audio/Video writer.
/// Utility class with reusable code. Depends on libavcodec/libavformat.
class AVWriter
{
public:
	AVWriter(const std::string& filename, VideoParams* videoParams=NULL, AudioParams* audioParams=NULL);
	virtual ~AVWriter();
	
	bool encodeAudioFrame(unsigned char* buffer, int bufferSize, int durationInMS);
	bool encodeVideoFrame(unsigned char* buffer, int bufferSize, int width, int height, int durationInMS);

	bool isInitialized() { return _isInitialized; };

private:
	void initialize();
	void close();
	
 	// Audio
	void openAudio();
	void closeAudio();

 	// Video
	AVFrame* createVideoFrame(PixelFormat pixfmt);
	void openVideo();
	void closeVideo();
	
private:
	static Poco::FastMutex _mutex; // Protects avcodec_open/close().
	std::string _filename;
	AVFormatContext* _formatCtx;
	bool _isInitialized;

 	// Audio
	AudioParams* _audioParams;
	AVStream* _audioStream;	
	AVFifoBuffer* _audioFifo;		
	UInt8* _audioFifoOutBuffer;
	AVCodec* _audioCodec;
	UInt8* _audioBuffer; 	
	int _audioOutSize; // AudioOutFrameSize = AudioOutFrameSize * 2 * NumAudioChannels

 	// Video
	VideoParams* _videoParams;
	AVFrame* _videoOutFrame;
	AVFrame* _videoInFrame;
	AVStream* _videoStream;
	SwsContext* _videoConvCtx;
	UInt8* _videoBuffer;

 	// Calculated video frame presentation timestamp.
	UInt64 _videoPTS;
	UInt64 _videoLastPTS;	

};


} } // namespace Sourcey::Media


#endif	// AV_WRITER_H

