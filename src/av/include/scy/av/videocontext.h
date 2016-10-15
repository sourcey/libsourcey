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


#ifndef SCY_AV_VideoContext_H
#define SCY_AV_VideoContext_H


#include "scy/base.h"

#ifdef HAVE_FFMPEG

#include "scy/av/types.h"
#include "scy/av/format.h"
#include "scy/av/ffmpeg.h"
#include "scy/av/fpscounter.h"
#include "scy/av/videoconverter.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/fifo.h>
#include <libavutil/opt.h>
#include <libavutil/pixdesc.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
}


namespace scy {
namespace av {


struct VideoContext
    /// Base video context from which all video encoders and decoders derive.
{
    VideoContext();
    virtual ~VideoContext();

    virtual void create();
        // Create the AVCodecContext using default values

    virtual void open();
        // Open the AVCodecContext

    virtual void close();
        // Close the AVCodecContext

    virtual AVFrame* convert(AVFrame* iframe);
        // Convert the video frame and return the result

    virtual void createConverter();
    virtual void freeConverter();

    VideoCodec iparams;      // input parameters
    VideoCodec oparams;      // output parameters
    AVStream* stream;        // encoder or decoder stream
    AVCodecContext* ctx;     // encoder or decoder context
    AVCodec* codec;          // encoder or decoder codec
    AVFrame* frame;          // encoder or decoder frame
    VideoConversionContext* conv; // video conversion context
    FPSCounter fps;          // encoder or decoder fps rate
    double pts;              // pts in decimal seconds
    std::string error;       // error message
};


//
// Inline helpers
//


AVFrame* createVideoFrame(AVPixelFormat pixelFmt, int width, int height);
void initVideoCodecFromContext(const AVCodecContext* ctx, VideoCodec& params);
void printAvailableEncoders(std::ostream& ost, const char* delim = " ");


} } // namespace scy::av


#endif
#endif // SCY_AV_VideoContext_H
