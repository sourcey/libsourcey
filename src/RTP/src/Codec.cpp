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


#include "Sourcey/RTP/Codec.h"
#include "Sourcey/Util.h"
#include <sstream>


using namespace std;


namespace Scy {
namespace RTP {


// ---------------------------------------------------------------------
Codec::Codec(int id, const string& name, int sampleRate, int bitRate) : //const string& name, 
	Media::Codec(name, sampleRate, bitRate, true), id(id)
{
}

	
Codec::Codec(const Codec& r) :
	Media::Codec(r.name, r.sampleRate, r.bitRate, r.enabled), id(r.id)
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
		id = Util::fromString<UInt32>(value.substr(start, end).c_str());
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
	sampleRate = Util::fromString<UInt32>(value.substr(start, end-start).c_str());
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
AudioCodec::AudioCodec(const Codec& r) :
	Codec(r)
{
}


AudioCodec::AudioCodec(const std::string& sdp) :
	Codec(sdp)
{
}


// ---------------------------------------------------------------------
VideoCodec::VideoCodec(const Codec& r) :
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
} // namespace Scy