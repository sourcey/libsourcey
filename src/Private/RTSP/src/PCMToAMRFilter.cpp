#include "Anionu/RTSP/PCMToAMRFilter.h"


using namespace std;
using namespace Anionu;
using namespace Anionu::RTSP;
using namespace Anionu::Input;
using namespace Anionu::AV;


// ============================================================================
//
// PCM To AMR Filter
//
// NOTE: NOT IMPLIMENTED!!
//
// ============================================================================
PCMToAMRFilter* PCMToAMRFilter::createNew(UsageEnvironment& env, FramedSource* audioSource, 
									int bitRate,		/* 64000 */
									int numChannels,	/* 2 */  
									int sampleRate)		/* 44100 */ 
{
	return new PCMToAMRFilter(env, audioSource, bitRate, numChannels, sampleRate);
}


PCMToAMRFilter::~PCMToAMRFilter()
{	
	cout << "PCMToAMRFilter::~PCMToAMRFilter()" << endl;

	if (_audioEncoder)
		delete _audioEncoder;
	_audioEncoder = NULL;

	if (_inputBuffer)
		delete _inputBuffer;
	_inputBuffer = NULL;

	_isInitialized = false;
}


PCMToAMRFilter::PCMToAMRFilter(UsageEnvironment& env, FramedSource* audioSource, 
						int bitRate, 
						int numChannels, 
						int sampleRate) : 
					FramedFilter(env, audioSource),
					_audioSource(audioSource),
					_bitRate(bitRate),
					_numChannels(numChannels),
					_sampleRate(sampleRate),
					_inputBuffer(NULL),
					_inputBufferSize(0)
{
	cout << "PCMToAMRFilter::PCMToAMRFilter" << endl;

	_isInitialized = false;
	_audioEncoder = new AudioEncoder(CODEC_ID_AMR_NB, _bitRate, _numChannels, _sampleRate);

	_amrFrameSize = ((144.0 * _bitRate) / _sampleRate) + 0;

	if (_audioEncoder != NULL) {
		cout << "PCMToAMRFilter::_isInitialized " << endl;
		_isInitialized = true;
	}
}


// Redefined virtual functions:
void PCMToAMRFilter::doGetNextFrame() 
{
	unsigned int maxBytesToRead = MAX_AUDIO_PACKET_SIZE;

  	if (maxBytesToRead > _inputBufferSize) {
    	delete[] _inputBuffer; 
		_inputBuffer = new unsigned char[maxBytesToRead];
    	_inputBufferSize = maxBytesToRead;
  	}

  	// Be sure to give a clean buffer where to store the
  	memset(_inputBuffer, 0, _inputBufferSize);

  	// Arrange to read samples into the input buffer:
  	_audioSource->getNextFrame(_inputBuffer, maxBytesToRead, afterGettingFrame, this, 
								FramedSource::handleClosure, this);
		
}


void PCMToAMRFilter::afterGettingFrame(void* clientData, unsigned frameSize,
					unsigned numTruncatedBytes, struct timeval presentationTime,					
					unsigned durationInMicroseconds) 
{
	PCMToAMRFilter* klass = (PCMToAMRFilter*)clientData;
	klass->afterGettingFrame1(frameSize, numTruncatedBytes,
								presentationTime, durationInMicroseconds);
	
}


void PCMToAMRFilter::afterGettingFrame1(unsigned frameSize, unsigned numTruncatedBytes,
						struct timeval presentationTime,  unsigned durationInMicroseconds)
{
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
	fNumTruncatedBytes = numTruncatedBytes;
	//fDurationInMicroseconds = durationInMicroseconds;
	fDurationInMicroseconds = (unsigned int)(1000000.0 * _amrFrameSize / _bitRate);

	//cout << "PCMToAMRFilter::fDurationInMicroseconds: %d\n", fDurationInMicroseconds);

	afterGetting(this);
}


char const* PCMToAMRFilter::MIMEtype() const 
{
	return "audio/amr";
}
