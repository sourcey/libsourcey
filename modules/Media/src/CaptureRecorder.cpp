//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2002 Sourcey
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


#include "Sourcey/Media/CaptureRecorder.h"
#include "Sourcey/Media/AVEncoder.h"
#include "Sourcey/Logger.h"


using namespace std;
using namespace Poco;


namespace Sourcey {
namespace Media {


CaptureRecorder::CaptureRecorder(const RecorderParams& params, 
								 Media::VideoCapture* video, 
								 Media::AudioCapture* audio,
								 bool destroyOnStop) : 
	AVEncoder(params),
	_video(video), 
	_audio(audio), 
	_destroyOnStop(destroyOnStop)
{
	Log("debug") << "[CaptureRecorder" << this << "] Creating" << endl;
}


CaptureRecorder::~CaptureRecorder()
{
	Log("debug") << "[CaptureRecorder" << this << "] Destroying" << endl;
	// A call to stop is required before destruction.
	//assert(!isReady());	

	uninitialize(); // cleanup() will be called
}


void CaptureRecorder::initialize()
{
	if (!_video && !_audio)
		throw Exception("No AV available devices for encoding.");

	// Disable AV codecs depending on capture availability
	if (!_video) {
		_params.iformat.video.enabled = false;
		_params.oformat.video.enabled = false;
	}
	if (!_audio) {
		_params.iformat.audio.enabled = false;
		_params.oformat.audio.enabled = false;
	}

	AVEncoder::initialize(); // will throw on failure
}

void CaptureRecorder::uninitialize()
{
	AVEncoder::uninitialize();
}


void CaptureRecorder::start()
{
	FastMutex::ScopedLock lock(_mutex);

	Log("debug") << "[CaptureRecorder" << this << "] Starting:"
		<< "\n\tPID: " << this
		<< "\n\tPath: " << _params.ofile
		<< "\n\tStarting At: " << time(0)
		<< "\n\tVideo: " << _params.oformat.video.toString()
		<< "\n\tAudio: " << _params.oformat.audio.toString()
		<< "\n\tUsing Video: " << _video
		<< "\n\tUsing Audio: " << _audio
		<< "\n\tStopping At: " << _params.stopAt
		<< "\n\tDuration: " << _params.stopAt-time(0) 
		<< endl;

	if (!isReady()) {
		//throw Exception("The encoder is not initialized.");
		initialize();
	}

	try {
		if (_video) {
			_video->attach(videoDelegate(this, &CaptureRecorder::onVideoCapture, _params.oformat.video.fps));	
		}
		if (_audio) {
			_audio->attach(audioDelegate(this, &CaptureRecorder::onAudioCapture));
		}

		setState(this, EncoderState::Encoding);
	} 
	catch (Exception& exc) {
		Log("error") << "[CaptureRecorder" << this << "] Error: " << exc.displayText() << endl;

		cleanup();		
		setStateMessage(exc.displayText());
		setState(this, EncoderState::Failed);
		exc.rethrow();
	}
}


void CaptureRecorder::stop() 
{
	FastMutex::ScopedLock lock(_mutex);

	Log("debug") << "[CaptureRecorder" << this << "] Stopping:"
		<< "\n\tPID: " << this
		<< "\n\tState: " << state()
		<< endl;

	//assert(!isIdle());

	// NOTE: We do not destroy capture objects
	if (_video) {
		_video->detach(videoDelegate(this, &CaptureRecorder::onVideoCapture));
	}
	if (_audio) {
		_audio->detach(audioDelegate(this, &CaptureRecorder::onAudioCapture));
	}

	setState(this, EncoderState::Ready);
	
	if (_destroyOnStop)
		delete this;
}


void CaptureRecorder::cleanup()
{
	AVEncoder::cleanup();
	
	if (_video) {
		_video->detach(videoDelegate(this, &CaptureRecorder::onVideoCapture));
	}
	if (_audio) {
		_audio->detach(audioDelegate(this, &CaptureRecorder::onAudioCapture));
	}
}


void CaptureRecorder::setVideoCapture(Media::VideoCapture* video)
{
	FastMutex::ScopedLock lock(_mutex);

	assert(!isReady());
	_video = video;
}


void CaptureRecorder::setAudioCapture(Media::AudioCapture* audio)
{
	FastMutex::ScopedLock lock(_mutex);

	assert(!isReady());
	_audio = audio;
}


void CaptureRecorder::extend(time_t stopAt) 
{
	FastMutex::ScopedLock lock(_mutex);

	Log("debug") << "[CaptureRecorder" << this << "] Extending to: " << stopAt << endl;
	assert(isReady());
	_params.stopAt = stopAt;
}


void CaptureRecorder::onVideoCapture(void*, VideoPacket& packet) 
{	
	FastMutex::ScopedLock lock(_mutex);

	if (!packet.mat || packet.mat->empty() || !isReady()) {
		Log("error") << "[CaptureRecorder" << this << "] Dropping video frame." << endl;
		return;
	}
	
	// Check the time limit and exit command			
	if (_params.stopAt &&
		_params.stopAt < time(0)) {
		Log("debug") << "[CaptureRecorder" << this << "] Time limit reached: " << _params.stopAt << endl;
		uninitialize();
		return;
	}

	try {
		encodeVideo(
			(unsigned char*)packet.mat->data, 
			packet.mat->total(), 
			packet.mat->cols, 
			packet.mat->rows); 
	} 
	catch (Exception& exc) {
		Log("error") << "[CaptureRecorder" << this << "] Encode Error: " << exc.displayText() << endl;
	}
}


void CaptureRecorder::onAudioCapture(void*, AudioPacket& packet)
{
	if (!packet.size() || !isReady()) {
		Log("error") << "[CaptureRecorder" << this << "] Dropping audio frame." << endl;
		return;
	}
			
	try  {
		encodeAudio((unsigned char*)packet.data(), packet.size()); 
	} catch (Exception& exc) {
		Log("error") << "[CaptureRecorder" << this << "] Audio Error: " << exc.displayText() << endl;
	}			
}


} } // namespace Sourcey::Media