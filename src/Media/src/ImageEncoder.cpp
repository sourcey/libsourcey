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


#include "Sourcey/Media/ImageEncoder.h"
#include "Sourcey/Media/VideoCapture.h"
#include "Sourcey/Logger.h"

#include "assert.h"


using namespace std;
using namespace Poco;


namespace scy {
namespace av {


ImageEncoder::ImageEncoder(EncoderOptions& options, vector<int> cvParams) : 
	_options(options),
	_params(cvParams)
{	
	debugL() << "[ImageEncoder" << this << "] Creating" << endl;

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
	debugL() << "[ImageEncoder" << this << "] Destroy" << endl;
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
	//traceL() << "[ImageEncoder:" << this <<"] Processing" << endl;

	MatPacket* mpacket = reinterpret_cast<MatPacket*>(&packet);	
	if (!mpacket->mat)
		throw Exception("Video packets must contain an OpenCV image.");
	
    vector<unsigned char> buffer;
	
	// FIXME: If the video capture is stopped before
	// this callback completes our Mat is corrupted.
	cv::Mat& source = *mpacket->mat;
	traceL() << "[ImageEncoder:" << this <<"] Broadcasting: " << mpacket->mat << endl;
	traceL() << "[ImageEncoder:" << this <<"] Broadcasting: " << source.cols << endl;
	traceL() << "[ImageEncoder:" << this <<"] Broadcasting: " << source.rows << endl;
	traceL() << "[ImageEncoder:" << this <<"] Broadcasting: " << _options.oformat.video.width << endl;
	traceL() << "[ImageEncoder:" << this <<"] Broadcasting: " << _options.oformat.video.height << endl;
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
	
	//traceL() << "[ImageEncoder:" << this <<"] Broadcasting: " << mpacket << endl;
	emit(this, *mpacket);
	//traceL() << "[ImageEncoder:" << this <<"] Broadcasting: OK: " << mpacket << endl;
}

	
EncoderOptions& ImageEncoder::options()
{
	return _options;
}


} } // namespace scy::av