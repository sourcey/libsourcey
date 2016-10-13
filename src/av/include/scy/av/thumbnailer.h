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


#ifndef SCY_AV_Thumbnailer_H
#define SCY_AV_Thumbnailer_H


#include "scy/base.h"
#include "scy/av/config.h"
#include "scy/av/avcapture.h"
#include "scy/av/videoencoder.h"

#include <iostream>
#include <string>
#include <list>


namespace scy {
namespace av {


struct ThumbnailerOptions
{
    std::string ifile;
    std::string ofile;
    int width;
    int height;
    double seek;

    ThumbnailerOptions(const std::string& ifile = "", const std::string& ofile = "",
        int width = 0, int height = 0, double seek = 0.0)
        : ifile(ifile), ofile(ofile), width(width), height(height), seek(seek) {};
};


#ifdef HAVE_FFMPEG

struct Thumbnailer
    // Creates video thumbnail using FFmpeg.
    // Note that even if FFmpeg is unavailable we still expose
    // ThumbnailerOptions for the Spot API.
{
    ThumbnailerOptions options;
    av::AVCapture reader;
    av::VideoEncoderContext encoder;

    Thumbnailer(const ThumbnailerOptions& options = ThumbnailerOptions());
    ~Thumbnailer() ;

    void open();
        // Open the input file
        // The encoder context may still be configured after this call
        // If the ofile path is empty a default one will be selected
        // using defaultThumbPath()

    void grab();
        // Initialize the image encoder and grab a thumbnail at the
        // specified seek position

    void onVideoPacket(void*, av::VideoPacket& packet);

    static std::string defaultThumbPath(const std::string& ifile, const std::string& ext = ".jpg", const std::string& suffix = "_thumb");
};

#endif


} // namespace av
} // namespace scy


#endif
