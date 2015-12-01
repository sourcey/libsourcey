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


#ifndef SCY_MEDIA_MatrixConverter_H
#define SCY_MEDIA_MatrixConverter_H

#include "scy/packetstream.h"
#include "scy/signal.h"

#if defined(HAVE_OPENCV) && defined(HAVE_FFMPEG)

extern "C" {
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}

#include <opencv/cv.h>


namespace scy { 
namespace av {


class MatrixConverter: public IPacketizer
	/// This class provides the ability to convert decoded
	/// video stream frames to OpenCV matrix images.
	/// Input packets must pass a VideoDecoderContext pointer
	/// as client data.
{
public:
	MatrixConverter() : 
		_convCtx(nullptr),
		_oframe(nullptr)
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
		return dynamic_cast<const av::VideoPacket*>(&packet) != 0; 
	}

	virtual void process(IPacket& packet)
	{
		VideoPacket& vpacket = reinterpret_cast<VideoPacket&>(packet);
		VideoDecoderContext* video = reinterpret_cast<VideoDecoderContext*>(packet.opaque);	
		if (video == nullptr)
			throw std::runtime_error("Matrix Converter: Video packets must contain a VideoDecoderContext pointer.");	
		
		// Create and allocate the conversion frame.
		if (_oframe == nullptr) {
			_oframe = av_frame_alloc();	
			if (_oframe == nullptr)
				throw std::runtime_error("Matrix Converter: Could not allocate the output frame.");

			avpicture_alloc(reinterpret_cast<AVPicture*>(_oframe), 
				AV_PIX_FMT_BGR24, video->ctx->width, video->ctx->height);
		}
	
		// Convert the image from its native format to BGR.
		if (_convCtx == nullptr) {
			_convCtx = sws_getContext(
				video->ctx->width, video->ctx->height, video->ctx->pix_fmt, 
				video->ctx->width, video->ctx->height, AV_PIX_FMT_BGR24, 
				SWS_BICUBIC, nullptr, nullptr, nullptr);
			_mat.create(video->ctx->height, video->ctx->width, CV_8UC(3));
		}
		if (_convCtx == nullptr)
			throw std::runtime_error("Matrix Converter: Unable to initialize the conversion context.");	
			
		// Scales the source data according to our SwsContext settings.
		if (sws_scale(_convCtx,
			video->frame->data, video->frame->linesize, 0, video->ctx->height,
			_oframe->data, _oframe->linesize) < 0)
			throw std::runtime_error("Matrix Converter: Pixel format conversion not supported.");

		// Populate the OpenCV Matrix.
		for (int y = 0; y < video->ctx->height; y++) {
			for (int x = 0; x < video->ctx->width; x++) {
				_mat.at<cv::Vec3b>(y,x)[0] = _oframe->data[0][y * _oframe->linesize[0] + x * 3 + 0];
				_mat.at<cv::Vec3b>(y,x)[1] = _oframe->data[0][y * _oframe->linesize[0] + x * 3 + 1];
				_mat.at<cv::Vec3b>(y,x)[2] = _oframe->data[0][y * _oframe->linesize[0] + x * 3 + 2];
			}
		}

		vpacket.mat = &_mat;
		emit(this, vpacket);
	}

	cv::Mat _mat;
	AVFrame* _oframe;
	struct SwsContext* _convCtx;
};


} } // namespace scy::av


#endif
#endif
