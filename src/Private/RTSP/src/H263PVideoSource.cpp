#include "Anionu/RTSP/H263PVideoSource.h"


using namespace std;
using namespace Anionu;
using namespace Anionu::RTSP;
using namespace Anionu::AV;
using namespace Anionu::Input;
 

// ============================================================================
//
// H263p Video Source
//
// ============================================================================
H263PVideoSource*
H263PVideoSource::createNew(UsageEnvironment& env, Anionu::Input::VideoStream* videoStream, int streamId /* = 0 */,
							    int fps /* = 25*/, int videoWidth /* = 400 */, int videoHeight /* = 300 */, int encoderBufferSize /*= 1000000*/) 
{
	return new H263PVideoSource(env, videoStream, streamId, fps, videoWidth, videoHeight, encoderBufferSize);
}


H263PVideoSource::H263PVideoSource(UsageEnvironment& env, Anionu::Input::VideoStream* videoStream, int streamId,
									int fps, int videoWidth, int videoHeight, int encoderBufferSize) : 
	FramedSource(env), 
	_videoStream(videoStream),
	_streamId(streamId),
	_fps(fps), 
	_videoWidthOut(videoWidth), 
	_videoHeightOut(videoHeight), 
	_encoderBufferSize(encoderBufferSize)
{
	cout << "H263VideoSource::H263VideoSource()" << endl;

	_isInitialized = false;
	_videoEncoder = NULL;	
	_rawFrame = cvCreateImage(cvSize(_videoWidthOut, _videoHeightOut), 8, 3);	

	// Set the max output packet buffer size
	OutPacketBuffer::maxSize = OUTPUT_PACKET_BUFFER_MAXSIZE;

	// Create the encoder.
	_videoEncoder = new VideoEncoder(CODEC_ID_H263, _videoWidthOut, _videoHeightOut, _fps);
	if (!_videoEncoder) {
		cout << "_audioEncoder = NULL" << endl;
		return;
	}

	// Initialize device
	if (//_videoStream->good() && 
		_videoEncoder->isInitialized()) {
		cout << "H263VideoSource::isInitialized()" << endl;
		_isInitialized = true;
	}
}

H263PVideoSource::~H263PVideoSource()
{
	if (_videoEncoder)
		delete _videoEncoder;
	_videoEncoder = NULL;

	if (!_rawFrame.empty())
		_rawFrame.release();
	_rawFrame = NULL;
		//cvReleaseImage(&_rawFrame);

	_isInitialized = false;
	// DO *NOT* delete _videoStream; otherwise in multi stream mode you'll have a crash
}


void H263PVideoSource::encodeFrame() 
{
}

//--------------------------------------------------------------------------
// 

void H263PVideoSource::doGetNextFrame() {
	// Arrange here for our "deliverFrame" member function to be called
	// when the next frame of data becomes available from the device.
	// This must be done in a non-blocking fashion - i.e., so that we
	// return immediately from this function even if no data is
	// currently available.
	//
	// If the device can be implemented as a readable socket, then one easy
	// way to do this is using a call to
	//     envir().taskScheduler().turnOnBackgroundReadHandling( ... )
	// (See examples of this call in the "liveMedia" directory.)

	// If, for some reason, the source device stops being readable
	// (e.g., it gets closed), then you do the following:
	if (!_videoEncoder || !_videoStream) {
		cout << "if (!_videoEncoder || !_videoStream)" << endl;
		handleClosure(this);
		return;
	}

	cv::Mat frame;
	_encodedFrameSize = 0;

		*_videoStream >> frame;

	// We cannot proceed with encoding, just exit here.
	if (frame.empty()) {
		cout << "_videoStream->getNextFrame = NULL" << endl;
		fFrameSize = _encodedFrameSize = 0;
		_encodedFrame = NULL;
		return;
	}
	gettimeofday(&_captureTime, NULL);

	cv::resize(frame, _rawFrame, cv::Size(_videoWidthOut, _videoHeightOut));
	//cvReleaseImage(&frame);

	_encodedFrame = _videoEncoder->encodeFrame(
		(unsigned char*)_rawFrame.data, 
		_rawFrame.operator IplImage().imageSize, 
		_rawFrame.cols, 
		_rawFrame.rows, 
		&_encodedFrameSize);

	if (_encodedFrameSize > _encoderBufferSize || _encodedFrame == NULL) {
		cout << "Error: the h263P frame to stream is too big or NULL. " << endl;
		return;
	}
		
    if (_encodedFrameSize <= 0 || (unsigned int)_encodedFrameSize > fMaxSize) {
		cout << "Error: the h263P frame to stream is too big. " << endl;
		return;
	}

	deliverFrame();
}

//--------------------------------------------------------------------------
//

void H263PVideoSource::deliverFrame() {
	// This would be called when new frame data is available from the device.
	// This function should deliver the next frame of data from the device,
	// using the following parameters (class members):
	// 'in' parameters (these should *not* be modified by this function):
	// FTo: The frame data is copied to this address.
	// (Note that the variable "fTo" is *not* modified.  Instead,
	// the frame data is copied to the address pointed to by "fTo".)
	// FMaxSize: This is the maximum number of bytes that can be copied
	// (If the actual frame is larger than this, then it should
	// be truncated, and "fNumTruncatedBytes" set accordingly.)
	// 'out' parameters (these are modified by this function):
	// FFrameSize: Should be set to the delivered frame size (<= fMaxSize).
	// FNumTruncatedBytes: Should be set iff the delivered frame would have been
	// bigger than "fMaxSize", in which case it's set to the number of bytes
	// that have been omitted.
	// FPresentationTime: Should be set to the frame's presentation time
	// (seconds, microseconds).
	// FDurationInMicroseconds: Should be set to the frame's duration, if known.

	// If our _videoStream source is a multi-stream one there is often the possibility that 
	// no Mats are available for streaming.
	// If that's the case ignore the _encodedFrame to NULL 	
	if (_encodedFrameSize <= 0 || _encodedFrame == NULL) {
		nextTask() = envir().taskScheduler().scheduleDelayedTask(0, (TaskFunc*)afterGetting, this);
		return;
	}

	// Set the 'presentation time': The time that this frame was captured	
	//gettimeofday(&fPresentationTime, NULL);
	fPresentationTime = _captureTime;	
	fDurationInMicroseconds = 1000000/_fps;

	fFrameSize = _encodedFrameSize;
	memcpy(fTo, _encodedFrame, fFrameSize);

	if (fFrameSize >= fMaxSize) {
		cout << "H263PVideoSource::doGetNextFrame(): " 
			<< "read maximum buffer size: " << fMaxSize 
			<< " bytes.  Frame may be truncated" << endl;
	}

	// After delivering the data, switch to another task, and inform
	// the reader that he has data:
	nextTask() = envir().taskScheduler().scheduleDelayedTask(0, (TaskFunc*)afterGetting, this);
}


