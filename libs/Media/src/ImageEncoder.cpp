//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is is distributed under a dual license that allows free, 
// open source use and closed source use under a standard commercial
// license.
//
// Non-Commercial Use:
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// Commercial Use:
// Please contact mail@sourcey.com
//


#include "Sourcey/Media/ImageEncoder.h"
#include "Sourcey/Logger.h"

#include "assert.h"


using namespace std;
using namespace Poco;
using namespace Sourcey;


namespace Sourcey {
namespace Media {


ImageEncoder::ImageEncoder(EncoderOptions& options, vector<int> cvParams) : 
	_options(options),
	_params(cvParams)
{	
	Log("debug") << "[ImageEncoder" << this << "] Creating" << endl;

	if (_options.oformat.id == Format::MJPEG)
		_extension = ".jpg";
	else if (_options.oformat.id == Format::MPNG)
		_extension = ".png";
	else 
		// TODO: support more!
		assert(false);
}


ImageEncoder::~ImageEncoder()
{
	Log("debug") << "[ImageEncoder" << this << "] Destroy" << endl;
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
	//Log("trace") << "[ImageEncoder:" << this <<"] Processing" << endl;

	Media::VideoPacket* vpacket = reinterpret_cast<Media::VideoPacket*>(&packet);
	//if (!vpacket) {
	//	dispatch(this, packet);
	//	return;
	//}
	
	if (!vpacket->mat)
		throw Exception("Video packets must contain an OpenCV image.");
	
    vector<unsigned char> buffer;

	// FIXME: If the video capture is stopped before
	// this callback completes our Mat is corrupted.
	cv::Mat& source = *vpacket->mat;
	if (source.cols != _options.oformat.video.width &&
		source.rows != _options.oformat.video.height) {
		cv::Mat resized;
		cv::resize(source, resized, cv::Size(_options.oformat.video.width, _options.oformat.video.height));
		cv::imencode(_extension, resized, buffer, _params);
	}
	else		
		cv::imencode(_extension, source, buffer, _params);
	
	vpacket->setData(&buffer[0]);
	vpacket->setSize(buffer.size());
	
	//Log("trace") << "[ImageEncoder:" << this <<"] Broadcasting: " << vpacket << endl;
	dispatch(this, *vpacket);
	//Log("trace") << "[ImageEncoder:" << this <<"] Broadcasting: OK: " << vpacket << endl;
}

	
EncoderOptions& ImageEncoder::options()
{
	return _options;
}


} } // namespace Sourcey::Media