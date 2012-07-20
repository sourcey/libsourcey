#ifndef AUDIO_INPUT2_H
#define AUDIO_INPUT2_H


#include "RtAudio.h"
#include <queue>


namespace Sourcey {
namespace Media {


/*
typedef char AUDIO_TYPE;
#define AUDIO_FORMAT RTAUDIO_SINT8
*/

typedef signed short AUDIO_TYPE;
#define AUDIO_FORMAT RTAUDIO_SINT16

/*
typedef signed long AUDIO_TYPE;
#define AUDIO_FORMAT RTAUDIO_SINT24

typedef signed long AUDIO_TYPE;
#define AUDIO_FORMAT RTAUDIO_SINT32

typedef float AUDIO_TYPE;
#define AUDIO_FORMAT RTAUDIO_FLOAT32

typedef double AUDIO_TYPE;
#define AUDIO_FORMAT RTAUDIO_FLOAT64
*/


struct MediaData {
	std::queue<AUDIO_TYPE*> buffer;
	unsigned int bufferFrames;
	unsigned int bufferBytes;
	unsigned int frameCounter;
	unsigned int maxQueuedFrames;
};


/**
 * Provides support for audio grabbing under Windows.
 */
class AudioMedia2
{
public:	
	/**
	 * Constructor.
	 * @param deviceId Port number in which the microphone is mapped to 
	 *        by the system.
	 * @param nChannels Number of channels the grabbing process will have: 
	 *		  2 = stereo
	 *        1 = mono.
	 * @param sampleRate Frequency of sampling.
	 */	 
	AudioMedia2(int deviceId,
		unsigned char nChannels,
		unsigned int sampleRate);
	virtual ~AudioMedia2();

public:
	virtual bool isInitialized() { return _isInitialized; };
  	virtual void start();
  	virtual void stop();
  	virtual unsigned getNextFrame(unsigned char* to, unsigned numBytesWanted);

protected:
	static int callback(void *outputBuffer, void *inputBuffer, unsigned int bufferSize,
           double streamTime, RtAudioStreamStatus status, void *data);

private:
	RtAudio*					_audio;
	MediaData*					_data;
	RtAudio::StreamParameters*	_iParams;
	bool						_isInitialized;
	static Poco::FastMutex		_mutex;
};


} } // namespace Sourcey::Media


#endif	//_AUDIO_INPUT_H_

