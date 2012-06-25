#include "Anionu/RTSP/H264VideoSource.h"


using namespace std;
using namespace Anionu;
using namespace Anionu::RTSP;
using namespace Anionu::Input;
using namespace Anionu::AV;


// ============================================================================
//
// H264 Video Source
//
// ============================================================================
H264VideoSource*
H264VideoSource::createNew(UsageEnvironment& env, FramedSource* inputSource, Anionu::Input::VideoStream* videoStream, int streamId /* = 0 */,
							    int fps /* = 25 */, int videoWidth /* = 400 */, int videoHeight /* = 300 */, int encoderBufferSize /*= 1000000*/) 
{
	return new H264VideoSource(env, inputSource, videoStream, streamId, fps, videoWidth, videoHeight, encoderBufferSize);
}


H264VideoSource::H264VideoSource(UsageEnvironment& env, FramedSource* inputSource, Anionu::Input::VideoStream* videoStream, int streamId,
									int fps, int videoWidth, int videoHeight, int encoderBufferSize) 
									: H264VideoStreamFramer(env, inputSource),
										_videoStream(videoStream),
										_streamId(streamId),
										_fps(fps), 
										_videoWidthOut(videoWidth), 
										_videoHeightOut(videoHeight), 
										_encoderBufferSize(encoderBufferSize)
{
	cout << "H264VideoSource::H264VideoSource()" << endl;

	_isInitialized = false;
	_videoEncoder = NULL;	
	_rawFrame = cvCreateImage(cvSize(_videoWidthOut, _videoHeightOut), 8, 3);	

	// Set the max output packet buffer size
	OutPacketBuffer::maxSize = OUTPUT_PACKET_BUFFER_MAXSIZE;

	// Create the encoder.
	_videoEncoder = new VideoEncoder(CODEC_ID_H264, _videoWidthOut, _videoHeightOut, _fps);
	if (!_videoEncoder) {
		cout << "_audioEncoder = NULL" << endl;
		return;
	}

	// Initialize device
	if (_videoStream->good() && _videoEncoder->isInitialized()) {
		cout << "H264VideoSource::isInitialized()" << endl;
		_isInitialized = true;
	}
}


H264VideoSource::~H264VideoSource()
{
	if (_videoEncoder)
		delete _videoEncoder;
	_videoEncoder = NULL;

	if (!_rawFrame.empty())
		_rawFrame.release();
	_rawFrame = NULL;
	//	cvReleaseImage(&_rawFrame);

	_isInitialized = false;
	// DO *NOT* delete _videoStream; otherwise in multi stream mode you'll have a crash
}


void H264VideoSource::encodeFrame() 
{
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

	_encodedFrame = _videoEncoder->encodeFrame(
		(unsigned char*)_rawFrame.data, 
		_rawFrame.operator IplImage().imageSize, 
		_rawFrame.cols, 
		_rawFrame.rows, 
		&_encodedFrameSize);

	if (_encodedFrameSize > _encoderBufferSize || _encodedFrame == NULL) {
		cout << "Error: the h264 frame to stream is too big or NULL. " << endl;
		return;
	}
		
    if (_encodedFrameSize <= 0 || (unsigned int)_encodedFrameSize > fMaxSize) {
		cout << "Error: the h264 frame to stream is too big. " << endl;
		return;
	}
}


void H264VideoSource::doGetNextFrame() {
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

	encodeFrame();
	deliverFrame();
}


void H264VideoSource::deliverFrame() {
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
		cout << "H264VideoSource::doGetNextFrame(): " 
			<< "read maximum buffer size: " << fMaxSize 
			<< " bytes.  Frame may be truncated" << endl;
	}

	// After delivering the data, switch to another task, and inform
	// the reader that he has data:
	nextTask() = envir().taskScheduler().scheduleDelayedTask(0, (TaskFunc*)afterGetting, this);
}


Boolean H264VideoSource::currentNALUnitEndsAccessUnit() {
  return True;
}


Boolean H264VideoSource::isH264VideoStreamFramer() const {
  return True;
}


char* H264VideoSource::encodeH264SpropParams(const unsigned char* inputBuffer, int inputSize)
{
    char* b64EncodedSPS = NULL;
    char* b64EncodedPPS = NULL;
	char outStr[256];

    while(inputSize > 4 &&
           inputBuffer[0] == 0 && inputBuffer[1] == 0 &&
           inputBuffer[2] == 0 && inputBuffer[3] == 1)
    {
        const int nalType = inputBuffer[4]&0x1f;
        int offset;
        int size = 0;

        size = inputSize;
        for (offset = 4; offset+3 < inputSize ; offset++)
        {
            if (inputBuffer[offset] == 0 && inputBuffer[offset+1] == 0 && inputBuffer[offset+2] == 0 && inputBuffer[offset+3] == 1)
            {
				// We found another start code
                size = offset;
                break;
            }
        }
        if (nalType == 7)
        {
            b64EncodedSPS = base64Encode((const char*)&inputBuffer[4], size - 4);
        }
        else if (nalType == 8)
        {
            b64EncodedPPS = base64Encode((const char*)&inputBuffer[4], size - 4);
        }
        inputSize -= size;
        inputBuffer += size;
    }
    if (b64EncodedSPS && b64EncodedPPS)
    {					
		if (sprintf(outStr, "%s,%s;", b64EncodedSPS, b64EncodedPPS) > 0) 
		{
			return outStr;
		}
    }
	return NULL;
}


void H264VideoSource::getH264SpropParams(char** params)
{
	encodeFrame();
	*params = H264VideoSource::encodeH264SpropParams(_encodedFrame, _encodedFrameSize);
}