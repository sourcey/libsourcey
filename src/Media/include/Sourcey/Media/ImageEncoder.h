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


#ifndef SOURCEY_MEDIA_ImageEncoder_H
#define SOURCEY_MEDIA_ImageEncoder_H


#include "Sourcey/Media/Types.h"
#include "Sourcey/Media/IEncoder.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>


namespace Sourcey {
namespace Media {


class ImageEncoder: public IPacketEncoder
	// This calss is an image stream encoder that uses OpenCV conversion
	// methods to encode images.
	//
	// See OpenCV documentation for cvParams information.
	// JPEG:
	//	0 = CV_IMWRITE_JPEG_QUALITY
	//	1 = (quality) default 95 [0-100]
	//
	// PNG:
	//	0 = CV_IMWRITE_PNG_COMPRESSION
	//	1 = (compression) default 3 [0-9]
	//
{
public:		
	ImageEncoder(EncoderOptions& options, std::vector<int> cvParams = std::vector<int>());
	virtual ~ImageEncoder();
	
	virtual void initialize();
	virtual void uninitialize();
	
	virtual bool accepts(IPacket& packet);
	virtual void process(IPacket& packet);
	
	virtual EncoderOptions& options();

private:
	std::string _extension;
    EncoderOptions _options;
    std::vector<int> _params;
};


} } // namespace Sourcey::Media


#endif	// SOURCEY_MEDIA_ImageEncoder_H

