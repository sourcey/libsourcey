//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2005 Sourcey
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


namespace Sourcey {
namespace RTP { 


struct Codec: public Media::Codec
{
	/*
	int id;
	std::string name;
	int clockrate;
	int preference;

	Codec(int id, const std::string& name, int clockrate, int preference = 0) : 
		id(id), name(name), clockrate(clockrate), preference(preference) {}
	Codec(const Codec& r);
	Codec() : 
		id(0), clockrate(0), preference(0) {}

	bool matches(int id, const std::string& name) const;

	static bool preferable(const Codec& first, const Codec& second) {
		return first.preference > second.preference;
	}
	*/
	
	Codec(UInt32 id, const std::string& name, int sampleRate, int bitRate);
	Codec(const Media::Codec& r);
	Codec(const std::string& sdp);

	//virtual std::string toString() const;
	virtual std::string toSDP() const;
};


typedef std::list<Codec> CodecList;
typedef std::list<Codec*> CodecPtrList;


struct AudioCodec: public Codec 
{	
	AudioCodec(const Media::Codec& r);
	AudioCodec(const std::string& sdp);
	/*
	int bitrate;
	int channels;
	AudioCodec(int id, const std::string& name, int clockrate, int bitrate, int channels = 1, int preference = 0) : 
		Codec(id, name, clockrate, preference), bitrate(bitrate), channels(channels) {}
	AudioCodec(const AudioCodec& r);
	AudioCodec() : 
		Codec(0, "", 0, 0), bitrate(0), channels(0) {}

	virtual std::string toString() const;
	*/
};
	

struct VideoCodec: public Codec 
{
	VideoCodec(const Media::Codec& r);
	VideoCodec(const std::string& sdp);
	
	/*
	//int id;
	//std::string name;
	//int clockrate;
	int width;
	int height;
	int framerate;
	//int preference;
	*/
	
		/*
	// Creates a codec with the given parameters.
	VideoCodec(int id, const std::string& name, int clockrate, int width, int height, int framerate, int preference) : 
		Codec(id, name, clockrate, preference), width(width), height(height), framerate(framerate) {}
	

	VideoCodec(const VideoCodec& r);
	VideoCodec() : 
		Codec(0, "", 0, 0), width(width), height(height), framerate(framerate) {}

	virtual std::string toString() const;
		*/
};


} // namespace Sourcey
} // namespace RTP 


#endif