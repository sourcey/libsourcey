#include "Anionu/RTSP/PCMToMP3Source.h"


using namespace std;
using namespace Anionu;
using namespace Anionu::RTSP;
using namespace Anionu::Input;
using namespace Anionu::AV;


// ============================================================================
//
// PCM To MP3 Source
//
// ============================================================================
PCMToMP3Source* PCMToMP3Source::createNew(UsageEnvironment& env, FramedSource* audioSource, 
									int bitRate,		/* 64000 */
									int numChannels,	/* 2 */  
									int sampleRate)		/* 44100 */ 
{
	return new PCMToMP3Source(env, audioSource, bitRate, numChannels, sampleRate);
}


PCMToMP3Source::~PCMToMP3Source()
{	
	cout << "PCMToMP3Source::~PCMToMP3Source()" << endl;

	if (_audioEncoder)
		delete _audioEncoder;

	if (_inputBuffer)
		delete _inputBuffer;
}


PCMToMP3Source::PCMToMP3Source(UsageEnvironment& env, FramedSource* audioSource, int bitRate, int numChannels, int sampleRate) : 
	FramedSource(env), 
	_audioSource(audioSource),
	_bitRate(bitRate),
	_numChannels(numChannels),
	_sampleRate(sampleRate),
	_inputBuffer(NULL),
	_inputBufferSize(0)
{
	cout << "PCMToMP3Source::PCMToMP3Source" << endl;

	_isInitialized = false;

	//FramedSource* asPCM = AudioInputDevice::createNew(envir(), 0, 16, 1, 8000, 20); //(unsigned int)(1000.0 * 1152 / 44100)
	//_mp3FrameSize = ((144.0 * _bitRate) / _sampleRate) + 0;
	//_mp3FrameDuration = (_mp3FrameSize * 8.0) / _bitRate;

	_audioEncoder = new AudioEncoder(CODEC_ID_MP3, _bitRate, _numChannels, _sampleRate);
	if (_audioEncoder != NULL) {
		cout << "PCMToMP3Source::_isInitialized " << endl;
		_isInitialized = true;
	}
}


// Redefined virtual functions:
void PCMToMP3Source::doGetNextFrame() 
{
	unsigned int maxBytesToRead = MAX_AUDIO_PACKET_SIZE;

  	if (maxBytesToRead > _inputBufferSize) {
    	delete[] _inputBuffer; 
		_inputBuffer = new unsigned char[maxBytesToRead];
    	_inputBufferSize = maxBytesToRead;
  	}

	/*
  	// Be sure to give a clean buffer where to store the
  	memset(_inputBuffer, 0, _inputBufferSize);

  	// Arrange to read samples into the input buffer:
  	_audioSource->getNextFrame(_inputBuffer, maxBytesToRead, afterGettingFrame, this, 
								FramedSource::handleClosure, this);
								*/
	deliverFrame();
		
}


/*
void PCMToMP3Source::afterGettingFrame(void* clientData, unsigned frameSize,
					unsigned numTruncatedBytes, struct timeval presentationTime,					
					unsigned durationInMicroseconds) 
{
	PCMToMP3Source* klass = (PCMToMP3Source*)clientData;

	//if (frameSize > 0)
		klass->afterGettingFrame1(frameSize, numTruncatedBytes,
								presentationTime, durationInMicroseconds);
	
}

void PCMToMP3Source::afterGettingFrame1(unsigned frameSize, unsigned numTruncatedBytes,
						struct timeval presentationTime,  unsigned durationInMicroseconds)
*/


void PCMToMP3Source::deliverFrame() {
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

	int frameSize = 0;
	int encodedFrameSize = 0;
	const unsigned char* encodedFrame;
		
	//fPresentationTime = presentationTime;
	gettimeofday(&fPresentationTime, NULL);

	// Encode the buffer
	encodedFrame = _audioEncoder->encodeFrame(_inputBuffer, frameSize, &encodedFrameSize);

	if ((unsigned int) encodedFrameSize > fMaxSize)
		cout << "error: if (encodedFrameSize > fMaxSize)" << endl;

	if (encodedFrameSize) 
		memcpy(fTo, encodedFrame, encodedFrameSize);

	// Complete delivery to the client:
	fFrameSize = encodedFrameSize;
	//fNumTruncatedBytes = numTruncatedBytes;
	fDurationInMicroseconds = (unsigned int)(1000000.0 * 576 / 8000);

	//cout << "PCMToMP3Source::fDurationInMicroseconds: %d\n", fDurationInMicroseconds);

	afterGetting(this);
}


char const* PCMToMP3Source::MIMEtype() const 
{
	return "audio/MPEG";
}
