#ifndef AUDIO_CAPTURE_ENCODER_H
#define AUDIO_CAPTURE_ENCODER_H


#include "Sourcey/Media/Types.h"
#include "Sourcey/Media/AudioCapture.h"
#include "Sourcey/Media/AudioEncoder.h"
#include "Sourcey/Base/Encoder.h"
#include "Sourcey/Base/Stateful.h"

#include "Poco/DateTimeFormatter.h"
#include "Poco/Timestamp.h"


namespace Sourcey {
namespace Media {


class AudioCaptureEncoder: public AudioEncoder
{
public:

	AudioCaptureEncoder(AudioCapture* capture, const AudioParams& params, bool destroyOnStop = false);
	virtual ~AudioCaptureEncoder();

	virtual void start();
	virtual void stop();
	
	void onAudioCapture(const AudioPacket& packet);	

	// Connect to PacketEncoded signal to receive encoded packets.

private:
	AudioParams		_params;
	AudioEncoder	_encoder; 
	AudioCapture*	_capture;
	//unsigned char*	_buffer;
	bool			_destroyOnStop;
};


} } // namespace Sourcey::Media


#endif // AUDIO_CAPTURE_ENCODER_H