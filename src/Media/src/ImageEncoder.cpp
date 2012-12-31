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
#include "Sourcey/Media/VideoCapture.h"
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

	MatPacket* mpacket = reinterpret_cast<MatPacket*>(&packet);	
	if (!mpacket->mat)
		throw Exception("Video packets must contain an OpenCV image.");
	
    vector<unsigned char> buffer;
	
	// FIXME: If the video capture is stopped before
	// this callback completes our Mat is corrupted.
	cv::Mat& source = *mpacket->mat;
	Log("trace") << "[ImageEncoder:" << this <<"] Broadcasting: " << mpacket->mat << endl;
	Log("trace") << "[ImageEncoder:" << this <<"] Broadcasting: " << source.cols << endl;
	Log("trace") << "[ImageEncoder:" << this <<"] Broadcasting: " << source.rows << endl;
	Log("trace") << "[ImageEncoder:" << this <<"] Broadcasting: " << _options.oformat.video.width << endl;
	Log("trace") << "[ImageEncoder:" << this <<"] Broadcasting: " << _options.oformat.video.height << endl;
	if (source.cols != _options.oformat.video.width &&
		source.rows != _options.oformat.video.height) {
		cv::Mat resized;
		cv::resize(source, resized, cv::Size(_options.oformat.video.width, _options.oformat.video.height));
		cv::imencode(_extension, resized, buffer, _params);
	}
	else		
		cv::imencode(_extension, source, buffer, _params);
	
	mpacket->setData(&buffer[0]);
	mpacket->setSize(buffer.size());
	
	//Log("trace") << "[ImageEncoder:" << this <<"] Broadcasting: " << mpacket << endl;
	dispatch(this, *mpacket);
	//Log("trace") << "[ImageEncoder:" << this <<"] Broadcasting: OK: " << mpacket << endl;
}

	
EncoderOptions& ImageEncoder::options()
{
	return _options;
}


} } // namespace Sourcey::Media