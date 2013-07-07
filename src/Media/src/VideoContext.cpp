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


#include "Sourcey/Media/VideoContext.h"
#include "Sourcey/Logger.h"


using namespace std;
using namespace Poco;
using namespace scy;


namespace scy {
namespace av {


VideoContext::VideoContext() :
	stream(NULL),
	codec(NULL),
	frame(NULL),
	pts(0.0)
{
	traceL("VideoContext", this) << "Creating" << endl;
	//reset();
}
	

VideoContext::~VideoContext()
{	
	traceL("VideoContext", this) << "Destroying" << endl;
	
	//assert((!frame && !codec && !stream) && "video context must be closed");
	close();
}


void VideoContext::create()
{
}


void VideoContext::open()
{
	traceL("VideoContext", this) << "Opening" << endl;	
	assert(ctx);
	assert(codec);

	// Open the video codec
	if (avcodec_open2(ctx, codec, NULL) < 0)
   		throw Exception("Cannot open the video codec.");
}


void VideoContext::close()
{
	traceL("VideoContext", this) << "Closing" << endl;

	if (frame) {
		av_free(frame);
		frame = NULL;
	}

	if (ctx) {
		avcodec_close(ctx);
		ctx = NULL;
	}

	// Streams are managed differently by each impl
	//if (stream)	{
		//stream = NULL;
		// NOTE: The stream is managed by the AVFormatContext
		//av_freep(stream);
	//}
	
	pts = 0.0;
	error = "";
	
	traceL("VideoContext", this) << "Closing: OK" << endl;
}


// ---------------------------------------------------------------------
// Video Encoder Context
//
VideoEncoderContext::VideoEncoderContext(AVFormatContext* format) :
	format(format),
	conv(NULL),
	buffer(NULL),
	bufferSize(0)
{
}
	

VideoEncoderContext::~VideoEncoderContext()
{
	close();
}


void VideoEncoderContext::create() //, const VideoCodec& params
{
	debugL("VideoEncoderContext", this) << "Creating: " 
		<< "\n\tInput: " << iparams.toString() 
		<< "\n\tOutput: " << oparams.toString() 
		<< endl;

	VideoContext::create();
	
	// Find the video encoder
	codec = avcodec_find_encoder_by_name(oparams.encoder.data());
	if (!codec) {
		codec = avcodec_find_encoder(format->oformat->video_codec);
		if (!codec)
   			throw Exception("Video encoder not found.");
	}

	format->oformat->video_codec = codec->id;
	
	// Add a video stream that uses the format's default video 
	// codec to the format context's streams[] array.
	stream = avformat_new_stream(format, codec);
	if (!stream)
		throw Exception("Cannot create video stream.");	
	
	ctx = stream->codec;

	InitVideoEncoderContext(ctx, codec, oparams);
	
	// Create the video conversion context if needed
	createConverter();

	// Some formats want stream headers to be separate
	if (format->oformat->flags & AVFMT_GLOBALHEADER)
		ctx->flags |= CODEC_FLAG_GLOBAL_HEADER;

	// Allocate the input frame
	frame = CreateVideoFrame(av_get_pix_fmt(iparams.pixelFmt), iparams.width, iparams.height);
	if (!frame)
		throw Exception("Cannot allocate input frame.");
	
	// Allocate the encode buffer
	// XXX: Disabling in favour of encoder manged buffer
    //bufferSize = avpicture_get_size(ctx->pix_fmt, ctx->width, ctx->height);
    //buffer = (UInt8*)av_malloc(bufferSize);
}


void VideoEncoderContext::createConverter()
{
	if (conv)
		throw Exception("A conversion context already exists.");
		
	// Create the conversion context
	if (iparams.width != oparams.width ||
		iparams.height != oparams.height ||
		strcmp(iparams.pixelFmt, oparams.pixelFmt) != 0) {
		conv = new VideoConversionContext();
		conv->create(iparams, oparams);
	}
}


void VideoEncoderContext::freeConverter()
{
	if (conv) {
		delete conv;
		conv = NULL;
	}

	//if (frame) {
	//	av_free(frame);
	//	frame = NULL;
	//}
}
 

void VideoEncoderContext::close()
{
	debugL("VideoEncoderContext", this) << "Closing" << endl;

	VideoContext::close();
	
	freeConverter();
	
	/*
	if (buffer) {
		av_free(buffer);
		buffer = NULL;
	}
	
	// Free the stream
	if (stream && format && format->nb_streams) {		
		for (unsigned int i = 0; i < format->nb_streams; i++) {
			if (format->streams[i] == stream) {		
				traceL("VideoContext", this) << "Closing: Removing Stream: " << stream << endl;
				av_freep(&format->streams[i]->codec);
				av_freep(&format->streams[i]);
				stream = NULL;
				format->nb_streams--;
			}
		}
	}
	*/
}


bool VideoEncoderContext::encode(unsigned char* data, int size, AVPacket& opacket)
{
	assert(data);
	assert(size);
    AVPacket ipacket;
    av_init_packet(&ipacket);
    ipacket.stream_index = stream->index;
    ipacket.data = data;
    ipacket.size = size;
	return encode(ipacket, opacket);
}


bool VideoEncoderContext::encode(AVPacket& ipacket, AVPacket& opacket)
{	
	assert(ipacket.data);
	assert(stream);
	assert(frame);
	assert(codec);

	frame->data[0] = (UInt8*)ipacket.data;

	// TODO: Correctly set the input frame PTS
	// http://thompsonng.blogspot.com.au/2011/09/ffmpeg-avinterleavedwriteframe-return.html
	// http://stackoverflow.com/questions/6603979/ffmpegavcodec-encode-video-setting-pts-h264
	// (1 / oparams.fps) * sample rate * frame number
	frame->pts = ctx->frame_number;

	return encode(frame, opacket);
}


bool VideoEncoderContext::encode(AVFrame* iframe, AVPacket& opacket)
{	
	assert(frame);
	assert(frame->data[0]);
	assert(codec);

	AVFrame* oframe = conv ? conv->convert(iframe) : iframe;
	oframe->pts = iframe->pts;

    av_init_packet(&opacket);	
    opacket.stream_index = stream->index;
	opacket.data = NULL; // use encoder assigned buffer
	opacket.size = 0;
	//opacket.data = this->buffer; // use our buffer, not ffmpeg assigned
	//opacket.size = this->bufferSize;

	int frameEncoded = 0;
	if (avcodec_encode_video2(ctx, &opacket, oframe, &frameEncoded) < 0) {
		error = "Fatal Encoder Error";
		errorL("VideoEncoderContext", this) << error << endl;
		throw Exception(error);
    }
	
	if (frameEncoded) {
		fps.tick();
		if (ctx->coded_frame->key_frame) 
		    opacket.flags |= AV_PKT_FLAG_KEY; 
        if (opacket.pts != AV_NOPTS_VALUE)
            opacket.pts = av_rescale_q(opacket.pts, ctx->time_base, stream->time_base);
        if (opacket.dts != AV_NOPTS_VALUE)
            opacket.dts = av_rescale_q(opacket.dts, ctx->time_base, stream->time_base);
				
		/*
		traceL() << "[VideoCodecEncoderContext:" << this << "] Encoded PTS:" 
			//<< "\n\tPTS: " << av_ts2str(opacket.pts)
			//<< "\n\tDTS: " << av_ts2str(opacket.dts)
			//<< "\n\tPTS Time: " << av_ts2timestr(opacket.pts, &stream->time_base)
			//<< "\n\tDTS Time: " << av_ts2timestr(opacket.dts, &stream->time_base)
			<< "\n\tPTS: " << opacket.pts
			<< "\n\tDTS: " << opacket.dts
			<< "\n\tFrame PTS: " << oframe->pts
			//<< "\n\tCodec Time Den: " << ctx->time_base.den
			//<< "\n\tCodec Time Num: " << ctx->time_base.num
			//<< "\n\tStream Time Den: " << stream->time_base.den
			//<< "\n\tStream Time Num: " << stream->time_base.num
			<< endl;
			*/
    }
	
	return frameEncoded > 0;
}


// ---------------------------------------------------------------------
// Video Codec Encoder Context
//
VideoCodecEncoderContext::VideoCodecEncoderContext() :
	conv(NULL),
	buffer(NULL),
	bufferSize(0)
{
}
	

VideoCodecEncoderContext::~VideoCodecEncoderContext()
{
	close();
}


void VideoCodecEncoderContext::create() //, const VideoCodec& paramsconst VideoCodec& params
{
	debugL() << "[VideoCodecEncoderContext:" << this << "] Creating: " 
		<< "\n\tInput: " << iparams.toString() 
		<< "\n\tOutput: " << oparams.toString() 
		<< endl;

	VideoContext::create();
		
	avcodec_register_all();

	// Find the video encoder
	codec = avcodec_find_encoder_by_name(oparams.encoder.data());
	if (!codec)
   		throw Exception("Video encoder not found.");

    ctx = avcodec_alloc_context();
    if (!ctx)
        throw Exception("Cannot allocate encoder context.");	

	InitVideoEncoderContext(ctx, codec, oparams);
		
	// Allocate the conversion context
	if (iparams.width != oparams.width ||
		iparams.height != oparams.height ||
		strcmp(iparams.pixelFmt, oparams.pixelFmt) == 0) {
		conv = new VideoConversionContext();
		conv->create(iparams, oparams);
	}

	// Allocate the input frame
	frame = CreateVideoFrame(ctx->pix_fmt, iparams.width, iparams.height);
	if (!frame)
		throw Exception("Cannot allocate input frame.");
	
	// Allocate the encode buffer
	// XXX: Disabling in favour of encoder manged buffer
    //bufferSize = avpicture_get_size(ctx->pix_fmt, ctx->width, ctx->height);
    //buffer = (UInt8*)av_malloc(bufferSize);
}


void VideoCodecEncoderContext::close()
{
	debugL() << "[VideoCodecEncoderContext:" << this << "] Closing" << endl;

	VideoContext::close();
	
	if (conv) {
		delete conv;
		conv = NULL;
	}

	if (buffer) {
		av_free(buffer);
		buffer = NULL;
	}
}


bool VideoCodecEncoderContext::encode(unsigned char* data, int size, AVPacket& opacket)
{
	assert(data);
	assert(size);
    AVPacket ipacket;
    av_init_packet(&ipacket);
    ipacket.data = data;
    ipacket.size = size;
	return encode(ipacket, opacket);
}


bool VideoCodecEncoderContext::encode(AVPacket& ipacket, AVPacket& opacket)
{	
	assert(stream == NULL);
	assert(ipacket.data);
	assert(frame);
	assert(conv);

	frame->data[0] = (UInt8*)ipacket.data;

	// TODO: Correctly set the input frame PTS
	// http://thompsonng.blogspot.com.au/2011/09/ffmpeg-avinterleavedwriteframe-return.html
	// http://stackoverflow.com/questions/6603979/ffmpegavcodec-encode-video-setting-pts-h264
	// (1 / oparams.fps) * sample rate * frame number
	frame->pts = ctx->frame_number;

	return encode(frame, opacket);
}


bool VideoCodecEncoderContext::encode(AVFrame* iframe, AVPacket& opacket)
{	
	traceL() << "[VideoCodecEncoderContext:" << this << "] Encoding Video Packet" << endl;

	AVFrame* oframe = conv ? conv->convert(iframe) : iframe;
	oframe->pts = iframe->pts;

    av_init_packet(&opacket);	
	opacket.data = NULL;
	opacket.size = 0;
	//opacket.data = this->buffer; // use our buffer, not ffmpeg assigned
	//opacket.size = this->bufferSize;

	int frameEncoded = 0;
	if (avcodec_encode_video2(ctx, &opacket, oframe, &frameEncoded) < 0) {
		error = "Fatal Encoder Error";
		errorL() << "[VideoCodecEncoderContext:" << this << "] Fatal Encoder Error" << endl;
		throw Exception(error);
    }

	if (frameEncoded) {		
		fps.tick();
		if (ctx->coded_frame->key_frame) 
		    opacket.flags |= AV_PKT_FLAG_KEY;
		// No stream pointer
		//opacket.stream_index = stream->index;
        //if (opacket.pts != AV_NOPTS_VALUE)
        //    opacket.pts = av_rescale_q(opacket.pts, ctx->time_base, stream->time_base);
        //if (opacket.dts != AV_NOPTS_VALUE)
        //    opacket.dts = av_rescale_q(opacket.dts, ctx->time_base, stream->time_base);

		/*	

		traceL() << "[VideoCodecEncoderContext:" << this << "] Encoded PTS:\n" 
			//<< "\n\tPTS: " << av_ts2str(opacket.pts)
			//<< "\n\tDTS: " << av_ts2str(opacket.dts)
			//<< "\n\tPTS Time: " << av_ts2timestr(opacket.pts, &stream->time_base)
			//<< "\n\tDTS Time: " << av_ts2timestr(opacket.dts, &stream->time_base)
			<< "\n\tPTS: " << opacket.pts
			<< "\n\tDTS: " << opacket.dts
			<< "\n\tFrame PTS: " << oframe->pts
			//<< "\n\tCodec Time Den: " << ctx->time_base.den
			//<< "\n\tCodec Time Num: " << ctx->time_base.num
			//<< "\n\tStream Time Den: " << stream->time_base.den
			//<< "\n\tStream Time Num: " << stream->time_base.num
			<< endl; 
		*/
    }
	
	return frameEncoded > 0;
}


// ---------------------------------------------------------------------
// Video Decoder Context
//
VideoDecoderContext::VideoDecoderContext()
{
}
	

VideoDecoderContext::~VideoDecoderContext()
{
	close();
}


void VideoDecoderContext::create(AVFormatContext *ic, int streamID)
{	
	debugL() << "[VideoDecoderContext:" << this << "] Creating: " << streamID << endl;
	VideoContext::create();

	assert(ic);
	assert(streamID >= 0);
	
	this->stream = ic->streams[streamID];
	this->ctx = this->stream->codec;

	codec = avcodec_find_decoder(this->ctx->codec_id);	
	if (!codec)
		throw Exception("Video codec missing or unsupported.");
	
	this->frame = avcodec_alloc_frame();
	if (this->frame == NULL)
		throw Exception("Could not allocate video input frame.");
}


void VideoDecoderContext::close()
{
	VideoContext::close();
}


bool VideoDecoderContext::decode(UInt8* data, int size, AVPacket& opacket)
{
    AVPacket ipacket;
    av_init_packet(&ipacket);
    ipacket.stream_index = stream->index;
    ipacket.data = data;
    ipacket.size = size;
	return decode(ipacket, opacket);
}


bool VideoDecoderContext::decode(AVPacket& ipacket, AVPacket& opacket)
{
	assert(ipacket.stream_index == stream->index);
	
	int frameDecoded = 0;
	int bytesDecoded = 0;
	int bytesRemaining = ipacket.size;
	
	av_init_packet(&opacket);
	opacket.data = NULL;
	opacket.size = 0;
	
	bytesDecoded = avcodec_decode_video2(ctx, frame, &frameDecoded, &ipacket);
	if (bytesDecoded < 0) {
		error = "Decoder error";
		errorL() << "[VideoDecoderContext:" << this << "] " << error << endl;
		throw Exception(error);
	}

	// XXX: Asserting here to make sure below looping 
	// avcodec_decode_video2 is actually redundant.
	// Otherwise we need to reimplement this pseudo code:
	// while(packet->size > 0)
	// {
	// 	 int ret = avcodec_decode_video2(..., ipacket);
	// 	 if(ret < -1)
	//		throw std::exception("error");
	//
	//	 ipacket->size -= ret;
	//	 ipacket->data += ret;
	// }
	assert(bytesDecoded == bytesRemaining);

	/*
	while (bytesRemaining) { // && !frameDecoded
		//debugL() << "[VideoDecoderContext:" << this << "] Decoding: " << ipacket.pts << endl;
		bytesRemaining -= bytesDecoded;
	}
	*/

	if (frameDecoded) {	
		fps.tick();
		InitDecodedVideoPacket(stream, ctx, frame, &opacket, &pts);
		/*
		traceL() << "[VideoDecoderContext:" << this << "] Decoded Frame:" 		
			<< "\n\tPTS: " << pts	
			<< "\n\tPacket Size: " << opacket.size
			<< "\n\tPacket PTS: " << opacket.pts
			<< "\n\tPacket DTS: " << opacket.dts
			<< "\n\tFrame Packet PTS: " << frame->pkt_pts
			<< "\n\tFrame Packet DTS: " << frame->pkt_dts
			<< "\n\tFrame Size: " << ctx->frame_size
			<< endl;
			*/
		
		return true;
	}
	return false;
}


bool VideoDecoderContext::flush(AVPacket& opacket)
{	
	AVPacket ipacket;
	av_init_packet(&ipacket);
	ipacket.data = NULL;
	ipacket.size = 0;

	av_init_packet(&opacket);
	opacket.data = NULL;
	opacket.size = 0;
	
	int frameDecoded = 0;
	avcodec_decode_video2(ctx, frame, &frameDecoded, &ipacket);
	if (frameDecoded) {
		InitDecodedVideoPacket(stream, ctx, frame, &opacket, &pts);
		debugL() << "[VideoDecoderContext:" << this << "] Flushed Video Frame: " << opacket.pts << endl;
		return true;
	}
	return false;
}


// ---------------------------------------------------------------------
// Video Conversion Context
//
VideoConversionContext::VideoConversionContext() :
	oframe(NULL),
	ctx(NULL)
{
}
	

VideoConversionContext::~VideoConversionContext()
{	
	free();
}


void VideoConversionContext::create(const VideoCodec& iparams, const VideoCodec& oparams)
{
	traceL("VideoConversionContext", this) << "Creating:" 
		<< "\n\tInput Width: " << iparams.width
		<< "\n\tInput Height: " << iparams.height
		<< "\n\tInput Pixel Format: " << iparams.pixelFmt
		<< "\n\tOutput Width: " << oparams.width
		<< "\n\tOutput Height: " << oparams.height
		<< "\n\tOutput Pixel Format: " << oparams.pixelFmt
		<< endl;

    if (ctx)
        throw Exception("Conversion context already initialized.");

	oframe = avcodec_alloc_frame();
	oframe->width = oparams.width;
	oframe->height = oparams.height;
    avpicture_alloc(reinterpret_cast<AVPicture*>(oframe), 
		av_get_pix_fmt(oparams.pixelFmt), oparams.width, oparams.height);
	ctx = sws_getContext(
		iparams.width, iparams.height, av_get_pix_fmt(iparams.pixelFmt),
        oparams.width, oparams.height, av_get_pix_fmt(oparams.pixelFmt), 
		/* SWS_FAST_BILINEAR */SWS_BICUBIC, NULL, NULL, NULL);
    if (!ctx) 
        throw Exception("Invalid conversion context.");

	this->iparams = iparams;
	this->oparams = oparams;
	
	traceL("VideoConversionContext", this) << "Creating: OK" << endl;
}
	

void VideoConversionContext::free()
{
	traceL("VideoConversionContext", this) << "Closing" << endl;

	if (oframe) {
		av_free(oframe);
		oframe = NULL;
	}
	
	if (ctx) {
		sws_freeContext(ctx);
		ctx = NULL;
	}

	traceL("VideoConversionContext", this) << "Closing: OK" << endl;
}


AVFrame* VideoConversionContext::convert(AVFrame* iframe)
{
	assert(iframe);
	assert(iframe->data[0]);

    if (!ctx)
        throw Exception("Conversion context must be initialized.");

	if (sws_scale(ctx,
		iframe->data, iframe->linesize, 0, iparams.height,
		oframe->data, oframe->linesize) < 0)
		throw Exception("Pixel format conversion not supported.");

	return oframe;
}


// ---------------------------------------------------------------------
// Inlines & Helpers
//		
AVFrame* CreateVideoFrame(::PixelFormat pixelFmt, int width, int height)
{
    AVFrame* picture = avcodec_alloc_frame();
    if (!picture)
        return NULL;
	
    int size = avpicture_get_size(pixelFmt, width, height);
    UInt8* buffer = (UInt8*)av_malloc(size);
    if (!buffer) {
        av_free(picture);
        return NULL;
	}
		
	avpicture_fill(reinterpret_cast<AVPicture*>(picture), buffer, pixelFmt, width, height);

    return picture;
}


void InitVideoEncoderContext(AVCodecContext* ctx, AVCodec* codec, VideoCodec& oparams) 
{
	assert(oparams.enabled);	

	avcodec_get_context_defaults3(ctx, codec);
	ctx->codec_id = codec->id;
	ctx->codec_type = AVMEDIA_TYPE_VIDEO;
	ctx->pix_fmt = av_get_pix_fmt(oparams.pixelFmt);
	
	// Resolution must be a multiple of two
	ctx->width = oparams.width;
	ctx->height = oparams.height;
	
	// Define encoding parameters
	ctx->bit_rate = oparams.bitRate;
	//ctx->bit_rate_tolerance = oparams.bitRate * 100;

	ctx->frame_number = 0;

	// Time base: this is the fundamental unit of time (in seconds) in terms
	// of which frame timestamps are represented. For fixed-fps content,
	// timebase should be 1/framerate and timestamp increments should be
	// identically 1.
	ctx->time_base.den = (int)oparams.fps;
	ctx->time_base.num = 1; //000; //; //

	// Emit one intra frame every twelve frames at most
	ctx->gop_size = 12; // oparams.fps;
	
	// Set some defaults for codecs of note.	
	// Also set optimal output pixel formats if the
	// default PIX_FMT_YUV420P was given.
	switch (ctx->codec_id) {
	case CODEC_ID_H264:
		// TODO: Use oparams.quality to determine profile
        av_opt_set(ctx->priv_data, "preset", "veryfast", 0); // slow // baseline
		break;
	case CODEC_ID_MJPEG:
	case CODEC_ID_LJPEG:
		if (ctx->pix_fmt == PIX_FMT_YUV420P)
			ctx->pix_fmt = PIX_FMT_YUVJ420P;
		
		// Use high quality JPEG
		// TODO: Use oparams.quality to determine values		
		ctx->mb_lmin			= ctx->lmin = ctx->qmin * FF_QP2LAMBDA; 
		ctx->mb_lmax			= ctx->lmax = ctx->qmax * FF_QP2LAMBDA; 
		ctx->flags          = CODEC_FLAG_QSCALE; 
		ctx->global_quality = ctx->qmin * FF_QP2LAMBDA; 
		break;
	case CODEC_ID_MPEG2VIDEO:
		ctx->max_b_frames = 2;
		break;		
	case CODEC_ID_MPEG1VIDEO:
	case CODEC_ID_MSMPEG4V3:
		// Needed to avoid using macroblocks in which some codecs overflow
 		// this doesn't happen with normal video, it just happens here as the
 		// motion of the chroma plane doesn't match the luma plane 
 		// avoid FFmpeg warning 'clipping 1 dct coefficients...'
		ctx->mb_decision = 2;
	break;
	case CODEC_ID_JPEGLS:
		// PIX_FMT_BGR24 or GRAY8 depending on if color...
		if (ctx->pix_fmt == PIX_FMT_YUV420P)
			ctx->pix_fmt = PIX_FMT_BGR24;
		break;
	case CODEC_ID_HUFFYUV:
		if (ctx->pix_fmt == PIX_FMT_YUV420P)
			ctx->pix_fmt = PIX_FMT_YUV422P;
		break;
	}
	
	// Update any modified values
	oparams.pixelFmt = av_get_pix_fmt_name(ctx->pix_fmt);
}
    

void InitDecodedVideoPacket(const AVStream* stream, const AVCodecContext* ctx, const AVFrame* frame, AVPacket* opacket, double* pts)
{		
	opacket->data = frame->data[0];
	opacket->size = avpicture_get_size(ctx->pix_fmt, ctx->width, ctx->height);
	opacket->dts = frame->pkt_dts; // Decoder PTS values can be unordered
	opacket->pts = frame->pkt_pts;	
		
	// Local PTS value represented as decimal seconds
    if (opacket->dts != AV_NOPTS_VALUE) {
		*pts = (double)opacket->pts;
		*pts *= av_q2d(stream->time_base);
	}

	assert(opacket->data);
	assert(opacket->size);
	//assert(opacket->dts >= 0);
	//assert(opacket->pts >= 0);	
	
	/*	
	traceL() << "[VideoContext] Init Decoded Frame Pcket:" 		
		<< "\n\tFrame DTS: " << frame->pkt_dts		
		<< "\n\tFrame PTS: " << frame->pkt_pts		
		<< "\n\tPacket Size: " << opacket->size	
		<< "\n\tPacket DTS: " << opacket->dts
		<< "\n\tPacket PTS: " << opacket->pts
		<< endl;

	// ffplay.c
	if (decoder_reorder_pts == -1) {
        *pts = av_frame_get_best_effort_timestamp(frame);
    } else if (decoder_reorder_pts) {
        *pts = frame->pkt_pts;
    } else {
        *pts = frame->pkt_dts;
    }

    if (*pts == AV_NOPTS_VALUE) {
        *pts = 0;
    }
	*/
}


// http://code.mythtv.org/cgit/mythtv/tree/mythtv/libs/libmythtv/avformatwriter.cpp
AVRational GetCodecTimeBase(AVCodec* codec, double fps)
{
    AVRational result;

    result.den = (int)floor(fps * 100);
    result.num = 100;

    if (codec && codec->supported_framerates) {
        const AVRational *p= codec->supported_framerates;
        AVRational req;
		req.den = result.den;
		req.num = result.num;// =
            //(AVRational){result.den, result.num};
        const AVRational *best = NULL;
        //AVRational best_error= (AVRational){INT_MAX, 1};
        AVRational best_error; //= (AVRational){INT_MAX, 1};
		best_error.den = INT_MAX;
		best_error.num = 1;
        for(; p->den!=0; p++) {
            AVRational error = av_sub_q(req, *p);
            if (error.num <0)
                error.num *= -1;
            if (av_cmp_q(error, best_error) < 0) {
                best_error = error;
                best = p;
            }
        }

        if (best && best->num && best->den) {
            result.den = best->num;
            result.num = best->den;
        }
    }

    if (result.den == 2997) {
         result.den = 30000;
         result.num = 1001;
    }
    else if (result.den == 5994) {
         result.den = 60000;
         result.num = 1001;
    }

    return result;
}


	
} } // namespace scy::av

		

	
	/*
	// Skip if exceeding maximum FPS
	if (maxFPS && ipacket.dts != AV_NOPTS_VALUE) {
		double fps = ctx->frame_number / (ipacket.dts * av_q2d(stream->time_base));
		if (fps > maxFPS) {
			traceL() << "[VideoDecoderContext:" << this << "] Skipping video frame at fps: " << fps << endl;
			return false;
		}
		traceL() << "[VideoDecoderContext:" << this << "] Decoding at fps: " << fps << ": " << ctx->frame_number << endl;
	}
	*/


	// Allocate the conversion context
	// TODO: Check if conversion is necessary.
	//conv = new VideoConversionContext();
	//conv->create(iparams, oparams);
	
	

	/*
	assert(oparams.enabled);	

	avcodec_get_context_defaults3(ctx, c);
	ctx->codec_id = c->id; //oc->oformat->video_codec;	
	ctx->codec_type = AVMEDIA_TYPE_VIDEO;
	
	// Resolution must be a multiple of two
	ctx->width = oparams.width;
	ctx->height = oparams.height;
	
	// Define encoding parameters
	ctx->bit_rate = oparams.bitRate; //800000; //700000; //1000000;
	//ctx->bit_rate_tolerance = 1000; //oparams.bitRate * 100;
		
	ctx->frame_number = 0;

	// Time base: this is the fundamental unit of time (in seconds) in terms
	// of which frame timestamps are represented. for fixed-fps content,
	// timebase should be 1/framerate and timestamp increments should be
	// identically 1.
	ctx->time_base.den = oparams.fps;
	ctx->time_base.num = 1;

	// Emit one intra frame every twelve frames at most
	ctx->gop_size = 12; 
	//ctx->gop_size = oparams.fps;

	if (ctx->codec_id == CODEC_ID_MPEG2VIDEO) {
		ctx->max_b_frames = 2;
	}

	if (ctx->codec_id == CODEC_ID_MPEG1VIDEO || 
		ctx->codec_id == CODEC_ID_MSMPEG4V3) {
		// Needed to avoid using macroblocks in which some codecs overflow
 		// this doesn't happen with normal video, it just happens here as the
 		// motion of the chroma plane doesn't match the luma plane 
 		// avoid FFmpeg warning 'clipping 1 dct coefficients...'
		ctx->mb_decision = 2;
	}

	// Set a few optimal pixel formats for lossless codecs of interest.
	//int bitrateScale = 64;
	switch (oc->oformat->video_codec) {
	case CODEC_ID_JPEGLS:
		// PIX_FMT_BGR24 or GRAY8 depending on is_color...
		ctx->pix_fmt = PIX_FMT_BGR24;
		break;
	case CODEC_ID_HUFFYUV:
		ctx->pix_fmt = PIX_FMT_YUV422P;
		break;
	case CODEC_ID_MJPEG:
	case CODEC_ID_LJPEG:
	  ctx->pix_fmt = PIX_FMT_YUVJ420P;
	  //bitrateScale = 128;
	  break;
	case CODEC_ID_RAWVIDEO:
	default:
		// Good for lossy formats, MPEG, etc.
		ctx->pix_fmt = PIX_FMT_YUV420P;
		break;
	}
	
    if (ctx->codec_id == CODEC_ID_H264)
        av_opt_set(ctx->priv_data, "preset", "veryfast", 0); // slow // baseline

	// Open the video codec
	if (avcodec_open2(ctx, codec, NULL) < 0)
   		throw Exception("Cannot open the video codec.");
	*/
	


		/*
		
		// Use the input packet timestamp if the
		// frame PTS wasn't set by the decoder.
		if (frame->pts == AV_NOPTS_VALUE)
			frame->pts = ipacket.dts;
		opacket.pts = frame->pts;
		
		// Local pts value represented as second decimal.
		pts = opacket.pts;
		pts *= av_q2d(stream->time_base);
		
		assert(opacket.data);
		assert(opacket.size);
				opacket.pts = ipacket.dts;
		
		// Local pts value represented as second decimal
		// http://dranger.com/ffmpeg/ffmpegtutorial_all.html
		if (ipacket.dts != AV_NOPTS_VALUE) {
			pts = ipacket.dts;
			pts *= av_q2d(stream->time_base);
		}
		*/
		
		//pts = 0;
		//opacket.pts = ctx->coded_frame->pts != AV_NOPTS_VALUE ? ctx->coded_frame->pts : ipacket.pts;
		//if(packet->dts == AV_NOPTS_VALUE 
		//   && pFrame->opaque && *(uint64_t*)pFrame->opaque != AV_NOPTS_VALUE) {
		//  pts = *(uint64_t *)pFrame->opaque;
		//} else 

		//if (opacket.pts != AV_NOPTS_VALUE) {
		//	pts = opacket.pts;
		//	pts *= av_q2d(stream->time_base);
		//}

		//if (frame->pts != AV_NOPTS_VALUE) {
		//	opacket.pts = frame->pts;
		//	pts = frame->pts;
		//	pts *= av_q2d(stream->time_base);
		//}

		//frame->type = FF_BUFFER_TYPE_USER;
		//frame->pkt_pts = ctx->pkt ? ctx->pkt->pts : AV_NOPTS_VALUE;
		//frame->width = ctx->width;
		//frame->height = ctx->height;
		//frame->format = ctx->pix_fmt;
		
			/*
		traceL() << "[VideoDecoderContext:" << this << "] Decoded Frame:" 			
			<< "\n\tFrame Size: " << opacket.size
			<< "\n\tFrame PTS: " << opacket.pts
			<< "\n\tDecoder PTS: " << pts
			<< "\n\tipacket PTS: " << ipacket.pts
			<< "\n\tframe->width: " << frame->width
			<< "\n\tframe->height: " << frame->height
			<< "\n\tframe->format: " << frame->format
			<< "\n\tframe->pkt_pts: " << frame->pkt_pts
			<< "\n\tctx->width: " << ctx->width
			<< "\n\tctx->height: " << ctx->height
			<< "\n\tctx->format: " << ctx->pix_fmt
			<< "\n\tctx->pkt->pts: " << ctx->pkt //->pts
			<< "\n\tctx->coded_frame->pts: " << ctx->coded_frame->pts
			<< endl;
			*/
	
