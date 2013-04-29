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


#include "Sourcey/Media/Format.h"
#include <sstream>
#include <algorithm>


using namespace std;


namespace Sourcey {
namespace Media {


Format::Format() : 
	name("Unknown"), id("rawvideo"), priority(0)
{
}


Format::Format(const string& name, const char* id, const VideoCodec& video, const AudioCodec& audio, int priority) : 
	name(name), id(id), video(video), audio(audio), priority(priority)
{
}


Format::Format(const string& name, const char* id, const VideoCodec& video, int priority) : 
	name(name), id(id), video(video), priority(priority)
{
}


Format::Format(const string& name, const char* id, const AudioCodec& audio, int priority) : 
	name(name), id(id), audio(audio), priority(priority)
{
}

	
Format::Format(const Format& r) :
	name(r.name),
	id(r.id),
	video(r.video),
	audio(r.audio),
	priority(r.priority) 
{
}

	
Format::Type Format::type() const
{
	if (video.enabled && audio.enabled)
		return Format::Multiplex;
	else if (video.enabled)
		return Format::Video;
	else if (audio.enabled)
		return Format::Audio;
	else
		return Format::None;
}


string Format::toString() const 
{
	ostringstream os;
	os << "Format[" << name << ":"  << id;
	if (video.enabled)
		os << "\n\t" << video.toString();
	if (audio.enabled)
		os << "\n\t" << audio.toString();
	os << "]";
	return os.str();
}


void Format::print(std::ostream& ost)
{
	ost << "Format[" << name << ":"  << id;
	if (video.enabled) {
		ost << "\n";
		video.print(ost);
		//ost << "\n";
	}
	if (audio.enabled) {
		ost << "\n";
		audio.print(ost);
		//ost << "\n";
	}
	ost << "\n]";
}


} // namespace Media 
} // namespace Sourcey





/*
string Format::name() const
{
	return Format::idToName(id);
}


string Format::extension() const
{
	string extension(this->id);
	std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
	return extension;
}
*/


/*
string Format::encoderName() const
{
	return Format::idToEncoderName(id);
}
*/
/*
Format::ID Format::nameToID(const std::string& name) 
{
	if (name == "Raw")
		return "rawvideo";

	// Video Formats
	if (name == "MP4")
		return "mp4";
	if (name == "FLV")
		return "flv";
	if (name == "OGG")
		return Format::OGG;
	if (name == "AVI")
		return Format::AVI;
	if (name == "MJPEG")
		return "mjpeg";
	if (name == "MPNG")
		return Format::MPNG;

	// Audio Formats
	if (name == "M4A")
		return Format::M4A;
	if (name == "MP3")
		return "mp3";

	return Format::Unknown;
}


std::string Format::idToName(const char* id) 
{
	switch(id)
	{
		case Format::Unknown:	return "Unknown";
		case "rawvideo":		return "Raw";
			  
		// Video Formats
		case "mp4":		return "MP4";
		case "flv":		return "FLV";
		case Format::OGG:		return "OGG";
		case Format::AVI:		return "AVI";
		case "mjpeg":		return "MJPEG";
		case Format::MPNG:		return "MPNG";

		// Audio Formats
		case Format::M4A:		return "M4A";
		case "mp3":		return "MP3";
	}
	return "Unknown";
}


std::string Format::idToEncoderName(const char* id) 
{
	switch(id)
	{
		case Format::Unknown:	return "";
		case "rawvideo":		return "";
			  
		// Video Formats
		case "mp4":		return "mp4";
		case "flv":		return "flv";
		case Format::OGG:		return "ogg";
		case Format::AVI:		return "avi";
		case "mjpeg":		return "mjpeg";
		case Format::MPNG:		return "mpng";

		// Audio Formats
		case Format::M4A:		return "m4a";
		case "mp3":		return "mp3";
	}
	return "";
}
		<< "\n\tName: " << name()
*/

