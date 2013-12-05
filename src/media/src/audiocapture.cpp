//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// LibSourcey is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//


#include "scy/media/audiocapture.h"
#include "scy/logger.h"

#ifdef HAVE_RTAUDIO


using std::endl;


namespace scy {
namespace av {


AudioCapture::AudioCapture(int deviceId, int channels, int sampleRate, RtAudioFormat format) : 
	_deviceId(deviceId),
	_channels(channels),
	_sampleRate(sampleRate),
	_format(format),
	_opened(false)
{
	TraceLS(this) << "Create" << endl;

	_iParams.deviceId = _deviceId;
	_iParams.nChannels = _channels;
	_iParams.firstChannel = 0;

	if (_audio.getDeviceCount() < 1) {
		WarnL << "No audio devices found!" << endl;
		return;
	}

	// Let RtAudio print messages to stderr.
	_audio.showWarnings(true);
		
	// Open the audio stream or throw an exception.
	open(); //channels, sampleRate
	TraceLS(this) << "Create: OK" << endl;
}


AudioCapture::~AudioCapture()
{
	TraceLS(this) << "Destroy" << endl;
}


void AudioCapture::open() //int channels, int sampleRate, RtAudioFormat format
{
	if (isOpen())
		close();

	Mutex::ScopedLock lock(_mutex);
	TraceLS(this) << "Opening: " << _channels << ": " << _sampleRate << endl;
	
	//_channels = channels;
	//_sampleRate = sampleRate;
	//_format = format;
	//_iParams.nChannels = _channels;
	unsigned int nBufferFrames = 1536; //256; //512; / 2

	try {	
		/*		
		  void openStream( RtAudio::StreamParameters *outputParameters,
						   RtAudio::StreamParameters *inputParameters,
						   RtAudioFormat format, unsigned int sampleRate,
						   unsigned int *bufferFrames, RtAudioCallback callback,
						   void *userData = NULL, RtAudio::StreamOptions *options = NULL, RtAudioErrorCallback errorCallback = NULL );
				   */
		_audio.openStream(nullptr, &_iParams, _format, _sampleRate, &nBufferFrames, &AudioCapture::audioCallback, (void*)this, nullptr, AudioCapture::errorCallback);

		_error = "";
		_opened = true;
		TraceLS(this) << "Opening: OK" << endl;
	}
	catch (RtError& e) {
		setError("Cannot open audio capture: " + e.getMessage());
	}
	catch (...) {
		setError("Cannot open audio capture.");
	}
}


void AudioCapture::close()
{	
	TraceLS(this) << "Closing" << endl;
	try {
		Mutex::ScopedLock lock(_mutex);
		_opened = false;
		if (_audio.isStreamOpen())
			_audio.closeStream();
		TraceLS(this) << "Closing: OK" << endl;
	}
	catch (RtError& e) {
		setError("Cannot close audio capture: " + e.getMessage());
	}
	catch (...) {
		setError("Cannot close audio capture.");
	}
}


void AudioCapture::start()
{	
	TraceLS(this) << "Starting" << endl;

	if (!running()) {
		try {
			Mutex::ScopedLock lock(_mutex);
			_audio.startStream();
			_error = "";
			TraceLS(this) << "Starting: OK" << endl;
		}
		catch (RtError& e) {
			setError("Cannot start audio capture: " + e.getMessage());
		}
		catch (...) {
			setError("Cannot start audio capture.");
		}
	}
}


void AudioCapture::stop()
{	
	TraceLS(this) << "Stopping" << endl;

	if (running()) {
		try {
			Mutex::ScopedLock lock(_mutex);
			TraceLS(this) << "Stopping: Before" << endl;
			_audio.stopStream();
			TraceLS(this) << "Stopping: OK" << endl;
		}
		catch (RtError& e) {
			setError("Cannot stop audio capture: " + e.getMessage());
		}
		catch (...) {
			setError("Cannot stop audio capture.");
		}
	}
}


/*
void AudioCapture::attach(const PacketDelegateBase& delegate)
{
	PacketSignal::attach(delegate);
	TraceLS(this) << "Added Delegate: " << refCount() << endl;
	if (refCount() == 1)
		start();
}


bool AudioCapture::detach(const PacketDelegateBase& delegate) 
{
	if (PacketSignal::detach(delegate)) {
		TraceLS(this) << "Removed Delegate: " << refCount() << endl;
		if (refCount() == 0)
			stop();
		TraceLS(this) << "Removed Delegate: OK" << endl;
		return true;
	}
	return false;
}
*/


int AudioCapture::audioCallback(void* /* outputBuffer */, void* inputBuffer, unsigned int nBufferFrames,
	double /* streamTime */, RtAudioStreamStatus status, void* data)
{
	AudioCapture* klass = (AudioCapture*)data;
	AudioPacket packet;
	
	if (status) 
		ErrorL << "Stream over/underflow detected" << endl;

	assert(inputBuffer);
	if (inputBuffer == nullptr) {
		ErrorL << "Input buffer is nullptr." << endl;
		return 2;
	} 

	{
		Mutex::ScopedLock lock(klass->_mutex);

		int size = 2;
		RtAudioFormat format = klass->_format;
		//  8-bit signed integer.
		if (format == RTAUDIO_SINT8)
			size = 1;
		// 16-bit signed integer.
		else if (format == RTAUDIO_SINT16)
			size = 2;
		// Lower 3 bytes of 32-bit signed integer.
		else if (format == RTAUDIO_SINT24)
			size = 4;
		// 32-bit signed integer.
		else if (format == RTAUDIO_SINT32)
			size = 4;
		// Normalized between plus/minus 1.0.
		else if (format == RTAUDIO_FLOAT32)
			size = 4;
		// Normalized between plus/minus 1.0.
		else if (format == RTAUDIO_FLOAT64)
			size = 8;
		else assert(0 && "unknown audio capture format");

		packet.setData((char*)inputBuffer, nBufferFrames * klass->_channels * size);
	}

	/*
	AudioPacket packet((char*)inputBuffer, 
		nBufferFrames * klass->_numChannels * size, //sizeof(AUDIO_DATA),

		// TODO: Add the process time to this value for 
		// consistency with the time value of other packets.
		(double)streamTime);
		*/

	//TraceL << "[AudioCapture] AudioPacket: " 
	//	<< "\n\tPacket Ptr: " << inputBuffer
	//	<< "\n\tPacket Size: " << packet.size() 
	//	<< "\n\tStream Time: " << packet.time
	//	<< endl;

	klass->emit(packet);
	return 0;
}


void AudioCapture::errorCallback(RtError::Type type, const std::string& errorText)
{
	ErrorL << "Audio system error: " << errorText << endl;
}


void AudioCapture::setError(const std::string& message, bool throwExec)
{
	ErrorLS(this) << "Error: " << message << endl;
	_error = message;
	if (throwExec)
		throw std::runtime_error(message);
}


RtAudioFormat AudioCapture::format() const
{ 
	Mutex::ScopedLock lock(_mutex);
	return _format;
}


bool AudioCapture::isOpen() const
{ 
	Mutex::ScopedLock lock(_mutex);
	return _opened;
}


bool AudioCapture::running() const
{
	Mutex::ScopedLock lock(_mutex);
	return _audio.isStreamRunning();
}


int AudioCapture::deviceId() const 
{
	Mutex::ScopedLock lock(_mutex);
	return _deviceId;
}


int AudioCapture::sampleRate() const 
{
	Mutex::ScopedLock lock(_mutex);
	return _sampleRate;
}


int AudioCapture::numChannels() const 
{
	Mutex::ScopedLock lock(_mutex);
	return _channels;
}

		
void AudioCapture::getEncoderFormat(Format& iformat) 
{
	Mutex::ScopedLock lock(_mutex);
	iformat.audio.sampleFmt = "s16"; // TODO: Convert from RtAudioFormat to SampleFormat
	iformat.audio.channels = _channels;
	iformat.audio.sampleRate = _sampleRate;
	iformat.audio.enabled = true;
}


} } // namespace scy::av


#endif