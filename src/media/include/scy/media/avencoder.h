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


#ifndef SCY_MEDIA_AVEncoder_H
#define SCY_MEDIA_AVEncoder_H


#include "scy/base.h"

#ifdef HAVE_FFMPEG

#include "scy/packetstream.h"
#include "scy/media/types.h"
#include "scy/media/ffmpeg.h"
#include "scy/media/iencoder.h"
#include "scy/media/videocontext.h"
#include "scy/media/audiocontext.h"
#include "scy/mutex.h"
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
	AVEncoder(const EncoderOptions& options);
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
		
	EncoderOptions& options();
	VideoEncoderContext* video();
	AudioEncoderContext* audio();

	//virtual void* self() { return this;	}
			
	PacketSignal emitter;
	
protected:
	//static Mutex _mutex; // Protects avcodec_open/close()

	EncoderOptions _options;
	AVFormatContext* _formatCtx;
	//clock_t			_startTime;
	AVIOContext*	_ioCtx;
	unsigned char*  _ioBuffer; 
	int				_ioBufferSize; 

	//
 	// Video
	//
	VideoEncoderContext* _video;
	//PTSCalculator* _videoPtsCalc;
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
	//PTSCalculator* _audioPtsCalc;
	//Int64 _audioPts;
	//FPSCounter		_audioFPS;
	//clock_t			_audioTime;	
};


} } // namespace scy::av


#endif
#endif	// SCY_MEDIA_AVEncoder_H