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


#ifndef SOURCEY_MEDIA_AVEncoder_H
#define SOURCEY_MEDIA_AVEncoder_H


#include "Sourcey/PacketStream.h"
#include "Sourcey/Media/Types.h"
#include "Sourcey/Media/FFmpeg.h"
#include "Sourcey/Media/IEncoder.h"
#include "Sourcey/Media/VideoContext.h"
#include "Sourcey/Media/AudioContext.h"

#include "Sourcey/Mutex.h"
#include <fstream>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/fifo.h>
#include <libswscale/swscale.h>
}


namespace scy {
namespace av {


class AVEncoder: public IEncoder
	/// This class implements an multiplex audio/video   
	/// encoder which depends on libavcodec/libavformat.
{
public:
	AVEncoder(const RecordingOptions& options);
	AVEncoder();
	virtual ~AVEncoder();

	virtual void initialize();
	virtual void uninitialize();
	virtual void cleanup();

	virtual void createVideo();
	virtual void freeVideo();
	virtual bool encodeVideo(unsigned char* buffer, int bufferSize, int width, int height, UInt64 time = 0);
	
	virtual void createAudio();
	virtual void freeAudio();
	virtual bool encodeAudio(unsigned char* buffer, int bufferSize, UInt64 time = 0);
		
	RecordingOptions& options();
	VideoEncoderContext* video();
	AudioEncoderContext* audio();

	//virtual void* self() { return this;	}
			
	PacketSignal emitter;
	
protected:
	//static Mutex _mutex; // Protects avcodec_open/close()

	RecordingOptions _options;
	AVFormatContext* _formatCtx;
	//clock_t			_startTime;
	AVIOContext*	_ioCtx;
	unsigned char*  _ioBuffer; 
	int				_ioBufferSize; 

	//
 	// Video
	//
	VideoEncoderContext* _video;
	//LivePTSCalculator* _videoPtsCalc;
	//bool _realtime;
	//Int64 _videoPts;
	//Int64 _lastVideoPTS;
	//UInt64 _lastVideoTime;
	double _videoPtsRemainder;
	//FPSCounter		_videoFPS;
	//clock_t			_videoTime;

	//
 	// Audio
	//
	AudioEncoderContext* _audio;
	AVFifoBuffer*	_audioFifo;		
	UInt8*			_audioBuffer;
	//LivePTSCalculator* _audioPtsCalc;
	//Int64 _audioPts;
	//FPSCounter		_audioFPS;
	//clock_t			_audioTime;	
};


} } // namespace scy::av


#endif	// SOURCEY_MEDIA_AVEncoder_H



/*
//
// Live PTS Calculator
//


struct LivePTSCalculator
	/// Helper class which calculates PTS values for a live source
{
	AVRational timeBase;
	clock_t frameTime;
	double frameDuration;
	double frameDiff;

	Int64 currentPTS;
	Int64 lastPTS;

	LivePTSCalculator()
	{
		reset();
	}

	void reset()
	{		
		lastPTS = 0;
		currentPTS = 0;
		frameTime = 0;
		frameDuration = 0;
		frameDiff = 0;
	}
	
	void log()
	{			
		Timestamp ts;
		debugL("LivePTSCalculator", this) << "Values:" 
			<< "\n\tCurrent PTS: " << currentPTS
			<< "\n\tLast PTS: " << lastPTS	
			<< "\n\tFrame Duration: " << frameDuration
			<< "\n\tFrame Diff: " << frameDiff
			<< "\n\tFrame Time: " << frameTime
			<< "\n\tTime Base: " << timeBase.den << ": " << timeBase.num
			<< std::endl;
	}

	Int64 tick()
	{
		// Initializing
		if (frameTime == 0) {
			assert(!frameDuration);
			frameTime = clock();
			currentPTS = 1;
		}

		// Updating
		else {
			frameDuration = (double)(clock() - frameTime) / CLOCKS_PER_SEC;
			frameTime = clock();
			frameDiff = timeBase.den/(timeBase.num/(frameDuration));
			currentPTS = lastPTS + frameDiff;
		}	

		log();

		assert(currentPTS > lastPTS);
		lastPTS = currentPTS;
		return currentPTS;
	}
};
*/


	//UInt32			_frameDuration;

 	// The following variables allow for dynamically
	// calculated video presentation timestamps (PTS).
	//Int64			_videoPTS;


/*
	//Int64			_videoLastPTS;
	//clock_t			_videoLastTime;
inline std::string GetEncoderFromCodecName(const std::string& name) 
	/// Attempts to get the FFmpeg encoder from a codec name.
{	
	AVCodec* c;
	string value = name;

	// make lowercase
	std::transform(value.begin(), value.end(), value.begin(), ::tolower);
	
	// try it
	if (c = avcodec_find_encoder_by_name(value.c_str())) 
		goto success;

	// try splitting first word
	//if (c = avcodec_find_encoder_by_name(value.c_str())) 
	//	goto success;
	
    return "";

success:
	 return c->name;
}
*/
