//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is is distributed under a dual license that allows free, 
// open source use and closed source use under a standard commercial
// license.
//
// Non-Commercial Use:
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// Commercial Use:
// Please contact mail@sourcey.com
//


#include "Sourcey/Media/AudioCapture.h"
#include "Sourcey/Logger.h"


using namespace std;
using namespace Poco;
using namespace Sourcey;


namespace Sourcey {
namespace Media {


AudioCapture::AudioCapture(int deviceId, int channels, int sampleRate, RtAudioFormat format) : 
	_deviceId(deviceId),
	_channels(channels),
	_sampleRate(sampleRate),
	_format(format),
	_isOpen(false)
{
	Log("trace") << "[AudioCapture:" << this << "] Creating" << endl;

	_iParams.deviceId = _deviceId;
	_iParams.nChannels = _channels;
	_iParams.firstChannel = 0;

	if (_audio.getDeviceCount() < 1) {
		Log("warn") << "[AudioCapture:" << this << "] No audio devices found!" << endl;
		return;
	}

	// Let RtAudio print messages to stderr.
	_audio.showWarnings(true);
		
	// Open the audio stream or throw an exception.
	open(); //channels, sampleRate
}


AudioCapture::~AudioCapture()
{
	Log("trace") << "[AudioCapture:" << this << "] Destroying" << endl;
}


void AudioCapture::open() //int channels, int sampleRate, RtAudioFormat format
{
	Log("trace") << "[AudioCapture:" << this << "] Opening: " << _channels << ": " << _sampleRate << endl;

	close();

	FastMutex::ScopedLock lock(_mutex);
	
	//_channels = channels;
	//_sampleRate = sampleRate;
	//_format = format;
	//_iParams.nChannels = _channels;
	unsigned int nBufferFrames = 256; //512;

	try {
		_audio.openStream(NULL, &_iParams, _format, _sampleRate, &nBufferFrames, &AudioCapture::callback, (void*)this);
		_error = "";
		_isOpen = true;
		Log("trace") << "[AudioCapture:" << this << "] Opening: OK" << endl;
	}
	catch (RtError& e) {
		setError("Failed to open audio capture: " + e.getMessage());
	}
	catch (...) {
		setError("Failed to open audio capture.");
	}
}


void AudioCapture::close()
{	
	Log("trace") << "[AudioCapture:" << this << "] Closing" << endl;
	try {
		FastMutex::ScopedLock lock(_mutex);
		_isOpen = false;
		if (_audio.isStreamOpen())
			_audio.closeStream();
		Log("trace") << "[AudioCapture:" << this << "] Closing: OK" << endl;
	}
	catch (RtError& e) {
		setError("Failed to close audio capture: " + e.getMessage());
	}
	catch (...) {
		setError("Failed to close audio capture.");
	}
}


void AudioCapture::start()
{	
	Log("trace") << "[AudioCapture:" << this << "] Starting" << endl;

	if (!isRunning()) {
		try {
			FastMutex::ScopedLock lock(_mutex);
			_audio.startStream();
			_error = "";
			Log("trace") << "[AudioCapture:" << this << "] Starting: OK" << endl;
		}
		catch (RtError& e) {
			setError("Failed to start audio capture: " + e.getMessage());
		}
		catch (...) {
			setError("Failed to start audio capture.");
		}
	}
}


void AudioCapture::stop()
{	
	Log("trace") << "[AudioCapture:" << this << "] Stopping" << endl;

	if (isRunning()) {
		try {
			FastMutex::ScopedLock lock(_mutex);
			Log("trace") << "[AudioCapture:" << this << "] Stopping: Before" << endl;
			_audio.stopStream();
			Log("trace") << "[AudioCapture:" << this << "] Stopping: OK" << endl;
		}
		catch (RtError& e) {
			setError("Failed to stop audio capture: " + e.getMessage());
		}
		catch (...) {
			setError("Failed to stop audio capture.");
		}
	}

}


void AudioCapture::attach(const PacketDelegateBase& delegate)
{
	PacketDispatcher::attach(delegate);
	Log("debug") << "[AudioCapture:" << this << "] Added Delegate: " << refCount() << endl;
	if (refCount() == 1)
		start();
}


bool AudioCapture::detach(const PacketDelegateBase& delegate) 
{
	if (PacketDispatcher::detach(delegate)) {
		Log("debug") << "[AudioCapture:" << this << "] Removed Delegate: " << refCount() << endl;
		if (refCount() == 0)
			stop();
		Log("debug") << "[AudioCapture:" << this << "] Removed Delegate: OK" << endl;
		return true;
	}
	return false;
}


void AudioCapture::setError(const string& message)
{
	_error = message;
	Log("error") << "[AudioCapture::" << this << "] Error: " << message << endl;
	throw Exception(message);
}


int AudioCapture::callback(void* outputBuffer, void* inputBuffer, unsigned int nBufferFrames,
	double streamTime, RtAudioStreamStatus status, void* data)
{
	AudioCapture* klass = (AudioCapture*)data;

	//Log("trace") << "[AudioCapture::" << klass << "] Callback" << endl;
	
	if (status) 
		Log("error") << "[AudioCapture::" << klass << "] Stream over/underflow detected" << endl;

	assert(inputBuffer != NULL);
	if (inputBuffer == NULL) {
		Log("error") << "[AudioCapture::" << klass << "] Input buffer is NULL." << endl;
		return 2;
	} 

	//if (!klass->isOpen())
	//	return 2;

	int size = 2;
	RtAudioFormat format = klass->format();
	// - \e RTAUDIO_SINT8:   8-bit signed integer.
	if (format == RTAUDIO_SINT8)
		size = 1;
    // - \e RTAUDIO_SINT16:  16-bit signed integer.
	else if (format == RTAUDIO_SINT16)
		size = 2;
    // - \e RTAUDIO_SINT24:  Lower 3 bytes of 32-bit signed integer.
	else if (format == RTAUDIO_SINT24)
		size = 4;
    // - \e RTAUDIO_SINT32:  32-bit signed integer.
	else if (format == RTAUDIO_SINT32)
		size = 4;
    // - \e RTAUDIO_FLOAT32: Normalized between plus/minus 1.0.
	else if (format == RTAUDIO_FLOAT32)
		size = 4;
    // - \e RTAUDIO_FLOAT64: Normalized between plus/minus 1.0.
	else if (format == RTAUDIO_FLOAT64)
		size = 8;
	else assert(0 && "unknown audio capture format");

	AudioPacket packet(
		(unsigned char*)inputBuffer, 
		nBufferFrames * klass->numChannels() * size, //sizeof(AUDIO_DATA),
		(double)streamTime);

	//Log("trace") << "[AudioCapture] AudioPacket: " 
	//	<< "\n\tPacket Ptr: " << inputBuffer
	//	<< "\n\tPacket Size: " << packet.size() 
	//	<< "\n\tStream Time: " << packet.time
	//	<< endl;

	klass->dispatch(klass, packet);
	//Log("trace") << "[AudioCapture::" << klass << "] Callback: OK" << endl;
	return 0;
}


RtAudioFormat AudioCapture::format() const
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _format;
}


bool AudioCapture::isOpen() const
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _isOpen;
}


bool AudioCapture::isRunning() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _audio.isStreamRunning();
}


int AudioCapture::deviceId() const 
{
	FastMutex::ScopedLock lock(_mutex);
	return _deviceId;
}


int AudioCapture::sampleRate() const 
{
	FastMutex::ScopedLock lock(_mutex);
	return _sampleRate;
}


int AudioCapture::numChannels() const 
{
	FastMutex::ScopedLock lock(_mutex);
	return _channels;
}


} } // namespace Sourcey::Media