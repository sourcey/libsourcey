#include "Sourcey/Media/AudioMedia.h"


using namespace std;
using namespace Poco;
using namespace Sourcey;
using namespace Sourcey::Media;


// ============================================================================
//
// Audio Media
//
// TODO: Check for update possibilities: 
// http://github.com/broxtronix/phosphoressence/blob/cdff2ff7713daee05e4e4de7eba7bd7261a5cd27/src/AudioMedia.cc
//
// ============================================================================
FastMutex AudioMedia::_mutex;


AudioMedia::AudioMedia(int deviceId, int channels, int sampleRate) : 
	_isInitialized(false)
{
	cout << "AudioMedia::AudioMedia" << endl;

	_iParams = new RtAudio::StreamParameters;
	_iParams->deviceId = deviceId;
	_iParams->nChannels = channels;
	_iParams->firstChannel = 0;

	// Set our stream parameters for input only.
	_data = new AudioMediaData;
	_data->samplesCounter = 0;
	_data->bufferFrames = 512;
	_data->bufferBytes = _data->bufferFrames * channels * sizeof(AUDIO_DATA);
	_data->maxQueuedFrames = 25;
	_data->deviceId = deviceId;
	_data->channels = channels;
	_data->sampleRate = sampleRate;

	_audio = new RtAudio();
	if (_audio->getDeviceCount() < 1) {
		cout << "No audio devices found!" << endl;
		return;
	}

	// Let RtAudio print messages to stderr.
	_audio->showWarnings(true);

	try {
		_audio->openStream(NULL, _iParams, AUDIO_FORMAT, sampleRate, &_data->bufferFrames, &AudioMedia::callback, (void *)_data);
	}
	catch (RtError& e) {
		cout << e.getMessage() << endl;
		return;
	}

	_isInitialized = true;
}


AudioMedia::~AudioMedia()
{
	cout << "AudioMedia::~AudioMedia" << endl;

	if (_audio->isStreamOpen()) 
		_audio->closeStream();

	while (!_data->buffer.empty())
	{			
		delete _data->buffer.front();
		_data->buffer.pop();
	}

	delete _audio;
	delete _data;
	delete _iParams;
}


void AudioMedia::start()
{
	try {
		_audio->startStream();
	}
	catch (RtError& e) {
		cout << e.getMessage() << endl;
	}
}


void AudioMedia::stop()
{
	try {
		_audio->stopStream();
	}
	catch (RtError& e) {
		cout << e.getMessage() << endl;
	}
}


virtual AudioMedia::isRunning()
{
	return _audio->isStreamRunning();
}


int AudioMedia::callback(void *outputBuffer, void *inputBuffer, unsigned int bufferSize,
						 double streamTime, RtAudioStreamStatus status, void *data)
{
	if (status) cout << "AudioMedia: Stream over/underflow detected." << endl;
	FastMutex::ScopedLock lock(_mutex);

	AudioMediaData *iData = (AudioMediaData*)data;

	/*
	cout << "AudioMedia::callback:\n" 
		<< "Buffer Size: " << bufferSize << "\n"
		<< "Stream Time: " << streamTime << "\n"
		<< "Media Buffer Size: " << iData->bufferBytes << "\n" 
		<< "Number of Samples: " << iData->buffer.size() << "\n" 
		<< endl;
		*/

	// Wait for our samples buffer to be drained before replenishing it.
	if (iData->buffer.size() > iData->maxQueuedFrames)
		return 0;	

	AUDIO_DATA* samples = new AUDIO_DATA[iData->bufferBytes];
	memcpy(samples, inputBuffer, iData->bufferBytes);

	iData->buffer.push(samples);
	iData->samplesCounter++;
	return 0;
}


bool AudioMedia::getSamples(Samples& samples) 
{
	FastMutex::ScopedLock lock(_mutex);
	
	if (!_audio->isStreamRunning()) start();

	samples.numBytesRead = 0;
	while (!_data->buffer.empty() && samples.numBytesRead <= samples.numBytesWanted)
	{			
		AUDIO_DATA* source = _data->buffer.front();
		memmove(&samples.buffer[samples.numBytesRead], source, _data->bufferBytes);
		samples.numBytesRead += _data->bufferBytes;
		_data->buffer.pop();
		delete source;
		
		cout << "AudioMedia::getSamples():\n"
			<< "Buffer size: " << _data->buffer.size() << "\n"
			<< "Bytes wanted: " << samples.numBytesWanted << "\n"
			<< "Bytes read: " << samples.numBytesRead << "\n"
			<< endl;
	}

	if (samples.numBytesRead)
		return true;
	return false;
}


unsigned int AudioMedia::getNextFrame(unsigned char* buffer, unsigned numBytesWanted) 
{
	FastMutex::ScopedLock lock(_mutex);

	unsigned int numBytesRead = 0;
	while (!_data->buffer.empty() && numBytesRead <= numBytesWanted)
	{			
		AUDIO_DATA* samples = _data->buffer.front();
		memmove(&buffer[numBytesRead], samples, _data->bufferBytes);
		numBytesRead += _data->bufferBytes;
		_data->buffer.pop();
		delete samples;
		
		cout << "AudioMedia::getNextFrame():\n"
			<< "Buffer size: " << _data->buffer.size() << "\n"
			<< "Bytes wanted: " << numBytesWanted << "\n"
			<< "Bytes read: " << numBytesRead << "\n"
			<< endl;
	}

	return numBytesRead;
}


unsigned int AudioMedia::getDeviceId()
{
	FastMutex::ScopedLock lock(_mutex);
	return _data->deviceId;
}


unsigned int AudioMedia::getSampleRate()
{
	FastMutex::ScopedLock lock(_mutex);
	return _data->sampleRate;
}


unsigned int AudioMedia::getNumChannels()
{
	FastMutex::ScopedLock lock(_mutex);
	return _data->channels;
}


/*
// ============================================================================
//
// Audio Stream
//
// ============================================================================
AudioStream::AudioStream(int deviceId, int channels, int sampleRate) : 
	_deviceId(deviceId),
	_channels(channels),
	_sampleRate(sampleRate),
	_capture(NULL)
{
	//if (_deviceId >= 0)
	//	start();
}


AudioStream::AudioStream(int deviceId) : 
	_deviceId(deviceId),
	_channels(2),
	_sampleRate(44100),
	_capture(NULL)
{
	//if (_deviceId >= 0)
	//	start();
}


AudioStream::~AudioStream()
{
	delete _capture;
}


bool AudioStream::start()
{
	_capture = new AudioMedia(_deviceId, _channels, _sampleRate);
	_capture->start();
	return _capture->isInitialized();
}

	
AudioStream& AudioStream::operator >> (Samples& samples)
{
	if (samples.numBytesWanted == 0)
		samples.numBytesWanted = 32768;
	if (samples.bufferSize == 0)
		samples.bufferSize = samples.numBytesWanted * 2;
	if (samples.buffer == 0)
		samples.buffer = new unsigned char[samples.bufferSize];

	if (_capture)
		_capture->getSamples(samples);

	return *this;
}


bool AudioStream::good()
{
	return _capture 
		&& _capture->isInitialized();
}


void AudioStream::stop()
{
	if (_capture)
		_capture->stop();
}
*/