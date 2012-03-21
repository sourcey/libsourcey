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


#ifndef SOURCEY_MEDIA_VideoEncoder_H
#define SOURCEY_MEDIA_VideoEncoder_H


#include "Sourcey/Media/Types.h"
#include "Sourcey/Media/FPSCounter.h"
#include "Sourcey/Media/IEncoder.h"

#include "Poco/Mutex.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/fifo.h>
#include <libswscale/swscale.h>
}


namespace Sourcey {
namespace Media {


class VideoEncoder: public IPacketEncoder 
	/// This class implements a video encoder which 
	/// depends on libavcodec/libavformat.
	///
	/// NOTE: Depreciated in favor of AVEncoder.
{
public:		
	VideoEncoder(const EncoderParams& params = EncoderParams()); //const VideoCodec& oCodec, const VideoCodec& iCodec
	virtual ~VideoEncoder();

	virtual void initialize();
	virtual void uninitialize();
	virtual void cleanup();

	virtual bool isReady();
	
	virtual bool accepts(IPacket& packet);
	virtual void process(IPacket& packet);

    virtual int encode(unsigned char *input, int inputSize/*, unsigned char *output, unsigned int outputSize*/);
	
    virtual EncoderParams& params() { return _params; }	
    virtual AVCodecContext* getEncoderContext() const { return _encoderContext; }
    virtual double getTimeBase() const { return _params.oformat.video.fps; }
    virtual double getNow() const { return _now; }	

    static AVFrame* MakeBlackFrame(PixelFormat::ID pixfmt, int width, int height);
    static void FreeFrame(AVFrame* f);
	
protected:
	virtual void initEncodeContext(); //const VideoCodec& oCodec
    virtual void resetEncodeContext();

    virtual void initScaleContext(); //const VideoCodec& iCodec
    virtual void resetScaleContext();
	
private:
	static Poco::FastMutex _mutex;	// Mutex locker to protect avcodec_open/close().
	FPSCounter		_counter;
	//VideoCodec		_iCodec;
	//VideoCodec		_oCodec;
	EncoderParams	_params;
    AVFrame*		_iFrame;
    AVFrame*		_oFrame;
	unsigned char*	_outBuffer;
    AVCodecContext*	_encoderContext;
    SwsContext*		_resizeContext;
    double			_now;
};


} } // namespace Sourcey::Media


#endif	// SOURCEY_MEDIA_VideoEncoder_H

