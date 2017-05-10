///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup av
/// @{


#ifndef SCY_AV_ImageEncoder_H
#define SCY_AV_ImageEncoder_H


#include "scy/av/iencoder.h"
#include "scy/av/types.h"

#ifdef HAVE_OPENCV

#include <opencv/cv.h>
#include <opencv/highgui.h>


namespace scy {
namespace av {


/// This class is a PacketStreamAdapter which uses OpenCV     /// to encode
/// images from raw data packets.
///
/// See OpenCV documentation for cvParams information.
/// JPEG:    ///    0 = CV_IMWRITE_JPEG_QUALITY
///    1 = (quality) default 95 [0-100]
///
/// PNG:    ///    0 = CV_IMWRITE_PNG_COMPRESSION
///    1 = (compression) default 3 [0-9]
//
class ImageEncoder : public PacketProcessor // IPacketEncoder
{
public:
    ImageEncoder(EncoderOptions& options,
                 std::vector<int> cvParams = std::vector<int>());
    virtual ~ImageEncoder();

    virtual void initialize();
    virtual void uninitialize();

    virtual bool accepts(IPacket* packet);
    virtual void process(IPacket& packet);

    virtual EncoderOptions& options();

    PacketSignal emitter;

private:
    std::string _extension;
    EncoderOptions _options;
    std::vector<int> _params;
};


} // namespace av
} // namespace scy


#endif
#endif // SCY_AV_ImageEncoder_H


/// @\}
