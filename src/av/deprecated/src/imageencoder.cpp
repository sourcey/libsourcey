///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup av
/// @{


#include "scy/av/imageencoder.h"
#include "scy/av/mediacapture.h"
#include "scy/logger.h"

#include "assert.h"

#ifdef HAVE_OPENCV


using std::endl;


namespace scy {
namespace av {


ImageEncoder::ImageEncoder(EncoderOptions& options, std::vector<int> cvParams)
    : PacketProcessor(this->emitter)
    , _options(options)
    , _params(cvParams)
{
    TraceS(this) << "Create" << endl;

    if (_options.oformat.id == "jpeg" || _options.oformat.id == "mjpeg")
        _extension = ".jpg";
    else if (_options.oformat.id == "png" || _options.oformat.id == "mpng")
        _extension = ".png";
    else
        // TODO: support more!
        assert(false);
}


ImageEncoder::~ImageEncoder()
{
    TraceS(this) << "Destroy" << endl;
}


void ImageEncoder::initialize()
{
}


void ImageEncoder::uninitialize()
{
}


bool ImageEncoder::accepts(IPacket& packet)
{
    return dynamic_cast<VideoPacket*>(&packet) != 0;
}


void ImageEncoder::process(IPacket& packet)
{
    // TraceS(this) << "Processing" << endl;

    MatrixPacket* mpacket = dynamic_cast<MatrixPacket*>(&packet);
    if (!mpacket)
        throw std::runtime_error("Unknown video packet type.");
    if (!mpacket->mat)
        throw std::runtime_error("Video packets must contain an OpenCV image.");

    std::vector<unsigned char> buffer;

    // FIXME: If the video capture is stopped before
    // this callback completes our Mat is corrupted.
    cv::Mat& source = *mpacket->mat;
    if (source.cols != _options.oformat.video.width &&
        source.rows != _options.oformat.video.height) {
        cv::Mat resized;
        cv::resize(source, resized, cv::Size(_options.oformat.video.width,
                                             _options.oformat.video.height));
        cv::imencode(_extension, resized, buffer, _params);
    } else
        cv::imencode(_extension, source, buffer, _params);

    // Temp reference on the stack only
    mpacket->_data = (char*)&buffer[0];
    mpacket->_size = buffer.size();

    // TraceS(this) << "Broadcasting: " << mpacket << endl;
    emit(*mpacket); // this,
    // TraceS(this) << "Broadcasting: OK: " << mpacket << endl;
}


EncoderOptions& ImageEncoder::options()
{
    return _options;
}


} // namespace av
} // namespace scy


#endif