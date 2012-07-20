#include "Sourcey/Media/AudioCaptureEncoder.h"

#include "assert.h"

using namespace std;
using namespace Poco;


namespace Sourcey {
namespace Media {


// ============================================================================
//
// Audio Capture Encoder
//
// ============================================================================
AudioCaptureEncoder::AudioCaptureEncoder(AudioCapture* capture, const AudioParams& params, bool destroyOnStop) : 
	_params(params), 
	_capture(capture),
	//_buffer(new unsigned char[MAX_AUDIO_PACKET_SIZE]),
	_destroyOnStop(destroyOnStop)
{
	cout << "[AudioCaptureEncoder] Initializing..." << endl;
	assert(_capture);	
	setState(EncoderState::Idle);
}


AudioCaptureEncoder::~AudioCaptureEncoder() {
	cout << "[AudioCaptureEncoder] Destroying..." << endl;

	// A call to stop() is required before destruction.
	assert(!_encoder.isInitialized());
}


void AudioCaptureEncoder::start() 
{
	cout << "[AudioCaptureEncoder] Starting..." << endl;
	try {
		assert(_capture);
		//assert(_capture->width());
		//assert(_capture->height());
		assert(!_encoder.isInitialized());

		_encoder.initEncodeContext(_params);

		AudioParams iParams;
		//iParams.codec = AudioCodecID::NoAudio;
		//iParams.width = _capture->width();
		//iParams.height = _capture->height();
		//iParams.pixfmt = AudioPixelFormat::YUV420P;
		//iParams.pixfmt = AudioPixelFormat::RGB24;
		//iParams.pixfmt = AudioPixelFormat::BGR24;
		//_encoder.initScaleContext(iParams);
		
		_capture->start(AudioCallback<AudioCaptureEncoder>(this, &AudioCaptureEncoder::onAudioCapture));
		
		setState(EncoderState::Encoding);
	} 
	catch (Exception& exc) {
		cerr << "Encoder Error: " << exc.displayText() << endl;
		setState(EncoderState::Error);
		stop();
		throw exc;
	}
}

void AudioCaptureEncoder::stop() 
{
	cout << "[AudioCaptureEncoder] Stopping..." << endl;
	try {
		assert(_capture);
		assert(_encoder.isInitialized());

		_capture->stop(AudioCallback<AudioCaptureEncoder>(this, &AudioCaptureEncoder::onAudioCapture));

		_encoder.resetEncodeContext();

		setState(EncoderState::Idle);

	} 
	catch (Exception& exc) {
		cerr << "Encoder Error: " << exc.displayText() << endl;
		setState(EncoderState::Error);
	}

	if (_destroyOnStop)
		delete this;
}
	

void AudioCaptureEncoder::onAudioCapture(const AudioPacket& packet) 
{
	cout << "AudioCaptureEncoder::onAudioCapture: " << packet.size << endl;
	try {
		if (!_encoder.isInitialized()) {
			throw Exception("The encoder is not initilaized.");
			return;
		}

		int size = _encoder.encode((unsigned char *)packet.data, packet.size/*, _buffer, MAX_AUDIO_PACKET_SIZE*/);

		/*
		AudioPacket encPacket;
		encPacket.data = (void*)_buffer;
		encPacket.size = size;
		encPacket.time = packet.time;
		PacketEncoded.send(this, encPacket);
		*/
	} 
	catch (Exception& exc) {
		cerr << "Encoder Error: " << exc.displayText() << endl;
		setState(EncoderState::Error);
		stop();
	}
}


} } // namespace Sourcey::Media