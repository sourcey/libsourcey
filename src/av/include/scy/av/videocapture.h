///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:    LGPL-2.1+
//
/// @addtogroup av
/// @{

#ifndef SCY_AV_VideoCapture_H
#define SCY_AV_VideoCapture_H


#include "scy/base.h"

#ifdef HAVE_FFMPEG

#include "scy/av/mediacapture.h"


namespace scy {
namespace av {

/// This class implements a cross platform audio capturer.
class SCY_EXTERN VideoCapture : public MediaCapture
{
public:
    typedef std::shared_ptr<VideoCapture> Ptr;

    VideoCapture();
    VideoCapture(const std::string& device, int width = -1, int height = -1,
                 double framerate = -1);
    virtual ~VideoCapture();

    virtual void open(const std::string& device, int width = -1,
                      int height = -1, double framerate = -1);
};


} // namespace av
} // namespace scy


#endif
#endif // SCY_AV_VideoCapture_H


/// @\}
