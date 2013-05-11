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


#ifndef SOURCEY_RTP_Codec_H
#define SOURCEY_RTP_Codec_H


#include "Sourcey/Media/Codec.h"

#include <iostream>
#include <string>
#include <list>


namespace Scy {
namespace RTP { 


struct Codec: public Media::Codec
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


} // namespace Scy
} // namespace RTP 


#endif