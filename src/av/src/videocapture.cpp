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


#include "scy/av/videocapture.h"

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


VideoCapture::VideoCapture(const std::string& device, int width, int height, double framerate)
{
    open(device, width, height, framerate);
}


VideoCapture::~VideoCapture()
{
}


void VideoCapture::open(const std::string& device, int width, int height, double framerate)
{
    TraceS(this) << "Opening camera: " << device << endl;

    auto iformat = DeviceManager::instance().findCameraInputFormat();
    if (!iformat)
        throw std::runtime_error("Couldn't find camera input format.");

    AVDictionary* iparams = nullptr;
    if (width > 0 && height > 0)
        av_dict_set(&iparams, "video_size", util::format("%dx%d", width, height).c_str(), 0);
    if (framerate > 0)
        av_dict_set(&iparams, "framerate", util::format("%f", framerate).c_str(), 0);

    // Set the desired pixel format
    av_dict_set(&iparams, "pixel_format", "yuv420p", 0);

    openStream(device.c_str(), iformat, &iparams);

    av_dict_free(&iparams); // FIXME: possible memory leak
}


} } // namespace scy::av


#endif
