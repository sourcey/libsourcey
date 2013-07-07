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


#ifndef SOURCEY_RTP_Codec_H
#define SOURCEY_RTP_Codec_H


#include "Sourcey/Media/Codec.h"

#include <iostream>
#include <string>
#include <list>


namespace scy {
namespace rtp { 


struct Codec: public av::Codec
{
	int id;
	
	Codec(int id, const std::string& name, int sampleRate, int bitRate);
	Codec(const Codec& r);
	Codec(const std::string& sdp);

	//virtual std::string toString() const;
	virtual std::string toSDP() const;
};


typedef std::list<Codec> CodecList;
typedef std::list<Codec*> CodecPtrList;


struct AudioCodec: public Codec 
{	
	AudioCodec(const Codec& r);
	AudioCodec(const std::string& sdp);
};
	

struct VideoCodec: public Codec 
{
	VideoCodec(const Codec& r);
	VideoCodec(const std::string& sdp);
};


} // namespace scy
} // namespace rtp 


#endif