#include "Sourcey/Media/Recorder.h"
#include "Sourcey/Media/AVEncoder.h"
#include "Sourcey/Base/Logger.h"


using namespace std;
using namespace Poco;


namespace Sourcey {
namespace Media {


// ============================================================================
//
// Audio/Video Recorder
//
// ============================================================================
Recorder::Recorder(const RecorderParams& params, 
								   Media::VideoCapture* video, 
								   Media::AudioCapture* audio,
								   bool destroyOnStop) : 
	AVEncoder(params),
	_video(video), 
	_audio(audio), 
	_destroyOnStop(destroyOnStop)
{
	Log("trace") << "[Recorder" << this << "] Recorder()" << endl;
}


Recorder::~Recorder()
{
	Log("trace") << "[Recorder" << this << "] ~Recorder()" << endl;
	// A call to stop is required before destruction.
	//assert(!isReady());	

	uninitialize(); // cleanup() will be called
}


/*
	Log("trace") << "[Recorder" << this << "] ~Recorder(): END" << endl;
*/

void Recorder::initialize()
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

void Recorder::uninitialize()
{
	AVEncoder::uninitialize();
}


void Recorder::start()
{
	FastMutex::ScopedLock lock(_mutex);

	Log("trace") << "[Recorder" << this << "] Starting:"
		<< "\n\tPID: " << this
		<< "\n\tPath: " << _params.ofile
		<< "\n\tStarting At: " << time(0)
		<< "\n\tVideo: " << _params.oformat.video.toString()
		<< "\n\tAudio: " << _params.oformat.audio.toString()
		<< "\n\tStopping At: " << _params.stopAt
		<< "\n\tDuration: " << _params.stopAt-time(0) 
		<< endl;

	if (!isReady()) {
		initialize();
	}
	//throw Exception("The encoder is not initialized.");

	try {
		if (_video) {
			_video->addCallback(VideoCallback<Recorder>(this, &Recorder::onVideoCapture, _params.oformat.video.fps));	
		}
		if (_audio) {
			_audio->addCallback(AudioCallback<Recorder>(this, &Recorder::onAudioCapture));
		}

		setState(EncoderState::Encoding);
	} 
	catch (Exception& exc) {
		Log("error") << "[Recorder" << this << "] Error: " << exc.displayText() << endl;

		cleanup();		
		state().setMessage(exc.displayText());
		setState(EncoderState::Failed);
		exc.rethrow();
	}
}


void Recorder::stop() 
{
	FastMutex::ScopedLock lock(_mutex);

	Log("trace") << "[Recorder" << this << "] Stopping:"
		<< "\n\tPID: " << this
		<< "\n\tState: " << state().toString()
		<< endl;

	//assert(!isIdle());

	// NOTE: We do not destroy capture objects
	if (_video) {
		_video->removeCallback(VideoCallback<Recorder>(this, &Recorder::onVideoCapture));
	}
	if (_audio) {
		_audio->removeCallback(AudioCallback<Recorder>(this, &Recorder::onAudioCapture));
	}

	setState(EncoderState::Ready);
	
	if (_destroyOnStop)
		delete this;
}


void Recorder::cleanup()
{
	AVEncoder::cleanup();
	
	if (_video) {
		_video->removeCallback(VideoCallback<Recorder>(this, &Recorder::onVideoCapture));
	}
	if (_audio) {
		_audio->removeCallback(AudioCallback<Recorder>(this, &Recorder::onAudioCapture));
	}
}


void Recorder::setVideoCapture(Media::VideoCapture* video)
{
	FastMutex::ScopedLock lock(_mutex);

	assert(!isReady());
	_video = video;
}


void Recorder::setAudioCapture(Media::AudioCapture* audio)
{
	FastMutex::ScopedLock lock(_mutex);

	assert(!isReady());
	_audio = audio;
}


void Recorder::extend(time_t stopAt) 
{
	FastMutex::ScopedLock lock(_mutex);

	Log("trace") << "[Recorder" << this << "] Extending to: " << stopAt << endl;
	assert(isReady());
	_params.stopAt = stopAt;
}


void Recorder::onVideoCapture(const VideoPacket& packet) 
{	
	FastMutex::ScopedLock lock(_mutex);

	if (!packet.mat || packet.mat->empty() || !isReady()) {
		Log("error") << "[Recorder" << this << "] Dropping video frame." << endl;
		return;
	}
	
	// Check the time limit and exit command			
	if (_params.stopAt &&
		_params.stopAt < time(0)) {
		Log("trace") << "[Recorder" << this << "] Time limit reached: " << _params.stopAt << endl;
		uninitialize();
		return;
	}

	// HACK: Resizing the Mat image here because FFMpeg's swscale 
	// is having unpredeictable results and it's late... yawn...
	//cv::Mat resized;	
	//cv::resize(*packet.mat, resized, cv::Size(_params.oformat.video.width, _params.oformat.video.height));

	try {
		encodeVideo(
			(unsigned char*)packet.mat->data, 
			((IplImage*)packet.mat)->imageSize, 
			packet.mat->cols, 
			packet.mat->rows//);			
			//(unsigned char*)resized.data, 
			//((IplImage)resized).imageSize, 
			//resized.cols, 
			//resized.rows
			); 
	} 
	catch (Exception& exc) {
		Log("error") << "[Recorder" << this << "] Encode Error: " << exc.displayText() << endl;
	}
}


void Recorder::onAudioCapture(const AudioPacket& packet)
{
	if (!packet.size || !isReady()) {
		Log("error") << "[Recorder" << this << "] Dropping audio frame." << endl;
		return;
	}
			
	try  {
		encodeAudio((unsigned char*)packet.data, packet.size); 
	} catch (Exception& exc) {
		Log("error") << "[Recorder" << this << "] Audio Error: " << exc.displayText() << endl;
	}			
}


} } // namespace Sourcey::Media