/*
void VideoEncoderContext::reset() 
{
	VideoContext::reset();	
}
*/
	//if (oframe)
	//	av_free(oframe);
	
  	//if (packet)
  	//	av_free_packet(packet);
  	
  	//if (convCtx)
	//	sws_freeContext(convCtx);
    //if (buffer)
    //    av_free(buffer);
	//packet = NULL;
	//convCtx = NULL;
	//oframe = NULL;
		
	//bufferSize = 0;
	//buffer = NULL;

	// Allocate the output frame
	//oframe = createVideoFrame(ctx->pix_fmt, ctx->width, ctx->height);
	//if (!oframe)
	//	throw Exception("Cannot allocate output frame.");
	
	
	/*
	// Initialize scale conversion context if uninitialized or if the
	// video input size has changed.
	if (convCtx == NULL) {
		convCtx = sws_getContext(
			iparams.width, iparams.height, (::PixelFormat)iparams.pixelFmt, 
			ctx->width, ctx->height, ctx->pix_fmt,
			SWS_BICUBIC, NULL, NULL, NULL);

		traceL("VideoEncoderContext", this) << "Video Conversion Context:\n" 
			<< "\n\tInput Width: " << iparams.width
			<< "\n\tInput Height: " << iparams.height
			<< "\n\tInput Pixel Format: " << iparams.pixelFmt
			<< "\n\tOutput Width: " << ctx->width
			<< "\n\tOutput Height: " << ctx->height
			<< "\n\tOutput Pixel Format: " << ctx->pix_fmt
			<< endl;
	}
	if (sws_scale(convCtx,
		frame->data, frame->linesize, 0, iparams.height,
		oframe->data, oframe->linesize) < 0)
		throw Exception("Pixel format conversion not supported.");
		*/
	/*
	int len = avcodec_encode_video(ctx, this->buffer, this->bufferSize, oframe);
	if (len < 0) {
		error = "Encoder error";
		errorL("VideoEncoderContext", this) << "Encoder Error" << endl;
		return false;
    }
				
	if (ctx->coded_frame->pts != AV_NOPTS_VALUE)
		opacket.pts = av_rescale_q(ctx->coded_frame->pts, ctx->time_base, stream->time_base);

	if (ctx->coded_frame->key_frame)
		opacket.flags |= AV_PKT_FLAG_KEY;

	opacket.data = this->buffer;
	opacket.size = len; //this->bufferSize;
	
	return true;	
	*/
		/*
		if (frame->pts != AV_NOPTS_VALUE) {
			opacket.pts = frame->pts * 1.0;
			opacket.pts *= av_q2d(stream->time_base);
		}
		

		if (frame->dts != AV_NOPTS_VALUE) {
			opacket.dts = frame->dts
			opacket.dts *= av_q2d(stream->time_base);
		}

			opacket.pts = ipacket.pts;
			opacket.dts = ipacket.dts;

		if (ipacket.pts != AV_NOPTS_VALUE) {
			opacket.pts = ipacket.pts;
			opacket.pts *= av_q2d(stream->time_base);
		}	
		if (ipacket.dts != AV_NOPTS_VALUE) {
			opacket.dts = ipacket.dts;
			opacket.dts *= av_q2d(stream->time_base);
		}
		*/
	


