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


#include "scy/media/thumbnailer.h"


using std::endl;


namespace scy {
namespace av {

			
Thumbnailer::Thumbnailer() : seek(0.0) 
{
	initializeFFmpeg();
}

Thumbnailer::~Thumbnailer() 
{
	uninitializeFFmpeg();
}

void Thumbnailer::init(const std::string& ifile, const std::string& ofile, int owidth, int oheight, double seek) 
{	
	this->ifile = ifile;
	this->ofile = ofile;
	if (seek) this->seek = seek;

	reader += packetDelegate(this, &Thumbnailer::onVideoPacket);
	reader.options().iFramesOnly = true;
	reader.options().disableAudio = true;
	reader.openFile(ifile);

	if (seek > reader.video()->stream->duration)
		throw std::runtime_error("Seek position exceeds stream duration");
							
	// TODO: More media types
	if (ofile.find(".png") != std::string::npos) {
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
	encoder.oparams.width = owidth ? owidth :  reader.video()->ctx->width;
	encoder.oparams.height = oheight ? oheight :  reader.video()->ctx->height;
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
}
		
void Thumbnailer::onVideoPacket(void*, av::VideoPacket& packet)
{				
	DebugL << "Thumbnail packet out: " << packet.size() << std::endl;
			
	// Skip frames before seek position
	if (reader.video()->pts < seek) {
		TraceL << "Skipping thumbnail frame: " << reader.video()->pts << " < " << seek << std::endl;
		return;
	}
		
	// Feed the decoded frame into the encoder
	AVPacket opacket;
	if (encoder.encode(reader.video()->frame, opacket)) {
			
		// Save the thumbnail
		saveFile(ofile, (const char*)opacket.data, (std::streamsize)opacket.size);

		// Break out of the reader scope
		reader.stop();
	}
}
	
void Thumbnailer::saveFile(const std::string& path, const char* data, int size)
{			
	std::ofstream ofs(path, std::ios_base::binary | std::ios_base::out);
	if (ofs.is_open())
		ofs.write(data, size);
	else
		throw std::runtime_error("Cannot save image: " + path);		
}


} } // namespace scy::av