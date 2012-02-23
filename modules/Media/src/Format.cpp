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


#include "Sourcey/Media/Format.h"
#include <sstream>


using namespace std;


namespace Sourcey {
namespace Media {


Format::Format() : 
	label("Unknown"), id(Unknown), type(None), priority(0)
{
}


Format::Format(const string& label, UInt32 id, const VideoCodec& video, const AudioCodec& audio, int priority) : 
	label(label), id(id), type(Multiplex), video(video), audio(audio), priority(priority)
{
}


Format::Format(const string& label, UInt32 id, const VideoCodec& video, int priority) : 
	label(label), id(id), type(Video), video(video), priority(priority)
{
}


Format::Format(const string& label, UInt32 id, const AudioCodec& audio, int priority) : 
	label(label), id(id), type(Audio), audio(audio), priority(priority)
{
}

	
Format::Format(const Format& r) :
	label(r.label),
	id(r.id),
	type(r.type),
	video(r.video),
	audio(r.audio),
	priority(r.priority) 
{
}

	
Format::ID Format::toID(const string& name) 
{
	if (name == "Raw")
		return Format::Raw;

	// Video Formats
	if (name == "MP4")
		return Format::MP4;
	if (name == "FLV")
		return Format::FLV;
	if (name == "OGG")
		return Format::OGG;
	if (name == "AVI")
		return Format::AVI;
	if (name == "MJPEG")
		return Format::MJPEG;
	if (name == "MPNG")
		return Format::MPNG;

	// Audio Formats
	if (name == "M4A")
		return Format::M4A;
	if (name == "MP3")
		return Format::MP3;

	return Format::Unknown;
}


string Format::idString(UInt32 id) 
{
	switch(id)
	{
		case Unknown:	return "Unknown";
		case Raw:		return "Raw";
			  
		// Video Formats
		case MP4:		return "MP4";
		case FLV:		return "FLV";
		case OGG:		return "OGG";
		case AVI:		return "AVI";
		case MJPEG:		return "MJPEG";
		case MPNG:		return "MPNG";

		// Audio Formats
		case M4A:		return "M4A";
		case MP3:		return "MP3";
	}
	return "Unknown";
}


string Format::name() const
{
	return Format::idString(id);
}


string Format::extension() const
{
	string extension = "." + Format::idString(id);
	std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
	return extension;
}


string Format::toString() const 
{
	ostringstream os;
	os << "Format[" << label << ":"  << id << ":" << name() << "]";
	return os.str();
}


void Format::print(std::ostream& ost)
{
	ost << "Format["
		<< "\n\tLabel: " << label
		<< "\n\tID: " << id
		<< "\n\tName: " << name()
		<< "\n\tType: " << type;
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