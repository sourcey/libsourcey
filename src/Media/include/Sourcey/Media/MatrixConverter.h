#ifndef SOURCEY_MEDIA_MatrixConverter_H
#define SOURCEY_MEDIA_MatrixConverter_H


#include "Sourcey/IPacketizer.h"
#include "Sourcey/Signal.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}


namespace Sourcey { 
namespace Media {


class MatrixConverter: public IPacketizer
	/// This class provides the ability to convert decoded
	/// video stream frames to OpenCV matrix images.
	/// Input packets must pass a VideoDecoderContext pointer
	/// as client data.
{
public:
	MatrixConverter() : 
		_convCtx(NULL),
		_oframe(NULL)
	{
	}

	~MatrixConverter()
	{
		if (_convCtx)
			sws_freeContext(_convCtx);

		if (_oframe) {
			if (_oframe->data)
				av_free(_oframe->data[0]);
			av_free(_oframe);
		}
	}
	
	virtual bool accepts(IPacket& packet) 
	{ 
		return dynamic_cast<const Media::VideoPacket*>(&packet) != 0; 
	}

	virtual void process(IPacket& packet)
	{
		VideoPacket& vpacket = reinterpret_cast<VideoPacket&>(packet);
		VideoDecoderContext* video = reinterpret_cast<VideoDecoderContext*>(packet.opaque);	
		if (video == NULL)
			throw Exception("Matrix Converter: Video packets must contain a VideoDecoderContext pointer.");	
		
		// Create and allocate the conversion frame.
		if (_oframe == NULL) {
			_oframe = avcodec_alloc_frame();	
			if (_oframe == NULL)
				throw Exception("Matrix Converter: Could not allocate the output frame.");

			avpicture_alloc(reinterpret_cast<AVPicture*>(_oframe), 
				PIX_FMT_BGR24, video->codec->width, video->codec->height);
		}
	
		// Convert the image from its native format to BGR.
		if (_convCtx == NULL) {
			_convCtx = sws_getContext(
				video->codec->width, video->codec->height, video->codec->pix_fmt, 
				video->codec->width, video->codec->height, PIX_FMT_BGR24, 
				SWS_BICUBIC, NULL, NULL, NULL);
			_mat.create(video->codec->height, video->codec->width, CV_8UC(3));
		}
		if (_convCtx == NULL)
			throw Exception("Matrix Converter: Unable to initialize the conversion context.");	
			
		// Scales the source data according to our SwsContext settings.
		if (sws_scale(_convCtx,
			video->frame->data, video->frame->linesize, 0, video->codec->height,
			_oframe->data, _oframe->linesize) < 0)
			throw Exception("Matrix Converter: Pixel format conversion not supported.");

		// Populate the OpenCV Matrix.
		for (int y = 0; y < video->codec->height; y++) {
			for (int x = 0; x < video->codec->width; x++) {
				_mat.at<cv::Vec3b>(y,x)[0] = _oframe->data[0][y * _oframe->linesize[0] + x * 3 + 0];
				_mat.at<cv::Vec3b>(y,x)[1] = _oframe->data[0][y * _oframe->linesize[0] + x * 3 + 1];
				_mat.at<cv::Vec3b>(y,x)[2] = _oframe->data[0][y * _oframe->linesize[0] + x * 3 + 2];
			}
		}

		vpacket.mat = &_mat;
		dispatch(this, vpacket);
	}

	cv::Mat _mat;
	AVFrame* _oframe;
	struct SwsContext* _convCtx;
};


} } // namespace Sourcey::Media


#endif
