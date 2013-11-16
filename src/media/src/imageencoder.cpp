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


#include "scy/media/imageencoder.h"
#include "scy/media/videocapture.h"
#include "scy/logger.h"

#include "assert.h"

#ifdef HAVE_OPENCV


using std::endl;


namespace scy {
namespace av {


ImageEncoder::ImageEncoder(EncoderOptions& options, std::vector<int> cvParams) : 
	PacketProcessor(this->emitter),
	_options(options),
	_params(cvParams)
{	
	traceL("ImageEncoder", this) << "Create" << endl;

	if (_options.oformat.id == "jpeg" ||
		_options.oformat.id == "mjpeg")
		_extension = ".jpg";
	else if (
		_options.oformat.id == "png" ||
		_options.oformat.id == "mpng")
		_extension = ".png";
	else 
		// TODO: support more!
		assert(false);
}


ImageEncoder::~ImageEncoder()
{
	traceL("ImageEncoder", this) << "Destroy" << endl;
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
	//traceL("ImageEncoder", this) << "Processing" << endl;

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
		cv::resize(source, resized, cv::Size(_options.oformat.video.width, _options.oformat.video.height));
		cv::imencode(_extension, resized, buffer, _params);
	}
	else		
		cv::imencode(_extension, source, buffer, _params);
	
	// Temp reference on the stack only
	mpacket->_data = (char*)&buffer[0];
	mpacket->_size = buffer.size();
	
	//traceL("ImageEncoder", this) << "Broadcasting: " << mpacket << endl;
	emit(*mpacket); //this, 
	//traceL("ImageEncoder", this) << "Broadcasting: OK: " << mpacket << endl;
}

	
EncoderOptions& ImageEncoder::options()
{
	return _options;
}


} } // namespace scy::av


#endif