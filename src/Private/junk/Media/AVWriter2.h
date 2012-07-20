#ifndef AV_WRITER_H
#define AV_WRITER_H


#include "Sourcey/Media/Common.h"

#include "Poco/Mutex.h"


namespace Sourcey {
namespace Media {


/// Audio/Video writer.
/// Utility class with reusable code. Depends on libavcodec/libavformat.
class AVWriter
{
public:		
 	// Constructor.
 	// @param filename The output filename.
 	// @param videoParams VideoParams.
 	// @param audioParams AudioParams.
	AVWriter(std::string filename, VideoParams* videoParams=NULL, AudioParams* audioParams=NULL);
	virtual ~AVWriter();
	
 	// Encode an audio frame.
 	// @param buffer Audio data buffer.
 	// @param bufferBytes Audio data buffer size.
 	// @param durationInMS The duration of the current frame in milliseconds.
 	// @return true on success, false on failure.
	bool encodeAudioFrame(unsigned char* buffer, int bufferBytes, int durationInMS);

 	// Encode a video frame.
 	// @param buffer Video data buffer.
 	// @param bufferBytes Video data buffer size.
 	// @param width Video width.
 	// @param height Video height.
 	// @param durationInMS The duration of the current frame in milliseconds.
 	// @return true on success, false on failure.
	bool encodeVideoFrame(unsigned char* buffer, int bufferBytes, int width, int height, int durationInMS);

 	// Returns weather the avwriter is initialized or not.
 	// @return true on success, false on failure.
	bool isInitialized() { return _isInitialized; };

private:
 	// Initializes the encoder 
 	// @return true on success, false on failure.
	bool initialize();

 	// Closes the encoder, and frees memory
	void close();
	
 	// -------------------------------------------------------------------
 	// Audio
 	// -------------------------------------------------------------------

 	// Opens the codec and initializes the internal fifo data buffer.
 	// @return true on success, false else.
	bool openAudio();

 	// Close the audio stream.
	void closeAudio();


 	// -------------------------------------------------------------------
 	// Video
 	// -------------------------------------------------------------------

 	// Creates an AVFrame.
 	// @param pixelFormat Pixel format of the resulting AVFrame. It's mainly used as
 	//        temporary buffer for conversions.
 	// @return A new AVFrame. 
 	// @see ffmpeg doc.
	AVFrame* createVideoFrame(PixelFormat pixelFormat);
	
 	// Opens the video codec.
	bool openVideo();

 	// Closes the video stream.
	void closeVideo();
	
private:
 	// Specifies whether the class is initialized or not.
	bool _isInitialized;
	
 	// Mutex locker to protect avcodec_open/close().
	static Poco::FastMutex _mutex;

 	// Filename
	std::string _filename;
	
 	// @see ffmpeg doc for this struct
	AVOutputFormat* _outputFmt;
	
 	// @see ffmpeg doc for this struct
	AVFormatContext* _formatCtx;


 	// -------------------------------------------------------------------
 	// Audio
 	// -------------------------------------------------------------------

 	// Struct containing video information.
	AudioParams* _audioParams;

 	// @see ffmpeg doc for this struct. 
	AVStream* _audioStream;	

 	// @see ffmpeg doc for this struct.
 	// FIFO to write incoming audio samples into.
	AVFifoBuffer* _audioFifo;		

 	// Buffer to read _out_ of the FIFO into.
	uint8_t* _audioFifoOutBuf;
	
 	// @see ffmpeg doc for this struct.
	AVCodecContext* _audioCodecCtx;

 	// @see ffmpeg doc for this struct.
	AVCodec* _audioCodec;

 	// Buffer internal used to encode.
	unsigned char* _audioOutputBuffer;
	
 	// Buffer internal used to encode.
	uint8_t* _audioOut;

 	// Size of the encoded frame. 
 	// AudioOutFrameSize = AudioOutFrameSize * 2 * NumAudioChannels
	int _audioOutSize;

 	// -------------------------------------------------------------------
 	// Video
 	// -------------------------------------------------------------------

 	// Struct containing video information.
	VideoParams* _videoParams;

 	// @see ffmpeg doc for this struct.
	AVCodec* _videoCodec;

 	// @see ffmpeg doc for this struct.
	AVFrame* _videoOutFrame,* _videoInFrame;
	
 	// @see ffmpeg doc for this struct.
	AVStream* _videoStream;
	
 	// @see ffmpeg doc for this struct.
	AVCodecContext* _videoCodecCtx;

 	// @see ffmpeg doc for this struct.
	PixelFormat _videoInPixFmt;

 	// Internal buffer used for conversions.
	uint8_t* _videoBuffer;

 	// Calculated video frame presentation timestamp.
	double _videoPTS;

 	// The video frame presentation timestamp of the previous frame.
	unsigned _videoPrevPTS;	

 	// @see ffmpeg doc for this struct
	SwsContext* _videoImgConvCtx;
};


} } // namespace Sourcey::AV


#endif	// AV_WRITER_H

