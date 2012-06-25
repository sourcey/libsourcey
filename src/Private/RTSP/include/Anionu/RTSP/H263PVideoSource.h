#ifndef H263P_VIDEO_SOURCE_H
#define H263P_VIDEO_SOURCE_H


#include "Common.h"

#include <liveMedia.hh>
#include <BasicUsageEnvironment.hh>


namespace Anionu {
namespace RTSP {


/**
 * H263P video encapsulator.
 * This class will take Mats from a video source and then will encode them into
 * h263P packets, encapsulating then into RTPs frames.
 * This class will subclass the event-driven library LiveMedia.
 * Derives FramedSource.
 */
class H263PVideoSource : public FramedSource
{
public:
		
	//
 	// Static method.
	// @param env UsageEnvironment, see LiveMedia doc. here
	//        http://www.live555.com/liveMedia/public/doxygen/html
	// @param videoStream A VideoStream class, can be a MultiVideoSource or a 
	//        SingleVideoSource kind of class.
	// @param streamId Stream id that will identify this H263PVideoSource 
	//        encapsulator class. It defaults to 0.
	// @param fps Frames Per Second value. Number of frames per time unit.
	// @param videoWidth The width of the encoded output video.
	// @param videoHeight The height of the encoded output video.
	// @param encoderBufferSize Encoder internal buffer size. Memory to alloc
	//        for encoding purposes. Defaults to 1000000.
	// @return New instance of H263PVideoSource.
	// @see MJPEGVideoSource
 	//
	static H263PVideoSource*
		createNew(UsageEnvironment& env, Anionu::Input::VideoStream* videoStream, int streamId = 0,
				  int fps = 25, int videoWidth = 400, int videoHeight = 300, int encoderBufferSize = 1000000);
	
protected:
	//
 	// Constructor.
	// @see createNew
 	//
	H263PVideoSource(UsageEnvironment& env, Anionu::Input::VideoStream* videoStream, int streamId,
						 int fps, int videoWidth, int videoHeight, int encoderBufferSize);
	virtual ~H263PVideoSource();
	
private: // Functions
	//
 	// Inherited function.\  Used to process next frame to send.
 	//
	void doGetNextFrame();

	// Provides RTP encapsulation
	void deliverFrame();
	
	// Captures a cv::Mat and encode it to a h263P buffer
	void encodeFrame();
	
private: // Data
	// Specify whether the class is initialized or not
	bool _isInitialized;
	
	// Frames per second.
	int _fps;

	//
	// Holds the pointer of the internal_buffer of VideoEncoder.
	// In this particular H263P case it will contain a varying packet size before 
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

#ifdef __LINUX__
	struct timezone dontKnow;
#else
	int dontKnow;
#endif
	
};


} } // namespace Anionu::RTSP


#endif	//H263P_VIDEO_SOURCE_H


