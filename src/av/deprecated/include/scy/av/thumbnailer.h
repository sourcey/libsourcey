///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup av
/// @{


#ifndef SCY_AV_Thumbnailer_H
#define SCY_AV_Thumbnailer_H


#if 0 // needs updating to latest API

#include "scy/av/config.h"
#include "scy/av/mediacapture.h"
#include "scy/av/videoencoder.h"
#include "scy/base.h"

#include <iostream>
#include <list>
#include <string>


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


    /// Creates video thumbnail using FFmpeg.
    /// Note that even if FFmpeg is unavailable we still expose
    /// ThumbnailerOptions for the Spot API.
struct Thumbnailer
{
    ThumbnailerOptions options;
    av::MediaCapture reader;
    av::VideoEncoder encoder;

    Thumbnailer(const ThumbnailerOptions& options = ThumbnailerOptions());
    ~Thumbnailer() ;    /// Open the input file
    /// The encoder context may still be configured after this call
    /// If the ofile path is empty a default one will be selected
    /// using defaultThumbPath()
    void open();

    /// Initialize the image encoder and grab a thumbnail at the
    /// specified seek position
    void grab();


    void onVideoPacket(void*, av::VideoPacket& packet);

    static std::string defaultThumbPath(const std::string& ifile, const std::string& ext = ".jpg", const std::string& suffix = "_thumb");
};

#endif


} // namespace av
} // namespace scy


#endif
#endif
