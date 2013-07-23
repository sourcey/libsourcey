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


class AVEncoder: public IPacketEncoder
	/// This class implements an Audio/Video encoder and writer
	/// which  depends on libavcodec/libavformat.
{
public:
	AVEncoder(const RecordingOptions& options);
	AVEncoder();
	virtual ~AVEncoder();

	virtual void initialize();
	virtual void uninitialize();
	virtual void cleanup();

	virtual void process(IPacket& packet);

	virtual void createVideo();
	virtual void freeVideo();
	virtual bool encodeVideo(unsigned char* buffer, int bufferSize, int width, int height);
	
	virtual void createAudio();
	virtual void freeAudio();
	virtual bool encodeAudio(unsigned char* buffer, int bufferSize);
		
	RecordingOptions& options();
	VideoEncoderContext* video();
	AudioEncoderContext* audio();

protected:			
	virtual void onStreamStateChange(const PacketStreamState& state);
	
protected:
	static Mutex _mutex; // Protects avcodec_open/close()

	RecordingOptions	_options;
	AVFormatContext* _formatCtx;
	clock_t			_startTime;
	AVIOContext*	_ioCtx;
	unsigned char*  _ioBuffer; 
	int				_ioBufferSize; 

	//
 	// Video
	//
	VideoEncoderContext* _video;
	//FPSCounter		_videoFPS;
	//clock_t			_videoTime;

	//
 	// Audio
	//
	AudioEncoderContext* _audio;
	AVFifoBuffer*	_audioFifo;		
	UInt8*			_audioBuffer;
	//FPSCounter		_audioFPS;
	//clock_t			_audioTime;	
};


} } // namespace scy::av


#endif	// SOURCEY_MEDIA_AVEncoder_H



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
	if (c = avcodec_find_encoder_by_name(value.data())) 
		goto success;

	// try splitting first word
	//if (c = avcodec_find_encoder_by_name(value.data())) 
	//	goto success;
	
    return "";

success:
	 return c->name;
}
*/
