#ifndef AUDIO_INPUT_H
#define AUDIO_INPUT_H


#include "Sourcey/Media/RtAudio.h"

#include "Poco/Foundation.h"
#include "Poco/Thread.h"
#include "Poco/BasicEvent.h"

#include <iostream>
#include <queue>
#include <map>


namespace Sourcey {
namespace Media {


/*
typedef char AUDIO_DATA;
#define AUDIO_FORMAT RTAUDIO_SINT8
*/

typedef signed short AUDIO_DATA;
#define AUDIO_FORMAT RTAUDIO_SINT16

/*
typedef signed long AUDIO_DATA;
#define AUDIO_FORMAT RTAUDIO_SINT24

typedef signed long AUDIO_DATA;
#define AUDIO_FORMAT RTAUDIO_SINT32

typedef float AUDIO_DATA;
#define AUDIO_FORMAT RTAUDIO_FLOAT32

typedef double AUDIO_DATA;
#define AUDIO_FORMAT RTAUDIO_FLOAT64
*/


struct Samples 
{
	unsigned char* buffer;
	unsigned int bufferSize;
	unsigned int numBytesWanted;
	unsigned int numBytesRead;
	Samples(unsigned int numBytesWanted=32768)
	{
		numBytesRead = 0;
		bufferSize = numBytesWanted*2;
		buffer = new unsigned char[bufferSize];
	};
	~Samples()
	{
		delete buffer;
	}
};


struct AudioMediaData 
{
	std::queue<AUDIO_DATA*> buffer;
	unsigned int bufferFrames;
	unsigned int bufferBytes;
	unsigned int samplesCounter;
	unsigned int maxQueuedFrames;
	unsigned int deviceId;
	unsigned int channels;
	unsigned int sampleRate;	
};


class AudioMedia
{
public:	
	AudioMedia(int deviceId, int channels, int sampleRate);
	virtual ~AudioMedia();

public:
	virtual bool isInitialized() { return _isInitialized; };
  	virtual void start();
  	virtual void stop();
  	virtual bool getSamples(Samples& samples);
	virtual unsigned int getNextFrame(unsigned char* buffer, unsigned numBytesWanted);
	virtual unsigned int getDeviceId();
	virtual unsigned int getSampleRate();
	virtual unsigned int getNumChannels();
	virtual bool isRunning();

protected:
	static int callback(void *outputBuffer, void *inputBuffer, unsigned int bufferSize,
           double streamTime, RtAudioStreamStatus status, void *data);

private:
	bool						_isInitialized;
	RtAudio*					_audio;
	AudioMediaData*				_data;
	RtAudio::StreamParameters*	_iParams;
	static Poco::FastMutex		_mutex;
};


/*
class AudioStream
{
public:
	AudioStream(int deviceId, int channels, int sampleRate);
	AudioStream(int deviceId);
	~AudioStream();
	
	virtual AudioStream& operator >> (Samples& samples);
	virtual bool good();
	virtual bool start();	
	virtual void stop();

protected:

protected:
	AudioMedia* _capture;
	int _deviceId;
	int _channels;
	int _sampleRate;
};
*/


} } // namespace Sourcey::Media


#endif
