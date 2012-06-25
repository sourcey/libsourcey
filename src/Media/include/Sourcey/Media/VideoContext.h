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


#ifndef SOURCEY_MEDIA_VideoContext_H
#define SOURCEY_MEDIA_VideoContext_H


#include "Sourcey/Media/Types.h"
#include "Sourcey/Media/Format.h"

//#include "Poco/Mutex.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/fifo.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
}


namespace Sourcey {
namespace Media {


struct VideoConversionContext;

	
inline AVFrame* CreateVideoFrame(::PixelFormat pixfmt, int width, int height);
inline AVRational GetCodecTimeBase(AVCodec* c, double fps);


struct VideoContext
{
	VideoContext();
	virtual ~VideoContext();
		
	virtual void open();
	virtual void close();	

	AVStream* stream;
	AVCodecContext* codec;
	AVFrame* frame;
    double pts;	
    std::string error;
};


// ---------------------------------------------------------------------
//
struct VideoEncoderContext: public VideoContext
{
	VideoEncoderContext();
	virtual ~VideoEncoderContext();	
	
	virtual void open(AVFormatContext* oc); //, const VideoCodec& params
	virtual void close();

	
	virtual bool encode(unsigned char* data, int size, AVPacket& opacket);
	virtual bool encode(AVPacket& ipacket, AVPacket& opacket);
		
	VideoConversionContext* conv;

    UInt8*			buffer;
    int				bufferSize;

	VideoCodec	iparams;
	VideoCodec	oparams;
};


// ---------------------------------------------------------------------
//
struct VideoDecoderContext: public VideoContext
{
	VideoDecoderContext();
	virtual ~VideoDecoderContext();
	
	virtual void open(AVFormatContext *ic, int streamID);
	virtual void close();
	
	
	
	virtual bool decode(UInt8* data, int size, AVPacket& opacket);
	virtual bool decode(AVPacket& ipacket, AVPacket& opacket);
		/// Decodes a frame from the given packet into the output packet.
		/// Returns true if a frame was decoded, false otherwise.
};


// ---------------------------------------------------------------------
//
struct VideoConversionContext
{
	VideoConversionContext();
	virtual ~VideoConversionContext();	
	
	virtual void create(const VideoCodec& iparams, const VideoCodec& oparams);
	virtual void free();

	virtual AVFrame* convert(AVFrame* iframe);

	AVFrame* oframe;
	struct SwsContext* ctx;
	VideoCodec iparams;
	VideoCodec oparams;
};


} } // namespace Sourcey::Media


#endif	// SOURCEY_MEDIA_VideoContext_H


	//virtual void reset();
	//virtual void reset();	
	
	//AVFrame* oframe;
	//struct SwsContext* convCtx;
/*
void VideoDecoderContext::reset() 
{
	VideoContext::reset();
}
*/
	//virtual void reset();	



	//virtual int encode(unsigned char* buffer, int bufferSize, AVPacket& opacket/*, unsigned pts = AV_NOPTS_VALUE*/);
		/// Encodes a video frame from given data buffer and
		/// stores it in the opacket.
		/// If a pts value is given it will be applied to the
		/// encoded video packet.