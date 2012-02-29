//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2005 Sourcey
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


#include "Sourcey/Media/VideoContext.h"
#include "Sourcey/Logger.h"


using namespace std;
using namespace Poco;
using namespace Sourcey;

namespace Sourcey {
namespace Media {


VideoContext::VideoContext()
{
	reset();
}
	

VideoContext::~VideoContext()
{	
	close();
}


void VideoContext::open()
{
}
	

void VideoContext::close()
{
	if (iframe)
		av_free(iframe);
	
	if (oframe)
		av_free(oframe);
	
  	//if (packet)
  	//	av_free_packet(packet);
  	
  	if (convertCtx)
		sws_freeContext(convertCtx);

	if (codec)
		avcodec_close(codec);

    if (buffer)
        av_free(buffer);
}


void VideoContext::reset()
{
	stream = NULL;
	codec = NULL;
	//packet = NULL;
	convertCtx = NULL;
	iframe = NULL;
	oframe = NULL;
		
	bufferSize = 0;
	buffer = NULL;

	pts = 0.0;

	error = "";
}

		
AVFrame* VideoContext::createVideoFrame(::PixelFormat pixfmt, int width, int height)
{
    AVFrame* picture;
    UInt8* buffer;
    int size;
    
    picture = avcodec_alloc_frame();
    if (!picture)
        return 0;
	
    size = avpicture_get_size(pixfmt, width, height);
    buffer = (UInt8*)av_malloc(size);
    if (!buffer) {
        av_free(picture);
        return 0;
	}
		
	avpicture_fill((AVPicture *)picture, buffer, pixfmt, width, height);

    return picture;
}

/*
void VideoContext::populate()
{
	assert(codec && stream);
	if (!codec || !stream)
		return;

	bitRate = codec->bit_rate;
	sampleRate = codec->sample_rate;
	bitsPerSample = codec->bits_per_raw_sample;
	if (!bitsPerSample)
		bitsPerSample = codec->bits_per_coded_sample;
	channels = codec->channels;
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



// ---------------------------------------------------------------------
//
// Video Encoder Context
//
// ---------------------------------------------------------------------
VideoEncoderContext::VideoEncoderContext()
{
}
	

VideoEncoderContext::~VideoEncoderContext()
{
}


void VideoEncoderContext::open(AVFormatContext *oc) //, const VideoCodec& params
{
	Log("debug") << "[VideoEncoderContext" << this << "] Opening Video" << endl;

	VideoContext::open();

	assert(oparams.enabled);

	// Add a video stream that uses the format's default video 
	// codec to the format context's streams[] array.
	stream = av_new_stream(oc, 0);
	if (!stream)
		throw Exception("Unable to initialize the video stream");	

	//codec->codec_id = av_guess_codec(oc->oformat, NULL, oc->filename, NULL, CODEC_TYPE_VIDEO);
	codec = stream->codec;
	codec->codec_id = oc->oformat->video_codec;	
	codec->codec_type = AVMEDIA_TYPE_VIDEO;

	// Put sample parameters
	codec->bit_rate = oparams.bitRate; 
	codec->bit_rate_tolerance = oparams.bitRate * 100;
	//mpa_vidctx->bit_rate = usebitrate;
   // mpa_vidctx->bit_rate_tolerance = usebitrate * 100;
	//codec->bit_rate         = 400000;      // TODO: BITRATE SETTINGS!
	//codec->bit_rate = 1000 * bitrateScale;
	//codec->bit_rate = oparams.width * oparams.height * bitrateScale; 

	// Resolution must be a multiple of two
	codec->width = oparams.width;
	codec->height = oparams.height;

	// time base: this is the fundamental unit of time (in seconds) in terms
	// of which frame timestamps are represented. for fixed-fps content,
	// timebase should be 1/framerate and timestamp increments should be
	// identically 1.
	codec->time_base.num = 1;
	codec->time_base.den = oparams.fps;

	// Emit one intra frame every twelve frames at most
	//codec->gop_size = 12; 
	codec->gop_size = oparams.fps;

	if (codec->codec_id == CODEC_ID_MPEG2VIDEO) {
		codec->max_b_frames = 2;
	}

	if (codec->codec_id == CODEC_ID_MPEG1VIDEO || 
		codec->codec_id == CODEC_ID_MSMPEG4V3) {
		// Needed to avoid using macroblocks in which some coecs overflow
 		// this doesn't happen with normal video, it just happens here as the
 		// motion of the chroma plane doesn't match the luma plane 
 		// avoid Ffmpeg warning 'clipping 1 dct coefficients...'
		codec->mb_decision = 2;
	}

	// Set a few optimal pixel formats for lossless codecs of interest.
	//int bitrateScale = 64;
	switch (oc->oformat->video_codec) {
	case CODEC_ID_JPEGLS:
		// PixelFormat::BGR24 or GRAY8 depending on is_color...
		codec->pix_fmt = PIX_FMT_BGR24;
		break;
	case CODEC_ID_HUFFYUV:
		codec->pix_fmt = PIX_FMT_YUV422P;
		break;
	case CODEC_ID_MJPEG:
	case CODEC_ID_LJPEG:
	  codec->pix_fmt = PIX_FMT_YUVJ420P;
	  //bitrateScale = 128;
	  break;
	case CODEC_ID_RAWVIDEO:
	default:
		// Good for lossy formats, MPEG, etc.
		codec->pix_fmt = PIX_FMT_YUV420P;
		break;
	}

	/*
	// libx264-medium.ffpreset preset
	codec->coder_type = 1;  // coder = 1
	codec->flags|=CODEC_FLAG_LOOP_FILTER;   // flags=+loop
	codec->me_cmp|= 1;  // cmp=+chroma, where CHROMA = 1
	codec->partitions|=X264_PART_I8X8+X264_PART_I4X4+X264_PART_P8X8+X264_PART_B8X8; // partitions=+parti8x8+parti4x4+partp8x8+partb8x8
	codec->me_method=ME_HEX;    // me_method=hex
	codec->me_subpel_quality = 7;   // subq=7
	codec->me_range = 16;   // me_range=16
	codec->gop_size = 250;  // g=250
	codec->keyint_min = 25; // keyint_min=25
	codec->scenechange_threshold = 40;  // sc_threshold=40
	codec->i_quant_factor = 0.71; // i_qfactor=0.71
	codec->b_frame_strategy = 1;  // b_strategy=1
	codec->qcompress = 0.6; // qcomp=0.6
	codec->qmin = 10;   // qmin=10
	codec->qmax = 51;   // qmax=51
	codec->max_qdiff = 4;   // qdiff=4
	codec->max_b_frames = 3;    // bf=3
	codec->refs = 3;    // refs=3
	codec->directpred = 1;  // directpred=1
	codec->trellis = 1; // trellis=1
	codec->flags2|=CODEC_FLAG2_BPYRAMID+CODEC_FLAG2_MIXED_REFS+CODEC_FLAG2_WPRED+CODEC_FLAG2_8X8DCT+CODEC_FLAG2_FASTPSKIP;  // flags2=+bpyramid+mixed_refs+wpred+dct8x8+fastpskip
	codec->weighted_p_pred = 2; // wpredp=2

	// libx264-main.ffpreset preset
	codec->flags2|=CODEC_FLAG2_8X8DCT;
	//codec->flags2^=CODEC_FLAG2_8X8DCT;    // flags2=-dct8x8
	*/

