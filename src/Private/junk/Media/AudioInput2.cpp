#include "Sourcey/Media/AudioMedia22.h"


using namespace std;
using namespace Poco;
using namespace Sourcey;
using namespace Sourcey::Media;


// ============================================================================
//
// Audio Media
//
// ============================================================================
FastMutex AudioMedia2::_mutex;

AudioMedia2::AudioMedia2(int deviceId, unsigned char nChannels, unsigned sampleRate) : 
	_isInitialized(false)
{
	cout << "AudioMedia2::AudioMedia2" << endl;

	_iParams = new RtAudio::StreamParameters;
	_iParams->deviceId = deviceId;
	_iParams->nChannels = nChannels;
	_iParams->firstChannel = 0;

	// Set our stream parameters for input only.
	_data = new MediaData;
	_data->frameCounter = 0;
	_data->bufferFrames = 512;
	_data->bufferBytes = _data->bufferFrames * nChannels * sizeof(AUDIO_TYPE);
	_data->maxQueuedFrames = 25;

	_audio = new RtAudio();
	if (_audio->getDeviceCount() < 1) {
		cout << "\nNo audio devices found!\n";
		return;
	}

	// Let RtAudio print messages to stderr.
	_audio->showWarnings(true);

	try {
		_audio->openStream(NULL, _iParams, AUDIO_FORMAT, sampleRate, &_data->bufferFrames, &AudioMedia2::callback, (void *)_data);
	}
	catch (RtError& e) {
		cout << '\n' << e.getMessage() << '\n' << endl;
		return;
	}

	_isInitialized = true;
}


AudioMedia2::~AudioMedia2()
{
	cout << "AudioMedia2::~AudioMedia2" << endl;

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


void AudioMedia2::start()
{
	cout << "AudioMedia2::start()" << endl;
	try {
		_audio->startStream();
	}
	catch (RtError& e) {
		cout << "\n" << e.getMessage() << '\n' << endl;
	}
}


void AudioMedia2::stop()
{
	cout << "AudioMedia2::stop()" << endl;
	try {
		_audio->stopStream();
	}
	catch (RtError& e) {
		cout << '\n' << e.getMessage() << '\n' << endl;
	}
}


// Interleaved buffers
int AudioMedia2::callback(void *outputBuffer, void *inputBuffer, unsigned int bufferSize,
						 double streamTime, RtAudioStreamStatus status, void *data)
{
	if (status) cout << "AudioMedia2: Stream over/underflow detected." << endl;

	FastMutex::ScopedLock lock(_mutex);

	MediaData *iData = (MediaData*)data;

	/*
	cout << "AudioMedia2::callback:\n" 
		<< "Buffer Size: " << bufferSize << "\n"
		<< "Stream Time: " << streamTime << "\n"
		<< "Media Buffer Size: " << iData->bufferBytes << "\n" 
		<< "Number of Samples: " << iData->buffer.size() << "\n" 
		<< endl;
		*/

	// Wait for our frame buffer to be drained before replenishing it.
	if (iData->buffer.size() > iData->maxQueuedFrames)
		return 0;	

	AUDIO_TYPE* samples = new AUDIO_TYPE[iData->bufferBytes];
	memcpy(samples, inputBuffer, iData->bufferBytes);

	iData->buffer.push(samples);
	iData->frameCounter++;
	return 0;
}



unsigned AudioMedia2::getNextFrame(unsigned char* to, unsigned numBytesWanted) 
{
	cout << "AudioMedia2::getNextFrame()" << endl;
	FastMutex::ScopedLock lock(_mutex);

	unsigned numBytesRead = 0;
	while (!_data->buffer.empty() && numBytesRead <= numBytesWanted)
	{			
		AUDIO_TYPE* samples = _data->buffer.front();
		memmove(&to[numBytesRead], samples, _data->bufferBytes);
		numBytesRead += _data->bufferBytes;
		_data->buffer.pop();
		delete samples;
		
		/*
		cout << "AudioMedia2::getNextFrame():\n"
			<< "Buffer size: " << _data->buffer.size() << "\n"
			<< "Bytes wanted: " << numBytesWanted << "\n"
			<< "Bytes read: " << numBytesRead << "\n"
			<< endl;
			*/
	}

	return numBytesRead;
}
