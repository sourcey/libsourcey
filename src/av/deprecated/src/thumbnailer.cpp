///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup av
/// @{


#include "scy/av/thumbnailer.h"

#if 0 // needs updating to latest API
#ifdef HAVE_FFMPEG

#include "scy/filesystem.h"

using std::endl;


namespace scy {
namespace av {


Thumbnailer::Thumbnailer(const ThumbnailerOptions& options) :
    options(options)
{
    initializeFFmpeg();
}


Thumbnailer::~Thumbnailer()
{
    uninitializeFFmpeg();
}


void Thumbnailer::open() //, int owidth, int oheight, double seek
{
    if (!fs::exists(options.ifile)) {
        throw std::runtime_error("Cannot create thumbnail: The input file doesn't exist.");
    }

    if (options.ofile.empty()) {
        options.ofile = defaultThumbPath(options.ifile);
    }

    reader.emitter += packetSlot(this, &Thumbnailer::onVideoPacket);
    reader.openFile(options.ifile);

    if (options.seek > reader.video()->stream->duration)
        throw std::runtime_error("Seek position exceeds stream duration");

    if (options.ofile.find(".png") != std::string::npos) {
        encoder.oparams.encoder = "png";
    }
    else if (options.ofile.find(".jpg") != std::string::npos) {
        encoder.oparams.encoder = "mjpeg";
        encoder.oparams.pixelFmt = "yuvj420p";
    }
    else {
        throw std::runtime_error("Unsupportted image format: " + options.ofile);
    }

    // reader.getEncoderFormat(encoder.iparams);
    auto ctx = reader.video()->ctx;
    encoder.iparams.width = ctx->width;
    encoder.iparams.height = ctx->height;
    encoder.iparams.pixelFmt = av_get_pix_fmt_name(ctx->pix_fmt);
    encoder.oparams.width = options.width ? options.width : ctx->width;
    encoder.oparams.height = options.height ? options.height : ctx->height;
    encoder.oparams.fps = 1;
    encoder.oparams.enabled = true;
    encoder.create();

    //encoder.oparams.print(cout);
}


void Thumbnailer::grab()
{
    // Open here so settings may be modified
    encoder.open();

    // Run the decoder loop
    reader.run();
    assert(fs::exists(options.ofile));
}


void Thumbnailer::onVideoPacket(void*, av::VideoPacket& packet)
{
    LDebug("Thumbnail packet out: ", packet.size())

    // Skip frames before seek position
    if (reader.video()->pts < options.seek) {
        LTrace("Skipping thumbnail frame: ", reader.video()->pts, " < ", options.seek)
        return;
    }

    if (!packet.iframe) {
        LTrace("Skipping non-iframe")
        return;
    }

    // Feed the decoded frame into the encoder
    AVPacket opacket;
    if (encoder.encode(reader.video()->frame, opacket)) {

        // Save the thumbnail
        fs::savefile(options.ofile, (const char*)opacket.data, (std::streamsize)opacket.size);

        // Break out of the reader scope
        reader.stop();
    }
}


std::string Thumbnailer::defaultThumbPath(const std::string& ifile, const std::string& ext, const std::string& suffix)
{
    if (ifile.find(suffix + ext) != std::string::npos)
        return ifile;
    std::string thumbpath(ifile);
    thumbpath = thumbpath.substr(0, thumbpath.length() - ext.length());
    thumbpath += suffix;
    thumbpath += ext;
    return thumbpath;
}


} // namespace av
} // namespace scy

#endif
#endif


/// @\}
