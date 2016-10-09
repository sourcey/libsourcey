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


#include "scy/av/thumbnailer.h"

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

    reader.emitter += packetDelegate(this, &Thumbnailer::onVideoPacket);
    reader.options().iFramesOnly = true;
    reader.options().disableAudio = true;
    reader.openFile(options.ifile);

    if (options.seek > reader.video()->stream->duration)
        throw std::runtime_error("Seek position exceeds stream duration");

    // TODO: More media types
    if (options.ofile.find(".png") != std::string::npos) {
        encoder.oparams.encoder = "png";
    }

    // Default to JPEG
    else {
        encoder.oparams.encoder = "mjpeg";
        encoder.oparams.pixelFmt = "yuvj420p";
    }

    encoder.iparams.width = reader.video()->ctx->width;
    encoder.iparams.height = reader.video()->ctx->height;
    encoder.iparams.pixelFmt = av_get_pix_fmt_name(reader.video()->ctx->pix_fmt);
    encoder.oparams.width = options.width ? options.width :  reader.video()->ctx->width;
    encoder.oparams.height = options.height ? options.height :  reader.video()->ctx->height;
    encoder.oparams.fps = 20; // avoid FFmpeg 0 fps bitrate tolerance error
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
    DebugL << "Thumbnail packet out: " << packet.size() << std::endl;

    // Skip frames before seek position
    if (reader.video()->pts < options.seek) {
        TraceL << "Skipping thumbnail frame: " << reader.video()->pts << " < " << options.seek << std::endl;
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


} } // namespace scy::av

#endif
