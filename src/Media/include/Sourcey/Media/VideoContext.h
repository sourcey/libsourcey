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
#include <libavutil/pixdesc.h>
#include <libswscale/swscale.h>
}


namespace Sourcey {
namespace Media {


struct VideoConversionContext;

	
inline AVFrame* CreateVideoFrame(::PixelFormat pixelFmt, int width, int height);
inline void InitVideoEncoderContext(AVCodecContext* ctx, AVCodec* codec, VideoCodec& oparams);
inline void InitDecodedVideoPacket(const AVStream* stream, const AVCodecContext* ctx, const AVFrame* frame, AVPacket* opacket, double* pts);
inline void InitVideoCodecFromContext(const AVCodecContext* ctx, VideoCodec& params);
inline AVRational GetCodecTimeBase(AVCodec* c, double fps);


// ---------------------------------------------------------------------
struct VideoContext
	/// Base video context which all encoders and decoders extend
{
	VideoContext();
	virtual ~VideoContext();
		
	virtual void create();
		/// Create the AVCodecContext using default values

	virtual void open();
		/// Open the AVCodecContext

	virtual void close();	
		/// Close the AVCodecContext

	AVStream* stream;		// encoder or decoder stream
	AVCodecContext* ctx;	// encoder or decoder context
	AVCodec* codec;			// encoder or decoder codec
	AVFrame* frame;			// encoded or decoded frame
    double pts;				// pts in decimal seconds
    std::string error;		// error message
};


// ---------------------------------------------------------------------
struct VideoEncoderContext: public VideoContext
{
	VideoEncoderContext(AVFormatContext* format);
	virtual ~VideoEncoderContext();	
	
	virtual void create();
	//virtual void open();
	virtual void close();
	
	virtual bool encode(unsigned char* data, int size, AVPacket& opacket);
	virtual bool encode(AVPacket& ipacket, AVPacket& opacket);
	virtual bool encode(AVFrame* iframe, AVPacket& opacket);
	
	virtual void createConverter();
	virtual void freeConverter();
	
	VideoConversionContext* conv;
	AVFormatContext* format;

    UInt8*	buffer;
    int		bufferSize;

	VideoCodec	iparams;
	VideoCodec	oparams;
};


// ---------------------------------------------------------------------
struct VideoCodecEncoderContext: public VideoContext
{
	VideoCodecEncoderContext();
	virtual ~VideoCodecEncoderContext();	
	
	virtual void create();
	//virtual void open(); //const VideoCodec& params
	virtual void close();
	
	virtual bool encode(unsigned char* data, int size, AVPacket& opacket);
	virtual bool encode(AVPacket& ipacket, AVPacket& opacket);
	virtual bool encode(AVFrame* iframe, AVPacket& opacket);
		
	VideoConversionContext* conv;

    UInt8*			buffer;
    int				bufferSize;

	VideoCodec	iparams;
	VideoCodec	oparams;
};


// ---------------------------------------------------------------------
struct VideoDecoderContext: public VideoContext
{
	VideoDecoderContext();
	virtual ~VideoDecoderContext();
	
	virtual void create(AVFormatContext *ic, int streamID);	
	//virtual void open();
	virtual void close();	
	
	virtual bool decode(UInt8* data, int size, AVPacket& opacket);
	virtual bool decode(AVPacket& ipacket, AVPacket& opacket);
		/// Decodes a the given input packet.
		/// Returns true an output packet was returned, 
		/// false otherwise.
    
	virtual bool flush(AVPacket& opacket);
		/// Flushes buffered frames.
		/// This method should be called after decoding
		/// until false is returned.

	//double maxFPS; 
		// Maximum decoding FPS. 
		// FPS is calculated from ipacket PTS. 
		// Extra frames will be dropped.
};


// ---------------------------------------------------------------------
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


// ---------------------------------------------------------------------
void InitVideoCodecFromContext(const AVCodecContext* ctx, VideoCodec& params)
{
	params.encoder = avcodec_get_name(ctx->codec_id);
	params.pixelFmt = av_get_pix_fmt_name(ctx->pix_fmt);
	params.width = ctx->width;
	params.height = ctx->height;
	params.sampleRate = ctx->sample_rate;
	params.bitRate = ctx->bit_rate;
	params.fps = 
		ctx->time_base.den / 
		ctx->time_base.num;
}



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