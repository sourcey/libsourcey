#ifndef MJPEG_VIDEO_SOURCE_H
#define MJPEG_VIDEO_SOURCE_H


#include <GroupsockHelper.hh>
#include <liveMedia.hh>
#include <JPEGVideoSource.hh>

#include "Common.h"


namespace Anionu {
namespace RTSP {


/**
 * Jpeg used to stream with liveMedia should have this default header size.
 * Other formats, even if conforming JPEG/JFIF standards, will not be used.
 * For example ImageMagick library uses "wrong" [and dynamic] header size. This 
 * requirement is done because the JPEGVideoRTPSink reconstructor needs
 * an header like this.
 */
#define JPEG_HEADER_MAX_SIZE 623

/**
 * Set a default maximum jpeg size of 500Kb bytes. Should fit greatly
 * a 640*480 stream. If you need more increase this.
 */
#define OUTPUT_PACKET_BUFFER_MAXSIZE	512000

/**
 * MJPEG video encapsulator.
 * This class will take Mats from a video source and then will encode them into
 * JPEG images. Concatenating them will result in a Motion-JPEG stream. 
 * This class will subclass the event-driven library LiveMedia.
 * Derives JPEGVideoSource.
 */
class MJPEGVideoSource : public JPEGVideoSource
{
public:
	//
	// Static method.
	// @param env UsageEnvironment, see LiveMedia doc. here
	//        http://www.live555.com/liveMedia/public/doxygen/html
	// @param videoStream A VideoStream class, can be a MultiVideoSource or a 
	//        SingleVideoSource kind of class.
	// @param streamId Stream id that will identify this MJPEGVideoSource 
	//        encapsulator class. It defaults to 0.
	// @param fps Frames Per Second value. Number of frames per time unit.
	// @param encoderBufferSize Encoder internal buffer size. Memory to alloc
	//        for encoding purposes. Defaults to 1000000.
	// @return New instance of MJPEGVideoSource.
	// @see H263PVideoSource
	//
	static MJPEGVideoSource*
		createNew(UsageEnvironment& env, Anionu::Input::VideoStream* videoStream, 
				  int streamId = 0, int fps = 25, int videoWidth = 400, int videoHeight = 300, 
				  int encoderBufferSize = 1000000);

	virtual ~MJPEGVideoSource();
	
protected:
	//
	// Constructor.
	// @see createNew
	//
	MJPEGVideoSource(UsageEnvironment& env, Anionu::Input::VideoStream* videoStream, 
					  int streamId, int fps, int videoWidth, int videoHeight, int encoderBufferSize);
	
private: // Functions
	//
 	// Inherited function.\  Used to process next frame to send.
 	//
	void doGetNextFrame();
	
	// Provides RTP encapsulation.
	void deliverFrame();
	
	// Captures a cv::Mat and encode it to a JPEG memory buffer.
	void encodeFrame();

	//
	// @return Type of the image. Usually it's 1 (= JPEG)
	//
	u_int8_t type();
	
	//
	// Quality factor of JPEG image.
	// @return ffmpeg needs 195 to be returned.
	//
	u_int8_t qFactor();
	
	//
	// Width of image.
	// @return Width of image.
	//
	u_int8_t width();			// # pixels/8 (or 0 for 2048 pixels)
	
	//
	// Height of image.
	// @return Height of image.
	//
	u_int8_t height();			// # pixels/8 (or 0 for 2048 pixels)

	//
	// If "qFactor()" returns a value >= 128, then this function is called
	// to tell us the quantization tables that are being used.
	// (The default implementation of this function just returns NULL.) 
	// "precision" and "length" are as defined in RFC 2435, section 3.1.8.
	// @return Quantization tables that are being used
	// 
	u_int8_t const* quantizationTables(u_int8_t& precision, u_int16_t& length);
	
	
	//
	// Try to find width and height markers and grab their values.
	// @param fJPEGHeader JPEG memory buffer. Obviously the image header must be inside
	//        that buffer.
	// @return -1 on error, >= 0 if went fine.
	//	
	int setParamsFromHeader(const unsigned char* fJPEGHeader);
	
	// redefined virtual functions */
	Boolean isJPEGVideoSource() const;
	
private: // Data	
	// Specify whether the class is initialized or not.
	bool _isInitialized;

	/* Time per frame */
	unsigned fTimePerFrame;
	
	// Preferred size of current frame to send */
	int _encodedFrameSize;

	// 
	// It will just hold the pointer of the internal_buffer of VideoEncoder.
	// In this particular MJPEG case it will contain jpeg header+data before it's sending
	//
	const unsigned char* _encodedFrame;
	
	// Contains the last capture time. Useful for internal packet ordering 
	struct timeval _captureTime;
	
	// Frames per second.	
	int _fps;	

	// The current OpenCV frame
	cv::Mat _rawFrame;
	
	// Quality factor, width and height of the jpeg image */
	u_int8_t fLastQFactor, fLastWidth, fLastHeight;
	
	// Quantization tables */
	u_int8_t *qTable;

	// Size of the internal buffer used for encoding
	int _encoderBufferSize;
	
	// Stream id.
	int _streamId;
	
	// The width of the encoded output video
	int _videoWidthOut;

	// The height of the encoded output video
	int _videoHeightOut;

	// VideoStream class instance pointer.
	Anionu::Input::VideoStream *_videoStream;
	
	// VideoEncoder class instance pointer.
	Anionu::AV::VideoEncoder *_videoEncoder;	   
	
#ifdef __LINUX__
	struct timezone dontKnow;	
#else
	int dontKnow;
#endif	
	
};


} } // namespace Anionu::RTSP


#endif	//MJPEG_VIDEO_SOURCE_H

