#ifndef H264_VIDEO_SOURCE_H
#define H264_VIDEO_SOURCE_H


#include "Common.h"
#include <liveMedia.hh>
#include <JPEGVideoSource.hh>
#include <Base64.hh>


namespace Anionu {
namespace RTSP {


/**
 * H264 video encapsulator.
 * This class will take Mats from a video source and then will encode them into
 * h264 packets, encapsulating then into RTPs frames.
 * This class will subclass the event-driven library LiveMedia.
 * Derives FramedSource.
 */
class H264VideoSource : public H264VideoStreamFramer
{
public:		
	//
 	// Static method.
	// @param env UsageEnvironment, see LiveMedia doc. here
	//        http://www.live555.com/liveMedia/public/doxygen/html
	// @param videoStream A VideoStream class, can be a MultiVideoSource or a 
	//        SingleVideoSource kind of class.
	// @param streamId Stream id that will identify this H264VideoSource 
	//        encapsulator class. It defaults to 0.
	// @param fps Frames Per Second value. Number of frames per time unit.
	// @param videoWidth The width of the encoded output video.
	// @param videoHeight The height of the encoded output video.
	// @param encoderBufferSize Encoder internal buffer size. Memory to alloc
	//        for encoding purposes. Defaults to 1000000.
	// @return New instance of H264VideoSource.
	// @see MJPEGVideoSource
 	//
	static H264VideoSource*
		createNew(UsageEnvironment& env, FramedSource* inputSource, Anionu::Input::VideoStream* videoStream, int streamId = 0,
				  int fps = 25, int videoWidth = 400, int videoHeight = 300, int encoderBufferSize = 1000000);
	
    // Returns true all the time	
	Boolean currentNALUnitEndsAccessUnit();

	//
	// Hack to get the sprop-parameter-sets for our sdp header before 
	// the rtp sink or encoder are initialized.
	// @see rfc 3984
 	//
	void getH264SpropParams(char** params);

protected:
	//
 	// Constructor.
	// @see createNew
 	//
	H264VideoSource(UsageEnvironment& env, FramedSource* inputSource, Anionu::Input::VideoStream* videoStream, int streamId,
						 int fps, int videoWidth, int videoHeight, int encoderBufferSize);
	virtual ~H264VideoSource();
	
private: // Functions
	//
 	// Inherited function.\  Used to process next frame to send.
 	//
	void doGetNextFrame();

	// Provides RTP encapsulation
	void deliverFrame();
	
	// Captures a cv::Mat and encode it to a h264 buffer
	void encodeFrame();
	
	//
 	// Retrieves NAL unit headers from h264 frame and encodes them as base64
	// for output in sprop-parameter-sets.
	// @see rfc 3984
 	//
	char* encodeH264SpropParams(const unsigned char* inputBuffer, int inputSize);
	
private: // Data
	// Specify whether the class is initialized or not
	bool _isInitialized;
	
	// Frames per second.
	int _fps;

	//
	// Holds the pointer of the internal_buffer of VideoEncoder.
	// In this particular H264 case it will contain a varying packet size before 
	// it's sending.
	//
	const unsigned char* _encodedFrame;

	// The current OpenCV frame
	cv::Mat _rawFrame;
	
	// size of current frame to send */
	int _encodedFrameSize;

	// Contains the last capture time. Useful for internal packet ordering 
	struct timeval _captureTime;
	
	// Size of the internal buffer used for encoding
	int _encoderBufferSize;
	
	// Stream id
	int _streamId;

	// The width of the encoded output video
	int _videoWidthOut;

	// The height of the encoded output video
	int _videoHeightOut;
	
	// VideoStream class instance pointer
	Anionu::Input::VideoStream *_videoStream;
	
	// VideoEncoder class instance pointer
	Anionu::AV::VideoEncoder *_videoEncoder;

	// redefined virtual functions:
	//virtual Boolean isH264VideoStreamFramer() const;
	Boolean isH264VideoStreamFramer() const;

#ifdef __LINUX__
	struct timezone dontKnow;
#else
	int dontKnow;
#endif
	
};


} } // namespace Anionu::RTSP


#endif	//H264_VIDEO_SOURCE_H

