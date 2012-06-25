#include "Anionu/RTSP/MJPEGVideoSource.h"


using namespace std;
using namespace Anionu;
using namespace Anionu::RTSP;
using namespace Anionu::Input;
using namespace Anionu::AV;


// ============================================================================
//
// MJPEG Video Source
//
// ============================================================================
MJPEGVideoSource* MJPEGVideoSource::createNew(UsageEnvironment& env, Anionu::Input::VideoStream* videoStream, int streamId, 
								   int fps /* = 25*/, int videoWidth /* = 400 */, int videoHeight /* = 300 */, int encoderBufferSize) 
{
	return new MJPEGVideoSource(env, videoStream, streamId, fps, videoWidth, videoHeight, encoderBufferSize);
}


MJPEGVideoSource::MJPEGVideoSource(UsageEnvironment& env, Anionu::Input::VideoStream* videoStream, int streamId, 
								   int fps /* = 25*/, int videoWidth /* = 400 */, int videoHeight /* = 300 */, int encoderBufferSize) 
										: JPEGVideoSource(env), _videoStream(videoStream), 
											_fps(fps),
											_videoWidthOut(videoWidth), 
											_videoHeightOut(videoHeight), 
											_streamId(streamId),
											_encoderBufferSize(encoderBufferSize)
{
	cout << "MJPEGVideoSource::MJPEGVideoSource()" << endl;

	_isInitialized = false;
	_videoEncoder = NULL;	
	_rawFrame = cvCreateImage(cvSize(_videoWidthOut, _videoHeightOut), 8, 3);	
	qTable = NULL;

	// Set the max output packet buffer size
	OutPacketBuffer::maxSize = OUTPUT_PACKET_BUFFER_MAXSIZE;

	// Create the encoder.
	_videoEncoder = new VideoEncoder(CODEC_ID_MJPEG, _videoWidthOut, _videoHeightOut, _fps);
	if (!_videoEncoder) {
		cout << "_audioEncoder = NULL" << endl;
		return;
	}

	// Initialize device
	if (_videoStream->good() && _videoEncoder->isInitialized()) {
		cout << "MJPEGVideoSource::isInitialized()" << endl;
		_isInitialized = true;
	}
}


void  MJPEGVideoSource::encodeFrame() 
{	
	cout << "MJPEGVideoSource::encodeFrame()" << endl;

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
		cout << "Error: the MJPEG frame to stream is too big or NULL. " << endl;
		return;
	}
		
    if (_encodedFrameSize <= 0 || (unsigned int)_encodedFrameSize > fMaxSize) {
		cout << "Error: the MJPEG frame to stream is too big. " << endl;
		return;
	}

	cout << "END MJPEGVideoSource::encodeFrame()" << endl;
}


MJPEGVideoSource::~MJPEGVideoSource() 
{
	if (_videoEncoder)
		delete _videoEncoder;
	_videoEncoder = NULL;
	
	// Not useful but anyway...
	_encodedFrame = NULL;	
	
	if (qTable != NULL)
		free(qTable);

	_isInitialized = false;
}


u_int8_t const* MJPEGVideoSource::quantizationTables(u_int8_t& precision, u_int16_t& length) 
{
	length = 0;
	precision = 0;
	
	if (qTable == NULL)
		return NULL;
	
	precision = 8;
	length = 64 *2; //Q-table is 64-bytes.
	
	return qTable;
}


Boolean MJPEGVideoSource::isJPEGVideoSource() const 
{
  return true;
}


void MJPEGVideoSource::doGetNextFrame() {
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


void MJPEGVideoSource::deliverFrame() {
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
	// FNumTruncatedBytes: Should be set if the delivered frame would have been
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

	cout << "MJPEGVideoSource::deliverFrame()" << endl;

	// Set the 'presentation time': the time that this frame was captured
	fPresentationTime = _captureTime;
	fDurationInMicroseconds = 1000000/_fps;

	// Now, read the previously captured frame:
	// Start with the JPEG header:
	int jpegHeaderSize = setParamsFromHeader(_encodedFrame);

	// if the header size has missed go on with the next task
	if (jpegHeaderSize <= 0 || _encodedFrameSize == 0) {
		nextTask() = envir().taskScheduler().scheduleDelayedTask(0, (TaskFunc*)afterGetting, this);
		return;
	}
	
	// Then, the JPEG payload: fTo *do not have to* include jpeg header. 
	// It will automatically filled with the rtp header which is
	// smaller.
	fFrameSize = _encodedFrameSize - jpegHeaderSize;
	memcpy(fTo, _encodedFrame + jpegHeaderSize, fFrameSize);

	if (fFrameSize >= fMaxSize) {
		cout << "MJPEGVideoSource::doGetNextFrame(): " 
			<< "read maximum buffer size: " << fMaxSize 
			<< " bytes.  Frame may be truncated" << endl;
	}

	// After delivering the data, switch to another task, and inform
	// the reader that he has data:
	nextTask() = envir().taskScheduler().scheduleDelayedTask(0, (TaskFunc*)afterGetting, this);

	cout << "END MJPEGVideoSource::deliverFrame()" << endl;
}


u_int8_t MJPEGVideoSource::type() {
	return 1; // 1 is for jpeg type.
}


u_int8_t MJPEGVideoSource::width() {
	return fLastWidth;
}


u_int8_t MJPEGVideoSource::height() {
	return fLastHeight;
}


u_int8_t MJPEGVideoSource::qFactor() {
 // Ffmpeg needs this to be returned.
	return 195;
}


// try to find width and height markers and grab their values.
// -1 on error
int MJPEGVideoSource::setParamsFromHeader(const unsigned char* fJPEGHeader) {
	// Look for the "SOF0" marker (0xFF 0xC0), to get the frame
	// width and height:
	bool found_sofo = false;
	bool found_hdr_size = false;
	int jpegHeaderSize;

	for (int i = 0; i < JPEG_HEADER_MAX_SIZE/*-8*/; ++i) {
		if (!found_sofo && fJPEGHeader[i] == 0xFF && fJPEGHeader[i+1] == 0xC0) {
			fLastHeight = (fJPEGHeader[i+5]<<5)|(fJPEGHeader[i+6]>>3);
			fLastWidth = (fJPEGHeader[i+7]<<5)|(fJPEGHeader[i+8]>>3);
			found_sofo = true;
		}

		if (fJPEGHeader[i] == 0xFF && fJPEGHeader[i+1] == 0xDB) {
			if (qTable != NULL)
				free (qTable);
			
			qTable = (u_int8_t *) malloc(sizeof(u_int8_t) *128);
			memcpy(qTable, fJPEGHeader + i + 5, 64);
			memcpy(qTable + 64, fJPEGHeader + i + 5, 64);
		}
		
		// we should look for the final byte of the 0xFF 0xDA marker,
		// the start of scan part.
		if (fJPEGHeader[i] == 0x3F && fJPEGHeader[i+1] == 0x00) {
			found_hdr_size = true;
			jpegHeaderSize = i+2;
			break;
		}
	}
	
	if (!found_sofo)
		cout << "MJPEGVideoSource: Failed to find SOF0 marker in header!" << endl;
		
	if (!found_hdr_size)	
		cout << "MJPEGVideoSource: Failed to find header size!" << endl;
		
	if (found_hdr_size == false)
		return -1;

	return jpegHeaderSize;
}

