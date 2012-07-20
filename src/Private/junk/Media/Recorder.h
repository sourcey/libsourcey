#ifndef RECORDER_H
#define RECORDER_H


#include "Sourcey/Media/Types.h"
#include "Sourcey/Media/VideoCapture.h"
#include "Sourcey/Media/AudioCapture.h"
#include "Sourcey/Media/StreamEncoder.h"

#include "Sourcey/Base/Stateful.h"

#include "Sourcey/Base/Signal.h"

#include "Poco/DateTimeFormatter.h"
#include "Poco/Timestamp.h"

#include "assert.h"


namespace Sourcey {
namespace Media {


class AVWriter;


struct RecorderState: StateT
{
	enum Type
	{
		Idle,
		Recording,
		Stopped,
		Error
	};
	virtual std::string str(unsigned int id) const 
	{ 	
		switch(id) {
		case RecorderState::Idle: return "Idle";
		case RecorderState::Recording: return "Recording";
		case RecorderState::Error: return "Error";
		}
		return "undefined"; 
	};
};


class Recorder: public Stateful<RecorderState>
{
public:

	/*
	struct Params
	{
		Params(
			const std::string& filename = "",
			time_t stopAt = time_t(0),
			const VideoParams& video = VideoParams(),
			const AudioParams& audio = AudioParams()) :
				filename(filename),
				stopAt(stopAt),
				video(video),
				audio(audio) {}
		Params(
			const std::string& filename,
			time_t stopAt,
			const VideoParams& video) :
				filename(filename),
				stopAt(stopAt),
				video(video) {}
		Params(
			const std::string& filename,
			time_t stopAt,
			const AudioParams& audio) :
				filename(filename),
				stopAt(stopAt),
				audio(audio) {}
		VideoParams video;
		AudioParams audio;
		std::string filename;
		time_t stopAt;
	};
	*/

	Recorder(
		const EncoderParams& params, 
		Media::VideoCapture* video = NULL, 
		Media::AudioCapture* audio = NULL, 
		bool destroyOnStop = false);
	~Recorder();

	void start();
	void stop();
	void extend(time_t stopAt);

	EncoderParams& params() { return _params; };
	bool isRecording() const;

	void setVideoCapture(Media::VideoCapture* video);
	void setAudioCapture(Media::AudioCapture* audio);

	void onVideoCapture(const VideoPacket& packet);
	void onAudioCapture(const AudioPacket& packet);

private:
	StreamEncoder*			_encoder;
	AVWriter*			_writer;
	VideoCapture*		_video;
	AudioCapture*		_audio;
	clock_t				_videoTime;
	clock_t				_audioTime;
	EncoderParams				_params;
	bool				_destroyOnStop;
};


} } // namespace Sourcey::Media


#endif // RECORDER_H