	// H264 defaults
	if (codec->codec_id == CODEC_ID_H264) {
		codec->me_range = 5;
		codec->max_qdiff = 5;
		codec->qmin = 20;
		codec->qmax = 30;
		codec->qcompress = 0.6f;
		codec->qblur = 0.1f;
		codec->gop_size = 3;
		codec->max_b_frames = 1;
		codec->flags = CODEC_FLAG_LOW_DELAY;
	}

	// Some formats want stream headers to be separate
	if (oc->oformat->flags & AVFMT_GLOBALHEADER)
		codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
	
	AVCodec* c = avcodec_find_encoder(codec->codec_id);
	if (!c)
   		throw Exception("Video codec not found");

	// Open the video codec
	if (avcodec_open(codec, c) < 0)
   		throw Exception("Unable to open the video codec");

	// Allocate the input frame
	//iframe = createVideoFrame(static_cast<::PixelFormat>(iparams.pixfmt), 
	iframe = createVideoFrame((::PixelFormat)iparams.pixfmt, iparams.width, iparams.height);
	if (!iframe)
		throw Exception("Unable to allocate input frame");

	// Allocate the output frame
	oframe = createVideoFrame(codec->pix_fmt, codec->width, codec->height);
	if (!oframe)
		throw Exception("Unable to allocate output frame");	
		
