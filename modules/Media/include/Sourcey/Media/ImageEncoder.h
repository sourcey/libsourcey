//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2002 Sourcey
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
	ImageEncoder(EncoderParams& params, std::vector<int> cvParams = std::vector<int>());
	virtual ~ImageEncoder();
	
	virtual void initialize();
	virtual void uninitialize();
	
	virtual bool accepts(IPacket& packet);
	virtual void process(IPacket& packet);
	
	virtual EncoderParams& params();

private:
	std::string _extension;
    EncoderParams _params;
    std::vector<int> _cvParams;
};


} } // namespace Sourcey::Media


#endif	// SOURCEY_MEDIA_ImageEncoder_H

