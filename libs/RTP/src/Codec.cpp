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


#include "Sourcey/RTP/Codec.h"
#include "Sourcey/Util.h"
#include <sstream>


using namespace std;


namespace Sourcey {
namespace RTP {


// ---------------------------------------------------------------------
//
Codec::Codec(UInt32 id, const string& name, int sampleRate, int bitRate) : //const string& name, 
	Media::Codec(id, name, sampleRate, bitRate, true)
{
}

	
Codec::Codec(const Media::Codec& r) :
	Media::Codec(r.id, r.name, r.sampleRate, r.bitRate, r.enabled)
{
}


Codec::Codec(const std::string& sdp) //: 
	//id(0), clockrate(0), preference(0) 
{

	string value = sdp;
	string::size_type start = 0;
	string::size_type end = 0;

	end = value.find("a=rtpmap:");
	if (end != string::npos) {
		start = end+9;
	}
		
	// codec id
	end = value.find(" ");
	if (end != string::npos) {
		id = Util::atoi(value.substr(start, end).c_str());
	}

	// codec name
	start = end+1;
	end = value.find("/");
	if (end != string::npos) {
		name = value.substr(start, end-start);
	}

	// clockrate
	start = end+1;
	end = value.length(); 
	sampleRate = Util::atoi(value.substr(start, end-start).c_str());
}


	/*
	id(r.id),
	name(r.name),
	sampleRate(r.sampleRate),
	bitRate(r.bitRate),
	enabled(r.enabled)
	Codec(r.id, r.name, r.sampleRate, r.bitRate, r.enabled)//,
bool Codec::matches(int id, const string& name) const 
{
	return this->id == id && this->name == name;
}


string Codec::toString() const 
{
	ostringstream os;
	os << "Codec[" << id << ":" << name << ":" << clockrate << ":" << preference << "]";
	return os.str();
}
*/


string Codec::toSDP() const 
{
	ostringstream os;
	os << "a=rtpmap:" 
		<< id << " " 
		<< name << "/" 
		<< sampleRate 
		<< "\r\n";
	return os.str();
}


// ---------------------------------------------------------------------
//
AudioCodec::AudioCodec(const Media::Codec& r) :
	Codec(r)
{
}


AudioCodec::AudioCodec(const std::string& sdp) :
	Codec(sdp)
{
}


// ---------------------------------------------------------------------
//
VideoCodec::VideoCodec(const Media::Codec& r) :
	Codec(r)
{
}


VideoCodec::VideoCodec(const std::string& sdp) :
	Codec(sdp)
{
}

	
/*


string AudioCodec::toString() const 
{
	ostringstream os;
	os << "AudioCodec[" << id << ":" << name << ":" << clockrate << ":" << bitrate
		<< ":" << channels << ":" << preference << "]";
	return os.str();
}
string VideoCodec::toString() const 
{
	ostringstream os;
	os << "VideoCodec[" << id << ":" << name << ":" << width << ":" << height
		<< ":" << framerate << ":" << preference << "]";
	return os.str();
}

*/


} // namespace RTP 
} // namespace Sourcey