/*
void VideoContext::reset()
{
	stream = NULL;
	codec = NULL;
	frame = NULL;

	pts = 0.0;

	error = "";
}
*/
		/*
    if (frameEncoded) {	
		
		if (opacket.pts != AV_NOPTS_VALUE)
			opacket.pts = av_rescale_q(opacket.pts, ctx->time_base, stream->time_base);
		if (opacket.dts != AV_NOPTS_VALUE)
			opacket.dts = av_rescale_q(opacket.dts, ctx->time_base, stream->time_base);		
		
		traceL("VideoEncoderContext", this) << "Encoding Video Packet:\n"
			//<< "\n\AV_NOPTS_VALUE: " << (ctx->coded_frame->pts != AV_NOPTS_VALUE)
			//<< "\n\ctx->coded_frame: " << ctx->coded_frame
			//<< "\n\ctx->coded_frame->pts: " << ctx->coded_frame->pts
			<< "\n\ctx->time_base: " << ctx->time_base.den
			<< "\n\ctx->time_base num: " << ctx->time_base.num
			<< "\n\stream->time_base: " << stream->time_base.den
			<< "\n\stream->time_base num: " << stream->time_base.num
			<< "\n\topacket.pts s: " << av_ts2str(opacket.pts)
			<< "\n\topacket.dts s: " << av_ts2str(opacket.dts)
			<< "\n\topacket.pts ts: " << av_ts2timestr(opacket.pts)
			<< "\n\topacket.dts ts: " << av_ts2timestr(opacket.dts)
			<< "\n\toframe->pts: " << oframe->pts
			<< "\n\topacket.pts: " << opacket.pts
			<< "\n\topacket.dts: " << opacket.dts
			<< endl;
    }
	
	return len;
			*/

		/*
		traceL() << "[VideoDecoderContext:" << this << "] bytesTotal: " << ipacket.size << endl;
		traceL() << "[VideoDecoderContext:" << this << "] frameDecoded: " << opacket.size << endl;
		traceL() << "[VideoDecoderContext:" << this << "] bytesRemaining: " << bytesRemaining << endl;
		traceL() << "[VideoDecoderContext:" << this << "] bytesDecoded: " << bytesDecoded << endl;
		*/

	/*		
	// If frameDecoded == 0, then no frame was produced.
	if (frameDecoded == 0) {
		warnL() << "[VideoDecoderContext:" << this << "] Decoding Video: No frame was decoded." << endl;
		return 0;
	}
	
    ipacket.data += len;	
	ipacket.size -= len;
	*/
	
	/*
	// Initialize scale conversion context if uninitialized or if the
	// video input size has changed.
	if (convCtx == NULL) {
		convCtx = sws_getContext(
			iparams.width, iparams.height, (::PixelFormat)iparams.pixelFmt, 
			ctx->width, ctx->height, ctx->pix_fmt,
			SWS_BICUBIC, NULL, NULL, NULL);

		traceL("VideoEncoderContext", this) << "Video Conversion Context:\n" 
			<< "\n\tInput Width: " << iparams.width
			<< "\n\tInput Height: " << iparams.height
			<< "\n\tInput Pixel Format: " << iparams.pixelFmt
			<< "\n\tOutput Width: " << ctx->width
			<< "\n\tOutput Height: " << ctx->height
			<< "\n\tOutput Pixel Format: " << ctx->pix_fmt
			<< endl;
	}
	if (sws_scale(convCtx,
		frame->data, frame->linesize, 0, iparams.height,
		oframe->data, oframe->linesize) < 0)
		throw Exception("Pixel format conversion not supported.");
		*/
	
	/*
	// http://stackoverflow.com/questions/9854293/encoding-video-only-flv
	ctx->time_base.num = 1; //oparams.fps; //
	ctx->time_base.den = 1000;
	//ctx->time_base= (AVRational){ 1, 1000 };
	
	// Set real frame rate
	stream->r_frame_rate.num = oparams.fps;
	stream->r_frame_rate.den = 1; //1000; //

	// Set average frame rate
	stream->avg_frame_rate.num = oparams.fps;
	stream->avg_frame_rate.den = 1; //1000; //
	*/
	
	//stream->avg_frame_rate  = (AVRational){ oparams.fps, 1 };
	//stream->r_frame_rate	= (AVRational){ oparams.fps, 1 };

	/*
	// Variable frame rate
	//      encoder.createFile(filename,width,height,bitrate,gop,fps);			// Fixed frame rate
   //else
   //   encoder.createFile(filename,width,height,bitrate*1000/fps,gop,1000);	// For variable frame rates: set the time base to e.g. 1ms (1000fps),
																		// and correct the bitrate according to the expected average frame 
	ctx->bit_rate = oparams.bitRate * 1000 / oparams.fps; 
	ctx->bit_rate_tolerance = oparams.bitRate * 100;
	
	ctx->time_base.den = 1000;
	ctx->time_base.num = 1;	

	
        coderSettings.bitRate = 700000;
        coderSettings.codec = ICodec.ID.CODEC_ID_H264;
        coderSettings.globalQuality = 75;
        coderSettings.sampleRate = 22050;
	*/
		//return opacket.size;

		/*
02027             if (pkt.pts == AV_NOPTS_VALUE && !(enc->ctx->capabilities & CODEC_CAP_DELAY))
02028                 pkt.pts = ost->sync_opts;
02029 
02030             if (pkt.pts != AV_NOPTS_VALUE)
02031                 pkt.pts = av_rescale_q(pkt.pts, enc->time_base, ost->st->time_base);
02032             if (pkt.dts != AV_NOPTS_VALUE)
02033                 pkt.dts = av_rescale_q(pkt.dts, enc->time_base, ost->st->time_base);
02034 
02035             if (debug_ts) {
02036                 av_log(NULL, AV_LOG_INFO, "encoder -> type:video "
02037                     "pkt_pts:%s pkt_pts_time:%s pkt_dts:%s pkt_dts_time:%s\n",
02038                     av_ts2str(pkt.pts), av_ts2timestr(pkt.pts, &ost->st->time_base),
02039                     av_ts2str(pkt.dts), av_ts2timestr(pkt.dts, &ost->st->time_base));
02040             }
02041 
02042             write_frame(s, &pkt, ost);
02043             frame_size = pkt.size;
02044             video_size += pkt.size;
02045             av_free_packet(&pkt);
02046 
02047             // if two pass, output log 
02048             if (ost->logfile && enc->stats_out) {
02049                 fprintf(ost->logfile, "%s", enc->stats_out);
02050             }
02051         }
*/
		/*
		//if (pts != AV_NOPTS_VALUE)          // Set custom pts again for subsequent processing
			ctx->coded_frame->pts = pts;  // Set the custom time stamp
			*/
		
		/*

		//if (pts != AV_NOPTS_VALUE)
		//	opacket.pts = pts;
		//else
		//if (ctx->coded_frame->pts != AV_NOPTS_VALUE)
		//	opacket.pts = av_rescale_q(ctx->coded_frame->pts, ctx->time_base, stream->time_base);		
		//opacket.pts = pts;
		//_videoPTS += ctx->time_base.den/(ctx->time_base.num/(/durationInMS/50/1000.0));
		pts += 1000/(1/(/durationInMS/100/1000.0)); //ctx->time_base.den/(ctx->time_base.num/(/durationInMS/100/1000.0));
		opacket.pts = pts;
		opacket.dts = pts;

		if (ctx->coded_frame->key_frame)
			opacket.flags |= AV_PKT_FLAG_KEY;

		opacket.stream_index = stream->index;
		opacket.data = this->buffer;
		opacket.size = len;	
			
		//frameNum++;
		*/


	
	/*
	if (frameNum == 0)
		pts = 0;
	else
		pts += sqrt((double)frameNum);

	//if (pts != AV_NOPTS_VALUE)			// Use custom pts if provided
		oframe->pts = pts;					// Set the custom time stamp
		ctx->coded_frame->pts = pts;		// Set the custom time stamp
		*/

	//oframe->pts = pts += 1; //00;
	//ctx->coded_frame->pts = pts;000