	// Initialize the video buffer
	//_videoBufferSize = codec->width * codec->height * 3;
	//_videoBuffer = new unsigned char[_videoBufferSize];

    this->bufferSize = codec->width * codec->height * 3; //MAX_AUDIO_PACKET_SIZE;
    this->buffer = (UInt8*)av_malloc(this->bufferSize);		
    //this->opacket = (AVPacket*)av_mallocz(sizeof(AVPacket));
   // av_init_packet(this->opacket);
}


void VideoEncoderContext::close()
{
	Log("debug") << "[VideoEncoderContext" << this << "] Closing Video" << endl;

	VideoContext::close();	
	
	/*
  	if (opacket)
  		av_free_packet(opacket);

	if (stream) {
		avcodec_close(codec);
		stream = NULL;
	}
    if (iframe) {
		//if (iframe->data)
		//	av_free(iframe->data[0]);
		av_free(iframe);
		iframe = NULL;
	}
    if (oframe) {
		if (oframe->data)
			av_free(oframe->data[0]);
        av_free(oframe);
		oframe = NULL;
    }
	if (_videoBuffer) {
		delete _videoBuffer;
		_videoBuffer = NULL;
	}
	if (convCtx) {
		sws_freeContext(convCtx);
		convCtx = NULL;
	}
	*/
}

	
void VideoEncoderContext::reset() 
{
	VideoContext::reset();

	//opacket = NULL;
}


int VideoEncoderContext::encode(unsigned char* buffer, int bufferSize, AVPacket& opacket)
{	
	Log("trace") << "[AVEncoder" << this << "] Encoding Video Packet" << endl;

	assert(iframe);
	assert(oframe);

	//if (iparams.width != width || 
	//	iparams.height != height)
	//	throw Exception("The input video frame is the wrong size");

	//if (!buffer || !bufferSize || !width || !height)
	//	throw Exception("Failed to encode video frame");

	//AVStream* stream = stream;
	//AVCodecContext* codec = stream->codec;

	// This is a hack to speed the things up a bit. We just assign
	// the pointer buffer to iframe->data[0] without using a memcpy.
	iframe->data[0] = (UInt8*)buffer;
	//memcpy(iframe->data[0], buffer, bufferSize);	
	
	// Initialize scale conversion context if uninitialized or if the
	// video input size has changed.
	if (convertCtx == NULL) {
		convertCtx = sws_getContext(
			iparams.width, iparams.height, (::PixelFormat)iparams.pixfmt, 
			codec->width, codec->height, codec->pix_fmt,
			SWS_BICUBIC, NULL, NULL, NULL);

		Log("trace") << "[VideoEncoderContext" << this << "] Video Conversion Context:\n" 
			<< "\n\tInput Width: " << iparams.width
			<< "\n\tInput Height: " << iparams.height
			<< "\n\tInput Pixel Format: " << iparams.pixfmt
			<< "\n\tOutput Width: " << codec->width
			<< "\n\tOutput Height: " << codec->height
			<< "\n\tOutput Pixel Format: " << codec->pix_fmt
			<< endl;
	}
	if (sws_scale(convertCtx,
		iframe->data, iframe->linesize, 0, iparams.height,
		oframe->data, oframe->linesize) < 0)
		throw Exception("Pixel format conversion not supported");

	int len = avcodec_encode_video(codec, this->buffer, this->bufferSize, oframe);
    if (len > 0) {

		av_init_packet(&opacket);
	
		if (codec->coded_frame &&
			codec->coded_frame->pts != AV_NOPTS_VALUE)
			opacket.pts = av_rescale_q(codec->coded_frame->pts, codec->time_base, stream->time_base);

		if (codec->coded_frame->key_frame)
			opacket.flags |= AV_PKT_FLAG_KEY;

		opacket.stream_index = stream->index;
		opacket.data = this->buffer;
		opacket.size = len;	
    }
	else 
		Log("warn") << "[VideoEncoderContext:" << this << "] Encoded frame is NULL" << endl;
	
	return len;

	/*
	// Initialize scale conversion context if uninitialized or if the
	// video input size has changed.
	if (convCtx == NULL) {
		convCtx = sws_getContext(
			width, height, (::PixelFormat)iparams.pixfmt, //static_cast<::PixelFormat>()
			codec->width, codec->height, codec->pix_fmt,
			SWS_BICUBIC, NULL, NULL, NULL);

		Log("trace") << "[AVEncoder" << this << "] Initializing Video Conversion Context:\n" 
			<< "\n\tInput Width: " << width
			<< "\n\tInput Height: " << height
			<< "\n\tInput Pixel Format: " << iparams.pixfmt
			<< "\n\tOutput Width: " << codec->width
			<< "\n\tOutput Height: " << codec->height
			<< "\n\tOutput Pixel Format: " << codec->pix_fmt
			<< endl;
	}

	if (sws_scale(convCtx,
		iframe->data, iframe->linesize, 0, height,
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
			Log("error") << "[AVEncoder" << this << "] Failed to write video frame." << endl;
			//throw Exception("Failed to write video frame");
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
			double fpsDiff = (codec->time_base.den / _fpsCounter.fps);
			_videoPTS = _videoPTS + fpsDiff;
			oframe->pts = _videoPTS; //++; // = oframe->pts + fpsDiff;
		
	}

	return true;
	*/

	/*
	int len = avcodec_encode_video(codec, this->buffer, this->bufferSize, (short*)buffer);
    if (len < 0) {
		error = "Encoder error";
		Log("error") << "[VideoEncoderContext:" << this << "] Decoding Video: Error: " << error << endl;
		return -1;
    }

	//AVPacket opacket;, AVPacket* output
	//av_init_opacket(&opacket);

	if (codec->coded_frame && 
		codec->coded_frame->pts != AV_NOPTS_VALUE) 
		opacket.pts = av_rescale_q(codec->coded_frame->pts, codec->time_base, stream->time_base);
			
	if (codec->coded_frame &&
		codec->coded_frame->key_frame) 
        opacket.flags |= AV_PKT_FLAG_KEY;

	opacket.stream_index = stream->index;
	opacket.data = this->buffer;
	*/
}

// ---------------------------------------------------------------------
//
// Video Decoder Context
//
// ---------------------------------------------------------------------
VideoDecoderContext::VideoDecoderContext()
{
}
	

VideoDecoderContext::~VideoDecoderContext()
{
}

void VideoDecoderContext::open(AVFormatContext *ic, int streamID)
{
	VideoContext::open();
	
	Log("info") << "[AVFileReader:" << this << "] Opening Video: " << streamID << endl;

	assert(ic);
	assert(streamID >= 0);
	
	this->stream = ic->streams[streamID];
	this->codec = this->stream->codec;
	AVCodec* c = avcodec_find_decoder(this->codec->codec_id);	
	if (c == NULL)
		throw Exception("The video codec is missing or unsupported");
	
	if (avcodec_open(this->codec, c) < 0)
		throw Exception("Could not open the video codec");
	
	this->iframe = avcodec_alloc_frame();
	if (this->iframe == NULL)
		throw Exception("Could not allocate the input frame");

	//this->oframe = avcodec_alloc_frame();	
	//if (this->oframe == NULL)
	//	throw Exception("Could not allocate the output frame");
	
	this->bufferSize = avpicture_get_size(PIX_FMT_BGR24, this->codec->width, this->codec->height);
	this->buffer = (uint8_t*)av_malloc(this->bufferSize * sizeof(uint8_t));	
	avpicture_fill((AVPicture*)this->oframe, this->buffer, PIX_FMT_BGR24, this->codec->width, this->codec->height);
	
    //this->packet = (AVPacket*)av_mallocz(sizeof(AVPacket));
    //av_init_packet(this->packet);

    //av_init_packet(&this->packet);

	// Allocate the OpenCV video matrix
	//_cvMatrix.create(this->codec->height, this->codec->width, CV_8UC(3));

	/*
	assert(ic);
	assert(streamID >= 0);
	
    stream = ic->streams[streamID];
    codec = stream->codec;

    AVCodec* c = avcodec_find_decoder (codec->codec_id);
    if (c == NULL)
		throw Exception("The video codec is missing or unsupported");

    if (avcodec_open(codec, c) < 0)
		throw Exception("Could not open the video codec");

    switch (codec->sample_fmt) {
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
    buffer = (uint8_t*)av_malloc(bufferSize);
    //packet = (AVPacket*)av_mallocz(sizeof(AVPacket));
    //av_init_packet(packet);
    //offset = 0;
    //return codec;
	*/
}


void VideoDecoderContext::close()
{
	VideoContext::close();
}


	
void VideoDecoderContext::reset() 
{
	VideoContext::reset();
	
	duration = 0.0;	
	width = -1;
	fp = false;
}


int VideoDecoderContext::decode(AVPacket& packet)
{
	assert(packet.stream_index == stream->index);

	int frameDecoded = -1;
	int len = avcodec_decode_video2(codec, iframe, &frameDecoded, &packet);
	if (len < 0) {
		error = "Decoder error";
		Log("error") << "[AVFileReader:" << this << "] Decoding Video: Error: " << error << endl;
		return -1;
    }
		
	// If frameDecoded == 0, then no frame was produced.
	if (frameDecoded == 0) {
		Log("warn") << "[AVFileReader:" << this << "] Decoding Video: No frame was encoded." << endl;
		return 0;
	}
		
	if (packet.dts != AV_NOPTS_VALUE) {
		pts = packet.dts;
		pts *= av_q2d(stream->time_base);
	}

	/*
	// Initialize scale conversion context if uninitialized or if the
	// video input size has changed.
	if (convCtx == NULL) {
		convCtx = sws_getContext(
			width, height, (::PixelFormat)iparams.pixfmt, //static_cast<::PixelFormat>()
			codec->width, codec->height, codec->pix_fmt,
			SWS_BICUBIC, NULL, NULL, NULL);

		Log("trace") << "[AVEncoder" << this << "] Initializing Video Conversion Context:\n" 
			<< "\n\tInput Width: " << width
			<< "\n\tInput Height: " << height
			<< "\n\tInput Pixel Format: " << iparams.pixfmt
			<< "\n\tOutput Width: " << codec->width
			<< "\n\tOutput Height: " << codec->height
			<< "\n\tOutput Pixel Format: " << codec->pix_fmt
			<< endl;
	}

	if (sws_scale(convCtx,
		iframe->data, iframe->linesize, 0, height,
		oframe->data, oframe->linesize) < 0)
		throw Exception("Pixel format conversion not supported");

	// Convert the image from its native format to BGR.
	if (convertCtx == NULL)
		convertCtx = sws_getContext(
			codec->width, codec->height, codec->pix_fmt, 
			codec->width, codec->height, PIX_FMT_BGR24, 
			SWS_BICUBIC, NULL, NULL, NULL);
	if (convertCtx == NULL)
		throw Exception("Cannot initialize the video conversion context");	
			
	// Scales the data in src according to our settings in our SwsContext.
	sws_scale(
		convertCtx, iframe->data, iframe->linesize, 
		0, codec->height, oframe->data, oframe->linesize);
		*/

	return len;
}

	
} } // namespace Sourcey::Media