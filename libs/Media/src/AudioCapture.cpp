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


AudioCapture::AudioCapture(int deviceId, int channels, int sampleRate) : 
	_deviceId(deviceId),
	_channels(channels),
	_sampleRate(sampleRate),
	_isInitialized(false)
{
	Log("debug") << "[AudioCapture" << this << "] Creating" << endl;

	_iParams.deviceId = _deviceId;
	_iParams.nChannels = _channels;
	_iParams.firstChannel = 0;

	if (_audio.getDeviceCount() < 1) {
		Log("error") << "[AudioCapture" << this << "] No audio devices found!" << endl;
		return;
	}

	// Let RtAudio print messages to stderr.
	_audio.showWarnings(true);
		
	// Open the audio stream or throw an exception.
	open(channels, sampleRate);

	_isInitialized = true;
}


AudioCapture::~AudioCapture()
{
	Log("debug") << "[AudioCapture" << this << "] Destroying" << endl;
	if (_audio.isStreamOpen()) 
		_audio.closeStream();
}


void AudioCapture::open(int channels, int sampleRate)
{
	Log("debug") << "[AudioCapture" << this << "] Opening: " << channels << ": " << sampleRate << endl;

	FastMutex::ScopedLock lock(_mutex);
	
	_channels = channels;
	_sampleRate = sampleRate;
	_iParams.nChannels = _channels;
	unsigned int nBufferFrames = 256; //512;

	try {
		//if (_audio.isStreamOpen()) 
		//	_audio.closeStream();	

		_audio.openStream(NULL, &_iParams, AUDIO_FORMAT, _sampleRate, &nBufferFrames, &AudioCapture::callback, (void*)this);
	}
	catch (RtError& e) {
		Log("error") << e.getMessage() << endl;
		throw Exception(e.getMessage());
	}
}


void AudioCapture::start()
{	
	Log("debug") << "[AudioCapture" << this << "] Starting" << endl;

	if (!isRunning()) {
		try {
			FastMutex::ScopedLock lock(_mutex);
			Log("debug") << "[AudioCapture" << this << "] Starting Stream" << endl;
			_audio.startStream();
		}
		catch (RtError& e) {
			Log("debug") << e.getMessage() << endl;
		}
	}
}


void AudioCapture::stop()
{	
	Log("debug") << "[AudioCapture" << this << "] Stopping" << endl;

	if (isRunning()) {
		try {
			FastMutex::ScopedLock lock(_mutex);
			_audio.stopStream();
		}
		catch (RtError& e) {
			Log("debug") << e.getMessage() << endl;
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


void AudioCapture::detach(const PacketDelegateBase& delegate) 
{
	PacketDispatcher::detach(delegate);
	Log("debug") << "[AudioCapture:" << this << "] Removed Delegate: " << refCount() << endl;
	if (refCount() == 0)
		stop();
}


int AudioCapture::callback(void* outputBuffer, void* inputBuffer, unsigned int nBufferFrames,
	double streamTime, RtAudioStreamStatus status, void *data)
{
	AudioCapture* klass = (AudioCapture*)data;

	Log("error") << "[AudioCapture" << klass << "] callback" << endl;

	if (status) 
		Log("error") << "[AudioCapture" << klass << "] Stream over/underflow detected" << endl;

	//if (inputBuffer == NULL) {
	//	Log("error") << "[AudioCapture" << klass << "] Input buffer is NULL." << endl;
	//	return 2;
	//}

	AudioPacket packet(
		(unsigned char*)inputBuffer, 
		nBufferFrames * klass->numChannels() * sizeof(AUDIO_DATA),
		(double)streamTime);

	/*
	Log("trace") << "[AudioCapture] AudioPacket: " 
		<< "\n\tPacket Ptr: " << inputBuffer << "\n"
		<< "\n\tPacket Size: " << packet.size << "\n"
		<< "\n\tStream Time: " << packet.time << "\n"
		<< endl;
		*/

	klass->dispatch(klass, packet);
	return 0;
}


bool AudioCapture::isInitialized() const
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _isInitialized;
}


bool AudioCapture::isRunning() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _audio.isStreamOpen();
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