/*
void VideoContext::populate()
{
	assert(codec && stream);
	if (!codec || !stream)
		return;

	bitRate = ctx->bit_rate;
	sampleRate = ctx->sample_rate;
	bitsPerSample = ctx->bits_per_raw_sample;
	if (!bitsPerSample)
		bitsPerSample = ctx->bits_per_coded_sample;
	channels = ctx->channels;
	duration = stream->duration * av_q2d(stream->time_base);
}


double VideoContext::pts()
{
	if (packet && stream) {
		double pts = (packet->dts != AV_NOPTS_VALUE) ? packet->dts : 0.0;
		pts *= av_q2d(stream->time_base);
		return pts;
	}
	return 0.0;
}
*/

	/*
	// Initialize scale conversion context if uninitialized or if the
	// video input size has changed.
	if (convCtx == NULL) {
		convCtx = sws_getContext(
			width, height, (::PixelFormat)iparams.pixelFmt, //static_cast<::PixelFormat>()
			ctx->width, ctx->height, ctx->pix_fmt,
			SWS_BICUBIC, NULL, NULL, NULL);

		traceL() << "[AVEncoder" << this << "] Initializing Video Conversion Context:\n" 
			<< "\n\tInput Width: " << width
			<< "\n\tInput Height: " << height
			<< "\n\tInput Pixel Format: " << iparams.pixelFmt
			<< "\n\tOutput Width: " << ctx->width
			<< "\n\tOutput Height: " << ctx->height
			<< "\n\tOutput Pixel Format: " << ctx->pix_fmt
			<< endl;
	}

	if (sws_scale(convCtx,
		frame->data, frame->linesize, 0, height,
		oframe->data, oframe->linesize) < 0)
		throw Exception("Pixel format conversion not supported");

	if (oc->oformat->flags & AVFMT_RAWPICTURE) {
		AVPacket opacket;
		av_init_opacket(&opacket);
		opacket.flags |= AV_PKT_FLAG_KEY;
		opacket.stream_index = stream->index;
		opacket.data = (UInt8 *)oframe;
		opacket.size = sizeof(AVPicture);

		int result = av_interleaved_write_frame(oc, &opacket);
		if (result < 0) {
			errorL() << "[AVEncoder" << this << "] Cannot write video frame." << endl;
			//throw Exception("Cannot write video frame");
			return false;
		}
	} 
	else {

		// PTS value will increment by 1 for input each frame at defined FPS value.
		// PTS value will need to be dynamically generated for variable FPS rates.
		_fpsCounter.tick();
		if (oframe->pts == AV_NOPTS_VALUE)
			oframe->pts = 0;
		else {
			double fpsDiff = (ctx->time_base.den / _fpsCounter.fps);
			_videoPTS = _videoPTS + fpsDiff;
			oframe->pts = _videoPTS; //++; // = oframe->pts + fpsDiff;
		
	}

	return true;
	*/

	/*
	int len = avcodec_encode_video2(ctx, this->buffer, this->bufferSize, (short*)buffer);
    if (len < 0) {
		error = "Encoder error";
		errorL("VideoEncoderContext", this) << "Decoding Video: Error: " << error << endl;
		return -1;
    }

	//AVPacket opacket;, AVPacket* output
	//av_init_opacket(&opacket);

	if (ctx->coded_frame && 
		ctx->coded_frame->pts != AV_NOPTS_VALUE) 
		opacket.pts = av_rescale_q(ctx->coded_frame->pts, ctx->time_base, stream->time_base);
			
	if (ctx->coded_frame &&
		ctx->coded_frame->key_frame) 
        opacket.flags |= AV_PKT_FLAG_KEY;

	opacket.stream_index = stream->index;
	opacket.data = this->buffer;
	*/

	//this->oframe = avcodec_alloc_frame();	
	//if (this->oframe == NULL)
	//	throw Exception("Could not allocate the output frame");
	
    //this->packet = (AVPacket*)av_mallocz(sizeof(AVPacket));
    //av_init_packet(this->packet);

	/*
	assert(ic);
	assert(streamID >= 0);
	
    stream = ic->streams[streamID];
    codec = stream->codec;

    AVCodec* c = avcodec_find_decoder (ctx->codec_id);
    if (c == NULL)
		throw Exception("The video codec is missing or unsupported");

    if (avcodec_open(ctx, c) < 0)
		throw Exception("Could not open the video codec");

    switch (ctx->sample_fmt) {
    case SAMPLE_FMT_S16:
        width = 16;
        fp = false;
        break;
    case SAMPLE_FMT_S32:
        width = 32;
        fp = false;
        break;
    case SAMPLE_FMT_FLT:
        width = 32;
        fp = true;
        break;
    case SAMPLE_FMT_DBL:
        width = 64;
        fp = true;
        break;
    default:
		throw Exception("Unsupported video sample format");
    }

    bufferSize = MAX_AUDIO_PACKET_SIZE;
    buffer = (UInt8*)av_malloc(bufferSize);
    //packet = (AVPacket*)av_mallocz(sizeof(AVPacket));
    //av_init_packet(packet);
    //offset = 0;
    //return codec;
	*/

	//traceL() << "[VideoDecoderContext:" << this << "] Decoder Size: " << frameDecoded << endl;
	//traceL() << "[VideoDecoderContext:" << this << "] Decoder DTS: " << packet.dts << endl;
	//traceL() << "[VideoDecoderContext:" << this << "] Decoder Time Base: " << stream->time_base.den << endl;


	
	//traceL() << "[VideoDecoderContext:" << this << "] Decoder PTS: " << packet.pts << endl;
	//traceL() << "[VideoDecoderContext:" << this << "] Decoder PTS 1: " << pts << endl;

	/*
	// Initialize scale conversion context if uninitialized or if the
	// video input size has changed.
	if (convCtx == NULL) {
		convCtx = sws_getContext(
			width, height, (::PixelFormat)iparams.pixelFmt, //static_cast<::PixelFormat>()
			ctx->width, ctx->height, ctx->pix_fmt,
			SWS_BICUBIC, NULL, NULL, NULL);

		traceL() << "[AVEncoder" << this << "] Initializing Video Conversion Context:\n" 
			<< "\n\tInput Width: " << width
			<< "\n\tInput Height: " << height
			<< "\n\tInput Pixel Format: " << iparams.pixelFmt
			<< "\n\tOutput Width: " << ctx->width
			<< "\n\tOutput Height: " << ctx->height
			<< "\n\tOutput Pixel Format: " << ctx->pix_fmt
			<< endl;
	}

	if (sws_scale(convCtx,
		frame->data, frame->linesize, 0, height,
		oframe->data, oframe->linesize) < 0)
		throw Exception("Pixel format conversion not supported");

	// Convert the image from its native format to BGR.
	if (convCtx == NULL)
		convCtx = sws_getContext(
			ctx->width, ctx->height, ctx->pix_fmt, 
			ctx->width, ctx->height, PIX_FMT_BGR24, 
			SWS_BICUBIC, NULL, NULL, NULL);
	if (convCtx == NULL)
		throw Exception("Cannot initialize the video conversion context");	
			
	// Scales the data in src according to our settings in our SwsContext.
	sws_scale(
		convCtx, frame->data, frame->linesize, 
		0, ctx->height, oframe->data, oframe->linesize);
		*/


	/*
	
	// H264 defaults
	if (ctx->codec_id == CODEC_ID_H264) {
		ctx->me_range = 5;
		ctx->max_qdiff = 5;
		ctx->qmin = 20;
		ctx->qmax = 30;
		ctx->qcompress = 0.6f;
		ctx->qblur = 0.1f;
		ctx->gop_size = 3;
		ctx->max_b_frames = 1;
		ctx->flags = CODEC_FLAG_LOW_DELAY;
	}

	// libx264-medium.ffpreset preset
	ctx->coder_type = 1;  // coder = 1
	ctx->flags|=CODEC_FLAG_LOOP_FILTER;   // flags=+loop
	ctx->me_cmp|= 1;  // cmp=+chroma, where CHROMA = 1
	ctx->partitions|=X264_PART_I8X8+X264_PART_I4X4+X264_PART_P8X8+X264_PART_B8X8; // partitions=+parti8x8+parti4x4+partp8x8+partb8x8
	ctx->me_method=ME_HEX;    // me_method=hex
	ctx->me_subpel_quality = 7;   // subq=7
	ctx->me_range = 16;   // me_range=16
	ctx->gop_size = 250;  // g=250
	ctx->keyint_min = 25; // keyint_min=25
	ctx->scenechange_threshold = 40;  // sc_threshold=40
	ctx->i_quant_factor = 0.71; // i_qfactor=0.71
	ctx->b_frame_strategy = 1;  // b_strategy=1
	ctx->qcompress = 0.6; // qcomp=0.6
	ctx->qmin = 10;   // qmin=10
	ctx->qmax = 51;   // qmax=51
	ctx->max_qdiff = 4;   // qdiff=4
	ctx->max_b_frames = 3;    // bf=3
	ctx->refs = 3;    // refs=3
	ctx->directpred = 1;  // directpred=1
	ctx->trellis = 1; // trellis=1
	ctx->flags2|=CODEC_FLAG2_BPYRAMID+CODEC_FLAG2_MIXED_REFS+CODEC_FLAG2_WPRED+CODEC_FLAG2_8X8DCT+CODEC_FLAG2_FASTPSKIP;  // flags2=+bpyramid+mixed_refs+wpred+dct8x8+fastpskip
	ctx->weighted_p_pred = 2; // wpredp=2

	// libx264-main.ffpreset preset
	ctx->flags2|=CODEC_FLAG2_8X8DCT;
	//ctx->flags2^=CODEC_FLAG2_8X8DCT;    // flags2=-dct8x8

	// H264 defaults
	if (ctx->codec_id == CODEC_ID_H264) {
		ctx->me_range = 5;
		ctx->max_qdiff = 5;
		ctx->qmin = 20;
		ctx->qmax = 30;
		ctx->qcompress = 0.6f;
		ctx->qblur = 0.1f;
		ctx->gop_size = 3;
		ctx->max_b_frames = 1;
		ctx->flags = CODEC_FLAG_LOW_DELAY;
	}
	*/