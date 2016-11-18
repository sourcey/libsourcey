///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup av
/// @{


#include "scy/av/videocapture.h"
#include "scy/util.h"

#ifdef HAVE_FFMPEG

#include "scy/av/devicemanager.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}


using std::endl;


namespace scy {
namespace av {


VideoCapture::VideoCapture()
{
}


VideoCapture::VideoCapture(const std::string& device, int width, int height,
                           double framerate)
{
    open(device, width, height, framerate);
}


VideoCapture::~VideoCapture()
{
}


void VideoCapture::open(const std::string& device, int width, int height,
                        double framerate)
{
    TraceS(this) << "Opening camera: " << device << endl;

    DeviceManager devman;
    auto iformat = devman.findVideoInputFormat();
    if (!iformat)
        throw std::runtime_error("Couldn't find camera input format.");

    AVDictionary* iparams = nullptr;
    if (width > 0 && height > 0)
        av_dict_set(&iparams, "video_size",
                    util::format("%dx%d", width, height).c_str(), 0);
    if (framerate > 0)
        av_dict_set(&iparams, "framerate",
                    util::format("%f", framerate).c_str(), 0);

    // Set the desired pixel format
    av_dict_set(&iparams, "pixel_format", "yuv420p", 0);

    openStream(device.c_str(), iformat, &iparams);

    av_dict_free(&iparams); // FIXME: possible memory leak
}


} // namespace av
} // namespace scy


#endif


/// @\}
